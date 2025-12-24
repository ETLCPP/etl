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

#include "etl/intrusive_stack.h"
#include "etl/intrusive_links.h"

#include <vector>

namespace
{
  typedef etl::forward_link<0>       link0;
  typedef etl::bidirectional_link<1> link1;

  struct Data : public link0, public link1
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

  SUITE(test_intrusive_stack)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      etl::intrusive_stack<Data, link0> stackD;
      etl::intrusive_stack<Data, link1> stackC;

      CHECK(stackD.empty());
      CHECK(stackC.empty());

      CHECK_EQUAL(0U, stackD.size());
      CHECK_EQUAL(0U, stackC.size());
    }

    //*************************************************************************
    TEST(test_empty)
    {
      Data data1(1);
      Data data2(2);

      etl::intrusive_stack<Data, link0> stackD;
      etl::intrusive_stack<Data, link1> stackC;

      CHECK(stackD.empty());
      CHECK(stackC.empty());

      stackD.push(data1);
      stackC.push(data2);

      CHECK(!stackD.empty());
      CHECK(!stackC.empty());
    }

    //*************************************************************************
    TEST(test_size)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_stack<Data, link0> stackD;
      etl::intrusive_stack<Data, link1> stackC;

      stackD.push(data1);
      stackD.push(data2);
      stackD.push(data3);

      stackC.push(data1);
      stackC.push(data2);

      CHECK_EQUAL(3U, stackD.size());
      CHECK_EQUAL(2U, stackC.size());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_stack<Data, link0> stackD;
      etl::intrusive_stack<Data, link1> stackC;

      stackD.push(data1);
      stackD.push(data2);
      stackD.push(data3);

      stackC.push(data1);
      stackC.push(data2);

      stackD.clear();

      CHECK_TRUE(stackD.empty());
      CHECK_FALSE(stackC.empty());

      CHECK_FALSE(etl::is_linked<link0>(data1));
      CHECK_FALSE(etl::is_linked<link0>(data2));
      CHECK_FALSE(etl::is_linked<link0>(data3));

      CHECK_TRUE(etl::is_linked<link1>(data1));
      CHECK_TRUE(etl::is_linked<link1>(data2));
      CHECK_FALSE(etl::is_linked<link1>(data3));

      stackC.clear();

      CHECK_FALSE(etl::is_linked<link0>(data1));
      CHECK_FALSE(etl::is_linked<link0>(data2));
      CHECK_FALSE(etl::is_linked<link0>(data3));

      CHECK_FALSE(etl::is_linked<link1>(data1));
      CHECK_FALSE(etl::is_linked<link1>(data2));
      CHECK_FALSE(etl::is_linked<link1>(data3));

      CHECK_TRUE(stackD.empty());
      CHECK_TRUE(stackC.empty());
    }

    //*************************************************************************
    TEST(test_push)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_stack<Data, link0> stackD;
      etl::intrusive_stack<Data, link1> stackC;

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
    }

    //*************************************************************************
    TEST(test_pop)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_stack<Data, link0> stackD;
      etl::intrusive_stack<Data, link1> stackC;

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
    }

    //*************************************************************************
    TEST(test_pop_into)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_stack<Data, link0> stack1;
      etl::intrusive_stack<Data, link0> stack2;

      stack1.push(data1);
      stack1.push(data2);
      stack1.push(data3);

      stack1.pop_into(stack2);
      CHECK_EQUAL(2U, stack1.size());
      CHECK_EQUAL(data2, stack1.top());

      CHECK_EQUAL(1U, stack2.size());
      CHECK_EQUAL(data3, stack2.top());

      stack1.pop_into(stack2);
      CHECK_EQUAL(1U, stack1.size());
      CHECK_EQUAL(data1, stack1.top());

      CHECK_EQUAL(2U, stack2.size());
      CHECK_EQUAL(data2, stack2.top());

      stack1.pop_into(stack2);
      CHECK_EQUAL(0U, stack1.size());

      CHECK_EQUAL(3U, stack2.size());
      CHECK_EQUAL(data1, stack2.top());
    }

    //*************************************************************************
    TEST(test_top_const)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);

      etl::intrusive_stack<Data, link0> stackD;
      const etl::intrusive_stack<Data, link0>& stackDR = stackD;

      stackD.push(data1);
      stackD.push(data2);
      stackD.push(data3);

      CHECK_EQUAL(stackD.top(), stackDR.top());
      stackD.pop();
      CHECK_EQUAL(stackD.top(), stackDR.top());
      stackD.pop();
      CHECK_EQUAL(stackD.top(), stackDR.top());
    }

    //*************************************************************************
    TEST(test_reverse1)
    {
      Data data1(1);

      etl::intrusive_stack<Data, link0> stack;

      stack.push(data1);

      stack.reverse();

      CHECK_EQUAL(1U, stack.size());
      CHECK_EQUAL(stack.top(), data1);
    }

    //*************************************************************************
    TEST(test_reverse5)
    {
      Data data1(1);
      Data data2(2);
      Data data3(3);
      Data data4(4);
      Data data5(5);

      etl::intrusive_stack<Data, link0> stack;

      stack.push(data1);
      stack.push(data2);
      stack.push(data3);
      stack.push(data4);
      stack.push(data5);

      stack.reverse();

      CHECK_EQUAL(5U, stack.size());

      CHECK_EQUAL(stack.top(), data1);
      stack.pop();

      CHECK_EQUAL(stack.top(), data2);
      stack.pop();

      CHECK_EQUAL(stack.top(), data3);
      stack.pop();

      CHECK_EQUAL(stack.top(), data4);
      stack.pop();

      CHECK_EQUAL(stack.top(), data5);
    }
  };
}
