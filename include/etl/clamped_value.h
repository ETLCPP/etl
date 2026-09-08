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
#include "absolute.h"
#include "algorithm.h"
#include "error_handler.h"
#include "limits.h"
#include "math.h"
#include "static_assert.h"
#include "type_traits.h"
#include "utility.h"

///\def ETL_HAS_FLOATING_POINT_CLAMPED_VALUE
/// Set to `1` when clamped_value supports floating-point types.
/// Floating-point support requires C++20 non-type template arguments.
/// The macro may be defined by the build or platform profile to override
/// automatic detection.
#if !defined(ETL_HAS_FLOATING_POINT_CLAMPED_VALUE)
  #if ETL_USING_CPP20 && defined(__cpp_nontype_template_args) && (__cpp_nontype_template_args >= 201911L)
    #define ETL_HAS_FLOATING_POINT_CLAMPED_VALUE 1
  #else
    #define ETL_HAS_FLOATING_POINT_CLAMPED_VALUE 0
  #endif
#endif

///\defgroup clamped_value clamped_value
/// Provides a value that is clamped between two limits.
/// Integral types are supported in all language modes. Floating-point types
/// are supported when ETL_HAS_FLOATING_POINT_CLAMPED_VALUE is `1`.
/// \ingroup utilities

namespace etl
{
  /// \cond INTERNAL
  namespace private_clamped_value
  {
    //*************************************************************************
    /// Type policy for clamped_value.
    /// Integral values use a signed difference type and unsigned saturation
    /// arithmetic. Floating-point values use `T` as the difference type.
    //*************************************************************************
    template <typename T, bool IsIntegral = etl::is_integral<T>::value>
    struct traits;

    template <typename T>
    struct traits<T, true>
    {
      typedef typename etl::make_signed<T>::type   difference_type;
      typedef typename etl::make_unsigned<T>::type unsigned_type;
      typedef etl::numeric_limits<T>               limits_type;
      typedef etl::numeric_limits<difference_type> difference_limits_type;
    };

    template <typename T>
    struct traits<T, false>
    {
      ETL_STATIC_ASSERT(ETL_HAS_FLOATING_POINT_CLAMPED_VALUE&& etl::is_floating_point<T>::value,
                        "clamped_value requires an integral type, or a floating-point type when supported");

      typedef T                      difference_type;
      typedef etl::numeric_limits<T> limits_type;
      typedef etl::numeric_limits<T> difference_limits_type;
    };

    template <typename T, bool IsIntegral = etl::is_integral<T>::value>
    struct arithmetic;

    //*************************************************************************
    /// Saturating arithmetic policy for integral values.
    //*************************************************************************
    template <typename T>
    struct arithmetic<T, true>
    {
      typedef typename traits<T>::unsigned_type unsigned_type;

      ETL_NODISCARD
      static ETL_CONSTEXPR14 T                         advance(T value, T min_value, T max_value, typename traits<T>::difference_type n) ETL_NOEXCEPT
      {
        if (n == 0)
        {
          return value;
        }

        const unsigned_type current = static_cast<unsigned_type>(value);
        const unsigned_type step    = etl::absolute_unsigned(n);

        if ((n > 0) && (value < max_value))
        {
          const unsigned_type distance = static_cast<unsigned_type>(max_value) - current;

          value = (step >= distance) ? max_value : static_cast<T>(current + step);
        }
        else if ((n < 0) && (value > min_value))
        {
          const unsigned_type distance = current - static_cast<unsigned_type>(min_value);

          value = (step >= distance) ? min_value : static_cast<T>(current - step);
        }

        return value;
      }

      ETL_NODISCARD
      static ETL_CONSTEXPR14 T subtract(T value, T min_value, T max_value, typename traits<T>::difference_type n) ETL_NOEXCEPT
      {
        if (n == traits<T>::difference_limits_type::lowest()) ETL_UNLIKELY
        {
          value = advance(value, min_value, max_value, traits<T>::difference_limits_type::max());
          return advance(value, min_value, max_value, 1);
        }

        return advance(value, min_value, max_value, static_cast<typename traits<T>::difference_type>(-n));
      }
    };

#if ETL_HAS_FLOATING_POINT_CLAMPED_VALUE
    //*************************************************************************
    /// Saturating arithmetic policy for floating-point values.
    //*************************************************************************
    template <typename T>
    struct arithmetic<T, false>
    {
      ETL_NODISCARD
      static ETL_CONSTEXPR14 T advance(T value, T min_value, T max_value, T n) ETL_NOEXCEPT
      {
        if ((n > T(0)) && (value < max_value))
        {
          const T distance = max_value - value;
          value            = (n >= distance) ? max_value : static_cast<T>(value + n);
        }
        else if ((n < T(0)) && (value > min_value))
        {
          const T distance = value - min_value;
          value            = (-n >= distance) ? min_value : static_cast<T>(value + n);
        }

        return value;
      }

