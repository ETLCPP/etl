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

#include "type_traits.h"
#include "integral_limits.h"

namespace etl
{
  //***************************************************************************
  // For signed types.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_signed<T>::value, T>::type
    absolute(T value)
  {
    return (value < T(0)) ? -value : value;
  }

  //***************************************************************************
  // For unsigned types.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    absolute(T value)
  {
    return value;
  }

  //***************************************************************************
  // For signed types.
  // Returns the result as the unsigned type.
  //***************************************************************************
  template <typename T, typename TReturn = typename etl::make_unsigned<T>::type>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_signed<T>::value, TReturn>::type
    absolute_unsigned(T value)
  {
    return (value == etl::integral_limits<T>::min) ? etl::integral_limits<TReturn>::max / 2U
      : (value < T(0)) ? TReturn(-value) : TReturn(value);
  }

  //***************************************************************************
  // For unsigned types.
  // Returns the result as the unsigned type.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    absolute_unsigned(T value)
  {
    return etl::absolute(value);
  }
}

#endif

