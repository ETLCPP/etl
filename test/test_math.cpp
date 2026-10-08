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

#include <cmath>
#include <limits>
#include <stdint.h>
#include <type_traits>

namespace
{
  //***************************************************************************
  template <typename T>
  void check_classification_of_special_values()
  {
    typedef std::numeric_limits<T> STD_NL;

    const T nan           = STD_NL::quiet_NaN();
    const T negative_nan  = std::copysign(STD_NL::quiet_NaN(), T(-1));
    const T inf           = STD_NL::infinity();
    const T zero          = T(0);
    const T negative_zero = -T(0);
    const T denorm        = STD_NL::denorm_min();
    const T min_value     = STD_NL::min();
    const T max_value     = STD_NL::max();
    const T lowest_value  = STD_NL::lowest();

    // is_nan
    CHECK_TRUE(etl::is_nan(nan));
    CHECK_TRUE(etl::is_nan(negative_nan));
    CHECK_TRUE(etl::is_nan(STD_NL::signaling_NaN()));
    CHECK_FALSE(etl::is_nan(inf));
    CHECK_FALSE(etl::is_nan(-inf));
    CHECK_FALSE(etl::is_nan(zero));
    CHECK_FALSE(etl::is_nan(negative_zero));
    CHECK_FALSE(etl::is_nan(denorm));
    CHECK_FALSE(etl::is_nan(max_value));
    CHECK_FALSE(etl::is_nan(lowest_value));

    // is_infinity
    CHECK_TRUE(etl::is_infinity(inf));
    CHECK_TRUE(etl::is_infinity(-inf));
    CHECK_FALSE(etl::is_infinity(nan));
    CHECK_FALSE(etl::is_infinity(negative_nan));
    CHECK_FALSE(etl::is_infinity(zero));
    CHECK_FALSE(etl::is_infinity(denorm));
    CHECK_FALSE(etl::is_infinity(max_value));
    CHECK_FALSE(etl::is_infinity(lowest_value));

    // is_zero
    CHECK_TRUE(etl::is_zero(zero));
    CHECK_TRUE(etl::is_zero(negative_zero));
    CHECK_FALSE(etl::is_zero(denorm));
    CHECK_FALSE(etl::is_zero(-denorm));
    CHECK_FALSE(etl::is_zero(min_value));
    CHECK_FALSE(etl::is_zero(nan));
    CHECK_FALSE(etl::is_zero(inf));
    CHECK_FALSE(etl::is_zero(-inf));
  }

