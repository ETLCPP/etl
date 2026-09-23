///\file

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

#ifndef ETL_MATH_INCLUDED
#define ETL_MATH_INCLUDED

#include "platform.h"

#if ETL_NOT_USING_STL && defined(ETL_COMPILER_ARM5) && !defined(__USE_C99_MATH)
  // Required for nan, nanf, nanl
  #define __USE_C99_MATH
#endif

#include <float.h>
#include <math.h>

#include "limits.h"
#include "type_traits.h"

namespace etl
{
  //***************************************************************************
  // is_nan
  //***************************************************************************
#if ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_nan(T value)
  {
    return fpclassify(value) == FP_NAN;
  }
#else
  #include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_nan(T value)
  {
    return (value != value);
  }
  #include "private/diagnostic_pop.h"
#endif

  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_integral<T>::value, bool>::type is_nan(T)
  {
    return false;
  }

  //***************************************************************************
  // is_infinity
  //***************************************************************************
#if ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_infinity(T value)
  {
    return fpclassify(value) == FP_INFINITE;
  }
#else
  #include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_infinity(T value)
  {
    return ((value == etl::numeric_limits<T>::infinity()) || (value == -etl::numeric_limits<T>::infinity()));
  }
  #include "private/diagnostic_pop.h"
#endif

  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_integral<T>::value, bool>::type is_infinity(T)
  {
    return false;
  }

  //***************************************************************************
  // is_zero
  //***************************************************************************
#if ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_zero(T value)
  {
    return fpclassify(value) == FP_ZERO;
  }
#else
  #include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_zero(T value)
  {
    return value == 0;
  }
  #include "private/diagnostic_pop.h"
#endif

  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_integral<T>::value, bool>::type is_zero(T value)
  {
    return (value == 0);
  }

  //***************************************************************************
  // is_exactly_equal
  //***************************************************************************
#include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR bool is_exactly_equal(T value1, T value2)
  {
    return value1 == value2;
  }
#include "private/diagnostic_pop.h"

  //***************************************************************************
  // These functions wrap the C library math functions.
  // The functions are overloaded for the different floating point types.
  // They provide a consistent interface for float, double and long double.
  // They are in the clib namespace to avoid conflicts with the C library and ETL functions.
  // These take the place of std::floor, std::ceil etc which may not be available `No STL` environments.
  // Avoids	unpredictable behaviour for global calls of ::floor, ::ceil etc when both <math.h> and <cmath> are included.
  //***************************************************************************
  namespace clib
  {
    //***************************************************************************
    // floor
    //***************************************************************************
    ETL_NODISCARD
    inline float floor(float value) ETL_NOEXCEPT
    {
      return ::floorf(value);
    }

    //*********************************
    ETL_NODISCARD
    inline double floor(double value) ETL_NOEXCEPT
    {
      return ::floor(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
      inline long double floor(long double value) ETL_NOEXCEPT
    {
      return ::floorl(value);
    }
#else
    //*********************************
    ETL_NODISCARD
      inline long double floor(long double value) ETL_NOEXCEPT
    {
      return static_cast<long double>(::floor(static_cast<double>(value)));
    }
#endif

    //***************************************************************************
    // ceil
    //***************************************************************************
    ETL_NODISCARD
    inline float ceil(float value) ETL_NOEXCEPT
    {
      return ::ceilf(value);
    }

    //*********************************
    ETL_NODISCARD
    inline double ceil(double value) ETL_NOEXCEPT
    {
      return ::ceil(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
      inline long double ceil(long double value) ETL_NOEXCEPT
    {
      return ::ceill(value);
    }
#else
    //*********************************
    ETL_NODISCARD
      inline long double ceil(long double value) ETL_NOEXCEPT
    {
      return static_cast<long double>(::ceil(static_cast<double>(value)));
    }
#endif

    //***************************************************************************
    // log10
    //***************************************************************************
    ETL_NODISCARD
    inline float log10(float value) ETL_NOEXCEPT
    {
      return ::log10f(value);
    }

    //*********************************
    ETL_NODISCARD
    inline double log10(double value) ETL_NOEXCEPT
    {
      return ::log10(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
      inline long double log10(long double value) ETL_NOEXCEPT
    {
      return ::log10l(value);
    }
#else
    //*********************************
    ETL_NODISCARD
      inline long double log10(long double value) ETL_NOEXCEPT
    {
      return static_cast<long double>(::log10(static_cast<double>(value)));
    }
#endif

    //***************************************************************************
    // round
    //***************************************************************************
    ETL_NODISCARD
    inline float round(float value) ETL_NOEXCEPT
    {
      return ::roundf(value);
    }

    //*********************************
    ETL_NODISCARD
    inline double round(double value) ETL_NOEXCEPT
    {
      return ::round(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
      inline long double round(long double value) ETL_NOEXCEPT
    {
      return ::roundl(value);
    }
#else
    //*********************************
    ETL_NODISCARD
      inline long double round(long double value) ETL_NOEXCEPT
    {
      return static_cast<long double>(::round(static_cast<double>(value)));
    }
#endif

    //***************************************************************************
    // power
    //***************************************************************************
    ETL_NODISCARD
    inline float pow(float base, float exponent) ETL_NOEXCEPT
    {
      return ::powf(base, exponent);
    }

    //*********************************
    ETL_NODISCARD
    inline double pow(double base, double exponent) ETL_NOEXCEPT
    {
      return ::pow(base, exponent);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
      inline long double pow(long double base, long double exponent) ETL_NOEXCEPT
    {
      return ::powl(base, exponent);
    }
#else
    //*********************************
    ETL_NODISCARD
      inline long double power(long double base, long double exponent) ETL_NOEXCEPT
    {
      return static_cast<long double>(::pow(static_cast<double>(base), static_cast<double>(exponent)));
    }
#endif

    //***************************************************************************
    // modf
    //***************************************************************************
    ETL_NODISCARD
    inline float modf(float value, float* intpart) ETL_NOEXCEPT
    {
      return ::modff(value, intpart);
    }

    //*********************************
    ETL_NODISCARD
    inline double modf(double value, double* intpart) ETL_NOEXCEPT
    {
      return ::modf(value, intpart);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
      inline long double modf(long double value, long double* intpart) ETL_NOEXCEPT
    {
      return ::modfl(value, intpart);
    }
#else
    //*********************************
    ETL_NODISCARD
      inline long double modf(long double value, long double* intpart) ETL_NOEXCEPT
    {
      double d_iptr;
      double result = ::modf(static_cast<double>(value), &d_iptr);
      *intpart      = static_cast<long double>(d_iptr);

      return static_cast<long double>(result);
    }
#endif
  } // namespace clib
} // namespace etl

#endif
