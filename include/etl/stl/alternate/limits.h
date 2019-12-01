///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#ifndef ETL_STL_ALTERNATE_LIMITS_INCLUDED
#define ETL_STL_ALTERNATE_LIMITS_INCLUDED

#include "../../platform.h"
#include "../../type_traits.h"
#include "../../char_traits.h"
#include "../../integral_limits.h"

#include "../../private/choose_tag_types.h"
#include "../../private/choose_pair_types.h"

#include <limits.h>
#include <stdint.h>
#include <float.h>

#define ETL_LOG2(x) (((x) * 301) / 1000)

namespace etlstd
{
  template<class T> class numeric_limits;

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

  //***************************************************************************
  // Base for integral types.
  template <typename T>
  class etl_integral_type
  {
  public:
    static ETL_CONST bool is_specialized = true;

    static ETL_CONST int max_digits10 = 0;

    static ETL_CONST bool is_integer = true;
    static ETL_CONST bool is_exact   = true;

    static ETL_CONST int radix = 2;
    static ETL_CONSTEXPR T epsilon() { return 0; }
    static ETL_CONSTEXPR T round_error() { return 0; }

    static ETL_CONST int digits   = (CHAR_BIT * sizeof(T)) - (etl::is_signed<T>::value ? 1 : 0);
    static ETL_CONST int digits10 = ETL_LOG2(digits);

    static ETL_CONST bool is_signed = etl::is_signed<T>::value;

    static ETL_CONST int min_exponent   = 0;
    static ETL_CONST int min_exponent10 = 0;
    static ETL_CONST int max_exponent   = 0;
    static ETL_CONST int max_exponent10 = 0;

    static ETL_CONST bool has_infinity             = false;
    static ETL_CONST bool has_quiet_NaN            = false;
    static ETL_CONST bool has_signaling_NaN        = false;
    static ETL_CONST float_denorm_style has_denorm = denorm_absent;
    static ETL_CONST bool has_denorm_loss          = false;

    static ETL_CONSTEXPR T infinity() { return 0; }
    static ETL_CONSTEXPR T quiet_NaN() { return 0; }
    static ETL_CONSTEXPR T signaling_NaN() { return 0; }
    static ETL_CONSTEXPR T denorm_min() { return 0; }

    static ETL_CONST bool is_iec559  = false;
    static ETL_CONST bool is_bounded = true;
    static ETL_CONST bool is_modulo  = etl::is_unsigned<T>::value;

    static ETL_CONST bool traps = false;
    static ETL_CONST bool tinyness_before = false;
    static ETL_CONST float_round_style round_style = round_toward_zero;
  };

  template <typename T> ETL_CONST bool etl_integral_type<T>::is_specialized;
  template <typename T> ETL_CONST int etl_integral_type<T>::digits;
  template <typename T> ETL_CONST int etl_integral_type<T>::digits10;
  template <typename T> ETL_CONST int etl_integral_type<T>::max_digits10;
  template <typename T> ETL_CONST bool etl_integral_type<T>::is_signed;
  template <typename T> ETL_CONST bool etl_integral_type<T>::is_integer;
  template <typename T> ETL_CONST bool etl_integral_type<T>::is_exact;
  template <typename T> ETL_CONST int etl_integral_type<T>::radix;
  template <typename T> ETL_CONST int etl_integral_type<T>::min_exponent;
  template <typename T> ETL_CONST int etl_integral_type<T>::min_exponent10;
  template <typename T> ETL_CONST int etl_integral_type<T>::max_exponent;
  template <typename T> ETL_CONST int etl_integral_type<T>::max_exponent10;
  template <typename T> ETL_CONST bool etl_integral_type<T>::has_infinity;
  template <typename T> ETL_CONST bool etl_integral_type<T>::has_quiet_NaN;
  template <typename T> ETL_CONST bool etl_integral_type<T>::has_signaling_NaN;
  template <typename T> ETL_CONST float_denorm_style etl_integral_type<T>::has_denorm;
  template <typename T> ETL_CONST bool etl_integral_type<T>::has_denorm_loss;
  template <typename T> ETL_CONST bool etl_integral_type<T>::is_iec559;
  template <typename T> ETL_CONST bool etl_integral_type<T>::is_bounded;
  template <typename T> ETL_CONST bool etl_integral_type<T>::is_modulo;
  template <typename T> ETL_CONST bool etl_integral_type<T>::traps;
  template <typename T> ETL_CONST bool etl_integral_type<T>::tinyness_before;
  template <typename T> ETL_CONST float_round_style etl_integral_type<T>::round_style;

