---
title: "weekday_indexed"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::weekday_indexed](https://en.cppreference.com/w/cpp/chrono/weekday_last.html)
{{< /callout >}}

A class representing an indexed weekday.

```cpp
class weekday_indexed
```

## Construction

```cpp
ETL_CONSTEXPR 
weekday_indexed()
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
weekday_indexed(const etl::chrono::weekday& wd, unsigned index)
ETL_NOEXCEPT
```
**Description**  
Construct from `weekday` and `index`.

---

```cpp
ETL_CONSTEXPR14 
weekday_indexed(const etl::chrono::weekday_indexed& other) 
ETL_NOEXCEPT
```
**Description**  
Copy constructor.

## Tests

ETL_NODISCARD
ETL_CONSTEXPR14 
bool ok() const 
ETL_NOEXCEPT
**Return**  
`true` if the weekday_indexed is valid

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool is_weekend() const 
ETL_NOEXCEPT
```
**Return**  
`true` if the `weekday_indexed` is a `Saturday` or `Sunday`.

## Assignment

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday_indexed& operator =(const etl::chrono::weekday_indexed& rhs) 
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

---

```cpp
ETL_CONSTEXPR14 
unsigned index() const 
ETL_NOEXCEPT
```
**Description**  
Gets the index.

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::weekday_indexed& m1, 
                 const etl::chrono::weekday_indexed& m2)
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::weekday_indexed& m1, 
                 const etl::chrono::weekday_indexed& m2) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

## Hash

```cpp
template <>
struct hash<etl::chrono::weekday_indexed>
```
**Description**  
Hash function for `etl::chrono::weekday_indexed`.
 