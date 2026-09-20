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
#include "largest.h"
#include "negative.h"
#include "static_assert.h"
#include "type_traits.h"

namespace etl
{
  namespace private_cyclic_value
  {
    template <typename T>
    struct traits
    {
      typedef typename etl::make_unsigned<T>::type unsigned_type;
    };

    //*************************************************************************
    /// Increments a value and wraps it at the supplied limits.
    ///\param value The current value.
    ///\param min_value The minimum value.
    ///\param max_value The maximum value.
    ///\return The new value.
    //*************************************************************************
    template <typename T>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 T increment(T value, T min_value, T max_value) ETL_NOEXCEPT
    {
      return value == max_value ? min_value : static_cast<T>(etl::to_unsigned(value) + 1U);
    }

    //*************************************************************************
    /// Decrements a value and wraps it at the supplied limits.
    ///\param value The current value.
    ///\param min_value The minimum value.
    ///\param max_value The maximum value.
    ///\return The new value.
    //*************************************************************************
    template <typename T>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 T decrement(T value, T min_value, T max_value) ETL_NOEXCEPT
    {
      return value == min_value ? max_value : static_cast<T>(etl::to_unsigned(value) - 1U);
    }

    //*************************************************************************
    /// Advances a value and wraps it at the supplied limits.
    ///\param value The current value.
    ///\param min_value The minimum value.
    ///\param max_value The maximum value.
    ///\param n The number of steps.
    ///\return The advanced value.
    //*************************************************************************
    template <typename T>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 T advance(T value, T min_value, T max_value, typename traits<T>::unsigned_type step, bool subtract) ETL_NOEXCEPT
    {
      typedef typename traits<T>::unsigned_type unsigned_type;

      switch (step)
      {
        case 0:
          {
            return value;
          }

        case 1:
          {
            return subtract ? decrement(value, min_value, max_value) : increment(value, min_value, max_value);
          }

        default:
          {
            if (subtract)
            {
              unsigned_type distance_to_first = static_cast<unsigned_type>(etl::to_unsigned(value) - etl::to_unsigned(min_value));

              if (step <= distance_to_first)
              {
                // Room to subtract the step.
                value = static_cast<T>(etl::to_unsigned(value) - step);
              }
              else
              {
                // Step would roll over.
                // Note: 'distance_to_first + 1U' can overflow to 0 when 'distance_to_first' is the maximum value of 'unsigned_type',
                // which only happens when the range spans the type's full width. This is safe and expected: unsigned overflow is
                // well-defined modular arithmetic, so 'step -= (distance_to_first + 1U)' remains correct modulo 2^N in that case.
                step -= distance_to_first + 1U;
                value = static_cast<T>(etl::to_unsigned(max_value) - step);
              }
            }
            else
            {
              unsigned_type distance_to_last = static_cast<unsigned_type>(etl::to_unsigned(max_value) - etl::to_unsigned(value));

              if (step <= distance_to_last)
              {
                // Room to add the step.
                value = static_cast<T>(etl::to_unsigned(value) + step);
              }
              else
              {
                // Step would roll over.
                // Note: 'distance_to_first + 1U' can overflow to 0 when 'distance_to_first' is the maximum value of 'unsigned_type',
                // which only happens when the range spans the type's full width. This is safe and expected: unsigned overflow is
                // well-defined modular arithmetic, so 'step -= (distance_to_first + 1U)' remains correct modulo 2^N in that case.
                step -= distance_to_last + 1U;
                value = static_cast<T>(etl::to_unsigned(min_value) + step);
              }
            }

            return value;
          }
      }
    }
  } // namespace private_cyclic_value

  struct cyclic_value_exception : etl::exception
  {
    cyclic_value_exception(string_type reason_, string_type file_, numeric_type line_)
      : etl::exception(reason_, file_, line_)
    {
    }
  };

  struct cyclic_value_reversed_limits : cyclic_value_exception
  {
    cyclic_value_reversed_limits(string_type file_, numeric_type line_)
      : cyclic_value_exception(ETL_ERROR_TEXT("cyclic_value_exception:reversed limits", ETL_CYCLIC_VALUE_FILE_ID"A"), file_, line_)
    {
    }
  };

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
  private:

