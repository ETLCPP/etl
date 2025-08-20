///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining numerator copy
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
#include "utility.h"

namespace etl
{
  namespace private_rounded_integral_division
  {
    //*****************************************************************************
    // Checks if two values have the same sign.
    // For signed integral types.
    //*****************************************************************************
    template <typename T>
    ETL_CONSTEXPR
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, bool>::type
      are_same_sign(T a, T b) ETL_NOEXCEPT
    {
      return ((a ^ b) >= 0);
    }

    //*****************************************************************************
    // Checks if two values have the same sign.
    // For unsigned integral types.
    //*****************************************************************************
    template <typename T>
    ETL_CONSTEXPR
    typename etl::enable_if<etl::is_integral<T>::value && etl::is_unsigned<T>::value, bool>::type
      are_same_sign(T /*a*/, T /*b*/) ETL_NOEXCEPT
    {
      return true;
    }
  }

  //***************************************************************************
  ///\brief Integral division with rounding to ceiling.
  /// For signed integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to ceiling.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value && etl::is_signed<T>::value, T>::type
    divide_round_to_ceiling(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T remainder = numerator % denominator;
    const T quotient  = numerator / denominator;

    // If remainder is zero, already exact
    if (remainder == 0)
    {
      return quotient;
    }

    // If signs are the same, increment quotient
    return private_rounded_integral_division::are_same_sign(numerator, denominator) ? quotient + 1 : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to ceiling.
  /// For signed integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to ceiling. 
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_ceiling(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_to_ceiling(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to ceiling.
  /// For unsigned integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to ceiling.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value,
                          T>::type
    divide_round_to_ceiling(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T remainder = numerator % denominator;
    const T quotient  = numerator / denominator;
    
    // If remainder is zero, already exact, otherwise, increment quotient
    return remainder == 0U ? quotient : quotient + 1U;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to ceiling.
  /// For unsigned integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to ceiling.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_ceiling(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_to_ceiling(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to floor.
  /// For signed integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to floor.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value,
                          T>::type
    divide_round_to_floor(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T remainder = numerator % denominator;
    const T quotient  = numerator / denominator;

    // If remainder is zero, already exact
    if (remainder == 0)
    {
      return quotient;
    }

    // If signs are different, decrement quotient
    return private_rounded_integral_division::are_same_sign(numerator, denominator) ? quotient : quotient - 1;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to floor.
  /// For signed integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to floor.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
    typename etl::enable_if<etl::is_integral<T1>::value &&
                            etl::is_integral<T2>::value &&
                            etl::is_signed<T1>::value &&
                            etl::is_signed<T2>::value,
                            typename etl::common_type<T1, T2>::type>::type
    divide_round_to_floor(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_to_floor(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to floor.
  /// For unsigned integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to floor.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value,
                          T>::type
    divide_round_to_floor(T numerator, T denominator) ETL_NOEXCEPT
  {
    return numerator / denominator;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to floor.
  /// For unsigned integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to floor.
  /// Return type is the etl::common_type<T1, T2>::type.
  /// //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_floor(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    const type common_numerator = numerator;
    const type common_denominator = denominator;

    return common_numerator / common_denominator;
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards infinity.
  /// For signed integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to infinity.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value, 
                          T>::type
    divide_round_to_infinity(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T remainder = numerator % denominator;
    const T quotient  = numerator / denominator;
    
    if (private_rounded_integral_division::are_same_sign(numerator, denominator))
    {
      // Same sign, round towards +infinity
      return (remainder != 0) ? quotient + 1 : quotient;
    }
    else
    {
      // Different signs, round towards -infinity
      return (remainder != 0) ? quotient - 1 : quotient;
    }
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards infinity.
  /// For signed integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to infinity.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value && 
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value, 
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_infinity(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_to_infinity(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards infinity.
  /// For unsigned integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to infinity.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value && 
                          etl::is_unsigned<T>::value, 
                          T>::type
    divide_round_to_infinity(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T remainder = numerator % denominator;
    const T quotient  = numerator / denominator;

    return remainder ? quotient + 1U : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards infinity.
  /// For unsigned integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to infinity.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value && 
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value, 
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_infinity(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_to_infinity(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards zero.
  /// For integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to zero.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value && 
                          etl::is_unsigned<T>::value, 
                          T>::type
    divide_round_to_zero(T numerator, T denominator) ETL_NOEXCEPT
  {
    return numerator / denominator;
  }

  //***************************************************************************
  ///\brief Integral division with rounding towards zero.
  /// For integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to zero.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value, 
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_to_zero(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    // Cast to common type to avoid overflow.
    const type common_numerator   = numerator;
    const type common_denominator = denominator;

    return common_numerator / common_denominator;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half up.
  /// For signed integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half up.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value,
                          T>::type
    divide_round_half_up(T numerator, T denominator) ETL_NOEXCEPT
  {
    // Normal division
    const T remainder = numerator % denominator;
    const T quotient  = numerator / denominator;

    // Work with magnitudes in unsigned form (avoids abs() overflow)
    typedef typename std::make_unsigned<T>::type utype;
    utype abs_remainder   = remainder < 0 ? utype(0) - utype(remainder) : utype(remainder);
    utype abs_denominator = denominator < 0 ? utype(0) - utype(denominator)       : utype(denominator);

    // Threshold for rounding up (half the denominatorominator, rounded up)
    utype half_denominator = (abs_denominator + 1) / 2;

    if (abs_remainder >= half_denominator) 
    {
      // Round away from zero
      if (private_rounded_integral_division::are_same_sign(numerator, denominator)) 
      {
        return quotient + 1; // same sign ? increment
      }
      else 
      {
        return quotient - 1; // different sign ? decrement
      }
    }

    return quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half up.
  /// For signed integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half up.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_half_up(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_half_up(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half up.
  /// For unsigned integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half up.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value && 
                          etl::is_unsigned<T>::value, 
                          T>::type
    divide_round_half_up(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T remainder = numerator % denominator;
    const T quotient = numerator / denominator;

    // If remainder is at least half the divisor, round up
    return (remainder >= (denominator / 2U) + (denominator % 2U)) ? quotient + 1U : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half up.
  /// For unsigned integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half up.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value && 
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value, typename etl::common_type<T1, T2>::type>::type
    divide_round_half_up(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_half_up(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half down.
  /// For signed integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half down.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value,
                          T>::type
    divide_round_half_down(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T quotient            = numerator / denominator;
    const T remainder           = numerator % denominator;
    const T abs_denominator     = etl::absolute(denominator);
    const T abs_remainderainder = etl::absolute(remainder);

    // Direction: +1 if result should be more positive, -1 if more negative
    const T direction = ((numerator >= 0) == (denominator >= 0)) ? 1 : -1;

    // Only round away from zero if remainder is strictly greater than half the divisor
    return abs_remainderainder > (abs_denominator / 2) ? quotient + direction : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half down.
  /// For signed integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half down.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_half_down(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_half_down(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half down.
  /// For unsigned integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half down.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value, 
                          T>::type
    divide_round_half_down(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T remainder = numerator % denominator;
    const T quotient  = numerator / denominator;

    // If remainder is at least half the divisor, round down
    return (remainder > (denominator / 2U)) ? quotient + 1U : quotient;
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half down.
  /// For unsigned integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half down.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value && 
                          etl::is_integral<T2>::value && 
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value, typename etl::common_type<T1, T2>::type>::type
    divide_round_half_down(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_half_down(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half even.
  /// For signed integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half even.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value, 
                          T>::type
    divide_round_half_even(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T quotient            = numerator / denominator;
    const T remainder           = numerator % denominator;
    const T abs_denominator     = etl::absolute(denominator);
    const T abs_remainderainder = etl::absolute(remainder);
    const T direction           = ((numerator >= 0) == (denominator >= 0)) ? 1 : -1;

    if ((abs_remainderainder * 2) < abs_denominator)
    {
      return quotient;
    }
    else if ((abs_remainderainder * 2) > abs_denominator)
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
  /// For signed integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half even.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_half_even(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_half_even(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half even.
  /// For unsigned integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half even.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value,
                          T>::type
    divide_round_half_even(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T quotient  = numerator / denominator;
    const T remainder = numerator % denominator;

    if ((remainder * 2U) < denominator)
    {
      // Less than halfway, round down
      return quotient;
    }
    else if ((remainder * 2U) > denominator)
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
  ///\brief Integral division with rounding to half even.
  /// For unsigned integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half even.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_half_even(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_half_even(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half odd.
  /// For signed integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half odd.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_signed<T>::value,
                          T>::type
    divide_round_half_odd(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T quotient            = numerator / denominator;
    const T remainder           = numerator % denominator;
    const T abs_denominator     = etl::absolute(denominator);
    const T abs_remainderainder = etl::absolute(remainder);
    const T direction           = ((numerator >= 0) == (denominator >= 0)) ? 1 : -1;

    if ((abs_remainderainder * 2) < abs_denominator)
    {
      return quotient;
    }
    else if ((abs_remainderainder * 2) > abs_denominator)
    {
      return quotient + direction;
    }
    else
    {
      // Exactly halfway, round to odd
      return (quotient & 1) != 0 ? quotient : quotient + direction;
    }
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half odd.
  /// For signed integral types.
  /// For different parameter types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half odd.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_signed<T1>::value &&
                          etl::is_signed<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_half_odd(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_half_odd(static_cast<type>(numerator), static_cast<type>(denominator));
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half odd.
  /// For unsigned integral types.
  /// For identical argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half odd.
  //***************************************************************************
  template <typename T>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T>::value &&
                          etl::is_unsigned<T>::value,
                          T>::type
    divide_round_half_odd(T numerator, T denominator) ETL_NOEXCEPT
  {
    const T quotient  = numerator / denominator;
    const T remainder = numerator % denominator;
    const T direction = ((numerator >= 0U) == (denominator >= 0U)) ? 1 : -1;

    if ((remainder * 2U) < denominator)
    {
      return quotient;
    }
    else if ((remainder * 2U) > denominator)
    {
      return quotient + direction;
    }
    else
    {
      // Exactly halfway, round to odd
      return (quotient & 1U) != 0U ? quotient : quotient + direction;
    }
  }

  //***************************************************************************
  ///\brief Integral division with rounding to half odd.
  /// For unsigned integral types.
  /// For different argument types.
  ///\param numerator The numerator.
  ///\param denominator The denominator.
  ///\return The result of the division rounded to half odd.
  /// Return type is the etl::common_type<T1, T2>::type.
  //***************************************************************************
  template <typename T1, typename T2>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_integral<T1>::value &&
                          etl::is_integral<T2>::value &&
                          etl::is_unsigned<T1>::value &&
                          etl::is_unsigned<T2>::value,
                          typename etl::common_type<T1, T2>::type>::type
    divide_round_half_odd(T1 numerator, T2 denominator) ETL_NOEXCEPT
  {
    typedef typename etl::common_type<T1, T2>::type type;

    return divide_round_half_odd(static_cast<type>(numerator), static_cast<type>(denominator));
  }
}

#endif
