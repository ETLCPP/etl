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
// (sqrt, pow, floor, round, log10, modf) from <math.h>.
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

  namespace private_math
  {
    //***************************************************************************
    // Elementary functions used internally by the ETL.
    // Either the GCC builtins or the functions from <cmath> / <math.h>.
    //***************************************************************************
#if ETL_MATH_USING_BUILTINS
    // clang-format off
    inline float       sqrt(float value)       { return __builtin_sqrtf(value); }
    inline double      sqrt(double value)      { return __builtin_sqrt(value); }
    inline long double sqrt(long double value) { return __builtin_sqrtl(value); }

    inline float       pow(float base, float exp)             { return __builtin_powf(base, exp); }
    inline double      pow(double base, double exp)           { return __builtin_pow(base, exp); }
    inline long double pow(long double base, long double exp) { return __builtin_powl(base, exp); }

    inline float       floor(float value)       { return __builtin_floorf(value); }
    inline double      floor(double value)      { return __builtin_floor(value); }
    inline long double floor(long double value) { return __builtin_floorl(value); }

    inline float       round(float value)       { return __builtin_roundf(value); }
    inline double      round(double value)      { return __builtin_round(value); }
    inline long double round(long double value) { return __builtin_roundl(value); }

    inline float       log10(float value)       { return __builtin_log10f(value); }
    inline double      log10(double value)      { return __builtin_log10(value); }
    inline long double log10(long double value) { return __builtin_log10l(value); }

    inline float       modf(float value, float* iptr)             { return __builtin_modff(value, iptr); }
    inline double      modf(double value, double* iptr)           { return __builtin_modf(value, iptr); }
    inline long double modf(long double value, long double* iptr) { return __builtin_modfl(value, iptr); }
    // clang-format on

    template <typename T>
    bool sign_bit(T value)
    {
      return __builtin_signbit(value);
    }
#elif ETL_MATH_USING_STD_CMATH
    template <typename T>
    T sqrt(T value)
    {
      return static_cast<T>(std::sqrt(value));
    }

    template <typename T>
    T pow(T base, T exp)
    {
      return static_cast<T>(std::pow(base, exp));
    }

    template <typename T>
    T floor(T value)
    {
      return static_cast<T>(std::floor(value));
    }

    template <typename T>
    T round(T value)
    {
      return static_cast<T>(std::round(value));
    }

    template <typename T>
    T log10(T value)
    {
      return static_cast<T>(std::log10(value));
    }

    template <typename T>
    T modf(T value, T* iptr)
    {
      return std::modf(value, iptr);
    }

    template <typename T>
    bool sign_bit(T value)
    {
      return std::signbit(value);
    }
#else
    template <typename T>
    T sqrt(T value)
    {
      return static_cast<T>(::sqrt(value));
    }

    template <typename T>
    T pow(T base, T exp)
    {
      return static_cast<T>(::pow(base, exp));
    }

    template <typename T>
    T floor(T value)
    {
      return static_cast<T>(::floor(value));
    }

    template <typename T>
    T round(T value)
    {
      return static_cast<T>(::round(value));
    }

    template <typename T>
    T log10(T value)
    {
      return static_cast<T>(::log10(value));
    }

    template <typename T>
    T modf(T value, T* iptr)
    {
      return ::modf(value, iptr);
    }

    template <typename T>
    bool sign_bit(T value)
    {
      return signbit(value);
    }
#endif
  } // namespace private_math
} // namespace etl

#endif
