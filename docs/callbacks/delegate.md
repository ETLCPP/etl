---
title: "delegate"
---

{{< callout type="info">}}
  Header: `delegate.h`  
  Since: `TBC`  
  Similar to: [std::function_ref](https://en.cppreference.com/w/cpp/utility/functional/function_ref.html)
{{< /callout >}}

`etl::delegate` is a type-safe, generic, and efficient delegate implementation. A delegate is essentially a type-safe function pointer.  

It can be used to encapsulate a callable unit (like a function, a function pointer, member function, or a callable object like a functor or a lambda function) and call it later.

`etl::delegate` is similar in some ways to `std::function_ref` introduced with C++26.

The delegate functions may be defined at compile time and/or runtime, depending on the function type.
Most delegates can only be constructed via a `create` function, except lambdas and functors, which can be created by the delegate constructor.

This may be used to implement a platform abstraction layer that allows an embedded application to interface with multiple hardware platforms.

Here's a high-level overview of how `etl::delegate` works:

**Creation**  
When you create a delegate, you specify the function signature that it should match. This is done using template parameters. For example, `etl::delegate<void(int)>` creates a delegate that can hold references to functions that take an integer as an argument and return `void`.  

**Assignment**  
You can assign a function to the delegate using the create method. The function you assign must match the delegate's function signature. For example, if you have a delegate of type `etl::delegate<void(int)>`, you could assign a function `void myFunction(int)` to it like this:  

`auto myDelegate = etl::delegate<void(int)>::create<myFunction>();`  

**Invocation**  
Once a function has been assigned to the delegate, you can call that function through the delegate just like you would call a regular function. For example, if `myDelegate` is a delegate that holds a reference to `myFunction`, you could call myFunction through the delegate like this: `myDelegate(123);`

## Notes
`<=20.21.0`  
This class only supports >=C++11.

---

`>=20.22.0`  
This class is automatically selects C++03 or C++11 versions based on the value of `ETL_USING_CPP11`  
The C++03 variant only supports delegates taking 0 or 1 parameters.

`etl::delegate` is non-owning. You cannot use lambdas that capture variables that will have gone out of scope when the delegate is called. This would result in  dangling references. Lambdas are not copied; The delegate merely keeps pointers to lambdas. This also applies to functors.

---

`>=20.40.0`  
Move constructors from lambdas or functors are disabled, thereby stopping a delegate being created from a temporary defined in a parameter.

`>=20.42.0`  
`etl::delegate` is `noexcept`  

---

`>=20.47.0`  
A delegate can be created from a run-time global function pointer, or a non-capturing lambda.

---

`>=20.22.0`  
*For C++03*  
`etl::delegate<TReturn(void)>`  
`etl::delegate<TReturn(TParam)>`

For the examples below, assume the following definitions.

```cpp
class Test
{
public:
  
  int member_function(int);
  static int member_static(int);
  int operator()(int);
  int operator()(int) const;
};

Test test;

int global(int);

auto lambda = [](int i){ return i; };
```
  
**Public types**  

```cpp
return_type    TReturn                     20.43.0
argument_types etl::type_list<TParams...>  20.43.0
```

### Run time construction

**Lambdas**  
```cpp
etl::delegate<int(int)> d(lambda);
etl::delegate<int(int)> d = lambda;
etl::delegate<int(int)> d = etl::delegate<int(int)>::create(lambda);

d.set(lambda);
```

**Functors**  
```cpp
etl::delegate<int(int)> d(test);
etl::delegate<int(int)> d = test;
etl::delegate<int(int)> d = etl::delegate<int(int)>::create(test);

d.set<test>();
```

**Member functions**  
```cpp
etl::delegate<int(int)> d = etl::delegate<int(int)>::create<Test, &Test::member_function>(test);

d.set<Test, &Test::member_function>(test);
d.set<Test, test, &Test::member_function>();
```
  
### Compile time construction

**Global functions**  
```cpp
auto d = etl::delegate<int(int)>::create<global>();
```

**Member functions** (if the instance is known at compile time)  
```cpp
auto d = etl::delegate<int(int)>::create<Test, test, &Test::member_function>(); // Deprecated
auto d = etl::delegate<int(int)>::create<Test, &Test::member_function, test>(); // New
```

**Functors** (if the instance is known at compile time)  
```cpp
auto d = etl::delegate<int(int)>::create<Test, test>();
auto d = etl::delegate<int(int)>::create<const Test, test>();
```
Note: These are disabled for GCC <= v8.1 as it generates an 'Internal Compiler Error'.

**Static member functions**  
```cpp
auto d = etl::delegate<int(int)>::create<Test::member_static>();
```

**Global functions or non-capturing lambdas**  
*From 20.47.0*
```cpp
auto d = etl::delegate<int(int)>::create(+[](int a){ return a * 2; })
```

### Constexpr

Most delegates can be declared as `constexpr`. (C++11 and above)
```cpp
static Test test;
constexpr auto d = etl::delegate<int(int)>::create<Test, test, &Test::member_function>();
```

All create functions are `constexpr` under C++14.
All create functions are `[[nodiscard]]` under C++17.

### Calling the delegate

The delegate may be called as a function with the defined parameter signature.

```cpp
etl::delegate<int(int)> d;

int r = d(3);
```

## Types

`return_type` The return type of the callback.  
`argument_types` An `etl::type_list` of the arguments.  

## Constructors

```cpp
ETL_CONSTEXPR14 delegate() ETL_NOEXCEPT
```
**Description**  
Constructs an uninitialised delegate.

---

```cpp
ETL_CONSTEXPR14 delegate(const delegate& other) ETL_NOEXCEPT
```
**Description**  
Copy constructs a delegate.

---

```cpp
delegate(TReturn (*)(TArgs...) fp) ETL_NOEXCEPT
```
**Description**  
Constructs from a run-time function pointer, or non-capturing lambda converted to a function pointer.  
*From `20.47.0`*

## Creation

```cpp
template <TReturn (*Method)(TArgs...)>
ETL_NODISCARD
static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
```
**Description**  
Create from a global function (Compile time).  

**Returns**  
A constructed delegate.

---

```cpp
ETL_NODISCARD
static delegate create(function_ptr fp) ETL_NOEXCEPT
```
**Description**  
Create from a run-time function pointer.  
*From `20.47.0`*  

**Returns**  
A constructed delegate.

---

```cpp
template <typename TLambda>
ETL_NODISCARD
static ETL_CONSTEXPR14 delegate create(TLambda& instance) ETL_NOEXCEPT
```
**Description**  
Create from lambda or functor.  

**Returns**  
A constructed delegate.

## Calling the delegate

```cpp
ETL_CONSTEXPR14 TReturn operator()(TParams... args) const ETL_NOEXCEPT
```
**Description**  
Calls the delegate.  
`constexpr` from `20.42.1`

---

20.17.0
```cpp
ETL_CONSTEXPR14 bool call_if(TParams... args) const ETL_NOEXCEPT
```
**Description**  
For delegates returning void.  
Calls the delegate if valid.  
Returns `true` if valid, otherwise `false`.  
`constexpr` from `20.42.2`

---

```cpp
ETL_CONSTEXPR14 etl::optional<TReturn> call_if(TParams... args) const ETL_NOEXCEPT
```
**Description**  
For delegates returning `TReturn`.  
Calls the delegate if valid.  
Returns a valid `etl::optional<TReturn>` containing the return value, if valid.  
`constexpr` from `20.42.2`

---

20.17.0
```cpp
template <typename TAlternative>
ETL_CONSTEXPR14 TReturn call_or(TAlternative alternative, TParams... args) const ETL_NOEXCEPT
```
**Description**  
Calls the delegate if valid, otherwise calls `alternative`.  
`constexpr` from `20.42.2`

---

```cpp
template <TReturn(*Method)(TParams...)>
ETL_CONSTEXPR14 TReturn call_or(TParams... args) const ETL_NOEXCEPT
```
**Description**  
Calls the delegate if valid, otherwise calls `Method`.

## Modifiers

```cpp
void clear() ETL_NOEXCEPT
```
**Description**  
Sets the delegate back to the uninitialised state.

## Status

```cpp
ETL_NODISCARD ETL_CONSTEXPR14 bool is_valid() const ETL_NOEXCEPT
ETL_CONSTEXPR14 operator bool() const
```
**Description**  
Returns true if the delegate has been initialised, otherwise `false`.

---

```cpp
ETL_CONSTEXPR14 operator =() ETL_NOEXCEPT
```
**Description**  
Assigns from a delegate or lambda.

---

```cpp
ETL_CONSTEXPR14 operator ==() ETL_NOEXCEPT
ETL_CONSTEXPR14 operator !=() ETL_NOEXCEPT
```
**Description**  
Compares delegates.

## Make a delegate

```cpp
make_delegate
```
**Description**  
For C++17 and above.
`20.39.5`

---

```cpp
template <auto Function>
constexpr auto make_delegate() noexcept
```
**Description**  
Make a delegate from a free function.  
Returns `etl::delegate<function_type>::create<Function>()`

---

```cpp
template <typename TLambda>
constexpr auto make_delegate(TLambda& instance) noexcept
```
**Description**  
Make a delegate from a functor or lambda function.
Returns `etl::delegate<function_type>(instance)`

---

```cpp
template <typename T, T& Instance>
constexpr auto make_delegate() noexcept
```
**Description**  
Make a delegate from a functor, compile time.  
Returns `etl::delegate<function_type>::create<T, Instance>()`

---

```cpp
template <typename T, auto Method, T& Instance>
constexpr auto make_delegate() noexcept
```
**Description**  
Make a delegate from a member function at compile time.  
Returns `etl::delegate<function_type>::create<T, Method, Instance>()`

---

```cpp
template <typename T, auto Method, const T& Instance>
constexpr auto make_delegate() noexcept
```
**Description**  
Make a delegate from a const member function at compile time.  
Returns `etl::delegate<function_type>::create<T, Method, Instance>()`

---

```cpp
template <typename T, auto Method>
constexpr auto make_delegate(T& instance) noexcept
```
**Description**  
Make a delegate from a member function at run time.  
Returns `etl::delegate<function_type>::create<T, Method>(instance)`

---

```cpp
template <typename T, auto Method>
constexpr auto make_delegate(const T& instance) noexcept
```
**Description**  
Make a delegate from a member function at run time.  
Returns `etl::delegate<function_type>::create<T, Method>(instance)`
