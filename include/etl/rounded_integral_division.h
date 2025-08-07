///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining num copy
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

#ifndef ETL_ROUNDED_INTEGRAL_DIVISION_INCLUDED
#define ETL_ROUNDED_INTEGRAL_DIVISION_INCLUDED

#include "type_traits.h"
#include "absolute.h"

namespace etl
{
  //***************************************************************************
  ///\brief Integral division with rounding to ceiling.
  /// For signed integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T1>::value &&
                            etl::is_integral<T2>::value &&
                            etl::is_signed<T1>::value &&
                            etl::is_signed<T2>::value,
                            typename etl::common_type<T1, T2>::type>::type
    divide_round_to_ceiling(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num = num;
    const type common_den = den;
    const type remainder  = common_num % common_den;

    type quotient = common_num / common_den;
    
    // If remainder is zero, already exact
    if (remainder == 0)
    {
      return quotient;
    }

    // If signs are the same, increment quotient
    return ((common_num ^ common_den) >= 0) ? quotient + 1 : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to ceiling.
  /// For unsigned integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_ceiling(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num = num;
    const type common_den = den;
    const type remainder  = common_num % common_den;

    type quotient = common_num / common_den;
    
    // If remainder is zero, already exact, otherwise, increment quotient
    return remainder == 0U ? quotient : quotient + 1U;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to floor.
  /// For signed integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_floor(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num = num;
    const type common_den = den;
    const type remainder  = common_num % common_den;

    type quotient = common_num / common_den;

    // If remainder is zero, already exact
    if (remainder == 0)
    {
      return quotient;
    }

    // If signs are different, decrement quotient
    return ((common_num ^ common_den) >= 0) ? quotient : quotient - 1;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to floor.
  /// For unsigned integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_floor(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num = num;
    const type common_den = den;

    return common_num / common_den;
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards infinity.
  /// For signed integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value && 
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value, 
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_infinity(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    // Cast to common type to avoid overflow.
    const type common_num = num;
    const type common_den = den;
    const type remainder  = common_num % common_den;

    type quotient = common_num / common_den;

    if ((common_num ^ common_den) >= 0)
    {
      // Same sign, round towards +infinity
      quotient = remainder ? quotient + 1 : quotient;
    }
    else
    {
      // Different signs, round towards -infinity
      quotient = remainder ? quotient - 1 : quotient;
    }

    return quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards infinity.
  /// For unsigned integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value && 
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value, 
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_infinity(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    // Cast to common type to avoid overflow.
    const type common_num = num;
    const type common_den = den;
    const type remainder  = common_num % common_den;

    type quotient = common_num / common_den;

    return remainder ? quotient + 1U : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards zero.
  /// For integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value, 
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_zero(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    // Cast to common type to avoid overflow.
    const type common_num = num;
    const type common_den = den;

    return common_num / common_den;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half up.
  /// For signed integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T1>::value &&
                            etl::is_integral<T2>::value &&
                            etl::is_signed<T1>::value &&
                            etl::is_signed<T2>::value,
                            typename etl::common_type<T1, T2>::type>::type
    divide_round_half_up(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num    = num;
    const type common_den    = den;
    const type quotient      = common_num / common_den;
    const type remainder     = common_num % common_den;
    const type abs_den       = etl::absolute(common_den);
    const type abs_remainder = etl::absolute(remainder);

    // Direction: +1 if result should be more positive, -1 if more negative
    const type direction = ((common_num >= 0) == (common_den >= 0)) ? 1 : -1;

    // If remainder is at least half the divisor, round away from zero
    return abs_remainder >= (abs_den + 1) / 2 ? quotient + direction : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half up.
  /// For unsigned integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value && 
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value, typename etl::common_type<T1, T2>::type>::type
    divide_round_half_up(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;
    
    const type common_num = num;
    const type common_den = den;
    const type remainder  = common_num % common_den;

    type quotient = common_num / common_den;
        
    // If remainder is at least half the divisor, round up
    if (remainder >= (den + 1U) / 2U)
    {
      ++quotient;
    }

    return quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half down.
  /// For signed integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_half_down(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num    = num;
    const type common_den    = den;
    const type quotient      = common_num / common_den;
    const type remainder     = common_num % common_den;
    const type abs_den       = etl::absolute(common_den);
    const type abs_remainder = etl::absolute(remainder);

    // Direction: +1 if result should be more positive, -1 if more negative
    const type direction = ((common_num >= 0) == (common_den >= 0)) ? 1 : -1;

    // Only round away from zero if remainder is strictly greater than half the divisor
    return abs_remainder > (abs_den / 2) ? quotient + direction : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half down.
  /// For unsigned integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value && 
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value, typename etl::common_type<T1, T2>::type>::type
    divide_round_half_down(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num = num;
    const type common_den = den;
    const type remainder  = common_num % common_den;

    type quotient = common_num / common_den;

    // If remainder is at least half the divisor, round down
    if (remainder > (den / 2U))
    {
      ++quotient;
    }

    return quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half even.
  /// For signed integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T1>::value &&
                            etl::is_integral<T2>::value &&
                            etl::is_signed<T1>::value &&
                            etl::is_signed<T2>::value,
                            typename etl::common_type<T1, T2>::type>::type
    divide_round_half_even(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num    = num;
    const type common_den    = den;
    const type quotient      = common_num / common_den;
    const type remainder     = common_num % common_den;
    const type abs_den       = etl::absolute(common_den);
    const type abs_remainder = etl::absolute(remainder);
    const type direction     = ((common_num >= 0) == (common_den >= 0)) ? 1 : -1;

    if ((abs_remainder * 2) < abs_den)
    {
      return quotient;
    }
    else if ((abs_remainder * 2) > abs_den)
    {
      return quotient + direction;
    }
    else
    {
      // Exactly halfway, round to even
      return (quotient & 1) == 0 ? quotient : quotient + direction;
    }
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half even.
  /// For unsigned integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T1>::value &&
                            etl::is_integral<T2>::value &&
                            etl::is_unsigned<T1>::value &&
                            etl::is_unsigned<T2>::value,
                            typename etl::common_type<T1, T2>::type>::type
    divide_round_half_even(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num = num;
    const type common_den = den;
    const type quotient   = common_num / common_den;
    const type remainder  = common_num % common_den;

    if ((remainder * 2U) < den)
    {
      // Less than halfway, round down
      return quotient;
    }
    else if ((remainder * 2U) > den)
    {
      // More than halfway, round up
      return quotient + 1U;
    }
    else
    {
      // Exactly halfway, round to even
      return (quotient & 1U) == 0U ? quotient : quotient + 1;
    }
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half odd.
  /// For signed integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T1>::value &&
                            etl::is_integral<T2>::value &&
                            etl::is_signed<T1>::value &&
                            etl::is_signed<T2>::value,
                            typename etl::common_type<T1, T2>::type>::type
    divide_round_half_odd(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num    = num;
    const type common_den    = den;
    const type quotient      = common_num / common_den;
    const type remainder     = common_num % common_den;
    const type common_den    = etl::absolute(common_den);
    const type abs_remainder = etl::absolute(remainder);
    const type direction     = ((common_num >= 0) == (common_den >= 0)) ? 1 : -1;

    if ((abs_remainder * 2) < common_den)
    {
      return quotient;
    }
    else if ((abs_remainder * 2) > common_den)
    {
      return quotient + direction;
    }
    else
    {
      // Exactly halfway, round to odd
      return (quotient & 1) == 1 ? quotient : quotient + direction;
    }
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half odd.
  /// For unsigned integral types.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T1>::value &&
                            etl::is_integral<T2>::value &&
                            etl::is_unsigned<T1>::value &&
                            etl::is_unsigned<T2>::value,
                            typename etl::common_type<T1, T2>::type>::type
    divide_round_half_odd(T1 num, T2 den) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_num = num;
    const type common_den = den;
    const type quotient   = common_num / common_den;
    const type remainder  = common_num % common_den;

    if ((remainder * 2U) < den)
    {
      // Less than halfway, round down
      return quotient;
    }
    else if ((remainder * 2U) > den)
    {
      // More than halfway, round up
      return quotient + 1U;
    }
    else
    {
      // Exactly halfway, round to odd
      return (quotient & 1U) == 1U ? quotient : quotient + 1U;
    }
  }
}

#endif