      ETL_NODISCARD
      static ETL_CONSTEXPR14 T subtract(T value, T min_value, T max_value, T n) ETL_NOEXCEPT
      {
        return advance(value, min_value, max_value, -n);
      }
    };
#endif

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
      return arithmetic<T>::advance(value, min_value, max_value, n);
    }

    //*************************************************************************
    /// Subtracts a value and saturates it at the supplied bounds.
    /// Handles the lowest integral difference without signed overflow.
    ///\param value The current value.
    ///\param min_value The minimum value.
    ///\param max_value The maximum value.
    ///\param n The number of steps to subtract.
    ///\return The value after subtraction.
    //*************************************************************************
    template <typename T>
    ETL_NODISCARD ETL_CONSTEXPR14 T subtract(T value, T min_value, T max_value, typename traits<T>::difference_type n) ETL_NOEXCEPT
    {
      return arithmetic<T>::subtract(value, min_value, max_value, n);
    }

    //*************************************************************************
    /// Verifies that a clamped value, bound, or difference is not NaN.
    /// Integral values always pass validation.
    ///\param value The value to validate.
    //*************************************************************************
    template <typename T>
    ETL_CONSTEXPR14 void validate(T value) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      ETL_ASSERT(!etl::is_nan(value), ETL_ERROR_GENERIC("clamped_value: NaN is not supported"));
    }

    //*************************************************************************
    /// Validates a value and its bounds before clamping.
    ///\param value The value to clamp.
    ///\param min_value The minimum permitted value.
    ///\param max_value The maximum permitted value.
    ///\return The value clamped to the supplied range.
    //*************************************************************************
    template <typename T>
    ETL_NODISCARD ETL_CONSTEXPR14 T validated_clamp(T value, T min_value, T max_value) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      validate(value);
      validate(min_value);
      validate(max_value);
      ETL_ASSERT(min_value <= max_value, ETL_ERROR_GENERIC("clamped_value: invalid range"));
      return etl::clamp(value, min_value, max_value);
    }
  } // namespace private_clamped_value
  /// \endcond

#include "private/diagnostic_float_equal_push.h"
  template <typename T, T Min = T(), T Max = T(), bool RuntimeSpecialisation = ((Min == T()) && (Max == T()))>
  class clamped_value;
#include "private/diagnostic_pop.h"

  //***************************************************************************
  /// Provides a value that is clamped between two compile-time limits.
  /// Supports incrementing, decrementing and arbitrary advance.
  ///@tparam T   An integral type, or a floating-point type when
  ///            ETL_HAS_FLOATING_POINT_CLAMPED_VALUE is `1`.
  ///@tparam Min The minimum value of the range.
  ///@tparam Max The maximum value of the range.
  ///\ingroup clamped_value
  //***************************************************************************
  template <typename T, T Min, T Max>
  class clamped_value<T, Min, Max, false>
  {
  public:

    /// The type used to advance or subtract from the stored value.
    typedef typename private_clamped_value::traits<T>::difference_type difference_type;

    /// Numeric limits for the stored value type.
    typedef typename private_clamped_value::traits<T>::limits_type limits_type;

    /// Numeric limits for difference_type.
    typedef typename private_clamped_value::traits<T>::difference_limits_type difference_limits_type;

    /// \cond INTERNAL
    ETL_STATIC_ASSERT(Min <= Max, "clamped_value minimum must not exceed maximum");
    /// \endcond

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
    /// NaN is rejected for floating-point values.
    ///\param initial The initial value.
    //*************************************************************************
    ETL_CONSTEXPR14 explicit clamped_value(T initial) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
      : value(private_clamped_value::validated_clamp(initial, Min, Max))
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
    /// NaN is rejected for floating-point values.
    ///\param value_ The value.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T value_) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      value = private_clamped_value::validated_clamp(value_, Min, Max);
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
    /// Advances the value by a number of steps.
    /// Saturates at the Min/Max range.
    /// Floating-point steps may be fractional. NaN is rejected.
    ///\param n The number of steps.
    //*************************************************************************
    ETL_CONSTEXPR14 void advance(difference_type n) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      private_clamped_value::validate(n);
      value = private_clamped_value::advance(value, Min, Max, n);
    }

    //*************************************************************************
    /// Conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 operator T() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    /// Advances by one.
    /// Saturates at Max.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator++() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = private_clamped_value::advance(value, Min, Max, difference_type(1));
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
    /// Subtracts one.
    /// Saturates at Min.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator--() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = private_clamped_value::advance(value, Min, Max, difference_type(-1));
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
    ETL_CONSTEXPR14 clamped_value& operator=(T value_) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      set(value_);
      return *this;
    }

    //*************************************************************************
    /// Adds a number of steps and clamps to the range.
    /// Floating-point steps may be fractional. NaN is rejected.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator+=(difference_type n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      advance(n);
      return *this;
    }

    //*************************************************************************
    /// Subtracts a number of steps and clamps to the range.
    /// Floating-point steps may be fractional. NaN is rejected.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator-=(difference_type n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      private_clamped_value::validate(n);
      value = private_clamped_value::subtract(value, Min, Max, n);
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
    ETL_CONSTEXPR14 void swap(clamped_value& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap;
      swap(value, other.value);
    }

    //*************************************************************************
    /// Swaps the values.
    ///\param lhs The first value.
    ///\param rhs The second value.
    //*************************************************************************
    friend ETL_CONSTEXPR14 void swap(clamped_value& lhs, clamped_value& rhs) ETL_NOEXCEPT
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
#include "private/diagnostic_float_equal_push.h"
      return lhs.value == rhs.value;
