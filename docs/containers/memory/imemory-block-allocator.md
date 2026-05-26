---
title: "imemory_block_allocator"
---

{{< callout >}}
  Header: `imemory_block_allocator.h`  
{{< /callout >}}

The base of all memory block allocators.  
Inherits from traits class `etl::successor<imemory_block_allocator>`.  

Defines the functionality and interface for derived memory block allocators.  

The class defines a public non-virtual interface and protected virtual overrides.  
A derived class must define these overrides.  

## Types

```cpp
successor_type
```

## Non-virtual public interface

```cpp
void* allocate(size_t required_size, size_t required_alignment)
```
**Description**  
Attempts to allocate a memory block of the required size (in char) and alignment and return a pointer to it.  
If the allocator is unable to do this and it has a successor, then the request will be passed on to it, otherwise `ETL_NULLPTR` will be returned.

---

```cpp
bool release(const void* const p)
```
**Description**  
Attempts to release a memory block and returns `true` if successful. 
If the allocator is unable to do this and it has a successor, then the request will be passed on to it, otherwise `false` will be returned.

---

## Virtual protected interface

```cpp
virtual void* allocate_block(size_t required_size, size_t required_alignment) = 0;
```
**Description**  
The derived class must implement this function.  
It will attempt to allocate a block of the required size. If it is unable to, it must return `ETL_NULLPTR`.  

---

```cpp
virtual bool release_block(const void* const) = 0;
```
**Description**  
The derived class must implement this function.  
It will attempt to release a block. If it is unable to, it must return `false`.

---

```cpp
void set_successor(successor_type& s)
```
**Description**  
Set the successor.

---

```cpp
successor_type& get_successor() const
```
**Description**  
Get the successor.

---

```cpp
bool has_successor() const
```
**Description**  
Returns `true` if a successor has been set.

