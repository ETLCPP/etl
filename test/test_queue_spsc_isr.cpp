/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#include "etl/queue_spsc_isr.h"

#include <thread>
#include <mutex>
#include <vector>

#if defined(ETL_COMPILER_MICROSOFT)
#include <Windows.h>
#endif

#include "data.h"

#define REALTIME_TEST 0

namespace
{
  class Access
  {
  public:

    static void clear()
    {
      called_lock = false;
      called_unlock = false;
    }

    static void lock()
    {
      called_lock = true;
    }

    static void unlock()
    {
      called_unlock = true;
    }

    static bool called_lock;
    static bool called_unlock;
  };

  bool Access::called_lock;
  bool Access::called_unlock;

  struct Data
  {
    Data(int a_, int b_ = 2, int c_ = 3, int d_ = 4)
      : a(a_)
      , b(b_)
      , c(c_)
      , d(d_)
    {
    }

    Data()
      : a(0)
      , b(0)
      , c(0)
      , d(0)
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

  using ItemM = TestDataM<int>;

  SUITE(test_queue_isr)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      Access::clear();

      etl::queue_spsc_isr<int, 4, Access> queue;

      CHECK_EQUAL(4U, queue.max_size());
      CHECK_EQUAL(4U, queue.capacity());

      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
    }

    //*************************************************************************
    TEST(test_size_push_pop)
    {
      Access::clear();

      etl::queue_spsc_isr<int, 4, Access> queue;

      CHECK_EQUAL(0U, queue.size_from_isr());

      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);

      Access::clear();

      CHECK_EQUAL(4U, queue.available_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);

      Access::clear();

