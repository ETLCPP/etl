---
title: "month"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `20.41.0`  
  Similar to: [std::chrono::month](https://en.cppreference.com/w/cpp/chrono/month.html)
{{< /callout >}}

A class representing a month.

```cpp
class month
```
## Member types
`rep` The internal representation.

## Construction
```cpp
ETL_CONSTEXPR 
month()
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR 
explicit month(unsigned value) 
ETL_NOEXCEPT
```
**Description**  
Construct from unsigned.

---

```cpp
ETL_CONSTEXPR14 
month(const etl::chrono::month& other) 
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
**Description**  
Returns true if the month is in range.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
int compare(const etl::chrono::month& other) const 
ETL_NOEXCEPT
```
**Description**  
Compare month with another.  
if month < other, returns -1  
else if month > other, returns 1  
else returns 0

## Assignment
```cpp
ETL_CONSTEXPR14 
etl::chrono::month& operator =(const etl::chrono::month& rhs) 
ETL_NOEXCEPT
```
**Description**  
Assignment operator      

## Increment / decrement

```cpp
ETL_CONSTEXPR14 
etl::chrono::month& operator ++() 
ETL_NOEXCEPT
```
**Description**  
Pre-increment operator.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::month operator ++(int) 
ETL_NOEXCEPT
```
**Description**  
Post-increment operator.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::month& operator --() 
ETL_NOEXCEPT
```
**Description**  
Pre-decrement operator.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::month operator --(int) 
ETL_NOEXCEPT
```
**Description**  
Post-decrement operator.

## Mathematical operators

```cpp
ETL_CONSTEXPR14 
etl::chrono::month& operator +=(const etl::chrono::months& ms) 
ETL_NOEXCEPT
```
**Description**  
Plus-equals operator adding `etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::month& operator -=(const etl::chrono::months& ms) 
ETL_NOEXCEPT
```
**Description**  
Minus-equals operator subtracting `etl::chrono::months`.

##  Constants

```cpp
ETL_NODISCARD
static 
ETL_CONSTEXPR14
etl::chrono::month min()
ETL_NOEXCEPT
```
**Description**  
The minimum month value for which ok() will return true.

---

```cpp
ETL_NODISCARD
static 
ETL_CONSTEXPR14 
etl::chrono::month max()
ETL_NOEXCEPT
```
**Description**  
The maximum month  value for which ok() will return true.

## Conversion

```cpp
ETL_CONSTEXPR14 
operator unsigned() const 
ETL_NOEXCEPT
```
**Description**  
Conversion operator to `unsigned int`.

## Non-member mathematical operators

```cpp
ETL_CONSTEXPR14 
etl::chrono::month operator +(const etl::chrono::month& m, 
                              const etl::chrono::months& ms) 
ETL_NOEXCEPT
```
**Description**  
Add `etl::chrono::months` to `etl::chrono::month`.
**Return**  
`etl::chrono::month.`

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::month operator +(const etl::chrono::months& ms, 
                              const etl::chrono::month& m) 
ETL_NOEXCEPT
```
**Description**  
Add `etl::chrono::month` to `etl::chrono::months`.
**Return**  
`etl::chrono::month`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::month operator -(const etl::chrono::month& m, 
                              const etl::chrono::months& ms) 
ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::months` from `etl::chrono::month`.
**Return**  
`etl::chrono::month`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::month operator -(const etl::chrono::months& ms, 
                              const etl::chrono::month& m) 
ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::month` from `etl::chrono::months`.
**Return**  
`etl::chrono::months`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::months operator -(const etl::chrono::month& m1, 
                               const etl::chrono::month& m2) ETL_NOEXCEPT
**Description**  
Subtract `etl::chrono::month` from `etl::chrono::month`.
**Return**  
`etl::chrono::months`.

## Non-member comparison operators

```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::month& m1, 
                 const etl::chrono::month& m2)
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::month& m1, 
                 const etl::chrono::month& m2) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::month& m1, 
                const etl::chrono::month& m2) 
ETL_NOEXCEPT
```
**Description**  
Less-than operator.

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::month& m1, 
                 const etl::chrono::month& m2) 
ETL_NOEXCEPT
```
**Description**  
Less-than-or-equal operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator >(const etl::chrono::month& m1, 
                const etl::chrono::month& m2) 
ETL_NOEXCEPT
```
**Description**  
Greater-than operator.

---

```cpp
ETL_CONSTEXPR14
bool operator >=(const etl::chrono::month& m1, 
                 const etl::chrono::month& m2) 
ETL_NOEXCEPT
```
**Description**  
Greater-than-or-equal operator.

---

```cpp
[[nodiscard]] inline constexpr 
auto operator <=>(const etl::chrono::month& m1, 
                  const etl::chrono::month& m2) 
noexcept
```
**Description**  
Spaceship operator.  
C++20

## Defined months
```cpp
etl::chrono::January
etl::chrono::February
etl::chrono::March
etl::chrono::April
etl::chrono::May
etl::chrono::June
etl::chrono::July
etl::chrono::August
etl::chrono::September
etl::chrono::October
etl::chrono::November
etl::chrono::December
```
## Hash
```cpp
template <>
struct hash<etl::chrono::month>
```
Hash function for `etl::chrono::month`.

