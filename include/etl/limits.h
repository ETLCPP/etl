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

#ifndef ETL_LIMITS_INCLUDED
#define ETL_LIMITS_INCLUDED

#include "etl/platform.h"
#include "etl/type_traits.h"
#include "etl/char_traits.h"
#include "etl/integral_limits.h"

#include <limits.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

#if defined(ETL_NO_STL)
#define ETL_LOG10_OF_2(x) (((x) * 301) / 1000)

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

  class etl_integral_limits
  {
  public:

    static const bool is_specialized    = true;
    static const bool is_integer        = true;
    static const bool is_exact          = true;
    static const int  max_digits10      = 0;
    static const int  radix             = 2;
    static const int  min_exponent      = 0;
    static const int  min_exponent10    = 0;
    static const int  max_exponent      = 0;
    static const int  max_exponent10    = 0;
    static const bool has_infinity      = false;
    static const bool has_quiet_NaN     = false;
    static const bool has_signaling_NaN = false;
    static const bool has_denorm_loss   = false;
    static const bool is_iec559         = false;
    static const bool is_bounded        = true;
    static const bool traps             = false;
    static const bool tinyness_before   = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const float_round_style round_style = round_toward_zero;
  };

  class etl_floating_point_limits
  {
  public:

    static const bool is_specialized = true;
    static const bool is_signed = true;
    static const bool is_integer = false;
    static const bool is_exact = false;
    static const int radix = 2;
    static const bool has_infinity = true;
    static const bool has_quiet_NaN = true;
    static const bool has_signaling_NaN = true;
    static const bool has_denorm_loss = false;
    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = false;
    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_denorm_style has_denorm = denorm_indeterminate;
    static const float_round_style round_style = round_indeterminate;

    static const float round_error() { return float(0.5); }
  };

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

  //***************************************************************************
  // bool
  template<>
  class numeric_limits<bool> : public etl_integral_limits
  {
  public:

    static const int  digits    = 1;
    static const int  digits10  = 0;
    static const bool is_signed = false;
    static const bool is_modulo = false;

    static const bool min() { return false; }
    static const bool max() { return true; }
    static const bool lowest() { return false; }
    static const bool epsilon() { return false; }
    static const bool round_error() { return false; }
    static const bool denorm_min() { return false; }
    static const bool infinity() { return false; }
    static const bool quiet_NaN() { return false; }
    static const bool signaling_NaN() { return false; }
  };

  //***************************************************************************
  // char
  template<>
  class numeric_limits<char> : public etl_integral_limits
  {
  public:

    static const int digits     = (CHAR_BIT * sizeof(char)) - (etl::is_signed<char>::value ? 1 : 0);
    static const int digits10   = ETL_LOG10_OF_2(digits);
    static const bool is_signed = etl::is_signed<char>::value;
    static const bool is_modulo = false;

    static const char min() { return char(CHAR_MIN); }
    static const char max() { return char(CHAR_MAX); }
    static const char lowest() { return char(CHAR_MIN); }
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }
    static const char denorm_min() { return 0; }
    static const char infinity() { return 0; }
    static const char quiet_NaN() { return 0; }
    static const char signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned char
  template<>
  class numeric_limits<unsigned char> : public etl_integral_limits
  {
  public:

    static const int digits     = (CHAR_BIT * sizeof(unsigned char)) - (etl::is_signed<unsigned char>::value ? 1 : 0);
    static const int digits10   = ETL_LOG10_OF_2(digits);
    static const bool is_signed = false;
    static const bool is_modulo = true;

    static const unsigned char min() { return 0U; }
    static const unsigned char max() { return UCHAR_MAX; }
    static const unsigned char lowest() { return 0U; }
    static const unsigned char epsilon() { return 0U; }
    static const unsigned char round_error() { return 0U; }
    static const unsigned char denorm_min() { return 0U; }
    static const unsigned char infinity() { return 0U; }
    static const unsigned char quiet_NaN() { return 0U; }
    static const unsigned char signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // signed char
  template<>
  class numeric_limits<signed char> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(char)) - (etl::is_signed<char>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = true;
    static const bool is_modulo = false;

    static const signed char min() { return SCHAR_MIN; }
    static const signed char max() { return SCHAR_MAX; }
    static const signed char lowest() { return SCHAR_MIN; }
    static const signed char epsilon() { return 0; }
    static const signed char round_error() { return 0; }
    static const signed char denorm_min() { return 0; }
    static const signed char infinity() { return 0; }
    static const signed char quiet_NaN() { return 0; }
    static const signed char signaling_NaN() { return 0; }
  };

