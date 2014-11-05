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

#include "../queue.h"

namespace
{		
  SUITE(TestQueue)
  {
    //*************************************************************************
    TEST(Size)
    {
      etl::queue<int, 4> queue;

      queue.push(1);
      queue.push(2);
      queue.push(3);

      CHECK_EQUAL(3, queue.size());
    }

    //*************************************************************************
    TEST(Capacity)
    {
      etl::queue<int, 4> queue;

      CHECK_EQUAL(4, queue.capacity());
    }

    //*************************************************************************
    TEST(Clear)
    {
      etl::queue<int, 4> queue;

      queue.push(1);
      queue.push(2);
      queue.clear();
      CHECK_EQUAL(0, queue.size());
    }

    //*************************************************************************
    TEST(Empty)
    {
      etl::queue<int, 4> queue;

      CHECK(queue.empty());

      queue.push(1);

      CHECK(!queue.empty());
    }

    //*************************************************************************
    TEST(Full)
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
    TEST(Push)
    {
      etl::queue<int, 4> queue;

      queue.push(1);
      CHECK_EQUAL(1, queue.size());

      queue.push(2);
      CHECK_EQUAL(2, queue.size());

      CHECK_EQUAL(1, queue.front());

      queue.pop();
      CHECK_EQUAL(2, queue.front());
    }

    //*************************************************************************
    TEST(PushVoid)
    {
      etl::queue<int, 4> queue;

      queue.push() = 1;
      CHECK_EQUAL(1, queue.size());

      queue.push() = 2;
      CHECK_EQUAL(2, queue.size());

      CHECK_EQUAL(1, queue.front());

      queue.pop();
      CHECK_EQUAL(2, queue.front());
    }

    //*************************************************************************
    TEST(PushExcess)
    {
      etl::queue<int, 4> queue;

      for (size_t i = 0; i < queue.capacity(); ++i)
      {
        queue.push(1);
      }

      CHECK_THROW(queue.push(1), etl::queue_full);
    }

    //*************************************************************************
    TEST(Pop)
    {
      etl::queue<int, 4> queue;

      queue.push(1);
      queue.push(2);
      queue.pop();
      CHECK_EQUAL(1, queue.size());
    }

    //*************************************************************************
    TEST(Front)
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
    TEST(FrontConst)
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
    TEST(FrontException)
    {
      etl::queue<int, 4> queue;

      CHECK_THROW(queue.front(), etl::queue_empty);
    }

    //*************************************************************************
    TEST(Back)
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
    TEST(BackConst)
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
    TEST(BackException)
    {
      etl::queue<int, 4> queue;

      CHECK_THROW(queue.back(), etl::queue_empty);
    }

    //*************************************************************************
    TEST(CheckMultiplePush)
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
    TEST(CheckMultiplePushVoid)
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
  };
}
