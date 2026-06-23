---
title: "Utility"
---

{{< callout type="info">}}
  Header: `utility.h`  
{{< /callout >}}

Useful utility functions and classes.

## pair
```cpp
template <typename T1, typename T2>
struct pair
```
**Description**  
A clone of std::pair

**C++03**  
```cpp
template <typename T1, typename T2>
pair<T1, T2> make_pair(T1 a, T2 b)
```

**C++11 and above**  
```cpp
template <typename T1, typename T2>
pair<T1, T2> make_pair(T1&& a, T2&& b) 
```
**Description**  
Returns a pair.

---

```cpp
template <size_t Index, typename T1, typename T2>
struct tuple_element<Index, ETL_OR_STD::pair<T1, T2>>
```
**Description**  
Specialisation for pair.  
Gets the type in the pair at `Index`.  
Static asserts if `Index` is not `0` or `1`.  
From: `20.40.1`

---

```cpp
template <typename T1, typename T2>
struct tuple_size<ETL_OR_STD::pair<T1, T2>>
```
**Description**  
Specialisation for pair.  
Gets the size of the pair, which is always `2`.  
Since:`20.40.1`

## Template deduction guides
**C++17 and above**  

```cpp
template <typename T1, typename T2>
pair(T1, T2) ->pair<T1, T2>;
```

## exchange

```cpp
template <typename T, typename U = T>
T exchange(T& object, const U& new_value)
```
**Description**  
Copies the new value to object and returns the old value.  
Note: This is not an atomic operation.

## add_const

```cpp
template <typename T>
typename etl::add_const<T>::type& as_const(T& t)
```
**Description**  
Returns a value of type `T` as a `const T`.

## coordinate_2d

```cpp
template <typename T>
struct coordinate_2d
```

**Member types**  
`T x;`  
`T y;`

## in_place disambiguation tags.

```cpp
struct in_place_t 
```

```cpp
inline constexpr in_place_t in_place{}; // C++17
```

---

```cpp
template <typename T> struct in_place_type_t 
```

```cpp
template <typename T>
inline constexpr in_place_type_t<T> in_place_type{}; // C++17
```

---

```cpp
template <size_t I> struct in_place_index_t 
```

```cpp
template <size_t I>
inline constexpr in_place_index_t<I> in_place_index{}; // C++17
```

## declval

```cpp
template <typename T>
typename etl::add_rvalue_reference<T>::type declval() ETL_NOEXCEPT;
```
C++11

## functor
For C++11 and above.
From: `20.27.0`

```cpp
template <typename TReturn, typename... TParams>
class functor
```
**Description**  
Wraps a free/global function in a functor.

---

```cpp
constexpr functor(TReturn(*ptr_)(TParams...))
```
Constructs a functor from a function pointer.

---

```cpp
constexpr TReturn operator()(TParams... args) const
```
**Description**  
Function operator.  
Calls the wrapped function with the forwarded parameters.

### Example

```cpp
int Function(int i)
{
  return i;
}

// Note that the functor deduces the template parameters.
constexpr etl::functor f(Function);
```

---

## member_function_wrapper
For C++11 and above.
From: `20.27.0`

```cpp
template <typename TReturn, typename... TParams>
class member_function_wrapper<TReturn(TParams...)>
```
**Description**  
Wraps a member function in a static member function.

---

```cpp
template <typename T, T& Instance, TReturn(T::* Method)(TParams...)>
static constexpr TReturn function(TParams... params)
```
**Description**  
The static function that calls the member function.

### Example
```cpp
class MyClass
{
public:

  int MemberFunction(int i)
  {
    return 2 * i;
  }
};

MyClass test;

constexpr int(*pf)(int) 
        = &etl::member_function_wrapper<int(int)>::function<MyClass, test, &MyClass::MemberFunction>;

// Call
int result = pf(1);
```

## functor_wrapper
For C++11 and above.  
From: `20.27.0`

```cpp
template <typename TReturn, typename... TParams>
class functor_wrapper<TReturn(TParams...)>
```
**Description**  
Wraps a functor in a static member function.

---

```cpp
template <typename TFunctor, TFunctor& Instance>
static constexpr TReturn function(TParams... params)
```
**Description**  
The static function that calls the member function.

### Example
```cpp
class MyClass
{
public:

  int operator()(int i)
  {
    return 2 * i;
  }
};

MyClass test;

constexpr int(*pf)(int) = &etl::functor_wrapper<int(int)>::function<MyClass, test>;

// Call
int result = pf(1);
```

## functor_as_static
For C++17 and above.
20.40.0

Wraps a functor with a static free function at compile time.
Creates a static member `call` that calls the specified functor.

