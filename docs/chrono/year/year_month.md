---
title: "year_month"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::year_month](https://en.cppreference.com/w/cpp/chrono/year_month.html)
{{< /callout >}}

A class representing a year and month.

```cpp
class year_month
```

## Construction
```cpp
ETL_CONSTEXPR 
year_month()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
year_month(const etl::chrono::year&  y, 
           const etl::chrono::month& m) 
ETL_NOEXCEPT
```
**Description**  
Construct from month and day.

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
int compare(const etl::chrono::year_month& other) const 
ETL_NOEXCEPT
```
**Description**  
If `y < other.y` return `-1`
If `y > other.y` return `1`
If `m < other.m` return `-1`
If `m > other.m` return `1`
Else return `0`

## Non-member mathematical operators

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month operator +(const etl::chrono::year_month& ym,
                                   const etl::chrono::years&      dy)
```
**Description**  
Adds `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month operator +(const etl::chrono::years&      dy,
                                   const etl::chrono::year_month& ym) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month operator +(const etl::chrono::year_month& ym,
                                   const etl::chrono::months&     dm) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month operator +(const etl::chrono::months& dm,
                                   const etl::chrono::year_month& ym) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month operator -(const etl::chrono::year_month& ym,
                                   const etl::chrono::years&      dy) 
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month operator -(const etl::chrono::year_month& ym,
                                   const etl::chrono::months&     dm) 
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::months operator -(const etl::chrono::year_month& ym1,
                               const etl::chrono::year_month& ym2) 
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::year_month`.

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::year_month& lhs, 
                 const etl::chrono::year_month& rhs) 
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14
bool operator !=(const etl::chrono::year_month& lhs, 
                 const etl::chrono::year_month& rhs) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
[[nodiscard]] friend constexpr
auto operator <=>(const etl::chrono::year_month& lhs, 
                  const etl::chrono::year_month& rhs)
ETL_NOEXCEPT
```
**Description**  
Spaceship operator  
C++20

---

```
ETL_CONSTEXPR14 
int compare(const year_month& other) const 
ETL_NOEXCEPT
```
**Description**  
Compare year_month with another.  
if `month < other.month`, returns `-1`  
else if `month > other.month`, returns `1`  
else if `day < other.day`, returns `-1`  
else if `day > other.day`, returns `1`  
else returns `0`;

## Hash

template <typename TRep, typename TPeriod>
struct hash<etl::chrono::year_month>
**Description**  
Hash function for `etl::chrono::year_month`.
