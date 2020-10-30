///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#ifndef ETL_FLAGS_INCLUDED
#define ETL_FLAGS_INCLUDED

#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include "platform.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "static_assert.h"

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
  #include <initializer_list>
#endif

namespace etl
{
  //*************************************************************************
  /// flags
  /// Flags based around an integral value.
  //*************************************************************************
  template <typename T, T MASK = etl::integral_limits<T>::max>
  class flags
  {
  public:

    ETL_STATIC_ASSERT(etl::is_integral<T>::value && etl::is_unsigned<T>::value, "Unsigned integral values only");

    typedef T value_type;

    static ETL_CONSTANT value_type ALL_SET   = etl::integral_limits<value_type>::max & MASK;
    static ETL_CONSTANT value_type ALL_CLEAR = 0;

    static const size_t NBITS = etl::integral_limits<value_type>::bits;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    flags() ETL_NOEXCEPT
      : data(value_type(0))
    {
    }

    flags(value_type initial) ETL_NOEXCEPT
      : data(initial & MASK)
    {
    }

    flags(const flags<T, MASK>& initial) ETL_NOEXCEPT
      : data(initial.value())
    {
    }

    //*************************************************************************
    /// Tests bits.
    //*************************************************************************
    template <value_type position>
    bool test() const ETL_NOEXCEPT
    {
      return (data & position) != value_type(0);
    }

    //*******************************************
    bool test(value_type position) const
    {
      return (data & position) != value_type(0);
    }

    //*************************************************************************
    /// Set the bits.
    //*************************************************************************
    template <value_type position, bool value>
    flags<T, MASK>& set() ETL_NOEXCEPT
    {
      if ETL_IF_CONSTEXPR(value)
      {
        data |= position;
      }
      else
      {
        data &= ~position;
      }

      data &= MASK;

      return *this;
    }

    //*******************************************
    template <value_type position>
    flags<T, MASK>& set(bool value) ETL_NOEXCEPT
    {
      if (value)
      {
        data |= position;
      }
      else
      {
        data &= ~position;
      }

      data &= MASK;

      return *this;
    }

    //*******************************************
    template <value_type position>
    flags<T, MASK>& set() ETL_NOEXCEPT
    {
      data |= position;
      data &= MASK;

      return *this;
    }

    //*******************************************
    flags<T, MASK>& set(value_type position) ETL_NOEXCEPT
    {
      data |= position;
      data &= MASK;

      return *this;
    }

    //*******************************************
    flags<T, MASK>& set(value_type position, bool value) ETL_NOEXCEPT
    {
      if (value)
      {
        data |= position;
      }
      else
      {
        data &= ~position;
      }

      data &= MASK;

      return *this;
    }

    //*************************************************************************
    /// Clear all of the flags.
    //*************************************************************************
    flags<T, MASK>& clear() ETL_NOEXCEPT
    {
      data = ALL_CLEAR;

      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the position.
    //*************************************************************************
    template <value_type position>
    flags<T, MASK>& reset() ETL_NOEXCEPT
    {
      data &= ~position;

      return *this;
    }

    //*******************************************
    flags<T, MASK>& reset(value_type position) ETL_NOEXCEPT
    {
      data &= ~position;

      return *this;
    }

    //*************************************************************************
    /// Flip bits.
    //*************************************************************************
    flags<T, MASK>& flip() ETL_NOEXCEPT
    {
      data = ~data;
      data &= MASK;

      return *this;
    }

    //*******************************************
    template <value_type position>
    flags<T, MASK>& flip() ETL_NOEXCEPT
    {
      data ^= position;
      data &= MASK;

      return *this;
    }

    //*******************************************
    flags<T, MASK>& flip(value_type position) ETL_NOEXCEPT
    {
      data ^= position;
      data &= MASK;

      return *this;
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    bool all() const ETL_NOEXCEPT
    {
      return data == MASK;
    }

    //*******************************************
    template <value_type position>
    bool all_of() const ETL_NOEXCEPT
    {
      return (data & (position & MASK)) == (position & MASK);
    }

    //*******************************************
    bool all_of(value_type position) const ETL_NOEXCEPT
    {  
      position &= MASK;
      return (data & position) == position;
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    bool none() const ETL_NOEXCEPT
    {
      return (data & MASK) == ALL_CLEAR;
    }

    //*******************************************
    template <value_type position>
    bool none_of() const ETL_NOEXCEPT
    {
      return !any_of(position);
    }

    //*******************************************
    bool none_of(value_type position) const ETL_NOEXCEPT
    {
      return !any_of(position);
    }

    //*************************************************************************
    /// Are any of the bits set?
    //*************************************************************************
    bool any() const ETL_NOEXCEPT
    {
      return (data & MASK) != value_type(0);
    }

    //*******************************************
    template <value_type position>
    bool any_of() const ETL_NOEXCEPT
    {
      return (data & (position & MASK)) != value_type(0);
    }

    //*******************************************
    bool any_of(value_type position) const
    {
      return (data & (position & MASK)) != value_type(0);
    }

    //*************************************************************************
    /// Return the value of the flags.
    //*************************************************************************
    value_type value() const ETL_NOEXCEPT
    {
      return data;
    }

    //*************************************************************************
    /// Set the value of the flags.
    //*************************************************************************
    void value(value_type initial) ETL_NOEXCEPT
    {
      data = initial & MASK;
    }

    //*************************************************************************
    /// Return the value of the flags.
    //*************************************************************************
    operator value_type() const ETL_NOEXCEPT
    {
      return data;
    }

    //*************************************************************************
    /// operator &=
    //*************************************************************************
    flags<T, MASK>& operator &=(value_type other) ETL_NOEXCEPT
    {
      data &= other;

      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    flags<T, MASK>& operator |=(value_type other) ETL_NOEXCEPT
    {
      data |= other;
      data &= MASK;

      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    flags<T, MASK>& operator ^=(value_type other) ETL_NOEXCEPT
    {
      data ^= other;
      data &= MASK;

      return *this;
    }

    //*************************************************************************
    /// operator =
    //*************************************************************************
    flags<T, MASK>& operator =(flags<T, MASK> other) ETL_NOEXCEPT
    {
      if (this != &other)
      {
        data = other.data;
      }

      return *this;
    }

    //*************************************************************************
    /// operator =
    //*************************************************************************
    flags<T, MASK>& operator =(value_type other) ETL_NOEXCEPT
    {
      data = (other & MASK);

      return *this;
    }

    //*************************************************************************
    /// swap
    //*************************************************************************
    void swap(flags<T, MASK>& other) ETL_NOEXCEPT
    {
      etl::swap(data, other.data);
    }

  private:

    value_type data;
  };

  //***************************************************************************
  /// operator ==
  //***************************************************************************
  template <typename T, T MASK>
  bool operator == (flags<T, MASK> lhs, flags<T, MASK> rhs) ETL_NOEXCEPT
  {
    return lhs.value() == rhs.value();
  }

  //***************************************************************************
  /// operator !=
  //***************************************************************************
  template <typename T, T MASK>
  bool operator != (flags<T, MASK> lhs, flags<T, MASK> rhs) ETL_NOEXCEPT
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// swap
  //*************************************************************************
  template <typename T, T MASK>
  void swap(etl::flags<T, MASK>& lhs, etl::flags<T, MASK>& rhs) ETL_NOEXCEPT
  {
    lhs.swap(rhs);
  }
}

#endif
