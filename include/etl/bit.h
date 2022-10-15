///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#ifndef ETL_BIT_INCLUDED
#define ETL_BIT_INCLUDED

#include "platform.h"
#include "type_traits.h"
#include "binary.h"
#include "integral_limits.h"
#include "endianness.h"
#include "type_traits.h"

#include <string.h>

#if ETL_USING_CPP20 && ETL_USING_STL
  #include <bit>
#endif

namespace etl
{
  //***************************************************************************
  /// bit_cast
  //***************************************************************************
  template <typename TDestination, typename TSource>
  ETL_CONSTEXPR14
  typename etl::enable_if<(sizeof(TDestination) == sizeof(TSource)) &&
                           etl::is_trivially_copyable<TSource>::value &&
                           etl::is_trivially_copyable<TDestination>::value, TDestination>::type
    bit_cast(const TSource& source) ETL_NOEXCEPT
  {
    TDestination destination;

    memcpy(&destination, &source, sizeof(TDestination));

    return destination;
  }

  //***************************************************************************
  /// byteswap
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value, T>::type
    byteswap(T value) ETL_NOEXCEPT
  {
    return etl::reverse_bytes(value);
  }

  //***************************************************************************
  /// has_single_bit
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, bool>::type has_single_bit(T value) ETL_NOEXCEPT
  {
    return (value & (value - 1)) == 0;
  }
  //***************************************************************************
  /// countl_zero
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type
    countl_zero(T value) ETL_NOEXCEPT
  {
    return etl::count_leading_zeros(value);
  }

  //***************************************************************************
  /// countl_one
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type
    countl_one(T value) ETL_NOEXCEPT
  {
    return etl::count_leading_ones(value);
  }

  //***************************************************************************
  /// countr_zero
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type
    countr_zero(T value) ETL_NOEXCEPT
  {
    return etl::count_trailing_zeros(value);
  }

  //***************************************************************************
  /// countr_one
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type
    countr_one(T value) ETL_NOEXCEPT
  {
    return etl::count_trailing_ones(value);;
  }


  //***************************************************************************
  /// bit_width
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    bit_width(T value) ETL_NOEXCEPT
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::bit_width(value);
#else
    return etl::integral_limits<T>::bits - etl::countl_zero(value);
#endif
  }

  //***************************************************************************
  /// bit_ceil
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    bit_ceil(T value)
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::bit_ceil(value);
#else
    if (value == T(0))
    {
      return T(1);
    }
    else
    {
      return T(1) << etl::bit_width(T(value - T(1)));
    }
#endif
  }

  //***************************************************************************
  /// bit_floor
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    bit_floor(T value) ETL_NOEXCEPT
  {
#if __cpp_lib_bitops && ETL_USING_STL
    return std::bit_floor(value);
#else
    if (value == T(0))
    {
      return T(0);
    }
    else
    {
      return T(1) << (etl::bit_width(value) - T(1));
    }
#endif
  }

  //***************************************************************************
  /// rotl
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    rotl(T value, int n) ETL_NOEXCEPT
  {
    if (n < 0)
    {
      return etl::rotate_right(value, -n);
    }
    else
    {
      return etl::rotate_left(value, n);
    }
  }

  //***************************************************************************
  /// rotr
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type 
    rotr(T value, int n) ETL_NOEXCEPT
  {
    if (n < 0)
    {
      return etl::rotate_left(value, -n);
    }
    else
    {
      return etl::rotate_right(value, n);
    }
  }
 
  //***************************************************************************
  /// popcount
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type
    popcount(T value) ETL_NOEXCEPT
  {
    return etl::count_bits(value);
  }
}

#endif
