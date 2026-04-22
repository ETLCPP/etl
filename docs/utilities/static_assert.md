---
title: "static assert"
---

{{< callout type="info">}}
  Header: `static_assert.h`  
{{< /callout >}}

## Macro
```cpp
ETL_STATIC_ASSERT
```
Either maps on to the built-in `static_assert` or supplies a suitable definition.

```cpp
ETL_STATIC_ASSERT(condition, message);
```

## Example
```cpp
template <typename T>
void Do_This(T value)
{
  ETL_STATIC_ASSERT(etl::is_integral<T>::value, "Not an integral type");
}
```
