---
title: "month_weekday"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::month_weekday](https://en.cppreference.com/w/cpp/chrono/month_weekday.html)
{{< /callout >}}

A class representing a month and weekday.

```cpp
class month_weekday
```

## Construction

```cpp
ETL_CONSTEXPR 
month_weekday()
```
**Decription**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
month_weekday(const etl::chrono::month&   m, 
              const etl::chrono::weekday_indexed& wd) 
ETL_NOEXCEPT
```
**Decription**  
Construct from month and weekday_indexed.

## Access

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::month month() const 
ETL_NOEXCEPT
```
**Returns**  
The month.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::weekday_indexed weekday_indexed() const 
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
bool operator ==(const etl::chrono::month_weekday& lhs, 
                 const etl::chrono::month_weekday& rhs) 
ETL_NOEXCEPT
```
**Decription**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::month_weekday& lhs, 
                 const etl::chrono::month_weekday& rhs) 
ETL_NOEXCEPT
```
**Decription**  
Inequality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month_weekday& lhs, 
                const etl::chrono::month_weekday& rhs) 
ETL_NOEXCEPT
```
**Decription**  
Less-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month_weekday& lhs, 
                 const etl::chrono::month_weekday& rhs) 
ETL_NOEXCEPT
```
**Decription**  
Less-than-equal operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month_weekday& lhs, 
                const etl::chrono::month_weekday& rhs) 
ETL_NOEXCEPT
```
**Decription**  
Greater-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month_weekday& lhs, 
                 const etl::chrono::month_weekday& rhs) 
ETL_NOEXCEPT
```
**Decription**  
Greater-than-equal operator.

---

```cpp
[[nodiscard]] constexpr
auto operator <=>(const etl::chrono::month_weekday& lhs, 
                  const etl::chrono::month_weekday& rhs)
ETL_NOEXCEPT
```
**Decription**  
Spaceship operator.  
C++20

## Hash

```cpp
template <typename TRep, typename TPeriod>
struct hash<etl::chrono::month_weekday>
```
**Decription**  
Hash function for `etl::chrono::month_weekday`.

