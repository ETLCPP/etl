---
title: "year_month_day_last"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::year_month_day_last](https://en.cppreference.com/w/cpp/chrono/year_month_day_last.html)
{{< /callout >}}

A class representing a year, month and last day.

```cpp
class year_month_day_last
```

## Construction

```cpp
ETL_CONSTEXPR year_month_day_last()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
year_month_day(const etl::chrono::year&  y, 
               const etl::chrono::month_day_last& mdl) 
ETL_NOEXCEPT
```
**Description**  
Construct from month and day.

---

```cpp
ETL_CONSTEXPR14 
year_month_day(const etl::chrono::year_month_day_last& ymdl) 
ETL_NOEXCEPT
```
**Description**  
Construct from `year_month_day_last`.

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

```coo
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::month month() const 
ETL_NOEXCEPT
```
**Return**  
The month.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::month_day_last month_day_last() const 
ETL_NOEXCEPT
```
**Returns**  
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
int compare(const etl::chrono::year_month_day_last& other) const 
ETL_NOEXCEPT
```
**Description**  
Compare `year_month_day` with another.
if `year < other.year`, returns `-1`
else if `year > other.year`, returns `1`
if `month < other.month`, returns `-1`
else if `month > other.month`, returns `1`
else returns `0`

## operators

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last& operator +=(const etl::chrono::years& dy) 
ETL_NOEXCEPT
```
Adds `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last& operator +=(const etl::chrono::months& dm) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month_day_last& operator -=(const etl::chrono::years& dy)
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last& operator -=(const etl::chrono::months& dm)
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::months`.

## Non-member operators

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month_day_last operator +(const etl::chrono::year_month_day_last& ym,
                                            const etl::chrono::years&               dy)
```
**Description**  
Adds `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month_day_last operator +(const etl::chrono::years&               dy,
                                            const etl::chrono::year_month_day_last& ym) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year_month_day_last operator +(const etl::chrono::year_month_day_last& ym,
                                            const etl::chrono::months&              dm) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last operator +(const etl::chrono::months&              dm,
                                            const etl::chrono::year_month_day_last& ym) 
ETL_NOEXCEPT
```
**Description**  
Adds `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_last operator -(const etl::chrono::year_month_day_last& ym,
                                        const etl::chrono::years&               dy) 
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year_month_day_last operator -(const etl::chrono::year_month_day& ym,
                                   const etl::chrono::months&     dm) 
ETL_NOEXCEPT
```
**Description**  
Subtracts `etl::chrono::months`.

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::year_month_day_last& lhs, 
                 const etl::chrono::year_month_day_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::year_month_day_last& lhs, 
                 const etl::chrono::year_month_day_last& rhs) 
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
Spaceship operator.  
C++20

## Hash

```cpp
template <>
struct hash<etl::chrono::year_month_day_last>
```
**Description**  
Hash function for `etl::chrono::year_month_day_last`.
