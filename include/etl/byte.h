

#ifndef ETL_BYTE_INCLUDED
#define ETL_BYTE_INCLUDED

#include "platform.h"
#include "type_traits.h"

#if ETL_CPP11_SUPPORTED

namespace etl
{
  enum class byte : unsigned char {};

  ///************************************************************************
  /// To intger.
  ///************************************************************************
  template <typename TInteger>
  constexpr 
  etl::enable_if_t<etl::is_integral<TInteger>::value, TInteger>
    to_integer(etl::byte b) noexcept
  {
      return TInteger(b);
  }

  ///************************************************************************
  /// Shift left.
  ///************************************************************************
  template <typename TInteger>
  constexpr
    etl::enable_if_t<etl::is_integral<TInteger>::value, etl::byte>
    operator <<(etl::byte b, TInteger shift) noexcept
  {
    return  etl::byte(static_cast<unsigned int>(b) << shift);
  }

  ///************************************************************************
  /// Shift right
  ///************************************************************************
  template <typename TInteger>
  constexpr
    etl::enable_if_t<etl::is_integral<TInteger>::value, etl::byte>
    operator >>(etl::byte b, TInteger shift) noexcept
  {
    return  etl::byte(static_cast<unsigned int>(b) >> shift);
  }

  ///************************************************************************
  /// Shift left equals.
  ///************************************************************************
  template <typename TInteger>
  constexpr 
  etl::enable_if_t<etl::is_integral<TInteger>::value, etl::byte&> 
    operator <<=(etl::byte& b, TInteger shift) noexcept
  {
    b = b << shift;

    return b;
  }

  ///************************************************************************
  /// Shift right equals.
  ///************************************************************************
  template <typename TInteger>
  constexpr 
  etl::enable_if_t<etl::is_integral<TInteger>::value, etl::byte&> 
    operator >>=(etl::byte& b, TInteger shift) noexcept
  {
    b = b >> shift;

    return b;
  }

  ///************************************************************************
  /// Or.
  ///************************************************************************
  constexpr etl::byte operator |(etl::byte lhs, etl::byte rhs) noexcept
  {
    return etl::byte(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
  }

  ///************************************************************************
  /// And.
  ///************************************************************************
  constexpr etl::byte operator &(etl::byte lhs, etl::byte rhs) noexcept
  {
    return etl::byte(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
  }

  ///************************************************************************
  /// Exclusive Or.
  ///************************************************************************
  constexpr etl::byte operator ^(etl::byte lhs, etl::byte rhs) noexcept
  {
    return etl::byte(static_cast<unsigned int>(lhs) ^ static_cast<unsigned int>(rhs));
  }

  ///************************************************************************
  /// Or equals.
  ///************************************************************************
  ETL_CONSTEXPR14 etl::byte& operator |=(etl::byte& lhs, etl::byte rhs) noexcept
  {
    return lhs = lhs | rhs;
  }

  ///************************************************************************
  /// And equals
  ///************************************************************************
  ETL_CONSTEXPR14 etl::byte& operator &=(etl::byte& lhs, etl::byte rhs) noexcept
  {
    return lhs = lhs & rhs;
  }

  ///************************************************************************
  /// Exclusive or equals.
  ///************************************************************************
  ETL_CONSTEXPR14 etl::byte& operator ^=(etl::byte& lhs, etl::byte rhs) noexcept
  {
    return lhs = lhs ^ rhs;
  }

  ///************************************************************************
  /// Not.
  ///************************************************************************
  constexpr etl::byte operator ~(etl::byte b) noexcept
  {
    return etl::byte(~static_cast<unsigned int>(b));
  }
}

#endif
#endif