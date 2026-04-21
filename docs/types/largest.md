---
title: "largest"
---

{{< callout type="info">}}
  Header: `largest.h`  
  Since: `TBC`  
{{< /callout >}}

Allows the largest type or largest alignment to be determined.  
This file is generated from `largest_generator.h`. See Generators

## largest_type
Template to determine the largest type and size.  
Defines type and size of the  largest type parameter.  

### C++ 03
By default, supports up to 16 types.

```cpp
template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
          typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
          typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
          typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
struct largest_type;
```

### C++ 11 and above
```cpp
template <typename... TTypes>
struct largest_type;
```

### C++14 and above
```cpp
template <typename... T>
using largest_type_t = typename largest_type<T...>::type;
```

### C++17 and above
```cpp
template <typename... T>
constexpr size_t largest_type_v = largest_type<T...>::size;
```

### Example
```cpp
template <typename T1, typename T2, typename T3>
struct Test
{
  // Defines largest_t as the largest type of T1, T2 and T3.
  typedef typename etl::largest_type<T1, T2, T3>::type largest_t;

  enum
  {
    // Defines size as the size of the largest type.
    size = etl::largest_type<T1, T2, T3>::size;
  }
};
```

## largest_alignment
Template to determine the largest alignment.    
Defines value which is the largest alignment.

### C++ 03
```cpp
template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
          typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
          typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
          typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
struct largest_alignment;
```

### C++ 11 and above
```cpp
template <typename... TTypes>
struct largest_alignment;
```

### C++ 17 and above
```cpp
template <typename... T>
constexpr size_t largest_alignment_v = largest_alignment<T...>::size;
```

### Example
```cpp
template <typename T1, typename T2, typename T3>
struct Test
{
  // Defines ALIGNMENT with the value of the largest alignment of T1, T2 and T3.
  enum
  {
    ALIGNMENT = etl::largest_alignment<T1, T2, T3>::value
  };
};
```

## larger_int_type
Template to determine the next larger sized signed int type. The template type maybe signed or unsigned.  
If there is no larger type then the signed version of the original type is returned.

```cpp
template <typename T>
struct larger_int_type;
```

### C++14 and above
```cpp
template <typename... T>
using larger_int_type_t = typename larger_int_type<T...>::type;
```

### Example
```cpp
// Would normally define type_t as an int
typedef etl::larger_int_type<short>::type type_t; 

// Would normally define type_t as an int
typedef etl::larger_int_type<unsigned short>::type type_t; 
```

## larger_uint_type
Template to determine the next larger sized unsigned int type. The template type maybe signed or unsigned.  
If there is no larger type then the unsigned version of the original type is returned.

```cpp
template <typename T>
struct larger_uint_type;
```

### C++14 and above
```cpp
template <typename... T>
using larger_int_type_t = typename larger_int_type<T...>::type;
```

### Example
```cpp
// Would normally define type_t as an unsigned int
typedef etl::larger_uint_type<short>::type type_t; 

// Would normally define type_t as an unsigned int
typedef etl::larger_uint_type<unsigned short>::type type_t; 
```

## larger_type
Template to determine the next larger sized int type. The template type maybe signed or unsigned.  
Signed parameters define signed types, unsigned parameters define unsigned types.  
If there is no larger type then the original type is returned.

```cpp
template <typename T>
struct larger_type;
```

### C++14 and above
```cpp
template <typename... T>
using larger_type_t = typename larger_type<T...>::type;
```

### Example
```cpp
// Would normally define type_t as an int
typedef etl::larger_type<short>::type type_t;

// Would normally define type_t as an unsigned int
typedef etl::larger_type<unsigned short>::type type_t;
```

## largest
A handy combination of two of the previous templates.  
Template to determine the largest type, size and alignment.  
Defines type which is the type of the largest type parameter.  
Defines size which is the size of the largest parameter.  
Defines alignment which largest alignment of all the type parameters.  
NOTE: This may not be the alignment of the largest type.  

### C++ 03
```cpp
template <typename T1,         typename T2  = void, typename T3  = void, typename T4  = void,
          typename T5  = void, typename T6  = void, typename T7  = void, typename T8  = void,
          typename T9  = void, typename T10 = void, typename T11 = void, typename T12 = void,
          typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
struct largest;
```

### C++ 11 and above
```cpp
template <typename... TTypes>
struct largest;
```

### C++14 and above
```cpp
template <typename... T>
using largest_t = typename largest<T...>::type;
```
