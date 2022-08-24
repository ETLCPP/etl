///\file

/******************************************************************************
The MIT License(MIT)
Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com
Copyright(c) 2022 John Wellbelove
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

#ifndef ETL_BYTE_INCLUDED
#define ETL_BYTE_INCLUDED

#include "platform.h"
#include "type_traits.h"

namespace etl
{
#if ETL_USING_CPP11 && !defined(ETL_BYTE_FORCE_CPP03_IMPLEMENTATION)

  enum class byte : unsigned char {};

  //*************************************************************************
  /// To integer.
  //*************************************************************************
  template <typename TInteger>
  constexpr
    typename etl::enable_if<etl::is_integral<TInteger>::value, TInteger>::type
    to_integer(etl::byte b) noexcept
  {
    return TInteger(b);
  }

  //*************************************************************************
  /// Shift left.
  //*************************************************************************
  template <typename TInteger>
  constexpr
    typename etl::enable_if<etl::is_integral<TInteger>::value, etl::byte>::type
    operator <<(etl::byte b, TInteger shift) noexcept
  {
    return  etl::byte(static_cast<unsigned int>(b) << shift);
  }

  //*************************************************************************
  /// Shift right
  //*************************************************************************
  template <typename TInteger>
  constexpr
    typename etl::enable_if<etl::is_integral<TInteger>::value, etl::byte>::type
    operator >>(etl::byte b, TInteger shift) noexcept
  {
    return  etl::byte(static_cast<unsigned int>(b) >> shift);
  }

  //*************************************************************************
  /// Shift left equals.
  //*************************************************************************
  template <typename TInteger>
  constexpr
    typename etl::enable_if<etl::is_integral<TInteger>::value, etl::byte&>::type
    operator <<=(etl::byte& b, TInteger shift) noexcept
  {
    return b = b << shift;;
  }

  //*************************************************************************
  /// Shift right equals.
  //*************************************************************************
  template <typename TInteger>
  constexpr
    typename etl::enable_if<etl::is_integral<TInteger>::value, etl::byte&>::type
    operator >>=(etl::byte& b, TInteger shift) noexcept
  {
    return b = b >> shift;
  }

  //*************************************************************************
  /// Or.
  //*************************************************************************
  constexpr etl::byte operator |(etl::byte lhs, etl::byte rhs) noexcept
  {
    return etl::byte(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
  }

  //*************************************************************************
  /// And.
  //*************************************************************************
  constexpr etl::byte operator &(etl::byte lhs, etl::byte rhs) noexcept
  {
    return etl::byte(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
  }

  //*************************************************************************
  /// Exclusive Or.
  //*************************************************************************
  constexpr etl::byte operator ^(etl::byte lhs, etl::byte rhs) noexcept
  {
    return etl::byte(static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs));
  }

  //*************************************************************************
  /// Or equals.
  //*************************************************************************
  ETL_CONSTEXPR14 etl::byte& operator |=(etl::byte& lhs, etl::byte rhs) noexcept
  {
    return lhs = lhs | rhs;
  }

  //*************************************************************************
  /// And equals
  //*************************************************************************
  ETL_CONSTEXPR14 etl::byte& operator &=(etl::byte& lhs, etl::byte rhs) noexcept
  {
    return lhs = lhs & rhs;
  }

  //*************************************************************************
  /// Exclusive or equals.
  //*************************************************************************
  ETL_CONSTEXPR14 etl::byte& operator ^=(etl::byte& lhs, etl::byte rhs) noexcept
  {
    return lhs = lhs ^ rhs;
  }

  //*************************************************************************
  /// Not.
  //*************************************************************************
  constexpr etl::byte operator ~(etl::byte b) noexcept
  {
    return etl::byte(~static_cast<unsigned int>(b));
  }

#else

  //*************************************************************************
  /// The byte class.
  //*************************************************************************
  class byte
  {
  public:

    // Friend functions
    template <typename TInteger>
    friend
    typename etl::enable_if<etl::is_integral<TInteger>::value, TInteger>::type
    to_integer(etl::byte b);

    friend bool operator ==(etl::byte lhs, etl::byte rhs);

    // Default constructor
    byte()
      : value(0U)
    {
    }

    // Construct from a value castable to unsigned char
    template <typename T>
    explicit byte(T v)
      : value(static_cast<unsigned char>(v))
    {
    }

  private:

    // Cast to a T
    template <typename T>
    operator T() const
    {
      return static_cast<T>(value);
    }

    // The byte value
    unsigned char value;
  };

  //*************************************************************************
  /// Equality test
  //*************************************************************************
  bool operator ==(etl::byte lhs, etl::byte rhs)
  {
    return (lhs.value == rhs.value);
  }

  //*************************************************************************
  /// Inequality test
  //*************************************************************************
  bool operator !=(etl::byte lhs, etl::byte rhs)
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// To integer.
  //*************************************************************************
  template <typename TInteger> 
  typename etl::enable_if<etl::is_integral<TInteger>::value, TInteger>::type
  to_integer(etl::byte b)
  {
    return TInteger(b);
  }

  //*************************************************************************
  /// Shift left.
  //*************************************************************************
  template <typename TInteger>
  typename etl::enable_if<etl::is_integral<TInteger>::value, etl::byte>::type
  operator <<(etl::byte b, TInteger shift)
  {
    return  etl::byte(to_integer<unsigned int>(b) << shift);
  }

  //*************************************************************************
  /// Shift right
  //*************************************************************************
  template <typename TInteger>
  typename etl::enable_if<etl::is_integral<TInteger>::value, etl::byte>::type
  operator >>(etl::byte b, TInteger shift)
  {
    return  etl::byte(to_integer<unsigned int>(b) >> shift);
  }

  //*************************************************************************
  /// Shift left equals.
  //*************************************************************************
  template <typename TInteger>  
  typename etl::enable_if<etl::is_integral<TInteger>::value, etl::byte&>::type
  operator <<=(etl::byte& b, TInteger shift)
  {
    b = b << shift;

    return b;
  }

  //*************************************************************************
  /// Shift right equals.
  //*************************************************************************
  template <typename TInteger>
  typename etl::enable_if<etl::is_integral<TInteger>::value, etl::byte&>::type
  operator >>=(etl::byte& b, TInteger shift)
  {
    b = b >> shift;

    return b;
  }

  //*************************************************************************
  /// Or.
  //*************************************************************************
  etl::byte operator |(etl::byte lhs, etl::byte rhs)
  {
    return etl::byte(to_integer<unsigned int>(lhs) | to_integer<unsigned int>(rhs));
  }

  //*************************************************************************
  /// And.
  //*************************************************************************
  etl::byte operator &(etl::byte lhs, etl::byte rhs)
  {
    return etl::byte(to_integer<unsigned int>(lhs) & to_integer<unsigned int>(rhs));
  }

  //*************************************************************************
  /// Exclusive Or.
  //*************************************************************************
   etl::byte operator ^(etl::byte lhs, etl::byte rhs)
  {
   return etl::byte(to_integer<unsigned int>(lhs) ^ to_integer<unsigned int>(rhs));
  }

  //*************************************************************************
  /// Or equals.
  //*************************************************************************
  etl::byte& operator |=(etl::byte& lhs, etl::byte rhs)
  {
    return lhs = lhs | rhs;
  }

  //*************************************************************************
  /// And equals
  //*************************************************************************
  etl::byte& operator &=(etl::byte& lhs, etl::byte rhs)
  {
    return lhs = lhs & rhs;
  }

  //*************************************************************************
  /// Exclusive or equals.
  //*************************************************************************
  etl::byte& operator ^=(etl::byte& lhs, etl::byte rhs)
  {
    return lhs = lhs ^ rhs;
  }

  //*************************************************************************
  /// Not.
  //*************************************************************************
   etl::byte operator ~(etl::byte b)
  {
    return etl::byte(~to_integer<unsigned char>(b));
  }

#endif


}

#endif