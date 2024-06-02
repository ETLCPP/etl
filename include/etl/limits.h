///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#ifndef ETL_LIMITS_INCLUDED
#define ETL_LIMITS_INCLUDED

#include "platform.h"
#include "type_traits.h"
#include "char_traits.h"
#include "integral_limits.h"

#if ETL_NOT_USING_STL && defined(ETL_COMPILER_ARM5) && !defined(__USE_C99_MATH)
  // Required for nan, nanf, nanl
  #define __USE_C99_MATH
#endif

#include <limits.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

#include "private/minmax_push.h"

#if defined(ETL_COMPILER_MICROSOFT)
  #pragma warning(push)
  #pragma warning(disable : 26812)
#endif

#if ETL_NOT_USING_STL
  #define ETL_LOG10_OF_2(x) (((x) * 301) / 1000)

#if !defined(LDBL_MIN) && defined(DBL_MIN)
  // Looks like we don't have these macros defined.
  // That probably means that 'long double' is the same size as 'double'.
  #define LDBL_MIN        DBL_MIN
  #define LDBL_MAX        DBL_MAX
  #define LDBL_EPSILON    DBL_EPSILON
  #define LDBL_MANT_DIG   DBL_MANT_DIG
  #define LDBL_DIG        DBL_DIG
  #define LDBL_MIN_EXP    DBL_MIN_EXP
  #define LDBL_MIN_10_EXP DBL_MIN_10_EXP
  #define LDBL_MAX_EXP    DBL_MAX_EXP
  #define LDBL_MAX_10_EXP DBL_MAX_10_EXP
#endif

#if !defined(HUGE_VAL)
  // Looks like we don't have these macros defined.
  // They're compiler implementation dependent, so we'll make them the same as the max values.
  #define HUGE_VALF FLT_MAX
  #define HUGE_VAL  DBL_MAX
  #define HUGE_VALL LDBL_MAX
#endif

#if defined(ETL_NO_CPP_NAN_SUPPORT)
  #if defined(NAN)
  #include "private/diagnostic_useless_cast_push.h"
    #define ETL_NANF    NAN
    #define ETL_NAN     static_cast<double>(NAN)
    #define ETL_NANL    static_cast<long double>(NAN)
    #define ETL_HAS_NAN true
  #include "private/diagnostic_pop.h"
  #else
  #include "private/diagnostic_useless_cast_push.h"
    #define ETL_NANF HUGE_VALF    
    #define ETL_NAN  HUGE_VAL
    #define ETL_NANL HUGE_VALL
    #define ETL_HAS_NAN false
  #include "private/diagnostic_pop.h"
  #endif
#else
  #define ETL_NANF    nanf("")
  #define ETL_NAN     nan("")
  #define ETL_NANL    nanl("")
  #define ETL_HAS_NAN true
#endif

namespace etl
{
  enum float_round_style
  {
    round_indeterminate       = -1,
    round_toward_zero         = 0,
    round_to_nearest          = 1,
    round_toward_infinity     = 2,
    round_toward_neg_infinity = 3,
  };

  enum float_denorm_style
  {
    denorm_indeterminate = -1,
    denorm_absent        = 0,
    denorm_present       = 1
  };

  namespace private_limits
  {
    //*********************************
    // Integral limits common
    template <typename T = void>
    class integral_limits_common
    {
    public:

      static ETL_CONSTANT bool is_specialized    = true;
      static ETL_CONSTANT bool is_integer        = true;
      static ETL_CONSTANT bool is_exact          = true;
      static ETL_CONSTANT int  max_digits10      = 0;
      static ETL_CONSTANT int  radix             = 2;
      static ETL_CONSTANT int  min_exponent      = 0;
      static ETL_CONSTANT int  min_exponent10    = 0;
      static ETL_CONSTANT int  max_exponent      = 0;
      static ETL_CONSTANT int  max_exponent10    = 0;
      static ETL_CONSTANT bool has_infinity      = false;
      static ETL_CONSTANT bool has_quiet_NaN     = false;
      static ETL_CONSTANT bool has_signaling_NaN = false;
      static ETL_CONSTANT bool has_denorm_loss   = false;
      static ETL_CONSTANT bool is_iec559         = false;
      static ETL_CONSTANT bool is_bounded        = true;
      static ETL_CONSTANT bool traps             = false;
      static ETL_CONSTANT bool tinyness_before   = false;
      static ETL_CONSTANT float_denorm_style has_denorm  = denorm_absent;
      static ETL_CONSTANT float_round_style  round_style = round_toward_zero;
    };

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::is_specialized;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::is_integer;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::is_exact;