  //***************************************************************************
  template <typename T>
  void check_private_math_functions()
  {
    typedef std::numeric_limits<T> STD_NL;

    const T inf       = STD_NL::infinity();
    const T tolerance = T(1e-5);

    // The overload for the exact floating point type must be selected.
    CHECK_TRUE((std::is_same<T, decltype(etl::private_math::sqrt(T(1)))>::value));
    CHECK_TRUE((std::is_same<T, decltype(etl::private_math::pow(T(1), T(1)))>::value));
    CHECK_TRUE((std::is_same<T, decltype(etl::private_math::floor(T(1)))>::value));
    CHECK_TRUE((std::is_same<T, decltype(etl::private_math::round(T(1)))>::value));
    CHECK_TRUE((std::is_same<T, decltype(etl::private_math::log10(T(1)))>::value));
    CHECK_TRUE((std::is_same<T, decltype(etl::private_math::modf(T(1), static_cast<T*>(ETL_NULLPTR)))>::value));

    // sqrt
    CHECK_EQUAL(T(2), etl::private_math::sqrt(T(4)));
    CHECK_CLOSE(T(1.41421356), etl::private_math::sqrt(T(2)), tolerance);
    CHECK_TRUE(etl::private_math::sign_bit(etl::private_math::sqrt(-T(0))));
    CHECK_TRUE(etl::is_nan(etl::private_math::sqrt(T(-1))));
    CHECK_TRUE(etl::is_infinity(etl::private_math::sqrt(inf)));

    // pow
    CHECK_EQUAL(T(1024), etl::private_math::pow(T(2), T(10)));
    CHECK_EQUAL(T(1), etl::private_math::pow(T(123.456), T(0)));
    CHECK_CLOSE(T(0.01), etl::private_math::pow(T(10), T(-2)), tolerance);
    CHECK_CLOSE(T(2), etl::private_math::pow(T(4), T(0.5)), tolerance);

    // floor
    CHECK_EQUAL(T(1), etl::private_math::floor(T(1.5)));
    CHECK_EQUAL(T(2), etl::private_math::floor(T(2)));
    CHECK_EQUAL(T(-2), etl::private_math::floor(T(-1.5)));
    CHECK_EQUAL(T(-1), etl::private_math::floor(T(-0.5)));
    CHECK_TRUE(etl::private_math::sign_bit(etl::private_math::floor(-T(0))));

    // round: halfway cases round away from zero.
    CHECK_EQUAL(T(3), etl::private_math::round(T(2.5)));
    CHECK_EQUAL(T(-3), etl::private_math::round(T(-2.5)));
    CHECK_EQUAL(T(1), etl::private_math::round(T(0.5)));
    CHECK_EQUAL(T(-1), etl::private_math::round(T(-0.5)));
    CHECK_EQUAL(T(1), etl::private_math::round(T(1.4999)));
    CHECK_TRUE(etl::is_zero(etl::private_math::round(T(-0.4))));
    CHECK_TRUE(etl::private_math::sign_bit(etl::private_math::round(T(-0.4))));

    // log10
    CHECK_EQUAL(T(0), etl::private_math::log10(T(1)));
    CHECK_CLOSE(T(3), etl::private_math::log10(T(1000)), tolerance);
    CHECK_CLOSE(T(-3), etl::private_math::log10(T(0.001)), tolerance);
    CHECK_TRUE(etl::is_infinity(etl::private_math::log10(T(0))));
    CHECK_TRUE(etl::private_math::sign_bit(etl::private_math::log10(T(0))));
    CHECK_TRUE(etl::is_nan(etl::private_math::log10(T(-1))));

    // modf
    T integral   = T(0);
    T fractional = etl::private_math::modf(T(3.75), &integral);
    CHECK_EQUAL(T(3), integral);
    CHECK_EQUAL(T(0.75), fractional);

    fractional = etl::private_math::modf(T(-1.25), &integral);
    CHECK_EQUAL(T(-1), integral);
    CHECK_EQUAL(T(-0.25), fractional);

    fractional = etl::private_math::modf(T(5), &integral);
    CHECK_EQUAL(T(5), integral);
    CHECK_TRUE(etl::is_zero(fractional));
    CHECK_FALSE(etl::private_math::sign_bit(fractional));

    fractional = etl::private_math::modf(-inf, &integral);
    CHECK_TRUE(etl::is_infinity(integral));
    CHECK_TRUE(etl::private_math::sign_bit(integral));
    CHECK_TRUE(etl::is_zero(fractional));
    CHECK_TRUE(etl::private_math::sign_bit(fractional));

    // sign_bit
    CHECK_FALSE(etl::private_math::sign_bit(T(0)));
    CHECK_TRUE(etl::private_math::sign_bit(-T(0)));
    CHECK_FALSE(etl::private_math::sign_bit(T(1)));
    CHECK_TRUE(etl::private_math::sign_bit(T(-1)));
    CHECK_FALSE(etl::private_math::sign_bit(inf));
    CHECK_TRUE(etl::private_math::sign_bit(-inf));
    CHECK_FALSE(etl::private_math::sign_bit(STD_NL::denorm_min()));
    CHECK_TRUE(etl::private_math::sign_bit(-STD_NL::denorm_min()));
    CHECK_FALSE(etl::private_math::sign_bit(std::copysign(STD_NL::quiet_NaN(), T(1))));
    CHECK_TRUE(etl::private_math::sign_bit(std::copysign(STD_NL::quiet_NaN(), T(-1))));
  }

#if ETL_MATH_USING_BUILTINS
  // The builtin based classification functions must remain usable in constant expressions.
  static_assert(etl::is_nan(__builtin_nan("")), "is_nan must be constexpr");
  static_assert(!etl::is_nan(1.0), "is_nan must be constexpr");
  static_assert(etl::is_infinity(etl::numeric_limits<double>::infinity()), "is_infinity must be constexpr");
  static_assert(etl::is_infinity(-etl::numeric_limits<float>::infinity()), "is_infinity must be constexpr");
  static_assert(!etl::is_infinity(etl::numeric_limits<long double>::max()), "is_infinity must be constexpr");
  static_assert(etl::is_zero(-0.0), "is_zero must be constexpr");
  static_assert(!etl::is_zero(etl::numeric_limits<double>::min()), "is_zero must be constexpr");
#endif

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
    TEST(test_classification_of_special_values_float)
    {
      check_classification_of_special_values<float>();
    }

    //*************************************************************************
    TEST(test_classification_of_special_values_double)
    {
      check_classification_of_special_values<double>();
    }

    //*************************************************************************
    TEST(test_classification_of_special_values_long_double)
    {
      check_classification_of_special_values<long double>();
    }

    //*************************************************************************
    TEST(test_private_math_functions_float)
    {
      check_private_math_functions<float>();
    }

    //*************************************************************************
    TEST(test_private_math_functions_double)
    {
      check_private_math_functions<double>();
    }

    //*************************************************************************
    TEST(test_private_math_functions_long_double)
    {
      check_private_math_functions<long double>();
    }
  }
} // namespace
