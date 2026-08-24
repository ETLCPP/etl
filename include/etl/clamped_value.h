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
#include "error_handler.h"
#include "limits.h"
#include "static_assert.h"
#include "type_traits.h"
#include "utility.h"

///\defgroup clamped_value clamped_value
/// Provides a value that is clamped between two limits.
/// \ingroup utilities

namespace etl
{
  namespace private_clamped_value
  {
    //*************************************************************************
    /// Common integral types and arithmetic for clamped_value.
    //*************************************************************************
    template <typename T>
    struct traits
    {
      ETL_STATIC_ASSERT(etl::is_integral<T>::value, "clamped_value requires an integral type");

      typedef typename etl::make_signed<T>::type   difference_type;
      typedef typename etl::make_unsigned<T>::type unsigned_type;
      typedef etl::numeric_limits<T>               limits_type;
      typedef etl::numeric_limits<difference_type> difference_limits_type;
    };

    //*************************************************************************
    /// Advances a value and saturates it at the supplied bounds.
    ///\param value The current value.
    ///\param min_value The minimum value.
    ///\param max_value The maximum value.
    ///\param n The number of steps.
    ///\return The advanced value.
    //*************************************************************************
    template <typename T>
    ETL_NODISCARD ETL_CONSTEXPR14 T advance(T value, T min_value, T max_value, typename traits<T>::difference_type n) ETL_NOEXCEPT
    {
      typedef typename traits<T>::unsigned_type unsigned_type;

      if ((n > 0) && (value < max_value))
      {
        const unsigned_type distance = static_cast<unsigned_type>(max_value) - static_cast<unsigned_type>(value);
        const unsigned_type step     = static_cast<unsigned_type>(n);

        value = (step >= distance) ? max_value : static_cast<T>(static_cast<unsigned_type>(value) + step);
      }

      if ((n < 0) && (value > min_value))
      {
        const unsigned_type distance = static_cast<unsigned_type>(value) - static_cast<unsigned_type>(min_value);
        const unsigned_type step     = static_cast<unsigned_type>(-(n + 1)) + 1U;

        value = (step >= distance) ? min_value : static_cast<T>(static_cast<unsigned_type>(value) - step);
      }

      return value;
    }
  } // namespace private_clamped_value

  template <typename T, T Min = 0, T Max = 0, bool RuntimeSpecialisation = ((Min == 0) && (Max == 0))>
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

    typedef typename private_clamped_value::traits<T>::difference_type        difference_type;
    typedef typename private_clamped_value::traits<T>::limits_type            limits_type;
    typedef typename private_clamped_value::traits<T>::difference_limits_type difference_limits_type;

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
      : value(etl::clamp(initial, Min, Max))
    {
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
    ///\param other The value to copy.
    ///\return A reference to this value.
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
    ETL_CONSTEXPR14 void advance(difference_type n) ETL_NOEXCEPT
    {
      value = private_clamped_value::advance(value, Min, Max, n);
    }

    //*************************************************************************
    /// Conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_CONSTEXPR14 operator T() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    /// Saturates at Max.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator++() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      if (value < Max)
      {
        ++value;
      }
      return *this;
    }