    template <typename T>
    ETL_CONSTANT int  integral_limits_common<T>::max_digits10;

    template <typename T>
    ETL_CONSTANT int  integral_limits_common<T>::radix;

    template <typename T>
    ETL_CONSTANT int  integral_limits_common<T>::min_exponent;

    template <typename T>
    ETL_CONSTANT int  integral_limits_common<T>::min_exponent10;

    template <typename T>
    ETL_CONSTANT int  integral_limits_common<T>::max_exponent;

    template <typename T>
    ETL_CONSTANT int  integral_limits_common<T>::max_exponent10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::has_infinity;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::has_quiet_NaN;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::has_signaling_NaN;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::has_denorm_loss;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::is_iec559;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::is_bounded;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::traps;

    template <typename T>
    ETL_CONSTANT bool integral_limits_common<T>::tinyness_before;

    template <typename T>
    ETL_CONSTANT float_denorm_style integral_limits_common<T>::has_denorm;

    template <typename T>
    ETL_CONSTANT float_round_style integral_limits_common<T>::round_style;

    //*********************************
    // bool
    template <typename T = void>
    struct integral_limits_bool
    {
      static ETL_CONSTANT int  digits    = 1;
      static ETL_CONSTANT int  digits10  = 0;
      static ETL_CONSTANT bool is_signed = false;
      static ETL_CONSTANT bool is_modulo = false;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_bool<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_bool<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_bool<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_bool<T>::is_modulo;

    //*********************************
    // char
    template <typename T = void>
    struct integral_limits_char
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(char)) - (etl::is_signed<char>::value ? 1 : 0);
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = etl::is_signed<char>::value;
      static ETL_CONSTANT bool is_modulo = etl::is_unsigned<char>::value;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_char<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_char<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_char<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_char<T>::is_modulo;

    //*********************************
    // unsigned char
    template <typename T = void>
    struct integral_limits_unsigned_char
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(unsigned char));
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = false;
      static ETL_CONSTANT bool is_modulo = true;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_char<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_char<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_char<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_char<T>::is_modulo;

    //*********************************
    // signed char
    template <typename T = void>
    struct integral_limits_signed_char
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(char)) - 1;
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = true;
      static ETL_CONSTANT bool is_modulo = false;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_signed_char<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_signed_char<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_signed_char<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_signed_char<T>::is_modulo;

#if ETL_HAS_NATIVE_CHAR8_T
    //*********************************
    // char8_t
    template <typename T = void>
    struct integral_limits_char8_t
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(char8_t)) - (etl::is_signed<char8_t>::value ? 1 : 0);
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = etl::is_signed<char8_t>::value;
      static ETL_CONSTANT bool is_modulo = false;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_char8_t<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_char8_t<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_char8_t<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_char8_t<T>::is_modulo;
#endif

#if ETL_HAS_NATIVE_CHAR16_T
    //*********************************
    // char16_t
    template <typename T = void>
    struct integral_limits_char16_t
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(char16_t));
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = false;
      static ETL_CONSTANT bool is_modulo = true;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_char16_t<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_char16_t<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_char16_t<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_char16_t<T>::is_modulo;
#endif

#if ETL_HAS_NATIVE_CHAR32_T
    //*********************************
    // char32_t
    template <typename T = void>
    struct integral_limits_char32_t
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(char32_t));
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = false;
      static ETL_CONSTANT bool is_modulo = true;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_char32_t<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_char32_t<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_char32_t<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_char32_t<T>::is_modulo;
