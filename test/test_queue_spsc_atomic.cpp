/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#include "UnitTest++.h"

#include <thread>
#include <chrono>
#include <vector>

#include "etl/queue_spsc_atomic.h"

#if defined(ETL_COMPILER_MICROSOFT)
  #include <Windows.h>
#endif

#define REALTIME_TEST 0

namespace
{
  struct Data
  {
    Data(int a_, int b_ = 2, int c_ = 3, int d_ = 4)
      : a(a_),
      b(b_),
      c(c_),
      d(d_)
    {
    }

    Data()
      : a(0),
      b(0),
      c(0),
      d(0)
    {
    }

    int a;
    int b;
    int c;
    int d;
  };

  bool operator ==(const Data& lhs, const Data& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b) && (lhs.c == rhs.c) && (lhs.d == rhs.d);
  }

  SUITE(test_queue_atomic)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      etl::queue_spsc_atomic<int, 4> queue;

      CHECK_EQUAL(4U, queue.max_size());
      CHECK_EQUAL(4U, queue.capacity());
    }

    //*************************************************************************
    TEST(test_size_push_pop)
    {
      etl::queue_spsc_atomic<int, 4> queue;

      CHECK_EQUAL(0U, queue.size());

      CHECK_EQUAL(4U, queue.available());
      CHECK_EQUAL(0U, queue.size());

      queue.push(1);
      CHECK_EQUAL(1U, queue.size());
      CHECK_EQUAL(3U, queue.available());

      queue.push(2);
      CHECK_EQUAL(2U, queue.size());
      CHECK_EQUAL(2U, queue.available());

      queue.push(3);
      CHECK_EQUAL(3U, queue.size());
      CHECK_EQUAL(1U, queue.available());

      queue.push(4);
      CHECK_EQUAL(4U, queue.size());
      CHECK_EQUAL(0U, queue.available());

      CHECK(!queue.push(5));
      CHECK(!queue.push(5));

      int i;

      CHECK(queue.pop(i));
      CHECK_EQUAL(1, i);
      CHECK_EQUAL(3U, queue.size());

      CHECK(queue.pop(i));
      CHECK_EQUAL(2, i);
      CHECK_EQUAL(2U, queue.size());

      CHECK(queue.pop(i));
      CHECK_EQUAL(3, i);
      CHECK_EQUAL(1U, queue.size());

      CHECK(queue.pop(i));
      CHECK_EQUAL(4, i);
      CHECK_EQUAL(0U, queue.size());

      CHECK(!queue.pop(i));
      CHECK(!queue.pop(i));
    }

    //*************************************************************************
    TEST(test_size_push_pop_iqueue)
    {
      etl::queue_spsc_atomic<int, 4> queue;

      etl::iqueue_spsc_atomic<int>& iqueue = queue;

      CHECK_EQUAL(0U, iqueue.size());

      iqueue.push(1);
      CHECK_EQUAL(1U, iqueue.size());

      iqueue.push(2);
      CHECK_EQUAL(2U, iqueue.size());

      iqueue.push(3);
      CHECK_EQUAL(3U, iqueue.size());

      iqueue.push(4);
      CHECK_EQUAL(4U, iqueue.size());

      CHECK(!iqueue.push(5));
      CHECK(!iqueue.push(5));

      int i;

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(1, i);
      CHECK_EQUAL(3U, iqueue.size());

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(2, i);
      CHECK_EQUAL(2U, iqueue.size());

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(3, i);
      CHECK_EQUAL(1U, iqueue.size());

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(4, i);
      CHECK_EQUAL(0U, iqueue.size());

      CHECK(!iqueue.pop(i));
      CHECK(!iqueue.pop(i));
    }

    //*************************************************************************
    TEST(test_size_push_pop_void)
    {
      etl::queue_spsc_atomic<int, 4> queue;

      CHECK_EQUAL(0U, queue.size());

      queue.push(1);
      CHECK_EQUAL(1U, queue.size());

      queue.push(2);
      CHECK_EQUAL(2U, queue.size());

      queue.push(3);
      CHECK_EQUAL(3U, queue.size());

      queue.push(4);
      CHECK_EQUAL(4U, queue.size());

      CHECK(!queue.push(5));
      CHECK(!queue.push(5));

      CHECK(queue.pop());
      CHECK_EQUAL(3U, queue.size());

      CHECK(queue.pop());
      CHECK_EQUAL(2U, queue.size());

      CHECK(queue.pop());
      CHECK_EQUAL(1U, queue.size());

      CHECK(queue.pop());
      CHECK_EQUAL(0U, queue.size());

      CHECK(!queue.pop());
      CHECK(!queue.pop());
    }

    //*************************************************************************
    TEST(test_multiple_emplace)
    {
      etl::queue_spsc_atomic<Data, 4> queue;

      queue.emplace(1);
      queue.emplace(1, 2);
      queue.emplace(1, 2, 3);
      queue.emplace(1, 2, 3, 4);

      CHECK_EQUAL(4U, queue.size());

      Data popped;

      queue.pop(popped);
      CHECK(popped == Data(1, 2, 3, 4));
      queue.pop(popped);
      CHECK(popped == Data(1, 2, 3, 4));
      queue.pop(popped);
      CHECK(popped == Data(1, 2, 3, 4));
      queue.pop(popped);
      CHECK(popped == Data(1, 2, 3, 4));
    }

    //*************************************************************************
    TEST(test_clear)
    {
      etl::queue_spsc_atomic<int, 4> queue;

      CHECK_EQUAL(0U, queue.size());

      queue.push(1);
      queue.push(2);
      queue.clear();
      CHECK_EQUAL(0U, queue.size());

      // Do it again to check that clear() didn't screw up the internals.
      queue.push(1);
      queue.push(2);
      CHECK_EQUAL(2U, queue.size());
      queue.clear();
      CHECK_EQUAL(0U, queue.size());
    }

    //*************************************************************************
    TEST(test_empty)
    {
      etl::queue_spsc_atomic<int, 4> queue;
      CHECK(queue.empty());

      queue.push(1);
      CHECK(!queue.empty());

      queue.clear();
      CHECK(queue.empty());

      queue.push(1);
      CHECK(!queue.empty());
    }

    //*************************************************************************
    TEST(test_full)
    {
      etl::queue_spsc_atomic<int, 4> queue;
      CHECK(!queue.full());

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);
      CHECK(queue.full());

      queue.clear();
      CHECK(!queue.full());

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);
      CHECK(queue.full());
    }

    //=========================================================================
