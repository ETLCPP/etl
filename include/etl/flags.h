///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#include "platform.h"
#include "algorithm.h"
#include "type_traits.h"
#include "integral_limits.h"
#include "static_assert.h"
#include "initializer_list.h"

#include <string.h>
#include <stddef.h>
#include <stdint.h>

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

    static ETL_CONSTANT size_t NBITS = etl::integral_limits<value_type>::bits;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ETL_CONSTEXPR flags() ETL_NOEXCEPT
      : data(value_type(0))
    {
    }

    ETL_CONSTEXPR flags(value_type pattern) ETL_NOEXCEPT
      : data(pattern & MASK)
    {
    }

    ETL_CONSTEXPR flags(const flags<T, MASK>& pattern) ETL_NOEXCEPT
      : data(pattern.value())
    {
    }

    //*************************************************************************
    /// Tests bits.
    //*************************************************************************
    template <value_type pattern>
    ETL_CONSTEXPR bool test() const ETL_NOEXCEPT
    {
      return (data & pattern) != value_type(0);
    }

    //*******************************************
    ETL_CONSTEXPR bool test(value_type pattern) const ETL_NOEXCEPT
    {
      return (data & pattern) != value_type(0);
    }

    //*************************************************************************
    /// Set the bits.
    //*************************************************************************
    template <value_type pattern, bool value>
    ETL_CONSTEXPR14 flags<T, MASK>& set() ETL_NOEXCEPT
    {
      value ? data |= (pattern & MASK) : data &= (~pattern & MASK);

      return *this;
    }

    //*******************************************
    template <value_type pattern>
    ETL_CONSTEXPR14 flags<T, MASK>& set(bool value) ETL_NOEXCEPT
    {
      value ? data |= (pattern & MASK) : data &= (~pattern & MASK);

      return *this;
    }

    //*******************************************
    template <value_type pattern>
    ETL_CONSTEXPR14 flags<T, MASK>& set() ETL_NOEXCEPT
    {
      data |= (pattern & MASK);

      return *this;
    }

    //*******************************************
    ETL_CONSTEXPR14 flags<T, MASK>& set(value_type pattern) ETL_NOEXCEPT
    {
      data |= (pattern & MASK);

      return *this;
    }

    //*******************************************
    ETL_CONSTEXPR14 flags<T, MASK>& set(value_type pattern, bool value) ETL_NOEXCEPT
    {
      value ? data |= (pattern & MASK) : data &= (~pattern & MASK);

      return *this;
    }

    //*************************************************************************
    /// Clear all of the flags.
    //*************************************************************************
    ETL_CONSTEXPR14 flags<T, MASK>& clear() ETL_NOEXCEPT
    {
      data = ALL_CLEAR;

      return *this;
    }

    //*************************************************************************
    /// Reset the bit at the pattern.
    //*************************************************************************
    template <value_type pattern>
    ETL_CONSTEXPR14 flags<T, MASK>& reset() ETL_NOEXCEPT
    {
      data &= ~pattern;

      return *this;
    }

    //*******************************************
    ETL_CONSTEXPR14 flags<T, MASK>& reset(value_type pattern) ETL_NOEXCEPT
    {
      data &= ~pattern;

      return *this;
    }

    //*************************************************************************
    /// Flip bits.
    //*************************************************************************
    ETL_CONSTEXPR14 flags<T, MASK>& flip() ETL_NOEXCEPT
    {
      data = (~data & MASK);

      return *this;
    }

    //*******************************************
    template <value_type pattern>
    ETL_CONSTEXPR14 flags<T, MASK>& flip() ETL_NOEXCEPT
    {
      data ^= pattern & MASK;

      return *this;
    }

    //*******************************************
    ETL_CONSTEXPR14 flags<T, MASK>& flip(value_type pattern) ETL_NOEXCEPT
    {
      data ^= pattern & MASK;

      return *this;
    }

    //*************************************************************************
    // Are all the bits sets?
    //*************************************************************************
    ETL_CONSTEXPR bool all() const ETL_NOEXCEPT
    {
      return data == MASK;
    }

    //*******************************************
    template <value_type pattern>
    ETL_CONSTEXPR bool all_of() const ETL_NOEXCEPT
    {
      return (data & (pattern & MASK)) == (pattern & MASK);
    }

    //*******************************************
    ETL_CONSTEXPR bool all_of(value_type pattern) const ETL_NOEXCEPT
    {
      return (data & (pattern & MASK)) == (pattern & MASK);
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR bool none() const ETL_NOEXCEPT
    {
      return (data & MASK) == ALL_CLEAR;
    }

    //*******************************************
    template <value_type pattern>
    ETL_CONSTEXPR bool none_of() const ETL_NOEXCEPT
    {
      return !any_of(pattern);
    }

    //*******************************************
    ETL_CONSTEXPR bool none_of(value_type pattern) const ETL_NOEXCEPT
    {
      return !any_of(pattern);
    }

    //*************************************************************************
    /// Are any of the bits set?
    //*************************************************************************
    ETL_CONSTEXPR bool any() const ETL_NOEXCEPT
    {
      return (data & MASK) != value_type(0);
    }

    //*******************************************
    template <value_type pattern>
    ETL_CONSTEXPR bool any_of() const ETL_NOEXCEPT
    {
      return (data & (pattern & MASK)) != value_type(0);
    }

    //*******************************************
    ETL_CONSTEXPR bool any_of(value_type pattern) const
    {
      return (data & (pattern & MASK)) != value_type(0);
    }

    //*************************************************************************
    /// Return the value of the flags.
    //*************************************************************************
    ETL_CONSTEXPR value_type value() const ETL_NOEXCEPT
    {
      return data;
    }

    //*************************************************************************
    /// Set the value of the flags.
    //*************************************************************************
    ETL_CONSTEXPR14 flags<T, MASK>& value(value_type pattern) ETL_NOEXCEPT
    {
      data = pattern & MASK;

      return *this;
    }

    //*************************************************************************
    /// Return the value of the flags.
    //*************************************************************************
    ETL_CONSTEXPR operator value_type() const ETL_NOEXCEPT
    {
      return data;
    }

    //*************************************************************************
    /// operator &=
    //*************************************************************************
    ETL_CONSTEXPR14 flags<T, MASK>& operator &=(value_type pattern) ETL_NOEXCEPT
    {
      data &= pattern;

      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    ETL_CONSTEXPR14 flags<T, MASK>& operator |=(value_type pattern) ETL_NOEXCEPT
    {
      data |= (pattern & MASK);

      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    ETL_CONSTEXPR14 flags<T, MASK>& operator ^=(value_type pattern) ETL_NOEXCEPT
    {
      data ^= (pattern & MASK);

      return *this;
    }

    //*************************************************************************
    /// operator =
    //*************************************************************************
    ETL_CONSTEXPR14 flags<T, MASK>& operator =(flags<T, MASK> other) ETL_NOEXCEPT
    {
      data = other.data;

      return *this;
    }

    //*************************************************************************
    /// operator =
    //*************************************************************************
    ETL_CONSTEXPR14 flags<T, MASK>& operator =(value_type pattern) ETL_NOEXCEPT
    {
      data = (pattern & MASK);

      return *this;
    }

    //*************************************************************************
    /// swap
    //*************************************************************************
    void swap(flags<T, MASK>& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap;
      swap(data, other.data);
    }

  private:

    value_type data;
  };

  //***************************************************************************
  /// operator ==
  //***************************************************************************
  template <typename T, T MASK>
  ETL_CONSTEXPR bool operator == (flags<T, MASK> lhs, flags<T, MASK> rhs) ETL_NOEXCEPT
  {
    return lhs.value() == rhs.value();
  }

  //***************************************************************************
  /// operator !=
  //***************************************************************************
  template <typename T, T MASK>
  ETL_CONSTEXPR bool operator != (flags<T, MASK> lhs, flags<T, MASK> rhs) ETL_NOEXCEPT
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
