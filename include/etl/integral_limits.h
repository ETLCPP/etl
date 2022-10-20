///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_INTEGRAL_LIMITS_INCLUDED
#define ETL_INTEGRAL_LIMITS_INCLUDED

#include "platform.h"
#include "type_traits.h"

#include <stddef.h>
#include <limits.h>

#include "private/minmax_push.h"

//*****************************************************************************
///\defgroup integral_limits integral_limits
/// A set of templated compile time constants that mirror some of std::numeric_limits functionality.
///\ingroup utilities
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <typename T>
  struct integral_limits;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<void>
  {
    static ETL_CONSTANT int min        = 0;
    static ETL_CONSTANT int max        = 0;
    static ETL_CONSTANT int bits       = 0;
    static ETL_CONSTANT bool is_signed = false;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<signed char>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT signed char min = SCHAR_MIN;
    static ETL_CONSTANT signed char max = SCHAR_MAX;
    static ETL_CONSTANT int bits        = CHAR_BIT;
    static ETL_CONSTANT bool is_signed  = etl::is_signed<signed char>::value;
#else
    enum : signed char {
      min = SCHAR_MIN,
      max = SCHAR_MAX
    };
    enum : int { bits = CHAR_BIT };
    enum : bool { is_signed = etl::is_signed<signed char>::value };
#endif
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned char>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT unsigned char min = 0;
    static ETL_CONSTANT unsigned char max = UCHAR_MAX;
    static ETL_CONSTANT int bits          = CHAR_BIT;
    static ETL_CONSTANT bool is_signed    = etl::is_signed<unsigned char>::value;
#else
    enum : unsigned char {
      min = 0,
      max = UCHAR_MAX
    };
    enum : int { bits = CHAR_BIT };
    enum : bool { is_signed = etl::is_signed<unsigned char>::value };
#endif
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
#if defined(ETL_COMPILER_MICROSOFT)
  #pragma warning(push)
  #pragma warning(disable : 4309)
#endif
  template <>
  struct integral_limits<char>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT char min       = (etl::is_signed<char>::value) ? SCHAR_MIN : 0;
    static ETL_CONSTANT char max       = (etl::is_signed<char>::value) ? SCHAR_MAX : static_cast<char>(UCHAR_MAX);
    static ETL_CONSTANT int bits       = CHAR_BIT;
    static ETL_CONSTANT bool is_signed = etl::is_signed<char>::value;
#else
    enum : char {
      min = (etl::is_signed<char>::value) ? SCHAR_MIN : 0,
      max = (etl::is_signed<char>::value) ? SCHAR_MAX : static_cast<char>(UCHAR_MAX)
    };
    enum : int { bits = CHAR_BIT };
    enum : bool { is_signed = etl::is_signed<char>::value };
#endif
  };
#if defined(ETL_COMPILER_MICROSOFT)
  #pragma warning(pop)
#endif

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<short>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT short min      = SHRT_MIN;
    static ETL_CONSTANT short max      = SHRT_MAX;
    static ETL_CONSTANT int bits       = CHAR_BIT * (sizeof(short) / sizeof(char));
    static ETL_CONSTANT bool is_signed = etl::is_signed<short>::value;
#else
    enum : short {
      min = SHRT_MIN,
      max = SHRT_MAX
    };
    enum : int { bits = CHAR_BIT * (sizeof(short) / sizeof(char)) };
    enum : bool { is_signed = etl::is_signed<short>::value };
#endif
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned short>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT unsigned short min = 0;
    static ETL_CONSTANT unsigned short max = USHRT_MAX;
    static ETL_CONSTANT int bits           = CHAR_BIT * (sizeof(unsigned short) / sizeof(char));
    static ETL_CONSTANT bool is_signed     = etl::is_signed<unsigned short>::value;
#else
    enum : unsigned short {
      min = 0,
      max = USHRT_MAX
    };
    enum : int { bits = CHAR_BIT * (sizeof(unsigned short) / sizeof(char)) };
    enum : bool { is_signed = etl::is_signed<unsigned short>::value };
