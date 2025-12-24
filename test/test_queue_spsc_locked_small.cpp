/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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

#include "etl/queue_spsc_locked.h"

#include <thread>
#include <mutex>
#include <vector>

#if defined(ETL_TARGET_OS_WINDOWS)
#include <Windows.h>
#endif

#include "data.h"

#define REALTIME_TEST 0

namespace
{
  class Access
  {
  public:

    void clear()
    {
      called_lock = false;
      called_unlock = false;
    }

    void lock()
    {
      called_lock = true;
    }

    void unlock()
    {
      called_unlock = true;
    }

    bool called_lock;
    bool called_unlock;
  };

  Access access;

  etl::function_imv<Access, access, &Access::lock>   lock;
  etl::function_imv<Access, access, &Access::unlock> unlock;

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

  typedef etl::queue_spsc_locked<int, 4, etl::memory_model::MEMORY_MODEL_SMALL> QueueInt;
  typedef etl::iqueue_spsc_locked<int, etl::memory_model::MEMORY_MODEL_SMALL>   IQueueInt;

  typedef etl::queue_spsc_locked<int, 255, etl::memory_model::MEMORY_MODEL_SMALL> QueueInt255;

  using ItemM = TestDataM<int>;

  SUITE(test_queue_locked)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      access.clear();

      QueueInt queue(lock, unlock);

      CHECK_EQUAL(4U, queue.max_size());
      CHECK_EQUAL(4U, queue.capacity());

      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
    }

    //*************************************************************************
    TEST(test_size_push_pop)
    {
      access.clear();

      QueueInt queue(lock, unlock);

      CHECK_EQUAL(0U, queue.size_from_unlocked());

      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);

      access.clear();

      CHECK_EQUAL(4U, queue.available_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);

      access.clear();

      CHECK_EQUAL(0U, queue.size());

      CHECK(access.called_lock);
      CHECK(access.called_unlock);

      access.clear();

      CHECK_EQUAL(4U, queue.available());

      CHECK(access.called_lock);
      CHECK(access.called_unlock);

      access.clear();

      queue.push_from_unlocked(1);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(1U, queue.size_from_unlocked());
      CHECK_EQUAL(3U, queue.available_from_unlocked());

      access.clear();

      queue.push(2);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(2U, queue.size_from_unlocked());
      CHECK_EQUAL(2U, queue.available_from_unlocked());

      access.clear();

      queue.push(3);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(3U, queue.size_from_unlocked());
      CHECK_EQUAL(1U, queue.available_from_unlocked());

      access.clear();

      queue.push(4);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(4U, queue.size_from_unlocked());
      CHECK_EQUAL(0U, queue.available_from_unlocked());

      access.clear();

      // Queue full.
      CHECK(!queue.push(5));

      queue.pop();
      // Queue not full (buffer rollover)
      CHECK(queue.push(5));

      // Queue full.
      CHECK(!queue.push(6));

      queue.pop();
      // Queue not full (buffer rollover)
      CHECK(queue.push(6));

      access.clear();

      int i;

      CHECK(queue.pop(i));
      CHECK_EQUAL(3, i);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(3U, queue.size_from_unlocked());

      access.clear();

      CHECK(queue.pop_from_unlocked(i));
      CHECK_EQUAL(4, i);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(2U, queue.size_from_unlocked());

      access.clear();

      CHECK(queue.pop_from_unlocked(i));
      CHECK_EQUAL(5, i);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(1U, queue.size_from_unlocked());

      access.clear();

      CHECK(queue.pop_from_unlocked(i));
      CHECK_EQUAL(6, i);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(0U, queue.size_from_unlocked());

      access.clear();

      CHECK(!queue.pop(i));
      CHECK(!queue.pop_from_unlocked(i));
    }