#if (ETL_NO_LARGE_CHAR_SUPPORT == false)
  //***************************************************************************
  // char16_t
  template<>
  class numeric_limits<char16_t> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(char16_t)) - (etl::is_signed<char16_t>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = false;
    static const bool is_modulo = true;

    static const char16_t min() { return 0U; }
    static const char16_t max() { return UINT_LEAST16_MAX; }
    static const char16_t lowest() { return 0U; }
    static const char16_t epsilon() { return 0U; }
    static const char16_t round_error() { return 0U; }
    static const char16_t denorm_min() { return 0U; }
    static const char16_t infinity() { return 0U; }
    static const char16_t quiet_NaN() { return 0U; }
    static const char16_t signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // char32_t
  template<>
  class numeric_limits<char32_t> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(char32_t)) - (etl::is_signed<char32_t>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = false;
    static const bool is_modulo = true;

    static const char32_t min() { return 0U; }
    static const char32_t max() { return UINT_LEAST32_MAX; }
    static const char32_t lowest() { return 0U; }
    static const char32_t epsilon() { return 0U; }
    static const char32_t round_error() { return 0U; }
    static const char32_t denorm_min() { return 0U; }
    static const char32_t infinity() { return 0U; }
    static const char32_t quiet_NaN() { return 0U; }
    static const char32_t signaling_NaN() { return 0U; }
  };