  //***************************************************************************
  // Base for floating point types.
  template <typename T>
  class etl_floating_point_type
  {
  public:
    static ETL_CONST bool is_specialized = true;

    static ETL_CONST bool is_signed  = true;
    static ETL_CONST bool is_integer = false;
    static ETL_CONST bool is_exact   = false;

    static ETL_CONST int radix = 2;

    static ETL_CONST bool has_infinity = false;
    static ETL_CONST bool has_quiet_NaN = false;
    static ETL_CONST bool has_signaling_NaN = false;
    static ETL_CONST float_denorm_style has_denorm = denorm_present;
    static ETL_CONST bool has_denorm_loss = true;

    static ETL_CONST bool is_iec559 = true;
    static ETL_CONST bool is_bounded = true;
    static ETL_CONST bool is_modulo = false;

    static ETL_CONSTEXPR T round_error() { return T(0.5); }
    static ETL_CONSTEXPR T infinity() { return 0; }
    static ETL_CONSTEXPR T quiet_NaN() { return 0; }
    static ETL_CONSTEXPR T signaling_NaN() { return 0; }

    static ETL_CONST bool traps = false;
    static ETL_CONST bool tinyness_before = true;
    static ETL_CONST float_round_style round_style = round_to_nearest;
  };

  template <typename T> ETL_CONST bool etl_floating_point_type<T>::is_specialized;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::is_signed;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::is_integer;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::is_exact;
  template <typename T> ETL_CONST int etl_floating_point_type<T>::radix;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::has_infinity;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::has_quiet_NaN;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::has_signaling_NaN;
  template <typename T> ETL_CONST float_denorm_style etl_floating_point_type<T>::has_denorm;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::has_denorm_loss;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::is_iec559;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::is_bounded;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::is_modulo;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::traps;
  template <typename T> ETL_CONST bool etl_floating_point_type<T>::tinyness_before;
  template <typename T> ETL_CONST float_round_style etl_floating_point_type<T>::round_style;

  //***************************************************************************
  // Default
  template<class T>
  class numeric_limits;

  //***************************************************************************
  // bool
  template<>
  class numeric_limits<bool>
  {
  public:

    static ETL_CONST bool is_specialized = true;

    static ETL_CONSTEXPR bool min() { return false; }
    static ETL_CONSTEXPR bool max() { return true; }
    static ETL_CONSTEXPR bool lowest() { return false; }

    static ETL_CONST int digits = 1;
    static ETL_CONST int digits10 = 0;
    static ETL_CONST int max_digits10 = 0;

    static ETL_CONST bool is_signed = false;
    static ETL_CONST bool is_integer = true;
    static ETL_CONST bool is_exact = true;

    static ETL_CONST int radix = 2;
    static ETL_CONST bool epsilon() { return 0; }
    static ETL_CONST bool round_error() { return 0; }

    static ETL_CONST int min_exponent = 0;
    static ETL_CONST int min_exponent10 = 0;
    static ETL_CONST int max_exponent = 0;
    static ETL_CONST int max_exponent10 = 0;

    static ETL_CONST bool has_infinity = false;
    static ETL_CONST bool has_quiet_NaN = false;
    static ETL_CONST bool has_signaling_NaN = false;
    static ETL_CONST float_denorm_style has_denorm = denorm_absent;
    static ETL_CONST bool has_denorm_loss = false;

    static ETL_CONSTEXPR bool infinity() { return 0; }
    static ETL_CONSTEXPR bool quiet_NaN() { return 0; }
    static ETL_CONSTEXPR bool signaling_NaN() { return 0; }
    static ETL_CONSTEXPR bool denorm_min() { return 0; }