    //*************************************************************************
    /// ++ operator.
    /// Saturates at Max.
    ///\return The value before incrementing.
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
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator--() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      if (value > Min)
      {
        --value;
      }
      return *this;
    }

    //*************************************************************************
    /// -- operator.
    /// Saturates at Min.
    ///\return The value before decrementing.
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
    ///\param value_ The value to assign.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator=(T value_) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      set(value_);
      return *this;
    }

    //*************************************************************************
    /// Adds a number of steps and clamps to the range.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator+=(difference_type n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      advance(n);
      return *this;
    }

    //*************************************************************************
    /// Subtracts a number of steps and clamps to the range.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator-=(difference_type n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      if (n == difference_limits_type::lowest()) ETL_UNLIKELY
      {
        advance(difference_limits_type::max());
        ++(*this);
      }
      else
      {
        advance(static_cast<difference_type>(-n));
      }

      return *this;
    }

    //*************************************************************************
    /// Gets the value.
    ///\return The current value.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR T get() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// Gets the minimum value.
    ///\return The minimum value.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR T min() const ETL_NOEXCEPT
    {
      return Min;
    }

    //*************************************************************************
    /// Gets the maximum value.
    ///\return The maximum value.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR T max() const ETL_NOEXCEPT
    {
      return Max;
    }

    //*************************************************************************
    /// Swaps the values.
    ///\param other The value to swap with.
    //*************************************************************************
    void swap(clamped_value& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap;
      swap(value, other.value);
    }

    //*************************************************************************
    /// Swaps the values.
    ///\param lhs The first value.
    ///\param rhs The second value.
    //*************************************************************************
    friend void swap(clamped_value& lhs, clamped_value& rhs) ETL_NOEXCEPT
    {
      lhs.swap(rhs);
    }

    //*************************************************************************
    /// Operator ==.
    ///\param lhs The left-hand value.
    ///\param rhs The right-hand value.
    ///\return `true` if the values are equal.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator==(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return lhs.value == rhs.value;
    }

    //*************************************************************************
    /// Operator !=.
    ///\param lhs The left-hand value.
    ///\param rhs The right-hand value.
    ///\return `true` if the values are not equal.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator!=(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Operator <.
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is less than rhs.
    /// @{
    friend ETL_CONSTEXPR bool operator<(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return lhs.value < rhs.value;
    }

    friend ETL_CONSTEXPR bool operator<(const clamped_value& lhs, T rhs) ETL_NOEXCEPT
    {
      return lhs.value < rhs;
    }

    friend ETL_CONSTEXPR bool operator<(T lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return lhs < rhs.value;
    }
    /// @}

    //*************************************************************************
    /// Operator <=.
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is less than or equal to rhs.
    /// @{
    friend ETL_CONSTEXPR bool operator<=(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }

    friend ETL_CONSTEXPR bool operator<=(const clamped_value& lhs, T rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }

    friend ETL_CONSTEXPR bool operator<=(T lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }
    /// @}

    //*************************************************************************
    /// Operator >.
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is greater than rhs.
    /// @{
    friend ETL_CONSTEXPR bool operator>(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }

    friend ETL_CONSTEXPR bool operator>(const clamped_value& lhs, T rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }

    friend ETL_CONSTEXPR bool operator>(T lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }
    /// @}

    //*************************************************************************
    /// Operator >=.
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is greater than or equal to rhs.
    /// @{
    friend ETL_CONSTEXPR bool operator>=(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }

    friend ETL_CONSTEXPR bool operator>=(const clamped_value& lhs, T rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }

    friend ETL_CONSTEXPR bool operator>=(T lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }
    /// @}

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

    typedef typename private_clamped_value::traits<T>::difference_type        difference_type;
    typedef typename private_clamped_value::traits<T>::limits_type            limits_type;
    typedef typename private_clamped_value::traits<T>::difference_limits_type difference_limits_type;

    //*************************************************************************
    /// Default constructor.
    /// Sets the value to the minimum representable value of `T` and the
    /// bounds to the full range of `T`.
    //*************************************************************************
    ETL_CONSTEXPR clamped_value() ETL_NOEXCEPT
      : value(limits_type::lowest())
      , min_value(limits_type::lowest())
      , max_value(limits_type::max())
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Sets the value to the minimum of the range.
    ///\param min_ The minimum value.
    ///\param max_ The maximum value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value(T min_, T max_) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
      : value(min_)
      , min_value(min_)
      , max_value(max_)
    {
      ETL_ASSERT(min_ <= max_, ETL_ERROR_GENERIC("clamped_value: invalid range"));
    }

    //*************************************************************************
    /// Constructor.
    /// Set to an initial value.
    /// Clamped to the range.
    ///\param min_ The minimum value.
    ///\param max_ The maximum value.
    ///\param initial The initial value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value(T min_, T max_, T initial) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
      : value(initial)
      , min_value(min_)
      , max_value(max_)
    {
      ETL_ASSERT(min_ <= max_, ETL_ERROR_GENERIC("clamped_value: invalid range"));
      value = etl::clamp(initial, min_, max_);
    }

    //*************************************************************************
    /// Copy constructor.
    ///\param other The value to copy.
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
    ETL_CONSTEXPR14 void set(T min_, T max_) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(min_ <= max_, ETL_ERROR_GENERIC("clamped_value: invalid range"));
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
    ETL_CONSTEXPR14 void advance(difference_type n) ETL_NOEXCEPT
    {
      value = private_clamped_value::advance(value, min_value, max_value, n);
    }

    //*************************************************************************
    /// Conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_CONSTEXPR14 operator T() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    /// Saturates at the maximum.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator++() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      if (value < max_value)
      {
        ++value;
      }
      return *this;
    }

    //*************************************************************************
    /// ++ operator.
    /// Saturates at the maximum.
    ///\return The value before incrementing.
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
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator--() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      if (value > min_value)
      {
        --value;
      }
      return *this;
    }

    //*************************************************************************
    /// -- operator.
    /// Saturates at the minimum.
    ///\return The value before decrementing.
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
    ///\param value_ The value to assign.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator=(T value_) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      set(value_);
      return *this;
    }

    //*************************************************************************
    /// = operator.
    /// Copies the value and runtime bounds.
    ///\param other The value to copy.
    ///\return A reference to this value.
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
    /// Adds a number of steps and clamps to the range.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator+=(difference_type n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      advance(n);
      return *this;
    }

    //*************************************************************************
    /// Subtracts a number of steps and clamps to the range.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator-=(difference_type n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      if (n == difference_limits_type::lowest()) ETL_UNLIKELY
      {
        advance(difference_limits_type::max());
        ++(*this);
      }
      else
      {
        advance(static_cast<difference_type>(-n));
      }

      return *this;
    }

    //*************************************************************************
    /// Gets the value.
    ///\return The current value.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR T get() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// Gets the minimum value.
    ///\return The minimum value.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR T min() const ETL_NOEXCEPT
    {
      return min_value;
    }

    //*************************************************************************
    /// Gets the maximum value.
    ///\return The maximum value.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR T max() const ETL_NOEXCEPT
    {
      return max_value;
    }

    //*************************************************************************
    /// Swaps the values and runtime bounds.
    ///\param other The value to swap with.
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
    ///\param lhs The first value.
    ///\param rhs The second value.
    //*************************************************************************
    friend void swap(clamped_value& lhs, clamped_value& rhs) ETL_NOEXCEPT
    {
      lhs.swap(rhs);
    }

    //*************************************************************************
    /// Operator ==.
    ///\param lhs The left-hand value.
    ///\param rhs The right-hand value.
    ///\return `true` if the values are equal.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator==(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return lhs.value == rhs.value;
    }

    //*************************************************************************
    /// Operator !=.
    ///\param lhs The left-hand value.
    ///\param rhs The right-hand value.
    ///\return `true` if the values differ.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator!=(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Operator <.
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is less than rhs.
    /// @{
    friend ETL_CONSTEXPR bool operator<(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return lhs.value < rhs.value;
    }
    friend ETL_CONSTEXPR bool operator<(const clamped_value& lhs, T rhs) ETL_NOEXCEPT
    {
      return lhs.value < rhs;
    }
    friend ETL_CONSTEXPR bool operator<(T lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return lhs < rhs.value;
    }
    /// @}

    //*************************************************************************
    /// Operator <=.
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is less than or equal to rhs.
    /// @{
    friend ETL_CONSTEXPR bool operator<=(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }

    friend ETL_CONSTEXPR bool operator<=(const clamped_value& lhs, T rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }

    friend ETL_CONSTEXPR bool operator<=(T lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }
    /// @}

    //*************************************************************************
    /// Operator >.
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is greater than rhs.
    /// @{
    friend ETL_CONSTEXPR bool operator>(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }

    friend ETL_CONSTEXPR bool operator>(const clamped_value& lhs, T rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }

    friend ETL_CONSTEXPR bool operator>(T lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }
    /// @}

    //*************************************************************************
    /// Operator >=.
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is greater than or equal to rhs.
    /// @{
    friend ETL_CONSTEXPR bool operator>=(const clamped_value& lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }

    friend ETL_CONSTEXPR bool operator>=(const clamped_value& lhs, T rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }

    friend ETL_CONSTEXPR bool operator>=(T lhs, const clamped_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }
    /// @}

  private:

    T value;
    T min_value;
    T max_value;
  };
} // namespace etl

#endif
