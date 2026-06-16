---
title: "nth_type"
---

{{< callout type="info">}}
  Header: `nth_type.h`  
  Since: `TBC`  
{{< /callout >}}

Defines the nth type in a variadic list of types.

## Member types
`type`      Defined as the nth type in the type list.  
`type_list` An etl::type_list of all of the types.  

Static asserts if N is out of range.

## Classes
```cpp
template <size_t Index, typename... TTypes>
struct nth_type
```

---

```cpp
template <size_t Index, typename... TTypes>
struct nth_type<Index, etl::type_list<TTypes...>>
```

---

```cpp
template <size_t Index, typename... TTypes>
using nth_type_t = typename nth_type<Index, TTypes...>::type;
```