    static ETL_CONST bool is_iec559 = false;
    static ETL_CONST bool is_bounded = true;
    static ETL_CONST bool is_modulo = false;

    static ETL_CONST bool traps = false;
    static ETL_CONST bool tinyness_before = false;
    static ETL_CONST float_round_style round_style = round_toward_zero;
  };

  ETL_CONST bool numeric_limits<bool>::is_specialized;
  ETL_CONST int numeric_limits<bool>::digits;
  ETL_CONST int numeric_limits<bool>::digits10;
  ETL_CONST int numeric_limits<bool>::max_digits10;
  ETL_CONST bool numeric_limits<bool>::is_signed;
  ETL_CONST bool numeric_limits<bool>::is_integer;
  ETL_CONST bool numeric_limits<bool>::is_exact;
  ETL_CONST int numeric_limits<bool>::radix;
  ETL_CONST int numeric_limits<bool>::min_exponent;
  ETL_CONST int numeric_limits<bool>::min_exponent10;
  ETL_CONST int numeric_limits<bool>::max_exponent;
  ETL_CONST int numeric_limits<bool>::max_exponent10;
  ETL_CONST bool numeric_limits<bool>::has_infinity;
  ETL_CONST bool numeric_limits<bool>::has_quiet_NaN;
  ETL_CONST bool numeric_limits<bool>::has_signaling_NaN;
  ETL_CONST float_denorm_style numeric_limits<bool>::has_denorm;
  ETL_CONST bool numeric_limits<bool>::has_denorm_loss;
  ETL_CONST bool numeric_limits<bool>::is_iec559;
  ETL_CONST bool numeric_limits<bool>::is_bounded;
  ETL_CONST bool numeric_limits<bool>::is_modulo;
  ETL_CONST bool numeric_limits<bool>::traps;
  ETL_CONST bool numeric_limits<bool>::tinyness_before;
  ETL_CONST float_round_style numeric_limits<bool>::round_style;

  //***************************************************************************
  // char
  template<>
  class numeric_limits<char> : public etl_integral_type<char>
  {
  public:

    static ETL_CONSTEXPR char min() { return CHAR_MIN; }
    static ETL_CONSTEXPR char max() { return CHAR_MAX; }
    static ETL_CONSTEXPR char lowest() { return CHAR_MIN; }
  };

  //***************************************************************************
  // unsigned char
  template<>
  class numeric_limits<unsigned char> : public etl_integral_type<unsigned char>
  {
  public:

    static ETL_CONSTEXPR unsigned char min() { return 0; }
    static ETL_CONSTEXPR unsigned char max() { return UCHAR_MAX; }
    static ETL_CONSTEXPR unsigned char lowest() { return 0; }
  };

  //***************************************************************************
  // signed char
  template<>
  class numeric_limits<signed char> : public etl_integral_type<signed char>
  {
  public:

    static ETL_CONSTEXPR signed char min() { return SCHAR_MIN; }
    static ETL_CONSTEXPR signed char max() { return SCHAR_MAX; }
    static ETL_CONSTEXPR signed char lowest() { return SCHAR_MIN; }
  };

  //***************************************************************************
  // char16_t
  template<>
  class numeric_limits<char16_t> : public etl_integral_type<char16_t>
  {
  public:

    static ETL_CONSTEXPR char16_t min() { return 0; }
    static ETL_CONSTEXPR char16_t max() { return UINT_LEAST16_MAX; }
    static ETL_CONSTEXPR char16_t lowest() { return 0; }
  };

  //***************************************************************************
  // char32_t
  template<>
  class numeric_limits<char32_t> : public etl_integral_type<char32_t>
  {
  public:

    static ETL_CONSTEXPR char32_t min() { return 0; }
    static ETL_CONSTEXPR char32_t max() { return UINT_LEAST32_MAX; }
    static ETL_CONSTEXPR char32_t lowest() { return 0; }
  };

