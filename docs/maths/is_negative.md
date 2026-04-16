---
title: "is_negative"
---

Allows a parameter of type `T` to be checked for a positive or negative value, while eliminating compiler warnings for checking `< 0` for unsigned types.

```cpp
template <typename T>
ETL_CONSTEXPR bool is_negative(const T value)
```
**Return**  
`true` if the value is negative, otherwise `false`.
