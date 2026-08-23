///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 John Wellbelove

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

#ifndef ETL_CLAMPED_VALUE_INCLUDED
#define ETL_CLAMPED_VALUE_INCLUDED

#include "platform.h"
#include "algorithm.h"

///\defgroup clamped_value clamped_value
/// Provides a value that is clamped between two limits.
/// \ingroup utilities

namespace etl
{
  template <typename T, T Min = 0, T Max = 0, bool EtlRuntimeSpecialisation = ((Min == 0) && (Max == 0))>
  class clamped_value;

  //***************************************************************************
  /// Provides a value that is clamped between two compile-time limits.
  /// Supports incrementing, decrementing and arbitrary advance.
  ///\tparam T   The type of the value.
  ///\tparam Min The minimum value of the range.
  ///\tparam Max The maximum value of the range.
  ///\ingroup clamped_value
  //***************************************************************************
  template <typename T, T Min, T Max>
  class clamped_value<T, Min, Max, false>
  {
  public:

    ETL_STATIC_ASSERT(Min <= Max, "clamped_value minimum must not exceed maximum");

    //*************************************************************************
    /// Default constructor.
    /// The initial value is set to the minimum value.
    //*************************************************************************
    ETL_CONSTEXPR clamped_value() ETL_NOEXCEPT
      : value(Min)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Set to an initial value.
    /// Clamped to the range.
    ///\param initial The initial value.
    //*************************************************************************
    ETL_CONSTEXPR14 explicit clamped_value(T initial) ETL_NOEXCEPT
    {
      set(initial);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    ETL_CONSTEXPR clamped_value(const clamped_value& other) ETL_NOEXCEPT
      : value(other.value)
    {
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator=(const clamped_value& other) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      if (this == &other)
      {
        return *this;
      }

      set(other.value);
      return *this;
    }

    //*************************************************************************
    /// Sets the value.
    /// Clamps to the Min/Max range.
    ///\param value_ The value.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T value_) ETL_NOEXCEPT
    {
      value = etl::clamp(value_, Min, Max);
    }

    //*************************************************************************
    /// Resets the value to the minimum in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void to_min() ETL_NOEXCEPT
    {
      value = Min;
    }

    //*************************************************************************
    /// Resets the value to the maximum in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void to_max() ETL_NOEXCEPT
    {
      value = Max;
    }

    //*************************************************************************
    /// Advances to value by a number of steps.
    /// Saturates at the Min/Max range.
    ///\param n The number of steps.
    //*************************************************************************
    ETL_CONSTEXPR14 void advance(int n) ETL_NOEXCEPT
    {
      while ((n > 0) && (value < Max))
      {
        ++(*this);
        --n;
      }

      while ((n < 0) && (value > Min))
      {
        --(*this);
        ++n;
      }
    }

    //*************************************************************************
    /// Conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_CONSTEXPR14 operator T() ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// Const conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_CONSTEXPR operator const T() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    /// Saturates at Max.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator++() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = etl::clamp(static_cast<T>(value + (value < Max)), Min, Max);
      return *this;
    }

    //*************************************************************************
    /// ++ operator.
    /// Saturates at Max.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value operator++(int) ETL_NOEXCEPT
    {
      clamped_value temp(*this);
      ++(*this);
      return temp;
    }

    //*************************************************************************
    /// -- operator.
    /// Saturates at Min.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator--() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = etl::clamp(static_cast<T>(value - (value > Min)), Min, Max);
      return *this;
    }

    //*************************************************************************
    /// -- operator.
    /// Saturates at Min.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value operator--(int) ETL_NOEXCEPT
    {
      clamped_value temp(*this);
      --(*this);
      return temp;
    }

