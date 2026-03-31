---
title: "delegate"
weight: 1
---

{{< callout type="info">}}
  Header: `delegate.h`  
  Supported: `20.45.0`  
  Similar to: [std::function](https://en.cppreference.com/w/cpp/utility/functional/function.html)
{{< /callout >}}

A small, in-place function wrapper that stores callable objects inside a fixed-size buffer (no dynamic allocation). It supports free functions, member functions, functors, and lambdas, with both runtime and compile-time bindings.

```cpp
template <typename TSignature,
          size_t Object_Size      = ETL_DEFAULT_INPLACE_FUNCTION_SIZE,
          size_t Object_Alignment = ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT>
class inplace_function;
```

The defaults are defined as follows:-

```cpp
#if !defined(ETL_DEFAULT_INPLACE_FUNCTION_SIZE)
  #define ETL_DEFAULT_INPLACE_FUNCTION_SIZE 32
#endif
```

```cpp
#if !defined(ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT)
  #define ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT alignof(void*)
#endif
```

Set your own definitions if you require different defaults.

## Template Parameters

```cpp
TSignature
```
The function signature. e.g. `int(char, float)`  

---

```cpp
Object_Size
```
Size of the internal storage buffer.  
Defaults to `ETL_DEFAULT_INPLACE_FUNCTION_SIZE`.

---

```cpp
Object_Alignment
```
Alignment of the internal storage buffer.  
Defaults to `ETL_DEFAULT_INPLACE_FUNCTION_ALIGNMENT`.

## Exceptions

```cpp
etl::inplace_function_exception
```
Base exception.

---

```cpp
etl::inplace_function_uninitialized
```
Thrown (via `ETL_ASSERT`) when invoked without a target.

---

## Member Types

```cpp
function_type
```

```cpp
return_type
```

```cpp
argument_types
```

## Constructors

**Description**  
Default constructor.

**Description**  
Copy/move constructors.

**Description**  
Construction from a free function pointer.

**Description**  
Construction from an object + member function pointer (const or non-const).

**Description**  
Construction from a lambda/functor (const or non-const).

## Assignment

**Description**  
Copy/move assignment.

**Description**  
Assignment from nullptr clears the target.

**Description**  
Assignment from function pointer.

**Description**  
Assignment from lambda/functor.

## Invocation

```cpp
operator()
```
**Description**  
Invokes the bound callable and asserts if uninitialised.

**Parameters**  

**Returns**  

---

```cpp
call_if(...)
```
**Description**  
Calls the delegate function, if valid.  
Indicates success in the return type.  

**Parameters**  
`TArgs...`  

**Returns**  
For `void` return `bool` indicating whether it executed.  
For non-void returns `etl::optional<TReturn>`.

---

```cpp
call_or(...)
```
**Description**  
Invokes the target or a fallback callable.

## Observers

```cpp
bool is_valid() const
```
**Returns**  
Returns `true` if there is a valid callable.

---

```cpp
explicit operator bool() const
```
**Description**  

**Parameters**  

**Returns**  
Returns the result of `is_valid()`

## Modifiers

```cpp
void clear()
```
**Description**  
Clears any stored callable.

---

```cpp
void swap(inplace_function& other)
```
**Description**  
Swaps with another inplace_function.

---

## Storage Introspection

```cpp
static constexpr size_t size()
```
**Returns**  
Returns the size of the internal storage.

---

```cpp
static constexpr size_t alignment()
```
**Returns**  
Returns the alignment of the internal storage.

## Compile-Time Binding (No Payload)

### Free function

```cpp
template <TReturn(*Function)(TArgs...)>
void set()
```
Sets the callable to `Function`.

---

```cpp
template <TReturn(*Function)(TArgs...)>
inplace_function<TReturn(TArgs...),
                         Object_Size,
                         Object_Alignment> create()
```
**Description**  
Creates an `etl::inplace_function` using `Function`.

### Member function + instance (external linkage)

```cpp
template <typename TObject, TReturn(TObject::*Method)(TArgs...), TObject& Instance>
set<TObject, &TMethod, Instance>()
```
**Description**  
Sets the callable to the member function `Method` for the object `Instance`.

---

```cpp
create<TObject, &TMethod, Instance>()
```
**Description**  

### Callable object + instance (operator())

```cpp
set<T, Instance>()
```
**Description**  

```cpp
create<T, Instance>()
```
**Description**  

## Helper Aliases

```cpp
etl::inplace_function_for<TSignature, TStorage>
```
**Description**  

```cpp
etl::inplace_function_for_any<TSignature, T0, ...>
```
**Description**  

## Helper Factories

```cpp
make_inplace_function(function_ptr)
```
**Description**  

```cpp
make_inplace_function(obj, &Type::Method)
```
**Description**  

```cpp
make_inplace_function(lambda_or_functor)
```
**Description**  

```cpp
make_inplace_function<TSignature>(function_like)
```
**Description**  

C++17-only overloads also exist for compile-time binding.

## Example

```cpp
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

## Notes

If the callable object is larger than `Object_Size` or requires stricter alignment than `Object_Alignment`, compilation fails with a `static_assert`.


`operator()` asserts when called without a target; use `call_if` or `call_or` to avoid this.

Prefer `inplace_function_for`, `inplace_function_for_any` or `make_inplace_function` to deduce storage sizes safely.





















