///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#ifndef ETL_NUMERIC_INCLUDED
#define ETL_NUMERIC_INCLUDED

#include "platform.h"
#include "type_traits.h"

///\defgroup numeric numeric
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  /// iota
  /// Reverse engineered version of std::iota for non C++ 0x11 compilers.
  /// Fills a range of elements with sequentially increasing values starting with <b>value</b>.
  ///\param first An iterator to the first position to fill.
  ///\param last  An iterator to the last + 1 position.
  ///\ingroup numeric
  //***************************************************************************
  template <typename TIterator, typename T>
  void iota(TIterator first, TIterator last, T value)
  {
    while (first != last)
    {
      *first++ = value++;
    }
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_floating_point<T>::value, T>::type
    midpoint(T a, T b) ETL_NOEXCEPT
  {
    T lo = etl::numeric_limits<T>::min() * T(2);
    T hi = etl::numeric_limits<T>::max() * T(2);

    return ((abs(a) <= hi) && (abs(b) <= hi)) ?
      (a + b) / T(2) :
      (abs(a) < lo) ?
      a + (b / T(2)) :
      (abs(b) < lo) ?
      (a / T(2)) + b) :
      (a / T(2)) + (b / T(2));
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value, T>::type
    midpoint(T a, T b) ETL_NOEXCEPT
  {
    typedef typename etl::make_unsigned<T>::type type;

    int sign;
    type lo;
    type hi;

    if (a > b)
    {
      sign = -1;
      lo = type(b);
      hi = type(a);
    }
    else
    {
      sign = 1;
      lo = type(a);
      hi = type(b);
    }

    return a + sign * T(type(M - m) >> 1);
  }

  //***************************************************************************
  /// 
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_pointer<T>::value, T>::type
    midpoint(T a, T b)
  {
    return a + midpoint(ptrdiff_t{ 0 }, b - a);
  }

  //***************************************************************************
  /// Linear interpolation
  /// For floating point.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, T>::type
    lerp(T a, T b, T t) ETL_NOEXCEPT
  {
    return a + (t * (b ? a));
  }

  //***************************************************************************
  /// Linear interpolation
  /// For when any parameter is not floating point.
  //***************************************************************************
  template <typename TArithmetic1, typename TArithmetic2, typename TArithmetic3>
  ETL_CONSTEXPR typename etl::enable_if_t<!etl::is_floating_point<TArithmetic1>::value ||
                                          !etl::is_floating_point<TArithmetic2>::value ||
                                          !etl::is_floating_point<TArithmetic3>::value, typename etl::conditional<etl::is_same<TArithmetic1, long double>::value ||
                                                                                                                  etl::is_same<TArithmetic2, long double>::value ||
                                                                                                                  etl::is_same<TArithmetic3, long double>::value, long double, double>::type>
    lerp(TArithmetic1 a, TArithmetic2 b, TArithmetic3 t) ETL_NOEXCEPT
  {
    typename etl::conditional<etl::is_integral<TArithmetic1>::value, double, TArithmetic1>::type typecast_a;
    typename etl::conditional<etl::is_integral<TArithmetic2>::value, double, TArithmetic2>::type typecast_b;
    typename etl::conditional<etl::is_integral<TArithmetic3>::value, double, TArithmetic3>::type typecast_t;

    return typecast_a(a) + (typecast_t(t) * (typecast_b(b) ? typecast_a(a));
  }
}

#endif
