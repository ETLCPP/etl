---
title: "generic_pool"
weight: 2
---

{{< callout >}}
  Header: `generic_pool.h`  
{{< /callout >}}

A fixed capacity object pool, where allocation and release are O(1) operations.

**Internally defined storage**  
```cpp
etl::generic_pool<const size_t Type_Size, size_t Alignment, size_t Size>
```

**Externally defined storage**  
```cpp
etl::generic_pool_ext<size_t Type_Size, size_t Alignment>
```

---

`etl::generic_pool` inherits from `etl::ipool`
etl::ipool may be used as a size and type independent pointer or reference type for any `etl::generic_pool` instance.

---

`ipool.h` defines `etl::ipool`.
`generic_pool.h` defines `etl::generic_pool`.

**Notes**  
When using `etl::generic_pool`, `Type_Size` must be set to the size of the largest type to be stored, and `Alignment` set to the largest alignment of all of the types to be stored.  
This may be achieved using `etl::largest`.  

```cpp
typedef etl::largest<uint8_t, uint32_t, double> largest;
etl::generic_pool<largest::size, largest::alignment, 4> pool;
```

**Notes**  
There are two methods for allocating objects from the pool.  

```cpp
allocate
release
```
`allocate` does not construct. It merely provides access to memory that is sized and aligned to contain a `T` object. The programmer must use placement `new` to construct the object.  
`release` returns the memory allocation to the to the pool.

```cpp
create
destroy
```
`create` allocates memory from the pool and calls its constructor.  
`destroy` will call the destructor for the object and release it back to the pool.

## Constructors

```cpp
generic_pool()
```
**Description**  
For `etl::generic_pool`.  
Constructs a generic_pool.  
No elements are constructed.

---

```cpp
generic_pool_ext(char* buffer, size_t size)
```
**Description**  
For `etl::generic_pool_ext`.
Constructs a generic_pool from an external bufffer.  
No elements are constructed.

## Operations

```cpp
template <typename T>
T* allocate()
```
**Description**  
Allocates an item from the pool and returns a pointer to it.  
If the pool has no free items then an `etl::pool_no_allocation()` is emitted and a `nullptr` is returned.  
**Note:** Does not call the object's constructor.

---

```cpp
void release(const void* const p_object);
```
**Description**  
Releases an object back to the pool.  
If the object does not belong to the pool an `etl::pool_object_not_in_pool()` is emitted.  
**Note:** Does not call the object's destructor.

---

```cpp
void release_all();
```
Releases all objects back to the pool.  
**Note:** Does not destruct any `T` objects.

---

```cpp
bool is_in_pool(const T* const p_object) const;
```
**Description**  
Checks to see if an object belongs to the pool.  
Returns `true` if it does, otherwise `false`.

---

**C++03**  
**Description**  
```cpp
template <typename T>
T* create()
```

```cpp
template <typename T, typename T1>
T* create(const T1& value1)
```

```cpp
template <typename T, typename T1, typename T2>
T* create(const T1& value1, const T2& value2)
```

```cpp
template <typename T, typename T1, typename T2, typename T3>
T* create(const T1& value1, const T2& value2, const T3& value3)
```

```cpp
template <typename T, typename T1, typename T2, typename T3, typename T4>
T* create(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```

---

**C++11**  
```cpp
template <typename T, typename... Args>
T* create(Args&&... args)
```

---

There is a matching destroy function.  

```cpp
template <typename T>
void destroy(const void* const p_object)
```

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns `true` if there are no allocated objects in the pool, otherwise `false`.

---

```cpp
bool full() const
```
**Description**  
Returns `true` if there are no free objects in the pool, otherwise `false`.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available free objects in the pool.

---

```cpp
size_t size() const
```
**Description**  
Returns the number of allocated objects in the pool.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum number of objects in the pool.

## Constants

`TYPE_SIZE`  The size of an item in the pool.  
`SIZE`       The maximum number of items in the pool.  
`ALIGNMENT`  The alignment of items in the pool.  

## The Technical Bit

The pool is based around a block of memory, with storage for Size items, properly aligned for type `T`.  
Each item in the pool is a `union` of a `uintptr_t` and a type `T`. Free items contain a pointer to the next free item. Allocated items contain a `T` value.  

Allocation is quick, as all that is necessary is to return the address of the next free item.  

Release is similarly quick, as the item's content is simply replaced with the address of the current next free item.  

On first use the memory block is uninitialised. On each new allocation a new item is initialised with the address of the next free item. This just-in-time initialisation means that construction does not involve writing to a potentially large amount of memory in one go.
