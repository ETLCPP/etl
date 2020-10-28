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

    //*************************************************************************
    /// Tests bits.
    //*************************************************************************
    template <value_type position>
    bool test() const ETL_NOEXCEPT
    {
      return (data & position) != value_type(0);
    }

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
    flags<T, MASK>& set() ETL_NOEXCEPT
    {
      data |= position;
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
    /// Reset the flags.
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
    /// Flip all of the bits.
    //*************************************************************************
    flags<T, MASK>& flip() ETL_NOEXCEPT
    {
      data = ~data;
      data &= MASK;

      return *this;
    }

    //*************************************************************************
    /// Flip the bit at the position.
    //*************************************************************************
#if ETL_CPP11_SUPPORTED && ETL_USING_STL
    //*******************************************
    template <value_type... positions>
    flags<T, MASK>& flip() ETL_NOEXCEPT
    {
      for (value_type position : {positions...})
      {
        data ^= position;
      }

      data &= MASK;

      return *this;
    }

    //*******************************************
    template<typename... TArgs>
    flags<T, MASK>& flip(TArgs... positions) ETL_NOEXCEPT
    {
      for (value_type position : {positions...})
      {
        data ^= position;
      }

      data &= MASK;

      return *this;
    }
#else
    //*******************************************
    template <value_type position>
    flags<T, MASK>& flip() ETL_NOEXCEPT
    {
      data ^= position;
      data &= MASK;

      return *this;
    }

    //*******************************************
    flags<T, MASK>& flip(size_type position) ETL_NOEXCEPT
    {
      data ^= position;
      data &= MASK;

      return *this;
    }
#endif

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
      return (data & MASK) == ALL_SET;
    }

#if ETL_CPP11_SUPPORTED&& ETL_USING_STL
    //*******************************************
    template <value_type... positions>
    bool all_of() const ETL_NOEXCEPT
    {
      for (value_type position : {positions...})
      {
        if ((data & position) == value_type(0))
        {
          return false;
        }
      }

      return true;
    }

    //*******************************************
    template<typename... TArgs>
    bool all_of(TArgs... positions) const ETL_NOEXCEPT
    {  
      for (value_type position : {positions...})
      {
        if ((data & position) == value_type(0))
        {
          return false;
        }
      }

      return true;
    }
#endif

    //*******************************************
    bool all_of(value_type position) const ETL_NOEXCEPT
    {
      return (data & position) != value_type(0);
    }

    //*************************************************************************
    /// Are none of the bits set?
    //*************************************************************************
    bool none() const ETL_NOEXCEPT
    {
      return (data & MASK) == ALL_CLEAR;
    }

#if ETL_CPP11_SUPPORTED && ETL_USING_STL
    //*******************************************
    template <value_type... positions>
    bool none_of() const ETL_NOEXCEPT
    {
      return !any_of(positions...);
    }

    //*******************************************
    template<typename... TArgs>
    bool none_of(TArgs... positions) const ETL_NOEXCEPT
    {
      return !any_of(positions...);
    }
#endif

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
      return !none();
    }

#if ETL_CPP11_SUPPORTED && ETL_USING_STL
    //*******************************************
    template <value_type... positions>
    bool any_of() const ETL_NOEXCEPT
    {
      for (value_type position : {positions...})
      {
        if ((data & position) != value_type(0))
        {
          return true;
        }
      }

      return false;
    }

    //*******************************************
    template<typename... TArgs>
    bool any_of(TArgs... positions) const ETL_NOEXCEPT
    {
      for (value_type position : {positions...})
      {
        if ((data & position) != value_type(0))
        {
          return true;
        }
      }

      return false;
    }
#endif

    //*******************************************
    bool any_of(value_type position) const
    {
      return (data & position) != value_type(0);
    }

    //*************************************************************************
    /// Set from a string.
    //*************************************************************************
    flags<T, MASK>& from_string(const char* text) ETL_NOEXCEPT
    {
      reset();

      size_t i = etl::min(NBITS, etl::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == L'1');
      }

      data &= MASK;

      return *this;
    }

    //*************************************************************************
    /// Set from a wide string.
    //*************************************************************************
    flags<T, MASK>& from_string(const wchar_t* text) ETL_NOEXCEPT
    {
      reset();

      size_t i = etl::min(NBITS, etl::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == L'1');
      }

      data &= MASK;

      return *this;
    }

    //*************************************************************************
    /// Set from a u16 string.
    //*************************************************************************
    flags<T, MASK>& from_string(const char16_t* text) ETL_NOEXCEPT
    {
      reset();

      size_t i = etl::min(NBITS, etl::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == u'1');
      }

      data &= MASK;

      return *this;
    }

    //*************************************************************************
    /// Set from a u32 string.
    //*************************************************************************
    flags<T, MASK>& from_string(const char32_t* text) ETL_NOEXCEPT
    {
      reset();

      size_t i = etl::min(NBITS, etl::strlen(text));

      while (i > 0)
      {
        set(--i, *text++ == U'1');
      }

      return *this;
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
    flags<T, MASK>& operator &=(flags<T, MASK> other) ETL_NOEXCEPT
    {
      data &= other.data;

      return *this;
    }

    //*************************************************************************
    /// operator |=
    //*************************************************************************
    flags<T, MASK>& operator |=(flags<T, MASK> other) ETL_NOEXCEPT
    {
      data |= other.data;

      return *this;
    }

    //*************************************************************************
    /// operator ^=
    //*************************************************************************
    flags<T, MASK>& operator ^=(flags<T, MASK> other) ETL_NOEXCEPT
    {
      data ^= other.data;

      return *this;
    }

    //*************************************************************************
    /// operator <<=
    //*************************************************************************
    flags<T, MASK>& operator <<=(size_t shift) ETL_NOEXCEPT
    {
      data <<= shift;

      return *this;
    }

    //*************************************************************************
    /// operator >>=
    //*************************************************************************
    flags<T, MASK>& operator >>=(size_t shift) ETL_NOEXCEPT
    {
      data >>= shift;

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
  /// operator &
  //***************************************************************************
  template <typename T, T MASK>
  flags<T, MASK> operator & (flags<T, MASK> lhs, flags<T, MASK> rhs) ETL_NOEXCEPT
  {
    flags<T, MASK> temp(lhs);
    temp &= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator |
  //***************************************************************************
  template <typename T, T MASK>
  flags<T, MASK> operator | (flags<T, MASK> lhs, flags<T, MASK> rhs) ETL_NOEXCEPT
  {
    flags<T, MASK> temp(lhs);
    temp |= rhs;
    return temp;
  }

  //***************************************************************************
  /// operator ^
  //***************************************************************************
  template <typename T, T MASK>
  flags<T, MASK> operator ^ (flags<T, MASK> lhs, flags<T, MASK> rhs) ETL_NOEXCEPT
  {
    flags<T, MASK> temp(lhs);
    temp ^= rhs;
    return temp;
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
