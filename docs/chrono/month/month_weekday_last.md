---
title: "month_weekday_last"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::month_weekday_last](https://en.cppreference.com/w/cpp/chrono/month_weekday_last.html)
{{< /callout >}}

A class representing a month and weekday_last.

```cpp
class month_weekday_last
```

## Construction

```cpp
ETL_CONSTEXPR 
month_weekday_last()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
month_weekday_last(const etl::chrono::month&   m, 
                   const etl::chrono::weekday_last& wd) 
ETL_NOEXCEPT
```
**Description**  
Construct from `month` and `weekday_last`.

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
etl::chrono::weekday_last weekday_last() const 
ETL_NOEXCEPT
```
**Return**  
The weekday.

## Tests

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool ok() const 
ETL_NOEXCEPT
```
**Return**  
`true` if the month and day is within the valid limits.

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::month_weekday_last& lhs, 
                 const etl::chrono::month_weekday_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::month_weekday_last& lhs, 
                 const etl::chrono::month_weekday_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month_weekday_last& lhs, 
                const etl::chrono::month_weekday_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Less-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month_weekday_last& lhs, 
                 const etl::chrono::month_weekday_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Less-than-equal operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month_weekday_last& lhs, 
                const etl::chrono::month_weekday_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Greater-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month_weekday_last& lhs, 
                 const etl::chrono::month_weekday_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Greater-than-equal operator.

## Hash

```cpp
template <typename TRep, typename TPeriod>
struct hash<etl::chrono::month_weekday_last>
```
**Description**  
Hash function for `etl::chrono::month_weekday_last`.
