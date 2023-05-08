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

#ifndef LLONG_MAX
  #define LLONG_MAX	9223372036854775807LL
#endif

#ifndef LLONG_MIN
  #define LLONG_MIN	(-LLONG_MAX - 1LL)
#endif

#ifndef ULLONG_MAX
  #define ULLONG_MAX 18446744073709551615ULL
#endif

namespace etl
{
  namespace private_integral_limits
  {
    //*****************************************************************************
    /// The technique of using templated base classes to declare and define the statics
    /// is to get around the limits of the One Definition Rule (ODR) that occurs when
    /// the top level integral_limits classes are specialised. This issue currently seems
    /// to only be apparent for GCC/C++14.
    /// In C++17 and above this is not necessary, as the inline keyword may be used.
    /// At the time of writing, the ETL is generally aimed to be compatible with C++98 and above.
    //*****************************************************************************
    
    //*********************************
    // signed char
    template <typename T = void>
    struct statics_signed_char
    {
      typedef signed char value_type;

      static ETL_CONSTANT signed char min       = SCHAR_MIN;
      static ETL_CONSTANT signed char max       = SCHAR_MAX;
      static ETL_CONSTANT int         bits      = CHAR_BIT;
      static ETL_CONSTANT bool        is_signed = etl::is_signed<signed char>::value;
    };

    template <typename T>
    ETL_CONSTANT signed char statics_signed_char<T>::min;
    
    template <typename T>
    ETL_CONSTANT signed char statics_signed_char<T>::max;
    
    template <typename T>
    ETL_CONSTANT int statics_signed_char<T>::bits;
    
    template <typename T>
    ETL_CONSTANT bool statics_signed_char<T>::is_signed;

    //***********************************
    // unsigned char
    template <typename T = void>
    struct statics_unsigned_char
    {
      typedef unsigned char value_type;

      static ETL_CONSTANT unsigned char min       = 0;
      static ETL_CONSTANT unsigned char max       = UCHAR_MAX;
      static ETL_CONSTANT int           bits      = CHAR_BIT;
      static ETL_CONSTANT bool          is_signed = etl::is_signed<unsigned char>::value;
    };

    template <typename T>
    ETL_CONSTANT unsigned char statics_unsigned_char<T>::min;

    template <typename T>
    ETL_CONSTANT unsigned char statics_unsigned_char<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_unsigned_char<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_unsigned_char<T>::is_signed;

    //***********************************
    // char
    template <typename T = void>
    struct statics_char
    {
      typedef char value_type;

      static ETL_CONSTANT char min       = (etl::is_signed<char>::value) ? SCHAR_MIN : 0;
      static ETL_CONSTANT char max       = (etl::is_signed<char>::value) ? SCHAR_MAX : static_cast<char>(UCHAR_MAX);
      static ETL_CONSTANT int  bits      = CHAR_BIT;
      static ETL_CONSTANT bool is_signed = etl::is_signed<char>::value;
    };

    template <typename T>
    ETL_CONSTANT char statics_char<T>::min;

    template <typename T>
    ETL_CONSTANT char statics_char<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_char<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_char<T>::is_signed;

    //***********************************
    // wchar_t
    template <typename T = void>
    struct statics_wchar_t
    {
      typedef wchar_t value_type;

      static ETL_CONSTANT wchar_t min       = WCHAR_MIN;
      static ETL_CONSTANT wchar_t max       = WCHAR_MAX;
      static ETL_CONSTANT int     bits      = CHAR_BIT * sizeof(wchar_t);
      static ETL_CONSTANT bool    is_signed = etl::is_signed<wchar_t>::value;
    };

    template <typename T>
    ETL_CONSTANT wchar_t statics_wchar_t<T>::min;

    template <typename T>
    ETL_CONSTANT wchar_t statics_wchar_t<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_wchar_t<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_wchar_t<T>::is_signed;

    //***********************************
    // short
#if defined(ETL_COMPILER_MICROSOFT)
  #pragma warning(push)
  #pragma warning(disable : 4309)
#endif

    template <typename T = void>
    struct statics_short
    {
      typedef short value_type;

      static ETL_CONSTANT short min       = SHRT_MIN;
      static ETL_CONSTANT short max       = SHRT_MAX;
      static ETL_CONSTANT int   bits      = CHAR_BIT * (sizeof(short) / sizeof(char));
      static ETL_CONSTANT bool  is_signed = etl::is_signed<short>::value;
    };

