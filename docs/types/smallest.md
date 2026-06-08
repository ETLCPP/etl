---
title: "smallest"
---

{{< callout type="info">}}
  Header: `smallest.h`  
  Since: `TBC`  
{{< /callout >}}

Allows the smallest type to be determined.  
This file is generated from smallest_generator.h. See Generators

## smallest _type
Template to determine the smallest type  
Defines type which is the type of the smallest parameter.  
Defines size which is the size of the smallest parameter.  

### C++ 03
By default, supports up to 16 types.

```cpp
template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
          typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
          typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
          typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
struct smallest_type;
```

### C++ 11 and above
```cpp
template <typename... TTypes>
struct smallest_type;
```

### C++14 and above
```cpp
template <typename... T>
using smallest_type_t = typename smallest_type<T...>::type;
```

### C++17 and above
```cpp
template <typename... T>
constexpr size_t smallest_type_v = smallest_type<T...>::size;
```

### Example
```cpp
template <typename T1, typename T2, typename T3>
struct Test
{
  // Defines smallest_t as the smallest type of T1, T2 and T3.
  typedef typename etl::smallest_type<T1, T2, T3>::type largest_t;

  enum
  {
    // Defines size as the size of the largest type.
    size = etl::largest_type<T1, T2, T3>::size;
  }
};
```

## smallest_int_for_bits
Template to determine the smallest integral type that will accommodate the required number of bits.  
Defines type which is the type of the smallest integer type.

```cpp
template <size_t N>
struct smallest_int_for_bits;
```

### C++14 and above
```cpp
template <size_t N>
using smallest_int_for_bits_t = typename smallest_int_for_bits<N>::type;
```

### Example
```cpp
// Defines type_t as the type that will accommodate 12 bits.
typedef typename etl::smallest_int_for_bits<12>::type type_t;
```

## smallest_uint_for_bits
Template to determine the smallest integral type that will accommodate the required number of bits.  
Defines type which is the type of the smallest unsigned integer type.

```cpp
template <size_t N>
struct smallest_uint_for_bits;
```

### C++14 and above
```cpp
template <size_t N>
using smallest_uint_for_bits_t = typename smallest_uint_for_bits<N>::type;
```

### Example
```cpp
// Defines type_t as the type that will accommodate 12 bits.
typedef typename etl::smallest_uint_for_bits<>::type type_t;
```

## smallest_int_for_value
Template to determine the smallest integral type that will accommodate the value.  
Defines type which is the type of the smallest integer type.

```cpp
template <intmax_t VALUE>
struct smallest_int_for_value;
```

### C++14 and above
```cpp
template <intmax_t VALUE>
using smallest_int_for_value_t = typename smallest_int_for_value<VALUE>::type;
```

### Example
```cpp
// Defines type_t as the signed type that will accommodate a value of 193.
typedef typename etl::smallest_int_for_value<193>::type type_t;
```

## smallest_uint_for_value
Template to determine the smallest unsigned integral type that will accommodate the value.  
Defines type which is the type of the smallest unsigned integer type.

```cpp
template <const uintmax_t VALUE>
struct smallest_uint_for_value;
```

### C++14 and above
```cpp
template <uintmax_t VALUE>
using smallest_uint_for_value_t = typename smallest_uint_for_value<VALUE>::type;
```

### Example
```cpp
// Defines type_t as the smallest unsigned type that will accommodate a value of 193.
typedef typename etl::smallest_uint_for_value<193>::type type_t;
```
