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

Defined in the `etl::clib` namespace.

---

```cpp
ETL_NODISCARD
float floor(float value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::floorf`.  

**Returns**  
`::floorf(value)`

---

```cpp
ETL_NODISCARD
double floor(double value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::floor`.  

**Returns**  
`::floor(value)`

---

```cpp
ETL_NODISCARD
long double floor(long double value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::floorl`.  

**Returns**  
If `ETL_NO_LONG_DOUBLE_MATH` is defined then returns  
`::floor` with casts to `double`.  
otherwise  
`::floorl(value)`  

## ceil

Defined in the `etl::clib` namespace.

---

```cpp
ETL_NODISCARD
float ceil(float value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::ceilf`.  

**Returns**  
`::ceilf(value)`

---

```cpp
ETL_NODISCARD
double ceil(double value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::ceil`.  

**Returns**  
`::ceil(value)`

---

```cpp
ETL_NODISCARD
long double ceil(long double value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::ceill`.  

**Returns**  
If `ETL_NO_LONG_DOUBLE_MATH` is defined then returns  
`::ceil` with casts to `double`.  
otherwise  
`::ceill(value)`  

## log10

Defined in the `etl::clib` namespace.

---

```cpp
ETL_NODISCARD
float log10(float value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::log10f`.  

**Returns**  
`::log10f(value)`

---

```cpp
ETL_NODISCARD
double log10(double value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::log10`.  

**Returns**  
`::log10(value)`

---

```cpp
ETL_NODISCARD
long double log10(long double value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::log10l`.  

**Returns**  
If `ETL_NO_LONG_DOUBLE_MATH` is defined then returns  
`::log10` with casts to `double`.   
otherwise  
`::log10l(value)`  

## round

Defined in the `etl::clib` namespace.

---

```cpp
ETL_NODISCARD
float round(float value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::roundf`.  

**Returns**  
`::roundf(value)`

---

```cpp
ETL_NODISCARD
double round(double value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::round`.  

**Returns**  
`::round(value)`

---

```cpp
ETL_NODISCARD
long double round(long double value) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::roundl`.  

**Returns**  
If `ETL_NO_LONG_DOUBLE_MATH` is defined then returns  
`::round` with casts to `double`.  
otherwise  
`::roundl(value)`  

## pow

Defined in the `etl::clib` namespace.

---

```cpp
ETL_NODISCARD
float pow(float base, float exponent) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::powf`.  

**Returns**  
`::powf(base, exponent)`

---

```cpp
ETL_NODISCARD
double pow(double base, double exponent) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::pow`.  

**Returns**  
`::pow(base, exponent)`

---

```cpp
ETL_NODISCARD
long double pow(long double base, long double exponent) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::powl`.  

**Returns**  
If `ETL_NO_LONG_DOUBLE_MATH` is defined then returns  
`::pow` with casts to `double`.  
otherwise  
`::powl(base, intpart)`  

## modf

Defined in the `etl::clib` namespace.

---

```cpp
ETL_NODISCARD
float modf(float value, float* intpart) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::modff`.  

**Returns**  
`::modff(value, intpart)`

---

```cpp
ETL_NODISCARD
double modf(double value, double* intpart) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::modf`.  

**Returns**  
`::modf(value, intpart)`

---

```cpp
ETL_NODISCARD
long double modf(long double value, long double* intpart) ETL_NOEXCEPT
```
**Description**  
A wrapper around `::modfl`.  

**Returns**  
If `ETL_NO_LONG_DOUBLE_MATH` is defined then returns  
`::modf` with casts to `double`.  
otherwise  
`::modfl(value, intpart)`  
