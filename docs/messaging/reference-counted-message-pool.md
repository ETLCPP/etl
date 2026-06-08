---
title: "reference_counted_message_pool"
---

{{< callout type="info">}}
  Header: `reference_counted_message_pool.h`  
  Header: `ireference_counted_message_pool.h`  
{{< /callout >}}

Allocates `etl::ireference_counted_message` types that are used by `etl::shared_message`.  
Uses a supplied `memory_block allocator` derived from `etl::imemory_block_allocator`.  

## ireference_counted_message_pool.h  
Defines the following class.  
`etl::ireference_counted_message_pool`  

## reference_counted_message_pool.h
Defines the following classes.  
`etl::reference_counted_message_pool_exception`
`etl::reference_counted_message_pool_allocation_failure`
`etl::reference_counted_message_pool_release_failure`


## ireference_counted_message_pool

```cpp
etl::ireference_counted_message_pool
```

The interface for reference counted message pools.  
```cpp
virtual ~ireference_counted_message_pool() {}
```
```cpp
virtual void release(const etl::ipool_message& msg) = 0;
```

Virtual `lock()` and `unlock()` functions are defined. The default action is to do nothing.  
A derived class may override these functions to provide a thread or interrupt safe pool.  
`virtual void lock()`
`virtual void unlock()`

## reference_counted_message_pool

```cpp
etl::reference_counted_message_pool<TCounter>
```
The concrete reference counted message pool.  

```cpp
reference_counted_message_pool(imemory_block_allocator& memory_block_allocator)
```
Constructs the pool and assigns the memory block allocator to it.  

---

```cpp
template <typename TMessage>
etl::reference_counted_message<TMessage, TCounter>* allocate()
```
Returns a pointer to a pool message that holds a `TMessage`.  
The message is default constructed.  
ETL_ASSERT if one cannot be allocated and returns `ETL_NULLPTR`.

---

```cpp
template <typename TMessage>
etl::reference_counted_message<TMessage, TCounter>* allocate(const TMessage& message)
```
Returns a pointer to a pool message that holds a `TMessage`.  
ETL_ASSERT if one cannot be allocated and returns `ETL_NULLPTR`.

---

```cpp
void release(const etl::ireference_counted_message& rcmessage)
```
Returns the reference counted to a pool message that holds a `TMessage`.  
`ETL_ASSERT` if it cannot be released. Reasons can include the message not belonging to the pool.

## pool_message_parameters

**C++03**  
The C++03 version defines the largest size and alignment of up to 8 types at a time.  

```cpp
template <typename TMessage1,              typename TMessage2  = TMessage1, 
          typename TMessage3  = TMessage1, typename TMessage4  = TMessage1,
          typename TMessage5  = TMessage1, typename TMessage6  = TMessage1,
          typename TMessage7  = TMessage1, typename TMessage8  = TMessage1>
struct pool_message_parameters
```

---

```cpp
static const size_t max_size;
```
The maximum size.

---

```cpp
static const size_t max_alignment;
```
The maximum alignment.

**C++11 or above**  
The C++11 version defines the largest size and alignment of a set of message types.  

```cpp
template <typename TMessage1, typename... TMessages>
struct pool_message_parameters
```

---

```cpp
static constexpr size_t max_size
```
The maximum size.  

---

```cpp
static constexpr size_t max_alignment;
```
The maximum alignment.

---

**For C++11, with atomic support.**  

```cpp
template <typename TObject>
using atomic_counted_message_pool = etl::reference_counted_message_pool<etl::atomic_int32_t>;
```
Defines an alias to a reference counted message pool that uses an atomic.