```cpp
template <auto& Instance>
struct functor_as_static
```

---

```cpp
template <typename... TArgs>
static constexpr auto call(TArgs&&... args)
```
**Description**  
Member static function that calls the functor

### Example
```cpp
struct Test
{
  int operator()(int i)
  {
    return 2 * i;
  }
};

Test test;

using fas_t = etl::functor_as_static<test>;
fas_t::call(1));
```

## member_function_as_static
For C++17 and above.  
From: `20.40.0`  

Wraps a member function with a static free function at compile time.  
Creates a static member function that calls the specified member function.  
```cpp
template <auto Method, auto& Instance>
struct member_function_as_static 
```

---

```cpp
template <typename... TArgs>
static constexpr auto call(TArgs&&... args)
```
**Description**  
Member static function that calls the member function.

### Example

```cpp
struct Test
{
  int Function()(int i)
  {
    return 2 * i;
  }
};

Test test;

using mfas_t = etl::functor_as_static<&Test::Function, test>;
mfas_t::call(1));
```

## member_function_as_functor
For C++17 and above.  
From: `20.40.0`  

Wraps a member function with a functor at compile time.  
Creates a functor that calls the specified member function.  

```cpp
template <auto Method, auto& Instance>
class member_function_as_functor
```

---

```cpp
template <typename... TArgs>
constexpr auto operator()(TArgs&&... args) const
```
**Description**  
Calls the functor.

### Example
```cpp
struct Test
{
  int Function()(int i)
  {
    return 2 * i;
  }
};

constexpr etl::member_function_as_functor<&Test::Function, test> mfaf;
mfaf(1);
```

## function_as_functor
For C++17 and above.  
From: `20.40.0`  

Wraps a function with a functor at compile time.  
Creates a functor that calls the specified free function.  

```cpp
template <auto Function>
class function_as_functor
```

---

```cpp
constexpr auto operator()(TArgs&&... args) const
```
**Description**  
Calls the functor.

### Example

```cpp
int Function()(int i)
{
  return 2 * i;
}

constexpr etl::function_as_functor<Function> faf;
faf(1);
```

## function_ptr_as_functor
For C++11 and above.  
From: `20.40.0`  

Wraps a function pointer with a functor at run time.  
Creates a functor that calls the specified free function.  

```cpp
template <typename TReturn, typename... TArgs>
class function_ptr_as_functor
```

---

```cpp
constexpr function_ptr_as_functor(TReturn(*ptr_)(TArgs...))
```
**Description**  
Construct from a function pointer.

---

```cpp
constexpr TReturn operator()(TArgs... args) const
```
**Description**  
Function operator.

### Example
```cpp
int Function()(int i)
{
  return 2 * i;
}

using function_type = decltype(Function);
constexpr function_type* fptr = Function;
constexpr etl::function_ptr_as_functor<function_type> fpaf(fptr)
```

## integer_sequence
From: `20.14.0`

```cpp
template <typename T, T... Integers>
class integer_sequence
```

## index_sequence
From: `20.14.0`

```cpp
template <size_t... Indices>
using index_sequence = etl::integer_sequence<size_t, Indices...>;
```
```cpp
template <size_t N>
make_index_sequence
```

## select1st & select2nd

`select1st`  
A functor object that takes a single argument, a pair, and returns the `pair::first element`.  

---

`select2nd`  
A functor object that takes a single argument, a pair, and returns the `pair::second element`.  

## size_of_type
From: `20.36.0`

```cpp
template <typename T>
ETL_CONSTEXPR size_t size_of_type()
```
**Description**  
Returns the size of the type defined in `T`.

---

```cpp
template <typename T>
ETL_CONSTEXPR size_t size_of_type(const T&)
```
**Description**  
Returns the size of the type defined in `T`.

---

```cpp
ETL_SIZE_OF_OBJECT_TYPE(Object, Type)
```
**Description**  
Returns the size of `Type` defined in the declared type of `Object`.
C++11 and above.  
From: `20.36.0`

```cpp
ETL_SIZE_OF_CLASS_TYPE(Class, Type)
```
**Description**  
Returns the size of `Type` defined in `Class`.
From: `20.36.0`

---

## nontype_t
Since `20.43.0`  

Wraps a non-type template parameter as a type.  
Defines a value associated with the template type.  

If `ETL_FORCE_CPP11_NONTYPE` is defined then the C++14 and below variant is used.

**C++14 and below**  
```cpp
template <typename T, T Value>
struct nontype_t;
```

```cpp
using FortyTwo = etl::nontype_t<int, 42>;
```

**C++17 and above**  
```cpp
template <auto Value>
struct nontype_t;
```

```cpp
using FortyTwo = etl::nontype_t<42>
```
