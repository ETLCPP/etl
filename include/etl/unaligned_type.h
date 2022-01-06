///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 jwellbelove

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

#ifndef ETL_UNALIGNED_TYPE_INCLUDED
#define ETL_UNALIGNED_TYPE_INCLUDED

///\defgroup unaligned_types unaligned_types
/// Unaligned types utilities
///\ingroup utilities

#include "platform.h"
#include "type_traits.h"
#include "endianness.h"

#include <string.h>

namespace etl
{
  template <typename T, int Endian, typename = typename etl::enable_if<etl::is_fundamental<T>::value && (Endian != etl::endian::native), void>::type>
  class unaligned_type
  {
  public:

    typedef T type;

    static ETL_CONSTANT size_t Size = sizeof(T);
    static ETL_CONSTANT int endian_type = Endian;

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    unaligned_type()
    {
      T value = T(0);

      memcpy(storage, &value, Size);
    }

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    unaligned_type(T value)
    {
      // Not same as host?
      if (Endian != etl::endianness::value())
      {
        value = etl::reverse_bytes(value);
      }

      memcpy(storage, &value, Size);
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    unaligned_type(const unaligned_type& other)
    {
      memcpy(storage, other.storage, Size);
    }

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    unaligned_type& operator =(const unaligned_type& other)
    {
      memcpy(storage, other.storage, Size);

      return *this;
    }

    //*************************************************************************
    /// Assignment operator from other endianness.
    //*************************************************************************
    template <int Endian_Other>
    unaligned_type& operator =(const unaligned_type<T, Endian_Other>& other)
    {
      memcpy(storage, other.storage, Size);

      // Not same?
      if (Endian != Endian_Other)
      {
        etl::reverse(storage, storage + Size);
      }
    }

    //*************************************************************************
    /// Equality operator
    //*************************************************************************
    friend bool operator ==(const unaligned_type& lhs, const unaligned_type& rhs)
    {
      return (memcmp(lhs.storage, rhs.storage, Size) == 0);
    }

    //*************************************************************************
    /// Inequality operator
    //*************************************************************************
    friend bool operator !=(const unaligned_type& lhs, const unaligned_type& rhs)
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Conversion operator
    //*************************************************************************
    operator T() const
    {
      T value;

      memcpy(&value, storage, Size);

      // Same as host?
      if (Endian != etl::endianness::value())
      {
        value = etl::reverse_bytes(value);
      }

      return value;
    }

  private:

    char storage[Size];
  };





  //template <typename T, typename D = typename etl::enable_if<etl::is_fundamental<T>::value, void>::type>
  //class unaligned_type<T, etl::endian::native, D>
  //{
  //public:

  //  typedef T type;

  //  static ETL_CONSTANT size_t Size = Size;
  //  static ETL_CONSTANT etl::endianness endian_type = etl::endianness::get();

  //  //*************************************************************************
  //  /// Default constructor
  //  //*************************************************************************
  //  unaligned_type()
  //  {
  //    T value = T(0);

  //    memcpy(storage, &value, Size);
  //  }

  //  //*************************************************************************
  //  /// Constructor
  //  //*************************************************************************
  //  unaligned_type(T value)
  //  {
  //    memcpy(storage, &value, Size);
  //  }

  //  //*************************************************************************
  //  /// Copy constructor
  //  //*************************************************************************
  //  unaligned_type(const unaligned_type& other)
  //  {
  //    memcpy(storage, other.storage, Size);
  //  }

  //  //*************************************************************************
  //  /// Assignment operator
  //  //*************************************************************************
  //  unaligned_type& operator =(const unaligned_type& other)
  //  {
  //    memcpy(storage, other.storage, Size);
  //  }

  //  //*************************************************************************
  //  /// Equality operator
  //  //*************************************************************************
  //  friend bool operator ==(const unaligned_type& lhs, const unaligned_type& rhs)
  //  {
  //    return (memcmp(lhs.storage, rhs.storage, Size) == 0);
  //  }

  //  //*************************************************************************
  //  /// Inequality operator
  //  //*************************************************************************
  //  friend bool operator !=(const unaligned_type& lhs, const unaligned_type& rhs)
  //  {
  //    return !(lhs == rhs);
  //  }

  //  //*************************************************************************
  //  /// Conversion operator
  //  //*************************************************************************
  //  operator T() const
  //  {
  //    T value;

  //    memcpy(&value, storage, Size);

  //    return value;
  //  }

  //private:

  //  char storage[Size];
  //};















