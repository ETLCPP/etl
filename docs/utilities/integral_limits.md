---
title: "integral_limits"
---

{{< callout type="info">}}
  Header: `integral_limits.h`  
  Since: `TBC`  
{{< /callout >}}

A set of templated constants to determine limits of integral types at compile time.  

## Member constants
`min`        The minimum value the type can hold.  
`max`        The maximum value the type can hold.  
`bits`       The number of bits used for this type.  
`is_signed`  Whether the type is signed.

## Example
```cpp
template <const size_t SIZE>
struct Value
{
  etl::array<T, SIZE> data;
};

template <typename T>
void foo(const Value<etl::integral_limits<T>::bits>& value)
{
  size_t size = value.data.size(); // Same as the number of bits that represent a T
}
```
