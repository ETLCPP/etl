---
title: "queue"
---

{{< callout >}}
  Header: `queue.h`  
{{< /callout >}}

A fixed capacity queue.

```cpp
etl::queue<typename T, 
           const size_t SIZE, 
           const size_t MEMORY_MODEL = etl::memory_model::MEMORY_MODEL_LARGE>
```

Inherits from `etl::iqueue<T>`.  
`etl::iqueue` may be used as a size independent pointer or reference type for any `etl::queue` instance.  

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

value_type      T
size_type        <based on memory model>
pointer          value_type*
const_pointer    const value_type*
reference        value_type&
const_reference  const value_type&
rvalue_reference value_type&&

## Constructor

etl::queue<typename T, const size_t SIZE>()

## Element access

```cpp
T& front()
const T& front() const
```
**Description**  
Returns a reference or const reference to the first element.

---

```cpp
T& back()
const T& back() const
```
**Description**  
Returns a reference or const reference to the last element.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the queue is zero, otherwise false.

---

```cpp
bool full() const
```
**Description**  
Returns true if the size of the queue is SIZE, otherwise false.

---

```cpp
size_type size() const
```
**Description**  
Returns the size of the queue.

---

```cpp
size_type available() const
```
**Description**  
Returns the remaining available capacity in the queue.

---

```cpp
ETL_CONSTEXPR size_type max_size() const
```
**Description**  
Returns the maximum possible size of the queue.

---

```cpp
ETL_CONSTEXPR size_type capacity() const
```
**Description**  
Returns the maximum possible size of the queue.

## Modifiers

```cpp
void push(const_reference value)
void push(rvalue_reference value)
```
**Description**  
Pushes a value to the back of the queue. 
Emits an `etl::queue_full` if the queue is full and `ETL_CHECK_PUSH_POP` is defined. If asserts or exceptions are not enabled then undefined behaviour occurs.

---

### C++03
```cpp
void emplace(const T1& value1)
void emplace(const T1& value1, const T2& value2)
void emplace(const T1& value1, const T2& value2, const T3& value3)
void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
Constructs an item in the the queue 'in place'.
Emits an `etl::queue_full` if the queue is full and `ETL_CHECK_PUSH_POP` is defined. If asserts or exceptions are not enabled then undefined behaviour occurs.

### C++11
```cpp
void emplace(Args&&… args)
```
**Description**  
Constructs an item in the the queue 'in place'.
Emits an `etl::queue_full` if the queue is full and `ETL_CHECK_PUSH_POP` is defined. If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
void pop()
```
**Description**  
Pop a value from the front of the list.
Emits an `etl::queue_empty` if the queue is empty and `ETL_CHECK_PUSH_POP` is defined. Undefined behaviour if the queue is empty.

---

```cpp
void pop_into(TContainer& destination)
```
**Description**  
Pop a value from the front of the list and places it in destination.
Emits an `etl::queue_empty` if the queue is empty and `ETL_CHECK_PUSH_POP` is defined. Undefined behaviour if the queue is empty.

---

```cpp
void clear()
```
**Description**  
Clears the queue to a size of zero.
