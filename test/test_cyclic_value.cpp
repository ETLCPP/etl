///\file

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

#include "../cyclic_value.h"

namespace
{		
  SUITE(TestCyclicValue)
  {
    //*************************************************************************
    TEST(CompileTimeInitialisation)
    {
      etl::cyclic_value<int, 2, 7> value;

      CHECK_EQUAL(2, value);
      CHECK_EQUAL(2, value.first());
      CHECK_EQUAL(7, value.last());
    }

    //*************************************************************************
    TEST(RunTimeInitialisation)
    {
      etl::cyclic_value<int> value(2, 7, 1);

      CHECK_EQUAL(2, value);
      CHECK_EQUAL(2, value.first());
      CHECK_EQUAL(7, value.last());
    }

    //*************************************************************************
    TEST(set)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);

      CHECK_EQUAL(2, value);
      CHECK_EQUAL(2, value.first());
      CHECK_EQUAL(7, value.last());
    }

    //*************************************************************************
    TEST(ToFirst)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);
      ++value;
      value.to_first();

      CHECK_EQUAL(value.first(), value);
    }

    //*************************************************************************
    TEST(ToLast)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);
      value.to_last();

      CHECK_EQUAL(value.last(), value);
    }

    //*************************************************************************
    TEST(Increment)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);

      for (int i = value.first(); i <= value.last(); ++i)
      {
        CHECK_EQUAL(i, value);
        ++value;
      }
    }

    //*************************************************************************
    TEST(Decrement)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);
      value.to_last();

      for (int i = value.last(); i >= value.first(); --i)
      {
        CHECK_EQUAL(i, value);
        --value;
      }
    }

    //*************************************************************************
    TEST(IncrementWrap)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);

      int expected[8] = { 2, 3, 4, 5, 6, 7, 2, 3 };

      for (int i = 0; i < 8; ++i)
      {
        CHECK_EQUAL(expected[i], value);
        ++value;
      }
    }

    //*************************************************************************
    TEST(DecrementWrap)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);

      int expected[8] = { 2, 7, 6, 5, 4, 3, 2, 7 };

      for (int i = 0; i > 8; ++i)
      {
        CHECK_EQUAL(expected[i], value);
        --value;
      }
    }

    //*************************************************************************
    TEST(NumberOfSteps)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7, 1);
      CHECK_EQUAL(7 - 2 + 1, value.number_of_steps());

      value.set(2, 7, 2);
      CHECK_EQUAL((7 - 2 + 1) / 2, value.number_of_steps());
    }

    //*************************************************************************
    TEST(AdvancePositive)
    {
      etl::cyclic_value<int, 2, 7, 1> value;

      value.advance(2);

      CHECK_EQUAL(4, value);
    }

    //*************************************************************************
    TEST(AdvanceNegative)
    {
      etl::cyclic_value<int, 2, 7, 1> value;

      value.to_last();
      value.advance(-2);

      CHECK_EQUAL(5, value);
    }

    //*************************************************************************
    TEST(TestException)
    {
      etl::cyclic_value<int> value;
      
      CHECK_THROW(value.set(2, 7, 4), etl::cyclic_value_illegal_range);
    }
  };
}