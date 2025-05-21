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
  /// \tparam Scaling The scaling factor.
  ///
  /// \example For emulating fixed point of two decimal places we could use a
  /// scaling factor of '100'. To round the result of scaled int calculations
  /// using 'Banker's Rounding' we would define this.
  /// \code
  /// int final_result = round_half_even_unscaled<100>(accumulated_result);
  /// \endcode
  /// \link http://www.clivemaxfield.com/diycalculator/sp-round.shtml
  //*****************************************************************************

  //***************************************************************************
  /// Round to more positive integer.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_ceiling_unscaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (Scaling == 1)
    {
      return value;
    }

    if (value >= 0)
    {
      return T((value + scale_t(Scaling - 1U)) / scale_t(Scaling));
    }
    else
    {
      return T(value / scale_t(Scaling));
    }
  }

  //***************************************************************************
  /// Round to more positive integer.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_ceiling_scaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return round_ceiling_unscaled<Scaling>(value) * scale_t(Scaling);
  }

  //***************************************************************************
  /// Round to more negative integer.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_floor_unscaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;
    if (Scaling == 1)
    {
      return value;
    }

    if (value >= 0)
    {
      return T(value / scale_t(Scaling));
    }
    else
    {
      return T((value - scale_t(Scaling - 1)) / scale_t(Scaling));
    }
  }

  //***************************************************************************
  /// Round to more negative integer.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_floor_scaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_floor_unscaled<Scaling>(value) * scale_t(Scaling));
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded up (to infinity).
  /// Uses 'symmetric up' rounding.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_half_up_unscaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (Scaling == 1)
    {
      return value;
    }
    else
    {
      if (value >= 0)
      {
        return T((value + scale_t(Scaling / 2U)) / scale_t(Scaling));
      }
      else
      {
        return T((value - scale_t(Scaling / 2U)) / scale_t(Scaling));
      }
    }
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded up (to infinity).
  /// Uses 'symmetric up' rounding.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_half_up_scaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_half_up_unscaled<Scaling>(value) * scale_t(Scaling));
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded down (to zero).
  /// Uses 'symmetric down' rounding.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_half_down_unscaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (Scaling == 1)
    {
      return value;
    }

    if (value >= 0)
    {
      return T((value + scale_t((Scaling - 1) / 2U)) / scale_t(Scaling));
    }
    else
    {
      return T((value - scale_t((Scaling - 1) / 2U)) / scale_t(Scaling));
    }
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded down (to zero).
  /// Uses 'symmetric down' rounding.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_half_down_scaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_half_down_unscaled<Scaling>(value) * scale_t(Scaling));
  }

  //***************************************************************************
  /// Round toward zero.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_zero_unscaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (Scaling == 1)
    {
      return value;
    }
    else
    {
      return T(value / scale_t(Scaling));
    }
  }

  //***************************************************************************
  /// Round toward zero.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_zero_scaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_zero_unscaled<Scaling>(value) * scale_t(Scaling));
  }

  //***************************************************************************
  /// Round twords infinity or away from zero.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_infinity_unscaled(T value) ETL_NOEXCEPT
  {
    if (value >= 0)
    {
      return etl::round_ceiling_unscaled<Scaling>(value);
    }
    else
    {
      return etl::round_floor_unscaled<Scaling>(value);
    }
  }

  //***************************************************************************
  /// Round twords infinity or away from zero.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_infinity_scaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_infinity_unscaled<Scaling>(value) * scale_t(Scaling));
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded to even integral.
  /// Also known as 'Banker's Rounding'.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_half_even_unscaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (Scaling == 1)
    {
      return value;
    }
    else
    {
      // Half?
      if ((etl::absolute(value) % scale_t(Scaling)) == scale_t(Scaling / 2U))
      {
        // Odd?
        if ((value / scale_t(Scaling)) & 1U)
        {
          return T(round_half_up_unscaled<Scaling>(value));
        }
        else
        {
          return T(round_half_down_unscaled<Scaling>(value));
        }
      }
      else
      {
        return T(round_half_up_unscaled<Scaling>(value));
      }
    }
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded to even integral.
  /// Also known as 'Banker's Rounding'.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_half_even_scaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_half_even_unscaled<Scaling>(value) * scale_t(Scaling));
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded to odd integral.
  /// Also known as 'Banker's Rounding'.
  /// \param value Scaled integral.
  /// \return Unscaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_half_odd_unscaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    if (Scaling == 1)
    {
      return value;
    }
    else
    {
      // Half?
      if ((etl::absolute(value) % scale_t(Scaling)) == scale_t(Scaling / 2U))
      {
        // Odd?
        if ((value / scale_t(Scaling)) & 1U)
        {
          return T(round_half_down_unscaled<Scaling>(value));
        }
        else
        {
          return T(round_half_up_unscaled<Scaling>(value));
        }
      }
      else
      {
        return T(round_half_up_unscaled<Scaling>(value));
      }
    }
  }

  //***************************************************************************
  /// Round to nearest integer. 'Half' value is rounded to odd integral.
  /// Also known as 'Banker's Rounding'.
  /// \param value Scaled integral.
  /// \return Scaled, rounded integral.
  //***************************************************************************
  template <uint32_t Scaling, typename T>
  ETL_NODISCARD
  ETL_CONSTEXPR14
  T round_half_odd_scaled(T value) ETL_NOEXCEPT
  {
    typedef typename scaled_rounding_t<T>::type scale_t;

    return T(round_half_odd_unscaled<Scaling>(value) * scale_t(Scaling));
  }
}

#endif
