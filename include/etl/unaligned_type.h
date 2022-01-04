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

    //*************************************************************************
    /// 
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
    /// 
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

  typedef unaligned_type<char, etl::endian::big>          net_char_t;
  typedef unaligned_type<signed char, etl::endian::big>   net_schar_t;
  typedef unaligned_type<unsigned char, etl::endian::big> net_uchar_t;
  typedef unaligned_type<int32_t, etl::endian::big>       net_int32_t_t;
  typedef unaligned_type<uint32_t, etl::endian::big>      net_uint32_t_t;

#if ETL_CPP11_SUPPORTED
  typedef unaligned_type<char, etl::endian::get()>          host_char_t;
  typedef unaligned_type<signed char, etl::endian::get()>   host_schar_t;
  typedef unaligned_type<unsigned char, etl::endian::get()> host_uchar_t;
  typedef unaligned_type<int32_t, etl::endian::get()>       host_int32_t_t;
  typedef unaligned_type<uint32_t, etl::endian::get()>      host_uint32_t_t;
#endif

#if ETL_CPP11_SUPPORTED
  template <typename T>
  using unaligned_type_t = typename etl::unaligned_type<T>::type
#endif
}

#endif
