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

namespace etl
{
  template <typename T, etl::endian Endian, typename = typename etl::enable_if<etl::is_fundamental<T>::value, void>::type>
  class unaligned_type
  {
  public:

    typedef type T;

    static ETL_CONSTANT size_t Size = sizeof(T);

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    unaligned_type()
    {
      T value = T(0);

      mempy(storage, &value, sizeof(T));
    }

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    unaligned_type(T value)
    {
      // Same as host?
      if (Endian != etl::endianness::get())
      {
        value = etl::reverse_bytes(value);
      }

      mempy(storage, &value, sizeof(T));
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    unaligned_type(const unaligned_type& other)
    {
      mempy(storage, other.storage, sizeof(T));
    }

    //*************************************************************************
    /// Assignment operator
    //*************************************************************************
    unaligned_type& operator =(const unaligned_type& other)
    {
      mempy(storage, other.storage, sizeof(T));
    }

    //*************************************************************************
    /// Equality operator
    //*************************************************************************
    friend bool operator ==(const unaligned_type& lhs, const unaligned_type& rhs)
    {
      return memcmp(lhs.storage, rhs.storage, sizeof(T)) == 0;
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

      mempy(&value, storage, sizeof(T));

      // Same as host?
      if (Endian != etl::endianness::get())
      {
        value = etl::reverse_bytes(value);
      }

      return value;
    }

  private:

    char storage[sizeof(T)];
  };

  // Network Order
  typedef unaligned_type<char,               etl::endian::big> net_char_t;
  typedef unaligned_type<signed char,        etl::endian::big> net_schar_t;
  typedef unaligned_type<unsigned char,      etl::endian::big> net_uchar_t;
  typedef unaligned_type<short,              etl::endian::big> net_short_t;
  typedef unaligned_type<unsigned short,     etl::endian::big> net_unsigned_short_t;
  typedef unaligned_type<int,                etl::endian::big> net_int_t;
  typedef unaligned_type<unsigned int,       etl::endian::big> net_unsigned_int_t;
  typedef unaligned_type<long,               etl::endian::big> net_long_t;
  typedef unaligned_type<unsigned long,      etl::endian::big> net_unsigned_long_t;
  typedef unaligned_type<long long,          etl::endian::big> net_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::big> net_unsigned_long_long_t;
#if ETL_8BIT_SUPPORT
  typedef unaligned_type<int8_t,             etl::endian::big> net_int8_t_t;
  typedef unaligned_type<uint8_t,            etl::endian::big> net_uint8_t_t;
#endif
  typedef unaligned_type<int16_t,            etl::endian::big> net_int16_t_t;
  typedef unaligned_type<uint16_t,           etl::endian::big> net_uint16_t_t;
  typedef unaligned_type<int32_t,            etl::endian::big> net_int32_t_t;
  typedef unaligned_type<uint32_t,           etl::endian::big> net_uint32_t_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endian::big> net_int64_t_t;
  typedef unaligned_type<uint64_t,           etl::endian::big> net_uint64_t_t;
#endif
  typedef unaligned_type<float,              etl::endian::big> net_float_t;
  typedef unaligned_type<double,             etl::endian::big> net_double_t;
  typedef unaligned_type<long double,        etl::endian::big> net_long_double_t;

#if ETL_CPP11_SUPPORTED
  // Host order
  typedef unaligned_type<char,               etl::endian::get()> host_char_t;
  typedef unaligned_type<signed char,        etl::endian::get()> host_schar_t;
  typedef unaligned_type<unsigned char,      etl::endian::get()> host_uchar_t;
  typedef unaligned_type<short,              etl::endian::get()> host_short_t;
  typedef unaligned_type<unsigned short,     etl::endian::get()> host_unsigned_short_t;
  typedef unaligned_type<int,                etl::endian::get()> host_int_t;
  typedef unaligned_type<unsigned int,       etl::endian::get()> host_unsigned_int_t;
  typedef unaligned_type<long,               etl::endian::get()> host_long_t;
  typedef unaligned_type<unsigned long,      etl::endian::get()> host_unsigned_long_t;
  typedef unaligned_type<long long,          etl::endian::get()> host_long_long_t;
  typedef unaligned_type<unsigned long long, etl::endian::get()> host_unsigned_long_long_t;
#if ETL_8BIT_SUPPORT
  typedef unaligned_type<int8_t,             etl::endian::get()> host_int8_t_t;
  typedef unaligned_type<uint8_t,            etl::endian::get()> host_uint8_t_t;
#endif
  typedef unaligned_type<int16_t,            etl::endian::get()> host_int16_t_t;
  typedef unaligned_type<uint16_t,           etl::endian::get()> host_uint16_t_t;
  typedef unaligned_type<int32_t,            etl::endian::get()> host_int32_t_t;
  typedef unaligned_type<uint32_t,           etl::endian::get()> host_uint32_t_t;
#if ETL_USING_64BIT_TYPES
  typedef unaligned_type<int64_t,            etl::endian::get()> host_int64_t_t;
  typedef unaligned_type<uint64_t,           etl::endian::get()> host_uint64_t_t;
#endif
  typedef unaligned_type<float,              etl::endian::get()> host_float_t;
  typedef unaligned_type<double,             etl::endian::get()> host_double_t;
  typedef unaligned_type<long double,        etl::endian::get()> host_long_double_t;
#endif

#if ETL_CPP11_SUPPORTED
  template <typename T>
  using unaligned_type_t = typename etl::unaligned_type<T>::type;
#endif
}

#endif
