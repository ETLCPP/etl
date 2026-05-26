---
title: "queue_spsc_atomic"
---

A fixed capacity single-producer, single-consumer queue for multi-threaded and interrupt driven systems.  
The queue may be used to transfer data to or from two threads or an ISR.

The queue makes use of `etl::atomic_size_t`.

```cpp
etl::queue_spsc_atomic<typename T,
                       const size_t SIZE, 
                       const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
```

Inherits from `etl::iqueue_spsc_atomic<T>`. 
`etl::iqueue_spsc_atomic` may be used as a size independent pointer or reference type for any `etl::queue_spsc_atomic` instance of the same implementation.  

The memory model determines the type used internally for indexes and size, to allow for the most efficient implementation for the application.

## Maximum queue sizes
```cpp
MEMORY_MODEL_SMALL   255
MEMORY_MODEL_MEDIUM  65535
MEMORY_MODEL_LARGE   2147483647
MEMORY_MODEL_HUGE    9223372036854775807
```

See `memory_model.h`

## Member types

```cpp
value_type      T
size_type       <based on memory model>
pointer         value_type*
const_pointer   const value_type*
reference       value_type&
const_reference const value_type&
```

## Constructor

```cpp
queue_spsc_atomic()
```

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the queue is zero, otherwise false.
Accurate from the 'pop' thread.
'Not empty' is a guess from the 'push' thread.

---

```cpp
bool full() const
```
**Description**  
Returns true if the size of the queue is SIZE, otherwise false.
Accurate from the 'push' thread.
'Not full' is a guess from the 'pop' thread.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the queue.
Due to concurrency, this is a guess.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the queue.
Due to concurrency, this is a guess.

---

```cpp
ETL_CONSTEXPR size_t max_size() const
```
**Description**  
Returns the maximum possible size of the queue.

---

```cpp
ETL_CONSTEXPR size_t capacity() const
```
**Description**  
Returns the maximum possible size of the queue.

## Modifiers
```cpp
bool push(const T& value)
bool push(T&& value)
```
**Description**  
Pushes a value to the back of the queue. 
Returns true if successful, otherwise false.

---

```cpp
bool pop()
```cpp
**Description**  
Pop a value from the front of the list.
Returns true if successful, otherwise false.

---

```cpp
bool pop(T& value)
bool pop(T&& value)
```
**Description**  
Pop a value from the front of the list and place it in value.
Returns true if successful, otherwise false.

---

```cpp
void clear()
```
**Description**  
Clears the queue to a size of zero.
Must be called from thread that pops the queue or when there is no  possibility of concurrent access.

---

### C++03
```cpp
bool emplace(const T1& value1)
bool emplace(const T1& value1, const T2& value2)
bool emplace(const T1& value1, const T2& value2, const T3& value3)
bool emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
Constructs an item in the the queue 'in place'.

### C++11
```cpp
bool emplace(Args&&… args)
```
**Description**  
Constructs an item in the the queue 'in place'.

## Notes

Remember that thread context switches may occur between calls to the access protected functions. For example, a call to `empty()` may return `true`, but a subsequent call to `pop()` may succeed if a context switch occurred between the two and pushed a new value.

## Example
```cpp
etl::queue_spsc_atomic<char, 10> queue;

int main()
{
  while (true)
  {
    char c;

    if (queue.pop(c))
    {
      Print(c);
    }
  }
}

void Thread()
{
  while (true)
  {
    queue.push('A');
  }
}
```
