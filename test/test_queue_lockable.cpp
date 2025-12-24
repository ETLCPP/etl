/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include "etl/queue_lockable.h"

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
  //***********************************
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

  using ItemM = TestDataM<int>;

  //***********************************
  class QueueInt : public etl::queue_lockable<int, 4>
  {
  public:

    QueueInt()
      : called_lock(false)
      , called_unlock(false)
    {
    }

    void lock() const override
    {
      called_lock = true;
    }

    void unlock() const override
    {
      called_unlock = true;
    }

    void clear_test_flags()
    {
      called_lock   = false;
      called_unlock = false;
    }

    mutable bool called_lock;
    mutable bool called_unlock;
  };

  //***********************************
  class QueueData : public etl::queue_lockable<Data, 4>
  {
  public:

    QueueData()
      : called_lock(false)
      , called_unlock(false)
    {
    }

    void lock() const override
    {
      called_lock = true;
    }

    void unlock() const override
    {
      called_unlock = true;
    }

    void clear_test_flags()
    {
      called_lock = false;
      called_unlock = false;
    }

    mutable bool called_lock;
    mutable bool called_unlock;
  };

  //***********************************
  class QueueItemM : public etl::queue_lockable<ItemM, 4>
  {
  public:

    QueueItemM()
      : called_lock(false)
      , called_unlock(false)
    {
    }

    void lock() const override
    {
      called_lock = true;
    }

    void unlock() const override
    {
      called_unlock = true;
    }

    void clear_test_flags()
    {
      called_lock = false;
      called_unlock = false;
    }

    mutable bool called_lock;
    mutable bool called_unlock;
  };

  SUITE(test_queue_lockable)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      QueueInt queue;

      CHECK_EQUAL(4U, queue.max_size());
      CHECK_EQUAL(4U, queue.capacity());
    }

    //*************************************************************************
    TEST(test_size_push_pop)
    {
      QueueInt queue;

      CHECK_EQUAL(0U, queue.size());

      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear_test_flags();

      CHECK_EQUAL(4U, queue.available());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear_test_flags();

      CHECK_EQUAL(0U, queue.size());

      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear_test_flags();

      CHECK_EQUAL(4U, queue.available());

      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear_test_flags();

      queue.push(1);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(1U, queue.size());
      CHECK_EQUAL(3U, queue.available());

      queue.clear_test_flags();

      queue.push(2);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(2U, queue.size());
      CHECK_EQUAL(2U, queue.available());

      queue.clear_test_flags();

      queue.push(3);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(3U, queue.size());
      CHECK_EQUAL(1U, queue.available());

      queue.clear_test_flags();

      queue.push(4);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(4U, queue.size());
      CHECK_EQUAL(0U, queue.available());

      queue.clear_test_flags();

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

      queue.clear_test_flags();

      int i;

      CHECK(queue.pop(i));
      CHECK_EQUAL(3, i);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(3U, queue.size());

      queue.clear_test_flags();

      CHECK(queue.pop(i));
      CHECK_EQUAL(4, i);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(2U, queue.size());

      queue.clear_test_flags();

      CHECK(queue.pop(i));
      CHECK_EQUAL(5, i);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(1U, queue.size());

      queue.clear_test_flags();

      CHECK(queue.pop(i));
      CHECK_EQUAL(6, i);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(0U, queue.size());

      queue.clear_test_flags();

      CHECK(!queue.pop(i));
      CHECK(!queue.pop(i));
    }

    //*************************************************************************
    TEST(test_move_push_pop)
    {
      QueueItemM queue;

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
      QueueInt queue;

      etl::iqueue_lockable<int>& iqueue = queue;

      CHECK_EQUAL(0U, iqueue.size());

      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear_test_flags();

      CHECK_EQUAL(0U, iqueue.size());

      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear_test_flags();

      iqueue.push(1);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(1U, iqueue.size());

      queue.clear_test_flags();

      iqueue.push(2);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(2U, iqueue.size());

      queue.clear_test_flags();

      iqueue.push(3);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(3U, iqueue.size());

      queue.clear_test_flags();

      iqueue.push(4);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(4U, iqueue.size());

      queue.clear_test_flags();

      CHECK(!iqueue.push(5));
      CHECK(!iqueue.push(5));

      queue.clear_test_flags();

      int i;

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(1, i);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(3U, iqueue.size());

      queue.clear_test_flags();

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(2, i);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(2U, iqueue.size());

      queue.clear_test_flags();

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(3, i);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(1U, iqueue.size());

      queue.clear_test_flags();

      CHECK(iqueue.pop(i));
      CHECK_EQUAL(4, i);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(0U, iqueue.size());

      queue.clear_test_flags();

      CHECK(!iqueue.pop(i));
      CHECK(!iqueue.pop(i));
    }

    //*************************************************************************
    TEST(test_size_push_pop_void)
    {
      QueueInt queue;

      CHECK_EQUAL(0U, queue.size());

      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear_test_flags();

      CHECK_EQUAL(0U, queue.size());

      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear_test_flags();

      queue.push(1);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(1U, queue.size());

      queue.clear_test_flags();

      queue.push(2);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(2U, queue.size());

      queue.clear_test_flags();

      queue.push(3);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(3U, queue.size());

      queue.clear_test_flags();

      queue.push(4);
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(4U, queue.size());

      queue.clear_test_flags();

      CHECK(!queue.push(5));
      CHECK(!queue.push(5));

      queue.clear_test_flags();

      CHECK(queue.pop());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(3U, queue.size());

      queue.clear_test_flags();

      CHECK(queue.pop());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(2U, queue.size());

      queue.clear_test_flags();

      CHECK(queue.pop());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(1U, queue.size());

      queue.clear_test_flags();

      CHECK(queue.pop());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(0U, queue.size());

      queue.clear_test_flags();

      CHECK(!queue.pop());
      CHECK(!queue.pop());
    }

    //*************************************************************************
    TEST(test_size_push_front_pop)
    {
      QueueInt queue;

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
      QueueData queue;

      queue.emplace();
      queue.emplace(1);
      queue.emplace(1, 2);
      queue.emplace(1, 2, 3);

      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(4U, queue.size());

      Data popped;

      queue.pop(popped);
      CHECK(popped == Data(0, 0, 0, 0));
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
      QueueInt queue;

      CHECK_EQUAL(0U, queue.size());

      queue.push(1);
      queue.push(2);
      queue.clear();
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
      CHECK_EQUAL(0U, queue.size());

      queue.clear_test_flags();

      // Do it again to check that clear() didn't screw up the internals.
      queue.push(1);
      queue.push(2);
      CHECK_EQUAL(2U, queue.size());
      queue.clear();
      CHECK_EQUAL(0U, queue.size());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
    }

    //*************************************************************************
    TEST(test_empty)
    {
      QueueInt queue;

      CHECK(queue.empty());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.push(1);

      queue.clear_test_flags();

      CHECK(!queue.empty());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear();
      queue.clear_test_flags();

      CHECK(queue.empty());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.push(1);

      queue.clear_test_flags();

      CHECK(!queue.empty());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
    }

    //*************************************************************************
    TEST(test_full)
    {
      QueueInt queue;

      CHECK(!queue.full());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      queue.clear_test_flags();

      CHECK(queue.full());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.clear();
      queue.clear_test_flags();

      CHECK(!queue.full());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      queue.clear_test_flags();

      CHECK(queue.full());
      CHECK(queue.called_lock);
      CHECK(queue.called_unlock);
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

    class QueueIntMutex : public etl::queue_lockable<int, 10>
    {
    public:

      void lock() const override
      {
        mutex.lock();
      }

      void unlock() const override
      {
        mutex.unlock();
      }

      mutable std::mutex mutex;
    };

    QueueIntMutex queue;

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
        if (queue.push(ticks))
        {
          ++ticks;
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
