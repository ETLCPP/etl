/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#include "etl/intrusive_queue.h"
#include "etl/intrusive_links.h"

#include <vector>

namespace
{
  typedef etl::forward_link<0>       link_fwd;
  typedef etl::bidirectional_link<1> link_bdir;

  struct Data : public link_fwd, public link_bdir
  {
    Data(int i_)
      : i(i_)
    {

    }

    int i;
  };

  bool operator ==(const Data& lhs, const Data& rhs)
  {
    return lhs.i == rhs.i;
  }

  std::ostream& operator << (std::ostream& os, const Data& data)
  {
    os << data.i;
    return os;
  }

  std::vector<Data> data =
  {
    Data(1), Data(2), Data(3), Data(4), Data(5), Data(6), Data(7), Data(8)
  };

  SUITE(test_intrusive_queue)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      etl::intrusive_queue<Data, link_fwd> queueD;
      etl::intrusive_queue<Data, link_bdir> queueC;

      CHECK(queueD.empty());
      CHECK(queueC.empty());

      CHECK_EQUAL(0U, queueD.size());
      CHECK_EQUAL(0U, queueC.size());
    }

    //*************************************************************************
    TEST(test_empty)
    {
      etl::intrusive_queue<Data, link_fwd> queueD;
      etl::intrusive_queue<Data, link_bdir> queueC;

      Data data1(1);
      Data data2(2);

      CHECK(queueD.empty());
      CHECK(queueC.empty());

      queueD.push(data1);
      queueC.push(data2);

      CHECK(!queueD.empty());
      CHECK(!queueC.empty());
    }

    //*************************************************************************
    TEST(test_size)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_fwd> queueD;
      etl::intrusive_queue<Data, link_bdir> queueC;

      queueD.push(data1);
      queueD.push(data2);
      queueD.push(data3);

      queueC.push(data1);
      queueC.push(data2);

      CHECK_EQUAL(3U, queueD.size());
      CHECK_EQUAL(2U, queueC.size());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_fwd> queueD;
      etl::intrusive_queue<Data, link_bdir> queueC;

      queueD.push(data1);
      queueD.push(data2);
      queueD.push(data3);

      queueC.push(data1);
      queueC.push(data2);

      queueD.clear();

      CHECK_TRUE(queueD.empty());
      CHECK_FALSE(queueC.empty());

      CHECK_FALSE(etl::is_linked<link_fwd>(data1));
      CHECK_FALSE(etl::is_linked<link_fwd>(data2));
      CHECK_FALSE(etl::is_linked<link_fwd>(data3));

      CHECK_TRUE(etl::is_linked<link_bdir>(data1));
      CHECK_TRUE(etl::is_linked<link_bdir>(data2));
      CHECK_FALSE(etl::is_linked<link_bdir>(data3));

      queueC.clear();

      CHECK_FALSE(etl::is_linked<link_fwd>(data1));
      CHECK_FALSE(etl::is_linked<link_fwd>(data2));
      CHECK_FALSE(etl::is_linked<link_fwd>(data3));

      CHECK_FALSE(etl::is_linked<link_bdir>(data1));
      CHECK_FALSE(etl::is_linked<link_bdir>(data2));
      CHECK_FALSE(etl::is_linked<link_bdir>(data3));

      CHECK_TRUE(queueD.empty());
      CHECK_TRUE(queueC.empty());
    }

    //*************************************************************************
    TEST(test_push)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_fwd> queueD;
      etl::intrusive_queue<Data, link_bdir> queueC;

      queueD.push(data1);
      CHECK_EQUAL(queueD.front(), data1);
      CHECK_EQUAL(queueD.back(), data1);

      queueD.push(data2);
      CHECK_EQUAL(queueD.front(), data1);
      CHECK_EQUAL(queueD.back(), data2);

      queueD.push(data3);
      CHECK_EQUAL(queueD.front(), data1);
      CHECK_EQUAL(queueD.back(), data3);

      queueC.push(data1);
      CHECK_EQUAL(queueC.front(), data1);
      CHECK_EQUAL(queueC.back(), data1);

      queueC.push(data2);
      CHECK_EQUAL(queueC.front(), data1);
      CHECK_EQUAL(queueC.back(), data2);
    }

    //*************************************************************************
    TEST(test_double_push)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);
      Data data4(4);

      etl::intrusive_queue<Data, link_fwd> queue;

      queue.push(data1);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_FALSE(data2.link_fwd::is_linked());
      CHECK_FALSE(data3.link_fwd::is_linked());
      CHECK_FALSE(data4.link_fwd::is_linked());

      queue.push(data2);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_FALSE(data3.link_fwd::is_linked());
      CHECK_FALSE(data4.link_fwd::is_linked());

      queue.push(data3);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_TRUE(data3.link_fwd::is_linked());
      CHECK_FALSE(data4.link_fwd::is_linked());

      CHECK_THROW(queue.push(data2), etl::intrusive_queue_value_is_already_linked);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_TRUE(data3.link_fwd::is_linked());
      CHECK_FALSE(data4.link_fwd::is_linked());

      queue.push(data4);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_TRUE(data3.link_fwd::is_linked());
      CHECK_TRUE(data4.link_fwd::is_linked());

      CHECK_EQUAL(4, queue.size());
    }

    //*************************************************************************
    TEST(test_pop)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_fwd> queueD;
      etl::intrusive_queue<Data, link_bdir> queueC;

      queueD.push(data1);
      queueD.push(data2);
      queueD.push(data3);

      queueC.push(data1);
      queueC.push(data2);

      CHECK_EQUAL(queueD.front(), data1);
      CHECK_EQUAL(queueD.back(), data3);
      queueD.pop();
      CHECK_FALSE(data1.link_fwd::is_linked());
      CHECK_EQUAL(queueD.front(), data2);
      CHECK_EQUAL(queueD.back(), data3);
      queueD.pop();
      CHECK_FALSE(data2.link_fwd::is_linked());
      CHECK_EQUAL(queueD.front(), data3);
      CHECK_EQUAL(queueD.back(), data3);
      queueD.pop();
      CHECK_FALSE(data3.link_fwd::is_linked());
      CHECK(queueD.empty());

      CHECK_EQUAL(queueC.front(), data1);
      CHECK_EQUAL(queueC.back(), data2);
      queueC.pop();
      CHECK_FALSE(data1.link_bdir::is_linked());
      CHECK_EQUAL(queueC.front(), data2);
      CHECK_EQUAL(queueC.back(), data2);
      queueC.pop();
      CHECK_FALSE(data2.link_bdir::is_linked());
      CHECK(queueC.empty());
    }

    //*************************************************************************
    TEST(test_pop_into)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_fwd> queue1;
      etl::intrusive_queue<Data, link_fwd> queue2;

      queue1.push(data1);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_FALSE(data2.link_fwd::is_linked());
      CHECK_FALSE(data3.link_fwd::is_linked());

      queue1.push(data2);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_FALSE(data3.link_fwd::is_linked());

      queue1.push(data3);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_TRUE(data3.link_fwd::is_linked());

      queue1.pop_into(queue2);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_EQUAL(2U, queue1.size());
      CHECK_EQUAL(data2, queue1.front());

      CHECK_EQUAL(1U, queue2.size());
      CHECK_EQUAL(data1, queue2.front());
      CHECK_EQUAL(data1, queue2.back());

      queue1.pop_into(queue2);
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_EQUAL(1U, queue1.size());
      CHECK_EQUAL(data3, queue1.front());

      CHECK_EQUAL(2U, queue2.size());
      CHECK_EQUAL(data1, queue2.front());
      CHECK_EQUAL(data2, queue2.back());

      queue1.pop_into(queue2);
      CHECK_TRUE(data3.link_fwd::is_linked());
      CHECK_EQUAL(0U, queue1.size());

      CHECK_EQUAL(3U, queue2.size());
      CHECK_EQUAL(data1, queue2.front());
      CHECK_EQUAL(data3, queue2.back());
    }

    //*************************************************************************
    TEST(test_pop_forward_link_clear)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_fwd> queue;

      queue.push(data1);
      queue.push(data2);
      queue.push(data3);
      CHECK_TRUE(data1.link_fwd::is_linked());
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_TRUE(data3.link_fwd::is_linked());

      queue.pop();
      CHECK_FALSE(data1.link_fwd::is_linked());
      CHECK_TRUE(data2.link_fwd::is_linked());
      CHECK_TRUE(data3.link_fwd::is_linked());

      queue.pop();
      CHECK_FALSE(data1.link_fwd::is_linked());
      CHECK_FALSE(data2.link_fwd::is_linked());
      CHECK_TRUE(data3.link_fwd::is_linked());

      queue.pop();
      CHECK_FALSE(data1.link_fwd::is_linked());
      CHECK_FALSE(data2.link_fwd::is_linked());
      CHECK_FALSE(data3.link_fwd::is_linked());
    }

    //*************************************************************************
    TEST(test_pop_bidirectional_link_clear)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_bdir> queue;

      queue.push(data1);
      queue.push(data2);
      queue.push(data3);
      CHECK_TRUE(data1.link_bdir::is_linked());
      CHECK_TRUE(data2.link_bdir::is_linked());
      CHECK_TRUE(data3.link_bdir::is_linked());

      queue.pop();
      CHECK_FALSE(data1.link_bdir::is_linked());
      CHECK_TRUE(data2.link_bdir::is_linked());
      CHECK_TRUE(data3.link_bdir::is_linked());

      queue.pop();
      CHECK_FALSE(data1.link_bdir::is_linked());
      CHECK_FALSE(data2.link_bdir::is_linked());
      CHECK_TRUE(data3.link_bdir::is_linked());

      queue.pop();
      CHECK_FALSE(data1.link_bdir::is_linked());
      CHECK_FALSE(data2.link_bdir::is_linked());
      CHECK_FALSE(data3.link_bdir::is_linked());
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_fwd> queueD;
      const etl::intrusive_queue<Data, link_fwd>& queueDR = queueD;

      queueD.push(data1);
      queueD.push(data2);
      queueD.push(data3);

      CHECK_EQUAL(queueD.front(), queueDR.front());
      queueD.pop();
      CHECK_EQUAL(queueD.front(), queueDR.front());
      queueD.pop();
      CHECK_EQUAL(queueD.front(), queueDR.front());
    }

    //*************************************************************************
    TEST(test_back_const)
    {

      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link_fwd> queueD;
      const etl::intrusive_queue<Data, link_fwd>& queueDR = queueD;

      queueD.push(data1);
      queueD.push(data2);
      queueD.push(data3);

      CHECK_EQUAL(queueD.back(), queueDR.back());
      queueD.pop();
      CHECK_EQUAL(queueD.back(), queueDR.back());
      queueD.pop();
      CHECK_EQUAL(queueD.back(), queueDR.back());
    }
  };
}
