---
title: "duration"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  Since: `TBC`  
  Similar to: [std::chrono::duration](https://en.cppreference.com/w/cpp/chrono/duration.html)  
  Similar to: [std::chrono::duration_values](https://en.cppreference.com/w/cpp/chrono/duration_values.html)
{{< /callout >}}

Templates representing a time interval.

## duration_values
```cpp
template <typename TRep>
struct duration_values
```
```cpp
ETL_NODISCARD static ETL_CONSTEXPR TRep zero() ETL_NOEXCEPT
```
**Return**  
`TRep(0)`

---

```cpp
ETL_NODISCARD static ETL_CONSTEXPR14 TRep min() ETL_NOEXCEPT
```
**Return**  
`etl::numeric_limits<TRep>::min()`

---

```cpp
ETL_NODISCARD static ETL_CONSTEXPR14 TRep max() ETL_NOEXCEPT
```
**Returns**  
`etl::numeric_limits<TRep>::max()`

## duration
```cpp
template <typename TRep, typename TPeriod = etl::ratio<1>>
class duration
```
```cpp
using rep = TRep
using period = typename TPeriod::type;
```

### Construction
```cpp
ETL_CONSTEXPR 
duration() 
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14
duration(const etl::chrono::duration<TRep, TPeriod>& other) 
ETL_NOEXCEPT
```
---

```cpp
template <typename TRep2>
ETL_CONSTEXPR14 
explicit duration(const TRep2& value) 
ETL_NOEXCEPT
```

---

```cpp
template <typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
duration(const etl::chrono::duration<TRep2, TPeriod2>& other) 
ETL_NOEXCEPT
```
**Description**  
Construct from another duration type.  
Enabled if `etl::ratio_divide<TPeriod2, TPeriod>::den == 1`

### Tests
```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
int compare(const time_point& other) const 
ETL_NOEXCEPT
```
**Description**  
Compare day with another.  
if `time_point < other`, returns `-1`  
else if `time_point > other`, returns `1`  
else returns `0`

### Assignment
```cpp
ETL_CONSTEXPR14 
etl::chrono::duration<TRep, TPeriod> operator =(const etl::chrono::duration<TRep, TPeriod>& other) ETL_NOEXCEPT
```

---

```cpp
template <typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
etl::chrono::duration<TRep, TPeriod> operator =(const etl::chrono::duration<TRep2, TPeriod2>& other) ETL_NOEXCEPT
```

### Convertion
```cpp
ETL_CONSTEXPR14 
TRep count() const 
ETL_NOEXCEPT
```
**Return**  
The duration count as a numeric.

---

```cpp
ETL_CONSTEXPR14 
etl::common_type_t<duration> operator +() const 
ETL_NOEXCEPT
```
**Description**  
Implements unary plus.

---

```cpp
ETL_CONSTEXPR14 
etl::common_type_t<duration> operator -() const 
ETL_NOEXCEPT
```
**Description**  
Implements unary minus.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::duration<TRep, TPeriod> absolute() const 
ETL_NOEXCEPT
```
**Return**  
The absolute value of the duration.

---

### Constants
```cpp
ETL_NODISCARD
static ETL_CONSTEXPR14 etl::chrono::duration<TRep, TPeriod> zero()  
ETL_NOEXCEPT
```
**Return**  
The duration zero value.

---

```cpp
ETL_NODISCARD
static ETL_CONSTEXPR14 etl::chrono::duration<TRep, TPeriod> min()
ETL_NOEXCEPT
```
**Return**  
The minimum duration value.

---

```cpp
ETL_NODISCARD
static ETL_CONSTEXPR14 etl::chrono::duration<TRep, TPeriod> max()
ETL_NOEXCEPT
---
**Return**  
The maximum duration value.

## Increment/decrement
```cpp
ETL_CONSTEXPR14 
duration& operator ++()
ETL_NOEXCEPT
```
**Description**  
Pre-increments the duration count.

---

```cpp
ETL_CONSTEXPR14 
duration operator ++(int)
ETL_NOEXCEPT
```
**Description**  
Post-increments the duration count.

---

```cpp
ETL_CONSTEXPR14 
duration& operator --()
ETL_NOEXCEPT
```
**Description**  
Pre-decrements the duration count.

---
```cpp
ETL_CONSTEXPR14 
duration operator --(int)
ETL_NOEXCEPT
```
**Description**  
Post-decrements the duration count.

### Mathematical operators
```cpp
ETL_CONSTEXPR14 
duration& operator +=(const duration<TRep, TPeriod>& d)
ETL_NOEXCEPT
```
**Description**  
Adds duration `d` to this duration.

---

```cpp
ETL_CONSTEXPR14 
duration& operator -=(const duration<TRep, TPeriod>& d)
ETL_NOEXCEPT
```
**Description**  
Subtracts duration `d` to this duration.

---
```cpp
ETL_CONSTEXPR14 
duration& operator *=(const TRep& r) 
ETL_NOEXCEPT
```
**Description**  
Multiplies this duration by `r`.

---

```cpp
ETL_CONSTEXPR14 
duration& operator /=(const TRep& r)
ETL_NOEXCEPT
```
**Description**  
Divides this duration by `r`.

---

```cpp
ETL_CONSTEXPR14 
duration& operator %=(const TRep& r)
ETL_NOEXCEPT
```
**Description**  
Sets this duration to the modulus of `r`.

---

```cpp
ETL_CONSTEXPR14 
duration& operator %=(const duration<TRep, TPeriod>& d)
ETL_NOEXCEPT
```
**Description**  
Sets this duration to the modulus of duration `d`.

### Comparison
```cpp
template <typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
int compare(const duration<TRep2, TPeriod2>& other) const
ETL_NOEXCEPT
```
**Description**  
Compare duration with another.  
If `duration < other`, returns `-1`  
else if `duration > other`, returns `1`  
else returns `0`

### Non-member mathematical operators
```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
etl::common_type_t<etl::chrono::duration<TRep1, TPeriod1>, 
                   etl::chrono::duration<TRep2, TPeriod2>>
operator +(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
           const etl::chrono::duration<TRep2, TPeriod2>& rhs)
ETL_NOEXCEPT
```
**Description**  
Operator `+`

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14
etl::common_type_t<etl::chrono::duration<TRep1, TPeriod1>, 
                   etl::chrono::duration<TRep2, TPeriod2> >::type 
operator -(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
           const etl::chrono::duration<TRep2, TPeriod2>& rhs)
ETL_NOEXCEPT
```
**Description**  
Operator `-`

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2>
ETL_CONSTEXPR14
etl::chrono::duration<etl::common_type_t<TRep1, TRep2>, TPeriod1>>
operator *(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
           const TRep2& rhs)
ETL_NOEXCEPT
```
**Description**  
Operator `*`  
Enabled if `TRep2` is not a specialization of `etl::chrono::duration`.

---

```cpp
template <typename TRep1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
etl::chrono::duration<etl::common_type_t<TRep1, TRep2>, TPeriod2>
operator *(const TRep1& lhs, 
           const etl::chrono::duration<TRep2, TPeriod2>& rhs)
ETL_NOEXCEPT
```
**Description**  
Operator `*`

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2>
ETL_CONSTEXPR14 
etl::common_type_t<TRep1, TRep2>, TPeriod1>
operator /(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
           const TRep2& rhs)
ETL_NOEXCEPT
```
**Description**  
Operator `/`  
Enabled if `TRep2` is not a specialization of `etl::chrono::duration`.

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14
etl::common_type_t<TRep1, TRep2>
operator /(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
           const etl::chrono::duration<TRep2, TPeriod2>& rhs)
ETL_NOEXCEPT
```
**Description**  
Operator `/`

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2>
ETL_CONSTEXPR14 
etl::chrono::duration<etl::common_type_t<TRep1, TRep2>, TPeriod1>
operator %(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
           const TRep2& rhs)
ETL_NOEXCEPT
```
**Description**  
Operator `%`

```

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
etl::common_type_t<etl::chrono::duration<TRep1, TPeriod1>, 
                   etl::chrono::duration<TRep2, TPeriod2>>
operator %(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
           const etl::chrono::duration<TRep2, TPeriod2>& rhs)
ETL_NOEXCEPT
```
**Description**  
Operator `%`

### Non-member comparison operators
```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14
bool operator ==(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                 const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Checks for equality.

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
bool operator !=(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                 const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Checks for inequality.

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
bool operator <(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Checks for less-than.

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
bool operator <=(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                 const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Checks for less-than-or-equal.

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14
bool operator >(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Checks for greater-than.

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14
bool operator >=(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                 const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Checks for greater-than-or-equal.

---

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
[[nodiscard]] 
constexpr 
auto operator <=>(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                  const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
noexcept
```
**Description**  
Spaceship operator.  
C++20

## common_type

```cpp
template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
struct common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2>>
```
**Description**  
`etl::common_type` specialisation for `etl::duration`.

## Hash
```cpp
template <typename TRep, typename TPeriod>
struct hash<etl::chrono::duration<TRep, TPeriod>>
```

## Pre-defined duration types

```cpp
etl::chrono::years
etl::chrono::months
etl::chrono::weeks
etl::chrono::days
etl::chrono::hours
etl::chrono::minutes
etl::chrono::seconds
etl::chrono::milliseconds
etl::chrono::microseconds
etl::chrono::nanoseconds
```

## duration_cast
```cpp
template <typename TToDuration, typename TRep, typename TPeriod>
ETL_CONSTEXPR14 
TToDuration duration_cast(const etl::chrono::duration<TRep, TPeriod>& d)
ETL_NOEXCEPT
```
**Description**  
Converts from one duration type to another.