  // Network Order
  typedef unaligned_type<char,               etl::endian::big> net_char_t;
  typedef unaligned_type<signed char,        etl::endian::big> net_schar_t;
  typedef unaligned_type<unsigned char,      etl::endian::big> net_uchar_t;
  typedef unaligned_type<short,              etl::endian::big> net_shor_t;
  typedef unaligned_type<unsigned short,     etl::endian::big> net_unsigned_shor_t;
  typedef unaligned_type<int,                etl::endian::big> net_in_t;
  typedef unaligned_type<unsigned int,       etl::endian::big> net_unsigned_in_t;
  typedef unaligned_type<long,               etl::endian::big> net_long_t;
  typedef unaligned_type<unsigned long,      etl::endian::big> net_unsigned_long_t;
  typedef unaligned_type<long long,          etl::endian::big> net_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::big> net_unsigned_long_long_t;
#if ETL_8BIT_SUPPORT
  typedef unaligned_type<int8_t,             etl::endian::big> net_int8_t;
  typedef unaligned_type<uint8_t,            etl::endian::big> net_uint8_t;
#endif
  typedef unaligned_type<int16_t,            etl::endian::big> net_int16_t;
  typedef unaligned_type<uint16_t,           etl::endian::big> net_uint16_t;
  typedef unaligned_type<int32_t,            etl::endian::big> net_int32_t;
  typedef unaligned_type<uint32_t,           etl::endian::big> net_uint32_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endian::big> net_int64_t;
  typedef unaligned_type<uint64_t,           etl::endian::big> net_uint64_t;
#endif
  typedef unaligned_type<float,              etl::endian::big> net_floa_t;
  typedef unaligned_type<double,             etl::endian::big> net_double_t;
  typedef unaligned_type<long double,        etl::endian::big> net_long_double_t;

//#if ETL_CPP11_SUPPORTED
//  // Host order
//  typedef unaligned_type<char,               int(etl::endianness::get())> host_char_t;
//  typedef unaligned_type<signed char,        etl::endianness::get()> host_schar_t;
//  typedef unaligned_type<unsigned char,      etl::endianness::get()> host_uchar_t;
//  typedef unaligned_type<short,              etl::endianness::get()> host_shor_t;
//  typedef unaligned_type<unsigned short,     etl::endianness::get()> host_unsigned_shor_t;
//  typedef unaligned_type<int,                etl::endianness::get()> host_in_t;
//  typedef unaligned_type<unsigned int,       etl::endianness::get()> host_unsigned_in_t;
//  typedef unaligned_type<long,               etl::endianness::get()> host_long_t;
//  typedef unaligned_type<unsigned long,      etl::endianness::get()> host_unsigned_long_t;
//  typedef unaligned_type<long long,          etl::endianness::get()> host_long_long_t;
//  typedef unaligned_type<unsigned long long, etl::endianness::get()> host_unsigned_long_long_t;
//#if ETL_8BIT_SUPPORT
//  typedef unaligned_type<int8_t,             etl::endianness::get()> host_int8_t;
//  typedef unaligned_type<uint8_t,            etl::endianness::get()> host_uint8_t;
//#endif
//  typedef unaligned_type<int16_t,            etl::endianness::get()> host_int16_t;
//  typedef unaligned_type<uint16_t,           etl::endianness::get()> host_uint16_t;
//  typedef unaligned_type<int32_t,            etl::endianness::get()> host_int32_t;
//  typedef unaligned_type<uint32_t,           etl::endianness::get()> host_uint32_t;
//#if ETL_USING_64BIT_TYPES
//  typedef unaligned_type<int64_t,            etl::endianness::get()> host_int64_t;
//  typedef unaligned_type<uint64_t,           etl::endianness::get()> host_uint64_t;
//#endif
//  typedef unaligned_type<float,              etl::endianness::get()> host_floa_t;
//  typedef unaligned_type<double,             etl::endianness::get()> host_double_t;
//  typedef unaligned_type<long double,        etl::endianness::get()> host_long_double_t;
//#endif

  //template <>
  //unaligned_type<char, etl::endian::native>

#if ETL_CPP11_SUPPORTED
  template <typename T, int Endian>
  using unaligned_type_t = typename etl::unaligned_type<T, Endian>::type;
#endif

#if ETL_CPP17_SUPPORTED
  template <typename T>
  constexpr size_t unaligned_type_v = etl::unaligned_type<T, Endian>::Size;
#endif
}

#endif