    template <typename T>
    ETL_CONSTANT short statics_short<T>::min;

    template <typename T>
    ETL_CONSTANT short statics_short<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_short<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_short<T>::is_signed;

#if defined(ETL_COMPILER_MICROSOFT)
  #pragma warning(pop)
#endif

    //***********************************
    // unsigned short
    template <typename T = void>
    struct statics_unsigned_short
    {
      typedef unsigned short value_type;

      static ETL_CONSTANT unsigned short min       = 0;
      static ETL_CONSTANT unsigned short max       = USHRT_MAX;
      static ETL_CONSTANT int            bits      = CHAR_BIT * (sizeof(unsigned short) / sizeof(char));
      static ETL_CONSTANT bool           is_signed = etl::is_signed<unsigned short>::value;
    };

    template <typename T>
    ETL_CONSTANT unsigned short statics_unsigned_short<T>::min;

    template <typename T>
    ETL_CONSTANT unsigned short statics_unsigned_short<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_unsigned_short<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_unsigned_short<T>::is_signed;

    //***********************************
    // int
    template <typename T = void>
    struct statics_int
    {
      typedef int value_type;

      static ETL_CONSTANT int  min       = INT_MIN;
      static ETL_CONSTANT int  max       = INT_MAX;
      static ETL_CONSTANT int  bits      = CHAR_BIT * (sizeof(int) / sizeof(char));
      static ETL_CONSTANT bool is_signed = etl::is_signed<int>::value;
    };

    template <typename T>
    ETL_CONSTANT int statics_int<T>::min;

    template <typename T>
    ETL_CONSTANT int statics_int<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_int<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_int<T>::is_signed;

    //***********************************
    // unsigned int
    template <typename T = void>
    struct statics_unsigned_int
    {
      typedef unsigned int value_type;

      static ETL_CONSTANT unsigned int min       = 0;
      static ETL_CONSTANT unsigned int max       = UINT_MAX;
      static ETL_CONSTANT int          bits      = CHAR_BIT * (sizeof(unsigned int) / sizeof(char));
      static ETL_CONSTANT bool         is_signed = etl::is_signed<unsigned int>::value;
    };

    template <typename T>
    ETL_CONSTANT unsigned int statics_unsigned_int<T>::min;

    template <typename T>
    ETL_CONSTANT unsigned int statics_unsigned_int<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_unsigned_int<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_unsigned_int<T>::is_signed;

    //***********************************
    // long
    template <typename T = void>
    struct statics_long
    {
      typedef long value_type;

      static ETL_CONSTANT long min       = LONG_MIN;
      static ETL_CONSTANT long max       = LONG_MAX;
      static ETL_CONSTANT int  bits      = CHAR_BIT * (sizeof(long) / sizeof(char));
      static ETL_CONSTANT bool is_signed = etl::is_signed<long>::value;
    };

    template <typename T>
    ETL_CONSTANT long statics_long<T>::min;

    template <typename T>
    ETL_CONSTANT long statics_long<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_long<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_long<T>::is_signed;

    //***********************************
    // unsigned long
    template <typename T = void>
    struct statics_unsigned_long
    {
      typedef unsigned long value_type;

      static ETL_CONSTANT unsigned long min       = 0;
      static ETL_CONSTANT unsigned long max       = ULONG_MAX;
      static ETL_CONSTANT int           bits      = CHAR_BIT * (sizeof(unsigned long) / sizeof(char));
      static ETL_CONSTANT bool          is_signed = etl::is_signed<unsigned long>::value;
    };

    template <typename T>
    ETL_CONSTANT unsigned long statics_unsigned_long<T>::min;

    template <typename T>
    ETL_CONSTANT unsigned long statics_unsigned_long<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_unsigned_long<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_unsigned_long<T>::is_signed;

    //***********************************
    // long long
    template <typename T = void>
    struct statics_long_long
    {
      typedef long long value_type;

      static ETL_CONSTANT long long min       = LLONG_MIN;
      static ETL_CONSTANT long long max       = LLONG_MAX;
      static ETL_CONSTANT int       bits      = CHAR_BIT * (sizeof(long long) / sizeof(char));
      static ETL_CONSTANT bool      is_signed = etl::is_signed<long long>::value;
    };

    template <typename T>
    ETL_CONSTANT long long statics_long_long<T>::min;

