---
title: "month_day"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::month_day](https://en.cppreference.com/w/cpp/chrono/month_day.html)
{{< /callout >}}

A class representing a month and day.

```cpp
class month_day
```

## Construction

```cpp
ETL_CONSTEXPR 
month_day()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
month_day(const etl::chrono::month& m, 
          const etl::chrono::day&   d) 
ETL_NOEXCEPT
```
**Description**  
Construct from `month` and `day`.

## Access

```cpp
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
etl::chrono::day day() const 
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
`true` if the month and day is within the valid limits.

---

```cpp
ETL_CONSTEXPR14 
int compare(const month_day& other) const 
ETL_NOEXCEPT 
```
**Description**  
Compare `month_day` with another.  
if `month < other.month`, returns `-1`  
else if `month > other.month`, returns `1`  
else if `day < other.day`, returns `-1`  
else if `day > other.day`, returns `1`  
else returns `0`

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::month_day& lhs, 
                 const etl::chrono::month_day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::month_day& lhs, 
                 const etl::chrono::month_day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month_day& lhs, 
                const etl::chrono::month_day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Less-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month_day& lhs, 
                 const etl::chrono::month_day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Less-than-equal operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month_day& lhs, 
                const etl::chrono::month_day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Greater-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month_day& lhs, 
                 const etl::chrono::month_day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Greater-than-equal operator.

---

```cpp
[[nodiscard]] constexpr
auto operator <=>(const etl::chrono::month_day& lhs, 
                  const etl::chrono::month_day& rhs)
ETL_NOEXCEPT
```
**Description**  
Spaceship operator.  
C++20

## Hash

```cpp
template <typename TRep, typename TPeriod>
struct hash<etl::chrono::month_day>
```
**Description**  
Hash function for `etl::chrono::month_day`.

