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

#include <queue>

#include "etl/queue.h"
#include "etl/math.h"

namespace
{
  struct Item
  {
    Item()
      : c('a')
      , i(1)
      , d(1.2)
    {
    }

    Item(char c_, int i_, double d_)
      : c(c_),
        i(i_),
        d(d_)
    {
    }

    char c;
    int i;
    double d;
  };

  bool operator == (const Item& lhs, const Item& rhs)
  {
#include "etl/private/diagnostic_float_equal_push.h"
    return (lhs.c == rhs.c) && (lhs.i == rhs.i) && (lhs.d == rhs.d);
#include "etl/private/diagnostic_pop.h"
  }

  struct ItemNTD
  {
    ItemNTD()
    {
      p = new char;
    }

    ItemNTD(const ItemNTD&)
      : p(new char)
    {
    }

    ~ItemNTD()
    {
      delete p;
    }

    char* p;
  };

  typedef etl::queue<int, 4, etl::memory_model::MEMORY_MODEL_SMALL> QueueInt;
  typedef etl::iqueue<int, etl::memory_model::MEMORY_MODEL_SMALL>   IQueueInt;

  typedef etl::queue<ItemNTD, 4, etl::memory_model::MEMORY_MODEL_SMALL> QueueItemNTD;
  typedef etl::iqueue<ItemNTD, etl::memory_model::MEMORY_MODEL_SMALL> IQueueItemNTD;

  typedef etl::queue<int, 255, etl::memory_model::MEMORY_MODEL_SMALL> QueueInt255;

  SUITE(test_queue)
  {
    //*************************************************************************
    TEST(test_copy_constructor)
    {
      QueueInt queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      QueueInt queue2(queue);

      CHECK(queue.size() == queue2.size());

      while (!queue.empty())
      {
        CHECK_EQUAL(queue.front(), queue2.front());
        queue.pop();
        queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_delete_via_iqueue)
    {
      QueueInt* pqueue = new QueueInt;

      IQueueInt* piqueue = pqueue;

      piqueue->push(1);
      piqueue->push(2);
      piqueue->push(3);
      piqueue->push(4);

      delete piqueue;
    }

    //*************************************************************************
    TEST(test_size)
    {
      QueueInt queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);

      CHECK_EQUAL(3U, queue.size());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      QueueInt queue;

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
    TEST(test_clear_non_pod)
    {
      QueueItemNTD queue;

      queue.push(ItemNTD());
      queue.push(ItemNTD());
      queue.clear();
      CHECK_EQUAL(0U, queue.size());

      // Do it again to check that clear() didn't screw up the internals.
      queue.push(ItemNTD());
      queue.push(ItemNTD());
      CHECK_EQUAL(2U, queue.size());
      queue.clear();
      CHECK_EQUAL(0U, queue.size());
    }

    //*************************************************************************
    TEST(test_empty)
    {
      QueueInt queue;

      CHECK(queue.empty());

      queue.push(1);

      CHECK(!queue.empty());
    }

    //*************************************************************************
    TEST(test_full)
    {
      QueueInt queue;

      CHECK(!queue.full());

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      CHECK(queue.full());
    }

    //*************************************************************************
    TEST(test_front)
    {
      QueueInt queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);

      CHECK_EQUAL(1, queue.front());

      queue.pop();
      CHECK_EQUAL(2, queue.front());

      queue.pop();
      CHECK_EQUAL(3, queue.front());
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      QueueInt queue;
      const QueueInt& constQueue = queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);

      CHECK_EQUAL(1, constQueue.front());

      queue.pop();
      CHECK_EQUAL(2, constQueue.front());

      queue.pop();
      CHECK_EQUAL(3, constQueue.front());
    }

    //*************************************************************************
    TEST(test_back)
    {
      QueueInt queue;

      queue.push(1);
      CHECK_EQUAL(1, queue.back());

      queue.push(2);
      CHECK_EQUAL(2, queue.back());

      queue.push(3);
      CHECK_EQUAL(3, queue.back());
    }

    //*************************************************************************
    TEST(test_back_const)
    {
      QueueInt queue;
      const QueueInt& constQueue = queue;

      queue.push(1);
      CHECK_EQUAL(1, constQueue.back());

      queue.push(2);
      CHECK_EQUAL(2, constQueue.back());

      queue.push(3);
      CHECK_EQUAL(3, constQueue.back());
    }

    //*************************************************************************
    TEST(test_push)
    {
      QueueInt queue;

      queue.push(1);
      CHECK_EQUAL(1U, queue.size());

      queue.push(2);
      CHECK_EQUAL(2U, queue.size());

      CHECK_EQUAL(1, queue.front());

      queue.pop();
      CHECK_EQUAL(2, queue.front());
    }

    //*************************************************************************
    TEST(test_push_255)
    {
      QueueInt255 queue;

      for (int i = 0; i < 255; ++i)
      {
        queue.push(i);
      }

      CHECK_EQUAL(255U, queue.size());
    }