    template <typename T>
    ETL_CONSTANT long long statics_long_long<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_long_long<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_long_long<T>::is_signed;

    //***********************************
    // unsigned long long
    template <typename T = void>
    struct statics_unsigned_long_long
    {
      typedef unsigned long value_type;

      static ETL_CONSTANT unsigned long long min       = 0;
      static ETL_CONSTANT unsigned long long max       = ULLONG_MAX;
      static ETL_CONSTANT int                bits      = CHAR_BIT * (sizeof(unsigned long long) / sizeof(char));
      static ETL_CONSTANT bool               is_signed = etl::is_signed<unsigned long long>::value;
    };

    template <typename T>
    ETL_CONSTANT unsigned long long statics_unsigned_long_long<T>::min;

    template <typename T>
    ETL_CONSTANT unsigned long long statics_unsigned_long_long<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_unsigned_long_long<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_unsigned_long_long<T>::is_signed;

#if ETL_HAS_NATIVE_CHAR8_T
    //***********************************
    // char8_t
    template <typename T = void>
    struct statics_char8_t
    {
      typedef char8_t value_type;

      static ETL_CONSTANT char8_t min       = (etl::is_signed<char8_t>::value) ? SCHAR_MIN : 0;
      static ETL_CONSTANT char8_t max       = (etl::is_signed<char8_t>::value) ? SCHAR_MAX : static_cast<char8_t>(UCHAR_MAX);
      static ETL_CONSTANT int     bits      = CHAR_BIT;
      static ETL_CONSTANT bool    is_signed = etl::is_signed<char8_t>::value;
    };

    template <typename T>
    ETL_CONSTANT char8_t statics_char8_t<T>::min;

    template <typename T>
    ETL_CONSTANT char8_t statics_char8_t<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_char8_t<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_char8_t<T>::is_signed;
#endif

#if ETL_HAS_NATIVE_CHAR16_T
    //***********************************
    // char16_t
    template <typename T = void>
    struct statics_char16_t
    {
      typedef char16_t value_type;

      static ETL_CONSTANT char16_t min       = 0;
      static ETL_CONSTANT char16_t max       = 0xFFFFU;
      static ETL_CONSTANT int      bits      = 16;
      static ETL_CONSTANT bool     is_signed = false;
    };

    template <typename T>
    ETL_CONSTANT char16_t statics_char16_t<T>::min;

    template <typename T>
    ETL_CONSTANT char16_t statics_char16_t<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_char16_t<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_char16_t<T>::is_signed;
#endif

#if ETL_HAS_NATIVE_CHAR32_T
    //***********************************
    // char32_t
    template <typename T = void>
    struct statics_char32_t
    {
      typedef char32_t value_type;

      static ETL_CONSTANT char32_t min       = 0;
      static ETL_CONSTANT char32_t max       = 0xFFFFFFFFU;
      static ETL_CONSTANT int      bits      = 32;
      static ETL_CONSTANT bool     is_signed = false;
    };
     
    template <typename T>
    ETL_CONSTANT char32_t statics_char32_t<T>::min;

    template <typename T>
    ETL_CONSTANT char32_t statics_char32_t<T>::max;

    template <typename T>
    ETL_CONSTANT int statics_char32_t<T>::bits;

    template <typename T>
    ETL_CONSTANT bool statics_char32_t<T>::is_signed;
#endif
  }

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <typename T>
  struct integral_limits;

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<signed char> : public private_integral_limits::statics_signed_char<>
  {
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned char> : public private_integral_limits::statics_unsigned_char<>
  {
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************

  template <>
  struct integral_limits<char> : public private_integral_limits::statics_char<>
  {
    typedef char value_type;
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<short> : public private_integral_limits::statics_short<>
  {
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned short> : public private_integral_limits::statics_unsigned_short<>
  {
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<int> : public private_integral_limits::statics_int<>
  {
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned int> : public private_integral_limits::statics_unsigned_int<>
  {
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<long> : public private_integral_limits::statics_long<>
  {
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned long> : public private_integral_limits::statics_unsigned_long<>
  {
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<long long> : public private_integral_limits::statics_long_long<>
  {    
  };

  //***************************************************************************
  ///\ingroup integral_limits
  //***************************************************************************
  template <>
  struct integral_limits<unsigned long long> : public private_integral_limits::statics_unsigned_long_long<>
  {
  };
}

#include "private/minmax_pop.h"

#endif
