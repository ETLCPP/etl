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

// The ETL only needs floating point classification and a few elementary functions
// (sqrt, pow, floor, ceil, round, log10, modf) from <math.h>.
// GCC and clang provide all of them as compiler builtins, so <math.h> is not required at all.
// This matters for ETL_NO_STL builds: libstdc++'s <math.h> (GCC 16) includes <cmath>, which in
// turn drags in large parts of the standard library (<string> etc.), defeating the point of a
// no-STL build. clang picks up the same <math.h> when using libstdc++.
// Define ETL_FORCE_INCLUDE_MATH_H to opt out and use the environment's <math.h>.
#if ETL_NOT_USING_STL && (defined(ETL_COMPILER_GCC) || defined(ETL_COMPILER_CLANG)) && !defined(ETL_FORCE_INCLUDE_MATH_H)
  #define ETL_MATH_USING_BUILTINS 1
#else
  #define ETL_MATH_USING_BUILTINS 0
#endif

// STL builds use <cmath>, which only guarantees the functions in namespace std.
// C++03 <cmath> lacks round, signbit and fpclassify, so C++03 keeps using <math.h>.
#if !ETL_MATH_USING_BUILTINS && ETL_USING_STL && ETL_USING_CPP11
  #define ETL_MATH_USING_STD_CMATH 1
#else
  #define ETL_MATH_USING_STD_CMATH 0
#endif

#include <float.h>
// When using the builtins, <math.h> is not included and its macros (NAN, INFINITY, HUGE_VAL etc.)
// are deliberately not defined here, as they would clash with a <math.h> included later.
#if ETL_MATH_USING_STD_CMATH
  #include <cmath>
#elif !ETL_MATH_USING_BUILTINS
  #include <math.h>
#endif

#include "limits.h"
#include "type_traits.h"

