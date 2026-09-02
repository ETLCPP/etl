---
title: "multi_array"
weight: 2
---

{{< callout >}}
  Header: `multi_array.h`  
{{< /callout >}}

A fixed capacity multi-dimensional array.  

For C++11 or greater only.  

```cpp
etl::multi_array<typename T, const size_t Dx...>
```

See also:  
[array]({{< relref "array" >}})  
[array_view]({{< relref "array-view" >}})  

## Description

The `etl::multi_array` class is defined as a recursive variadic template.  
It defines a multi-dimensional array class based on nested `etl::array` definitions.

## Example

```cpp
etl::multi_array<int, 2, 3, 4>
```

is equivalent to

```cpp
etl::array<etl::array<etl::array<int, 4>, 3>, 2>
```

Each dimension of an `etl::multi_array` supports all of the members of an `etl::array`.
