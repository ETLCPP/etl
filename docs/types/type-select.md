---
title: "type_select"
---

Compile time mapping of id to type. When etl::type_lookup is OTT.  
Type ids must be sequential from zero.  

This file is generated from `type_select_generator.h` and can be created by running `generate_type_select.bat`.  

## type_select

Creates a class for compile time id to type.  
```cpp
etl::type_select<typename T1, ...>
```

The number of types that can be handled is determined by the generator.  
The class contains a nested template.  

### select
Gets the type from the id.  
```cpp
select<const size_t ID>
```

**Defines**  
`type` The associated type.  

**C++14 and above**  
```cpp
template <size_t ID>
using select_t = typename select<ID>::type;
```

## Examples

```cpp
enum
{
  INT32  = 0,
  STRING = 1,
  UINT8  = 2,
  OTHER  = 3
};

// Type from Id.
typedef etl::type_select<int32_t, std::string, uint8_t> Types;

// int32_t
typename Types::select_t<INT32>

// uint8_t
typename Types::select_t<UINT8>

// std::string
typename Types::select_t<STRING>

// Compile error
typename Types::select_t<OTHER>
```