#endif

    //*********************************
    // wchar_t
    template <typename T = void>
    struct integral_limits_wchar_t
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(wchar_t)) - (etl::is_signed<wchar_t>::value ? 1 : 0);
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = etl::is_signed<wchar_t>::value;
      static ETL_CONSTANT bool is_modulo = etl::is_unsigned<wchar_t>::value;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_wchar_t<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_wchar_t<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_wchar_t<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_wchar_t<T>::is_modulo;

    //*********************************
    // short
    template <typename T = void>
    struct integral_limits_short
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(short)) - 1;
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = true;
      static ETL_CONSTANT bool is_modulo = false;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_short<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_short<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_short<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_short<T>::is_modulo;

    //*********************************
    // unsigned short
    template <typename T = void>
    struct integral_limits_unsigned_short
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(unsigned short));
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = false;
      static ETL_CONSTANT bool is_modulo = true;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_short<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_short<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_short<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_short<T>::is_modulo;

    //*********************************
    // int
    template <typename T = void>
    struct integral_limits_int
    {
      static ETL_CONSTANT int digits     = (CHAR_BIT * sizeof(int)) - 1;
      static ETL_CONSTANT int digits10   = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = true;
      static ETL_CONSTANT bool is_modulo = false;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_int<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_int<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_int<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_int<T>::is_modulo;

    //*********************************
    // unsigned int
    template <typename T = void>
    struct integral_limits_unsigned_int
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(unsigned int));
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = false;
      static ETL_CONSTANT bool is_modulo = true;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_int<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_int<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_int<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_int<T>::is_modulo;

    //*********************************
    // long
    template <typename T = void>
    struct integral_limits_long
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(long)) - 1;
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = true;
      static ETL_CONSTANT bool is_modulo = false;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_long<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_long<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_long<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_long<T>::is_modulo;

    //*********************************
    // unsigned long
    template <typename T = void>
    struct integral_limits_unsigned_long
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(unsigned long));
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = false;
      static ETL_CONSTANT bool is_modulo = true;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_long<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_long<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_long<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_long<T>::is_modulo;

    //*********************************
    // long long
    template <typename T = void>
    struct integral_limits_long_long
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(long long)) - 1;
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = true;
      static ETL_CONSTANT bool is_modulo = false;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_long_long<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_long_long<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_long_long<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_long_long<T>::is_modulo;

    //*********************************
    // unsigned long long
    template <typename T = void>
    struct integral_limits_unsigned_long_long
    {
      static ETL_CONSTANT int  digits    = (CHAR_BIT * sizeof(unsigned long long));
      static ETL_CONSTANT int  digits10  = ETL_LOG10_OF_2(digits);
      static ETL_CONSTANT bool is_signed = false;
      static ETL_CONSTANT bool is_modulo = true;
    };

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_long_long<T>::digits;

    template <typename T>
    ETL_CONSTANT int integral_limits_unsigned_long_long<T>::digits10;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_long_long<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool integral_limits_unsigned_long_long<T>::is_modulo;

    //*********************************
    // Floating point limits common
    template <typename T = void>
    class floating_point_limits_common
    {
    public:

      static ETL_CONSTANT bool is_specialized    = true;
      static ETL_CONSTANT bool is_signed         = true;
      static ETL_CONSTANT bool is_integer        = false;
      static ETL_CONSTANT bool is_exact          = false;
      static ETL_CONSTANT int  radix             = 2;
      static ETL_CONSTANT bool has_infinity      = true;
      static ETL_CONSTANT bool has_quiet_NaN     = ETL_HAS_NAN;
      static ETL_CONSTANT bool has_signaling_NaN = ETL_HAS_NAN;
      static ETL_CONSTANT bool has_denorm_loss   = false;
      static ETL_CONSTANT bool is_iec559         = false;
      static ETL_CONSTANT bool is_bounded        = true;
      static ETL_CONSTANT bool is_modulo         = false;
      static ETL_CONSTANT bool traps             = false;
      static ETL_CONSTANT bool tinyness_before   = false;
      static ETL_CONSTANT float_denorm_style has_denorm  = denorm_indeterminate;
      static ETL_CONSTANT float_round_style  round_style = round_indeterminate;
    };

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::is_specialized;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::is_signed;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::is_integer;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::is_exact;

    template <typename T>
    ETL_CONSTANT int  floating_point_limits_common<T>::radix;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::has_infinity;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::has_quiet_NaN;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::has_signaling_NaN;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::has_denorm_loss;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::is_iec559;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::is_bounded;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::is_modulo;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::traps;

    template <typename T>
    ETL_CONSTANT bool floating_point_limits_common<T>::tinyness_before;

    template <typename T>
    ETL_CONSTANT float_denorm_style floating_point_limits_common<T>::has_denorm;

    template <typename T>
    ETL_CONSTANT float_round_style floating_point_limits_common<T>::round_style;

    //*********************************
    // float
    template <typename T = void>
    struct floating_point_limits_float
    {
      static ETL_CONSTANT int digits         = FLT_MANT_DIG;
      static ETL_CONSTANT int digits10       = FLT_DIG;
      static ETL_CONSTANT int max_digits10   = ETL_LOG10_OF_2(FLT_MANT_DIG) + 2;

      static ETL_CONSTANT int min_exponent   = FLT_MIN_EXP;
      static ETL_CONSTANT int min_exponent10 = FLT_MIN_10_EXP;
      static ETL_CONSTANT int max_exponent   = FLT_MAX_EXP;
      static ETL_CONSTANT int max_exponent10 = FLT_MAX_10_EXP;
    };

    template <typename T>
    ETL_CONSTANT int floating_point_limits_float<T>::digits;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_float<T>::digits10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_float<T>::max_digits10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_float<T>::min_exponent;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_float<T>::min_exponent10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_float<T>::max_exponent;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_float<T>::max_exponent10;

    //*********************************
    // double
    template <typename T = void>
    struct floating_point_limits_double
    {
      static ETL_CONSTANT int digits         = DBL_MANT_DIG;
      static ETL_CONSTANT int digits10       = DBL_DIG;
      static ETL_CONSTANT int max_digits10   = ETL_LOG10_OF_2(DBL_MANT_DIG) + 2;

      static ETL_CONSTANT int min_exponent   = DBL_MIN_EXP;
      static ETL_CONSTANT int min_exponent10 = DBL_MIN_10_EXP;
      static ETL_CONSTANT int max_exponent   = DBL_MAX_EXP;
      static ETL_CONSTANT int max_exponent10 = DBL_MAX_10_EXP;
    };

    template <typename T>
    ETL_CONSTANT int floating_point_limits_double<T>::digits;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_double<T>::digits10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_double<T>::max_digits10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_double<T>::min_exponent;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_double<T>::min_exponent10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_double<T>::max_exponent;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_double<T>::max_exponent10;

    //*********************************
    // long double
    template <typename T = void>
    struct floating_point_limits_long_double
    {
      static ETL_CONSTANT int digits         = LDBL_MANT_DIG;
      static ETL_CONSTANT int digits10       = LDBL_DIG;
      static ETL_CONSTANT int max_digits10   = ETL_LOG10_OF_2(LDBL_MANT_DIG) + 2;

      static ETL_CONSTANT int min_exponent   = LDBL_MIN_EXP;
      static ETL_CONSTANT int min_exponent10 = LDBL_MIN_10_EXP;
      static ETL_CONSTANT int max_exponent   = LDBL_MAX_EXP;
      static ETL_CONSTANT int max_exponent10 = LDBL_MAX_10_EXP;
    };

    template <typename T>
    ETL_CONSTANT int floating_point_limits_long_double<T>::digits;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_long_double<T>::digits10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_long_double<T>::max_digits10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_long_double<T>::min_exponent;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_long_double<T>::min_exponent10;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_long_double<T>::max_exponent;

    template <typename T>
    ETL_CONSTANT int floating_point_limits_long_double<T>::max_exponent10;
  }

  //***************************************************************************
  // Default
  template <typename T>
  class numeric_limits;

  template <typename T>
  class numeric_limits<const T> : public numeric_limits<T> { };

  template <typename T>
  class numeric_limits<volatile T> : public numeric_limits<T> { };

  template <typename T>
  class numeric_limits<const volatile T> : public numeric_limits<T> { };

  //***********************************
  // bool
  template<>
  class numeric_limits<bool> : public private_limits::integral_limits_common<>, 
                               public private_limits::integral_limits_bool<>
  {
  public:

    static ETL_CONSTEXPR bool min() { return false; }
    static ETL_CONSTEXPR bool max() { return true; }
    static ETL_CONSTEXPR bool lowest() { return false; }
    static ETL_CONSTEXPR bool epsilon() { return false; }
    static ETL_CONSTEXPR bool round_error() { return false; }
    static ETL_CONSTEXPR bool denorm_min() { return false; }
    static ETL_CONSTEXPR bool infinity() { return false; }
    static ETL_CONSTEXPR bool quiet_NaN() { return false; }
    static ETL_CONSTEXPR bool signaling_NaN() { return false; }
  };

  //***************************************************************************
  // char
  template<>
  class numeric_limits<char> : public private_limits::integral_limits_common<>,
                               public private_limits::integral_limits_char<>
  {
  public:

    static ETL_CONSTEXPR char min() { return char(CHAR_MIN); }
    static ETL_CONSTEXPR char max() { return char(CHAR_MAX); }
    static ETL_CONSTEXPR char lowest() { return char(CHAR_MIN); }
    static ETL_CONSTEXPR char epsilon() { return 0; }
    static ETL_CONSTEXPR char round_error() { return 0; }
    static ETL_CONSTEXPR char denorm_min() { return 0; }
    static ETL_CONSTEXPR char infinity() { return 0; }
    static ETL_CONSTEXPR char quiet_NaN() { return 0; }
    static ETL_CONSTEXPR char signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned char
  template<>
  class numeric_limits<unsigned char> : public private_limits::integral_limits_common<>,
                                        public private_limits::integral_limits_unsigned_char<>
  {
  public:

    static ETL_CONSTEXPR unsigned char min() { return 0U; }
    static ETL_CONSTEXPR unsigned char max() { return UCHAR_MAX; }
    static ETL_CONSTEXPR unsigned char lowest() { return 0U; }
    static ETL_CONSTEXPR unsigned char epsilon() { return 0U; }
    static ETL_CONSTEXPR unsigned char round_error() { return 0U; }
    static ETL_CONSTEXPR unsigned char denorm_min() { return 0U; }
    static ETL_CONSTEXPR unsigned char infinity() { return 0U; }
    static ETL_CONSTEXPR unsigned char quiet_NaN() { return 0U; }
    static ETL_CONSTEXPR unsigned char signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // signed char
  template<>
  class numeric_limits<signed char> : public private_limits::integral_limits_common<>,
                                      public private_limits::integral_limits_signed_char<>
  {
  public:

    static ETL_CONSTEXPR signed char min() { return SCHAR_MIN; }
    static ETL_CONSTEXPR signed char max() { return SCHAR_MAX; }
    static ETL_CONSTEXPR signed char lowest() { return SCHAR_MIN; }
    static ETL_CONSTEXPR signed char epsilon() { return 0; }
    static ETL_CONSTEXPR signed char round_error() { return 0; }
    static ETL_CONSTEXPR signed char denorm_min() { return 0; }
    static ETL_CONSTEXPR signed char infinity() { return 0; }
    static ETL_CONSTEXPR signed char quiet_NaN() { return 0; }
    static ETL_CONSTEXPR signed char signaling_NaN() { return 0; }
  };

#if ETL_HAS_NATIVE_CHAR8_T
  //***************************************************************************
  // char8_t
  template<>
  class numeric_limits<char8_t> : public private_limits::integral_limits_common<>,
                                  public private_limits::integral_limits_char8_t<>
  {
  public:
    
    static ETL_CONSTEXPR char8_t min() { return char8_t(CHAR_MIN); }
    static ETL_CONSTEXPR char8_t max() { return char8_t(CHAR_MAX); }
    static ETL_CONSTEXPR char8_t lowest() { return char8_t(CHAR_MIN); }
    static ETL_CONSTEXPR char8_t epsilon() { return 0; }
    static ETL_CONSTEXPR char8_t round_error() { return 0; }
    static ETL_CONSTEXPR char8_t denorm_min() { return 0; }
    static ETL_CONSTEXPR char8_t infinity() { return 0; }
    static ETL_CONSTEXPR char8_t quiet_NaN() { return 0; }
    static ETL_CONSTEXPR char8_t signaling_NaN() { return 0; }
  };
#endif

#if ETL_HAS_NATIVE_CHAR16_T
  //***************************************************************************
  // char16_t
  template<>
  class numeric_limits<char16_t> : public private_limits::integral_limits_common<>,
                                   public private_limits::integral_limits_char16_t<>
  {
  public:

    static ETL_CONSTEXPR char16_t min() { return 0U; }
    static ETL_CONSTEXPR char16_t max() { return UINT_LEAST16_MAX; }
    static ETL_CONSTEXPR char16_t lowest() { return 0U; }
    static ETL_CONSTEXPR char16_t epsilon() { return 0U; }
    static ETL_CONSTEXPR char16_t round_error() { return 0U; }
    static ETL_CONSTEXPR char16_t denorm_min() { return 0U; }
    static ETL_CONSTEXPR char16_t infinity() { return 0U; }
    static ETL_CONSTEXPR char16_t quiet_NaN() { return 0U; }
    static ETL_CONSTEXPR char16_t signaling_NaN() { return 0U; }
  };
#endif

#if ETL_HAS_NATIVE_CHAR32_T
  //***************************************************************************
  // char32_t
  template<>
  class numeric_limits<char32_t> : public private_limits::integral_limits_common<>,
                                   public private_limits::integral_limits_char32_t<>
  {
  public:

    static ETL_CONSTEXPR char32_t min() { return 0U; }
    static ETL_CONSTEXPR char32_t max() { return UINT_LEAST32_MAX; }
    static ETL_CONSTEXPR char32_t lowest() { return 0U; }
    static ETL_CONSTEXPR char32_t epsilon() { return 0U; }
    static ETL_CONSTEXPR char32_t round_error() { return 0U; }
    static ETL_CONSTEXPR char32_t denorm_min() { return 0U; }
    static ETL_CONSTEXPR char32_t infinity() { return 0U; }
    static ETL_CONSTEXPR char32_t quiet_NaN() { return 0U; }
    static ETL_CONSTEXPR char32_t signaling_NaN() { return 0U; }
  };
#endif

  //***************************************************************************
  // wchar_t
  template<>
  class numeric_limits<wchar_t> : public private_limits::integral_limits_common<>,
                                  public private_limits::integral_limits_wchar_t<>
  {
  public:

    static ETL_CONSTEXPR wchar_t min() { return WCHAR_MIN; }
    static ETL_CONSTEXPR wchar_t max() { return WCHAR_MAX; }
    static ETL_CONSTEXPR wchar_t lowest() { return WCHAR_MIN; }
    static ETL_CONSTEXPR wchar_t epsilon() { return wchar_t(0); }
    static ETL_CONSTEXPR wchar_t round_error() { return wchar_t(0); }
    static ETL_CONSTEXPR wchar_t denorm_min() { return wchar_t(0); }
    static ETL_CONSTEXPR wchar_t infinity() { return wchar_t(0); }
    static ETL_CONSTEXPR wchar_t quiet_NaN() { return wchar_t(0); }
    static ETL_CONSTEXPR wchar_t signaling_NaN() { return wchar_t(0); }
  };

  //***************************************************************************
  // short
  template<>
  class numeric_limits<short> : public private_limits::integral_limits_common<>,
                                public private_limits::integral_limits_short<>
  {
  public:

    static ETL_CONSTEXPR short min() { return SHRT_MIN; }
    static ETL_CONSTEXPR short max() { return SHRT_MAX; }
    static ETL_CONSTEXPR short lowest() { return SHRT_MIN; }
    static ETL_CONSTEXPR short epsilon() { return 0; }
    static ETL_CONSTEXPR short round_error() { return 0; }
    static ETL_CONSTEXPR short denorm_min() { return 0; }
    static ETL_CONSTEXPR short infinity() { return 0; }
    static ETL_CONSTEXPR short quiet_NaN() { return 0; }
    static ETL_CONSTEXPR short signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned short
  template<>
  class numeric_limits<unsigned short> : public private_limits::integral_limits_common<>,
                                         public private_limits::integral_limits_unsigned_short<>
  {
  public:

    static ETL_CONSTEXPR unsigned short min() { return 0U; }
    static ETL_CONSTEXPR unsigned short max() { return USHRT_MAX; }
    static ETL_CONSTEXPR unsigned short lowest() { return 0U; }
    static ETL_CONSTEXPR unsigned short epsilon() { return 0U; }
    static ETL_CONSTEXPR unsigned short round_error() { return 0U; }
    static ETL_CONSTEXPR unsigned short denorm_min() { return 0U; }
    static ETL_CONSTEXPR unsigned short infinity() { return 0U; }
    static ETL_CONSTEXPR unsigned short quiet_NaN() { return 0U; }
    static ETL_CONSTEXPR unsigned short signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // int
  template<>
  class numeric_limits<int> : public private_limits::integral_limits_common<>,
                              public private_limits::integral_limits_int<>
  {
  public:

    static ETL_CONSTEXPR int min() { return INT_MIN; }
    static ETL_CONSTEXPR int max() { return INT_MAX; }
    static ETL_CONSTEXPR int lowest() { return INT_MIN; }
    static ETL_CONSTEXPR int epsilon() { return 0; }
    static ETL_CONSTEXPR int round_error() { return 0; }
    static ETL_CONSTEXPR int denorm_min() { return 0; }
    static ETL_CONSTEXPR int infinity() { return 0; }
    static ETL_CONSTEXPR int quiet_NaN() { return 0; }
    static ETL_CONSTEXPR int signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned int
  template<>
  class numeric_limits<unsigned int> : public private_limits::integral_limits_common<>,
                                       public private_limits::integral_limits_unsigned_int<>
  {
  public:

    static ETL_CONSTEXPR unsigned int min() { return 0U; }
    static ETL_CONSTEXPR unsigned int max() { return UINT_MAX; }
    static ETL_CONSTEXPR unsigned int lowest() { return 0U; }
    static ETL_CONSTEXPR unsigned int epsilon() { return 0U; }
    static ETL_CONSTEXPR unsigned int round_error() { return 0U; }
    static ETL_CONSTEXPR unsigned int denorm_min() { return 0U; }
    static ETL_CONSTEXPR unsigned int infinity() { return 0U; }
    static ETL_CONSTEXPR unsigned int quiet_NaN() { return 0U; }
    static ETL_CONSTEXPR unsigned int signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // long
  template<>
  class numeric_limits<long> : public private_limits::integral_limits_common<>,
                               public private_limits::integral_limits_long<>
  {
  public:

    static ETL_CONSTEXPR long min() { return LONG_MIN; }
    static ETL_CONSTEXPR long max() { return LONG_MAX; }
    static ETL_CONSTEXPR long lowest() { return LONG_MIN; }
    static ETL_CONSTEXPR long epsilon() { return 0; }
    static ETL_CONSTEXPR long round_error() { return 0; }
    static ETL_CONSTEXPR long denorm_min() { return 0; }
    static ETL_CONSTEXPR long infinity() { return 0; }
    static ETL_CONSTEXPR long quiet_NaN() { return 0; }
    static ETL_CONSTEXPR long signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned long
  template<>
  class numeric_limits<unsigned long> : public private_limits::integral_limits_common<>,
                                        public private_limits::integral_limits_unsigned_long<>
  {
  public:

    static ETL_CONSTEXPR unsigned long min() { return 0U; }
    static ETL_CONSTEXPR unsigned long max() { return ULONG_MAX; }
    static ETL_CONSTEXPR unsigned long lowest() { return 0U; }
    static ETL_CONSTEXPR unsigned long epsilon() { return 0U; }
    static ETL_CONSTEXPR unsigned long round_error() { return 0U; }
    static ETL_CONSTEXPR unsigned long denorm_min() { return 0U; }
    static ETL_CONSTEXPR unsigned long infinity() { return 0U; }
    static ETL_CONSTEXPR unsigned long quiet_NaN() { return 0U; }
    static ETL_CONSTEXPR unsigned long signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // long long
  template<>
  class numeric_limits<long long> : public private_limits::integral_limits_common<>,
                                    public private_limits::integral_limits_long_long<>
  {
  public:

    static ETL_CONSTEXPR long long min() { return LLONG_MIN; }
    static ETL_CONSTEXPR long long max() { return LLONG_MAX; }
    static ETL_CONSTEXPR long long lowest() { return LLONG_MIN; }
    static ETL_CONSTEXPR long long epsilon() { return 0; }
    static ETL_CONSTEXPR long long round_error() { return 0; }
    static ETL_CONSTEXPR long long denorm_min() { return 0; }
    static ETL_CONSTEXPR long long infinity() { return 0; }
    static ETL_CONSTEXPR long long quiet_NaN() { return 0; }
    static ETL_CONSTEXPR long long signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned long long
  template<>
  class numeric_limits<unsigned long long> : public private_limits::integral_limits_common<>,
                                             public private_limits::integral_limits_unsigned_long_long<>
  {
  public:

    static ETL_CONSTEXPR unsigned long long min() { return 0U; }
    static ETL_CONSTEXPR unsigned long long max() { return ULLONG_MAX; }
    static ETL_CONSTEXPR unsigned long long lowest() { return 0U; }
    static ETL_CONSTEXPR unsigned long long epsilon() { return 0U; }
    static ETL_CONSTEXPR unsigned long long round_error() { return 0U; }
    static ETL_CONSTEXPR unsigned long long denorm_min() { return 0U; }
    static ETL_CONSTEXPR unsigned long long infinity() { return 0U; }
    static ETL_CONSTEXPR unsigned long long quiet_NaN() { return 0U; }
    static ETL_CONSTEXPR unsigned long long signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // float
  template<>
  class numeric_limits<float> : public private_limits::floating_point_limits_common<>,
                                public private_limits::floating_point_limits_float<>
  {
  public:

    static ETL_CONSTEXPR float min() { return FLT_MIN; }
    static ETL_CONSTEXPR float max() { return FLT_MAX; }
    static ETL_CONSTEXPR float lowest() { return -FLT_MAX; }
    static ETL_CONSTEXPR float epsilon() { return FLT_EPSILON; }
    static ETL_CONSTEXPR float denorm_min() { return FLT_MIN; }
    static ETL_CONSTEXPR float infinity() { return HUGE_VALF; }
    static float round_error() { return 0.5f; }
    static float quiet_NaN() { return ETL_NANF; }
    static float signaling_NaN() { return ETL_NANF; }
  };

  //***************************************************************************
  // double
  template<>
  class numeric_limits<double> : public private_limits::floating_point_limits_common<>,
                                 public private_limits::floating_point_limits_double<>
  {
  public:

    static ETL_CONSTEXPR double min() { return DBL_MIN; }
    static ETL_CONSTEXPR double max() { return DBL_MAX; }
    static ETL_CONSTEXPR double lowest() { return -DBL_MAX; }
    static ETL_CONSTEXPR double epsilon() { return DBL_EPSILON; }
    static ETL_CONSTEXPR double denorm_min() { return DBL_MIN; }
    static ETL_CONSTEXPR double infinity() { return HUGE_VAL; }
    static double round_error() { return 0.5; }
    static double quiet_NaN() { return ETL_NAN; }
    static double signaling_NaN() { return ETL_NAN; }
  };

  //***************************************************************************
  // long double
  template<>
  class numeric_limits<long double> : public private_limits::floating_point_limits_common<>,
                                      public private_limits::floating_point_limits_long_double<>
  {
  public:

    static ETL_CONSTEXPR long double min() { return LDBL_MIN; }
    static ETL_CONSTEXPR long double max() { return LDBL_MAX; }
    static ETL_CONSTEXPR long double lowest() { return -LDBL_MAX; }
    static ETL_CONSTEXPR long double epsilon() { return LDBL_EPSILON; }
    static ETL_CONSTEXPR long double denorm_min() { return LDBL_MIN; }
    static ETL_CONSTEXPR long double infinity() { return HUGE_VALL; }
    static long double round_error() { return 0.5L; }
    static long double quiet_NaN() { return ETL_NANL; }
    static long double signaling_NaN() { return ETL_NANL; }
  };
}

#else

#include <limits>

namespace etl
{
  enum float_round_style
  {
    round_indeterminate       = std::round_indeterminate,
    round_toward_zero         = std::round_toward_zero,
    round_to_nearest          = std::round_to_nearest,
    round_toward_infinity     = std::round_toward_infinity,
    round_toward_neg_infinity = std::round_toward_neg_infinity,
  };

  enum float_denorm_style
  {
    denorm_indeterminate = std::denorm_indeterminate,
    denorm_absent        = std::denorm_absent,
    denorm_present       = std::denorm_present
  };

#if ETL_USING_CPP11
  template <typename T>
  using numeric_limits = std::numeric_limits<T>;
#else
  template <typename T>
  class numeric_limits : public std::numeric_limits<T>
  {
  };
#endif
}
#endif

#if defined(ETL_COMPILER_MICROSOFT)
  #pragma warning(pop)
#endif

#include "private/minmax_pop.h"

#endif
