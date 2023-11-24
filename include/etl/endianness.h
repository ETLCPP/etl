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

#ifndef ETL_ENDIAN_INCLUDED
#define ETL_ENDIAN_INCLUDED

#include "platform.h"
#include "enum_type.h"
#include "binary.h"

#include <stdint.h>

#if ETL_USING_CPP20 && ETL_USING_STL
  #include <bit>
#endif

///\defgroup endian endian
/// Constants & utilities for endianness
///\ingroup utilities

// Have we not already defined ETL_ENDIAN_NATIVE?
#if !defined(ETL_ENDIAN_NATIVE)
  // Can we use the C++20 definitions?
  #if ETL_USING_CPP20 && ETL_USING_STL
    #define ETL_ENDIAN_LITTLE static_cast<int>(std::endian::little)
    #define ETL_ENDIAN_BIG    static_cast<int>(std::endian::big)
    #define ETL_ENDIAN_NATIVE static_cast<int>(std::endian::native)
  // Is this the IAR compiler?
  #elif defined(ETL_COMPILER_IAR) && defined(__LITTLE_ENDIAN__)
    #define ETL_ENDIAN_LITTLE 0
    #define ETL_ENDIAN_BIG    1
    #if __LITTLE_ENDIAN__ == 1
      #define ETL_ENDIAN_NATIVE ETL_ENDIAN_LITTLE
    #elif __LITTLE_ENDIAN__ == 0
      #define ETL_ENDIAN_NATIVE ETL_ENDIAN_BIG
    #endif
  // If not can we use the compiler macros?
  #elif defined(__BYTE_ORDER__)
    // Test the two versions of the macro we are likely to see.
    #if defined(__ORDER_LITTLE_ENDIAN__)
      #define ETL_ENDIAN_LITTLE __ORDER_LITTLE_ENDIAN__
      #define ETL_ENDIAN_BIG    __ORDER_BIG_ENDIAN__
      #define ETL_ENDIAN_NATIVE __BYTE_ORDER__
    #elif defined(__LITTLE_ENDIAN__)
      #define ETL_ENDIAN_LITTLE __LITTLE_ENDIAN__
      #define ETL_ENDIAN_BIG    __BIG_ENDIAN__
      #define ETL_ENDIAN_NATIVE __BYTE_ORDER__
    #endif
  #else
    // The user needs to define ETL_ENDIAN_NATIVE.
    #error Unable to determine native endianness at compile time. ETL_ENDIAN_NATIVE must be defined either as 0 for 'little endian' or 1 for 'big endian'.
  #endif
#else
  // Default values for little and big endianness.
  #define ETL_ENDIAN_LITTLE 0
  #define ETL_ENDIAN_BIG    1
#endif

// If true, then the endianness of the platform can be constexpr.
#if (ETL_USING_CPP11 && defined(ETL_ENDIAN_NATIVE))
  #define ETL_HAS_CONSTEXPR_ENDIANNESS 1
#else
  #define ETL_HAS_CONSTEXPR_ENDIANNESS 0
#endif

namespace etl
{
  //***************************************************************************
  /// Constants to denote endianness of operations.
  ///\ingroup endian
  //***************************************************************************
  struct endian
  {
    enum enum_type
    {
      little  = ETL_ENDIAN_LITTLE,
      big     = ETL_ENDIAN_BIG,
      native  = ETL_ENDIAN_NATIVE
    };

    ETL_DECLARE_ENUM_TYPE(endian, int)
    ETL_ENUM_TYPE(little, "little")
    ETL_ENUM_TYPE(big,    "big")
    ETL_END_ENUM_TYPE
  };

  //***************************************************************************
  /// Checks the endianness of the platform.
  ///\ingroup endian
  //***************************************************************************
  struct endianness
  {
    etl::endian operator ()() const
    {
      return etl::endian(*this);
    }

#if ETL_HAS_CONSTEXPR_ENDIANNESS
    ETL_CONSTEXPR
#endif
    operator etl::endian() const
    {
      return get();
    }

#if ETL_HAS_CONSTEXPR_ENDIANNESS
    static ETL_CONSTEXPR etl::endian value()
#else
    static etl::endian value()
#endif
    {
      return get();
    }

  private:

#if ETL_HAS_CONSTEXPR_ENDIANNESS
    static ETL_CONSTEXPR etl::endian get()
    {
      return etl::endian::native;
    }
#else
    static etl::endian get()
    {
      static const uint32_t i = 0xFFFF0000;

      return (*reinterpret_cast<const unsigned char*>(&i) == 0) ? etl::endian::little : etl::endian::big;
    }
#endif
  };

  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value, T>::type
    ntoh(T value)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(value);
    }
    else
    {
      return value;
    }
  }

  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value, T>::type
    hton(T value)
  {
    if (endianness::value() == endian::little)
    {
      return etl::reverse_bytes(value);
    }
    else
    {
      return value;
    }
  }
}

#endif