#endif
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<int>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT int min        = INT_MIN;
    static ETL_CONSTANT int max        = INT_MAX;
    static ETL_CONSTANT int bits       = CHAR_BIT * (sizeof(int) / sizeof(char));
    static ETL_CONSTANT bool is_signed = etl::is_signed<int>::value;
#else
    enum : int {
      min = INT_MIN,
      max = INT_MAX
    };
    enum : int { bits = CHAR_BIT * (sizeof(int) / sizeof(char)) };
    enum : bool { is_signed = etl::is_signed<int>::value };
#endif
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned int>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT unsigned int min = 0;
    static ETL_CONSTANT unsigned int max = UINT_MAX;
    static ETL_CONSTANT int bits         = CHAR_BIT * (sizeof(unsigned int) / sizeof(char));
    static ETL_CONSTANT bool is_signed   = etl::is_signed<unsigned int>::value;
#else
    enum : unsigned int {
      min = 0,
      max = UINT_MAX
    };
    enum : int { bits = CHAR_BIT * (sizeof(unsigned int) / sizeof(char)) };
    enum : bool { is_signed = etl::is_signed<unsigned int>::value };
#endif
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<long>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT long min       = LONG_MIN;
    static ETL_CONSTANT long max       = LONG_MAX;
    static ETL_CONSTANT int bits       = CHAR_BIT * (sizeof(long) / sizeof(char));
    static ETL_CONSTANT bool is_signed = etl::is_signed<long>::value;
#else
    enum : long {
      min = LONG_MIN,
      max = LONG_MAX
    };
    enum : int { bits = CHAR_BIT * (sizeof(long) / sizeof(char)) };
    enum : bool { is_signed = etl::is_signed<long>::value };
#endif
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned long>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT unsigned long min = 0;
    static ETL_CONSTANT unsigned long max = ULONG_MAX;
    static ETL_CONSTANT int bits          = CHAR_BIT * (sizeof(unsigned long) / sizeof(char));
    static ETL_CONSTANT bool is_signed    = etl::is_signed<unsigned long>::value;
#else
    enum : unsigned long {
      min = 0,
      max = ULONG_MAX
    };
    enum : int { bits = CHAR_BIT * (sizeof(unsigned long) / sizeof(char)) };
    enum : bool { is_signed = etl::is_signed<unsigned long>::value };
#endif
  };

#ifndef LLONG_MAX
#define LLONG_MAX	9223372036854775807LL
#endif

#ifndef LLONG_MIN
#define LLONG_MIN	(-LLONG_MAX - 1LL)
#endif

#ifndef ULLONG_MAX
#define ULLONG_MAX 18446744073709551615ULL
#endif

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<long long>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT long long min  = LLONG_MIN;
    static ETL_CONSTANT long long max  = LLONG_MAX;
    static ETL_CONSTANT int bits       = CHAR_BIT * (sizeof(long long) / sizeof(char));
    static ETL_CONSTANT bool is_signed = etl::is_signed<long long>::value;
#else
    enum : long long {
      min = LLONG_MIN,
      max = LLONG_MAX
    };
    enum : int { bits = CHAR_BIT * (sizeof(long long) / sizeof(char)) };
    enum : bool { is_signed = etl::is_signed<long long>::value };
#endif
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned long long>
  {
#if ETL_CPP17_SUPPORTED
    static ETL_CONSTANT unsigned long long min = 0;
    static ETL_CONSTANT unsigned long long max = ULLONG_MAX;
    static ETL_CONSTANT int bits               = CHAR_BIT * (sizeof(unsigned long long) / sizeof(char));
    static ETL_CONSTANT bool is_signed         = etl::is_signed<unsigned long long>::value;
#else
    enum : unsigned long long {
      min = 0,
      max = ULLONG_MAX
    };
    enum : int { bits = CHAR_BIT * (sizeof(unsigned long long) / sizeof(char)) };
    enum : bool { is_signed = etl::is_signed<unsigned long long>::value };
#endif
  };
}

#include "private/minmax_pop.h"

#endif
