---
title: "type_traits"
---

Reverse engineered types traits classes from C++11 plus several ETL extensions.  
This file is generated from type_traits_generator.h. See Generators  
Not all traits have been defined as some rely on compiler intrinsics that are not available on all compiler platforms.  

`integral_constant`  
`remove_reference`  
`add_reference`  
`remove_pointer`  
`add_pointer`  
`is_const`  
`remove_const`  
`add_const`  
`is_volatile`  
`remove_volatile`  
`add_volatile`  
`remove_cv`  
`add_cv`  
`add_volatile`  
`remove_cvref` `20.17.0`  
`is_integral`  
`is_signed`  
`is_unsigned`  
`is_floating_point`  
`is_same`  
`is_void`  
`is_arithmetic`  
`is_fundamental`  
`is_compound`  
`is_array`  
`is_pointer`  
`is_reference`  
`is_base_of`  
`make_signed`  
`make_unsigned`  
`enable_if`  
`conditional`  
`extent`  
`remove_extent`  
`remove_all_extents`  
`rank`  
`decay`  
`alignment_of`  
`conjunction` `20.14.0`  
`disjunction` `20.14.0`  
`negation`  
`is_lvalue_assignable`  
`void_t`        `20.28.0`  
`declvar`       `20.28.0`   
`common_type`  
`is_enum`       `20.30.0`  
`underlying_type` `20.42.0`  
&emsp;Unless the ETL is set to use builtins, the user must specialise the template for their enumerations.  

The definitions will wrap those defined in C++11's `<type_traits>` if available.  

---

The following will be defined according to the C++ standard and user defined macros.

`is_assignable`  
`is_constructible`  
`is_copy_constructible`  
`is_move_constructible`  
`is_trivially_constructible`  
`is_trivially_copy_constructible`  
`is_trivially_destructible`  
`is_trivially_copy_assignable`  
`is_trivially_copyable`  

## Scenario 1
**Using C++11 or above and the STL.**  

If `ETL_CPP11_SUPPORTED and ETL_USING_STL == 1` and `!defined(ETL_USE_TYPE_TRAITS_BUILTINS)`
and `!defined(ETL_USER_DEFINED_TYPE_TRAITS)` and `((!defined(ARDUINO) && ETL_NOT_USING_STLPORT)` or `defined(ETL_GCC_V5_TYPE_TRAITS_SUPPORTED))`  

The ETL's definitions wrap the STL definitions, except in the following case.  
if `ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED` is not defined then they will be defined as in the `ETL_USER_DEFINED_TYPE_TRAITS` option.

## Scenario 2
**Not using  the STL but the compiler has type trait built-ins.**  

If the user has defined `ETL_USE_TYPE_TRAITS_BUILTINS` then the ETL will use the generally available compiler built-ins.  
This option is useful for when you are not using the STL, but are using a compatible compiler.

## Scenario 3
**Not using  the STL and have the compiler has no type trait built-ins.**  

If the user has defined `ETL_USER_DEFINED_TYPE_TRAITS` then the ETL will define these type traits for arithmetic and pointer types only. For all other types the traits will be undefined, unless the user explicitly specialises them.  

```cpp
  struct Copyable
  {
    Copyable() {}
    Copyable(const Copyable& other) {}
    Copyable& operator =(const Copyable& rhs) { return *this; }

    Copyable(Copyable&& other) = delete;
    Copyable& operator =(Copyable& rhs) = delete;
  };

  using etl::is_assignable;
  using etl::is_constructible;
  using etl::is_copy_constructible;
  using etl::is_move_constructible;

  template <>
  struct etl::is_assignable<Copyable, Copyable> : public etl::true_type
  {
  };

  template <>
  struct etl::is_constructible<Copyable> : public etl::true_type
  {
  };

  template <>
  struct etl::is_copy_constructible<Copyable> : public etl::true_type
  {
  };

  template <>
  struct etl::is_move_constructible<Copyable> : public etl::false_type
  {
  };
```

## Scenario 4
**Not using  the STL, the compiler has no type trait built-ins, and no user define specialisations are defined.**  

The ETL will define these type traits as `true` for arithmetic and pointer types only. For all other types the traits will have a value of `false`.

## is_one_of
**ETL extension**  

**C++03**  
```cpp
template <typename T,
          typename T1,         typename T2  = void, typename T3  = void, typename T4  = void, 
          typename T5 = void,  typename T6  = void, typename T7  = void, typename T8  = void, 
          typename T9 = void,  typename T10 = void, typename T11 = void, typename T12 = void, 
          typename T13 = void, typename T14 = void, typename T15 = void, typename T16 = void>
struct is_one_of
```

By default the ETL allows up the 16 types. This may be changed by running the type traits generator.  

**C++11 or above**  
```cpp
template <typename T, typename... TTypelist>
struct is_one_of
```

### Members
`value`

Set to `true` if the first template type is one of the subsequent types, otherwise `false`.

If C++17 is supported then this definition available.  
```cpp
template <typename T>
constexpr bool is_one_of_v = etl::is_one_of<T, TRest...>::value;
```

**Example**  
```cpp
bool isOK;

isOK = etl::is_one_of<int, char, short, int, long>::value;    // Sets 'isOK' to true.
isOK = etl::is_one_of<double, char, short, int, long>::value; // Sets 'isOK' to false.
```

## are_all_same
**ETL extension**  
C++11 or above  
```cpp
template <typename T, typename... TTypelist>
struct are_all_same
```

### Members
`value`  

Set to `true` if all of the template types are the same, otherwise `false`.

If C++17 is supported then this definition available.  
```cpp
template <typename T>
constexpr bool are_all_same_v = etl::is_one_of<T, TRest...>::value;
```

