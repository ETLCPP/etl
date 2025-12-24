/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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
#include <string>

#include "etl/user_type.h"

ETL_DECLARE_USER_TYPE(CompassDirection, int)
ETL_USER_TYPE(North, 0)
ETL_USER_TYPE(South, 180)
ETL_USER_TYPE(East,  90)
ETL_USER_TYPE(West,  270)
ETL_END_USER_TYPE(CompassDirection)

namespace
{
  SUITE(test_user_type)
  {
    //*************************************************************************
    TEST(Values)
    {
      CHECK_EQUAL(0,   CompassDirection::North);
      CHECK_EQUAL(180, CompassDirection::South);
      CHECK_EQUAL(90,  CompassDirection::East);
      CHECK_EQUAL(270, CompassDirection::West);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      CompassDirection value1 = CompassDirection::North;
      CompassDirection value2 = CompassDirection(135);

      value1 = value2;

      CHECK_EQUAL(135, value1);
    }

    //*************************************************************************
    TEST(test_equality)
    {
      CompassDirection value1 = CompassDirection::North;
      CompassDirection value2 = CompassDirection::South;
      CompassDirection value3 = CompassDirection::South;

      CHECK(value1 != value2);
      CHECK(value2 == value3);
    }

    //*************************************************************************
    TEST(test_get_value)
    {
      CompassDirection actual   = CompassDirection::North;
      CompassDirection expected = CompassDirection::North;

      CHECK_EQUAL(expected, actual.get());
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      CompassDirection value = CompassDirection::North;
      CompassDirection expected = CompassDirection(CompassDirection::North + 1);

      CHECK_EQUAL(expected, ++value);
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      CompassDirection value = CompassDirection::North;
      CompassDirection expected = CompassDirection(CompassDirection::North + 1);

      CHECK_EQUAL(CompassDirection::North, value++);
      CHECK_EQUAL(expected, value);
    }


    //*************************************************************************
    TEST(test_pre_decrement)
    {
      CompassDirection value = CompassDirection::North;
      CompassDirection expected = CompassDirection(CompassDirection::North - 1);

      CHECK_EQUAL(expected, --value);
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      CompassDirection value = CompassDirection::North;
      CompassDirection expected = CompassDirection(CompassDirection::North - 1);

      CHECK_EQUAL(CompassDirection::North, value--);
      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_add_equal)
    {
      CompassDirection value = CompassDirection::North;
      value += 3;
      CompassDirection expected = CompassDirection(CompassDirection::North + 3);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_subtract_equal)
    {
      CompassDirection value = CompassDirection::North;
      value -= 3;
      CompassDirection expected = CompassDirection(CompassDirection::North - 3);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_multiply_equal)
    {
      CompassDirection value = CompassDirection::North;
      value *= 3;
      CompassDirection expected = CompassDirection(CompassDirection::North * 3);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_divide_equal)
    {
      CompassDirection value = CompassDirection::North;
      value /= 3;
      CompassDirection expected = CompassDirection(CompassDirection::North / 3);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_mod_equal)
    {
      CompassDirection value = CompassDirection::North;
      value %= 3;
      CompassDirection expected = CompassDirection(CompassDirection::North % 3);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_and_equal)
    {
      CompassDirection value = CompassDirection::North;
      value &= 0xAAAAAAAAUL;
      CompassDirection expected = CompassDirection(CompassDirection::North & 0xAAAAAAAAUL);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_or_equal)
    {
      CompassDirection value = CompassDirection::North;
      value |= 0xAAAAAAAAUL;
      CompassDirection expected = CompassDirection(CompassDirection::North | 0xAAAAAAAAUL);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_xor_equal)
    {
      CompassDirection value = CompassDirection::North;
      value ^= 0xAAAAAAAAUL;
      CompassDirection expected = CompassDirection(CompassDirection::North ^ 0xAAAAAAAAUL);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_shift_left)
    {
      CompassDirection value = CompassDirection::North;
      value <<= 2;
      CompassDirection expected = CompassDirection(CompassDirection::North << 2);

      CHECK_EQUAL(expected, value);
    }

    //*************************************************************************
    TEST(test_shift_right)
    {
      CompassDirection value = CompassDirection::North;
      value >>= 2U;
      CompassDirection expected = CompassDirection(CompassDirection::North >> 2U);

      CHECK_EQUAL(expected, value);
    }
  };
}
