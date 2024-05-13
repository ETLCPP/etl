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

#ifndef ETL_GDC_INCLUDED
#define ETL_GDC_INCLUDED

#include "type_traits.h"
#include "absolute.h"
#include "static_assert.h"

namespace etl
{
  //***************************************************************************
  // Greatest Common Divisor.
  // For unsigned types.
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_unsigned<T>::value, T>::type
    gcd(T a, T b) ETL_NOEXCEPT
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Integral type required");

    if ((a == 0 || b == 0))
    {
      return (a + b);
    }

    while (b != 0) 
    {
      T t = b;
      b = a % b;
      a = t;
    }
    
    return a;
  }

  //***************************************************************************
  // Greatest Common Divisor.
  // For signed types.
  //***************************************************************************
  template <typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_signed<T>::value, T>::type
    gcd(T a, T b) ETL_NOEXCEPT
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Integral type required");

    typedef typename etl::make_unsigned<T>::type utype;

    utype ua = etl::absolute_unsigned(a);
    utype ub = etl::absolute_unsigned(b);

    return static_cast<T>(gcd(ua, ub));
  }

#if ETL_USING_CPP11
  #if ETL_HAS_INITIALIZER_LIST
  //***************************************************************************
  // Greatest Common Divisor.
  // Non-recursive, using an initializer_list.
  // Top level variadic function.
  //***************************************************************************
  template<typename T, typename... TRest>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T gcd(T first, TRest... rest) ETL_NOEXCEPT
  {
    T result = first;

    for (T value : {rest...})
    {
      result = gcd(result, value);

      if (result == 1)
      {
        // Early termination: if the GCD is one, it will remain one
        // no matter what other numbers are processed.
        return 1;
      }
    }

    return result;
  }
  #else
  //***************************************************************************
  // Greatest Common Divisor.
  // Recursive.
  // Top level variadic function.
  //***************************************************************************
  template<typename T, typename... TRest>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T gcd(T a, T b, TRest... rest) ETL_NOEXCEPT
  {
    T gcd_ab = gcd(a, b);

    if (gcd_ab == 1)
    {
      // Early termination: if the GCD is one, it will remain one
      // no matter what other numbers are processed.
      return 1;
    }
    else
    {
      return gcd(gcd_ab, rest...);
    }
  }
  #endif
#endif
}

#endif

