/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#include <UnitTest++/UnitTest++.h>

#include <stack>

#include "../stack.h"

namespace
{		
  SUITE(test_stack)
  {
    //*************************************************************************
    TEST(test_empty)
    {
      etl::stack<int, 4> stack;

      CHECK(stack.empty());

      stack.push(1);

      CHECK(!stack.empty());
    }

    //*************************************************************************
    TEST(test_full)
    {
      etl::stack<int, 4> stack;

      CHECK(!stack.full());

      stack.push(1);
      stack.push(2);
      stack.push(3);
      stack.push(4);

      CHECK(stack.full());
    }

    //*************************************************************************
    TEST(test_size)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      CHECK_EQUAL(3, stack.size());
    }

    //*************************************************************************
    TEST(test_capacity)
    {
      etl::stack<int, 4> stack;

      CHECK_EQUAL(4, stack.capacity());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.clear();
      CHECK_EQUAL(0, stack.size());
    }

    //*************************************************************************
    TEST(test_push)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      CHECK_EQUAL(1, stack.size());

      stack.push(2);
      CHECK_EQUAL(2, stack.size());

      CHECK_EQUAL(2, stack.top());

      stack.pop();
      CHECK_EQUAL(1, stack.top());
    }

    //*************************************************************************
    TEST(test_push_void)
    {
      etl::stack<int, 4> stack;

      stack.push() = 1;
      CHECK_EQUAL(1, stack.size());

      stack.push() = 2;
      CHECK_EQUAL(2, stack.size());

      CHECK_EQUAL(2, stack.top());

      stack.pop();
      CHECK_EQUAL(1, stack.top());
    }

    //*************************************************************************
    TEST(test_push_excess)
    {
      etl::stack<int, 4> stack;

      for (size_t i = 0; i < stack.capacity(); ++i)
      {
        stack.push(1);
      }

      CHECK_THROW(stack.push(1), etl::stack_full);
    }

    //*************************************************************************
    TEST(test_pop)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.pop();
      CHECK_EQUAL(1, stack.size());
    }

    //*************************************************************************
    TEST(test_top)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      CHECK_EQUAL(3, stack.top());

      stack.pop();
      CHECK_EQUAL(2, stack.top());

      stack.pop();
      CHECK_EQUAL(1, stack.top());
    }

    //*************************************************************************
    TEST(test_top_const)
    {
      etl::stack<int, 4> stack;
      const etl::stack<int, 4>& constQueue = stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      CHECK_EQUAL(3, constQueue.top());

      stack.pop();
      CHECK_EQUAL(2, constQueue.top());

      stack.pop();
      CHECK_EQUAL(1, constQueue.top());
    }

    //*************************************************************************
    TEST(test_multiple_push)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      bool pass = true;

      if (stack.top() != 3)
      {
        pass = false;
      }

      stack.pop();

      if (stack.top() != 2)
      {
        pass = false;
      }

      stack.pop();

      if (stack.top() != 1)
      {
        pass = false;
      }

      CHECK(pass);
    }

    //*************************************************************************
    TEST(test_multiple_push_void)
    {
      etl::stack<int, 4> stack;

      stack.push() = 1;
      stack.push() = 2;
      stack.push() = 3;

      bool pass = true;

      if (stack.top() != 3)
      {
        pass = false;
      }

      stack.pop();

      if (stack.top() != 2)
      {
        pass = false;
      }

      stack.pop();

      if (stack.top() != 1)
      {
        pass = false;
      }

      CHECK(pass);
    }

    //*************************************************************************
    TEST(test_swap)
    {
      std::stack<int> compare1;
      std::stack<int> compare2;
      etl::stack<int, 4> stack1;
      etl::stack<int, 4> stack2;

      compare1.push(1);
      compare1.push(2);
      compare1.push(3);
      compare1.push(4);

      stack1.push(1);
      stack1.push(2);
      stack1.push(3);
      stack1.push(4);

      compare2.push(5);
      compare2.push(6);

      stack2.push(5);
      stack2.push(6);

      swap(stack1, stack2);

      CHECK_EQUAL(compare2.size(), stack1.size());
      CHECK_EQUAL(compare1.size(), stack2.size());

      for (size_t i = 0; i < stack1.size(); ++i)
      {
        CHECK_EQUAL(compare2.top(), stack1.top());
        compare2.pop();
        stack1.pop();
      }

      for (size_t i = 0; i < stack2.size(); ++i)
      {
        CHECK_EQUAL(compare1.top(), stack2.top());
        compare1.pop();
        stack2.pop();
      }
    }
  };
}
