---
title: "year_month_day"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::year_month_day](https://en.cppreference.com/w/cpp/chrono/year_month_day.html)
{{< /callout >}}

A template representing a year, month, and day.

```cpp
class year_month_day
```

## Construction

```cpp
ETL_CONSTEXPR year_month_day()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
year_month_day(const etl::chrono::year&  y, 
               const etl::chrono::month& m, 
               const etl::chrono::day&   d) 
ETL_NOEXCEPT
```
**Description**  
Construct from `month` and `day`.

---

```cpp
ETL_CONSTEXPR14 
year_month_day(const etl::chrono::year_month_day_last& ymdl) 
ETL_NOEXCEPT
```
**Description**  
Construct from `year_month_day_last`.

---

```cpp
ETL_CONSTEXPR14 
year_month_day(const etl::chrono::sys_days& sd) 
ETL_NOEXCEPT
```
**Description**  
Construct from `sys_days`.

---

```cpp
ETL_CONSTEXPR14
year_month_day(const etl::chrono::local_days& ld) 
ETL_NOEXCEPT
```
**Description**  
Construct from `local_days`.

## Access

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::year year() const 
ETL_NOEXCEPT
```
**Return**  
The year.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::month month() const 
ETL_NOEXCEPT
```
**Return**  
The month.

---

```
ETL_NODISCARD
ETL_CONSTEXPR14 etl::chrono::day day() const
ETL_NOEXCEPT
```
**Return**  
The day.

## Tests

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool ok() const 
ETL_NOEXCEPT
```
**Return**  
`true` if the year and month is within the valid limits.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
int compare(const etl::chrono::year_month_day& other) const 
ETL_NOEXCEPT
```
**Description**  
Compare `year_month_day` with another.  
if `year < other.year`, returns `-1`  
else if `year > other.year`, returns `1`  
if `month < other.month`, returns `-1`  
else if `month > other.month`, returns `1`  
else if `day < other.day`, returns `-1`  
else if `day > other.day`, returns `1`  
else returns `0`

## operators

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day& operator +=(const etl::chrono::years& dy) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day& operator +=(const etl::chrono::months& dm) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month_day& operator -=(const etl::chrono::years& dy)
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::years`.

---

```
ETL_CONSTEXPR14 
etl::chrono::year_month_day& operator -=(const etl::chrono::months& dm)
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::months`.

## Non-member operators

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month_day operator +(const etl::chrono::year_month_day& ym,
                                       const etl::chrono::years&          dy)
```
**Description**  
Adds `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month_day operator +(const etl::chrono::years&          dy,
                                       const etl::chrono::year_month_day& ym) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month_day operator +(const etl::chrono::year_month_day& ym,
                                       const etl::chrono::months&         dm) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::months`.

---

```
ETL_CONSTEXPR14 
etl::chrono::year_month operator +(const etl::chrono::months&         dm,
                                   const etl::chrono::year_month_day& ym) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month operator -(const etl::chrono::year_month_day& ym,
                                   const etl::chrono::years&          dy) 
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month operator -(const etl::chrono::year_month_day& ym,
                                   const etl::chrono::months&         dm) 
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::months`.

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::year_month_day& lhs, 
                 const etl::chrono::year_month_day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::year_month_day& lhs, 
                 const etl::chrono::year_month_day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
[[nodiscard]] friend constexpr
auto operator <=>(const etl::chrono::year_month_day& lhs, 
                  const etl::chrono::year_month_day& rhs)
ETL_NOEXCEPT
```
**Description**  
Spaceship operator  
C++20

## Hash

```cpp
template <>
struct hash<etl::chrono::year_month_day>
```
**Description**  
Hash function for `etl::chrono::year_month_day`.


  
