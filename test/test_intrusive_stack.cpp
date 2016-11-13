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

#include "../src/intrusive_stack.h"
#include "../src/intrusive_links.h"

#include <vector>

namespace
{
  enum
  {
    DEFAULT,
    AUTO,
    CHECKED
  };

  etl::forward_link<DEFAULT, etl::link_option::DEFAULT> link;

  typedef etl::forward_link<DEFAULT, etl::link_option::DEFAULT> default_link;
  typedef etl::forward_link<CHECKED, etl::link_option::CHECKED> checked_link;

  struct Data : public default_link, public checked_link
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

  SUITE(test_intrusive_stack)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      etl::intrusive_stack<Data, default_link> stackD;
      etl::intrusive_stack<Data, checked_link> stackC;

      CHECK(stackD.empty());
      CHECK(stackC.empty());

      CHECK_EQUAL(0, stackD.size());
      CHECK_EQUAL(0, stackC.size());
    }

    //*************************************************************************
    TEST(test_empty)
    {
      etl::intrusive_stack<Data, default_link> stackD;
      etl::intrusive_stack<Data, checked_link> stackC;

      Data data1(1);
      Data data2(2);

      CHECK(stackD.empty());
      CHECK(stackC.empty());

      stackD.push(data1);
      stackC.push(data2);

      CHECK(!stackD.empty());
      CHECK(!stackC.empty());

      data1.checked_link::clear();
      data2.checked_link::clear();
    }

    //*************************************************************************
    TEST(test_size)
    {
      etl::intrusive_stack<Data, default_link> stackD;
      etl::intrusive_stack<Data, checked_link> stackC;

      Data data1(1);
      Data data2(2);
      Data data3(3);

      stackD.push(data1);
      stackD.push(data2);
      stackD.push(data3);

      stackC.push(data1);
      stackC.push(data2);

      CHECK_EQUAL(3U, stackD.size());
      CHECK_EQUAL(2U, stackC.size());

      data1.checked_link::clear();
      data2.checked_link::clear();
    }

    //*************************************************************************
    TEST(test_clear)
    {
      etl::intrusive_stack<Data, default_link> stackD;
      etl::intrusive_stack<Data, checked_link> stackC;

      Data data1(1);
      Data data2(2);
      Data data3(3);

      stackD.push(data1);
      stackD.push(data2);
      stackD.push(data3);

      stackC.push(data1);
      stackC.push(data2);

      stackD.clear();
      stackC.clear();

      CHECK(stackD.empty());
      CHECK(stackC.empty());
    }

    //*************************************************************************
    TEST(test_push)
    {
      etl::intrusive_stack<Data, default_link> stackD;
      etl::intrusive_stack<Data, checked_link> stackC;

      Data data1(1);
      Data data2(2);
      Data data3(3);

      stackD.push(data1);
      CHECK_EQUAL(stackD.top(), data1);

      stackD.push(data2);
      CHECK_EQUAL(stackD.top(), data2);

      stackD.push(data3);
      CHECK_EQUAL(stackD.top(), data3);

      stackC.push(data1);
      CHECK_EQUAL(stackC.top(), data1);

      stackC.push(data2);
      CHECK_EQUAL(stackC.top(), data2);

      data1.checked_link::clear();
      data2.checked_link::clear();
    }


    //*************************************************************************
    TEST(test_pop)
    {
      etl::intrusive_stack<Data, default_link> stackD;
      etl::intrusive_stack<Data, checked_link> stackC;

      Data data1(1);
      Data data2(2);
      Data data3(3);

      stackD.push(data1);
      stackD.push(data2);
      stackD.push(data3);

      stackC.push(data1);
      stackC.push(data2);

      CHECK_EQUAL(stackD.top(), data3);
      stackD.pop();
      CHECK_EQUAL(stackD.top(), data2);
      stackD.pop();
      CHECK_EQUAL(stackD.top(), data1);
      stackD.pop();
      CHECK(stackD.empty());

      CHECK_EQUAL(stackC.top(), data2);
      stackC.pop();
      CHECK_EQUAL(stackC.top(), data1);
      stackC.pop();
      CHECK(stackC.empty());

      data1.checked_link::clear();
      data2.checked_link::clear();
    }

    //*************************************************************************
    TEST(test_top_const)
    {
      etl::intrusive_stack<Data, default_link> stackD;
      const etl::intrusive_stack<Data, default_link>& stackDR = stackD;

      Data data1(1);
      Data data2(2);
      Data data3(3);

      stackD.push(data1);
      stackD.push(data2);
      stackD.push(data3);

      CHECK_EQUAL(stackD.top(), stackDR.top());
      stackD.pop();
      CHECK_EQUAL(stackD.top(), stackDR.top());
      stackD.pop();
      CHECK_EQUAL(stackD.top(), stackDR.top());
    }
  };
}
