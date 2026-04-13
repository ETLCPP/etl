---
title: "month_day_last"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::month_day_last](https://en.cppreference.com/w/cpp/chrono/month_day_last.html)
{{< /callout >}}

A class representing a month and last day.

```cpp
class month_day_last
```

## Construction

```cpp
ETL_CONSTEXPR14 
explicit month_day_last(const etl::chrono::month& m) 
ETL_NOEXCEPT
```
**Description**  
Construct from month.

## Access

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
`true` if the month is within the valid limits.

---

```cpp
ETL_CONSTEXPR14 
int compare(const month_day_last& other) const 
ETL_NOEXCEPT 
```
**Description**  
Compare `month_day_last` with another.  
if `month < other.month`, returns `-1`  
else if `month > other.month`, returns `1`  
else returns `0`

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::month_day_last& lhs, 
                 const etl::chrono::month_day_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::month_day_last& lhs, 
                 const etl::chrono::month_day_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month_day_last& lhs, 
                const etl::chrono::month_day_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Less-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month_day_last& lhs, 
                 const etl::chrono::month_day_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Less-than-equal operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month_day_last& lhs, 
                const etl::chrono::month_day_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Greater-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month_day_last& lhs, 
                 const etl::chrono::month_day_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Greater-than-equal operator.

---

```cpp
[[nodiscard]] constexpr
auto operator <=>(const etl::chrono::month_day_last& lhs, 
                  const etl::chrono::month_day_last& rhs)
ETL_NOEXCEPT
```
**Description**  
Spaceship operator.  
C++20

## Hash

```cpp
template <typename TRep, typename TPeriod>
struct hash<etl::chrono::month_day_last>
```
**Description**  
Hash function for `etl::chrono::month_day_last`.