#if !defined(ETL_FORCE_TEST_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_move_push_pop)
    {
      etl::queue_spsc_locked<ItemM, 4, etl::memory_model::MEMORY_MODEL_SMALL> queue(lock, unlock);

      ItemM p1(1);
      ItemM p2(2);
      ItemM p3(3);
      ItemM p4(4);

      queue.push(std::move(p1));
      queue.push(std::move(p2));
      queue.push(std::move(p3));
      queue.push(std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      ItemM pr(0);

      queue.pop(pr);
      CHECK_EQUAL(1, pr.value);

      queue.pop(pr);
      CHECK_EQUAL(2, pr.value);

      queue.pop(pr);
      CHECK_EQUAL(3, pr.value);

      queue.pop(pr);
      CHECK_EQUAL(4, pr.value);
    }
#endif

    //*************************************************************************
    TEST(test_size_push_pop_iqueue)
    {
      access.clear();

      QueueInt queue(lock, unlock);

      IQueueInt& iqueue = queue;

      CHECK_EQUAL(0U, iqueue.size_from_unlocked());

      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);

      access.clear();

      CHECK_EQUAL(0U, iqueue.size());

      CHECK(access.called_lock);
      CHECK(access.called_unlock);

      access.clear();

      iqueue.push_from_unlocked(1);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(1U, iqueue.size_from_unlocked());

      access.clear();

      iqueue.push(2);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(2U, iqueue.size_from_unlocked());

      access.clear();

      iqueue.push(3);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(3U, iqueue.size_from_unlocked());

      access.clear();

      iqueue.push(4);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(4U, iqueue.size_from_unlocked());

      access.clear();

      CHECK(!iqueue.push(5));
      CHECK(!iqueue.push_from_unlocked(5));

      access.clear();

      int i;

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(1, i);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(3U, iqueue.size_from_unlocked());

      access.clear();

      CHECK(iqueue.pop_from_unlocked(i));
      CHECK_EQUAL(2, i);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(2U, iqueue.size_from_unlocked());

      access.clear();

      CHECK(iqueue.pop_from_unlocked(i));
      CHECK_EQUAL(3, i);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(1U, iqueue.size_from_unlocked());

      access.clear();

      CHECK(iqueue.pop_from_unlocked(i));
      CHECK_EQUAL(4, i);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(0U, iqueue.size_from_unlocked());

      access.clear();

      CHECK(!iqueue.pop(i));
      CHECK(!iqueue.pop_from_unlocked(i));
    }

    //*************************************************************************
    TEST(test_size_push_pop_void)
    {
      access.clear();

      QueueInt queue(lock, unlock);

      CHECK_EQUAL(0U, queue.size_from_unlocked());

      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);

      access.clear();

      CHECK_EQUAL(0U, queue.size());

      CHECK(access.called_lock);
      CHECK(access.called_unlock);

      access.clear();

      queue.push_from_unlocked(1);
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(1U, queue.size_from_unlocked());

      access.clear();

      queue.push(2);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(2U, queue.size_from_unlocked());

      access.clear();

      queue.push(3);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(3U, queue.size_from_unlocked());

      access.clear();

      queue.push(4);
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(4U, queue.size_from_unlocked());

      access.clear();

      CHECK(!queue.push(5));
      CHECK(!queue.push_from_unlocked(5));

      access.clear();

      CHECK(queue.pop());
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(3U, queue.size_from_unlocked());

      access.clear();

      CHECK(queue.pop_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(2U, queue.size_from_unlocked());

      access.clear();

      CHECK(queue.pop_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(1U, queue.size_from_unlocked());

      access.clear();

      CHECK(queue.pop_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
      CHECK_EQUAL(0U, queue.size_from_unlocked());

      access.clear();

      CHECK(!queue.pop());
      CHECK(!queue.pop_from_unlocked());
    }

    //*************************************************************************
    TEST(test_push_255)
    {
      QueueInt255 queue(lock, unlock);

      for (int i = 0; i < 255; ++i)
      {
        queue.push(i);
      }

      CHECK_EQUAL(255U, queue.size());
    }

    //*************************************************************************
    TEST(test_size_push_front_pop)
    {
      access.clear();

      etl::queue_spsc_locked<int, 4, etl::memory_model::MEMORY_MODEL_SMALL> queue(lock, unlock);

      CHECK_EQUAL(0U, queue.size());

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);
      CHECK_EQUAL(4U, queue.size());

      CHECK_EQUAL(1, queue.front());
      CHECK_EQUAL(4U, queue.size());

      CHECK_EQUAL(1, queue.front());
      CHECK_EQUAL(4U, queue.size());

      CHECK(queue.pop());
      CHECK_EQUAL(3U, queue.size());

      CHECK(queue.pop());
      CHECK_EQUAL(2U, queue.size());

      CHECK(queue.pop());
      CHECK_EQUAL(1U, queue.size());

      CHECK_EQUAL(4, queue.front());
      CHECK_EQUAL(1U, queue.size());

      CHECK_EQUAL(4, queue.front());
      CHECK_EQUAL(1U, queue.size());

      CHECK(queue.pop());
      CHECK_EQUAL(0U, queue.size());
    }

    //*************************************************************************
    TEST(test_multiple_emplace)
    {
      etl::queue_spsc_locked<Data, 5, etl::memory_model::MEMORY_MODEL_SMALL> queue(lock, unlock);

      queue.emplace();
      queue.emplace(1);
      queue.emplace(1, 2);
      queue.emplace(1, 2, 3);
      queue.emplace(1, 2, 3, 4);

      CHECK_EQUAL(5U, queue.size());

      Data popped;

      queue.pop(popped);
      CHECK(popped == Data(0, 0, 0, 0));
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
      access.clear();

      QueueInt queue(lock, unlock);

      CHECK_EQUAL(0U, queue.size());

      queue.push(1);
      queue.push(2);
      queue.clear();
      CHECK(access.called_lock);
      CHECK(access.called_unlock);
      CHECK_EQUAL(0U, queue.size());

      access.clear();

      // Do it again to check that clear() didn't screw up the internals.
      queue.push_from_unlocked(1);
      queue.push_from_unlocked(2);
      CHECK_EQUAL(2U, queue.size_from_unlocked());
      queue.clear_from_unlocked();
      CHECK_EQUAL(0U, queue.size_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
    }

    //*************************************************************************
    TEST(test_empty)
    {
      access.clear();

      QueueInt queue(lock, unlock);

      CHECK(queue.empty());
      CHECK(access.called_lock);
      CHECK(access.called_unlock);

      queue.push(1);

      access.clear();

      CHECK(!queue.empty());
      CHECK(access.called_lock);
      CHECK(access.called_unlock);

      queue.clear();
      access.clear();

      CHECK(queue.empty_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);

      queue.push(1);

      access.clear();

      CHECK(!queue.empty_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
    }

    //*************************************************************************
    TEST(test_full)
    {
      access.clear();

      QueueInt queue(lock, unlock);

      CHECK(!queue.full());
      CHECK(access.called_lock);
      CHECK(access.called_unlock);

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      access.clear();

      CHECK(queue.full());
      CHECK(access.called_lock);
      CHECK(access.called_unlock);

      queue.clear();
      access.clear();

      CHECK(!queue.full_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      access.clear();

      CHECK(queue.full_from_unlocked());
      CHECK(!access.called_lock);
      CHECK(!access.called_unlock);
    }

    //*************************************************************************
#if REALTIME_TEST
#if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently supported
    #define RAISE_THREAD_PRIORITY  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)
    #define FIX_PROCESSOR_AFFINITY SetThreadAffinityMask(GetCurrentThread(), 1);
  #else
    #define RAISE_THREAD_PRIORITY
    #define FIX_PROCESSOR_AFFINITY
  #endif

    size_t ticks = 0UL;

    struct ThreadLock
    {
      void lock()
      {
        mutex.lock();
      }

      void unlock()
      {
        mutex.unlock();
      }

      std::mutex mutex;
    };

    ThreadLock threadLock;

    etl::function_imv<ThreadLock, threadLock, &ThreadLock::lock>   lock;
    etl::function_imv<ThreadLock, threadLock, &ThreadLock::unlock> unlock;

    etl::queue_spsc_locked<int, 10> queue(lock, unlock);

    const size_t LENGTH = 1000UL;

    void timer_thread()
    {
      RAISE_THREAD_PRIORITY;
      FIX_PROCESSOR_AFFINITY;

      const size_t TICK = 1UL;
      size_t tick = TICK;
      ticks = 1;

      while (ticks <= LENGTH)
      {
        if (threadLock.mutex.try_lock())
        {
          if (queue.push_from_unlocked(ticks))
          {
            ++ticks;
          }

          threadLock.mutex.unlock();
        }

        Sleep(0);
      }
    }

    TEST(queue_threads)
    {
      FIX_PROCESSOR_AFFINITY;

      std::vector<int> tick_list;
      tick_list.reserve(LENGTH);

      std::thread t1(timer_thread);

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

      for (size_t i = 0UL; i < LENGTH; ++i)
      {
        CHECK_EQUAL(i + 1, tick_list[i]);
      }
    }
#endif
  };
}
