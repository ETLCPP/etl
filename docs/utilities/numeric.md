---
title: "numeric"
---

{{< callout type="info">}}
  Header: `numeric.h`  
  Since: `TBC`  
{{< /callout >}}

Numeric functions reverse engineered from C++ 11.

## iota
```cpp
template <typename TIterator, typename T>
void iota(TIterator first, TIterator last, T value);
```
**Description**  
Fills a range of elements with sequentially increasing values starting with `value`.  
See [std::iota](https://en.cppreference.com/cpp/algorithm/iota_)


## lerp
```cpp
template <typename T>
T lerp(T a, T b, T t) ETL_NOEXCEPT
```
**Description**  
Linear interpolation.  
Returns the result of `a + t(b − a)`.

## midpoint
```cpp
template <typename T>
ETL_CONSTEXPR T midpoint(T a, T b)
```
**Description**  
Calculates the midpoint of integers or floating-point numbers.

---

```cpp
template <typename T>
ETL_CONSTEXPR T midpoint(T* a, T* b)
```
**Description**  
Calculates the midpoint of pointers.
