---
title: "weekday"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::weekday](https://en.cppreference.com/w/cpp/chrono/weekday.html)
{{< /callout >}}

A class representing a weekday.

```cpp
class weekday
```

## Construction

```cpp
ETL_CONSTEXPR 
weekday()
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR 
explicit weekday(unsigned value) 
ETL_NOEXCEPT
```
**Description**  
Construct from `unsigned`.

---

```cpp
ETL_CONSTEXPR14 
weekday(const etl::chrono::sys_days& sd)
ETL_NOEXCEPT
```
**Description**  
Construct from `sys_days`.

---

```cpp
ETL_CONSTEXPR14 
weekday(const etl::chrono::local_days& sd)
ETL_NOEXCEPT
```
**Description**  
Construct from `local_days`.

---

```cpp
ETL_CONSTEXPR14 
weekday(const etl::chrono::weekday& other) 
ETL_NOEXCEPT
```
**Description**  
Copy constructor.

## Tests

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool ok() const 
ETL_NOEXCEPT
```
**Return**  
`true` if the weekday is valid

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool is_weekend() const 
ETL_NOEXCEPT
```
**Return**  
`true` if the weekday is a Saturday or Sunday.

## Assignment

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday& operator =(const etl::chrono::weekday& rhs) 
ETL_NOEXCEPT
```
**Description**  
Assignment operator      

## Access

```cpp
ETL_CONSTEXPR14 
int c_encoding() const 
ETL_NOEXCEPT
```
**Description**  
Gets the value of the weekday using C encoding.

---

```cpp
ETL_CONSTEXPR14 
int iso_encoding() const 
ETL_NOEXCEPT
```
**Description**  
Gets the value of the weekday using ISO encoding.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::weekday_indexed operator[](unsigned index) const 
ETL_NOEXCEPT
```
**Description**  
Index operator, from index.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::weekday_last operator[](etl::chrono::last_spec last) const 
ETL_NOEXCEPT
```
**Description**  
Index operator, from `etl::chrono::last_spec`.

## Increment/decrement

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday& operator ++() 
ETL_NOEXCEPT
```
**Description**  
Pre-increment operator.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::weekday operator ++(int) 
ETL_NOEXCEPT
```
**Description**  
Post-increment operator.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::weekday& operator --() 
ETL_NOEXCEPT
```
**Description**  
Pre-decrement operator.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday operator --(int) 
ETL_NOEXCEPT
```
**Description**  
Post-decrement operator.

## Mathematical operators

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday& operator +=(const etl::chrono::weekdays& ms) 
ETL_NOEXCEPT
```
**Description**  
Plus-equals operator adding `etl::chrono::weekdays`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday& operator -=(const etl::chrono::weekdays& ms) 
ETL_NOEXCEPT
```
**Description**  
Minus-equals operator subtracting `etl::chrono::weekdays`.

## Constants

```cpp
ETL_NODISCARD
static 
ETL_CONSTEXPR14
etl::chrono::weekday min()
ETL_NOEXCEPT
```
**Description**  
The minimum weekday value for which `ok()` will return true.

---

```cpp
ETL_NODISCARD
static 
ETL_CONSTEXPR14 
etl::chrono::weekday max()
ETL_NOEXCEPT
```
**Description**  
The maximum weekday  value for which `ok()` will return true.

## Non-member mathematical operators

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday operator +(const etl::chrono::weekday& m, 
                                const etl::chrono::weekdays& ms) 
ETL_NOEXCEPT
```
**Description**  
Add `etl::chrono::weekdays` to `etl::chrono::weekday`.
**Return**  
`etl::chrono::weekday`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday operator +(const etl::chrono::weekdays& ms, 
                                const etl::chrono::weekday& m) 
ETL_NOEXCEPT
```
**Description**  
Add `etl::chrono::weekday` to `etl::chrono::weekdays`.
**Return**  
`etl::chrono::weekday`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekday operator -(const etl::chrono::weekday& m, 
                                const etl::chrono::weekdays& ms) 
ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::weekdays` from `etl::chrono::weekday`.
**Return**  
`etl::chrono::weekday`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::weekday operator -(const etl::chrono::weekdays& ms, 
                                const etl::chrono::weekday& m) 
ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::weekday` from `etl::chrono::weekdays`.
**Return**  
`etl::chrono::weekdays`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::weekdays operator -(const etl::chrono::weekday& m1, 
                                 const etl::chrono::weekday& m2) ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::weekday` from `etl::chrono::weekday`.
**Return**  
`etl::chrono::weekdays`.

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::weekday& m1, 
                 const etl::chrono::weekday& m2)
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::weekday& m1, 
                 const etl::chrono::weekday& m2) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::weekday& m1, 
                const etl::chrono::weekday& m2) 
ETL_NOEXCEPT
```
**Description**  
Less-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::weekday& m1, 
                 const etl::chrono::weekday& m2) 
ETL_NOEXCEPT
```
**Description**  
Less-than-or-equal operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator >(const etl::chrono::weekday& m1, 
                const etl::chrono::weekday& m2) 
ETL_NOEXCEPT
```
**Description**  
Greater-than operator.

---

```cpp
ETL_CONSTEXPR14
bool operator >=(const etl::chrono::weekday& m1, 
                 const etl::chrono::weekday& m2) 
ETL_NOEXCEPT
```
**Description**  
Greater-than-or-equal operator.

---

```cpp
[[nodiscard]] inline constexpr 
auto operator <=>(const etl::chrono::weekday& m1, 
                  const etl::chrono::weekday& m2) 
noexcept
```
**Description**  
Spaceship operator.  
C++20

## Defined weekdays

```cpp
etl::chrono::Monday
etl::chrono::Tuesday
etl::chrono::Wednesday
etl::chrono::Thursday
etl::chrono::Friday
etl::chrono::Saturday
etl::chrono::Sunday
```

## Hash

```cpp
template <>
struct hash<etl::chrono::weekday>
```
**Description**  
Hash function for `etl::chrono::weekday`.
