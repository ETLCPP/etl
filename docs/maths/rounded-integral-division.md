---
title: "Rounded integral division"
---

{{< callout type="info">}}
  Header: `rounded_integral_division.h`  
  Since: `20.9.0`
{{< /callout >}}

Rounded division algorithms for integral values.  

These algorithms perform integer division while rounding the result as though the fractional part were actually present. They do not check for a denominator of zero.

## divide_round_to_ceiling
Values are rounded to the next more positive integral value.

```cpp
template <typename T1, typename T2>
ETL_CONSTEXPR14
etl::common_type_t<T1, T2>
divide_round_to_ceiling(T1 numerator, T2 denominator) ETL_NOEXCEPT
```
```cpp
.151 / 100 ==  2
.150 / 100 ==  2
.149 / 100 ==  2
.51 / 100 ==  1
..50 / 100 ==  1
..49 / 100 ==  1
.-49 / 100 ==  0
.-50 / 100 ==  0
.-51 / 100 ==  0
-149 / 100 == -1
-150 / 100 == -1
-151 / 100 == -1
```

## divide_round_to_floor
Values are rounded to the next more negative integral value.

```cpp
template <typename T1, typename T2>
ETL_CONSTEXPR14
etl::common_type_t<T1, T2>
divide_round_to_floor(T1 numerator, T2 denominator) ETL_NOEXCEPT
```
```cpp
.151 / 100 ==  1
.150 / 100 ==  1
.149 / 100 ==  1
..51 / 100 ==  0
..50 / 100 ==  0
..49 / 100 ==  0
.-49 / 100 == -1
.-50 / 100 == -1
.-51 / 100 == -1
-149 / 100 == -2
-150 / 100 == -2
-151 / 100 == -2
```

## divide_round_to_zero
Values are rounded towards zero.

```cpp
template <typename T1, typename T2>
ETL_CONSTEXPR14
etl::common_type_t<T1, T2>
divide_round_to_zero(T1 numerator, T2 denominator) ETL_NOEXCEPT
```
```cpp
.151 / 100 ==  1
.150 / 100 ==  1
.149 / 100 ==  1
..51 / 100 ==  0
..50 / 100 ==  0
..49 / 100 ==  0
.-49 / 100 ==  0
.-50 / 100 ==  0
.-51 / 100 ==  0
-149 / 100 == -1
-150 / 100 == -1
-151 / 100 == -1
```

## divide_round_to_infinity
Values are rounded towards infinity.

```cpp
template <typename T1, typename T2>
ETL_CONSTEXPR14
etl::common_type_t<T1, T2>
divide_round_to_infinity(T1 numerator, T2 denominator) ETL_NOEXCEPT
```
```cpp
.151 / 100 ==  2
.150 / 100 ==  2
.149 / 100 ==  2
..51 / 100 ==  1
..50 / 100 ==  1
..49 / 100 ==  1
.-49 / 100 == -1
.-50 / 100 == -1
.-51 / 100 == -1
-149 / 100 == -2
-150 / 100 == -2
-151 / 100 == -2
```

## divide_round_half_up
Values are rounded to the nearest integral value.  
'Half' values are rounded up (towards infinity).

```cpp
template <typename T1, typename T2>
ETL_CONSTEXPR14
etl::common_type_t<T1, T2>
divide_round_half_up(T1 numerator, T2 denominator) ETL_NOEXCEPT
```
```cpp
.151 / 100 ==  3
.150 / 100 ==  3
.149 / 100 ==  1
..51 / 100 ==  1
..50 / 100 ==  1
..49 / 100 ==  0
.-49 / 100 ==  0
.-50 / 100 == -1
.-51 / 100 == -1
-149 / 100 == -1
-150 / 100 == -3
-151 / 100 == -3
```

## divide_round_half_down
Values are rounded to the nearest integral value.  
'Half' values are rounded up (towards zero)

```cpp
template <typename T1, typename T2>
ETL_CONSTEXPR14
etl::common_type_t<T1, T2>
divide_round_half_down(T1 numerator, T2 denominator) ETL_NOEXCEPT
```cpp
.151 / 100 ==  3
.150 / 100 ==  2
.149 / 100 ==  1
..51 / 100 ==  1
..50 / 100 ==  0
..49 / 100 ==  0
.-49 / 100 ==  0
.-50 / 100 ==  0
.-51 / 100 == -1
-149 / 100 == -1
-150 / 100 == -2
-151 / 100 == -3
```

## divide_round_half_even
(Banker's rounding)  
Values are rounded to the nearest integral value.  
'Half' values are rounded to the nearest even value.
```cpp
template <typename T1, typename T2>
ETL_CONSTEXPR14
etl::common_type_t<T1, T2>
divide_round_half_even(T1 numerator, T2 denominator) ETL_NOEXCEPT
```
```cpp
.151 / 100 ==  2
.150 / 100 ==  2
.149 / 100 ==  1
..51 / 100 ==  1
..50 / 100 ==  0
..49 / 100 ==  0
.-49 / 100 ==  0
.-50 / 100 ==  0
.-51 / 100 == -1
-149 / 100 == -1
-150 / 100 == -2
-151 / 100 == -2
```

## divide_round_half_odd
Values are rounded to the nearest integral value.  
'Half' values are rounded to the nearest odd value.

```cpp
template <typename T1, typename T2>
ETL_CONSTEXPR14
etl::common_type_t<T1, T2>
divide_round_half_even(T1 numerator, T2 denominator) ETL_NOEXCEPT
```
```cpp
.151 / 100 ==  2
.150 / 100 ==  1
.149 / 100 ==  1
..51 / 100 ==  1
..50 / 100 ==  1
..49 / 100 ==  0
.-49 / 100 ==  0
.-50 / 100 == -1
.-51 / 100 == -1
-149 / 100 == -1
-150 / 100 == -1
-151 / 100 == -2
```
