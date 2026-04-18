---
title: alignment
---

A way of aligning memory storage through template parameters.

## type_with_alignment
Returns a fundamental type that has the same alignment as that specified in the template parameter.

```cpp
template <const size_t ALIGNMENT>
class type_with_alignment
```
**Example**  
```cpp
using type_t = etl::type_with_alignment<4>::type;
```

## aligned_storage
Creates a memory store of the specified length at the specified alignment.

```cpp
template <const size_t LENGTH, const size_t ALIGNMENT>
struct aligned_storage;
```

**Example**  
```cpp
// Creates aligned storage of length 100 at an alignment of 8.
etl::aligned_storage<100, 8>::type storage;
```

The class defines various conversion operators for ease of use.

Conversions are supplied to `T&`, `const T&`, `T*`, `const T*`, plus explicit get_address and get_reference member functions.

```cpp
aligned_storage_as
```
Creates a memory store of the specified length at the same alignment as the specified type.

```cpp
template <const size_t LENGTH, typename T>
struct aligned_storage_as;
```

**Example**  
```cpp
// Creates aligned storage of length 100 at an alignment of a double.
etl::aligned_storage_as<100, double>::type storage;
```

## typed_storage
Since: `20.40.1`

```cpp
template <typename T>
class typed_storage;
```
Wrapper class that provides a memory area and lets the user create an instance of `T` in this memory at runtime.  
This class also erases the destructor call of `T`, i.e. if typed_storage goes out of scope, the destructor if the wrapped type will not be called. This can be done explicitly by calling `destroy()`. 

```cpp
T        value_type
T&       reference
const T& const_reference
T*       pointer
const T* const_pointer
```

---

```cpp
typed_storage()
```
**Description**  
Constructor

---

```cpp
~typed_storage() = default;
```
**Description**  
Defaulted destructor which will NOT call the destructor of the object which was created by calling `create()`.

---

```cpp
void destroy()
```
**Description**  
Calls the destructor of the wrapped object and asserts if `has_value()` is `false`.

---

```cpp
bool has_value() const
```
**Returns**  
`true` if object has been constructed using `create()`, otherwise `false`.

---

```cpp
template <typename... Args>
reference create(Args&&... args)
```
**Description**  
Constructs the instance of `T` forwarding the given args to its constructor and asserts `etl::typed_storage_error` if `has_value()` is `false`.  
Returns the instance of `T` which has been constructed in the internal byte array.

---

```cpp
pointer operator->()
```
**Return**  
A pointer of type `T` and asserts `etl::typed_storage_error` if `has_value()` is `false`.

---

```cpp
const_pointer operator->() const
```
**Return**  
A const pointer of type `T` and asserts `etl::typed_storage_error` if `has_value()` is `false`.

---

```cpp
reference operator*()
```
**Return**  
Reference of type `T` and asserts if `etl::typed_storage_error` if `has_value()` is `false`.

---

```cpp
const_reference operator*() const
```
**Return**  
Const reference of type `T` and asserts `etl::typed_storage_error` if `has_value()` is `false`.

## is_aligned
Since: `20.35.12`

```cpp
bool is_aligned(void* p, size_t alignment)
```
**Description**  
Check that `p` has `alignment`.

```cpp
template <size_t Alignment>
bool is_aligned(void* p)
```
**Description**  
Check that `p` has `Alignment`.

---

```cpp
template <typename T>
bool is_aligned(void* p)
```
**Description**  
Check that `p` has the alignment of `T`.

## alignment_exception
Since: `20.35.12`
Exception base for alignment

---

## alignment_error
Since: `20.35.12`
Memory misalignment exception.