#endif

  //***************************************************************************
  // wchar_t
  template<>
  class numeric_limits<wchar_t> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(wchar_t)) - (etl::is_signed<wchar_t>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = etl::is_signed<wchar_t>::value;
    static const bool is_modulo = etl::is_unsigned<wchar_t>::value;

    static const wchar_t min() { return WCHAR_MIN; }
    static const wchar_t max() { return WCHAR_MAX; }
    static const wchar_t lowest() { return WCHAR_MIN; }
    static const wchar_t epsilon() { return wchar_t(0); }
    static const wchar_t round_error() { return wchar_t(0); }
    static const wchar_t denorm_min() { return wchar_t(0); }
    static const wchar_t infinity() { return wchar_t(0); }
    static const wchar_t quiet_NaN() { return wchar_t(0); }
    static const wchar_t signaling_NaN() { return wchar_t(0); }
  };

  //***************************************************************************
  // short
  template<>
  class numeric_limits<short> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(short)) - (etl::is_signed<short>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = true;
    static const bool is_modulo = false;

    static const short min() { return SHRT_MIN; }
    static const short max() { return SHRT_MAX; }
    static const short lowest() { return SHRT_MIN; }
    static const short epsilon() { return 0; }
    static const short round_error() { return 0; }
    static const short denorm_min() { return 0; }
    static const short infinity() { return 0; }
    static const short quiet_NaN() { return 0; }
    static const short signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned short
  template<>
  class numeric_limits<unsigned short> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(unsigned short)) - (etl::is_signed<unsigned short>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = false;
    static const bool is_modulo = true;

    static const unsigned short min() { return 0U; }
    static const unsigned short max() { return USHRT_MAX; }
    static const unsigned short lowest() { return 0U; }
    static const unsigned short epsilon() { return 0U; }
    static const unsigned short round_error() { return 0U; }
    static const unsigned short denorm_min() { return 0U; }
    static const unsigned short infinity() { return 0U; }
    static const unsigned short quiet_NaN() { return 0U; }
    static const unsigned short signaling_NaN() { return 0U; }

  };

  //***************************************************************************
  // int
  template<>
  class numeric_limits<int> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(int)) - (etl::is_signed<int>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = true;
    static const bool is_modulo = false;

    static const int min() { return INT_MIN; }
    static const int max() { return INT_MAX; }
    static const int lowest() { return INT_MIN; }
    static const int epsilon() { return 0; }
    static const int round_error() { return 0; }
    static const int denorm_min() { return 0; }
    static const int infinity() { return 0; }
    static const int quiet_NaN() { return 0; }
    static const int signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned int
  template<>
  class numeric_limits<unsigned int> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(unsigned int)) - (etl::is_signed<unsigned int>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = false;
    static const bool is_modulo = true;

    static const unsigned int min() { return 0U; }
    static const unsigned int max() { return UINT_MAX; }
    static const unsigned int lowest() { return 0U; }
    static const unsigned int epsilon() { return 0U; }
    static const unsigned int round_error() { return 0U; }
    static const unsigned int denorm_min() { return 0U; }
    static const unsigned int infinity() { return 0U; }
    static const unsigned int quiet_NaN() { return 0U; }
    static const unsigned int signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // long
  template<>
  class numeric_limits<long> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(long)) - (etl::is_signed<long>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = true;
    static const bool is_modulo = false;

    static const long min() { return LONG_MIN; }
    static const long max() { return LONG_MAX; }
    static const long lowest() { return LONG_MIN; }
    static const long epsilon() { return 0; }
    static const long round_error() { return 0; }
    static const long denorm_min() { return 0; }
    static const long infinity() { return 0; }
    static const long quiet_NaN() { return 0; }
    static const long signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned long
  template<>
  class numeric_limits<unsigned long> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(unsigned long)) - (etl::is_signed<unsigned long>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = false;
    static const bool is_modulo = true;

    static const unsigned long min() { return 0U; }
    static const unsigned long max() { return ULONG_MAX; }
    static const unsigned long lowest() { return 0U; }
    static const unsigned long epsilon() { return 0U; }
    static const unsigned long round_error() { return 0U; }
    static const unsigned long denorm_min() { return 0U; }
    static const unsigned long infinity() { return 0U; }
    static const unsigned long quiet_NaN() { return 0U; }
    static const unsigned long signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // long long
  template<>
  class numeric_limits<long long> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(long long)) - (etl::is_signed<long long>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = true;
    static const bool is_modulo = false;

    static const long long min() { return LLONG_MIN; }
    static const long long max() { return LLONG_MAX; }
    static const long long lowest() { return LLONG_MIN; }
    static const long long epsilon() { return 0; }
    static const long long round_error() { return 0; }
    static const long long denorm_min() { return 0; }
    static const long long infinity() { return 0; }
    static const long long quiet_NaN() { return 0; }
    static const long long signaling_NaN() { return 0; }
  };

  //***************************************************************************
  // unsigned long long
  template<>
  class numeric_limits<unsigned long long> : public etl_integral_limits
  {
  public:

    static const int digits = (CHAR_BIT * sizeof(unsigned long long)) - (etl::is_signed<unsigned long long>::value ? 1 : 0);
    static const int digits10 = ETL_LOG10_OF_2(digits);
    static const bool is_signed = false;
    static const bool is_modulo = true;

    static const unsigned long long min() { return 0U; }
    static const unsigned long long max() { return ULLONG_MAX; }
    static const unsigned long long lowest() { return 0U; }
    static const unsigned long long epsilon() { return 0U; }
    static const unsigned long long round_error() { return 0U; }
    static const unsigned long long denorm_min() { return 0U; }
    static const unsigned long long infinity() { return 0U; }
    static const unsigned long long quiet_NaN() { return 0U; }
    static const unsigned long long signaling_NaN() { return 0U; }
  };

  //***************************************************************************
  // float
  template<>
  class numeric_limits<float> : public etl_floating_point_limits
  {
  public:

    static const float min() { return FLT_MIN; }
    static const float max() { return FLT_MAX; }
    static const float lowest() { return -FLT_MAX; }
    static const float epsilon() { return FLT_EPSILON; }
    static const float denorm_min() { return FLT_MIN; }
    static const float infinity() { return HUGE_VALF; }
    static const float quiet_NaN() { return nanf(""); }
    static const float signaling_NaN() { return nanf(""); }

    static const int digits = FLT_MANT_DIG;
    static const int digits10 = FLT_DIG;
    static const int max_digits10 = ETL_LOG10_OF_2(FLT_MANT_DIG) + 2;

    static const int min_exponent = FLT_MIN_EXP;
    static const int min_exponent10 = FLT_MIN_10_EXP;
    static const int max_exponent = FLT_MAX_EXP;
    static const int max_exponent10 = FLT_MAX_10_EXP;
  };

  //***************************************************************************
  // double
  template<>
  class numeric_limits<double> : public etl_floating_point_limits
  {
  public:

    static const double min() { return DBL_MIN; }
    static const double max() { return DBL_MAX; }
    static const double lowest() { return -DBL_MAX; }
    static const double epsilon() { return DBL_EPSILON; }
    static const double denorm_min() { return DBL_MIN; }
    static const double infinity() { return HUGE_VAL; }
    static const double quiet_NaN() { return nan(""); }
    static const double signaling_NaN() { return nan(""); }

    static const int digits = DBL_MANT_DIG;
    static const int digits10 = DBL_DIG;
    static const int max_digits10 = ETL_LOG10_OF_2(DBL_MANT_DIG) + 2;

    static const int min_exponent = DBL_MIN_EXP;
    static const int min_exponent10 = DBL_MIN_10_EXP;
    static const int max_exponent = DBL_MAX_EXP;
    static const int max_exponent10 = DBL_MAX_10_EXP;
  };

  //***************************************************************************
  // long double
  template<>
  class numeric_limits<long double> : public etl_floating_point_limits
  {
  public:

    static const long double min() { return LDBL_MIN; }
    static const long double max() { return LDBL_MAX; }
    static const long double lowest() { return -LDBL_MAX; }
    static const long double epsilon() { return LDBL_EPSILON; }
    static const long double denorm_min() { return LDBL_MIN; }
    static const long double infinity() { return HUGE_VALL; }
    static const long double quiet_NaN() { return nanl(""); }
    static const long double signaling_NaN() { return nanl(""); }


    static const int digits = LDBL_MANT_DIG;
    static const int digits10 = LDBL_DIG;
    static const int max_digits10 = ETL_LOG10_OF_2(LDBL_MANT_DIG) + 2;

    static const int min_exponent = LDBL_MIN_EXP;
    static const int min_exponent10 = LDBL_MIN_10_EXP;
    static const int max_exponent = LDBL_MAX_EXP;
    static const int max_exponent10 = LDBL_MAX_10_EXP;
  };
}

#else

#include <limits>

namespace etl
{
  enum float_round_style
  {
    round_indeterminate       = std::float_round_style::round_indeterminate,
    round_toward_zero         = std::float_round_style::round_toward_zero,
    round_to_nearest          = std::float_round_style::round_to_nearest,
    round_toward_infinity     = std::float_round_style::round_toward_infinity,
    round_toward_neg_infinity = std::float_round_style::round_toward_neg_infinity,
  };

  enum float_denorm_style
  {
    denorm_indeterminate = std::float_denorm_style::denorm_indeterminate,
    denorm_absent        = std::float_denorm_style::denorm_absent,
    denorm_present       = std::float_denorm_style::denorm_present
  };

  template <typename T>
  class numeric_limits : public std::numeric_limits<T>
  {
  };
}
#endif

#endif
