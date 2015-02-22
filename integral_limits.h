///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_INTEGRAL_LIMITS__
#define __ETL_INTEGRAL_LIMITS__

#include <climits>
#include <stddef.h>

#include "type_traits.h"

#if WIN32
#undef min
#undef max
#endif

//*****************************************************************************
///\defgroup integral_limits integral_limits
/// A set of templated compile time constants that mirror some of std::numeric_limits funtionality.
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
    static const int min        = 0;
    static const int max        = 0;
    static const int bits       = 0;
    static const bool is_signed = false;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<signed char>
  {
    static const signed char min = SCHAR_MIN;
    static const signed char max = SCHAR_MAX;
    static const int bits        = CHAR_BIT;
    static const bool is_signed  = etl::is_signed<signed char>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned char>
  {
    static const unsigned char min = 0;
    static const unsigned char max = UCHAR_MAX;
    static const int bits          = CHAR_BIT;
    static const bool is_signed    = etl::is_signed<unsigned char>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<char>
  {  
    static const char min       = (etl::is_signed<char>::value) ? SCHAR_MIN : 0;
    static const char max       = (etl::is_signed<char>::value) ? SCHAR_MAX : UCHAR_MAX;
    static const int bits       = CHAR_BIT;
    static const bool is_signed = etl::is_signed<char>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<short>
  {
    static const short min      = SHRT_MIN;
    static const short max      = SHRT_MAX;
    static const int bits       = CHAR_BIT * (sizeof(short) / sizeof(char));
    static const bool is_signed = etl::is_signed<short>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned short>
  {
    static const unsigned short min = 0;
    static const unsigned short max = USHRT_MAX;
    static const int bits           = CHAR_BIT * (sizeof(unsigned short) / sizeof(char));
    static const bool is_signed     = etl::is_signed<unsigned short>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<int>
  {
    static const int min        = INT_MIN;
    static const int max        = INT_MAX;
    static const int bits       = CHAR_BIT * (sizeof(int) / sizeof(char));
    static const bool is_signed = etl::is_signed<int>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned int>
  {
    static const unsigned int min = 0;
    static const unsigned int max = UINT_MAX;
    static const int bits         = CHAR_BIT * (sizeof(unsigned int) / sizeof(char));
    static const bool is_signed   = etl::is_signed<unsigned int>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<long>
  {
    static const long min       = LONG_MIN;
    static const long max       = LONG_MAX;
    static const int bits       = CHAR_BIT * (sizeof(long) / sizeof(char));
    static const bool is_signed = etl::is_signed<long>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned long>
  {
    static const unsigned long min = 0;
    static const unsigned long max = ULONG_MAX;
    static const int bits          = CHAR_BIT * (sizeof(unsigned long) / sizeof(char));
    static const bool is_signed    = etl::is_signed<unsigned long>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<long long>
  {
    static const long long min  = LLONG_MIN;
    static const long long max  = LLONG_MAX;
    static const int bits       = CHAR_BIT * (sizeof(long long) / sizeof(char));
    static const bool is_signed = etl::is_signed<long long>::value;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned long long>
  {
    static const unsigned long long min = 0;
    static const unsigned long long max = ULLONG_MAX;
    static const int bits               = CHAR_BIT * (sizeof(unsigned long long) / sizeof(char));
    static const bool is_signed         = etl::is_signed<unsigned long long>::value;
  };
}

#if WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#endif
