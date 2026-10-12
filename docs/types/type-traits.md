---
title: "type_traits"
---

Reverse engineered types traits classes from C++11 plus several ETL extensions.  
This file is generated from `type_traits_generator.h`. See Generators  
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
`is_null_pointer`  
`is_scalar`  
`is_union`  
`is_empty`  
`is_polymorphic`  
`is_abstract`  
`is_final`  
`is_aggregate`  
`is_trivial`  
`is_standard_layout`  
`is_layout_compatible`  
`is_pointer_interconvertible_base_of`  
`is_destructible`  
`is_nothrow_destructible`  
`has_virtual_destructor`  
`has_unique_object_representations`  
`is_bounded_array`  
`is_unbounded_array`  
`is_swappable`  
`is_swappable_with`  
`is_nothrow_swappable`  
`is_nothrow_swappable_with`  
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
`is_scoped_enum` `20.49.0`  
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
`is_trivially_default_constructible`  
`is_trivially_move_constructible`  
`is_trivially_destructible`  
`is_trivially_assignable`  
`is_trivially_copy_assignable`  
`is_trivially_move_assignable`  
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

## is_object_pointer  
From: `20.49.0`

```cpp
template <typename T>
struct is_object_pointer
```
Checks if `T` is a pointer to an object.  
Pointers to functions and member functions return `false`.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_object_pointer_v = etl::is_object_pointer<T>::value;
```
C++17

## is_pod
From: `20.49.0`

```cpp
template <typename T>
struct is_pod
```
Checks if `T` is a POD (Plain Old Data) type.  
The result is found in the member `value`.  
&emsp;When using the STL, this is defined as `is_standard_layout<T> && is_trivially_default_constructible<T> && is_trivially_copyable<T>`.  
&emsp;When not using the STL, this is conservatively defined as `is_fundamental<T> || is_pointer<T>`.

```cpp
template <typename T>
inline constexpr bool is_pod_v = etl::is_pod<T>::value;
```
C++17

## is_standard_layout
From: `20.49.0`

```cpp
template <typename T>
struct is_standard_layout
```
Checks if `T` is a standard-layout type.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_standard_layout_v = etl::is_standard_layout<T>::value;
```
C++17

## is_trivial
From: `20.49.0`

```cpp
template <typename T>
struct is_trivial
```
Checks if `T` is a trivial type; i.e. it is trivially copyable and has one or more
default constructors, all of which are trivial.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_trivial_v = etl::is_trivial<T>::value;
```
C++17

**Note:** When not using the STL, this trait requires compiler support (the `__is_trivial` intrinsic).
If that is not available then the trait is not defined, as there is no portable way of
determining whether a type is trivial.

## is_bounded_array
From: `20.49.0`

```cpp
template <typename T>
struct is_bounded_array
```
Checks if `T` is an array type of known bound.  
`T[N]` returns `true`, `T[]` and all other types return `false`.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_bounded_array_v = etl::is_bounded_array<T>::value;
```
C++17

## is_unbounded_array
From: `20.49.0`

```cpp
template <typename T>
struct is_unbounded_array
```
Checks if `T` is an array type of unknown bound.  
`T[]` returns `true`, `T[N]` and all other types return `false`.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_unbounded_array_v = etl::is_unbounded_array<T>::value;
```
C++17

## is_destructible
From: `20.49.0`

```cpp
template <typename T>
struct is_destructible
```
Checks if `T` is a destructible type; that is, an object type whose destructor is not deleted and is accessible, a reference type, or an array of a destructible type.  
Incomplete types, `void`, function types and arrays of unknown bound are not destructible.  
The result is found in the member `value`.  
&emsp;When not using the STL, the trait is detected by checking whether `declval<T&>().~T()` is a valid expression.  
&emsp;Requires either compiler support or C++11. When neither is available, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool is_destructible_v = etl::is_destructible<T>::value;
```
C++17

## is_nothrow_destructible
From: `20.49.0`

