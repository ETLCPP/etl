/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove, rlindeman

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

#include "priority_queue.h"

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

  bool operator < (const Item& lhs, const Item& rhs)
  {
    return (lhs.c < rhs.c);
  }

  std::ostream& operator << (std::ostream& os, const Item& item)
  {
    os << item.c << "," << item.i << "," << item.d;
    return os;
  }

  SUITE(test_priority_queue)
  {
    static const size_t SIZE = 4;

    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      CHECK_EQUAL(priority_queue.size(), size_t(0));
      CHECK_EQUAL(priority_queue.available(), SIZE);
      CHECK_EQUAL(priority_queue.max_size(), SIZE);
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      priority_queue.push(3);
      priority_queue.push(1);
      priority_queue.push(4);
      priority_queue.push(2);

      etl::priority_queue<int, SIZE> priority_queue2(priority_queue);

      CHECK(priority_queue.size() == priority_queue2.size());

      while (!priority_queue.empty())
      {
        CHECK_EQUAL(priority_queue.top(), priority_queue2.top());
        priority_queue.pop();
        priority_queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_constructor_range)
    {
      int n[] = { 3, 4, 1, 2 };
      etl::priority_queue<int, SIZE> priority_queue(std::begin(n), std::end(n));
      std::priority_queue<int> compare_priority_queue(std::begin(n), std::end(n));

      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK(!priority_queue.empty());

      while (!priority_queue.empty())
      {
        CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());
        compare_priority_queue.pop();
        priority_queue.pop();
      }
    }

    //*************************************************************************
    TEST(test_size)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      priority_queue.push(1);
      priority_queue.push(2);
      priority_queue.push(3);

      CHECK_EQUAL(3U, priority_queue.size());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      priority_queue.push(1);
      priority_queue.push(2);
      priority_queue.clear();
      CHECK_EQUAL(0U, priority_queue.size());
    }

    //*************************************************************************
    TEST(test_assign_range)
    {
      int n[] = { 3, 4, 1, 2 };
      etl::priority_queue<int, SIZE> priority_queue;
      std::priority_queue<int> compare_priority_queue(std::begin(n), std::end(n));

      priority_queue.assign(std::begin(n), std::end(n));

      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK(!priority_queue.empty());

      while (!priority_queue.empty())
      {
        CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());
        compare_priority_queue.pop();
        priority_queue.pop();
      }
    }

    //*************************************************************************
    TEST(test_empty)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      CHECK(priority_queue.empty());

      priority_queue.push(1);

      CHECK(!priority_queue.empty());
    }

    //*************************************************************************
    TEST(test_full)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      CHECK(!priority_queue.full());

      priority_queue.push(1);
      priority_queue.push(2);
      priority_queue.push(3);
      priority_queue.push(4);

      CHECK(priority_queue.full());
    }

    //*************************************************************************
    TEST(test_top)
    {
      etl::priority_queue<int, SIZE> priority_queue;
      std::priority_queue<int> compare_priority_queue;

      priority_queue.push(1);
      compare_priority_queue.push(1);
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.push(3);
      compare_priority_queue.push(3);
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.push(2);
      compare_priority_queue.push(2);
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.push(4);
      compare_priority_queue.push(4);
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());
    }

    //*************************************************************************
    TEST(test_top_const)
    {
      etl::priority_queue<int, SIZE> priority_queue;
      const etl::priority_queue<int, SIZE>& constQueue = priority_queue;

      priority_queue.push(1);
      CHECK_EQUAL(1, constQueue.top());
      priority_queue.push(3);
      CHECK_EQUAL(3, constQueue.top());
      priority_queue.push(2);
      CHECK_EQUAL(3, constQueue.top());
      priority_queue.push(4);
      CHECK_EQUAL(4, constQueue.top());
    }

    //*************************************************************************
    TEST(test_push)
    {
      etl::priority_queue<int, SIZE> priority_queue;
      std::priority_queue<int> compare_priority_queue;

      priority_queue.push(1);
      compare_priority_queue.push(1);

      priority_queue.push(2);
      compare_priority_queue.push(2);

      priority_queue.push(3);
      compare_priority_queue.push(3);

      priority_queue.push(4);
      compare_priority_queue.push(4);
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());
    }

    //*************************************************************************
    TEST(test_emplace)
    {
      etl::priority_queue<Item, SIZE> priority_queue;
      std::priority_queue<Item> compare_priority_queue;

      priority_queue.emplace('d', 4, 7.8);
      compare_priority_queue.emplace('d', 4, 7.8);

      priority_queue.emplace('b', 2, 3.4);
      compare_priority_queue.emplace('b', 2, 3.4);

      priority_queue.emplace('a', 1, 1.2);
      compare_priority_queue.emplace('a', 1, 1.2);

      priority_queue.emplace('c', 3, 5.6);
      compare_priority_queue.emplace('c', 3, 5.6);
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());
    }

    //*************************************************************************
    TEST(test_push_excess)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      for (size_t i = 0; i < priority_queue.max_size(); ++i)
      {
        priority_queue.push(1);
      }

      CHECK_THROW(priority_queue.push(1), etl::priority_queue_full);
    }

    //*************************************************************************
    TEST(test_pop)
    {
      etl::priority_queue<int, SIZE> priority_queue;
      std::priority_queue<int> compare_priority_queue;

      priority_queue.push(1);
      compare_priority_queue.push(1);
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());

      priority_queue.push(3);
      compare_priority_queue.push(3);
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.push(1);
      compare_priority_queue.push(1);
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.push(1);
      compare_priority_queue.push(1);
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.push(2);
      compare_priority_queue.push(2);
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.push(1);
      compare_priority_queue.push(1);
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), priority_queue.top());

      priority_queue.pop();
      compare_priority_queue.pop();
      CHECK_EQUAL(compare_priority_queue.size(), priority_queue.size());
    }

    //*************************************************************************
    TEST(test_pop_into)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      priority_queue.push(1);
      priority_queue.push(3);
      priority_queue.push(4);
      priority_queue.push(2);

      int i;

      priority_queue.pop_into(i);
      CHECK_EQUAL(4, i);

      priority_queue.pop_into(i);
      CHECK_EQUAL(3, i);

      priority_queue.pop_into(i);
      CHECK_EQUAL(2, i);

      priority_queue.pop_into(i);
      CHECK_EQUAL(1, i);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      etl::priority_queue<int, SIZE> priority_queue;

      priority_queue.push(1);
      priority_queue.push(4);
      priority_queue.push(3);
      priority_queue.push(2);

      etl::priority_queue<int, SIZE> priority_queue2;

      priority_queue2 = priority_queue;

      CHECK(priority_queue.size() == priority_queue2.size());

      while (!priority_queue.empty())
      {
        CHECK_EQUAL(priority_queue.top(), priority_queue2.top());
        priority_queue.pop();
        priority_queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_assignment_interface)
    {
      etl::priority_queue<int, SIZE> priority_queue1;
      
      priority_queue1.push(1);
      priority_queue1.push(4);
      priority_queue1.push(3);
      priority_queue1.push(2);

      etl::priority_queue<int, SIZE> priority_queue2;

      etl::ipriority_queue<int, etl::vector<int, SIZE>, std::less<int>>& ipriority_queue1 = priority_queue1;      
      etl::ipriority_queue<int, etl::vector<int, SIZE>, std::less<int>>& ipriority_queue2 = priority_queue2;

      ipriority_queue2 = ipriority_queue1;

      CHECK(priority_queue1.size() == priority_queue2.size());

      while (!priority_queue1.empty())
      {
        CHECK_EQUAL(priority_queue1.top(), priority_queue2.top());
        priority_queue1.pop();
        priority_queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_self_assignment)
    {
      etl::priority_queue<int, SIZE> priority_queue1;

      priority_queue1.push(1);
      priority_queue1.push(4);
      priority_queue1.push(3);
      priority_queue1.push(2);

      etl::priority_queue<int, SIZE> priority_queue2 = priority_queue1;

      priority_queue1 = priority_queue1;

      CHECK(priority_queue1.size() == priority_queue2.size());

      while (!priority_queue1.empty())
      {
        CHECK_EQUAL(priority_queue1.top(), priority_queue2.top());
        priority_queue1.pop();
        priority_queue2.pop();
      }
    }

    //*************************************************************************
    TEST(test_interface)
    {
      typedef etl::priority_queue<int, SIZE> priority_queue_t;
      priority_queue_t priority_queue;
      etl::ipriority_queue<int, priority_queue_t::container_type, priority_queue_t::compare_type>& ipriority_queue = priority_queue;

      std::priority_queue<int> compare_priority_queue;

      ipriority_queue.push(1);
      compare_priority_queue.push(1);

      ipriority_queue.push(2);
      compare_priority_queue.push(2);

      ipriority_queue.push(3);
      compare_priority_queue.push(3);

      ipriority_queue.push(4);
      compare_priority_queue.push(4);
      CHECK_EQUAL(compare_priority_queue.size(), ipriority_queue.size());
      CHECK_EQUAL(compare_priority_queue.top(), ipriority_queue.top());
    }
  };
}