**Example**  
```cpp
bool isOK;

isOK = etl::are_all_same<int, int, int, int>::value;    // Sets 'isOK' to true.
isOK = etl::is_one_of<int, int, char, int>::value; // Sets 'isOK' to false.
```

## conditional_integral_constant
**ETL extension**  

### Members
`value`  

Set to the constant determined by the condition.  

**Example**  
int value;

value = etl::conditional_integral_constant<true,  1, 2>::value; // Sets value to 1
value = etl::conditional_integral_constant<false, 1, 2>::value; // Sets value to 2

## types
**ETL extension**  
Extracts the basic types from a template type.  

`type`  
The underlying type.  

---

`reference`  
A reference to the underlying type.  

---

`const_reference`  
A const reference to the underlying type.

---

`pointer`  
A pointer to the underlying type.

---

```cpp
const_pointer
```
A const pointer to the underlying type.  

---

**Example**  
```cpp
typedef const int* const MyType;

etl::types<MyType>::type                int
etl::types<MyType>::reference           int&
etl::types<MyType>::const_reference     const int&
etl::types<MyType>::pointer             int*
etl::types<MyType>::const_pointer       const int*
etl::types<MyType>::const_pointer_const const int* const
```

## unsigned_type
**ETL extension**  
From: `20.29.0`  

```cpp
template <typename T>
struct unsigned_type
```
Defines one of five unsigned types that has the same size as `T`.  
Defines one of `unsigned char`, `unsigned short`, `unsigned int`, `unsigned long`, `unsigned long long`.  

---

```cpp
template <typename T>
using unsigned_type_t = typename unsigned_type<T>::type;
```
C++11

## signed_type
**ETL extension**  
From: `20.29.0`  

```cpp
template <typename T>
struct signed_type
```
Defines one of five signed types that has the same size as `T`.  
Defines one of `char`, `short`, `int`, `long`, `long long`.  

---

```cpp
template <typename T>
using signed_type_t = typename signed_type<T>::type;
```
C++11

---

## has_duplicates
**ETL extension**  
```cpp
template <typename... TTypes>
struct has_duplicates
```
From: `20.39.1`
C++11 or above

### Members
`value`  
Set to `true` if the typelist `TTypes` contains any duplicate types, otherwise `false`.

---

If C++17 is supported then this definition is available

```cpp
template <typname... TTypes>
constexpr bool has_duplicates_v = etl::has_duplicates<TTypes...>::value;
```

## has_duplicates_of
**ETL extension**  
```cpp
template <typename T, typename... TTypes>
struct has_duplicates_of
```
From: `20.39.1`  
C++11 or above  

### Members
`value`
Set to `true` if the typelist `TTypes` contains duplicates of types `T`, otherwise `false`.

---

If C++17 is supported then this definition is available.
```cpp
template <typename T, typname... TTypes>
constexpr bool has_duplicates_of_v = etl::has_duplicates_of<T, TTypes...>::value;
```

## count_of
**ETL extension** 
```cpp 
template <typename T, typename... TTypes>
struct count_of
```
From: `20.39.1`  
C++11 or above

### Members
`value`  
The count of the occurrences of type `T` in the typelist `TTypes`.

---

If C++17 is supported then this definition is available.
```cpp
template <typename T, typname... TTypes>
constexpr bool count_of_v = etl::count_of<T, TTypes...>::value;
```

## conjunction
From: `20.40.1`  

```cpp
template <typename... Types>
struct conjunction;
```
Forms the logical conjunction (AND) of `Types...`  
The result is found in the member `value`.

```cpp
template <typename... Types>
inline constexpr bool conjunction_v;
```
Forms the logical conjunction (AND) of `Types...`  
C++17

## disjunction
From: `20.40.1`  

```cpp
template <typename... Types>
struct disjunction;
```
Forms the logical disjunction (OR) of `Types...`  
The result is found in the member `value`.

```cpp
template <typename... Types>
inline constexpr bool disjunction_v;
```
Forms the logical disjunction (OR) of `Types...`  
C++17

## exclusive_disjunction
From: `20.40.1`  

```cpp
template <typename... Types>
struct exclusive_disjunction;
```
Forms the logical exclusive disjunction (XOR) of `Types...`  
The result is found in the member `value`.

```cpp
template <typename... Types>
inline constexpr bool exclusive_disjunction_v;
```
Forms the logical exclusive disjunction (XOR) of `Types...`  
C++17

## unwrap_reference, unwrap_ref_decay 
From: `20.40.1`

```cpp
template <class T>
struct unwrap_reference;
```
Unwraps any `etl::reference_wrapper`, changing `etl::reference_wrapper<U>` to `U&`.  
If `T` is a specialisation of `etl::reference_wrapper`, unwraps it, otherwise `T` remains the same.

```cpp
template <typename T>
using unwrap_reference_t;
```
C++11

---

```cpp
template <typename T>
struct unwrap_ref_decay;
```
Unwraps any `etl::reference_wrapper`, changing `etl::reference_wrapper<U>` to `U&`.  
If decayed `T` is a specialisation of `etl::reference_wrapper`, unwraps it, otherwise `T` is decayed.

```cpp
template <typename T>
using unwrap_ref_decay_t;
```
C++11

## is_specialization
From: `20.41.0`  

```cpp
template <typename T, template <typename...> class Template>
struct is_specialization
```
Checks if `T` is a specialization of template type `Template`.  
The result is found in the member `value`.  
C++11

```cpp
template <typename T, template <typename...> class Template>
inline constexpr bool is_specialization_v = etl::is_specialization<T, Template>::value;
```
C++17
