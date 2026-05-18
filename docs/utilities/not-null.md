---
title: "not_null"
---

{{< callout type="info">}}
  Header: `not_null.h`  
  Since: `20.43.0`  
{{< /callout >}}

A container for pointers that are not allowed to be null.

```cpp
template <typename T>
class not_null;
```

**Specialised for pointers and etl::unique_ptr.**  

```cpp
template <typename T>
class not_null<T*> 
```

---

```cpp
template <typename T, typename TDeleter>
class not_null<etl::unique_ptr<T, TDeleter>> 
```

## Public types

```cpp
T        value_type;
T*       pointer;
const T* const_pointer;
T&       reference;
const T& const_reference;
pointer  underlying_type;
```

## Member functions
```cpp
explicit not_null(underlying_type ptr)
Constructs a not_null from an underlying type.
```
**Description**  
Asserts `etl::not_null_contains_null` if the pointer is null.

---

```cpp
not_null(const etl::not_null<T*>& other)
```
**Description**  
Copy constructor from a `not_null`.

---

```cpp
not_null& operator =(const etl::not_null<T*>& rhs) 
```
**Description**  
Assignment from a `not_null`.

---

```cpp
not_null& operator =(underlying_type rhs) 
```
**Description**  
Assignment from a pointer.  
Asserts `etl::not_null_contains_null` if the pointer is null.

---

```cpp
pointer get() const 
```
**Description**  
Gets the underlying pointer.

---

```cpp
operator pointer() const 
```
**Description**  
Implicit conversion to pointer.

---

```cpp
reference operator*() const
```
**Description**  
Dereference operator.

---

```cpp
pointer operator->() const 
```
**Description**  
Arrow operator.

---

```cpp
underlying_type& underlying() 
```
**Description**  
Gets a reference to the underlying type.

---

```cpp
const underlying_type& underlying() const 
```
**Description**  
Gets a const reference to the underlying type.
