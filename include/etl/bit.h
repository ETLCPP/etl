///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#ifndef ETL_BIT_CAST_INCLUDED
#define ETL_BIT_CAST_INCLUDED

#include <string.h>

#include "platform.h"
#include "type_traits.h"
#include "binary.h"

namespace etl
{
  //***************************************************************************
  /// bit_cast
  //***************************************************************************
  template <typename TDestination, typename TSource>
  typename etl::enable_if<(sizeof(TDestination) == sizeof(TSource)) &&
                           etl::is_trivially_copyable<TSource>::value &&
                           etl::is_trivially_copyable<TDestination>::value, TDestination>::type
    bit_cast(const TSource& source) ETL_NOEXCEPT
  {
    TDestination destination;

    memcpy(&destination, &source, sizeof(TDestination));

    return destination;
  }

  //template <typename TDestination, typename TSource>
  //ETL_CONSTEXPR
  //typename etl::enable_if<(sizeof(TDestination) == sizeof(TSource))   &&
  //                         etl::is_trivially_copyable<TSource>::value &&
  //                         etl::is_trivially_copyable<TDestination>::value, TDestination>::type
  //  bit_cast(const TSource& source) ETL_NOEXCEPT
  //{
  //  TDestination destination;
  //
  //  __builtin_memcpy(&destination, &source, sizeof(TDestination));
  //
  //  return destination;
  //}

  //***************************************************************************
  /// byteswap
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T>::value, T>::type
    byteswap(T value) ETL_NOEXCEPT
  {
    return 0;
    //return etl::reverse_bytes(value);
  }

  //***************************************************************************
  /// has_single_bit
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, bool>::type has_single_bit(T x) ETL_NOEXCEPT
  {
    return false;
    //return etl::is_power_of_2(x);
  }

  //***************************************************************************
  /// bit_ceil
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    bit_ceil(T x)
  {
    return 0;
  }

  //***************************************************************************
  /// bit_floor
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    bit_floor(T x) ETL_NOEXCEPT
  {
    return 0;
  }

  //***************************************************************************
  /// bit_width
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    bit_width(T x) ETL_NOEXCEPT
  {
    return 0;
  }

  //***************************************************************************
  /// rotl
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    rotl(T x, int s) ETL_NOEXCEPT
  {
    return 0;
  }

  //***************************************************************************
  /// rotr
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, T>::type 
    rotr(T x, int s) ETL_NOEXCEPT
  {
    return 0;
  }

  //***************************************************************************
  /// countl_zero
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type
    countl_zero(T x) ETL_NOEXCEPT
  {
    return 0;
  }
  
  //***************************************************************************
  /// countl_one
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type 
    countl_one(T x) ETL_NOEXCEPT
  {
    return 0;
  }

  //***************************************************************************
  /// countr_zero
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR 
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type
    countr_zero(T x) ETL_NOEXCEPT
  {
    return 0;
    //return etl::count_trailing_zeros(x);
  }
  
  //***************************************************************************
  /// countr_one
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type 
    countr_one(T x) ETL_NOEXCEPT
  {
    return 0;
  }
  
  //***************************************************************************
  /// popcount
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD ETL_CONSTEXPR
    typename etl::enable_if<etl::is_unsigned<T>::value, int>::type
    popcount(T x) ETL_NOEXCEPT
  {
    return 0;
    //return etl::count_bits(x);
  }
}

#endif
