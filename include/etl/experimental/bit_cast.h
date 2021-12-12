#pragma once

template <typename To, typename From>
etl::enable_if_t<sizeof(To) == sizeof(From) && etl::is_trivially_copyable_v<From> && etl::is_trivially_copyable_v<To>, To>
//ETL_CONSTEXPR
bit_cast(const From& src) ETL_NOEXCEPT
{
  To dst;
  memcpy(&dst, &src, sizeof(To));

  // __builtin_memcpy(&dst, &src, sizeof(To));

  return dst;
}

std::midpoint
// https://en.cppreference.com/w/cpp/numeric/midpoint

std::lerp
// https://en.cppreference.com/w/cpp/numeric/lerp