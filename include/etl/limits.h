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

#include "platform.h"
#include "type_traits.h"
#include "char_traits.h"
#include "integral_limits.h"

#include <limits.h>
#include <stdint.h>
#include <float.h>

#if defined(ETL_NO_STL)
#define ETL_LOG2(x) (((x) * 301) / 1000)

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
  class numeric_limits<bool>
  {
  public:

    static const bool is_specialized = true;

    static const int digits = 1;
    static const int digits10 = 0;
    static const int max_digits10 = 0;

    static const bool is_signed = false;
    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const bool epsilon() { return false; }
    static const bool round_error() { return false; }

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const bool infinity() { return false; }
    static const bool quiet_NaN() { return false; }
    static const bool signaling_NaN() { return false; }
    static const bool denorm_min() { return false; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = false;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const bool min() { return false; }
    static const bool max() { return true; }
    static const bool lowest() { return false; }
  };

  //***************************************************************************
  // char
  template<>
  class numeric_limits<char>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(char)) - (etl::is_signed<char>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<char>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const char infinity() { return 0; }
    static const char quiet_NaN() { return 0; }
    static const char signaling_NaN() { return 0; }
    static const char denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<char>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const char min() { return char(CHAR_MIN); }
    static const char max() { return char(CHAR_MAX); }
    static const char lowest() { return char(CHAR_MIN); }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<char>::is_specialized;
  const int numeric_limits<char>::digits;
  const int numeric_limits<char>::digits10;
  const int numeric_limits<char>::max_digits10;
  const bool numeric_limits<char>::is_signed;
  const bool numeric_limits<char>::is_integer;
  const bool numeric_limits<char>::is_exact;
  const int numeric_limits<char>::radix;
  const int numeric_limits<char>::min_exponent;
  const int numeric_limits<char>::min_exponent10;
  const int numeric_limits<char>::max_exponent;
  const int numeric_limits<char>::max_exponent10;
  const bool numeric_limits<char>::has_infinity;
  const bool numeric_limits<char>::has_quiet_NaN;
  const bool numeric_limits<char>::has_signaling_NaN;
  const float_denorm_style numeric_limits<char>::has_denorm;
  const bool numeric_limits<char>::has_denorm_loss;
  const bool numeric_limits<char>::is_iec559;
  const bool numeric_limits<char>::is_bounded;
  const bool numeric_limits<char>::is_modulo;
  const bool numeric_limits<char>::traps;
  const bool numeric_limits<char>::tinyness_before;
  const float_round_style numeric_limits<char>::round_style;
  #endif

  //***************************************************************************
  // unsigned char
  template<>
  class numeric_limits<unsigned char>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(unsigned char)) - (etl::is_signed<unsigned char>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<unsigned char>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<unsigned char>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const unsigned char min() { return 0; }
    static const unsigned char max() { return UCHAR_MAX; }
    static const unsigned char lowest() { return 0; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<unsigned char>::is_specialized;
  const int numeric_limits<unsigned char>::digits;
  const int numeric_limits<unsigned char>::digits10;
  const int numeric_limits<unsigned char>::max_digits10;
  const bool numeric_limits<unsigned char>::is_signed;
  const bool numeric_limits<unsigned char>::is_integer;
  const bool numeric_limits<unsigned char>::is_exact;
  const int numeric_limits<unsigned char>::radix;
  const int numeric_limits<unsigned char>::min_exponent;
  const int numeric_limits<unsigned char>::min_exponent10;
  const int numeric_limits<unsigned char>::max_exponent;
  const int numeric_limits<unsigned char>::max_exponent10;
  const bool numeric_limits<unsigned char>::has_infinity;
  const bool numeric_limits<unsigned char>::has_quiet_NaN;
  const bool numeric_limits<unsigned char>::has_signaling_NaN;
  const float_denorm_style numeric_limits<unsigned char>::has_denorm;
  const bool numeric_limits<unsigned char>::has_denorm_loss;
  const bool numeric_limits<unsigned char>::is_iec559;
  const bool numeric_limits<unsigned char>::is_bounded;
  const bool numeric_limits<unsigned char>::is_modulo;
  const bool numeric_limits<unsigned char>::traps;
  const bool numeric_limits<unsigned char>::tinyness_before;
  const float_round_style numeric_limits<unsigned char>::round_style;
  #endif

  //***************************************************************************
  // signed char
  template<>
  class numeric_limits<signed char>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(char)) - (etl::is_signed<char>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<char>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const char infinity() { return 0; }
    static const char quiet_NaN() { return 0; }
    static const char signaling_NaN() { return 0; }
    static const char denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<char>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const signed char min() { return SCHAR_MIN; }
    static const signed char max() { return SCHAR_MAX; }
    static const signed char lowest() { return SCHAR_MIN; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<signed char>::is_specialized;
  const int numeric_limits<signed char>::digits;
  const int numeric_limits<signed char>::digits10;
  const int numeric_limits<signed char>::max_digits10;
  const bool numeric_limits<signed char>::is_signed;
  const bool numeric_limits<signed char>::is_integer;
  const bool numeric_limits<signed char>::is_exact;
  const int numeric_limits<signed char>::radix;
  const int numeric_limits<signed char>::min_exponent;
  const int numeric_limits<signed char>::min_exponent10;
  const int numeric_limits<signed char>::max_exponent;
  const int numeric_limits<signed char>::max_exponent10;
  const bool numeric_limits<signed char>::has_infinity;
  const bool numeric_limits<signed char>::has_quiet_NaN;
  const bool numeric_limits<signed char>::has_signaling_NaN;
  const float_denorm_style numeric_limits<signed char>::has_denorm;
  const bool numeric_limits<signed char>::has_denorm_loss;
  const bool numeric_limits<signed char>::is_iec559;
  const bool numeric_limits<signed char>::is_bounded;
  const bool numeric_limits<signed char>::is_modulo;
  const bool numeric_limits<signed char>::traps;
  const bool numeric_limits<signed char>::tinyness_before;
  const float_round_style numeric_limits<signed char>::round_style;
  #endif

#if (ETL_NO_LARGE_CHAR_SUPPORT == false)
  //***************************************************************************
  // char16_t
  template<>
  class numeric_limits<char16_t>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(char16_t)) - (etl::is_signed<char16_t>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<char16_t>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = true;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const char16_t min() { return 0; }
    static const char16_t max() { return UINT_LEAST16_MAX; }
    static const char16_t lowest() { return 0; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<char16_t>::is_specialized;
  const int numeric_limits<char16_t>::digits;
  const int numeric_limits<char16_t>::digits10;
  const int numeric_limits<char16_t>::max_digits10;
  const bool numeric_limits<char16_t>::is_signed;
  const bool numeric_limits<char16_t>::is_integer;
  const bool numeric_limits<char16_t>::is_exact;
  const int numeric_limits<char16_t>::radix;
  const int numeric_limits<char16_t>::min_exponent;
  const int numeric_limits<char16_t>::min_exponent10;
  const int numeric_limits<char16_t>::max_exponent;
  const int numeric_limits<char16_t>::max_exponent10;
  const bool numeric_limits<char16_t>::has_infinity;
  const bool numeric_limits<char16_t>::has_quiet_NaN;
  const bool numeric_limits<char16_t>::has_signaling_NaN;
  const float_denorm_style numeric_limits<char16_t>::has_denorm;
  const bool numeric_limits<char16_t>::has_denorm_loss;
  const bool numeric_limits<char16_t>::is_iec559;
  const bool numeric_limits<char16_t>::is_bounded;
  const bool numeric_limits<char16_t>::is_modulo;
  const bool numeric_limits<char16_t>::traps;
  const bool numeric_limits<char16_t>::tinyness_before;
  const float_round_style numeric_limits<char16_t>::round_style;
  #endif

  //***************************************************************************
  // char32_t
  template<>
  class numeric_limits<char32_t>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(char32_t)) - (etl::is_signed<char32_t>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<char32_t>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = true;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const char32_t min() { return 0; }
    static const char32_t max() { return UINT_LEAST32_MAX; }
    static const char32_t lowest() { return 0; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<char32_t>::is_specialized;
  const int numeric_limits<char32_t>::digits;
  const int numeric_limits<char32_t>::digits10;
  const int numeric_limits<char32_t>::max_digits10;
  const bool numeric_limits<char32_t>::is_signed;
  const bool numeric_limits<char32_t>::is_integer;
  const bool numeric_limits<char32_t>::is_exact;
  const int numeric_limits<char32_t>::radix;
  const int numeric_limits<char32_t>::min_exponent;
  const int numeric_limits<char32_t>::min_exponent10;
  const int numeric_limits<char32_t>::max_exponent;
  const int numeric_limits<char32_t>::max_exponent10;
  const bool numeric_limits<char32_t>::has_infinity;
  const bool numeric_limits<char32_t>::has_quiet_NaN;
  const bool numeric_limits<char32_t>::has_signaling_NaN;
  const float_denorm_style numeric_limits<char32_t>::has_denorm;
  const bool numeric_limits<char32_t>::has_denorm_loss;
  const bool numeric_limits<char32_t>::is_iec559;
  const bool numeric_limits<char32_t>::is_bounded;
  const bool numeric_limits<char32_t>::is_modulo;
  const bool numeric_limits<char32_t>::traps;
  const bool numeric_limits<char32_t>::tinyness_before;
  const float_round_style numeric_limits<char32_t>::round_style;
  #endif

#endif

  //***************************************************************************
  // wchar_t
  template<>
  class numeric_limits<wchar_t>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(wchar_t)) - (etl::is_signed<wchar_t>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<wchar_t>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<wchar_t>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const wchar_t min() { return WCHAR_MIN; }
    static const wchar_t max() { return WCHAR_MAX; }
    static const wchar_t lowest() { return WCHAR_MIN; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<wchar_t>::is_specialized;
  const int numeric_limits<wchar_t>::digits;
  const int numeric_limits<wchar_t>::digits10;
  const int numeric_limits<wchar_t>::max_digits10;
  const bool numeric_limits<wchar_t>::is_signed;
  const bool numeric_limits<wchar_t>::is_integer;
  const bool numeric_limits<wchar_t>::is_exact;
  const int numeric_limits<wchar_t>::radix;
  const int numeric_limits<wchar_t>::min_exponent;
  const int numeric_limits<wchar_t>::min_exponent10;
  const int numeric_limits<wchar_t>::max_exponent;
  const int numeric_limits<wchar_t>::max_exponent10;
  const bool numeric_limits<wchar_t>::has_infinity;
  const bool numeric_limits<wchar_t>::has_quiet_NaN;
  const bool numeric_limits<wchar_t>::has_signaling_NaN;
  const float_denorm_style numeric_limits<wchar_t>::has_denorm;
  const bool numeric_limits<wchar_t>::has_denorm_loss;
  const bool numeric_limits<wchar_t>::is_iec559;
  const bool numeric_limits<wchar_t>::is_bounded;
  const bool numeric_limits<wchar_t>::is_modulo;
  const bool numeric_limits<wchar_t>::traps;
  const bool numeric_limits<wchar_t>::tinyness_before;
  const float_round_style numeric_limits<wchar_t>::round_style;
  #endif

  //***************************************************************************
  // short
  template<>
  class numeric_limits<short>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(short)) - (etl::is_signed<short>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<short>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<short>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const short min() { return SHRT_MIN; }
    static const short max() { return SHRT_MAX; }
    static const short lowest() { return SHRT_MIN; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<short>::is_specialized;
  const int numeric_limits<short>::digits;
  const int numeric_limits<short>::digits10;
  const int numeric_limits<short>::max_digits10;
  const bool numeric_limits<short>::is_signed;
  const bool numeric_limits<short>::is_integer;
  const bool numeric_limits<short>::is_exact;
  const int numeric_limits<short>::radix;
  const int numeric_limits<short>::min_exponent;
  const int numeric_limits<short>::min_exponent10;
  const int numeric_limits<short>::max_exponent;
  const int numeric_limits<short>::max_exponent10;
  const bool numeric_limits<short>::has_infinity;
  const bool numeric_limits<short>::has_quiet_NaN;
  const bool numeric_limits<short>::has_signaling_NaN;
  const float_denorm_style numeric_limits<short>::has_denorm;
  const bool numeric_limits<short>::has_denorm_loss;
  const bool numeric_limits<short>::is_iec559;
  const bool numeric_limits<short>::is_bounded;
  const bool numeric_limits<short>::is_modulo;
  const bool numeric_limits<short>::traps;
  const bool numeric_limits<short>::tinyness_before;
  const float_round_style numeric_limits<short>::round_style;
  #endif

  //***************************************************************************
  // unsigned short
  template<>
  class numeric_limits<unsigned short>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(unsigned short)) - (etl::is_signed<unsigned short>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<unsigned short>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<unsigned short>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const unsigned short min() { return 0; }
    static const unsigned short max() { return USHRT_MAX; }
    static const unsigned short lowest() { return 0; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<unsigned short>::is_specialized;
  const int numeric_limits<unsigned short>::digits;
  const int numeric_limits<unsigned short>::digits10;
  const int numeric_limits<unsigned short>::max_digits10;
  const bool numeric_limits<unsigned short>::is_signed;
  const bool numeric_limits<unsigned short>::is_integer;
  const bool numeric_limits<unsigned short>::is_exact;
  const int numeric_limits<unsigned short>::radix;
  const int numeric_limits<unsigned short>::min_exponent;
  const int numeric_limits<unsigned short>::min_exponent10;
  const int numeric_limits<unsigned short>::max_exponent;
  const int numeric_limits<unsigned short>::max_exponent10;
  const bool numeric_limits<unsigned short>::has_infinity;
  const bool numeric_limits<unsigned short>::has_quiet_NaN;
  const bool numeric_limits<unsigned short>::has_signaling_NaN;
  const float_denorm_style numeric_limits<unsigned short>::has_denorm;
  const bool numeric_limits<unsigned short>::has_denorm_loss;
  const bool numeric_limits<unsigned short>::is_iec559;
  const bool numeric_limits<unsigned short>::is_bounded;
  const bool numeric_limits<unsigned short>::is_modulo;
  const bool numeric_limits<unsigned short>::traps;
  const bool numeric_limits<unsigned short>::tinyness_before;
  const float_round_style numeric_limits<unsigned short>::round_style;
  #endif

  //***************************************************************************
  // int
  template<>
  class numeric_limits<int>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(int)) - (etl::is_signed<int>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<int>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<int>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const int min() { return INT_MIN; }
    static const int max() { return INT_MAX; }
    static const int lowest() { return INT_MIN; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<int>::is_specialized;
  const int numeric_limits<int>::digits;
  const int numeric_limits<int>::digits10;
  const int numeric_limits<int>::max_digits10;
  const bool numeric_limits<int>::is_signed;
  const bool numeric_limits<int>::is_integer;
  const bool numeric_limits<int>::is_exact;
  const int numeric_limits<int>::radix;
  const int numeric_limits<int>::min_exponent;
  const int numeric_limits<int>::min_exponent10;
  const int numeric_limits<int>::max_exponent;
  const int numeric_limits<int>::max_exponent10;
  const bool numeric_limits<int>::has_infinity;
  const bool numeric_limits<int>::has_quiet_NaN;
  const bool numeric_limits<int>::has_signaling_NaN;
  const float_denorm_style numeric_limits<int>::has_denorm;
  const bool numeric_limits<int>::has_denorm_loss;
  const bool numeric_limits<int>::is_iec559;
  const bool numeric_limits<int>::is_bounded;
  const bool numeric_limits<int>::is_modulo;
  const bool numeric_limits<int>::traps;
  const bool numeric_limits<int>::tinyness_before;
  const float_round_style numeric_limits<int>::round_style;
  #endif

  //***************************************************************************
  // unsigned int
  template<>
  class numeric_limits<unsigned int>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(unsigned int)) - (etl::is_signed<unsigned int>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<unsigned int>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<unsigned int>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const unsigned int min() { return 0; }
    static const unsigned int max() { return UINT_MAX; }
    static const unsigned int lowest() { return 0; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<unsigned int>::is_specialized;
  const int numeric_limits<unsigned int>::digits;
  const int numeric_limits<unsigned int>::digits10;
  const int numeric_limits<unsigned int>::max_digits10;
  const bool numeric_limits<unsigned int>::is_signed;
  const bool numeric_limits<unsigned int>::is_integer;
  const bool numeric_limits<unsigned int>::is_exact;
  const int numeric_limits<unsigned int>::radix;
  const int numeric_limits<unsigned int>::min_exponent;
  const int numeric_limits<unsigned int>::min_exponent10;
  const int numeric_limits<unsigned int>::max_exponent;
  const int numeric_limits<unsigned int>::max_exponent10;
  const bool numeric_limits<unsigned int>::has_infinity;
  const bool numeric_limits<unsigned int>::has_quiet_NaN;
  const bool numeric_limits<unsigned int>::has_signaling_NaN;
  const float_denorm_style numeric_limits<unsigned int>::has_denorm;
  const bool numeric_limits<unsigned int>::has_denorm_loss;
  const bool numeric_limits<unsigned int>::is_iec559;
  const bool numeric_limits<unsigned int>::is_bounded;
  const bool numeric_limits<unsigned int>::is_modulo;
  const bool numeric_limits<unsigned int>::traps;
  const bool numeric_limits<unsigned int>::tinyness_before;
  const float_round_style numeric_limits<unsigned int>::round_style;
  #endif

  //***************************************************************************
  // long
  template<>
  class numeric_limits<long>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(long)) - (etl::is_signed<long>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<long>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<long>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const long min() { return LONG_MIN; }
    static const long max() { return LONG_MAX; }
    static const long lowest() { return LONG_MIN; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<long>::is_specialized;
  const int numeric_limits<long>::digits;
  const int numeric_limits<long>::digits10;
  const int numeric_limits<long>::max_digits10;
  const bool numeric_limits<long>::is_signed;
  const bool numeric_limits<long>::is_integer;
  const bool numeric_limits<long>::is_exact;
  const int numeric_limits<long>::radix;
  const int numeric_limits<long>::min_exponent;
  const int numeric_limits<long>::min_exponent10;
  const int numeric_limits<long>::max_exponent;
  const int numeric_limits<long>::max_exponent10;
  const bool numeric_limits<long>::has_infinity;
  const bool numeric_limits<long>::has_quiet_NaN;
  const bool numeric_limits<long>::has_signaling_NaN;
  const float_denorm_style numeric_limits<long>::has_denorm;
  const bool numeric_limits<long>::has_denorm_loss;
  const bool numeric_limits<long>::is_iec559;
  const bool numeric_limits<long>::is_bounded;
  const bool numeric_limits<long>::is_modulo;
  const bool numeric_limits<long>::traps;
  const bool numeric_limits<long>::tinyness_before;
  const float_round_style numeric_limits<long>::round_style;
  #endif

  //***************************************************************************
  // unsigned long
  template<>
  class numeric_limits<unsigned long>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(unsigned long)) - (etl::is_signed<unsigned long>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<unsigned long>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<unsigned long>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const unsigned long min() { return 0; }
    static const unsigned long max() { return ULONG_MAX; }
    static const unsigned long lowest() { return 0; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<unsigned long>::is_specialized;
  const int numeric_limits<unsigned long>::digits;
  const int numeric_limits<unsigned long>::digits10;
  const int numeric_limits<unsigned long>::max_digits10;
  const bool numeric_limits<unsigned long>::is_signed;
  const bool numeric_limits<unsigned long>::is_integer;
  const bool numeric_limits<unsigned long>::is_exact;
  const int numeric_limits<unsigned long>::radix;
  const int numeric_limits<unsigned long>::min_exponent;
  const int numeric_limits<unsigned long>::min_exponent10;
  const int numeric_limits<unsigned long>::max_exponent;
  const int numeric_limits<unsigned long>::max_exponent10;
  const bool numeric_limits<unsigned long>::has_infinity;
  const bool numeric_limits<unsigned long>::has_quiet_NaN;
  const bool numeric_limits<unsigned long>::has_signaling_NaN;
  const float_denorm_style numeric_limits<unsigned long>::has_denorm;
  const bool numeric_limits<unsigned long>::has_denorm_loss;
  const bool numeric_limits<unsigned long>::is_iec559;
  const bool numeric_limits<unsigned long>::is_bounded;
  const bool numeric_limits<unsigned long>::is_modulo;
  const bool numeric_limits<unsigned long>::traps;
  const bool numeric_limits<unsigned long>::tinyness_before;
  const float_round_style numeric_limits<unsigned long>::round_style;
  #endif

  //***************************************************************************
  // long long
  template<>
  class numeric_limits<long long>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(long long)) - (etl::is_signed<long long>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<long long>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<long long>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const long long min() { return LLONG_MIN; }
    static const long long max() { return LLONG_MAX; }
    static const long long lowest() { return LLONG_MIN; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<long long>::is_specialized;
  const int numeric_limits<long long>::digits;
  const int numeric_limits<long long>::digits10;
  const int numeric_limits<long long>::max_digits10;
  const bool numeric_limits<long long>::is_signed;
  const bool numeric_limits<long long>::is_integer;
  const bool numeric_limits<long long>::is_exact;
  const int numeric_limits<long long>::radix;
  const int numeric_limits<long long>::min_exponent;
  const int numeric_limits<long long>::min_exponent10;
  const int numeric_limits<long long>::max_exponent;
  const int numeric_limits<long long>::max_exponent10;
  const bool numeric_limits<long long>::has_infinity;
  const bool numeric_limits<long long>::has_quiet_NaN;
  const bool numeric_limits<long long>::has_signaling_NaN;
  const float_denorm_style numeric_limits<long long>::has_denorm;
  const bool numeric_limits<long long>::has_denorm_loss;
  const bool numeric_limits<long long>::is_iec559;
  const bool numeric_limits<long long>::is_bounded;
  const bool numeric_limits<long long>::is_modulo;
  const bool numeric_limits<long long>::traps;
  const bool numeric_limits<long long>::tinyness_before;
  const float_round_style numeric_limits<long long>::round_style;
  #endif

  //***************************************************************************
  // unsigned long long
  template<>
  class numeric_limits<unsigned long long>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_integer = true;
    static const bool is_exact = true;

    static const int radix = 2;
    static const char epsilon() { return 0; }
    static const char round_error() { return 0; }

    static const int digits = (CHAR_BIT * sizeof(unsigned long long)) - (etl::is_signed<unsigned long long>::value ? 1 : 0);
    static const int digits10 = ETL_LOG2(digits);
    static const int max_digits10 = 0;

    static const bool is_signed = etl::is_signed<unsigned long long>::value;

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static const unsigned char infinity() { return 0; }
    static const unsigned char  quiet_NaN() { return 0; }
    static const unsigned char  signaling_NaN() { return 0; }
    static const unsigned char  denorm_min() { return 0; }

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = etl::is_unsigned<unsigned long long>::value;

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;

    static const unsigned long long min() { return 0; }
    static const unsigned long long max() { return ULLONG_MAX; }
    static const unsigned long long lowest() { return 0; }
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<unsigned long long>::is_specialized;
  const int numeric_limits<unsigned long long>::digits;
  const int numeric_limits<unsigned long long>::digits10;
  const int numeric_limits<unsigned long long>::max_digits10;
  const bool numeric_limits<unsigned long long>::is_signed;
  const bool numeric_limits<unsigned long long>::is_integer;
  const bool numeric_limits<unsigned long long>::is_exact;
  const int numeric_limits<unsigned long long>::radix;
  const int numeric_limits<unsigned long long>::min_exponent;
  const int numeric_limits<unsigned long long>::min_exponent10;
  const int numeric_limits<unsigned long long>::max_exponent;
  const int numeric_limits<unsigned long long>::max_exponent10;
  const bool numeric_limits<unsigned long long>::has_infinity;
  const bool numeric_limits<unsigned long long>::has_quiet_NaN;
  const bool numeric_limits<unsigned long long>::has_signaling_NaN;
  const float_denorm_style numeric_limits<unsigned long long>::has_denorm;
  const bool numeric_limits<unsigned long long>::has_denorm_loss;
  const bool numeric_limits<unsigned long long>::is_iec559;
  const bool numeric_limits<unsigned long long>::is_bounded;
  const bool numeric_limits<unsigned long long>::is_modulo;
  const bool numeric_limits<unsigned long long>::traps;
  const bool numeric_limits<unsigned long long>::tinyness_before;
  const float_round_style numeric_limits<unsigned long long>::round_style;
  #endif

  //***************************************************************************
  // float
  template<>
  class numeric_limits<float>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_signed = true;
    static const bool is_integer = false;
    static const bool is_exact = false;

    static const int radix = 2;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_indeterminate;
    static const bool has_denorm_loss = false;

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = false;

    static const float round_error() { return float(0.5); }

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_indeterminate;

    static const float min() { return FLT_MIN; }
    static const float max() { return FLT_MAX; }
    static const float lowest() { return -FLT_MAX; }
    static const float epsilon() { return FLT_EPSILON; }
    static const float denorm_min() { return FLT_MIN; }

    static const int digits = FLT_MANT_DIG;
    static const int digits10 = FLT_DIG;
    static const int max_digits10 = ETL_LOG2(FLT_MANT_DIG) + 2;

    static const int min_exponent = FLT_MIN_EXP;
    static const int min_exponent10 = FLT_MIN_10_EXP;
    static const int max_exponent = FLT_MAX_EXP;
    static const int max_exponent10 = FLT_MAX_10_EXP;
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<float>::is_specialized;
  const bool numeric_limits<float>::is_signed;
  const bool numeric_limits<float>::is_integer;
  const bool numeric_limits<float>::is_exact;
  const int numeric_limits<float>::radix;
  const bool numeric_limits<float>::has_infinity;
  const bool numeric_limits<float>::has_quiet_NaN;
  const bool numeric_limits<float>::has_signaling_NaN;
  const float_denorm_style numeric_limits<float>::has_denorm;
  const bool numeric_limits<float>::has_denorm_loss;
  const bool numeric_limits<float>::is_iec559;
  const bool numeric_limits<float>::is_bounded;
  const bool numeric_limits<float>::is_modulo;
  const bool numeric_limits<float>::traps;
  const bool numeric_limits<float>::tinyness_before;
  const float_round_style numeric_limits<float>::round_style;
  const int numeric_limits<float>::digits;
  const int numeric_limits<float>::digits10;
  const int numeric_limits<float>::max_digits10;
  const int numeric_limits<float>::min_exponent;
  const int numeric_limits<float>::min_exponent10;
  const int numeric_limits<float>::max_exponent;
  const int numeric_limits<float>::max_exponent10;
  #endif

  //***************************************************************************
  // double
  template<>
  class numeric_limits<double>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_signed = true;
    static const bool is_integer = false;
    static const bool is_exact = false;

    static const int radix = 2;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_indeterminate;
    static const bool has_denorm_loss = false;

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = false;

    static const float round_error() { return double(0.5); }

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_indeterminate;

    static const double min() { return DBL_MIN; }
    static const double max() { return DBL_MAX; }
    static const double lowest() { return -DBL_MAX; }
    static const double epsilon() { return DBL_EPSILON; }
    static const double denorm_min() { return DBL_MIN; }

    static const int digits = DBL_MANT_DIG;
    static const int digits10 = DBL_DIG;
    static const int max_digits10 = ETL_LOG2(DBL_MANT_DIG) + 2;

    static const int min_exponent = DBL_MIN_EXP;
    static const int min_exponent10 = DBL_MIN_10_EXP;
    static const int max_exponent = DBL_MAX_EXP;
    static const int max_exponent10 = DBL_MAX_10_EXP;
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<double>::is_specialized;
  const bool numeric_limits<double>::is_signed;
  const bool numeric_limits<double>::is_integer;
  const bool numeric_limits<double>::is_exact;
  const int numeric_limits<double>::radix;
  const bool numeric_limits<double>::has_infinity;
  const bool numeric_limits<double>::has_quiet_NaN;
  const bool numeric_limits<double>::has_signaling_NaN;
  const float_denorm_style numeric_limits<double>::has_denorm;
  const bool numeric_limits<double>::has_denorm_loss;
  const bool numeric_limits<double>::is_iec559;
  const bool numeric_limits<double>::is_bounded;
  const bool numeric_limits<double>::is_modulo;
  const bool numeric_limits<double>::traps;
  const bool numeric_limits<double>::tinyness_before;
  const float_round_style numeric_limits<double>::round_style;
  const int numeric_limits<double>::digits;
  const int numeric_limits<double>::digits10;
  const int numeric_limits<double>::max_digits10;
  const int numeric_limits<double>::min_exponent;
  const int numeric_limits<double>::min_exponent10;
  const int numeric_limits<double>::max_exponent;
  const int numeric_limits<double>::max_exponent10;
  #endif

  //***************************************************************************
  // long double
  template<>
  class numeric_limits<long double>
  {
  public:

    static const bool is_specialized = true;

    static const bool is_signed = true;
    static const bool is_integer = false;
    static const bool is_exact = false;

    static const int radix = 2;

    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_indeterminate;
    static const bool has_denorm_loss = false;

    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = false;

    static const float round_error() { return (long double)(0.5); }

    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_indeterminate;

    static const long double min() { return LDBL_MIN; }
    static const long double max() { return LDBL_MAX; }
    static const long double lowest() { return -LDBL_MAX; }
    static const long double epsilon() { return LDBL_EPSILON; }
    static const long double denorm_min() { return LDBL_MIN; }

    static const int digits = LDBL_MANT_DIG;
    static const int digits10 = LDBL_DIG;
    static const int max_digits10 = ETL_LOG2(LDBL_MANT_DIG) + 2;

    static const int min_exponent = LDBL_MIN_EXP;
    static const int min_exponent10 = LDBL_MIN_10_EXP;
    static const int max_exponent = LDBL_MAX_EXP;
    static const int max_exponent10 = LDBL_MAX_10_EXP;
  };

  #if !defined(ETL_COMPILER_MICROSOFT)
  const bool numeric_limits<long double>::is_specialized;
  const bool numeric_limits<long double>::is_signed;
  const bool numeric_limits<long double>::is_integer;
  const bool numeric_limits<long double>::is_exact;
  const int numeric_limits<long double>::radix;
  const bool numeric_limits<long double>::has_infinity;
  const bool numeric_limits<long double>::has_quiet_NaN;
  const bool numeric_limits<long double>::has_signaling_NaN;
  const float_denorm_style numeric_limits<long double>::has_denorm;
  const bool numeric_limits<long double>::has_denorm_loss;
  const bool numeric_limits<long double>::is_iec559;
  const bool numeric_limits<long double>::is_bounded;
  const bool numeric_limits<long double>::is_modulo;
  const bool numeric_limits<long double>::traps;
  const bool numeric_limits<long double>::tinyness_before;
  const float_round_style numeric_limits<long double>::round_style;
  const int numeric_limits<long double>::digits;
  const int numeric_limits<long double>::digits10;
  const int numeric_limits<long double>::max_digits10;
  const int numeric_limits<long double>::min_exponent;
  const int numeric_limits<long double>::min_exponent10;
  const int numeric_limits<long double>::max_exponent;
  const int numeric_limits<long double>::max_exponent10;
  #endif
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
