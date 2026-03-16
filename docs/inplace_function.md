---
title: "inplace_function"
---

<link rel="stylesheet" href="custom.css">

# inplace_function

A small, in-place function wrapper that stores callable objects inside a fixed-size buffer (no dynamic allocation). It supports free functions, member functions, functors, and lambdas, with both runtime and compile-time bindings.

#### Supported: 20.45.0
#### Header: `inplace_function.h`

```C++
template <typename TSignature, 
          size_t Object_Size      = ETL_DEFAULT_INPLACE_FUNCTION_SIE, 
          size_t Object_Alignment = ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT>
class inplace_function;
```

The defaults are defined as follows:-  

```C++
#if !defined(ETL_DEFAULT_INPLACE_FUNCTION_SIE)
  #define ETL_DEFAULT_INPLACE_FUNCTION_SIE 32
#endif
```

```C++
#if !defined(ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT)
  #define ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT alignof(void*)
#endif
```

Set your own definitions if you require different defaults.

---

## Template Parameters

```C++
TSignature
```  
The function signature.  
e.g. `int(char, float)`

```C++
Object_Size
```
Size of the internal storage buffer.  
Defaults to `ETL_DEFAULT_INPLACE_FUNCTION_SIE`.

```C++
Object_Alignment
```
Alignment of the internal storage buffer.  
Defaults to `ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT`.

---

## Exceptions
 
```C++
etl::inplace_function_exception
```
Base exception.

```C++
etl::inplace_function_uninitialized
```
Thrown (via `ETL_ASSERT`) when invoked without a target.

---

## Member Types

```C++
function_type
```

```C++
return_type
```

```C++
argument_types
```

---

## Constructors

Default constructor.

Copy/move constructors.

Construction from a free function pointer.

Construction from an object + member function pointer (const or non-const).

Construction from a lambda/functor (const or non-const).

---

## Assignment  

Copy/move assignment.

Assignment from nullptr clears the target.

Assignment from function pointer.

Assignment from lambda/functor.

---

## Invocation

```C++
operator()
```
Invokes the bound callable and asserts if uninitialised.

```C++
call_if(...)
```
For `void` return bool indicating whether it executed.  
For non-void returns etl::optional<TReturn>.

```C++
call_or(...)
```
Invokes the target or a fallback callable.

---

## Observers

```C++
bool is_valid() const
```
Returns `true` if there is a valid callable.

```C++
explicit operator bool() const
```
Returns the result of `is_valid()`

---

## Modifiers

```C++
void clear()
```
Clears any stored callable

```C++
void swap(inplace_function& other)
```
Swaps with another inplace_function.

---

## Storage Introspection

```C++
static constexpr size_t size()
```
Returns the size of the internal storage.


```C++
static constexpr size_t alignment()
```
Returns the alignment of the internal storage.

---

## Compile-Time Binding (No Payload)
 
### Free function
```C++
template <TReturn(*Function)(TArgs...)>
void set()
```
kvbklfalkbflk  

```C++
template <TReturn(*Function)(TArgs...)>
inplace_function<TReturn(TArgs...),
                         Object_Size,
                         Object_Alignment> create()
```
kvbklfalkbflk

### Member function + instance (external linkage)

```C++
template <typename TObject, TReturn(TObject::*Method)(TArgs...), TObject& Instance>
set<TObject, &TMethod, Instance>()
```

```C++
create<TObject, &TMethod, Instance>()
```

### Callable object + instance (operator())

```C++
set<T, Instance>()
```

```C++
create<T, Instance>()
```

---

## Helper Aliases

```C++
etl::inplace_function_for<TSignature, TStorage>
```

```C++
etl::inplace_function_for_any<TSignature, T0, ...>
```

---

## Helper Factories
 
```C++
make_inplace_function(function_ptr)
```

```C++
make_inplace_function(obj, &Type::Method)
```

```C++
make_inplace_function(lambda_or_functor)
```

```C++
make_inplace_function<TSignature>(function_like)
```

C++17-only overloads also exist for compile-time binding.

---

## Example
 
```C++
#include "etl/inplace_function.h"

int add(int a, int b) { return a + b; }

struct Accumulator
{
  int base = 0;

  int add_to(int v) { return base + v; }
};

void example()
{
  etl::inplace_function<int(int, int)> f(add);
  int sum = f(1, 2);

  Accumulator acc{ 10 };

  etl::inplace_function<int(int)> g(&Accumulator::add_to, acc);

  int result = g(5);
}
```

---

## Notes

If the callable object is larger than `Object_Size` or requires stricter alignment than `Object_Alignment`, compilation fails with a `static_assert`.


`operator()` asserts when called without a target; use `call_if` or `call_or` to avoid this.


Prefer `inplace_function_for`, `inplace_function_for_any` or `make_inplace_function` to deduce storage sizes safely.

 

 

 

 

 

 

 

 

 

 

