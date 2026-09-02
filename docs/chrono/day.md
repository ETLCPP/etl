---
title: "day"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `TBC`  
  Similar to: [std::chrono::day](https://en.cppreference.com/w/cpp/chrono/day.html)
{{< /callout >}}

A class representing a day.

```cpp
class day
```

## Typesdefs
rep  The internal representation.

## Construction
```cpp
ETL_CONSTEXPR 
day()
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---
```cpp
ETL_CONSTEXPR 
explicit day(unsigned value) 
ETL_NOEXCEPT
```
**Description**  
Construct from unsigned.

---

```cpp
ETL_CONSTEXPR14 
day(const etl::chrono::day& other) 
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
`true` if the day is within the valid range.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
int compare(const etl::chrono::day& other) const 
ETL_NOEXCEPT
```
**Description**  
Compare `day` with another.  
if `day < other`, returns `-1`  
else if `day > other`, returns `1`  
else returns `0`

## Assignment
```cpp
ETL_CONSTEXPR14 
etl::chrono::day& operator =(const etl::chrono::day& rhs) 
ETL_NOEXCEPT
```
**Description**  
Assignment operator      

## Increment/decrement
```cpp
ETL_CONSTEXPR14 
etl::chrono::day& operator ++() 
ETL_NOEXCEPT
```
**Description**  
Pre-increment operator.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::day operator ++(int) 
ETL_NOEXCEPT
```
**Description**  
Post-increment operator.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::day& operator --() 
ETL_NOEXCEPT
```
**Description**  
Pre-decrement operator.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::day operator --(int) 
ETL_NOEXCEPT
```
**Description**  
Post-decrement operator.

## Mathematical operators
```cpp
ETL_CONSTEXPR14 
etl::chrono::day& operator +=(const etl::chrono::days& ms) 
ETL_NOEXCEPT
```
**Description**  
Plus-equals operator adding etl::chrono::days.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::day& operator -=(const etl::chrono::days& ms) 
ETL_NOEXCEPT
```
**Description**  
Minus-equals operator subtracting `etl::chrono::days`.

## Conversion
```cpp
ETL_CONSTEXPR14 
operator unsigned() const 
ETL_NOEXCEPT
```
**Description**  
Conversion operator to unsigned int.

## Non-member mathematical operators
```cpp
ETL_CONSTEXPR14 
etl::chrono::day operator +(const etl::chrono::day& m, 
                              const etl::chrono::days& ms) 
ETL_NOEXCEPT
```
**Description**  
Add `etl::chrono::days` to `etl::chrono::day`.  

**Return**  
`etl::chrono::day.`

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::day operator +(const etl::chrono::days& ms, 
                              const etl::chrono::day& m) 
ETL_NOEXCEPT
```
**Description**  
Add `etl::chrono::day` to `etl::chrono::days`.  

**Return**  
`etl::chrono::day`.

---
```cpp
ETL_CONSTEXPR14 
etl::chrono::day operator -(const etl::chrono::day& m, 
                              const etl::chrono::days& ms) 
ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::days` from `etl::chrono::day`.  

**Return**  
`etl::chrono::day`.

---

```cpp
ETL_CONSTEXPR14
etl::chrono::day operator -(const etl::chrono::days& ms, 
                              const etl::chrono::day& m) 
ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::day` from `etl::chrono::days`.  

**Return**  
`etl::chrono::days`.

---

```cpp
ETL_CONSTEXPR14 
etl::chrono::days operator -(const etl::chrono::day& m1, 
                               const etl::chrono::day& m2) ETL_NOEXCEPT
```
**Description**  
Subtract `etl::chrono::day` from `etl::chrono::day`.

**Return**  
`etl::chrono::days`.

## Non-member comparison operators
```cpp
ETL_CONSTEXPR14 
bool operator ==(const etl::chrono::day& m1, 
                 const etl::chrono::day& m2)
ETL_NOEXCEPT
```
**Description**  
Equality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::day& m1, 
                 const etl::chrono::day& m2) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::day& m1, 
                const etl::chrono::day& m2) 
ETL_NOEXCEPT
```
**Description**  
Less-than operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::day& m1, 
                 const etl::chrono::day& m2) 
ETL_NOEXCEPT
```
**Description**  
Less-than-or-equal operator.

---

```cpp
ETL_CONSTEXPR14 
bool operator >(const etl::chrono::day& m1, 
                const etl::chrono::day& m2) 
ETL_NOEXCEPT
```
**Description**  
Greater-than operator.

---

```cpp
ETL_CONSTEXPR14
bool operator >=(const etl::chrono::day& m1, 
                 const etl::chrono::day& m2) 
ETL_NOEXCEPT
```
**Description**  
Greater-than-or-equal operator.

---

```cpp
[[nodiscard]] inline constexpr 
auto operator <=>(const etl::chrono::day& m1, 
                  const etl::chrono::day& m2) 
noexcept
```
**Description**  
Spaceship operator.
C++20

## Defined days

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
struct hash<etl::chrono::day>
```
**Description**  
Hash function for `etl::chrono::day`.