    //*************************************************************************
    TEST(test_push_excess)
    {
      QueueInt queue;

      for (size_t i = 0UL; i < queue.max_size(); ++i)
      {
        queue.push(1);
      }

      CHECK_THROW(queue.push(1), etl::queue_full);
    }

    //*************************************************************************
    TEST(test_multiple_push)
    {
      QueueInt queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);

      bool pass = true;

      if (queue.front() != 1)
      {
        pass = false;
      }

      queue.pop();

      if (queue.front() != 2)
      {
        pass = false;
      }

      queue.pop();

      if (queue.front() != 3)
      {
        pass = false;
      }

      CHECK(pass);
    }

    //*************************************************************************
    TEST(test_multiple_emplace)
    {
      etl::queue<Item, 5> queue;

      queue.emplace();
      queue.emplace('b', 2, 2.3);
      queue.emplace('c', 3, 3.4);
      queue.emplace('d', 4, 4.5);
      queue.emplace('e', 5, 5.6);

      CHECK(queue.front() == Item('a', 1, 1.2));
      queue.pop();
      CHECK(queue.front() == Item('b', 2, 2.3));
      queue.pop();
      CHECK(queue.front() == Item('c', 3, 3.4));
      queue.pop();
      CHECK(queue.front() == Item('d', 4, 4.5));
      queue.pop();
      CHECK(queue.front() == Item('e', 5, 5.6));
      queue.pop();
    }

    //*************************************************************************
    TEST(test_pop)
    {
      QueueInt queue;

      queue.push(1);
      queue.push(2);
      queue.pop();
      CHECK_EQUAL(1U, queue.size());
    }

    //*************************************************************************
    TEST(test_pop_into)
    {
      QueueInt queue;

      int i;

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      queue.pop_into(i);
      CHECK_EQUAL(1, i);
      CHECK_EQUAL(3U, queue.size());

      queue.pop_into(i);
      CHECK_EQUAL(2, i);
      CHECK_EQUAL(2U, queue.size());

      queue.pop_into(i);
      CHECK_EQUAL(3, i);
      CHECK_EQUAL(1U, queue.size());

      queue.pop_into(i);
      CHECK_EQUAL(4, i);
      CHECK_EQUAL(0U, queue.size());
    }

    //*************************************************************************
    TEST(test_pop_into_queue)
    {
      QueueInt queue1;
      QueueInt queue2;

      queue1.push(1);
      queue1.push(2);
      queue1.push(3);
      queue1.push(4);

      queue1.pop_into(queue2);
      CHECK_EQUAL(1U, queue2.size());
      CHECK_EQUAL(1, queue2.front());
      CHECK_EQUAL(1, queue2.back());

      queue1.pop_into(queue2);
      CHECK_EQUAL(2U, queue2.size());
      CHECK_EQUAL(1, queue2.front());
      CHECK_EQUAL(2, queue2.back());

      queue1.pop_into(queue2);
      CHECK_EQUAL(3U, queue2.size());
      CHECK_EQUAL(1, queue2.front());
      CHECK_EQUAL(3, queue2.back());

      queue1.pop_into(queue2);
      CHECK_EQUAL(4U, queue2.size());
      CHECK_EQUAL(1, queue2.front());
      CHECK_EQUAL(4, queue2.back());

      int i;

      queue2.pop_into(i);
      CHECK_EQUAL(1, i);

      queue2.pop_into(i);
      CHECK_EQUAL(2, i);

      queue2.pop_into(i);
      CHECK_EQUAL(3, i);

      queue2.pop_into(i);
      CHECK_EQUAL(4, i);
    }

    //*************************************************************************
    TEST(test_pop_exception)
    {
      QueueInt queue;

      queue.push(1);
      queue.push(2);
      queue.pop();
      queue.pop();

      CHECK_THROW(queue.pop(), etl::queue_empty);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      QueueInt queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      QueueInt queue2;

      queue2 = queue;

      CHECK(queue.size() == queue2.size());

      while (!queue.empty())
      {
        CHECK_EQUAL(queue.front(), queue2.front());
        queue.pop();
        queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_assignment_interface)
    {
      QueueInt queue1;

      queue1.push(1);
      queue1.push(2);
      queue1.push(3);
      queue1.push(4);

      QueueInt queue2;

      IQueueInt& iqueue1 = queue1;
      IQueueInt& iqueue2 = queue2;

      iqueue2 = iqueue1;

      CHECK(queue1.size() == queue2.size());

      while (!queue1.empty())
      {
        CHECK_EQUAL(queue1.front(), queue2.front());
        queue1.pop();
        queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_self_assignment)
    {
      QueueInt queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      queue = queue;
#include "etl/private/diagnostic_pop.h" 
      
      CHECK(queue.max_size() == queue.size());

      CHECK_EQUAL(1, queue.front());
      queue.pop();

      CHECK_EQUAL(2, queue.front());
      queue.pop();

      CHECK_EQUAL(3, queue.front());
      queue.pop();

      CHECK_EQUAL(4, queue.front());
      queue.pop();
    }
  };
}