      CHECK_EQUAL(0U, queue.size());

      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);

      Access::clear();

      CHECK_EQUAL(4U, queue.available());

      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);

      Access::clear();

      queue.push_from_isr(1);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(1U, queue.size_from_isr());
      CHECK_EQUAL(3U, queue.available_from_isr());

      Access::clear();

      queue.push(2);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(2U, queue.size_from_isr());
      CHECK_EQUAL(2U, queue.available_from_isr());

      Access::clear();

      queue.push(3);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(3U, queue.size_from_isr());
      CHECK_EQUAL(1U, queue.available_from_isr());

      Access::clear();

      queue.push(4);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(4U, queue.size_from_isr());
      CHECK_EQUAL(0U, queue.available_from_isr());

      Access::clear();

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

      Access::clear();

      int i;

      CHECK(queue.pop(i));
      CHECK_EQUAL(3, i);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(3U, queue.size_from_isr());

      Access::clear();

      CHECK(queue.pop_from_isr(i));
      CHECK_EQUAL(4, i);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(2U, queue.size_from_isr());

      Access::clear();

      CHECK(queue.pop_from_isr(i));
      CHECK_EQUAL(5, i);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(1U, queue.size_from_isr());

      Access::clear();

      CHECK(queue.pop_from_isr(i));
      CHECK_EQUAL(6, i);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(0U, queue.size_from_isr());

      Access::clear();

      CHECK(!queue.pop(i));
      CHECK(!queue.pop_from_isr(i));
    }

    //*************************************************************************
    TEST(test_move_push_pop)
    {
      etl::queue_spsc_isr<ItemM, 4, Access> queue;

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

    //*************************************************************************
    TEST(test_size_push_pop_iqueue)
    {
      Access::clear();

      etl::queue_spsc_isr<int, 4, Access> queue;

      etl::iqueue_spsc_isr<int, Access>& iqueue = queue;

      CHECK_EQUAL(0U, iqueue.size_from_isr());

      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);

      Access::clear();

      CHECK_EQUAL(0U, iqueue.size());

      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);

      Access::clear();

      iqueue.push_from_isr(1);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(1U, iqueue.size_from_isr());

      Access::clear();

      iqueue.push(2);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(2U, iqueue.size_from_isr());

      Access::clear();

      iqueue.push(3);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(3U, iqueue.size_from_isr());

      Access::clear();

      iqueue.push(4);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(4U, iqueue.size_from_isr());

      Access::clear();

      CHECK(!iqueue.push(5));
      CHECK(!iqueue.push_from_isr(5));

      Access::clear();

      int i;

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(1, i);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(3U, iqueue.size_from_isr());

      Access::clear();

      CHECK(iqueue.pop_from_isr(i));
      CHECK_EQUAL(2, i);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(2U, iqueue.size_from_isr());

      Access::clear();

      CHECK(iqueue.pop_from_isr(i));
      CHECK_EQUAL(3, i);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(1U, iqueue.size_from_isr());

      Access::clear();

      CHECK(iqueue.pop_from_isr(i));
      CHECK_EQUAL(4, i);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(0U, iqueue.size_from_isr());

      Access::clear();

      CHECK(!iqueue.pop(i));
      CHECK(!iqueue.pop_from_isr(i));
    }

    //*************************************************************************
    TEST(test_size_push_pop_void)
    {
      Access::clear();

      etl::queue_spsc_isr<int, 4, Access> queue;

      CHECK_EQUAL(0U, queue.size_from_isr());

      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);

      Access::clear();

      CHECK_EQUAL(0U, queue.size());

      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);

      Access::clear();

      queue.push_from_isr(1);
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(1U, queue.size_from_isr());

      Access::clear();

      queue.push(2);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(2U, queue.size_from_isr());

      Access::clear();

      queue.push(3);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(3U, queue.size_from_isr());

      Access::clear();

      queue.push(4);
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(4U, queue.size_from_isr());

      Access::clear();

      CHECK(!queue.push(5));
      CHECK(!queue.push_from_isr(5));

      Access::clear();

      CHECK(queue.pop());
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(3U, queue.size_from_isr());

      Access::clear();

      CHECK(queue.pop_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(2U, queue.size_from_isr());

      Access::clear();

      CHECK(queue.pop_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(1U, queue.size_from_isr());

      Access::clear();

      CHECK(queue.pop_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
      CHECK_EQUAL(0U, queue.size_from_isr());

      Access::clear();

      CHECK(!queue.pop());
      CHECK(!queue.pop_from_isr());
    }

    //*************************************************************************
    TEST(test_size_push_front_pop)
    {
      Access::clear();

      etl::queue_spsc_isr<int, 4, Access> queue;

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
      etl::queue_spsc_isr<Data, 5, Access> queue;

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
      Access::clear();

      etl::queue_spsc_isr<int, 4, Access> queue;

      CHECK_EQUAL(0U, queue.size());

      queue.push(1);
      queue.push(2);
      queue.clear();
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);
      CHECK_EQUAL(0U, queue.size());

      Access::clear();

      // Do it again to check that clear() didn't screw up the internals.
      queue.push_from_isr(1);
      queue.push_from_isr(2);
      CHECK_EQUAL(2U, queue.size_from_isr());
      queue.clear_from_isr();
      CHECK_EQUAL(0U, queue.size_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
    }

    //*************************************************************************
    TEST(test_empty)
    {
      Access::clear();

      etl::queue_spsc_isr<int, 4, Access> queue;

      CHECK(queue.empty());
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);

      queue.push(1);

      Access::clear();

      CHECK(!queue.empty());
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);

      queue.clear();
      Access::clear();

      CHECK(queue.empty_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);

      queue.push(1);

      Access::clear();

      CHECK(!queue.empty_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
    }

    //*************************************************************************
    TEST(test_full)
    {
      Access::clear();

      etl::queue_spsc_isr<int, 4, Access> queue;

      CHECK(!queue.full());
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      Access::clear();

      CHECK(queue.full());
      CHECK(Access::called_lock);
      CHECK(Access::called_unlock);

      queue.clear();
      Access::clear();

      CHECK(!queue.full_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      Access::clear();

      CHECK(queue.full_from_isr());
      CHECK(!Access::called_lock);
      CHECK(!Access::called_unlock);
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
      static void lock()
      {
        mutex.lock();
      }

      static void unlock()
      {
        mutex.unlock();
      }

      static std::mutex mutex;
    };

    std::mutex ThreadLock::mutex;

    etl::queue_spsc_isr<int, 10, ThreadLock> queue;

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
        if (ThreadLock::mutex.try_lock())
        {
          if (queue.push_from_isr(ticks))
          {
            ++ticks;
          }

          ThreadLock::mutex.unlock();
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
