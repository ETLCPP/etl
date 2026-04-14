---
title: "weekday_last"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::weekday_last](https://en.cppreference.com/w/cpp/chrono/weekday_last.html)
{{< /callout >}}

A class representing the last day of a weekday.

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
ETL_CONSTEXPR14 
weekday_last(const etl::chrono::weekday_last& other) 
ETL_NOEXCEPT
```
**Description**  
Copy constructor.

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
ETL_CONSTEXPR14 
etl::chrono::weekday_last& operator =(const etl::chrono::weekday_last& rhs) 
ETL_NOEXCEPT
```
**Description**  
Assignment operator      

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
