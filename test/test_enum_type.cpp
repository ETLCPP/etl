/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/enum_type.h"

struct enum_test
{
  enum enum_type
  {
    ZERO,
    ONE,
    THREE = 3,
    FOUR
  };

  ETL_DECLARE_ENUM_TYPE(enum_test, int)
  ETL_ENUM_TYPE(ZERO,  "ZERO")
  ETL_ENUM_TYPE(ONE,   "ONE")
  ETL_ENUM_TYPE(THREE, "THREE")
  ETL_END_ENUM_TYPE
};

namespace 
{		
  SUITE(test_enum_type)
  {
    //*************************************************************************
    TEST(test_values)
    {
      CHECK_EQUAL(0, enum_test::value_type(enum_test::ZERO));
      CHECK_EQUAL(1, enum_test::value_type(enum_test::ONE));
      CHECK_EQUAL(3, enum_test::value_type(enum_test::THREE));
      CHECK_EQUAL(4, enum_test::value_type(enum_test::FOUR));
    }

    //*************************************************************************
#if ETL_USING_CPP14
    TEST(test_constexpr_values)
    {
      constexpr enum_test zero = enum_test::ZERO;
      constexpr enum_test zero2(zero);
      constexpr enum_test zero3 = zero;

      constexpr int int_zero = zero;
      constexpr int int_zero2 = zero.get_value();

      constexpr enum_test enum_zero = zero;
      constexpr enum_test enum_zero2 = zero.get_enum();

      constexpr const char* string_zero = zero.c_str();

      CHECK_EQUAL(0, zero.get_value());
      CHECK_EQUAL(0, zero2.get_value());
      CHECK_EQUAL(0, zero3.get_value());
      CHECK_EQUAL(0, int_zero);
      CHECK_EQUAL(0, int_zero2);
      CHECK_EQUAL(0, enum_zero.get_value());
      CHECK_EQUAL(0, enum_zero2.get_value());
      CHECK_EQUAL(std::string("ZERO"), std::string(string_zero));
    }
#endif

    //*************************************************************************
    TEST(test_c_str)
    {
      enum_test value;

      value = enum_test::ZERO;
      CHECK_EQUAL(std::string("ZERO"), std::string(value.c_str()));

      value = enum_test::ONE;
      CHECK_EQUAL(std::string("ONE"), std::string(value.c_str()));

      value = enum_test::THREE;
      CHECK_EQUAL(std::string("THREE"), std::string(value.c_str()));

      // No ETL_ENUM_TYPE definition.
      value = enum_test::FOUR;
      CHECK_EQUAL(std::string("?"), std::string(value.c_str()));

      // Illegal value.
      value = enum_test(5);
      CHECK_EQUAL(std::string("?"), std::string(value.c_str()));
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      enum_test value1 = enum_test::ZERO;
      enum_test value2 = enum_test::ONE;

      value1 = value2;

      CHECK_EQUAL(enum_test::ONE, value1);
    }

    //*************************************************************************
    TEST(test_equality)
    {
      enum_test value1 = enum_test::ZERO;
      enum_test value2 = enum_test::ONE;
      enum_test value3 = enum_test::ONE;

      CHECK(value1 != value2);
      CHECK(value2 == value3);
    }

    //*************************************************************************
    TEST(test_get_value)
    {
      enum_test actual = enum_test::THREE;
      enum_test::value_type expected = enum_test::THREE; 

      CHECK_EQUAL(expected, actual.get_value());
    }

    //*************************************************************************
    TEST(test_get_enum)
    {
      enum_test actual = enum_test::THREE;
      enum_test::enum_type expected = enum_test::THREE;

      CHECK_EQUAL(expected, actual.get_enum());
    }
  };
}