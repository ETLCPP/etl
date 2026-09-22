---
title: "math"
---

{{< callout type="info">}}
  Header: `math.h`  
  From: `20.50.0`  
{{< /callout >}}

Various mathematical utilities

## is_nan

```cpp
template <typename T>
ETL_CONSTEXPR bool is_nan(T value)
```
**Returns**  
`true` if `value` is `NAN` (Not A Number).  
Always returns `false` if `T` is an integral.

## is_infinity

```cpp
template <typename T>
ETL_CONSTEXPR bool is_infinity(T value)
```
**Returns**  
`true` if `value` is infinity.  
Always returns `false` if `T` is an integral.

## is_zero

```cpp
template <typename T>
ETL_CONSTEXPR bool is_zero(T value)
```
**Returns**  
`true` if `value` is Zero.  

## is_exactly_zero

```cpp
template <typename T>
ETL_CONSTEXPR bool is_exactly_equal(T value1, T value2)
```
**Returns**  
`true` if `value1` is equal to `value2`.  

## floor

```cpp
ETL_NODISCARD
float floor(float value) ETL_NOEXCEPT
```
**Returns**  
`::floorf(value)`

---

```cpp
ETL_NODISCARD
double floor(double value) ETL_NOEXCEPT
```
**Returns**  
`::floor(value)`

---

```cpp
ETL_NODISCARD
long double floor(long double value) ETL_NOEXCEPT
```
**Returns**  
`::floorl(value)`

## ceil

```cpp
ETL_NODISCARD
float ceil(float value) ETL_NOEXCEPT
```
**Returns**  
`::ceilf(value)`

---

```cpp
ETL_NODISCARD
double ceil(double value) ETL_NOEXCEPT
```
**Returns**  
`::ceil(value)`

---

```cpp
ETL_NODISCARD
long double ceil(long double value) ETL_NOEXCEPT
```
**Returns**  
`::ceill(value)`
