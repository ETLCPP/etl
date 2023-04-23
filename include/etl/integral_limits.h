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
    typedef void value_type;

    static ETL_CONSTANT int min        = 0;
    static ETL_CONSTANT int max        = 0;
    static ETL_CONSTANT int bits       = 0;
    static ETL_CONSTANT bool is_signed = false;
  };

  ETL_CONSTANT int integral_limits<void>::min;
  ETL_CONSTANT int integral_limits<void>::max;
  ETL_CONSTANT int integral_limits<void>::bits;
  ETL_CONSTANT bool integral_limits<void>::is_signed;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<signed char>
  {
    typedef signed char value_type;

    static ETL_CONSTANT signed char min = SCHAR_MIN;
    static ETL_CONSTANT signed char max = SCHAR_MAX;
    static ETL_CONSTANT int bits        = CHAR_BIT;
    static ETL_CONSTANT bool is_signed  = etl::is_signed<signed char>::value;
  };

  ETL_CONSTANT signed char integral_limits<signed char>::min;
  ETL_CONSTANT signed char integral_limits<signed char>::max;
  ETL_CONSTANT int integral_limits<signed char>::bits;
  ETL_CONSTANT bool integral_limits<signed char>::is_signed;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned char>
  {
    typedef unsigned char value_type;

    static ETL_CONSTANT unsigned char min = 0;
    static ETL_CONSTANT unsigned char max = UCHAR_MAX;
    static ETL_CONSTANT int bits          = CHAR_BIT;
    static ETL_CONSTANT bool is_signed    = etl::is_signed<unsigned char>::value;
  };

  ETL_CONSTANT unsigned char integral_limits<unsigned char>::min;
  ETL_CONSTANT unsigned char integral_limits<unsigned char>::max;
  ETL_CONSTANT int integral_limits<unsigned char>::bits;
  ETL_CONSTANT bool integral_limits<unsigned char>::is_signed;

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
    typedef char value_type;

    static ETL_CONSTANT char min       = (etl::is_signed<char>::value) ? SCHAR_MIN : 0;
    static ETL_CONSTANT char max       = (etl::is_signed<char>::value) ? SCHAR_MAX : static_cast<char>(UCHAR_MAX);
    static ETL_CONSTANT int bits       = CHAR_BIT;
    static ETL_CONSTANT bool is_signed = etl::is_signed<char>::value;
  };

  ETL_CONSTANT char integral_limits<char>::min;
  ETL_CONSTANT char integral_limits<char>::max;
  ETL_CONSTANT int integral_limits<char>::bits;
  ETL_CONSTANT bool integral_limits<char>::is_signed;

#if defined(ETL_COMPILER_MICROSOFT)
  #pragma warning(pop)
