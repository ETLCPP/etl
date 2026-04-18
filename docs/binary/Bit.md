---
title: "Bit"
---

{{< callout type="info">}}
  Header: `bit.h`  
  Since: `TBC`  
  Similar to: [STL bit header](https://en.cppreference.com/cpp/header/bit)
{{< /callout >}}

Utility functions for manipulating binary numbers.  
A reverse engineered version of C++20's and C++23's `<bit>` header.

## bit_cast
```cpp
template <typename TDestination, typename TSource>>
ETL_CONSTEXPR14 TDestination bit_cast(const TSource& source) ETL_NOEXCEPT
```
**Description**  
Returns a value of type `TDestination` by reinterpreting the `TSource` object.

## byteswap
```cpp
template <typename T>
ETL_CONSTEXPR14 T byteswap(T n) ETL_NOEXCEPT
```
**Description**  
Reverses the bytes in `n`.

## has_single_bit
```cpp
template <typename T>
ETL_CONSTEXPR14 bool has_single_bit(T n) ETL_NOEXCEPT
```
**Description**  
Checks if `n` is a power of two, or has one bit set.

## bit_ceil
```cpp
template <typename T>
ETL_CONSTEXPR14 T bit_ceil(T n);
```
**Description**  
Calculates the smallest power of two, that is not smaller than `n`.

## bit_floor
```cpp
template <typename T>
ETL_CONSTEXPR14 T bit_floor(T n) ETL_NOEXCEPT
```
**Description**  
Calculates the smallest power of two, that is not greater than `n`.

## bit_width
```cpp
template <typename T>
ETL_CONSTEXPR14 T bit_width(T n) ETL_NOEXCEPT
```
**Description**  
If `n` is not `0`, calculates the number of bits needed to store it.  
If `n` is `0`, then the result is `0`.

## rotl
```cpp
template <typename T>
ETL_NODISCARD ETL_CONSTEXPR14 T rotl(T value, int n) ETL_NOEXCEPT
```
**Description**  
Computes a left circular shift of value by `n`.

## rotr
```cpp
template <typename T>
ETL_NODISCARD ETL_CONSTEXPR14 T rotr(T value, int n) ETL_NOEXCEPT
```
**Description**  
Computes a right circular shift of value by `n`.

## countl_zero
```cpp
template <typename T>
ETL_NODISCARD ETL_CONSTEXPR14 int countl_zero(T value) ETL_NOEXCEPT
```
**Description**  
Returns the number of consecutive `0` bits in `n`, starting from the most significant bit (left).

## countl_one
```cpp
template <typename T>
ETL_NODISCARD ETL_CONSTEXPR14 int countl_one(T value) ETL_NOEXCEPT
```
**Description**  
Returns the number of consecutive `1` bits in `n`, starting from the most significant bit (left).

## countr_zero
```cpp
template <typename T>
ETL_NODISCARD ETL_CONSTEXPR14 int countr_zero(T value) ETL_NOEXCEPT
```
**Description**  
Returns the number of consecutive `0` bits in `n`, starting from the least significant bit (right).

## countr_one
```cpp
template <typename T>
ETL_NODISCARD ETL_CONSTEXPR14 int countr_one(T value) ETL_NOEXCEPT
```
**Description**  
Returns the number of consecutive `1` bits in `n`, starting from the least significant bit (right).

## popcount
```cpp
template <typename T>
ETL_NODISCARD ETL_CONSTEXPR14 int popcount(T value) ETL_NOEXCEPT
```
**Description**  
Counts the number of `1` bits in an unsigned integer.
