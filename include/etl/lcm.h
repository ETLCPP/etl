///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2024 John Wellbelove

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

#ifndef ETL_LCM_INCLUDED
#define ETL_LCM_INCLUDED

#include "type_traits.h"
#include "absolute.h"
#include "static_assert.h"
#include "gcd.h"

namespace etl
{
  //***************************************************************************
  // Least Common Multiple.
  // For unsigned types.
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    lcm(T a, T b) ETL_NOEXCEPT
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Integral type required");

    // Early termination: if either number is zero, the LCM is zero.
    if (a == 0 || b == 0)
    {
      return 0;
    }
    else
    {
      return a * (b / gcd(a, b));
    }
  }

  //***************************************************************************
  // Least Common Multiple.
  // For signed types.
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_signed<T>::value, T>::type
    lcm(T a, T b) ETL_NOEXCEPT
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Integral type required");

    typedef typename etl::make_unsigned<T>::type utype;

    utype ua = etl::absolute_unsigned(a);
    utype ub = etl::absolute_unsigned(b);

    return static_cast<T>(lcm(ua, ub));
  }

#if ETL_USING_CPP11
  #if ETL_HAS_INITIALIZER_LIST
  //***************************************************************************
  // Least Common Multiple.
  // Non-recursive, using an initializer_list.
  // Top level variadic function.
  //***************************************************************************
  template<typename T, typename... TRest>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T lcm(T first, TRest... rest) ETL_NOEXCEPT
  {
    T result = first;

    for (T value : {rest...})
    {
      result = lcm(result, value);

      if (result == 0)
      {
        // Early termination: if the LCM is zero, it will remain zero
        // no matter what other numbers are processed.
        return 0;
      }
    }

    return result;
  }
  #else
  //***************************************************************************
  // Least Common Multiple.
  // Recursive
  // Top level variadic function.
  //***************************************************************************
  template<typename T, typename... TRest>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T lcm(T a, T b, TRest... rest) ETL_NOEXCEPT
  {
    T lcm_ab = lcm(a, b);

    if (lcm_ab == 0)
    {
      // Early termination: if the LCM is zero, it will remain zero
      // no matter what other numbers are processed.
      return 0;
    }
    else
    {
      return lcm(lcm_ab, rest...);
    }
  }
  #endif
#endif
}

#endif