```cpp
template <typename T>
struct is_nothrow_destructible
```
Checks if `T` is a destructible type whose destructor is known not to throw.  
Types that are not destructible are not nothrow destructible.  
The result is found in the member `value`.  
&emsp;When not using the STL, the trait is detected by checking `noexcept(declval<T&>().~T())` for destructible types.  
&emsp;Requires either compiler support or C++11. When neither is available, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool is_nothrow_destructible_v = etl::is_nothrow_destructible<T>::value;
```
C++17

## is_swappable
From: `20.49.0`

```cpp
template <typename T>
struct is_swappable
```
Checks if `T` is a referenceable type and lvalues of type `T` can be swapped by an unqualified call to `swap`.  
The result is found in the member `value`.  
&emsp;When using the STL and C++17, this is defined as `std::is_swappable<T>`.  
&emsp;Otherwise the trait checks whether `swap(declval<T&>(), declval<T&>())` is a valid expression, where the `swap` considered is `etl::swap`, `std::swap` if it is available, and any `swap` found by argument dependent lookup.  
&emsp;Requires C++11. Without it there is no way of detecting the validity of an expression, so this trait is not defined.  
&emsp;Note that when using the STL with C++11 or C++14, the accuracy of this trait depends on `std::swap` being constrained by the standard library implementation. Standard libraries that only constrain `std::swap` from C++17 will cause this trait to report `true` for types that cannot actually be swapped.  

```cpp
template <typename T>
inline constexpr bool is_swappable_v = etl::is_swappable<T>::value;
```
C++17

## is_swappable_with
From: `20.49.0`

```cpp
template <typename T, typename U>
struct is_swappable_with
```
Checks if expressions of type `T` and `U` can be swapped by unqualified calls to `swap`, in both directions.  
The result is found in the member `value`.  
&emsp;When using the STL and C++17, this is defined as `std::is_swappable_with<T, U>`.  
&emsp;Otherwise the trait checks whether both `swap(declval<T>(), declval<U>())` and `swap(declval<U>(), declval<T>())` are valid expressions, where the `swap` considered is `etl::swap`, `std::swap` if it is available, and any `swap` found by argument dependent lookup.  
&emsp;Note that, unlike `is_swappable`, no references are added to `T` and `U`, so the value categories of the arguments are those of `T` and `U`. `is_swappable_with<int, int>::value` is `false`, whereas `is_swappable_with<int&, int&>::value` is `true`.  
&emsp;Requires C++11. Without it there is no way of detecting the validity of an expression, so this trait is not defined.  
&emsp;Note that when using the STL with C++11 or C++14, the accuracy of this trait depends on `std::swap` being constrained by the standard library implementation. Standard libraries that only constrain `std::swap` from C++17 will cause this trait to report `true` for types that cannot actually be swapped.  

```cpp
template <typename T, typename U>
inline constexpr bool is_swappable_with_v = etl::is_swappable_with<T, U>::value;
```
C++17

## is_nothrow_swappable
From: `20.49.0`

```cpp
template <typename T>
struct is_nothrow_swappable
```
Checks if `T` is a referenceable type and lvalues of type `T` can be swapped by an unqualified call to `swap`, and that the swap cannot throw.  
The result is found in the member `value`.  
&emsp;When using the STL and C++17, this is defined as `std::is_nothrow_swappable<T>`.  
&emsp;Otherwise the trait checks that `etl::is_swappable<T>::value` is `true` and that `swap(declval<T&>(), declval<T&>())` is `noexcept`.  
&emsp;Requires C++11. Without it there is no way of detecting the validity of an expression, so this trait is not defined.  
&emsp;Note that when neither the STL nor the type traits builtins are available, `etl::is_nothrow_move_constructible` and `etl::is_nothrow_move_assignable` are conservatively `false` for class types, so `etl::swap` is not `noexcept` for them and this trait is conservatively `false` too.  

```cpp
template <typename T>
inline constexpr bool is_nothrow_swappable_v = etl::is_nothrow_swappable<T>::value;
```
C++17

## is_nothrow_swappable_with
From: `20.49.0`

```cpp
template <typename T, typename U>
struct is_nothrow_swappable_with
```
Checks if expressions of type `T` and `U` can be swapped by unqualified calls to `swap`, in both directions, and that neither swap can throw.  
The result is found in the member `value`.  
&emsp;When using the STL and C++17, this is defined as `std::is_nothrow_swappable_with<T, U>`.  
&emsp;Otherwise the trait checks that `etl::is_swappable_with<T, U>::value` is `true` and that both `swap(declval<T>(), declval<U>())` and `swap(declval<U>(), declval<T>())` are `noexcept`.  
&emsp;Note that, as for `is_swappable_with`, no references are added to `T` and `U`, so the value categories of the arguments are those of `T` and `U`. `is_nothrow_swappable_with<int, int>::value` is `false`, whereas `is_nothrow_swappable_with<int&, int&>::value` is `true`.  
&emsp;Requires C++11. Without it there is no way of detecting the validity of an expression, so this trait is not defined.  
&emsp;Note that when neither the STL nor the type traits builtins are available, `etl::is_nothrow_move_constructible` and `etl::is_nothrow_move_assignable` are conservatively `false` for class types, so `etl::swap` is not `noexcept` for them and this trait is conservatively `false` too.  

```cpp
template <typename T, typename U>
inline constexpr bool is_nothrow_swappable_with_v = etl::is_nothrow_swappable_with<T, U>::value;
```
C++17

## is_union
From: `20.49.0`

```cpp
template <typename T>
struct is_union
```
Checks if `T` is a union type.  
The result is found in the member `value`.  
&emsp;When the compiler does not provide the required intrinsic, this trait conservatively reports `false` for every type.  

```cpp
template <typename T>
inline constexpr bool is_union_v = etl::is_union<T>::value;
```
C++17

## is_empty
From: `20.49.0`

```cpp
template <typename T>
struct is_empty
```
Checks if `T` is an empty class type; that is, a non-union class type with no non-static data members other than zero-sized bit-fields, no virtual functions, no virtual base classes and no non-empty base classes.  
The result is found in the member `value`.  
&emsp;Requires compiler support. When the compiler does not provide the required intrinsic, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool is_empty_v = etl::is_empty<T>::value;
```
C++17

