---
title: "time_point"
---

{{< callout type="info">}}
  Header: `chrono.h`  
  From: `20.41.0`  
  Similar to: [std::chrono::time_point](https://en.cppreference.com/w/cpp/chrono/time_point.html)
{{< /callout >}}

```cpp
template <typename TClock, typename TDuration = typename TClock::duration>
class time_point;
```
Represents a point in time storing a `TDuration` indicating the time 
interval from the start of the `TClock`'s epoch.

## Member types

```cpp
using clock    = TClock
using duration = TDuration
using rep      = typename TDuration::rep
using period   = typename TDuration::period
```

## Constructors

```cpp
ETL_CONSTEXPR 
time_point() 
ETL_NOEXCEPT
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14 
explicit time_point(const duration& dur_) 
ETL_NOEXCEPT
```
**Description**  
Construct from a duration.

---

```cpp
ETL_CONSTEXPR14 
time_point(const time_point& rhs) 
ETL_NOEXCEPT
```
**Description**  
Copy constructor.

---

```cpp
template <typename TDuration2>
ETL_CONSTEXPR14 
explicit time_point(const time_point<clock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Copy construct from another time_point with a different duration type.

## Assignment
```cpp
ETL_CONSTEXPR14 
time_point& operator =(const time_point& rhs) 
ETL_NOEXCEPT
```
**Description**  
Assignment operator.

## Access
```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 duration time_since_epoch() const 
ETL_NOEXCEPT
```
**Return**  
A duration representing the amount of time between this and the clock's epoch.

## Member arithmetic operators
```cpp
ETL_CONSTEXPR14 time_point& operator +=(const duration& rhs) 
ETL_NOEXCEPT
```
**Description**  
Adds a duration.

---

```cpp
ETL_CONSTEXPR14 time_point& operator -=(const duration& rhs) 
ETL_NOEXCEPT
```
**Description**  
Subtracts a duration.

## Increment/decrement
```cpp
ETL_CONSTEXPR14 time_point& operator ++() 
ETL_NOEXCEPT
```
**Description**  
Pre-increments the stored duration by one tick.

---

```cpp
ETL_CONSTEXPR14 time_point operator ++(int) 
ETL_NOEXCEPT
```
**Description**  
Post-increments the stored duration by one tick.

---

```cpp
ETL_CONSTEXPR14 time_point& operator --() 
ETL_NOEXCEPT
```
**Description**  
Pre-decrements the stored duration by one tick.

---

```cpp
ETL_CONSTEXPR14 time_point operator --(int) 
ETL_NOEXCEPT
```
**Description**  
Post-decrements the stored duration by one tick.

## Constants
```cpp
ETL_NODISCARD
static ETL_CONSTEXPR14 time_point min() 
ETL_NOEXCEPT
```
**Return**  
A time_point with the smallest possible duration.

---

```cpp
ETL_NODISCARD
static ETL_CONSTEXPR14 time_point max() 
ETL_NOEXCEPT
```
**Return**  
A time_point with the largest possible duration.

## Rounding
```cpp
template <typename TToDuration, typename TClock, typename TDuration>
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::time_point<TClock, TToDuration>
  floor(const etl::chrono::time_point<TClock, TDuration>& tp) 
ETL_NOEXCEPT
```
**Description**  
Rounds down a duration to the nearest lower precision.

---

```cpp
template <typename TToDuration, typename TClock, typename TDuration>
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::time_point<TClock, TToDuration>
  ceil(const etl::chrono::time_point<TClock, TDuration>& tp) 
ETL_NOEXCEPT
```
**Description**  
Rounds up a duration to the nearest higher precision.

---

```cpp
template <typename TToDuration, typename TClock, typename TDuration>
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::time_point<TClock, TToDuration>
  round(const etl::chrono::time_point<TClock, TDuration>& tp) 
ETL_NOEXCEPT
```
**Description**  
Rounds a duration to the nearest precision.
If the duration is exactly halfway, it rounds away from zero.

## Casting
```cpp
template <typename TToDuration, typename TClock, typename TDuration>
ETL_NODISCARD
ETL_CONSTEXPR14 
etl::chrono::time_point<TClock, TToDuration>
  time_point_cast(const etl::chrono::time_point<TClock, TDuration>& tp) 
ETL_NOEXCEPT
```

## Comparison
```cpp
ETL_NODISCARD
ETL_CONSTEXPR14 int compare(const time_point& other) const 
ETL_NOEXCEPT
```
**Description**  
Compare day with another.  
If time_point < other, returns -1  
else if time_point > other, returns 1  
else returns 0

## Non-member mathematical operators
```cpp
template <typename TClock, typename TDuration1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
etl::chrono::time_point<TClock, typename etl::common_type<TDuration1, etl::chrono::duration<TRep2, TPeriod2> >::type>
  operator +(const time_point<TClock, TDuration1>& lhs, 
             const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Adds a duration to a time_point.  
**Return**  
A time_point whose duration is the common type of the two.

---

```cpp
template <typename TRep1, typename TPeriod1, typename TClock, typename TDuration2>
ETL_CONSTEXPR14 
etl::chrono::time_point<TClock, typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, TDuration2>::type>
  operator +(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
             const time_point<TClock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Adds a duration to a time_point.  
**Return**  
A time_point whose duration is the common type of the two.

---

```cpp
template <typename TClock, typename TDuration1, typename TRep2, typename TPeriod2>
ETL_CONSTEXPR14 
etl::chrono::time_point<TClock, typename etl::common_type<TDuration1, etl::chrono::duration<TRep2, TPeriod2> >::type>
  operator -(const time_point<TClock, TDuration1>& lhs, 
             const etl::chrono::duration<TRep2, TPeriod2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Subtracts a duration from a time_point.  
**Return**  
A time_point whose duration is the common type of the two.

---

```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
ETL_CONSTEXPR14 
typename etl::common_type<TDuration1, TDuration2>::type 
  operator -(const time_point<TClock, TDuration1>& lhs, 
             const time_point<TClock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Subtracts one time_point from another.  
**Return**  
The duration between them as the common type of the two durations.

## Non-member comparison operators
```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
ETL_CONSTEXPR14 
bool operator ==(const time_point<TClock, TDuration1>& lhs, 
                 const time_point<TClock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Equality operator

---

```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
ETL_CONSTEXPR14 
bool operator !=(const time_point<TClock, TDuration1>& lhs, 
                 const time_point<TClock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Inequality operator

---

```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
ETL_CONSTEXPR14 
bool operator <(const time_point<TClock, TDuration1>& lhs, 
                const time_point<TClock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Less-than operator

---

```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
ETL_CONSTEXPR14 
bool operator <=(const time_point<TClock, TDuration1>& lhs, 
                 const time_point<TClock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Less-than-equal operator

---

```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
ETL_CONSTEXPR14 
bool operator >(const time_point<TClock, TDuration1>& lhs, 
                const time_point<TClock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Greater-than operator

---

```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
ETL_CONSTEXPR14 
bool operator >=(const time_point<TClock, TDuration1>& lhs,
                 const time_point<TClock, TDuration2>& rhs) 
ETL_NOEXCEPT
```
**Description**  
Greater-than-equal operator

---

```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
[[nodiscard]] constexpr auto operator <=>(const etl::chrono::time_point<TClock, TDuration1>& lhs, 
                                          const etl::chrono::time_point<TClock, TDuration2>& rhs) 
noexcept
```
**Description**  
Spaceship operator
C++20

## etl::common_type specialisation
```cpp
template <typename TClock, typename TDuration1, typename TDuration2>
struct common_type<etl::chrono::time_point<TClock, TDuration1>, 
                   etl::chrono::time_point<TClock, TDuration2>>
```
**Description**  
Defines type, which is the common type of two `etl::chrono::time_point`.
