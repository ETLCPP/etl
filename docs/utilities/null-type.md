---
title: "null_type"
---

{{< callout type="info">}}
  Header: `null_type.h`  
  Since: `TBC`  
{{< /callout >}}

A placeholder type to use as unique default types in a list of template type parameters.

```cpp
template <size_t N>
class null_type
{
};
```

## Example
```cpp
template <typename T1 = etl::null_type<1>, typename T2 = etl::null_type<2>>
class MyClass
```
