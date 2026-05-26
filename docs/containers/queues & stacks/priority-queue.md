---
title: "priority_queue"
---

{{< callout >}}
  Header: `priority_queue.h`  
  Similar to: `std::priority_queue`
{{< /callout >}}

A fixed capacity priority queue.

```cpp
etl::priority_queue<typename T, const size_t SIZE>
```

Inherits from `etl::ipriority_queue<T>`  
`etl::ipriority_queue` may be used as a size independent pointer or reference type for any `etl::priority_queue` instance.

## Member types
```cpp
value_type T
size_type  std::size_t
```

## Constructor

```cpp
etl::priority_queue<typename T, const size_t SIZE>();
```

## Element access

```cpp
T& top()
const T& top() const
```
**Description**  
Returns a reference or const reference to the first element.  
Undefined behaviour if the queue is empty.

## Capacity
```cpp
bool empty() const
```
**Description**  
Returns `true` if the size of the queue is zero, otherwise `false`.

---

```cpp
bool full() const
```
**Description**  
Returns `true` if the size of the queue is SIZE, otherwise `false`.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the queue.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the queue.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the queue.

---

```cpp
size_t capacity() const
```
**Description**  
Returns the maximum possible size of the queue.

## Modifiers

```cpp
void push(const T& value);
void push(T&& value);
```
**Description**  
Pushes a value to the queue.  
If the queue is full then emits an `etl::queue_full error`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

**C++03**  
```cpp
void emplace(const T1& value1);
void emplace(const T1& value1, const T2& value2);
void emplace(const T1& value1, const T2& value2, const T3& value3);
void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4);
```

**C++11**  
```cpp
void emplace(Args&&… args);
```
**Description**  
Constructs an item in the the queue 'in place'.  
C++03: Supports up to four constructor parameters.  
Emits an `etl::queue_full` if the queue is full and `ETL_CHECK_PUSH_POP` is defined.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
void pop();
```
**Description**  
Pop a value from the front of the list.  
Undefined behaviour if the queue is empty.

---

```cpp
void clear();
```
**Description**  
Clears the queue to a size of zero.
