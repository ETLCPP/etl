/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "unit_test_framework.h"

#include <thread>
#include <vector>
#include <numeric>
#include <array>
#include <algorithm>
#include <queue>
#include <atomic>

#include "etl/atomic.h"
#include "etl/queue_spsc_atomic.h"
#include "etl/buffer_descriptors.h"

#if defined(ETL_TARGET_OS_WINDOWS)
  #include <Windows.h>
#endif

#define REALTIME_TEST 0

namespace
{
  constexpr size_t BUFFER_SIZE = 16UL;
  constexpr size_t N_BUFFERS   = 4UL;
  constexpr size_t DATA_COUNT  = BUFFER_SIZE / 2UL;

  using BD = etl::buffer_descriptors<char, BUFFER_SIZE, N_BUFFERS, std::atomic_char>;

  char buffers[N_BUFFERS][BUFFER_SIZE];

  //***********************************
  struct Receiver
  {
    void receive(BD::notification n)
    {
      pbuffer = n.get_descriptor().data();
      count   = n.get_count();
    }

    void clear()
    {
      pbuffer = nullptr;
      count   = 0U;
    }

    BD::pointer   pbuffer;
    BD::size_type count;
  };

  Receiver receiver;

  SUITE(test_buffer_descriptors)
  {
    //*************************************************************************
    TEST(test_constructor_plus_buffer)
    {
      BD bd(&buffers[0][0]);

      CHECK_EQUAL(N_BUFFERS, bd.N_BUFFERS);
      CHECK_EQUAL(BUFFER_SIZE, bd.BUFFER_SIZE);
      CHECK(!bd.is_valid());
    }

    //*************************************************************************
    TEST(test_constructor_plus_buffer_and_callback)
    {
      receiver.clear();
      BD::callback_type callback = BD::callback_type::create<Receiver, &Receiver::receive>(receiver);

      BD bd(&buffers[0][0], callback);

      CHECK_EQUAL(N_BUFFERS, bd.N_BUFFERS);
      CHECK_EQUAL(BUFFER_SIZE, bd.BUFFER_SIZE);
      CHECK(bd.is_valid());
    }

    //*************************************************************************
    TEST(test_constructor_plus_buffer_set_callback)
    {
      receiver.clear();
      BD::callback_type callback = BD::callback_type::create<Receiver, &Receiver::receive>(receiver);

      BD bd(&buffers[0][0]);
      bd.set_callback(callback);

      CHECK_EQUAL(N_BUFFERS, bd.N_BUFFERS);
      CHECK_EQUAL(BUFFER_SIZE, bd.BUFFER_SIZE);
      CHECK(bd.is_valid());
    }

    //*************************************************************************
    TEST(test_buffers)
    {
      BD bd(&buffers[0][0]);

      for (size_t i = 0UL; i < N_BUFFERS; ++i)
      {
        BD::descriptor desc = bd.allocate();

        CHECK(desc.is_valid());
        CHECK(desc.is_allocated());
        CHECK(!desc.is_released());
        CHECK_EQUAL(BUFFER_SIZE, desc.max_size());
        CHECK_EQUAL(uintptr_t(&buffers[i][0]), uintptr_t(desc.data()));
      }
    }

    //*************************************************************************
    TEST(test_clear)
    {
      BD bd(&buffers[0][0]);
      BD::descriptor desc[4];

      for (size_t i = 0UL; i < N_BUFFERS; ++i)
      {
        desc[i] = bd.allocate();
      }

      bd.clear();

      for (size_t i = 0UL; i < N_BUFFERS; ++i)
      {
        CHECK(desc[i].is_valid());
        CHECK(!desc[i].is_allocated());
        CHECK(desc[i].is_released());
      }
    }

    //*************************************************************************
    TEST(test_buffers_with_allocate_fill)
    {
      std::array<char, BUFFER_SIZE> test =
      {
        char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU),
        char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU), char(0xFFU)
      };

      BD bd(&buffers[0][0]);

