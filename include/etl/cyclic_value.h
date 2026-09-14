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

#ifndef ETL_CYCLIC_VALUE_INCLUDED
#define ETL_CYCLIC_VALUE_INCLUDED

///\defgroup cyclic_value cyclic_value
/// Provides a value that cycles between two limits.
/// \ingroup utilities

#include "platform.h"
#include "algorithm.h"
#include "exception.h"
#include "integral_limits.h"
#include "static_assert.h"
#include "type_traits.h"

namespace etl
{
  namespace private_cyclic_value
  {
    //*************************************************************************
    /// Advances a value and wraps it at the supplied limits.
    ///\param value The current value.
    ///\param min_value The minimum value.
    ///\param max_value The maximum value.
    ///\param n The number of steps.
    ///\return The advanced value.
    //*************************************************************************
    template <typename T, typename TStep>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 T advance(T value, T min_value, T max_value, TStep n) ETL_NOEXCEPT
    {
      typedef typename etl::make_unsigned<T>::type unsigned_type;

      if (n == 0)
      {
        return value;
      }

      // Check for full range limits.
      const bool is_full_range = (min_value == etl::integral_limits<T>::min) && (max_value == etl::integral_limits<T>::max);

      if (is_full_range)
      {
        // This is safe as the type's own overflow behaviour is modulo wrap-around.
        return static_cast<T>(etl::to_unsigned(value) + static_cast<unsigned_type>(n));
      }
      else
      {
        // Size of the wrap-around range.
        const unsigned_type range = (etl::to_unsigned(max_value) - etl::to_unsigned(min_value)) + 1U;

        // Value expressed as an offset from min_value (0 .. range - 1).
        const unsigned_type current = etl::to_unsigned(value) - etl::to_unsigned(min_value);

        // Reduce the step to something within a single lap of the range.
        const unsigned_type step = etl::absolute_unsigned(n) % range;

        // Direction-corrected offset to add, always in [0, range).
        const unsigned_type offset = (n > 0) ? step : (range - step) % range;

        return static_cast<T>(etl::to_unsigned(min_value) + (current + offset) % range);
      }
    }
  } // namespace private_cyclic_value

  //***************************************************************************
  /// Provides a value that cycles between two limits.
  //***************************************************************************
  template <typename T, T First = 0, T Last = 0, bool EtlRuntimeSpecialisation = ((First == 0) && (Last == 0))>
  class cyclic_value;