## is_polymorphic
From: `20.49.0`

```cpp
template <typename T>
struct is_polymorphic
```
Checks if `T` is a polymorphic class type; that is, a class that declares or inherits at least one virtual function.  
The result is found in the member `value`.  
&emsp;Requires compiler support. When the compiler does not provide the required intrinsic, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool is_polymorphic_v = etl::is_polymorphic<T>::value;
```
C++17

## is_abstract
From: `20.49.0`

```cpp
template <typename T>
struct is_abstract
```
Checks if `T` is an abstract class type; that is, a class that declares or inherits at least one pure virtual function.  
The result is found in the member `value`.  
&emsp;Requires compiler support. When the compiler does not provide the required intrinsic, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool is_abstract_v = etl::is_abstract<T>::value;
```
C++17

## is_final
From: `20.49.0`

```cpp
template <typename T>
struct is_final
```
Checks if `T` is a class or union type marked `final`, optionally cv-qualified.  
The result is found in the member `value`.  
&emsp;When using the STL, this requires C++14 or above. Otherwise it requires compiler support. When neither is available, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool is_final_v = etl::is_final<T>::value;
```
C++17

## is_aggregate
From: `20.49.0`

```cpp
template <typename T>
struct is_aggregate
```
Checks if `T` is an aggregate type, optionally cv-qualified.  
An aggregate is an array type or a class type that has no user-declared or inherited constructors, no private or protected non-static data members, no virtual functions and no virtual, private or protected base classes.  
The result is found in the member `value`.  
&emsp;When using the STL, this requires C++17 or above. Otherwise it requires compiler support. When neither is available, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool is_aggregate_v = etl::is_aggregate<T>::value;
```
C++17

## has_virtual_destructor
From: `20.49.0`

```cpp
template <typename T>
struct has_virtual_destructor
```
Checks if `T` is a class type with a virtual destructor, optionally cv-qualified.  
The result is found in the member `value`.  
&emsp;When not using the STL, this requires compiler support. When it is not available, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool has_virtual_destructor_v = etl::has_virtual_destructor<T>::value;
```
C++17

## has_unique_object_representations
From: `20.49.0`

```cpp
template <typename T>
struct has_unique_object_representations
```
Checks if `T` is trivially copyable and any two objects of type `T` with the same value have the same object representation; that is, the type has no padding bits and no two distinct object representations compare equal.  
Arrays and cv-qualifiers are ignored; the trait is applied to `etl::remove_all_extents<typename etl::remove_cv<T>::type>::type`.  
The result is found in the member `value`.  
&emsp;When using the STL, this requires C++17 or above. Otherwise it requires compiler support. When neither is available, this trait is not defined.  

```cpp
template <typename T>
inline constexpr bool has_unique_object_representations_v = etl::has_unique_object_representations<T>::value;
```
C++17

## is_null_pointer
From: `20.49.0`

```cpp
template <typename T>
struct is_null_pointer
```
Checks if `T` is `etl::nullptr_t`, optionally cv-qualified.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_null_pointer_v = etl::is_null_pointer<T>::value;
```
C++17

