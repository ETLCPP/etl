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
      float       f1  = NAN;
      float       f2  = 0;
      double      d1  = NAN;
      double      d2  = 0;
      long double ld1 = NAN;
      long double ld2 = 0;
      int         i   = 0;

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
      float       f1a  = INFINITY;
      float       f1b  = -INFINITY;
      float       f2   = 0;
      double      d1a  = INFINITY;
      double      d1b  = -INFINITY;
      double      d2   = 0;
      long double ld1a = INFINITY;
      long double ld1b = -INFINITY;
      long double ld2  = 0;
      int         ia   = INT_MAX;
      int         ib   = 0;
      int         ic   = INT_MIN;

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
      float       f1  = 0;
      float       f2  = 1.0f;
      double      d1  = 0;
      double      d2  = 1.0;
      long double ld1 = 0;
      long double ld2 = 1.0L;
      int         i1  = 0;
      int         i2  = 1;

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
      float       f1  = 0;
      float       f2  = 1.0f;
      double      d1  = 0;
      double      d2  = 1.0;
      long double ld1 = 0;
      long double ld2 = 1.0L;
      int         i1  = 0;
      int         i2  = 1;

      CHECK_TRUE(etl::is_exactly_equal(f1, f1));
      CHECK_FALSE(etl::is_exactly_equal(f1, f2));
      CHECK_TRUE(etl::is_exactly_equal(d1, d1));
      CHECK_FALSE(etl::is_exactly_equal(d1, d2));
      CHECK_TRUE(etl::is_exactly_equal(ld1, ld1));
      CHECK_FALSE(etl::is_exactly_equal(ld1, ld2));
      CHECK_TRUE(etl::is_exactly_equal(i1, i1));
      CHECK_FALSE(etl::is_exactly_equal(i1, i2));
    }

    //*************************************************************************
    TEST(test_floor)
    {
      float       f1  = 1.7f;
      float       f2  = -1.7f;
      double      d1  = 1.7;
      double      d2  = -1.7;
      long double ld1 = 1.7L;
      long double ld2 = -1.7L;

      CHECK_CLOSE(1.0f, etl::clib::floor(f1), 0.0001f);
      CHECK_CLOSE(-2.0f, etl::clib::floor(f2), 0.0001f);
      CHECK_CLOSE(1.0, etl::clib::floor(d1), 0.0001);
      CHECK_CLOSE(-2.0, etl::clib::floor(d2), 0.0001);
      CHECK_CLOSE(1.0L, etl::clib::floor(ld1), 0.0001L);
      CHECK_CLOSE(-2.0L, etl::clib::floor(ld2), 0.0001L);
    }

    //*************************************************************************
    TEST(test_ceil)
    {
      float       f1  = 1.2f;
      float       f2  = -1.2f;
      double      d1  = 1.2;
      double      d2  = -1.2;
      long double ld1 = 1.2L;
      long double ld2 = -1.2L;

      CHECK_CLOSE(2.0f, etl::clib::ceil(f1), 0.0001f);
      CHECK_CLOSE(-1.0f, etl::clib::ceil(f2), 0.0001f);
      CHECK_CLOSE(2.0, etl::clib::ceil(d1), 0.0001);
      CHECK_CLOSE(-1.0, etl::clib::ceil(d2), 0.0001);
      CHECK_CLOSE(2.0L, etl::clib::ceil(ld1), 0.0001L);
      CHECK_CLOSE(-1.0L, etl::clib::ceil(ld2), 0.0001L);
    }

    //*************************************************************************
    TEST(test_log10)
    {
      float       f1  = 100.0f;
      double      d1  = 1000.0;
      long double ld1 = 10000.0L;

      CHECK_CLOSE(2.0f, etl::clib::log10(f1), 0.0001f);
      CHECK_CLOSE(3.0, etl::clib::log10(d1), 0.0001);
      CHECK_CLOSE(4.0L, etl::clib::log10(ld1), 0.0001L);
    }

    //*************************************************************************
    TEST(test_fabs)
    {
      float       f1  = -1.5f;
      float       f2  = 1.5f;
      double      d1  = -2.5;
      double      d2  = 2.5;
      long double ld1 = -3.5L;
      long double ld2 = 3.5L;

      CHECK_CLOSE(1.5f, etl::clib::fabs(f1), 0.0001f);
      CHECK_CLOSE(1.5f, etl::clib::fabs(f2), 0.0001f);
      CHECK_CLOSE(2.5, etl::clib::fabs(d1), 0.0001);
      CHECK_CLOSE(2.5, etl::clib::fabs(d2), 0.0001);
      CHECK_CLOSE(3.5L, etl::clib::fabs(ld1), 0.0001L);
      CHECK_CLOSE(3.5L, etl::clib::fabs(ld2), 0.0001L);
    }

    //*************************************************************************
    TEST(test_pow)
    {
      float       fb  = 2.0f;
      float       fe  = 3.0f;
      double      db  = 2.0;
      double      de  = 10.0;
      long double ldb = 2.0L;
      long double lde = 4.0L;

      CHECK_CLOSE(8.0f, etl::clib::pow(fb, fe), 0.0001f);
      CHECK_CLOSE(1024.0, etl::clib::pow(db, de), 0.0001);
      CHECK_CLOSE(16.0L, etl::clib::pow(ldb, lde), 0.0001L);
    }

    //*************************************************************************
    TEST(test_modf)
    {
      float       f1         = 3.75f;
      float       f_intpart  = 0.0f;
      double      d1         = 3.75;
      double      d_intpart  = 0.0;
      long double ld1        = 3.75L;
      long double ld_intpart = 0.0L;

      float       f_frac  = etl::clib::modf(f1, &f_intpart);
      double      d_frac  = etl::clib::modf(d1, &d_intpart);
      long double ld_frac = etl::clib::modf(ld1, &ld_intpart);

      CHECK_CLOSE(3.0f, f_intpart, 0.0001f);
      CHECK_CLOSE(0.75f, f_frac, 0.0001f);
      CHECK_CLOSE(3.0, d_intpart, 0.0001);
      CHECK_CLOSE(0.75, d_frac, 0.0001);
      CHECK_CLOSE((double)3.0L, (double)ld_intpart, 0.0001);
      CHECK_CLOSE((double)0.75L, (double)ld_frac, 0.0001);
    }
  }
} // namespace
