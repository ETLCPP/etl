/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "spsc_queue.h"

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

    char   c;
    int    i;
    double d;
  };

  bool operator == (const Item& lhs, const Item& rhs)
  {
    return (lhs.c == rhs.c) && (lhs.i == rhs.i) && (lhs.d == rhs.d);
  }

  typedef etl::spsc_queue<Item, 4, etl::spsc_queue_implementation::STD_ATOMIC> Queue;
  typedef etl::ispsc_queue<Item, etl::spsc_queue_implementation::STD_ATOMIC>   IQueue;

  SUITE(test_queue)
  {
    Item item1(1, 2, 3.4);
    Item item2(2, 3, 4.5);
    Item item3(3, 4, 5.6);
    Item item4(4, 5, 6.7);

    TEST(test_normal_operation)
    {
      Queue queue;

      IQueue& iqueue = queue;

      CHECK(queue.empty());
      CHECK(!queue.full());

      CHECK_EQUAL(4U, queue.capacity());
      CHECK_EQUAL(4U, queue.max_size());

      // Fill the queue.
      CHECK(queue.push(item1));
      CHECK_EQUAL(1U, queue.size());
      CHECK_EQUAL(3U, queue.available());
      CHECK(!queue.empty());
      CHECK(!queue.full());

      CHECK(iqueue.push(item2));
      CHECK_EQUAL(2U, iqueue.size());
      CHECK_EQUAL(2U, iqueue.available());
      CHECK(!iqueue.empty());
      CHECK(!iqueue.full());

      CHECK(queue.push(item3));
      CHECK_EQUAL(3U, queue.size());
      CHECK_EQUAL(1U, queue.available());
      CHECK(!queue.empty());
      CHECK(!queue.full());

      CHECK(iqueue.push(item4));
      CHECK_EQUAL(4U, iqueue.size());
      CHECK_EQUAL(0U, iqueue.available());
      CHECK(!iqueue.empty());
      CHECK(iqueue.full());

      // Too many
      CHECK(!queue.push(item4));
      CHECK_EQUAL(4U, queue.size());
      CHECK_EQUAL(0U, queue.available());
      CHECK(!queue.empty());
      CHECK(queue.full());
    }
  };
}
