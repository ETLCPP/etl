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
#include "static_assert.h"
#include "exception.h"
#include "static_assert.h"
#include "type_traits.h"
#include "algorithm.h"

#include <stddef.h>

namespace etl
{
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
    cyclic_value()
      : value(First)
    {
    }

    //*************************************************************************
    /// Constructor.
    /// Set to an initial value.
    /// Clamped to the range.
    //*************************************************************************
    explicit cyclic_value(T initial)
    {
      set(initial);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    cyclic_value(const cyclic_value<T, First, Last>& other)
      : value(other.value)
    {
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    cyclic_value& operator =(const cyclic_value<T, First, Last>& other)
    {
      value = other.value;

      return *this;
    }

    //*************************************************************************
    /// Sets the value.
    /// Truncates to the First/Last range.
    ///\param value The value.
    //*************************************************************************
    void set(T value_)
    {
      if (value_ > Last)
      {
        value_ = Last;
      }
      else if (value_ < First)
      {
        value_ = First;
      }

      value = value_;
    }

    //*************************************************************************
    /// Resets the value to the first in the range.
    //*************************************************************************
    void to_first()
    {
      value = First;
    }

    //*************************************************************************
    /// Resets the value to the last in the range.
    //*************************************************************************
    void to_last()
    {
      value = Last;
    }

    //*************************************************************************
    /// Advances to value by a number of steps.
    ///\param n The number of steps to advance.
    //*************************************************************************
    void advance(int n)
    {
      if (n > 0)
      {
        for (int i = 0; i < n; ++i)
        {
          operator ++();
        }
      }
      else
      {
        for (int i = 0; i < -n; ++i)
        {
          operator --();
        }
      }
    }

    //*************************************************************************
    /// Conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    operator T()
    {
      return value;
    }

    //*************************************************************************
    /// Const conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    operator const T() const
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    //*************************************************************************
    cyclic_value& operator ++()
    {
      if (value >= Last) ETL_UNLIKELY
      {
        value = First;
      }
      else
      {
        ++value;
      }

      return *this;
    }

    //*************************************************************************
    /// ++ operator.
    //*************************************************************************
    cyclic_value operator ++(int)
    {
      cyclic_value temp(*this);

      operator++();

      return temp;
    }

    //*************************************************************************
    /// -- operator.
    //*************************************************************************
    cyclic_value& operator --()
    {
      if (value <= First) ETL_UNLIKELY
      {
        value = Last;
      }
      else
      {
        --value;
      }

      return *this;
    }

    //*************************************************************************
    /// -- operator.
    //*************************************************************************
    cyclic_value operator --(int)
    {
      cyclic_value temp(*this);

      operator--();

      return temp;
    }

    //*************************************************************************
    /// = operator.
    //*************************************************************************
    cyclic_value& operator =(T t)
    {
      set(t);
      return *this;
    }

    //*************************************************************************
    /// = operator.
    //*************************************************************************
    template <const T FIRST2, const T LAST2>
    cyclic_value& operator =(const cyclic_value<T, FIRST2, LAST2>& other)
    {
      set(other.get());
      return *this;
    }

    //*************************************************************************
    /// Gets the value.
    //*************************************************************************
    T get() const
    {
      return value;
    }

    //*************************************************************************
    /// Gets the first value.
    //*************************************************************************
    static ETL_CONSTEXPR T first()
    {
      return First;
    }

    //*************************************************************************
    /// Gets the last value.
    //*************************************************************************
    static ETL_CONSTEXPR T last()
    {
      return Last;
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    void swap(cyclic_value<T, First, Last>& other)
    {
      using ETL_OR_STD::swap; // Allow ADL

      swap(value, other.value);
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    friend void swap(cyclic_value<T, First, Last>& lhs, cyclic_value<T, First, Last>& rhs)
    {
      lhs.swap(rhs);
    }

    //*************************************************************************
    /// Operator ==.
    //*************************************************************************
    friend bool operator == (const cyclic_value<T, First, Last>& lhs, const cyclic_value<T, First, Last>& rhs)
    {
      return lhs.value == rhs.value;
    }

    //*************************************************************************
    /// Operator !=.
    //*************************************************************************
    friend bool operator != (const cyclic_value<T, First, Last>& lhs, const cyclic_value<T, First, Last>& rhs)
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
    cyclic_value()
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
    cyclic_value(T first_, T last_)
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
    cyclic_value(T first_, T last_, T initial)
      : first_value(first_)
      , last_value(last_)
    {
      set(initial);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    cyclic_value(const cyclic_value& other)
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
    void set(T first_, T last_)
    {
      first_value = first_;
      last_value  = last_;
      value       = first_;
    }

    //*************************************************************************
    /// Sets the value.
    ///\param value The value.
    //*************************************************************************
    void set(T value_)
    {
      if (value_ > last_value)
      {
        value_ = last_value;
      }
      else if (value_ < first_value)
      {
        value_ = first_value;
      }

      value = value_;
    }

    //*************************************************************************
    /// Resets the value to the first in the range.
    //*************************************************************************
    void to_first()
    {
      value = first_value;
    }

    //*************************************************************************
    /// Resets the value to the last in the range.
    //*************************************************************************
    void to_last()
    {
      value = last_value;
    }

    //*************************************************************************
    /// Advances to value by a number of steps.
    ///\param n The number of steps to advance.
    //*************************************************************************
    void advance(int n)
    {
      if (n > 0)
      {
        for (int i = 0; i < n; ++i)
        {
          operator ++();
        }
      }
      else
      {
        for (int i = 0; i < -n; ++i)
        {
          operator --();
        }
      }
    }

    //*************************************************************************
    /// Conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    operator T()
    {
      return value;
    }

    //*************************************************************************
    /// Const conversion operator.
    /// \return The value of the underlying type.
    //*************************************************************************
    operator const T() const
    {
      return value;
    }

    //*************************************************************************
    /// ++ operator.
    //*************************************************************************
    cyclic_value& operator ++()
    {
      if (value >= last_value)
      {
        value = first_value;
      }
      else
      {
        ++value;
      }

      return *this;
    }

    //*************************************************************************
    /// ++ operator.
    //*************************************************************************
    cyclic_value operator ++(int)
    {
      cyclic_value temp(*this);

      operator++();

      return temp;
    }

    //*************************************************************************
    /// -- operator.
    //*************************************************************************
    cyclic_value& operator --()
    {
      if (value <= first_value)
      {
        value = last_value;
      }
      else
      {
        --value;
      }

      return *this;
    }

    //*************************************************************************
    /// -- operator.
    //*************************************************************************
    cyclic_value operator --(int)
    {
      cyclic_value temp(*this);

      operator--();

      return temp;
    }

    //*************************************************************************
    /// = operator.
    //*************************************************************************
    cyclic_value& operator =(T t)
    {
      set(t);
      return *this;
    }

    //*************************************************************************
    /// = operator.
    //*************************************************************************
    cyclic_value& operator =(const cyclic_value& other)
    {
      value       = other.value;
      first_value = other.first_value;
      last_value  = other.last_value;
      return *this;
    }

    //*************************************************************************
    /// Gets the value.
    //*************************************************************************
    T get() const
    {
      return value;
    }

    //*************************************************************************
    /// Gets the first value.
    //*************************************************************************
    T first() const
    {
      return first_value;
    }

    //*************************************************************************
    /// Gets the last value.
    //*************************************************************************
    T last() const
    {
      return last_value;
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    void swap(cyclic_value<T, First, Last>& other)
    {
      using ETL_OR_STD::swap; // Allow ADL

      swap(first_value, other.first_value);
      swap(last_value, other.last_value);
      swap(value, other.value);
    }

    //*************************************************************************
    /// Swaps the values.
    //*************************************************************************
    friend void swap(cyclic_value<T, First, Last>& lhs, cyclic_value<T, First, Last>& rhs)
    {
      lhs.swap(rhs);
    }

    //*************************************************************************
    /// Operator ==.
    //*************************************************************************
    friend bool operator == (const cyclic_value<T, First, Last>& lhs, const cyclic_value<T, First, Last>& rhs)
    {
      return (lhs.value       == rhs.value) &&
             (lhs.first_value == rhs.first_value) &&
             (lhs.last_value  == rhs.last_value);
    }

    //*************************************************************************
    /// Operator !=.
    //*************************************************************************
    friend bool operator != (const cyclic_value<T, First, Last>& lhs, const cyclic_value<T, First, Last>& rhs)
    {
      return !(lhs == rhs);
    }

  private:

    T value;       ///< The current value.
    T first_value; ///< The first value in the range.
    T last_value;  ///< The last value in the range.
  };
}

#endif
