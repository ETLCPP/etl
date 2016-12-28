/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#include <UnitTest++/UnitTest++.h>

#include "../src/intrusive_queue.h"
#include "../src/intrusive_links.h"

#include <vector>

namespace
{
  typedef etl::forward_link<0>       link0;
  typedef etl::bidirectional_link<1> link1;

  struct Data : public link0, public link1
  {
    Data(int i)
      : i(i)
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
      etl::intrusive_queue<Data, link0> queueD;
      etl::intrusive_queue<Data, link1> queueC;

      CHECK(queueD.empty());
      CHECK(queueC.empty());

      CHECK_EQUAL(0U, queueD.size());
      CHECK_EQUAL(0U, queueC.size());
    }

    //*************************************************************************
    TEST(test_empty)
    {
      etl::intrusive_queue<Data, link0> queueD;
      etl::intrusive_queue<Data, link1> queueC;

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

      etl::intrusive_queue<Data, link0> queueD;
      etl::intrusive_queue<Data, link1> queueC;

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

      etl::intrusive_queue<Data, link0> queueD;
      etl::intrusive_queue<Data, link1> queueC;

      queueD.push(data1);
      queueD.push(data2);
      queueD.push(data3);

      queueC.push(data1);
      queueC.push(data2);

      queueD.clear();
      queueC.clear();

      CHECK(queueD.empty());
      CHECK(queueC.empty());
    }

    //*************************************************************************
    TEST(test_push)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link0> queueD;
      etl::intrusive_queue<Data, link1> queueC;

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
    TEST(test_pop)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link0> queueD;
      etl::intrusive_queue<Data, link1> queueC;

      queueD.push(data1);
      queueD.push(data2);
      queueD.push(data3);

      queueC.push(data1);
      queueC.push(data2);

      CHECK_EQUAL(queueD.front(), data1);
      CHECK_EQUAL(queueD.back(), data3);
      queueD.pop();
      CHECK_EQUAL(queueD.front(), data2);
      CHECK_EQUAL(queueD.back(), data3);
      queueD.pop();
      CHECK_EQUAL(queueD.front(), data3);
      CHECK_EQUAL(queueD.back(), data3);
      queueD.pop();
      CHECK(queueD.empty());

      CHECK_EQUAL(queueC.front(), data1);
      CHECK_EQUAL(queueC.back(), data2);
      queueC.pop();
      CHECK_EQUAL(queueC.front(), data2);
      CHECK_EQUAL(queueC.back(), data2);
      queueC.pop();
      CHECK(queueC.empty());
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_queue<Data, link0> queueD;
      const etl::intrusive_queue<Data, link0>& queueDR = queueD;

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

      etl::intrusive_queue<Data, link0> queueD;
      const etl::intrusive_queue<Data, link0>& queueDR = queueD;

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
