---
title: "year"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::year](https://en.cppreference.com/w/cpp/chrono/year.html)
{{< /callout >}}

A template representing a year.

```cpp
class year
```

## Member types

`rep` The internal representation.

## Construction
```cpp
ETL_CONSTEXPR 
year()
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR 
explicit year(unsigned value) 
ETL_NOEXCEPT
```
**Description**  
Construct from unsigned.

---

```cpp
ETL_CONSTEXPR14 
year(const etl::chrono::year& other) 
ETL_NOEXCEPT
```
**Description**  
Copy constructor.

## Tests
```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
int compare(const etl::chrono::year& other) const 
ETL_NOEXCEPT
```
**Description**  
Compare day with another.
if `year < other`, returns `-1`
else if `year > other`, returns `1`
else returns `0`

## Assignment
```cpp
ETL_CONSTEXPR14 
etl::chrono::year& operator =(const etl::chrono::year& rhs) 
ETL_NOEXCEPT
```
**Description**  
Assignment operator      

## Increment / decrement
```cpp
ETL_CONSTEXPR14 
etl::chrono::year& operator ++() 
ETL_NOEXCEPT
```
**Description**  
Pre-increment operator.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year operator ++(int) 
ETL_NOEXCEPT
```
**Description**  
Post-increment operator.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year& operator --() 
ETL_NOEXCEPT
```
**Description**  
Pre-decrement operator.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year operator --(int) 
ETL_NOEXCEPT
```
**Description**  
Post-decrement operator.

## Mathematical operators
```cpp
ETL_CONSTEXPR14 
etl::chrono::year& operator +=(const etl::chrono::years& ys) 
ETL_NOEXCEPT
```
**Description**  
Plus-equals operator adding etl::chrono::years.

---
```cpp
ETL_CONSTEXPR14 
etl::chrono::year& operator -=(const etl::chrono::years& ys) 
ETL_NOEXCEPT
```
**Description**  
Minus-equals operator subtracting etl::chrono::years.

## Tests
```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool ok() const 
ETL_NOEXCEPT
```
**Description**  
Returns true if the year is within the valid -32767 to 32767 range.

---
```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
bool is_leap() const 
ETL_NOEXCEPT
```
**Description**  
Returns true if the year is a leap year.

## Constants
```cpp
ETL_NODISCARD
static 
ETL_CONSTEXPR14
etl::chrono::year min()
ETL_NOEXCEPT
```
**Description**  
The minimum year value for which ok() will return true.

---

```cpp
ETL_NODISCARD
static 
ETL_CONSTEXPR14 
etl::chrono::year max()
ETL_NOEXCEPT
```
**Description**  
The maximum year value for which ok() will return true.

## Conversion
```cpp
ETL_CONSTEXPR14 
operator int() const 
ETL_NOEXCEPT
```
**Description**  
Conversion operator to `unsigned int`.

## Non-member mathematical operators
```cpp
ETL_CONSTEXPR14 
etl::chrono::year operator +(const etl::chrono::year& y, 
                             const etl::chrono::years& ys) 
ETL_NOEXCEPT
```
**Description**  
Add `etl::chrono::years` to `etl::chrono::year`.
**Return**  
`etl::chrono::year`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year operator +(const etl::chrono::years& ys, 
                             const etl::chrono::year& y) 
ETL_NOEXCEPT
```
**Description**  
Add `etl::chrono::year` to `etl::chrono::years`.
**Return**  
`etl::chrono::year`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::year operator -(const etl::chrono::year& y, 
                             const etl::chrono::years& ys) 
ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::years` from `etl::chrono::year`.
**Return**  
`etl::chrono::year`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::year operator -(const etl::chrono::years& ys, 
                             const etl::chrono::year& y) 
ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::year` from `etl::chrono::years`.
**Return**  
`etl::chrono::years`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::years operator -(const etl::chrono::year& y1, 
                              const etl::chrono::year& y2) ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::year` from `etl::chrono::year`.
**Return**  
`etl::chrono::years`.

## Non-member comparison operators
```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::year& y1, 
                 const etl::chrono::year& y2)
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::year& y1, 
                 const etl::chrono::year& y2) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::year& y1, 
                const etl::chrono::year& y2) 
ETL_NOEXCEPT
```
**Description**  
Less-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::year& y1, 
                 const etl::chrono::year& y2) 
ETL_NOEXCEPT
```
**Description**  
Less-than-or-equal operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator >(const etl::chrono::year& y1, 
                const etl::chrono::year& y2) 
ETL_NOEXCEPT
```
**Description**  
Greater-than operator.

---

```cpp
ETL_CONSTEXPR14
bool operator >=(const etl::chrono::year& y1, 
                 const etl::chrono::year& y2) 
ETL_NOEXCEPT
```
**Description**  
Greater-than-or-equal operator.

---

```cpp
[[nodiscard]] inline constexpr 
auto operator <=>(const etl::chrono::year& y1, 
                  const etl::chrono::year& y2) 
noexcept
```
**Description**  
Spaceship operator.  
C++20

## Hash
```cpp
template <typename TRep, typename TPeriod>
struct hash<etl::chrono::year>
```
**Description**  
Hash function for `etl::chrono::year`.