#if REALTIME_TEST && defined(ETL_COMPILER_MICROSOFT)
    #if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently supported
      #define FIX_PROCESSOR_AFFINITY1 SetThreadAffinityMask(GetCurrentThread(), 1);
      #define FIX_PROCESSOR_AFFINITY2 SetThreadAffinityMask(GetCurrentThread(), 2);
    #else
      #error No thread priority modifier defined
    #endif

    size_t ticks = 0;

    etl::queue_spsc_atomic<int, 10> queue;

    const size_t LENGTH = 1000000;

    void timer_event()
    {
      FIX_PROCESSOR_AFFINITY1;

      const size_t TICK = 1;
      size_t tick = TICK;
      ticks = 1;

      while (ticks <= LENGTH)
      {
        if (queue.push(ticks))
        {
          ++ticks;
        }
      }
    }

    TEST(queue_threads)
    {
      FIX_PROCESSOR_AFFINITY2;

      std::vector<int> tick_list;
      tick_list.reserve(LENGTH);

      std::thread t1(timer_event);

      while (tick_list.size() < LENGTH)
      {
        int i;

        if (queue.pop(i))
        {
          tick_list.push_back(i);
        }
      }

      // Join the thread with the main thread
      t1.join();

      CHECK_EQUAL(LENGTH, tick_list.size());

      for (size_t i = 0; i < LENGTH; ++i)
      {
        CHECK_EQUAL(i + 1, tick_list[i]);
      }
    }
#endif
  };
}
