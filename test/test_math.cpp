 /******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#include "etl/math.h"

#include <stdint.h>
#include <type_traits>

namespace
{
  SUITE(test_math)
  {
    //*************************************************************************
    TEST(test_is_nan)
    {
      float  f1 = NAN;
      float  f2 = 0;
      double d1 = NAN;
      double d2 = 0;
      long double ld1 = NAN;
      long double ld2 = 0;
      int i = 0;

      CHECK_TRUE(etl::is_nan(f1));
      CHECK_FALSE(etl::is_nan(f2));
      CHECK_TRUE(etl::is_nan(d1));
      CHECK_FALSE(etl::is_nan(d2));
      CHECK_TRUE(etl::is_nan(ld1));
      CHECK_FALSE(etl::is_nan(ld2));
      CHECK_FALSE(etl::is_nan(i));
    }

    //*************************************************************************
    TEST(test_is_infinity)
    {
      float  f1a = INFINITY;
      float  f1b = -INFINITY;
      float  f2 = 0;
      double d1a = INFINITY;
      double d1b = -INFINITY;
      double d2 = 0;
      long double ld1a = INFINITY;
      long double ld1b = -INFINITY;
      long double ld2 = 0;
      int ia = INT_MAX;
      int ib = 0;
      int ic = INT_MIN;

      CHECK_TRUE(etl::is_infinity(f1a));
      CHECK_TRUE(etl::is_infinity(f1b));
      CHECK_FALSE(etl::is_infinity(f2));
      CHECK_TRUE(etl::is_infinity(d1a));
      CHECK_TRUE(etl::is_infinity(d1b));
      CHECK_FALSE(etl::is_infinity(d2));
      CHECK_TRUE(etl::is_infinity(ld1a));
      CHECK_TRUE(etl::is_infinity(ld1b));
      CHECK_FALSE(etl::is_infinity(ld2));
      CHECK_FALSE(etl::is_infinity(ia));
      CHECK_FALSE(etl::is_infinity(ib));
      CHECK_FALSE(etl::is_infinity(ic));
    }

    //*************************************************************************
    TEST(test_is_zero)
    {
      float  f1 = 0;
      float  f2 = 1.0f;
      double d1 = 0;
      double d2 = 1.0;
      long double ld1 = 0;
      long double ld2 = 1.0L;
      int i1 = 0;
      int i2 = 1;

      CHECK_TRUE(etl::is_zero(f1));
      CHECK_FALSE(etl::is_zero(f2));
      CHECK_TRUE(etl::is_zero(d1));
      CHECK_FALSE(etl::is_zero(d2));
      CHECK_TRUE(etl::is_zero(ld1));
      CHECK_FALSE(etl::is_zero(ld2));
      CHECK_TRUE(etl::is_zero(i1));
      CHECK_FALSE(etl::is_zero(i2));
    }

    //*************************************************************************
    TEST(test_is_exactly_equal)
    {
      float  f1 = 0;
      float  f2 = 1.0f;
      double d1 = 0;
      double d2 = 1.0;
      long double ld1 = 0;
      long double ld2 = 1.0L;
      int i1 = 0;
      int i2 = 1;

      CHECK_TRUE(etl::is_exactly_equal(f1, f1));
      CHECK_FALSE(etl::is_exactly_equal(f1, f2));
      CHECK_TRUE(etl::is_exactly_equal(d1, d1));
      CHECK_FALSE(etl::is_exactly_equal(d1, d2));
      CHECK_TRUE(etl::is_exactly_equal(ld1, ld1));
      CHECK_FALSE(etl::is_exactly_equal(ld1, ld2));
      CHECK_TRUE(etl::is_exactly_equal(i1, i1));
      CHECK_FALSE(etl::is_exactly_equal(i1, i2));
    }
  };
}