  //***************************************************************************
  // wchar_t
  template<>
  class numeric_limits<wchar_t> : public etl_integral_type<wchar_t>
  {
  public:

    static ETL_CONSTEXPR wchar_t min() { return WCHAR_MIN; }
    static ETL_CONSTEXPR wchar_t max() { return WCHAR_MAX; }
    static ETL_CONSTEXPR wchar_t lowest() { return WCHAR_MIN; }
  };

  //***************************************************************************
  // short
  template<>
  class numeric_limits<short> : public etl_integral_type<short>
  {
  public:

    static ETL_CONSTEXPR short min() { return SHRT_MIN; }
    static ETL_CONSTEXPR short max() { return SHRT_MAX; }
    static ETL_CONSTEXPR short lowest() { return SHRT_MIN; }
  };

  //***************************************************************************
  // unsigned short
  template<>
  class numeric_limits<unsigned short> : public etl_integral_type<unsigned short>
  {
  public:

    static ETL_CONSTEXPR unsigned short min() { return 0; }
    static ETL_CONSTEXPR unsigned short max() { return USHRT_MAX; }
    static ETL_CONSTEXPR unsigned short lowest() { return 0; }
  };

  //***************************************************************************
  // int
  template<>
  class numeric_limits<int> : public etl_integral_type<int>
  {
  public:

    static ETL_CONSTEXPR int min() { return INT_MIN; }
    static ETL_CONSTEXPR int max() { return INT_MAX; }
    static ETL_CONSTEXPR int lowest() { return INT_MIN; }
  };

  //***************************************************************************
  // unsigned int
  template<>
  class numeric_limits<unsigned int> : public etl_integral_type<unsigned int>
  {
  public:

    static ETL_CONSTEXPR unsigned int min() { return 0; }
    static ETL_CONSTEXPR unsigned int max() { return UINT_MAX; }
    static ETL_CONSTEXPR unsigned int lowest() { return 0; }
  };

  //***************************************************************************
  // long
  template<>
  class numeric_limits<long> : public etl_integral_type<long>
  {
  public:

    static ETL_CONSTEXPR long min() { return LONG_MIN; }
    static ETL_CONSTEXPR long max() { return LONG_MAX; }
    static ETL_CONSTEXPR long lowest() { return LONG_MIN; }
  };

  //***************************************************************************
  // unsigned long
  template<>
  class numeric_limits<unsigned long> : public etl_integral_type<unsigned long>
  {
  public:

    static ETL_CONSTEXPR unsigned long min() { return 0; }
    static ETL_CONSTEXPR unsigned long max() { return ULONG_MAX; }
    static ETL_CONSTEXPR unsigned long lowest() { return 0; }
  };

  //***************************************************************************
  // long long
  template<>
  class numeric_limits<long long> : public etl_integral_type<long long>
  {
  public:

    static ETL_CONSTEXPR long long min() { return LLONG_MIN; }
    static ETL_CONSTEXPR long long max() { return LLONG_MAX; }
    static ETL_CONSTEXPR long long lowest() { return LLONG_MIN; }
  };

  //***************************************************************************
  // unsigned long long
  template<>
  class numeric_limits<unsigned long long> : public etl_integral_type<unsigned long long>
  {
  public:

    static ETL_CONSTEXPR unsigned long long min() { return 0; }
    static ETL_CONSTEXPR unsigned long long max() { return ULLONG_MAX; }
    static ETL_CONSTEXPR unsigned long long lowest() { return 0; }
  };

  //***************************************************************************
  // float
  template<>
  class numeric_limits<float> : public etl_floating_point_type<float>
  {
  public:

    static ETL_CONSTEXPR float min() { return FLT_MIN; }
    static ETL_CONSTEXPR float max() { return FLT_MAX; }
    static ETL_CONSTEXPR float lowest() { return -FLT_MAX; }
    static ETL_CONSTEXPR float epsilon() { return FLT_EPSILON; }
    static ETL_CONSTEXPR float denorm_min() { return FLT_MIN; }

