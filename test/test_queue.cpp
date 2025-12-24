/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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
#include "data.h"

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
      : c(c_)
      , i(i_)
      , d(d_)
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

  using ItemM = TestDataM<int>;

  SUITE(test_queue)
  {
    //*************************************************************************
    TEST(test_copy_constructor)
    {
      etl::queue<int, 4> queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      etl::queue<int, 4> queue2(queue);

      CHECK(queue.size() == queue2.size());

      while (!queue.empty())
      {
        CHECK_EQUAL(queue.front(), queue2.front());
        queue.pop();
        queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_move_constructor)
    {
      etl::queue<ItemM, 4> queue;

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

      etl::queue<ItemM, 4> queue2(std::move(queue));

      CHECK(queue.size() == queue2.size());

      while (!queue.empty())
      {
        CHECK(!bool(queue.front())); // Queue1 entry is invalid.
        CHECK(bool(queue2.front())); // Queue2 value is valid.
        queue.pop();
        queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_delete_via_iqueue)
    {
      etl::queue<int, 4>* pqueue = new etl::queue<int, 4>;

      etl::iqueue<int>* piqueue = pqueue;

      piqueue->push(1);
      piqueue->push(2);
      piqueue->push(3);
      piqueue->push(4);

      delete piqueue;
    }

    //*************************************************************************
    TEST(test_size)
    {
      etl::queue<int, 4> queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);

      CHECK_EQUAL(3U, queue.size());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      etl::queue<int, 4> queue;

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
      etl::queue<ItemNTD, 4> queue;

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
      etl::queue<int, 4> queue;

      CHECK(queue.empty());

      queue.push(1);

      CHECK(!queue.empty());
    }

    //*************************************************************************
    TEST(test_full)
    {
      etl::queue<int, 4> queue;

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
      etl::queue<int, 4> queue;

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
      etl::queue<int, 4> queue;
      const etl::queue<int, 4>& constQueue = queue;

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
      etl::queue<int, 4> queue;

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
      etl::queue<int, 4> queue;
      const etl::queue<int, 4>& constQueue = queue;

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
      etl::queue<int, 4> queue;

      queue.push(1);
      CHECK_EQUAL(1U, queue.size());

      queue.push(2);
      CHECK_EQUAL(2U, queue.size());

      CHECK_EQUAL(1, queue.front());

      queue.pop();
      CHECK_EQUAL(2, queue.front());
    }

    //*************************************************************************
    TEST(test_push_excess)
    {
      etl::queue<int, 4> queue;

      for (size_t i = 0UL; i < queue.max_size(); ++i)
      {
        queue.push(1);
      }

      CHECK_THROW(queue.push(1), etl::queue_full);
    }

    //*************************************************************************
    TEST(test_multiple_push)
    {
      etl::queue<int, 4> queue;

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

      Item& item_a = queue.emplace();
      Item& item_b = queue.emplace('b', 2, 2.3);
      Item& item_c = queue.emplace('c', 3, 3.4);
      Item& item_d = queue.emplace('d', 4, 4.5);
      Item& item_e = queue.emplace('e', 5, 5.6);

      CHECK(item_a == Item('a', 1, 1.2));
      CHECK(item_b == Item('b', 2, 2.3));
      CHECK(item_c == Item('c', 3, 3.4));
      CHECK(item_d == Item('d', 4, 4.5));
      CHECK(item_e == Item('e', 5, 5.6));

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
      etl::queue<int, 4> queue;

      queue.push(1);
      queue.push(2);
      queue.pop();
      CHECK_EQUAL(1U, queue.size());
    }

    //*************************************************************************
    TEST(test_pop_into)
    {
      etl::queue<int, 4> queue;

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
      etl::queue<int, 4> queue1;
      etl::queue<int, 4> queue2;

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
      etl::queue<int, 4> queue;

      queue.push(1);
      queue.push(2);
      queue.pop();
      queue.pop();

      CHECK_THROW(queue.pop(), etl::queue_empty);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      etl::queue<int, 4> queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);
      queue.push(4);

      etl::queue<int, 4> queue2;

      // These should be overwritten.
      queue2.push(5);
      queue2.push(6);

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
      etl::queue<int, 4> queue1;

      queue1.push(1);
      queue1.push(2);
      queue1.push(3);
      queue1.push(4);

      etl::queue<int, 4> queue2;

      etl::iqueue<int>& iqueue1 = queue1;
      etl::iqueue<int>& iqueue2 = queue2;

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
      etl::queue<int, 4> queue;

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
