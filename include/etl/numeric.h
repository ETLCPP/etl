///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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
#include "iterator.h"
#include "limits.h"
#include "type_traits.h"

#if ETL_USING_STL
  #include <iterator>
#endif

///\defgroup numeric numeric
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  /// iota
  /// Reverse engineered version of std::iota for non C++11 compilers.
  /// Fills a range of elements with sequentially increasing values starting
  /// with <b>value</b>.
  ///\param first An iterator to the first position to fill.
  ///\param last  An iterator to the last + 1 position.
  ///\ingroup numeric
  //***************************************************************************
  template <typename TIterator, typename T>
  ETL_CONSTEXPR14 void iota(TIterator first, TIterator last, T value)
  {
    while (first != last)
    {
      *first++ = value++;
    }
  }

  //***************************************************************************
  /// midpoint
  /// For floating point.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if< !etl::is_pointer<T>::value && !etl::is_integral<T>::value && etl::is_floating_point<T>::value, T>::type
    midpoint(T a, T b) ETL_NOEXCEPT
  {
    T lo = etl::numeric_limits<T>::min() * T(2);
    T hi = etl::numeric_limits<T>::max() * T(2);

    return ((abs(a) <= hi) && (abs(b) <= hi)) ? (a + b) / T(2)
           : (abs(a) < lo)                    ? a + (b / T(2))
           : (abs(b) < lo)                    ? ((a / T(2)) + b)
                                              : (a / T(2)) + (b / T(2));
  }

  //***************************************************************************
  /// midpoint
  /// For unsigned integrals.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<
    !etl::is_pointer<T>::value && etl::is_integral<T>::value && !etl::is_floating_point<T>::value && etl::is_unsigned<T>::value, T>::type
    midpoint(T a, T b) ETL_NOEXCEPT
  {
    if (a > b)
    {
      return a - ((a - b) >> 1);
    }
    else
    {
      return a + ((b - a) >> 1);
    }
  }

  //***************************************************************************
  /// midpoint
  /// For signed integrals.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<
    !etl::is_pointer<T>::value && etl::is_integral<T>::value && !etl::is_floating_point<T>::value && etl::is_signed<T>::value, T>::type
    midpoint(T a, T b) ETL_NOEXCEPT
  {
    typedef typename etl::make_unsigned<T>::type utype;

    if (a > b)
    {
      return a - T(utype(utype(a) - utype(b)) >> 1);
    }
    else
    {
      return a + T((utype(b) - utype(a)) >> 1);
    }
  }

  //***************************************************************************
  /// midpoint
  /// For pointers.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if< etl::is_pointer<T>::value && !etl::is_integral<T>::value && !etl::is_floating_point<T>::value, T>::type
    midpoint(T a, T b) ETL_NOEXCEPT
  {
    if (a > b)
    {
      return b + (etl::distance(b, a) / 2);
    }
    else
    {
      return a + (etl::distance(a, b) / 2);
    }
  }

  //***************************************************************************
  /// midpoint
  /// For ETL random access iterators.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T midpoint(
    T a, T b,
    typename etl::enable_if< !etl::is_pointer<T>::value && !etl::is_integral<T>::value && !etl::is_floating_point<T>::value
                               && etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::random_access_iterator_tag>::value,
                             int>::type = 0)
  {
    if (a > b)
    {
      return b + (etl::distance(b, a) / 2);
    }
    else
    {
      return a + (etl::distance(a, b) / 2);
    }
  }

  //***************************************************************************
  /// midpoint
  /// For ETL forward and bidirectional iterators.
  /// Parameter 'a' must be before 'b', otherwise the result is undefined.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 T midpoint(T a, T b,
                       typename etl::enable_if<
                         (!etl::is_pointer<T>::value && !etl::is_integral<T>::value && !etl::is_floating_point<T>::value
                          && (etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::forward_iterator_tag>::value
                              || etl::is_same<typename etl::iterator_traits<T>::iterator_category, ETL_OR_STD::bidirectional_iterator_tag>::value)),
                         int>::type = 0)
  {
    etl::advance(a, etl::distance(a, b) / 2);
    return a;
  }

  //***************************************************************************
  /// Linear interpolation
  /// For floating point.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR typename etl::enable_if<etl::is_floating_point<T>::value, T>::type lerp(T a, T b, T t) ETL_NOEXCEPT
  {
    return a + (t * (b - a));
  }

  //***************************************************************************
  /// Linear interpolation
  /// For when any parameter is not floating point.
  //***************************************************************************
  template <typename TArithmetic1, typename TArithmetic2, typename TArithmetic3>
  ETL_CONSTEXPR typename etl::enable_if<
    !etl::is_floating_point<TArithmetic1>::value || !etl::is_floating_point<TArithmetic2>::value || !etl::is_floating_point<TArithmetic3>::value,
    typename etl::conditional< etl::is_same<TArithmetic1, long double>::value || etl::is_same<TArithmetic2, long double>::value
                                 || etl::is_same<TArithmetic3, long double>::value,
                               long double, double>::type>::type
    lerp(TArithmetic1 a, TArithmetic2 b, TArithmetic3 t) ETL_NOEXCEPT
  {
    typedef typename etl::conditional<etl::is_integral<TArithmetic1>::value, double, TArithmetic1>::type typecast_a;
    typedef typename etl::conditional<etl::is_integral<TArithmetic2>::value, double, TArithmetic2>::type typecast_b;
    typedef typename etl::conditional<etl::is_integral<TArithmetic3>::value, double, TArithmetic3>::type typecast_t;

    return typecast_a(a) + (typecast_t(t) * (typecast_b(b) - typecast_a(a)));
  }

  //***************************************************************************
  /// Saturating addition for unsigned integers.
  /// Returns x + y, clamped to the range of T.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value, T>::type add_sat(T x, T y) ETL_NOEXCEPT
  {
    T result = static_cast<T>(x + y);
    // Overflow occurred if result < x
    if (result < x)
    {
      return etl::numeric_limits<T>::max();
    }
    return result;
  }

  //***************************************************************************
  /// Saturating addition for signed integers.
  /// Returns x + y, clamped to the range of T.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type add_sat(T x, T y) ETL_NOEXCEPT
  {
    // Check for overflow: both operands have same sign and result has different sign
    if (y > T(0))
    {
      // Positive overflow check
      if (x > (etl::numeric_limits<T>::max() - y))
      {
        return etl::numeric_limits<T>::max();
      }
    }
    else
    {
      // Negative overflow check
      if (x < (etl::numeric_limits<T>::min() - y))
      {
        return etl::numeric_limits<T>::min();
      }
    }

    return static_cast<T>(x + y);
  }

  //***************************************************************************
  /// Saturating subtraction for unsigned integers.
  /// Returns x - y, clamped to the range of T.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value, T>::type sub_sat(T x, T y) ETL_NOEXCEPT
  {
    // Underflow occurred if y > x
    if (y > x)
    {
      return T(0);
    }
    return static_cast<T>(x - y);
  }

  //***************************************************************************
  /// Saturating subtraction for signed integers.
  /// Returns x - y, clamped to the range of T.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type sub_sat(T x, T y) ETL_NOEXCEPT
  {
    // Check for overflow/underflow
    if (y > T(0))
    {
      // Subtracting positive: check for underflow
      if (x < (etl::numeric_limits<T>::min() + y))
      {
        return etl::numeric_limits<T>::min();
      }
    }
    else
    {
      // Subtracting negative (adding): check for overflow
      if (x > (etl::numeric_limits<T>::max() + y))
      {
        return etl::numeric_limits<T>::max();
      }
    }

    return static_cast<T>(x - y);
  }

  //***************************************************************************
  /// Saturating multiplication for unsigned integers.
  /// Returns x * y, clamped to the range of T.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value, T>::type mul_sat(T x, T y) ETL_NOEXCEPT
  {
    if ((x == T(0)) || (y == T(0)))
    {
      return T(0);
    }

    // Check for overflow: x * y > max  =>  x > max / y
    if (x > (etl::numeric_limits<T>::max() / y))
    {
      return etl::numeric_limits<T>::max();
    }

    return static_cast<T>(x * y);
  }

  //***************************************************************************
  /// Saturating multiplication for signed integers.
  /// Returns x * y, clamped to the range of T.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type mul_sat(T x, T y) ETL_NOEXCEPT
  {
    if ((x == T(0)) || (y == T(0)))
    {
      return T(0);
    }

    // Both positive
    if ((x > T(0)) && (y > T(0)))
    {
      if (x > (etl::numeric_limits<T>::max() / y))
      {
        return etl::numeric_limits<T>::max();
      }
    }
    // Both negative
    else if ((x < T(0)) && (y < T(0)))
    {
      if (x < (etl::numeric_limits<T>::max() / y))
      {
        return etl::numeric_limits<T>::max();
      }
    }
    // Different signs (x positive, y negative)
    else if ((x > T(0)) && (y < T(0)))
    {
      if (y < (etl::numeric_limits<T>::min() / x))
      {
        return etl::numeric_limits<T>::min();
      }
    }
    // Different signs (x negative, y positive)
    else
    {
      if (x < (etl::numeric_limits<T>::min() / y))
      {
        return etl::numeric_limits<T>::min();
      }
    }

    return static_cast<T>(x * y);
  }

  //***************************************************************************
  /// Saturating division for unsigned integers.
  /// Returns x / y. For unsigned types, no saturation is needed.
  /// \pre y != 0 (undefined behaviour if y is zero, matching C++26).
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value, T>::type div_sat(T x, T y) ETL_NOEXCEPT
  {
    return static_cast<T>(x / y);
  }

  //***************************************************************************
  /// Saturating division for signed integers.
  /// Returns x / y, clamped to the range of T.
  /// The only case of overflow is min / -1.
  /// \pre y != 0 (undefined behaviour if y is zero, matching C++26).
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14 typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type div_sat(T x, T y) ETL_NOEXCEPT
  {
    // The only overflow case: min / -1 would be max + 1
    if ((x == etl::numeric_limits<T>::min()) && (y == T(-1)))
    {
      return etl::numeric_limits<T>::max();
    }

    return static_cast<T>(x / y);
  }

  //***************************************************************************
  /// saturate_cast
  /// Converts an integer value to another integer type, clamping the value
  /// to the representable range of the destination type.
  /// C++26 equivalent of std::saturate_cast.
  ///
  /// When the source value is within the range of R, returns the value as R.
  /// When the source value is below R's minimum, returns numeric_limits<R>::min().
  /// When the source value is above R's maximum, returns numeric_limits<R>::max().
  //***************************************************************************

  // Case 1: Both unsigned.
  template <typename R, typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<R>::value && etl::is_integral<T>::value && etl::is_unsigned<R>::value && etl::is_unsigned<T>::value,
                            R>::type
    saturate_cast(T value) ETL_NOEXCEPT
  {
    // If sizeof(R) >= sizeof(T), all values of T fit in R.
    // If sizeof(R) < sizeof(T), clamp to R's max. The comparison is safe
    // because R's max fits in T when T is wider.
    if ((sizeof(R) < sizeof(T)) && (value > static_cast<T>(etl::numeric_limits<R>::max())))
    {
      return etl::numeric_limits<R>::max();
    }
    return static_cast<R>(value);
  }

  // Case 2: Both signed.
  template <typename R, typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<R>::value && etl::is_integral<T>::value && etl::is_signed<R>::value && etl::is_signed<T>::value, R>::type
    saturate_cast(T value) ETL_NOEXCEPT
  {
    // Only need to clamp when narrowing (R is smaller than T).
    // When sizeof(R) >= sizeof(T), all values of T fit in R.
    if (sizeof(R) < sizeof(T))
    {
      if (value > static_cast<T>(etl::numeric_limits<R>::max()))
      {
        return etl::numeric_limits<R>::max();
      }
      if (value < static_cast<T>(etl::numeric_limits<R>::min()))
      {
        return etl::numeric_limits<R>::min();
      }
    }
    return static_cast<R>(value);
  }

  // Case 3: Signed source -> Unsigned destination.
  template <typename R, typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<R>::value && etl::is_integral<T>::value && etl::is_unsigned<R>::value && etl::is_signed<T>::value,
                            R>::type
    saturate_cast(T value) ETL_NOEXCEPT
  {
    if (value < T(0))
    {
      return R(0);
    }

    typedef typename etl::make_unsigned<T>::type unsigned_t;
    unsigned_t                                   uvalue = static_cast<unsigned_t>(value);

    // Compare in unsigned domain. R's max is always representable as unsigned_t
    // when sizeof(T) > sizeof(R), and when sizeof(R) >= sizeof(T) all positive
    // values of T fit in R.
    if ((sizeof(R) < sizeof(T)) && (uvalue > static_cast<unsigned_t>(etl::numeric_limits<R>::max())))
    {
      return etl::numeric_limits<R>::max();
    }
    return static_cast<R>(value);
  }

  // Case 4: Unsigned source -> Signed destination.
  template <typename R, typename T>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<R>::value && etl::is_integral<T>::value && etl::is_signed<R>::value && etl::is_unsigned<T>::value,
                            R>::type
    saturate_cast(T value) ETL_NOEXCEPT
  {
    // R's max is positive, so we can safely represent it as T (unsigned) when
    // sizeof(T) >= sizeof(R). When sizeof(T) < sizeof(R), all values of T fit.
    typedef typename etl::make_unsigned<R>::type unsigned_r;

    if (value > static_cast<T>(static_cast<unsigned_r>(etl::numeric_limits<R>::max())))
    {
      return etl::numeric_limits<R>::max();
    }
    return static_cast<R>(value);
  }
} // namespace etl

#endif
