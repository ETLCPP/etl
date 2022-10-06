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

#ifndef ETL_SCALED_ROUNDING_INCLUDED
#define ETL_SCALED_ROUNDING_INCLUDED

#include "static_assert.h"
#include "type_traits.h"
#include "absolute.h"

namespace etl
{
  template <typename T>
  struct scaled_rounding_t
  {
    typedef typename etl::conditional<etl::is_signed<T>::value, int32_t, uint32_t>::type type;
  };

  //*****************************************************************************
  /// A set of rounding algorithms for scaled integrals.
  /// \tparam T       The integral type.
  /// \tparam SCALING The scaling factor.
  ///
  /// \example For emulating fixed point of two decimal places we could use a
  /// scaling factor of '100'. To round the result of scaled int calculations
  /// using 'Banker's Rounding' we would define this.
  /// \code
  /// typedef etl::scaled_rounding<int, 100> Rounding;
  /// int final_result = Rounding::round_half_even_unscaled(accumulated_result);
  /// \endcode
  /// \link http://www.clivemaxfield.com/diycalculator/sp-round.shtml
  //*****************************************************************************

  //***************************************************************************
  /// Round to more positive integer.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_ceiling_unscaled(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Type must be an integral");
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (value >= 0)
    {
      return T((value + scale_t(SCALING)) / scale_t(SCALING));
    }
    else
    {
      return T(value / scale_t(SCALING));
    }
  }

  //***************************************************************************
  /// Round to more positive integer.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_ceiling_scaled(T value)
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return round_ceiling_unscaled<SCALING>(value) * scale_t(SCALING);
  }

  //***************************************************************************
  /// Round to more negative integer.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_floor_unscaled(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Type must be an integral");
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (value >= 0)
    {
      return T(value / scale_t(SCALING));
    }
    else
    {
      return T((value - scale_t(SCALING)) / scale_t(SCALING));
    }
  }

  //***************************************************************************
  /// Round to more negative integer.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_floor_scaled(T value)
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_floor_unscaled<SCALING>(value) * scale_t(SCALING));
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded up (to infinity).
  /// Uses 'symmetric up' rounding.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_half_up_unscaled(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Type must be an integral");
    ETL_STATIC_ASSERT((((SCALING / 2U) * 2U) == SCALING), "Scaling must be divisible by 2");
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (value >= 0)
    {
      return T((value + scale_t(SCALING / 2U)) / scale_t(SCALING));
    }
    else
    {
      return T((value - scale_t(SCALING / 2U)) / scale_t(SCALING));
    }
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded up (to infinity).
  /// Uses 'symmetric up' rounding.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_half_up_scaled(T value)
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_half_up_unscaled<SCALING>(value) * scale_t(SCALING));
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded down (to zero).
  /// Uses 'symmetric down' rounding.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_half_down_unscaled(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Type must be an integral");
    ETL_STATIC_ASSERT((((SCALING / 2U) * 2U) == SCALING), "Scaling must be divisible by 2");
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (value >= 0)
    {
      return T((value + scale_t((SCALING / 2U) - 1U)) / scale_t(SCALING));
    }
    else
    {
      return T((value - scale_t((SCALING / 2U) - 1U)) / scale_t(SCALING));
    }
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded down (to zero).
  /// Uses 'symmetric down' rounding.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_half_down_scaled(T value)
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_half_down_unscaled<SCALING>(value) * scale_t(SCALING));
  }

  //***************************************************************************
  /// Round toward zero.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_zero_unscaled(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Type must be an integral");
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(value / scale_t(SCALING));
  }

  //***************************************************************************
  /// Round toward zero.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_zero_scaled(T value)
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_zero_unscaled<SCALING>(value) * scale_t(SCALING));
  }

  //***************************************************************************
  /// Round toward infinity.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_infinity_unscaled(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Type must be an integral");
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (value >= 0)
    {
      return T((value + scale_t(SCALING)) / scale_t(SCALING));
    }
    else
    {
      return T((value - scale_t(SCALING)) / scale_t(SCALING));
    }
  }

  //***************************************************************************
  /// Round toward infinity.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_infinity_scaled(T value)
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_infinity_unscaled<SCALING>(value) * scale_t(SCALING));
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded to even integral.
  /// Also known as 'Banker's Rounding'.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_half_even_unscaled(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Type must be an integral");
    typedef typename scaled_rounding_t<T>::type scale_t;

    // Half?
    if ((etl::absolute(value) % scale_t(SCALING)) == scale_t(SCALING / 2U))
    {
      // Odd?
      if ((value / scale_t(SCALING)) & 1U)
      {
        return T(round_half_up_unscaled<SCALING>(value));
      }
      else
      {
        return T(round_half_down_unscaled<SCALING>(value));
      }
    }
    else
    {
      return T(round_half_up_unscaled<SCALING>(value));
    }
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded to even integral.
  /// Also known as 'Banker's Rounding'.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_half_even_scaled(T value)
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_half_even_unscaled<SCALING>(value) * scale_t(SCALING));
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded to odd integral.
  /// Also known as 'Banker's Rounding'.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_half_odd_unscaled(T value)
  {
    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Type must be an integral");
    typedef typename scaled_rounding_t<T>::type scale_t;

    // Half?
    if ((etl::absolute(value) % scale_t(SCALING)) == scale_t(SCALING / 2U))
    {
      // Odd?
      if ((value / scale_t(SCALING)) & 1U)
      {
        return T(round_half_down_unscaled<SCALING>(value));
      }
      else
      {
        return T(round_half_up_unscaled<SCALING>(value));
      }
    }
    else
    {
      return T(round_half_up_unscaled<SCALING>(value));
    }
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded to odd integral.
  /// Also known as 'Banker's Rounding'.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t SCALING, typename T>
  T round_half_odd_scaled(T value)
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_half_odd_unscaled<SCALING>(value) * scale_t(SCALING));
  }
}

#endif
