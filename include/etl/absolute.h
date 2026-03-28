///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#ifndef ETL_ABSOLUTE_INCLUDED
#define ETL_ABSOLUTE_INCLUDED

#include "platform.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "error_handler.h"

namespace etl
{
  namespace private_absolute
  {
    //*************************************************************************
    // Non-constexpr function that is never called for valid inputs.
    // If reached during constant evaluation, the compiler emits an error
    // because it's not constexpr.
    // At runtime, triggers the ETL assert handler.
    //*************************************************************************
    template <typename T>
    inline T signed_min_error()
    {
      ETL_ASSERT_FAIL(ETL_ERROR_GENERIC("absolute value of minimum signed integer is undefined"));
      return T(0);
    }
  }

  //***************************************************************************
  // For signed types.
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_signed<T>::value && etl::is_integral<T>::value, T>::type
    absolute(T value)
  {
    return (value == etl::integral_limits<T>::min) ? etl::private_absolute::signed_min_error<T>()
                                                   : static_cast<T>((value < T(0)) ? -value : value);
  }

  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_signed<T>::value && !etl::is_integral<T>::value, T>::type
    absolute(T value) ETL_NOEXCEPT
  {
    return (value < T(0)) ? -value : value;
  }

  //***************************************************************************
  // For unsigned types.
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    absolute(T value) ETL_NOEXCEPT
  {
    return value;
  }

  //***************************************************************************
  // For signed types.
  // Returns the result as the unsigned type.
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR 
  typename etl::enable_if<etl::is_signed<T>::value, typename etl::make_unsigned<T>::type>::type
    absolute_unsigned(T value) ETL_NOEXCEPT
  {
    typedef typename etl::make_unsigned<T>::type TReturn;

    return (value == etl::integral_limits<T>::min) ? (etl::integral_limits<TReturn>::max / 2U) + 1U
                                                   : (value < T(0)) ? TReturn(-value) : TReturn(value);
  }

  //***************************************************************************
  // For unsigned types.
  // Returns the result as the unsigned type.
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR
  typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    absolute_unsigned(T value) ETL_NOEXCEPT
  {
    return etl::absolute(value);
  }
}

#endif