  //***************************************************************************
  /// Provides a value that cycles between two compile time limits.
  /// Supports incrementing and decrementing.
  ///\tparam T     The type of the variable.
  ///\tparam First The first value of the range.
  ///\tparam Last  The last value of the range.
  ///\ingroup cyclic_value
  //***************************************************************************
  template <typename T, T First, T Last>
  class cyclic_value<T, First, Last, false>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    /// The initial value is set to the first value.
    //*************************************************************************
    ETL_CONSTEXPR cyclic_value() ETL_NOEXCEPT
      : value(First)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Set to an initial value.
    /// Clamped to the range.
    //*************************************************************************
    ETL_CONSTEXPR explicit cyclic_value(T initial) ETL_NOEXCEPT
      : value(etl::clamp(initial, First, Last))
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    ETL_CONSTEXPR cyclic_value(const cyclic_value<T, First, Last>& other) ETL_NOEXCEPT
      : value(other.value)
    {
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value& operator=(const cyclic_value<T, First, Last>& other) ETL_NOEXCEPT
    {
      value = other.value;

      return *this;
    }

    //*************************************************************************
    /// Adds a number of steps and clamps to the range.
    /// Floating-point steps may be fractional. NaN is rejected.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    template <typename TStep>
    ETL_CONSTEXPR14 cyclic_value& operator+=(TStep n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
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
    template <typename TStep>
    ETL_CONSTEXPR14 cyclic_value& operator-=(TStep n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = private_cyclic_value::advance(value, First, Last, -n);
      return *this;
    }

    //*************************************************************************
    /// Sets the value.
    /// Truncates to the First/Last range.
    ///\param value The value.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T value_) ETL_NOEXCEPT
    {
      value = etl::clamp(value_, First, Last);
    }

    //*************************************************************************
    /// Resets the value to the first in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void to_first() ETL_NOEXCEPT
    {
      value = First;
    }

    //*************************************************************************
    /// Resets the value to the last in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void to_last() ETL_NOEXCEPT
    {
      value = Last;
    }

    //*************************************************************************
    /// Advances to value by a number of steps.
    ///\param n The number of steps to advance.
    //*************************************************************************
    template <typename TStep>
    ETL_CONSTEXPR14 void advance(TStep n) ETL_NOEXCEPT
    {
      value = private_cyclic_value::advance(value, First, Last, n);
    }

    //*************************************************************************
    /// Const conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_CONSTEXPR operator T() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value& operator++() ETL_NOEXCEPT
    {
      value = private_cyclic_value::advance(value, First, Last, 1);

      return *this;
    }

    //*************************************************************************
    /// ++ operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value operator++(int) ETL_NOEXCEPT
    {
      cyclic_value temp(*this);

      operator++();

      return temp;
    }

    //*************************************************************************
    /// -- operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value& operator--() ETL_NOEXCEPT
    {
      value = private_cyclic_value::advance(value, First, Last, int(-1));

      return *this;
    }

    //*************************************************************************
    /// -- operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value operator--(int) ETL_NOEXCEPT
    {
      cyclic_value temp(*this);

      operator--();

      return temp;
    }

    //*************************************************************************
    /// = operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value& operator=(T t) ETL_NOEXCEPT
    {
      set(t);
      return *this;
    }

    //*************************************************************************
    /// = operator.
    //*************************************************************************
    template <T FIRST2, T LAST2>
    ETL_CONSTEXPR14 cyclic_value& operator=(const cyclic_value<T, FIRST2, LAST2>& other) ETL_NOEXCEPT
    {
      set(other.get());
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
    /// Gets the first value.
    //*************************************************************************
    static ETL_CONSTEXPR T first() ETL_NOEXCEPT
    {
      return First;
    }

    //*************************************************************************
    /// Gets the last value.
    //*************************************************************************
    static ETL_CONSTEXPR T last() ETL_NOEXCEPT
    {
      return Last;
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    ETL_CONSTEXPR14 void swap(cyclic_value<T, First, Last>& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap; // Allow ADL

      swap(value, other.value);
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    friend ETL_CONSTEXPR14 void swap(cyclic_value<T, First, Last>& lhs, cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
    {
      lhs.swap(rhs);
    }

    //*************************************************************************
    /// Operator ==.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator==(const cyclic_value<T, First, Last>& lhs, const cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
    {
      return lhs.value == rhs.value;
    }

    //*************************************************************************
    /// Operator !=.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator!=(const cyclic_value<T, First, Last>& lhs, const cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

  private:

    T value; ///< The current value.
  };

  //***************************************************************************
  /// Provides a value that cycles between two run time limits.
  /// Supports incrementing and decrementing.
  ///\tparam T     The type of the variable.
  ///\tparam First The first value of the range.
  ///\tparam Last  The last value of the range.
  ///\ingroup cyclic_value
  //***************************************************************************
  template <typename T, T First, T Last>
  class cyclic_value<T, First, Last, true>
  {
  public:

    //*************************************************************************
    /// Constructor.
    /// Sets 'first' and 'last' to the template parameter values.
    /// The initial value is set to the first value.
    //*************************************************************************
    ETL_CONSTEXPR cyclic_value() ETL_NOEXCEPT
      : value(First)
      , first_value(First)
      , last_value(Last)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Sets the value to the first of the range.
    ///\param first The first value in the range.
    ///\param last  The last value in the range.
    //*************************************************************************
    ETL_CONSTEXPR cyclic_value(T first_, T last_) ETL_NOEXCEPT
      : value(first_)
      , first_value(first_)
      , last_value(last_)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Set to an initial value.
    /// Clamped to the range.
    ///\param first The first value in the range.
    ///\param last  The last value in the range.
    //*************************************************************************
    ETL_CONSTEXPR cyclic_value(T first_, T last_, T initial) ETL_NOEXCEPT
      : value(etl::clamp(initial, first_, last_))
      , first_value(first_)
      , last_value(last_)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    ETL_CONSTEXPR cyclic_value(const cyclic_value& other) ETL_NOEXCEPT
      : value(other.value)
      , first_value(other.first_value)
      , last_value(other.last_value)
    {
    }

    //*************************************************************************
    /// Sets the range.
    /// Sets the value to the first of the range.
    ///\param first The first value in the range.
    ///\param last  The last value in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T first_, T last_) ETL_NOEXCEPT
    {
      first_value = first_;
      last_value  = last_;
      value       = first_;
    }

    //*************************************************************************
    /// Sets the value.
    ///\param value The value.
    //*************************************************************************
    ETL_CONSTEXPR14 void set(T value_) ETL_NOEXCEPT
    {
      value = etl::clamp(value_, first_value, last_value);
    }

    //*************************************************************************
    /// Resets the value to the first in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void to_first() ETL_NOEXCEPT
    {
      value = first_value;
    }

    //*************************************************************************
    /// Resets the value to the last in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 void to_last() ETL_NOEXCEPT
    {
      value = last_value;
    }

    //*************************************************************************
    /// Advances to value by a number of steps.
    ///\param n The number of steps to advance.
    //*************************************************************************
    ETL_CONSTEXPR14 void advance(int n) ETL_NOEXCEPT
    {
      value = private_cyclic_value::advance(value, first_value, last_value, n);
    }

    //*************************************************************************
    /// Const conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    ETL_CONSTEXPR operator T() const ETL_NOEXCEPT
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value& operator++() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = private_cyclic_value::advance(value, first_value, last_value, 1);

      return *this;
    }

    //*************************************************************************
    /// ++ operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value operator++(int) ETL_NOEXCEPT
    {
      cyclic_value temp(*this);

      operator++();

      return temp;
    }

    //*************************************************************************
    /// -- operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value& operator--() ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = private_cyclic_value::advance(value, first_value, last_value, int(-1));

      return *this;
    }

    //*************************************************************************
    /// -- operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value operator--(int) ETL_NOEXCEPT
    {
      cyclic_value temp(*this);

      operator--();

      return temp;
    }

    //*************************************************************************
    /// = operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value& operator=(T t) ETL_NOEXCEPT
    {
      set(t);
      return *this;
    }

    //*************************************************************************
    /// = operator.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value& operator=(const cyclic_value& other) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value       = other.value;
      first_value = other.first_value;
      last_value  = other.last_value;
      return *this;
    }

    //*************************************************************************
    /// Adds a number of steps and clamps to the range.
    /// Floating-point steps may be fractional. NaN is rejected.
    ///\param n The number of steps.
    ///\return A reference to this value.
    //*************************************************************************
    template <typename TStep>
    ETL_CONSTEXPR14 cyclic_value& operator+=(TStep n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
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
    template <typename TStep>
    ETL_CONSTEXPR14 cyclic_value& operator-=(TStep n) ETL_LVALUE_REF_QUALIFIER ETL_NOEXCEPT
    {
      value = private_cyclic_value::advance(value, first_value, last_value, -n);
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
    /// Gets the first value.
    //*************************************************************************
    ETL_CONSTEXPR T first() const ETL_NOEXCEPT
    {
      return first_value;
    }

    //*************************************************************************
    /// Gets the last value.
    //*************************************************************************
    ETL_CONSTEXPR T last() const ETL_NOEXCEPT
    {
      return last_value;
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    ETL_CONSTEXPR14 void swap(cyclic_value<T, First, Last>& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap; // Allow ADL

      swap(first_value, other.first_value);
      swap(last_value, other.last_value);
      swap(value, other.value);
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    friend ETL_CONSTEXPR14 void swap(cyclic_value<T, First, Last>& lhs, cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
    {
      lhs.swap(rhs);
    }

    //*************************************************************************
    /// Operator ==
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator==(const cyclic_value<T, First, Last>& lhs, const cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
    {
      return (lhs.value == rhs.value);
    }

    //*************************************************************************
    /// Operator !=
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator!=(const cyclic_value<T, First, Last>& lhs, const cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// Operator <
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is less than rhs.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator<(const cyclic_value& lhs, const cyclic_value& rhs) ETL_NOEXCEPT
    {
      return lhs.value < rhs.value;
    }

    //*************************************************************************
    /// Operator <=
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is less than or equal to rhs.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator<=(const cyclic_value& lhs, const cyclic_value& rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }

    //*************************************************************************
    /// Operator >
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is greater than rhs.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator>(const cyclic_value& lhs, const cyclic_value& rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }

    //*************************************************************************
    /// Operator >=
    ///\param lhs The left-hand operand.
    ///\param rhs The right-hand operand.
    ///\return `true` if lhs is greater than or equal to rhs.
    //*************************************************************************
    friend ETL_CONSTEXPR bool operator>=(const cyclic_value& lhs, const cyclic_value& rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }

  private:

    T value;       ///< The current value.
    T first_value; ///< The first value in the range.
    T last_value;  ///< The last value in the range.
  };
} // namespace etl

#endif