#endif

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<short>
  {
    typedef short value_type;

    static ETL_CONSTANT short min      = SHRT_MIN;
    static ETL_CONSTANT short max      = SHRT_MAX;
    static ETL_CONSTANT int bits       = CHAR_BIT * (sizeof(short) / sizeof(char));
    static ETL_CONSTANT bool is_signed = etl::is_signed<short>::value;
  };

  ETL_CONSTANT short integral_limits<short>::min;
  ETL_CONSTANT short integral_limits<short>::max;
  ETL_CONSTANT int integral_limits<short>::bits;
  ETL_CONSTANT bool integral_limits<short>::is_signed;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned short>
  {
    typedef unsigned short value_type;

    static ETL_CONSTANT unsigned short min = 0;
    static ETL_CONSTANT unsigned short max = USHRT_MAX;
    static ETL_CONSTANT int bits           = CHAR_BIT * (sizeof(unsigned short) / sizeof(char));
    static ETL_CONSTANT bool is_signed     = etl::is_signed<unsigned short>::value;
  };

  ETL_CONSTANT unsigned short integral_limits<unsigned short>::min;
  ETL_CONSTANT unsigned short integral_limits<unsigned short>::max;
  ETL_CONSTANT int integral_limits<unsigned short>::bits;
  ETL_CONSTANT bool integral_limits<unsigned short>::is_signed;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<int>
  {
    typedef int value_type;

    static ETL_CONSTANT int min        = INT_MIN;
    static ETL_CONSTANT int max        = INT_MAX;
    static ETL_CONSTANT int bits       = CHAR_BIT * (sizeof(int) / sizeof(char));
    static ETL_CONSTANT bool is_signed = etl::is_signed<int>::value;
  };

  ETL_CONSTANT int integral_limits<int>::min;
  ETL_CONSTANT int integral_limits<int>::max;
  ETL_CONSTANT int integral_limits<int>::bits;
  ETL_CONSTANT bool integral_limits<int>::is_signed;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned int>
  {
    typedef unsigned int value_type;

    static ETL_CONSTANT unsigned int min = 0;
    static ETL_CONSTANT unsigned int max = UINT_MAX;
    static ETL_CONSTANT int bits         = CHAR_BIT * (sizeof(unsigned int) / sizeof(char));
    static ETL_CONSTANT bool is_signed   = etl::is_signed<unsigned int>::value;
  };

  ETL_CONSTANT unsigned int integral_limits<unsigned int>::min;
  ETL_CONSTANT unsigned int integral_limits<unsigned int>::max;
  ETL_CONSTANT int integral_limits<unsigned int>::bits;
  ETL_CONSTANT bool integral_limits<unsigned int>::is_signed;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<long>
  {
    typedef long value_type;

    static ETL_CONSTANT long min       = LONG_MIN;
    static ETL_CONSTANT long max       = LONG_MAX;
    static ETL_CONSTANT int bits       = CHAR_BIT * (sizeof(long) / sizeof(char));
    static ETL_CONSTANT bool is_signed = etl::is_signed<long>::value;
  };

  ETL_CONSTANT long integral_limits<long>::min;
  ETL_CONSTANT long integral_limits<long>::max;
  ETL_CONSTANT int integral_limits<long>::bits;
  ETL_CONSTANT bool integral_limits<long>::is_signed;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned long>
  {
    typedef unsigned long value_type;

    static ETL_CONSTANT unsigned long min = 0;
    static ETL_CONSTANT unsigned long max = ULONG_MAX;
    static ETL_CONSTANT int bits          = CHAR_BIT * (sizeof(unsigned long) / sizeof(char));
    static ETL_CONSTANT bool is_signed    = etl::is_signed<unsigned long>::value;
  };

  ETL_CONSTANT unsigned long integral_limits<unsigned long>::min;
  ETL_CONSTANT unsigned long integral_limits<unsigned long>::max;
  ETL_CONSTANT int integral_limits<unsigned long>::bits;
  ETL_CONSTANT bool integral_limits<unsigned long>::is_signed;

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
    typedef long long value_type;

    static ETL_CONSTANT long long min  = LLONG_MIN;
    static ETL_CONSTANT long long max  = LLONG_MAX;
    static ETL_CONSTANT int bits       = CHAR_BIT * (sizeof(long long) / sizeof(char));
    static ETL_CONSTANT bool is_signed = etl::is_signed<long long>::value;
  };

  ETL_CONSTANT long long integral_limits<long long>::min;
  ETL_CONSTANT long long integral_limits<long long>::max;
  ETL_CONSTANT int integral_limits<long long>::bits;
  ETL_CONSTANT bool integral_limits<long long>::is_signed;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned long long>
  {
    typedef unsigned long long value_type;

    static ETL_CONSTANT unsigned long long min = 0;
    static ETL_CONSTANT unsigned long long max = ULLONG_MAX;
    static ETL_CONSTANT int bits               = CHAR_BIT * (sizeof(unsigned long long) / sizeof(char));
    static ETL_CONSTANT bool is_signed         = etl::is_signed<unsigned long long>::value;
  };

  ETL_CONSTANT unsigned long long integral_limits<unsigned long long>::min;
  ETL_CONSTANT unsigned long long integral_limits<unsigned long long>::max;
  ETL_CONSTANT int integral_limits<unsigned long long>::bits;
  ETL_CONSTANT bool integral_limits<unsigned long long>::is_signed;
}

#include "private/minmax_pop.h"

#endif
