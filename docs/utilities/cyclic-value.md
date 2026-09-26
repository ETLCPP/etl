---
title: "cyclic_value"
---

{{< callout type="info">}}
  Header: `cyclic_value.h`  
  Since: `TBC`  
{{< /callout >}}

Provides a value that cycles between two limits.  
Supports increments, decrements and arbitrary advance.  
Supports compile time and runtime variants.  

```cpp
template <typename T, const T First = 0, const T Last = 0>
class cyclic_value
```
Static asserts if `Last` < `First`.  

**Example**

```cpp
etl::cyclic_value<int, 2, 7> value_ct; // Compile time. Fixed range of 2 to 7 inclusive.

etl::cyclic_value<int> value_rt(2, 7); // Run time. Modifiable range.
value_rt.set(3, 8);
```

## Constructor

### Run time limits

```cpp
cyclic_value<T>() noexcept;
```
Constructs a cyclic value of type `T` with an initial value of `first`.  
`constexpr` from C++11.

---

```cpp
cyclic_value<T>(T first, T last);
```
Constructs a *run time* cyclic value of type `T` with limits of `first` and `last` and an initial value of `first`.  
Asserts an `etl::cyclic_value_reversed_limits` if `last` < `first`.  
`constexpr` from C++11.

---

```cpp
cyclic_value<T>(T first, T last, T initial);
```
Constructs a cyclic value of type `T` with limits of `first` and `last` and an initial value of `initial`.  
`initial` will be clamped to be in the range `(first, last)`.  
Asserts an `etl::cyclic_value_reversed_limits` if `last` < `first`.  
`constexpr` from C++11.

### Compile time limits

```cpp
cyclic_value<T, First, Last>() noexcept;
```
Constructs a cyclic value of type `T` default constructed with an initial value of `First`.  
`constexpr` from C++11.

---

```cpp
cyclic_value<T, First, Last>(T initial) noexcept;
```
Constructs a cyclic value of type `T` constructed with an `initial`.  
`initial` will be clamped to be in the range `(First, Last)`.  
`constexpr` from C++11.

## Modifiers

```cpp
cyclic_value& operator ++() noexcept;
cyclic_value& operator ++(int) noexcept;
```
Increments the value. If the value is at the last value then is set to the first.  
`constexpr` from C++14.

---

```cpp
cyclic_value& operator --() noexcept;
cyclic_value& operator --(int) noexcept;
```
Decrements the value. If the value is at the first value then is set to the last.  
`constexpr` from C++14.

---

```cpp
template <typename TStep>
void advance(TStep n) noexcept;
```
Advances the value by the specified amount, wrapping if necessary.  
`constexpr` from C++14.

---

```cpp
template <typename TStep>
cyclic_value& operator +=(TStep n) noexcept;
```
Advances the value by the specified amount, wrapping if necessary.  
`constexpr` from C++14.

---

```cpp
template <typename TStep>
cyclic_value& operator -=(TStep n) noexcept;
```
Decrements the value by the specified amount, wrapping if necessary.  
`constexpr` from C++14.

## Access

```cpp
T get() const;
```
Gets the current value.  
`constexpr` from C++11.

---

```cpp
T first() const noexcept;
```
Gets the first value.  
`constexpr` from C++11.

---

```cpp
T last() const noexcept;
```
Gets the last value.  
`constexpr` from C++11.

---

```cpp
void set(T first, T last) noexcept;
```
Sets the new *run time* `first` and `last` values. Sets the current value to `first`.  
Asserts an `etl::cyclic_value_reversed_limits` if `last` < `first`.  
`constexpr` from C++14.

---

```cpp
void set(T value) noexcept;
```
Sets the current value.  
`constexpr` from C++14.

---

```cpp
void to_first() noexcept;
```
Sets the current value to the first value.  
`constexpr` from C++14.

---

```cpp
void to_last() noexcept;
```
Sets the current value to the last value.  
`constexpr` from C++14.

## Operations

```cpp
void swap(cyclic_value<T, FIRST, LAST>& other) noexcept;
```
Swaps with another cyclic value.  
`constexpr` from C++14.


## Operators

```cpp
operator T() noexcept;
operator const T() const noexcept;
```
Conversion operators to `T`.  
`constexpr` from C++11.

---

```cpp
cyclic_value& operator =(T t) noexcept;
```
Sets the current value to `t`.  
`constexpr` from C++14.

## Non-member functions
```cpp
template <typename T, const T FIRST, const T LAST>
void swap(cyclic_value<T, FIRST, LAST>& lhs, cyclic_value<T, FIRST, LAST>& rhs) noexcept
```
Swaps two cyclic values.

---

```cpp
template <typename T, const T FIRST, const T LAST>
bool operator == (cyclic_value<T, FIRST, LAST>& lhs, cyclic_value<T, FIRST, LAST>& rhs) noexcept
```
Checks equality of two cyclic values.  
`constexpr` from C++11.

{{< callout type="warning">}}
  After version `20.49.0` equality is judged only by the current value.  
  Previously the first and last limits were also considered.

  <= 20.49.0  
  `etl::cyclic_value<2, 7>(3) != etl::cyclic_value<2, 6>(3)`  
  
  &gt; 20.49.0  
  `etl::cyclic_value<2, 7>(3) == etl::cyclic_value<2, 6>(3)`
{{< /callout >}}

---

```cpp
template <typename T, const T FIRST, const T LAST>
bool operator != (cyclic_value<T, FIRST, LAST>& lhs, cyclic_value<T, FIRST, LAST>& rhs) noexcept
```
Checks inequality of two cyclic values.  
`constexpr` from C++11.

---

```cpp
template <typename T, const T FIRST, const T LAST>
bool operator < (cyclic_value<T, FIRST, LAST>& lhs, cyclic_value<T, FIRST, LAST>& rhs) noexcept
```
Checks if the current value of `lhs` is less than the current value of `rhs`.  
`constexpr` from C++11.

---

```cpp
template <typename T, const T FIRST, const T LAST>
bool operator <= (cyclic_value<T, FIRST, LAST>& lhs, cyclic_value<T, FIRST, LAST>& rhs) noexcept
```
Checks if the current value of `lhs` is less than or equal the current value of `rhs`.  
`constexpr` from C++11.

---

```cpp
template <typename T, const T FIRST, const T LAST>
bool operator > (cyclic_value<T, FIRST, LAST>& lhs, cyclic_value<T, FIRST, LAST>& rhs) noexcept
```
Checks if the current value of `lhs` is greater than the current value of `rhs`.  
`constexpr` from C++11.

---

```cpp
template <typename T, const T FIRST, const T LAST>
bool operator >= (cyclic_value<T, FIRST, LAST>& lhs, cyclic_value<T, FIRST, LAST>& rhs) noexcept
```
Checks if the current value of `lhs` is greater than or equal the current value of `rhs`.  
`constexpr` from C++11.