      for (size_t i = 0UL; i < N_BUFFERS; ++i)
      {
        BD::descriptor desc = bd.allocate(char(0xFFU));

        CHECK_EQUAL(BUFFER_SIZE, desc.max_size());
        CHECK_EQUAL(uintptr_t(&buffers[i][0]), uintptr_t(desc.data()));
        CHECK_ARRAY_EQUAL(test.data(), desc.data(), BUFFER_SIZE);
      }
    }

    //*************************************************************************
    TEST(test_notifications)
    {
      std::array<char, BUFFER_SIZE> test = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0 };

      std::fill(&buffers[0][0], &buffers[N_BUFFERS - 1][0] + BUFFER_SIZE , 0U);

      receiver.clear();
      BD::callback_type callback = BD::callback_type::create<Receiver, &Receiver::receive>(receiver);

      BD bd(&buffers[0][0], callback);

      for (size_t i = 0UL; i < N_BUFFERS; ++i)
      {
        BD::descriptor desc = bd.allocate();

        CHECK(desc.is_valid());

        std::copy(test.begin(), test.begin() + DATA_COUNT, desc.data());
        bd.notify(BD::notification(desc, DATA_COUNT));
        desc.release();

        CHECK_EQUAL(DATA_COUNT, receiver.count);
        CHECK_EQUAL(uintptr_t(&buffers[i][0]), uintptr_t(receiver.pbuffer));
        CHECK_ARRAY_EQUAL(test.data(), desc.data(), BUFFER_SIZE);
      }
    }

    //*************************************************************************
    TEST(test_allocate_overflow)
    {
      BD bd(&buffers[0][0]);

      // Use up all of the descriptors.
      for (size_t i = 0UL; i < N_BUFFERS; ++i)
      {
        BD::descriptor desc = bd.allocate();
        CHECK(desc.is_valid());
      }

      BD::descriptor desc = bd.allocate();
      CHECK(!desc.is_valid());
    }

    //*************************************************************************
    TEST(test_allocate_release_rollover)
    {
      std::queue<BD::descriptor> desc_queue;

      BD bd(&buffers[0][0]);

      // Use up all of the descriptors, then release/allocate for the rest.
      for (size_t i = 0UL; i < (N_BUFFERS * 2); ++i)
      {
        BD::descriptor desc = bd.allocate();
        desc_queue.push(desc);

        CHECK(desc.is_valid());

        if (i >= (N_BUFFERS - 1))
        {
          desc_queue.front().release();
          desc_queue.pop();
        }
      }
    }

    //*************************************************************************
    TEST(test_descriptors)
    {
      BD bd(&buffers[0][0]);

      BD::descriptor desc1 = bd.allocate();
      BD::descriptor desc2 = bd.allocate();
      BD::descriptor desc3 = bd.allocate();
      BD::descriptor desc4 = bd.allocate();

      CHECK(desc1.is_allocated());
      CHECK(desc2.is_allocated());
      CHECK(desc3.is_allocated());
      CHECK(desc4.is_allocated());

      CHECK(desc1.data() == &buffers[0][0]);
      CHECK(desc2.data() == &buffers[1][0]);
      CHECK(desc3.data() == &buffers[2][0]);
      CHECK(desc4.data() == &buffers[3][0]);

      CHECK(desc1.max_size() == BUFFER_SIZE);
      CHECK(desc2.max_size() == BUFFER_SIZE);
      CHECK(desc3.max_size() == BUFFER_SIZE);
      CHECK(desc4.max_size() == BUFFER_SIZE);

      CHECK(desc1.MAX_SIZE == BUFFER_SIZE);
      CHECK(desc2.MAX_SIZE == BUFFER_SIZE);
      CHECK(desc3.MAX_SIZE == BUFFER_SIZE);
      CHECK(desc4.MAX_SIZE == BUFFER_SIZE);

      CHECK(desc1.is_valid());
      CHECK(desc2.is_valid());
      CHECK(desc3.is_valid());
      CHECK(desc4.is_valid());

      desc1.release();
      desc2.release();
      desc3.release();
      desc4.release();

      CHECK(desc1.is_released());
      CHECK(desc2.is_released());
      CHECK(desc3.is_released());
      CHECK(desc4.is_released());
    }

    //*************************************************************************
#if REALTIME_TEST

#if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently supported
#define RAISE_THREAD_PRIORITY  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)
#define FIX_PROCESSOR_AFFINITY1 SetThreadAffinityMask(GetCurrentThread(), 1)
#define FIX_PROCESSOR_AFFINITY2 SetThreadAffinityMask(GetCurrentThread(), 2)
#else
#define RAISE_THREAD_PRIORITY
#define FIX_PROCESSOR_AFFINITY1
#define FIX_PROCESSOR_AFFINITY2
#endif

    std::atomic_bool start = false;

    //*********************************
    struct Notification
    {
      BD::descriptor    desc;
      BD::size_type count;
    };

    constexpr int N_ITERATIONS = 1000000;

    etl::queue_spsc_atomic<BD::notification, N_ITERATIONS + 100> desc_queue;

    //*********************************
    void Callback(BD::notification n)
    {
      desc_queue.push(n);
    }

    //*********************************
    void Producer()
    {
      static char buffers[N_BUFFERS][BUFFER_SIZE];

      BD bd(&buffers[0][0], BD::callback_type::create<Callback>());

      RAISE_THREAD_PRIORITY;
      FIX_PROCESSOR_AFFINITY1;

      // Wait for the start flag.
      while (!start);

      int errors = 0;

      for (int i = 0; i < N_ITERATIONS; ++i)
      {
        BD::descriptor desc;

        // Wait until we can allocate a descriptor.
        do
        {
          desc = bd.allocate();
        } while (desc.is_valid() == false);

        if (!desc.is_allocated())
        {
          ++errors;
        }

        // Send a notification to the callback function.
        bd.notify(BD::notification(desc, BUFFER_SIZE));
      }

      CHECK_EQUAL(0, errors);
    }

    //*********************************
    void Consumer()
    {
      RAISE_THREAD_PRIORITY;
      FIX_PROCESSOR_AFFINITY2;

      // Wait for the start flag.
      while (!start);

      int errors = 0;

      for (int i = 0; i < N_ITERATIONS;)
      {
        BD::notification notification;

        // Try to get a notification from the queue.
        if (desc_queue.pop(notification))
        {
          CHECK_EQUAL(BUFFER_SIZE, notification.get_count());
          CHECK(notification.get_descriptor().is_allocated());

          if (!notification.get_descriptor().is_allocated())
          {
            ++errors;
          }

          notification.get_descriptor().release();
          ++i;
        }

        CHECK_EQUAL(0, errors);
      }
    }

    //*********************************
    TEST(test_multi_thread)
    {
      std::thread t1(Producer);
      std::thread t2(Consumer);

      start = true;

      t1.join();
      t2.join();
    }
#endif
  };
}
