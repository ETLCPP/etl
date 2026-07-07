---
title: "hh_mm_ss"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  From: `20.41.0`  
  Similar to: [std::chrono::hh_mm_ss](https://en.cppreference.com/w/cpp/chrono/hh_mm_ss.html)
{{< /callout >}}

A type to hold hours, minutes, and seconds.

```cpp
template <typename TDuration>
class hh_mm_ss
```
**Description**
Static asserts if `TDuration` is not a specialisation of `etl::chrono::duration`.


## Constants
```cpp
static constexpr int fractional_width;
```
**Description**  
The number of fractional digits.

## Member types
```cpp
precision
```
**Description**  
The return type for `to_duration`.

## Constructors
```cpp
ETL_CONSTEXPR 
hh_mm_ss() 
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14
explicit hh_mm_ss(TDuration d) 
ETL_NOEXCEPT
```
**Description**  
Construct from `duration`.

## Access
```pp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool is_negative() const 
ETL_NOEXCEPT
```
**Description**  
Checks for negative duration.

---

```cpp
ETL_NODISCARD 
ETL_CONSTEXPR14 
etl::chrono::hours hours() const 
ETL_NOEXCEPT
```
**Return**  
The hours.

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::minutes minutes() const 
ETL_NOEXCEPT
```
**Return**  
The minutes.

---

```cpp
ETL_NODISCARD 
ETL_CONSTEXPR14
etl::chrono::seconds seconds() const 
ETL_NOEXCEPT
```
**Return**  
The seconds.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
precision subseconds() const 
ETL_NOEXCEPT
```
**Return**  
The subseconds.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
explicit operator precision() const 
ETL_NOEXCEPT
```
**Return**  
The duration.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
precision to_duration() const 
ETL_NOEXCEPT
```
**Return***  
The duration.

## absolute
```cpp
template <typename TDuration>
ETL_NODISCARD
ETL_CONSTEXPR14 
TDuration absolute(TDuration dur) 
ETL_NOEXCEPT
```
A specialisation of absolute for `etl::chrono::duration`.
**Return**  
The absolute duration value.

## 12/24 hour functions
```cpp
ETL_NODISCARD
inline ETL_CONSTEXPR14 
bool is_am(const etl::chrono::hours& h) 
ETL_NOEXCEPT
```
**Description**  
Returns `true` if `h` is in the range [0h, 11h] (an am hour).

---

```cpp
ETL_NODISCARD
inline ETL_CONSTEXPR14 
bool is_pm(const etl::chrono::hours& h) 
ETL_NOEXCEPT
```
**Description**  
Returns `true` if `h` is in the range [12h, 23h] (a pm hour).

---

```cpp
ETL_NODISCARD
inline ETL_CONSTEXPR14 
etl::chrono::hours make12(const etl::chrono::hours& h) 
ETL_NOEXCEPT
```
**Description**  
Returns the 12-hour equivalent of `h` in the range [1h, 12h].  
If `h` is not in the range [0h, 23h], the value returned is unspecified.

---

```cpp
ETL_NODISCARD
inline ETL_CONSTEXPR14 
etl::chrono::hours make24(const etl::chrono::hours& h, bool is_pm) 
ETL_NOEXCEPT
```
**Description**  
Returns the 24-hour equivalent of `h`, which is assumed to represent the hour with the indicated am/pm.  
If `h` is not in the range [1h, 12h], the value returned is unspecified.