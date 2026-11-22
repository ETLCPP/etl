---
title: "weekday_last"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  From: `20.41.0`  
  Similar to: [std::chrono::weekday_last](https://en.cppreference.com/w/cpp/chrono/weekday_last.html)
{{< /callout >}}

A class representing the last day of a weekday.  
In C++11 and later, `etl::chrono::weekday_last` is trivially copyable.

```cpp
class weekday_last
```

## Construction

```cpp
ETL_CONSTEXPR 
weekday_last()
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
weekday_last(const etl::chrono::weekday& wd)
ETL_NOEXCEPT
```
**Description**  
Construct from weekday and index.

---

```cpp
weekday_last(const weekday_last&) = default; // Since C++11

ETL_CONSTEXPR14 
weekday_last(const etl::chrono::weekday_last& other) ETL_NOEXCEPT // C++03
```
**Description**  
Copy constructor. (Defaulted and trivially copyable in C++11 and later).

---

## Tests

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool ok() const 
ETL_NOEXCEPT
```
**Return**  
`true` if the weekday_last valid.

## Assignment

```cpp
weekday_last& operator =(const weekday_last&) = default; // Since C++11

ETL_CONSTEXPR14 
etl::chrono::weekday_last& operator =(const etl::chrono::weekday_last& rhs) ETL_NOEXCEPT // C++03
```
**Description**  
Assignment operator. (Defaulted in C++11 and later).      

## Access

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday weekday() const 
ETL_NOEXCEPT
```
**Description**  
Gets the weekday.

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::weekday_last& m1, 
                 const etl::chrono::weekday_last& m2)
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::weekday_last& m1, 
                 const etl::chrono::weekday_last& m2) 
ETL_NOEXCEPT
**Description**  
Inequality operator.

## Hash

```cpp
template <>
struct hash<etl::chrono::weekday_last>
```
**Description**  
Hash function for `etl::chrono::weekday_last`.