    static ETL_CONST int digits       = FLT_MANT_DIG;
    static ETL_CONST int digits10     = FLT_DIG;
    static ETL_CONST int max_digits10 = ETL_LOG2(FLT_MANT_DIG) + 2;

    static ETL_CONST int min_exponent   = FLT_MIN_EXP;
    static ETL_CONST int min_exponent10 = FLT_MIN_10_EXP;
    static ETL_CONST int max_exponent   = FLT_MAX_EXP;
    static ETL_CONST int max_exponent10 = FLT_MAX_10_EXP;
  };

  ETL_CONST int numeric_limits<float>::digits;
  ETL_CONST int numeric_limits<float>::max_digits10;
  ETL_CONST int numeric_limits<float>::digits10;
  ETL_CONST int numeric_limits<float>::min_exponent;
  ETL_CONST int numeric_limits<float>::min_exponent10;
  ETL_CONST int numeric_limits<float>::max_exponent;
  ETL_CONST int numeric_limits<float>::max_exponent10;

  //***************************************************************************
  // double
  template<>
  class numeric_limits<double> : public etl_floating_point_type<double>
  {
  public:

    static ETL_CONSTEXPR double min() { return DBL_MIN; }
    static ETL_CONSTEXPR double max() { return DBL_MAX; }
    static ETL_CONSTEXPR double lowest() { return -DBL_MAX; }
    static ETL_CONSTEXPR double epsilon() { return DBL_EPSILON; }
    static ETL_CONSTEXPR double denorm_min() { return DBL_MIN; }

    static ETL_CONST int digits       = DBL_MANT_DIG;
    static ETL_CONST int digits10     = DBL_DIG;
    static ETL_CONST int max_digits10 = ETL_LOG2(DBL_MANT_DIG) + 2;

    static ETL_CONST int min_exponent   = DBL_MIN_EXP;
    static ETL_CONST int min_exponent10 = DBL_MIN_10_EXP;
    static ETL_CONST int max_exponent   = DBL_MAX_EXP;
    static ETL_CONST int max_exponent10 = DBL_MAX_10_EXP;
  };

  ETL_CONST int numeric_limits<double>::digits;
  ETL_CONST int numeric_limits<double>::max_digits10;
  ETL_CONST int numeric_limits<double>::digits10;
  ETL_CONST int numeric_limits<double>::min_exponent;
  ETL_CONST int numeric_limits<double>::min_exponent10;
  ETL_CONST int numeric_limits<double>::max_exponent;
  ETL_CONST int numeric_limits<double>::max_exponent10;

  //***************************************************************************
  // long double
  template<>
  class numeric_limits<long double> : public etl_floating_point_type<long double>
  {
  public:

    static ETL_CONSTEXPR long double min() { return LDBL_MIN; }
    static ETL_CONSTEXPR long double max() { return LDBL_MAX; }
    static ETL_CONSTEXPR long double lowest() { return -LDBL_MAX; }
    static ETL_CONSTEXPR long double epsilon() { return LDBL_EPSILON; }
    static ETL_CONSTEXPR long double denorm_min() { return LDBL_MIN; }

    static ETL_CONST int digits       = LDBL_MANT_DIG;
    static ETL_CONST int digits10     = LDBL_DIG;
    static ETL_CONST int max_digits10 = ETL_LOG2(LDBL_MANT_DIG) + 2;

    static ETL_CONST int min_exponent   = LDBL_MIN_EXP;
    static ETL_CONST int min_exponent10 = LDBL_MIN_10_EXP;
    static ETL_CONST int max_exponent   = LDBL_MAX_EXP;
    static ETL_CONST int max_exponent10 = LDBL_MAX_10_EXP;
  };

  ETL_CONST int numeric_limits<long double>::digits;
  ETL_CONST int numeric_limits<long double>::max_digits10;
  ETL_CONST int numeric_limits<long double>::digits10;
  ETL_CONST int numeric_limits<long double>::min_exponent;
  ETL_CONST int numeric_limits<long double>::min_exponent10;
  ETL_CONST int numeric_limits<long double>::max_exponent;
  ETL_CONST int numeric_limits<long double>::max_exponent10;
}

#endif
