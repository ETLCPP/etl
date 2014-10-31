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

#include "../stack.h"

namespace
{		
  SUITE(TestStack)
  {
    //*************************************************************************
    TEST(Size)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      CHECK_EQUAL(3, stack.size());
    }

    //*************************************************************************
    TEST(Capacity)
    {
      etl::stack<int, 4> stack;

      CHECK_EQUAL(4, stack.capacity());
    }

    //*************************************************************************
    TEST(Clear)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.clear();
      CHECK_EQUAL(0, stack.size());
    }

    //*************************************************************************
    TEST(Empty)
    {
      etl::stack<int, 4> stack;

      CHECK(stack.empty());

      stack.push(1);

      CHECK(!stack.empty());
    }

    //*************************************************************************
    TEST(Full)
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
    TEST(Push)
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
    TEST(PushVoid)
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
    TEST(PushExcess)
    {
      etl::stack<int, 4> stack;

      for (size_t i = 0; i < stack.capacity(); ++i)
      {
        stack.push(1);
      }

      CHECK_THROW(stack.push(1), etl::stack_full_exception);
    }

    //*************************************************************************
    TEST(Pop)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.pop();
      CHECK_EQUAL(1, stack.size());
    }

    //*************************************************************************
    TEST(Top)
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
    TEST(TopConst)
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
    TEST(TopException)
    {
      etl::stack<int, 4> stack;

      CHECK_THROW(stack.top(), etl::stack_empty_exception);
    }

    //*************************************************************************
    TEST(CheckMultiplePush)
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
    TEST(CheckMultiplePushVoid)
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
  };
}