## is_scalar
From: `20.49.0`

```cpp
template <typename T>
struct is_scalar
```
Checks if `T` is a scalar type; that is, an arithmetic, enumeration, pointer, pointer to member or `etl::nullptr_t` type, optionally cv-qualified.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_scalar_v = etl::is_scalar<T>::value;
```
C++17

## is_scoped_enum
From: `20.49.0`

```cpp
template <typename T>
struct is_scoped_enum
```
Checks if `T` is a scoped enumeration type; that is, an enumeration type that is not implicitly convertible to an integer.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_scoped_enum_v = etl::is_scoped_enum<T>::value;
```
C++17

## is_trivially_default_constructible
From: `20.49.0`

```cpp
template <typename T>
struct is_trivially_default_constructible
```
Checks if `T` is default constructible and the default construction is trivial.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_trivially_default_constructible_v = etl::is_trivially_default_constructible<T>::value;
```
C++17

## is_trivially_move_constructible
From: `20.49.0`

```cpp
template <typename T>
struct is_trivially_move_constructible
```
Checks if `T` is move constructible and the move construction is trivial.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_trivially_move_constructible_v = etl::is_trivially_move_constructible<T>::value;
```
C++17

## is_trivially_move_assignable
From: `20.49.0`

```cpp
template <typename T>
struct is_trivially_move_assignable
```
Checks if `T` is move assignable and the move assignment is trivial.  
The result is found in the member `value`.  

```cpp
template <typename T>
inline constexpr bool is_trivially_move_assignable_v = etl::is_trivially_move_assignable<T>::value;
```
C++17

## is_trivially_assignable

```cpp
template <typename T1, typename T2>
struct is_trivially_assignable
```
Checks if an expression of type `T2` can be assigned to an lvalue expression of type `T1`, and that the assignment is trivial.  
The result is found in the member `value`.  
&emsp;See Scenarios 1 to 4 above for how this trait is defined when the STL or the compiler built-ins are unavailable.  

```cpp
template <typename T1, typename T2>
inline constexpr bool is_trivially_assignable_v = etl::is_trivially_assignable<T1, T2>::value;
```
C++17

## is_layout_compatible
From: `20.49.0`

```cpp
template <typename T, typename U>
struct is_layout_compatible
```
Checks if `T` and `U` are layout-compatible types.  
The result is found in the member `value`.  
&emsp;When using the STL with C++20 or above, this is defined as `std::is_layout_compatible<T, U>`.  
&emsp;Otherwise it requires compiler support (the `__is_layout_compatible` intrinsic). When neither is available, this trait is not defined, as there is no portable way of determining layout compatibility.  

```cpp
template <typename T, typename U>
inline constexpr bool is_layout_compatible_v = etl::is_layout_compatible<T, U>::value;
```
C++17

## is_pointer_interconvertible_base_of
From: `20.49.0`

```cpp
template <typename TBase, typename TDerived>
struct is_pointer_interconvertible_base_of
```
Checks if `TBase` is a pointer-interconvertible base class of `TDerived`; that is, every object of type `TDerived` has its `TBase` base class subobject at the same address as the complete object.  
The result is found in the member `value`.  
&emsp;When using the STL with C++20 or above, this is defined as `std::is_pointer_interconvertible_base_of<TBase, TDerived>`.  
&emsp;Otherwise it requires compiler support (the `__is_pointer_interconvertible_base_of` intrinsic). When neither is available, this trait is not defined, as there is no portable way of determining pointer interconvertibility.  
&emsp;Note that, as required by the standard, the program is ill-formed if `TDerived` is a complete non-union class type and `TBase` is an incomplete class type. The trait cannot be used to probe possibly-incomplete types.  

```cpp
template <typename TBase, typename TDerived>
inline constexpr bool is_pointer_interconvertible_base_of_v = etl::is_pointer_interconvertible_base_of<TBase, TDerived>::value;
```
C++17
