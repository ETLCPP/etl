---
title: "stack"
---

{{< callout >}}
  Header: `stack.h`  
  Similar to: `std::stack`  
{{< /callout >}}

A fixed capacity stack.

```cpp
etl::stack<typename T, const size_t SIZE>
```

Inherits from `etl::istack<T>`  
`etl::istack` may be used as a size independent pointer or reference type for any `etl::stack` instance.

## Member types

```cpp
value_type T
size_type  std::size_t
```

## Constructor

```cpp
etl::stack<typename T, const size_t SIZE>();
```
**Description**  
Default constructs `SIZE` elements.

## Element access

```cpp
T& top()
const T& top() const
```
**Description**  
Returns a reference or const reference to the element at the top of the stack.  
Undefined behaviour if the stack is empty.

## Capacity
```cpp
bool empty() const
```
**Description**  
Returns `true` if the size of the stack is zero, otherwise `false`.

---

```cpp
bool full() const
```
**Description**  
Returns `true` if the size of the stack is `SIZE`, otherwise `false`.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the stack.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the stack.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the stack.

---

```cpp
size_t capacity() const
```
**Description**  
Returns the maximum possible size of the stack.

## Modifiers

```cpp
void push(parameter_t value);
```
**Description**  
Pushes a value to the top of the stack.  
Emits an `etl::stack_full` if the stack is full and `ETL_CHECK_PUSH_POP` is defined.  
If asserts or exceptions are not enabled undefined behaviour occurs.

---

C++03
```cpp
void emplace(const T1& value1);
void emplace(const T1& value1, const T2& value2);
void emplace(const T1& value1, const T2& value2, const T3& value3);
void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4);
```

C++11
```cpp
void emplace(Args&&… args);
```
**Description**  
Constructs an item in the the stack 'in place'.  
Supports up to four constructor parameters.  
Emits an `etl::stack_full` if the stack is full and `ETL_CHECK_PUSH_POP` is defined.  
If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
void pop();
```
**Description**  
Pop a value from the top of the stack.  
Emits an `etl::stack_empty` if the stack is empty and `ETL_CHECK_PUSH_POP` is defined.  
If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
void pop_into(const T& destination);
```
**Description**  
Pop a value from the top of the stack and places it in destination.  
Emits an `etl::stack_empty` if the queue is empty and `ETL_CHECK_PUSH_POP` is defined.  
If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
void reverse();
```
**Description**  
Reverses the stack order.

---

```cpp
void clear();
```
**Description**  
Clears the stack to a size of zero.
