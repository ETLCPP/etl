///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include "../src/cyclic_value.h"

namespace
{		
  SUITE(test_cyclic_value)
  {
    //*************************************************************************
    TEST(test_compile_time_initialisation)
    {
      etl::cyclic_value<int, 2, 7> value;

      CHECK_EQUAL(2, value);
      CHECK_EQUAL(2, value.first());
      CHECK_EQUAL(7, value.last());
    }

    //*************************************************************************
    TEST(test_run_time_initialisation)
    {
      etl::cyclic_value<int> value(2, 7);

      CHECK_EQUAL(2, value);
      CHECK_EQUAL(2, value.first());
      CHECK_EQUAL(7, value.last());
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      etl::cyclic_value<int> value(2, 7);
      etl::cyclic_value<int> value2(value);
      CHECK(value == value2);

      value2.set(3, 6);
      CHECK(value != value2);
    }

    //*************************************************************************
    TEST(test_set)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);

      CHECK_EQUAL(2, value);
      CHECK_EQUAL(2, value.first());
      CHECK_EQUAL(7, value.last());
    }

    //*************************************************************************
    TEST(test_to_first)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);
      ++value;
      value.to_first();

      CHECK_EQUAL(value.first(), value);
    }

    //*************************************************************************
    TEST(test_to_last)
    {
      etl::cyclic_value<int> value;

      value.set(2, 7);
      value.to_last();

      CHECK_EQUAL(value.last(), value);
    }

    //*************************************************************************
    TEST(test_increment)
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
    TEST(test_decrement)
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
    TEST(test_increment_wrap)
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
    TEST(test_decrement_wrap)
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
    TEST(test_advance_positive)
    {
      etl::cyclic_value<int, 2, 7> value;

      value.advance(2);

      CHECK_EQUAL(4, value);
    }

    //*************************************************************************
    TEST(test_advance_negative)
    {
      etl::cyclic_value<int, 2, 7> value;

      value.to_last();
      value.advance(-2);

      CHECK_EQUAL(5, value);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      etl::cyclic_value<int, 2, 7> value1;
      etl::cyclic_value<int, 3, 8> value2;

      value1 = value2;
      CHECK((int)value1 == (int)value2);

      value1 = 4;
      CHECK((int)value1 == 4);
    }

    //*************************************************************************
    TEST(test_equality)
    {
      etl::cyclic_value<int, 2, 7> value1;
      etl::cyclic_value<int, 3, 8> value2;
      etl::cyclic_value<int, 3, 9> value3;

      CHECK(value1 != value2);
      CHECK(value2 == value3);
    }

    //*************************************************************************
    TEST(test_swap)
    {
      etl::cyclic_value<int> compare1;
      etl::cyclic_value<int> compare2;

      compare1.set(2, 7);
      compare2.set(3, 8);

      etl::cyclic_value<int> data1(compare1);
      etl::cyclic_value<int> data2(compare2);

      swap(data1, data2);

      CHECK(data1 == compare2);
      CHECK(data2 == compare1);
    }
  };
}
