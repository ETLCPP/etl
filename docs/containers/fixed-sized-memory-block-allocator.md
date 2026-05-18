---
title: "fixed_sized_memory_block_allocator"
---

{{< callout >}}
  Header: `fixed_sized_memory_block_allocator.h`  
{{< /callout >}}

A fixed capacity memory block pool.  
Implements an `etl::imemory_block_allocator`.

```cpp
template <size_t VBlock_Size, size_t VAlignment, size_t VSize>
class fixed_sized_memory_block_allocator : public imemory_block_allocator
```

## Template parameters

```cpp
VBlock_Size
```
The required size of each block.

---

```cpp
VAlignment
```
The required alignment of each block.

---

```cpp
VSize
```
The number of blocks.

## Constants

```cpp
size_t Block_Size = VBlock_Size;
size_t Alignment  = VAlignment;
size_t Size       = VSize;
```