namespace etl
{
  //***************************************************************************
  // is_nan
  //***************************************************************************
#if ETL_MATH_USING_BUILTINS
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_nan(T value)
  {
    return __builtin_isnan(value);
  }
#elif ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_nan(T value)
  {
  #if ETL_MATH_USING_STD_CMATH
    return std::fpclassify(value) == FP_NAN;
  #else
    return fpclassify(value) == FP_NAN;
  #endif
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
#if ETL_MATH_USING_BUILTINS
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_infinity(T value)
  {
    return __builtin_isinf(value);
  }
#elif ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_infinity(T value)
  {
  #if ETL_MATH_USING_STD_CMATH
    return std::fpclassify(value) == FP_INFINITE;
  #else
    return fpclassify(value) == FP_INFINITE;
  #endif
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
#if ETL_MATH_USING_BUILTINS
  // FP_ZERO means 'exactly zero', which is precisely what '== 0' tests for.
  #include "private/diagnostic_float_equal_push.h"
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_zero(T value)
  {
    return value == 0;
  }
  #include "private/diagnostic_pop.h"
#elif ETL_USING_CPP11 && !defined(ETL_NO_CPP_NAN_SUPPORT)
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, bool>::type is_zero(T value)
  {
  #if ETL_MATH_USING_STD_CMATH
    return std::fpclassify(value) == FP_ZERO;
  #else
    return fpclassify(value) == FP_ZERO;
  #endif
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
  // Selects the implementation of a C math function for float, double and long double.
  // Either the compiler builtins, the overloads from <cmath> or the C functions from <math.h>.
  //***************************************************************************
#if ETL_MATH_USING_BUILTINS
  #define ETL_MATH_FUNCTION_F(name) __builtin_##name##f
  #define ETL_MATH_FUNCTION_D(name) __builtin_##name
  #define ETL_MATH_FUNCTION_L(name) __builtin_##name##l
#elif ETL_MATH_USING_STD_CMATH
  #define ETL_MATH_FUNCTION_F(name) std::name
  #define ETL_MATH_FUNCTION_D(name) std::name
  #define ETL_MATH_FUNCTION_L(name) std::name
#else
  #define ETL_MATH_FUNCTION_F(name) ::name##f
  #define ETL_MATH_FUNCTION_D(name) ::name
  #define ETL_MATH_FUNCTION_L(name) ::name##l
#endif

  //***************************************************************************
  // These functions wrap the C math.h functions.
  // The functions are overloaded for the different floating point types.
  // They provide a consistent interface for float, double and long double.
  // They are in the `math` namespace to avoid conflicts with the C library and ETL functions.
  // These take the place of std::floor, std::ceil etc which may not be available `No STL` environments.
  // For No-STL GCC/clang builds they use the compiler builtins, so <math.h> is not required.
  // Avoids	unpredictable behaviour for global calls of ::floor, ::ceil etc when both <math.h> and <cmath> are included.
  //***************************************************************************
  namespace math
  {
    //***************************************************************************
    // floor
    //***************************************************************************
    ETL_NODISCARD
    inline float floor(float value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_F(floor)(value);
    }

    //*********************************
    ETL_NODISCARD
    inline double floor(double value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_D(floor)(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
    inline long double floor(long double value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_L(floor)(value);
    }
#else
    //*********************************
    ETL_NODISCARD
    inline long double floor(long double value) ETL_NOEXCEPT
    {
      return static_cast<long double>(ETL_MATH_FUNCTION_D(floor)(static_cast<double>(value)));
    }
#endif

    //***************************************************************************
    // ceil
    //***************************************************************************
    ETL_NODISCARD
    inline float ceil(float value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_F(ceil)(value);
    }

    //*********************************
    ETL_NODISCARD
    inline double ceil(double value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_D(ceil)(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
    inline long double ceil(long double value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_L(ceil)(value);
    }
#else
    //*********************************
    ETL_NODISCARD
    inline long double ceil(long double value) ETL_NOEXCEPT
    {
      return static_cast<long double>(ETL_MATH_FUNCTION_D(ceil)(static_cast<double>(value)));
    }
#endif

    //***************************************************************************
    // log10
    //***************************************************************************
    ETL_NODISCARD
    inline float log10(float value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_F(log10)(value);
    }

    //*********************************
    ETL_NODISCARD
    inline double log10(double value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_D(log10)(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
    inline long double log10(long double value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_L(log10)(value);
    }
#else
    //*********************************
    ETL_NODISCARD
    inline long double log10(long double value) ETL_NOEXCEPT
    {
      return static_cast<long double>(ETL_MATH_FUNCTION_D(log10)(static_cast<double>(value)));
    }
#endif

    //***************************************************************************
    // round
    //***************************************************************************
    ETL_NODISCARD
    inline float round(float value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_F(round)(value);
    }

    //*********************************
    ETL_NODISCARD
    inline double round(double value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_D(round)(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
    inline long double round(long double value) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_L(round)(value);
    }
#else
    //*********************************
    ETL_NODISCARD
    inline long double round(long double value) ETL_NOEXCEPT
    {
      return static_cast<long double>(ETL_MATH_FUNCTION_D(round)(static_cast<double>(value)));
    }
#endif

    //***************************************************************************
    // power
    //***************************************************************************
    ETL_NODISCARD
    inline float pow(float base, float exponent) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_F(pow)(base, exponent);
    }

    //*********************************
    ETL_NODISCARD
    inline double pow(double base, double exponent) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_D(pow)(base, exponent);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    ETL_NODISCARD
    inline long double pow(long double base, long double exponent) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_L(pow)(base, exponent);
    }
#else
    //*********************************
    ETL_NODISCARD
    inline long double pow(long double base, long double exponent) ETL_NOEXCEPT
    {
      return static_cast<long double>(ETL_MATH_FUNCTION_D(pow)(static_cast<double>(base), static_cast<double>(exponent)));
    }
#endif

    //***************************************************************************
    // modf
    //***************************************************************************
    inline float modf(float value, float* intpart) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_F(modf)(value, intpart);
    }

    //*********************************
    inline double modf(double value, double* intpart) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_D(modf)(value, intpart);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    inline long double modf(long double value, long double* intpart) ETL_NOEXCEPT
    {
      return ETL_MATH_FUNCTION_L(modf)(value, intpart);
    }
#else
    //*********************************
    inline long double modf(long double value, long double* intpart) ETL_NOEXCEPT
    {
      double d_iptr;
      double result = ETL_MATH_FUNCTION_D(modf)(static_cast<double>(value), &d_iptr);
      *intpart      = static_cast<long double>(d_iptr);

      return static_cast<long double>(result);
    }
#endif
  } // namespace math

  namespace private_math
  {
    //***************************************************************************
    // Functions used internally by the ETL.
    //***************************************************************************
    inline float sqrt(float value)
    {
      return ETL_MATH_FUNCTION_F(sqrt)(value);
    }

    //*********************************
    inline double sqrt(double value)
    {
      return ETL_MATH_FUNCTION_D(sqrt)(value);
    }

#if ETL_USING_LONG_DOUBLE_MATH
    //*********************************
    inline long double sqrt(long double value)
    {
      return ETL_MATH_FUNCTION_L(sqrt)(value);
    }
#else
    //*********************************
    inline long double sqrt(long double value)
    {
      return static_cast<long double>(ETL_MATH_FUNCTION_D(sqrt)(static_cast<double>(value)));
    }
#endif

    //*********************************
    template <typename T>
    bool sign_bit(T value)
    {
#if ETL_MATH_USING_BUILTINS
      return __builtin_signbit(value);
#elif ETL_MATH_USING_STD_CMATH
      return std::signbit(value);
#else
      return signbit(value);
#endif
    }
  } // namespace private_math

#undef ETL_MATH_FUNCTION_F
#undef ETL_MATH_FUNCTION_D
#undef ETL_MATH_FUNCTION_L
} // namespace etl

#endif
