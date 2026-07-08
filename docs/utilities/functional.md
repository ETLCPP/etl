---
title: "functional"
---

{{< callout type="info">}}
  Header: `functional.h`  
{{< /callout >}}

Defines functional classes from the STL.

## reference_wrapper

`etl::reference_wrapper` is a class template that wraps a reference in a copyable, assignable object.  
A clone of `std::reference_wrapper`.

## unary_function

```cpp
template <typename TArgumentType, typename TResultType>
struct unary_function
```

### Typedefs
```cpp
TArgumentType argument_type
TResultType   result_type
```

## binary_function

```cpp
template <typename TFirstArgumentType, typename TSecondArgumentType, typename TResultType>
struct binary_function
```

### Typedefs
```cpp
TFirstArgumentType  first_argument_type
TSecondArgumentType second_argument_type
TResultType         result_type
```

## bind1st

```cpp
template <typename TFunction>
class binder1st
```

---

```cpp
template <typename F, typename T>
binder1st<F> bind1st(const F& f, const T& x)
```
**Description**  
Binds a given argument to a first parameter of the given binary function object.

## bind2nd

```cpp
template <typename TFunction>
class binder2nd
```

---

```cpp
template <typename F, typename T>
binder2nd<F> bind2nd(const F& f, const T& x)
```
**Description**  
Binds a given argument to a second parameter of the given binary function object.

## Less

```cpp
template <typename T = void>
struct less
```
**Description**  
A function object that performs a strictly less-than comparison `a < b`) between two values.

---

```cpp
template <>
struct less<void> 
```
**Description**  
A function object that performs a strictly less-than comparison `a < b`) between two values.  
Transparent comparator.  
Defines `is_transparent`.  
From: `20.21.0`

## less_equal

```cpp
template <typename T = void>
struct less_equal
```
**Description**  
A function object that performs a strictly less-than-equal comparison `a <= b`) between two values.  

---

```cpp
template <>
struct less_equal<void>
```
**Description**  
A function object that performs a strictly less-than-equal comparison `a <= b`) between two values.  
Transparent comparator. 
Defines `is_transparent`.
From: `20.21.0`

## greater

```cpp
template <typename T = void>
struct greater
```
**Description**  
A function object that performs a strictly greater-than comparison `a > b`) between two values.  

---

```cpp
template <>
struct greater<void>
```
**Description**  
A function object that performs a strictly greater-than comparison `a > b`) between two values.  
Transparent comparator.  
Defines `is_transparent`.
From: `20.21.0`

## greater_equal

```cpp
template <typename T = void>
struct greater_equal
```
**Description**  
A function object that performs a strictly greater-than-equal comparison `a >= b`) between two values.  

---

```cpp
template <>
struct greater_equal<void>
```
**Description**  
A function object that performs a strictly greater-than-equal comparison `a >= b`) between two values.  
Transparent comparator.  
Defines `is_transparent`.
From: `20.21.0`

## equal_to

```cpp
template <typename T = void>
struct equal_to
```
**Description**  
A function object that performs a strictly equal-to comparison `a == b`) between two values.  

---

```cpp
template <>
struct equal_to<void>
```
**Description**  
A function object that performs a strictly equal-to comparison `a == b`) between two values.  
Transparent comparator.  
Defines `is_transparent`.  
From: `20.21.0`

## not_equal_to

```cpp
template <typename T = void>
struct not_equal_to
```
**Description**  
A function object that performs a strictly not-equal-to comparison `a !== b`) between two values.  

---

```cpp
template <>
struct not_equal_to<void>
```
**Description**  
A function object that performs a strictly not-equal-to comparison `a !== b`) between two values.  
Transparent comparator.  
Defines `is_transparent`.  
From: `20.21.0`

## mem_fn

```cpp
template <typename TReturnType, typename TClassType, typename... TArgs>
<functor> mem_fn(TReturnType(TClassType::* member_function)(TArgs...))
```
**Description**  
Returns a functor that wraps a member function of `TClassType`, taking TArgs and returning `TReturnType`.  
From: `20.38.11`

---

```cpp
template <typename TReturnType, typename TClassType, typename... TArgs>
<functor> mem_fn(TReturnType(TClassType::* member_function)(TArgs...) const)
```
**Description**  
Returns a functor that wraps a const member function of `TClassType`, taking TArgs and returning `TReturnType`.  
From: `20.38.11`
