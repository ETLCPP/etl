---
title: "bip_buffer_spsc_atomic"
---

---

{{< callout type="">}}
  Header: `bip_buffer_spsc_atomic.h`  
  Supported: tbc
{{< /callout >}}

A fixed capacity bipartite buffer.  

A bipartite buffer is a memory buffer design that uses two contiguous halves of the same underlying memory block to create a seamless, wraparound-free view of a circular/ring buffer's contents.

**The Problem It Solves**  
Ring buffers are efficient for streaming data, but when data wraps around the end of the buffer back to the beginning, reading a contiguous chunk across that boundary normally requires two separate reads or a temporary copy.

**How It Works**  
A bipartite buffer allocates a backing array that is twice the needed capacity. The two halves (the "bipartite" part) are mapped to the same physical memory, so:

The first region covers indices 0 to N-1
The second region covers indices N to 2N-1, but maps to the same memory as the first

This means any contiguous slice of up to N bytes can always be read as a single, linear pointer — no wraparound, no copying, no splitting.

```C++
etl::bip_buffer_spsc_atomic<typename T, 
                            size_t SIZE, 
                            size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
```

Inherits from `etl::ibip_buffer_spsc_atomic<T>`.  

`etl::ibip_buffer_spsc_atomic` may be used as a size independent pointer or reference type for any `etl::bip_buffer_spsc_atomic instance`.

## Member types

`value_type`  
`size_type`  
`reference`  
`const_reference`  
`rvalue_reference`  

## Static Constants

`MAX_SIZE`  The maximum size of the circular_buffer.

## Constructor
```C++
etl::bip_buffer_spsc_atomic<typename T, 
                            size_t SIZE, 
                            size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>();
```

## Capacity

```C++
bool empty() const
```
**Description**  
Returns `true` if the size of the circular buffer is zero, otherwise `false`.

---

```C++
bool full() const
```
**Description**  
Returns `true` if the size of the circular buffer is `SIZE`, otherwise `false`.

---

```C++
size_t size() const
```
**Description**  
Returns the size of the circular buffer.

---

```C++
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the circular buffer.

---

```C++
size_t capacity() const
```
**Description**  
Returns the maximum possible size of the circular buffer.

---

```C++
size_t available() const
```
**Description**  
Returns the remaining available capacity in the circular buffer.

## Modifiers

```C++
etl::span<T> read_reserve(size_type max_reserve_size = numeric_limits<size_type>::max())
```
**Description**  
Reserves a memory area for reading (up to the `max_reserve_size`).

```C++
void read_commit(const etl::span<T> &reserve)
```
**Description**  
Commits the previously reserved read memory area
the reserve can be trimmed at the end before committing.
Throws `bip_buffer_reserve_invalid`.

---

```C++
etl::span<T> write_reserve(size_type max_reserve_size)
```
**Description**  
Reserves a memory area for writing up to the `max_reserve_size`.

---

```C++
etl::span<T> write_reserve_optimal(size_type min_reserve_size = 1U)
```
**Description**  
Reserves an optimal memory area for writing. The buffer will only wrap
around if the available forward space is less than `min_reserve_size`.

---

```C++
void write_commit(const etl::span<T> &reserve)
```
**Description**  
Commits the previously reserved write memory area
the reserve can be trimmed at the end before committing.
Throws `bip_buffer_reserve_invalid`

---

```C++
void clear()
```
**Description**  
Clears the buffer, destructing any elements that haven't been read.

