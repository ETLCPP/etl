---
title: "ipool"
weight: 1
---

{{< callout >}}
  Header: `ipool.h`  
{{< /callout >}}

The base of all fixed capacity object pools.  

## Exceptions classes

```cpp
class pool_exception : public exception
```
**Description**  
The base class for pool exceptions.
  
---
  
```cpp
class pool_no_allocation : public pool_exception
```
**Description**  
The exception thrown when the pool has no more free items.
  
---

```cpp
class pool_object_not_in_pool : public pool_exception
```
**Description**    
The exception thrown when an object is released which does not belong to the pool.

---
  
```cpp
class pool_element_size : public pool_exception
```
**Description**    
The exception thrown when an the type requested is larger than the element size.

## Member Types

`size_type = size_t`

## Iterators

```cpp
ipool_iterator
```
**Description**  
Supports increment, dereference and comparison.

---

```cpp
const_ipool_iterator
```
**Description**  
Supports increment, dereference and comparison.

## Member funcctions

```cpp
iterator begin()
```

---

```cpp
iterator end()
```

---

```cpp
const_iterator begin() const
```

---

```cpp
const_iterator end() const
```

---

```cpp
const_iterator cbegin() const
```

---

```cpp
const_iterator cend() const
```

---

```cpp
template <typename T>
T* allocate()
```
**Description**  
Allocates storage for an object from the pool.  
If asserts or exceptions are enabled and there are no more free items an `etl::pool_no_allocation` if emitted, otherwise a null pointer is returned.

---

**C++03**  
```cpp
template <typename T>
T* create()
```
**Description**  
Emplace with no parameters.

---

```cpp
template <typename T, typename T1>
T* create(const T1& value1)
```
**Description**  
Emplace with 1 parameter.

---

```cpp
template <typename T, typename T1, typename T2>
T* create(const T1& value1, const T2& value2)
```
**Description**  
Emplace with 2 parameters.

---

```cpp
template <typename T, typename T1, typename T2, typename T3>
T* create(const T1& value1, const T2& value2, const T3& value3)
```
**Description**  
Emplace with 3 parameters.

---

```cpp
template <typename T, typename T1, typename T2, typename T3, typename T4>
T* create(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
Emplace with 4 parameters.

---

**C++11 and above**  
```cpp
template <typename T, typename... Args>
T* create(Args&&... args)
```
**Description**  
Emplace with variadic constructor parameters.

---

```cpp
    template <typename T>
    void destroy(const T* const p_object)
```
**Description**  
Destroys the object.  
Undefined behaviour if the pool does not contain a `T`.  

**Parameters**  
`p_object` A pointer to the object to be destroyed.

---

```cpp
void release(const void* const p_object)
```
**Description**  
Release an object in the pool.  
If asserts or exceptions are enabled and the object does not belong to this pool then an `etl::pool_object_not_in_pool` is thrown. 

**Parameters**  
`p_object` A pointer to the object to be released.

---

```cpp
void release_all()
```
**Description**  
Release all objects in the pool.

---

```cpp
bool is_in_pool(const void* const p_object) const
```
**Description**  
Check to see if the object belongs to the pool.  

**Parameters**  
`p_object` A pointer to the object to be checked.  

**Return**  
`true` if it does, otherwise `false`.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum number of items in the pool.

---

```cpp
size_t max_item_size() const
```
**Description**  
Returns the maximum size of an item in the pool.

---

```cpp
size_t capacity() const
```
**Description**  
Returns the maximum number of items in the pool.

---

```cpp
size_t available() const
```
**Description**  
Returns the number of free items in the pool.

---

```cpp
size_t size() const
```
**Description**  
Returns the number of allocated items in the pool.

---

```cpp
bool empty() const
```
**Description**  
Checks to see if there are no allocated items in the pool.

**Return**  
`true` if there are none allocated.

---

```cpp
bool full() const
```
**Description**  
Checks to see if there are no free items in the pool.

**Return**  
`true` if there are none free.
