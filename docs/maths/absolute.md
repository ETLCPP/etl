---
title: "absolute"
---

A template functions that return the absolute value.

```cpp
template <typename T>
ETL_CONSTEXPR T absolute(T value)
```
**Return**  
The absolute of `value`.

If T is `int8_t`:-
`0` => `0`
`127` => `127`
`-127` => `127`

**Error**
Asserts if `value == etl::integral_limits<T>::min`.

---

```cpp
template <typename T>
ETL_CONSTEXPR etl::make_unsigned_t<T> absolute_unsigned(T value)
```
**Return**  
The absolute of `value`, cast to the unsigned version of type `T`.

If T is `int8_t`:-

Return type is `etl::make_unsigned_t<T>`
`0` => `0`
`127` => `127`
`-127` => `127`
`-128` => `128`