#include "private/diagnostic_pop.h"
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

    /// @}

  private:

    T value;
  };

  //***************************************************************************
  /// Provides a value that is clamped between two runtime limits.
  /// Supports incrementing, decrementing and arbitrary advance.
  ///@tparam T   An integral type, or a floating-point type when
  ///            ETL_HAS_FLOATING_POINT_CLAMPED_VALUE is `1`.
  ///@tparam Min Ignored for this specialisation.
  ///@tparam Max Ignored for this specialisation.
  ///\ingroup clamped_value
  //***************************************************************************
  template <typename T, T Min, T Max>
  class clamped_value<T, Min, Max, true>
  {
  public:

    /// The type used to advance or subtract from the stored value.
    typedef typename private_clamped_value::traits<T>::difference_type difference_type;

    /// Numeric limits for the stored value type.
    typedef typename private_clamped_value::traits<T>::limits_type limits_type;

    /// Numeric limits for difference_type.
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
    /// Reversed bounds and NaN bounds are rejected.
    ///\param min_ The minimum value.
    ///\param max_ The maximum value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value(T min_, T max_) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
      : value(min_)
      , min_value(min_)
      , max_value(max_)
    {
      private_clamped_value::validate(min_);
      private_clamped_value::validate(max_);
      ETL_ASSERT(min_ <= max_, ETL_ERROR_GENERIC("clamped_value: invalid range"));
    }

    //*************************************************************************
    /// Constructor.
    /// Set to an initial value.
    /// Clamped to the range.
    /// Reversed bounds and NaN values or bounds are rejected.
    ///\param min_ The minimum value.
    ///\param max_ The maximum value.
    ///\param initial The initial value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value(T min_, T max_, T initial) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
      : value(private_clamped_value::validated_clamp(initial, min_, max_))
      , min_value(min_)
      , max_value(max_)
    {
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
    /// Reversed bounds and NaN bounds are rejected.
    ///\param min_ The minimum value.
    ///\param max_ The maximum value.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T min_, T max_) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      private_clamped_value::validate(min_);
      private_clamped_value::validate(max_);
      ETL_ASSERT(min_ <= max_, ETL_ERROR_GENERIC("clamped_value: invalid range"));
      min_value = min_;
      max_value = max_;
      value     = min_;
    }

    //*************************************************************************
    /// Sets the value.
    /// Clamps to the runtime Min/Max range.
    /// NaN is rejected for floating-point values.
    ///\param value_ The value.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T value_) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      value = private_clamped_value::validated_clamp(value_, min_value, max_value);
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
    /// Advances the value by a number of steps.
    /// Saturates at the runtime Min/Max range.
    /// Floating-point steps may be fractional. NaN is rejected.
    ///\param n The number of steps.
    //*************************************************************************
    ETL_CONSTEXPR14 void advance(difference_type n) ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      private_clamped_value::validate(n);
      value = private_clamped_value::advance(value, min_value, max_value, n);
    }

    //*************************************************************************
    /// Conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 operator T() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    /// Advances by one.
    /// Saturates at the maximum.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator++() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = private_clamped_value::advance(value, min_value, max_value, difference_type(1));
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
    /// Subtracts one.
    /// Saturates at the minimum.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator--() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = private_clamped_value::advance(value, min_value, max_value, difference_type(-1));
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
    ETL_CONSTEXPR14 clamped_value& operator=(T value_) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
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
    /// Floating-point steps may be fractional. NaN is rejected.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator+=(difference_type n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      advance(n);
      return *this;
    }

    //*************************************************************************
    /// Subtracts a number of steps and clamps to the range.
    /// Floating-point steps may be fractional. NaN is rejected.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    ETL_CONSTEXPR14 clamped_value& operator-=(difference_type n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT_IF(ETL_NOT_USING_EXCEPTIONS)
    {
      private_clamped_value::validate(n);
      value = private_clamped_value::subtract(value, min_value, max_value, n);
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
    ETL_CONSTEXPR14 void swap(clamped_value& other) ETL_NOEXCEPT
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
    friend ETL_CONSTEXPR14 void swap(clamped_value& lhs, clamped_value& rhs) ETL_NOEXCEPT
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
#include "private/diagnostic_float_equal_push.h"
      return lhs.value == rhs.value;
#include "private/diagnostic_pop.h"
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

    /// @}

  private:

    T value;
    T min_value;
    T max_value;
  };
} // namespace etl

#endif
