/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include <queue>

#include "queue.h"

namespace
{
  struct Item
  {
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
    return (lhs.c == rhs.c) && (lhs.i == rhs.i) && (lhs.d == rhs.d);
  }

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
    TEST(test_push_void)
    {
      etl::queue<int, 4> queue;

      queue.push() = 1;
      CHECK_EQUAL(1U, queue.size());

      queue.push() = 2;
      CHECK_EQUAL(2U, queue.size());

      CHECK_EQUAL(1, queue.front());

      queue.pop();
      CHECK_EQUAL(2, queue.front());
    }

    //*************************************************************************
    TEST(test_push_excess)
    {
      etl::queue<int, 4> queue;

      for (size_t i = 0; i < queue.max_size(); ++i)
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
      etl::queue<Item, 4> queue;

      queue.emplace('a', 1, 1.2);
      queue.emplace('b', 2, 3.4);
      queue.emplace('c', 3, 5.6);
      queue.emplace('d', 4, 7.8);

      CHECK(queue.front() == Item('a', 1, 1.2));
      queue.pop();
      CHECK(queue.front() == Item('b', 2, 3.4));
      queue.pop();
      CHECK(queue.front() == Item('c', 3, 5.6));
      queue.pop();
      CHECK(queue.front() == Item('d', 4, 7.8));
      queue.pop();
    }

    //*************************************************************************
    TEST(test_multiple_push_void)
    {
      etl::queue<int, 4> queue;

      queue.push() = 1;
      queue.push() = 2;
      queue.push() = 3;

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

      queue = queue;

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