    typedef typename private_cyclic_value::traits<T>::unsigned_type unsigned_type;

  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "T must be an integral type");
    ETL_STATIC_ASSERT(First <= Last, "First is not <= Last");

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
      do_advance(n, !etl::is_negative(n));

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
    ETL_CONSTEXPR14 T advance(TStep n) ETL_NOEXCEPT
    {
      do_advance(n, etl::is_negative(n));

      return value;
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
      value = private_cyclic_value::increment(value, First, Last);

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
      value = private_cyclic_value::decrement(value, First, Last);

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

  private:

    //*************************************************************************
    template <typename TStep>
    void do_advance(TStep n, bool subtract) ETL_NOEXCEPT
    {
      // Determine the widest unsigned type of the two to use for limiting the step count.
      typedef typename etl::largest<typename etl::make_unsigned<TStep>::type, unsigned_type>::type wide_unsigned_type;

      // Force the step to be within the range of the cyclic value.
      const wide_unsigned_type range              = static_cast<wide_unsigned_type>(Last - First) + 1U;
      const wide_unsigned_type wide_unsigned_step = static_cast<wide_unsigned_type>(etl::absolute_unsigned(n)) % range;

      // This is safe as wide_unsigned_step is now within the range of the cyclic value.
      const unsigned_type step = static_cast<unsigned_type>(wide_unsigned_step);

      value = private_cyclic_value::advance(value, First, Last, step, subtract);
    }

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
  private:

    typedef typename private_cyclic_value::traits<T>::unsigned_type unsigned_type;

  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value, "T must be an integral type");

    //*************************************************************************
    /// Constructor.
    /// Sets 'first' and 'last' to the template parameter values which will be zero.
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
    ETL_CONSTEXPR14 cyclic_value(T first_, T last_)
      : value(first_)
      , first_value(first_)
      , last_value(last_)
    {
      ETL_ASSERT(first_ <= last_, ETL_ERROR(cyclic_value_reversed_limits));
    }

    //*************************************************************************
    /// Constructor.
    /// Set to an initial value.
    /// Clamped to the range.
    ///\param first The first value in the range.
    ///\param last  The last value in the range.
    //*************************************************************************
    ETL_CONSTEXPR14 cyclic_value(T first_, T last_, T initial)
      : value(etl::clamp(initial, first_, last_))
      , first_value(first_)
      , last_value(last_)
    {
      ETL_ASSERT(first_ <= last_, ETL_ERROR(cyclic_value_reversed_limits));
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
    ETL_CONSTEXPR14 void set(T first_, T last_)
    {
      ETL_ASSERT(first_ <= last_, ETL_ERROR(cyclic_value_reversed_limits));

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
    template <typename TStep>
    ETL_CONSTEXPR14 void advance(TStep n) ETL_NOEXCEPT
    {
      do_advance(n, etl::is_negative(n));
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
      value = private_cyclic_value::increment(value, first_value, last_value);

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
      value = private_cyclic_value::decrement(value, first_value, last_value);

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
      do_advance(n, !etl::is_negative(n));

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

  private:

    //*************************************************************************
    template <typename TStep>
    void do_advance(TStep n, bool subtract) ETL_NOEXCEPT
    {
      // Determine the widest unsigned type of the two to use for limiting the step count.
      typedef typename etl::largest<typename etl::make_unsigned<TStep>::type, unsigned_type>::type wide_unsigned_type;

      // Force the step to be within the range of the cyclic value.
      const wide_unsigned_type range              = static_cast<wide_unsigned_type>(last_value - first_value) + 1U;
      const wide_unsigned_type wide_unsigned_step = static_cast<wide_unsigned_type>(etl::absolute_unsigned(n)) % range;

      // This is safe as wide_unsigned_step is now within the range of the cyclic value.
      const unsigned_type step = static_cast<unsigned_type>(wide_unsigned_step);

      value = private_cyclic_value::advance(value, first_value, last_value, step, subtract);
    }

    T value;       ///< The current value.
    T first_value; ///< The first value in the range.
    T last_value;  ///< The last value in the range.
  };
} // namespace etl

//*************************************************************************
/// Swaps the values.
//*************************************************************************
template <typename T, T First, T Last>
ETL_CONSTEXPR14 void swap(etl::cyclic_value<T, First, Last>& lhs, etl::cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
{
  lhs.swap(rhs);
}

//*************************************************************************
/// Operator ==
///\param lhs The left-hand operand.
///\param rhs The right-hand operand.
///\return `true` if lhs == rhs.
//*************************************************************************
template <typename T, T First, T Last>
ETL_CONSTEXPR bool operator==(const etl::cyclic_value<T, First, Last>& lhs, const etl::cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
{
  return (lhs.get() == rhs.get());
}

//*************************************************************************
/// Operator !=
///\param lhs The left-hand operand.
///\param rhs The right-hand operand.
///\return `true` if lhs != rhs.
//*************************************************************************
template <typename T, T First, T Last>
ETL_CONSTEXPR bool operator!=(const etl::cyclic_value<T, First, Last>& lhs, const etl::cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
{
  return !(lhs == rhs);
}

//*************************************************************************
/// Operator <
///\param lhs The left-hand operand.
///\param rhs The right-hand operand.
///\return `true` if lhs is less than rhs.
//*************************************************************************
template <typename T, T First, T Last>
ETL_CONSTEXPR bool operator<(const etl::cyclic_value<T, First, Last>& lhs, const etl::cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
{
  return lhs.get() < rhs.get();
}

//*************************************************************************
/// Operator <=
///\param lhs The left-hand operand.
///\param rhs The right-hand operand.
///\return `true` if lhs is less than or equal to rhs.
//*************************************************************************
template <typename T, T First, T Last>
ETL_CONSTEXPR bool operator<=(const etl::cyclic_value<T, First, Last>& lhs, const etl::cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
{
  return !(rhs < lhs);
}

//*************************************************************************
/// Operator >
///\param lhs The left-hand operand.
///\param rhs The right-hand operand.
///\return `true` if lhs is greater than rhs.
//*************************************************************************
template <typename T, T First, T Last>
ETL_CONSTEXPR bool operator>(const etl::cyclic_value<T, First, Last>& lhs, const etl::cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
{
  return rhs < lhs;
}

//*************************************************************************
/// Operator >=
///\param lhs The left-hand operand.
///\param rhs The right-hand operand.
///\return `true` if lhs is greater than or equal to rhs.
//*************************************************************************
template <typename T, T First, T Last>
ETL_CONSTEXPR bool operator>=(const etl::cyclic_value<T, First, Last>& lhs, const etl::cyclic_value<T, First, Last>& rhs) ETL_NOEXCEPT
{
  return !(lhs < rhs);
}

#endif
