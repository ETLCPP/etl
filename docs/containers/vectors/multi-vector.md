---
title: "multi_vector"
weight: 2
---

{{< callout type="info">}}
  Header: `multi_vector.h`  
{{< /callout >}}

A fixed capacity multi-dimensional vector.  

For C++11 or greater only.  

```cpp
etl::multi_vector<typename T, const size_t Dx...>
```

## Description

The `etl::multi_vector` class is defined as a recursive variadic template.  
It defines a multi-dimensional array class based on nested `etl::vector` definitions.

## Example

```cpp
etl::multi_vector<int, 2, 3, 4>
```

is equivalent to

```cpp
etl::vector<etl::vector<etl::vector<int, 4>, 3>, 2>
```

Each dimension of an `etl::multi_vector` supports all of the members of an `etl::vector`.