    //*************************************************************************
    /// = operator.
    /// Clamps to the Min/Max range.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator=(T value_) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      set(value_);
      return *this;
    }

    //*************************************************************************
    /// Gets the value.
    //*************************************************************************
    ETL_CONSTEXPR T get() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// Gets the minimum value.
    //*************************************************************************
    static ETL_CONSTEXPR T min() ETL_NOEXCEPT
    {
      return Min;
    }

    //*************************************************************************
    /// Gets the maximum value.
    //*************************************************************************
    static ETL_CONSTEXPR T max() ETL_NOEXCEPT
    {
      return Max;
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    void swap(clamped_value& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap;
      swap(value, other.value);
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    friend void swap(clamped_value& lhs, clamped_value& rhs) ETL_NOEXCEPT
    {
      lhs.swap(rhs);
    }

    //*************************************************************************
    /// Operator ==.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator==(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return lhs.value == rhs.value;
    }

    //*************************************************************************
    /// Operator !=.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator!=(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

  private:

    T value;
  };

  //***************************************************************************
  /// Provides a value that is clamped between two runtime limits.
  /// Supports incrementing, decrementing and arbitrary advance.
  ///\tparam T   The type of the value.
  ///\tparam Min The default minimum value.
  ///\tparam Max The default maximum value.
  ///\ingroup clamped_value
  //***************************************************************************
  template <typename T, T Min, T Max>
  class clamped_value<T, Min, Max, true>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    /// Sets the value and bounds to the template parameter values.
    //*************************************************************************
    ETL_CONSTEXPR clamped_value() ETL_NOEXCEPT
      : value(Min)
      , min_value(Min)
      , max_value(Max)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Sets the value to the minimum of the range.
    ///\param min_ The minimum value.
    ///\param max_ The maximum value.
    //*************************************************************************
    ETL_CONSTEXPR clamped_value(T min_, T max_) ETL_NOEXCEPT
      : value(min_)
      , min_value(min_)
      , max_value(max_)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Set to an initial value.
    /// Clamped to the range.
    ///\param min_ The minimum value.
    ///\param max_ The maximum value.
    ///\param initial The initial value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value(T min_, T max_, T initial) ETL_NOEXCEPT
      : min_value(min_)
      , max_value(max_)
    {
      set(initial);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    ETL_CONSTEXPR clamped_value(const clamped_value& other) ETL_NOEXCEPT
      : value(other.value)
      , min_value(other.min_value)
      , max_value(other.max_value)
    {
    }

    //*************************************************************************
    /// Sets the range.
    /// Sets the value to the minimum of the range.
    ///\param min_ The minimum value.
    ///\param max_ The maximum value.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T min_, T max_) ETL_NOEXCEPT
    {
      min_value = min_;
      max_value = max_;
      value     = min_;
    }

    //*************************************************************************
    /// Sets the value.
    /// Clamps to the runtime Min/Max range.
    ///\param value_ The value.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T value_) ETL_NOEXCEPT
    {
      value = etl::clamp(value_, min_value, max_value);
    }

    //*************************************************************************
    /// Resets the value to the minimum in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void to_min() ETL_NOEXCEPT
    {
      value = min_value;
    }

    //*************************************************************************
    /// Resets the value to the maximum in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void to_max() ETL_NOEXCEPT
    {
      value = max_value;
    }

    //*************************************************************************
    /// Advances to value by a number of steps.
    /// Saturates at the runtime Min/Max range.
    ///\param n The number of steps.
    //*************************************************************************
    ETL_CONSTEXPR14 void advance(int n) ETL_NOEXCEPT
    {
      while ((n > 0) && (value < max_value))
      {
        ++(*this);
        --n;
      }

      while ((n < 0) && (value > min_value))
      {
        --(*this);
        ++n;
      }
    }

    //*************************************************************************
    /// Conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_CONSTEXPR14 operator T() ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// Const conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_CONSTEXPR operator const T() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    /// Saturates at the maximum.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator++() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = etl::clamp(static_cast<T>(value + (value < max_value)), min_value, max_value);
      return *this;
    }

    //*************************************************************************
    /// ++ operator.
    /// Saturates at the maximum.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value operator++(int) ETL_NOEXCEPT
    {
      clamped_value temp(*this);
      ++(*this);
      return temp;
    }

    //*************************************************************************
    /// -- operator.
    /// Saturates at the minimum.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator--() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = etl::clamp(static_cast<T>(value - (value > min_value)), min_value, max_value);
      return *this;
    }

    //*************************************************************************
    /// -- operator.
    /// Saturates at the minimum.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value operator--(int) ETL_NOEXCEPT
    {
      clamped_value temp(*this);
      --(*this);
      return temp;
    }

    //*************************************************************************
    /// = operator.
    /// Clamps to the runtime Min/Max range.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator=(T value_) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      set(value_);
      return *this;
    }

    //*************************************************************************
    /// = operator.
    /// Copies the value and runtime bounds.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator=(const clamped_value& other) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      if (this == &other)
      {
        return *this;
      }

      value     = other.value;
      min_value = other.min_value;
      max_value = other.max_value;
      return *this;
    }

    //*************************************************************************
    /// Gets the value.
    //*************************************************************************
    ETL_CONSTEXPR T get() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// Gets the minimum value.
    //*************************************************************************
    ETL_CONSTEXPR T min() const ETL_NOEXCEPT
    {
      return min_value;
    }

    //*************************************************************************
    /// Gets the maximum value.
    //*************************************************************************
    ETL_CONSTEXPR T max() const ETL_NOEXCEPT
    {
      return max_value;
    }

    //*************************************************************************
    /// Swaps the values and runtime bounds.
    //*************************************************************************
    void swap(clamped_value& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap;
      swap(value, other.value);
      swap(min_value, other.min_value);
      swap(max_value, other.max_value);
    }

    //*************************************************************************
    /// Swaps the values and runtime bounds.
    //*************************************************************************
    friend void swap(clamped_value& lhs, clamped_value& rhs) ETL_NOEXCEPT
    {
      lhs.swap(rhs);
    }

    //*************************************************************************
    /// Operator ==.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator==(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return (lhs.value == rhs.value) && (lhs.min_value == rhs.min_value) && (lhs.max_value == rhs.max_value);
    }

    //*************************************************************************
    /// Operator !=.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator!=(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

  private:

    T value;
    T min_value;
    T max_value;
  };
} // namespace etl

#endif
