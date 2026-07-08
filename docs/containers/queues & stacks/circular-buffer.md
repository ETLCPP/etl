---
title: "circular_buffer"
---

{{< callout >}}
  Header: `circular_buffer.h`  
{{< /callout >}}

A fixed capacity circular buffer.

```cpp
etl::circular_buffer<typename T, size_t SIZE>
etl::circular_buffer_ext<typename T>
```

Inherits from `etl::icircular_buffer<T>`  
`etl::icircular_buffer` may be used as a size independent pointer or reference type for any `etl::circular_buffer` instance.  

## External buffer

```cpp
etl::circular_buffer_ext<typename T>
```
For this template the constructor expects pointer and size parameters to the externally provided buffer. This buffer must not be shared concurrently with any other container.  
When a `circular_buffer` with an external buffer is moved, the data is moved, not the pointer to the buffer.  
The external buffer MUST be declared one item larger that the intended capacity of the circular buffer.  

A swap on a `circular_buffer` with an external buffer is fast as it will swap buffer pointers rather than copying items.  

## Template deduction guides
```cpp
template <typename T, typename... Ts>
etl::circular_buffer(T, Ts...)
  -> etl::circular_buffer<T, 1U + sizeof...(Ts)>;
```
C++17 and above.  
Only enabled if `Ts...` is the same type as `T`.

### Example
```cpp
etl::circular_buffer data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```

Defines data as a `circular_buffer` of `int`, of length `10`, containing the supplied data.

## Member types

```cpp
value_type              T
size_type               std::size_t
difference_type         std::ptrdiff_t
reference               value_type&
const_reference         const value_type&
rvalue_reference        value_type&&
pointer                 value_type*
const_pointer           const value_type*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        reverse_iterator<iterator>
const_reverse_iterator  reverse_iterator<const_iterator>
```

## Static Constants

`MAX_SIZE`  The maximum size of the circular_buffer.

## Constructors

```cpp
circular_buffer<typename T, size_t SIZE>();

circular_buffer<typename T, size_t SIZE>(const etl::circular_buffer<typename T, size_t SIZE>& other);

circular_buffer<typename T, size_t SIZE>(etl::circular_buffer<typename T, size_t SIZE>&& other);
```

---

```cpp
circular_buffer_ext(void* buffer, size_t max_size)

circular_buffer_ext(size_t max_size) 20.32.1

template <typename TIterator>
circular_buffer_ext(TIterator first, const TIterator& last, void* buffer, size_t max_size)

circular_buffer_ext(std::initializer_list<T> init, void* buffer, size_t max_size)

circular_buffer_ext(const circular_buffer_ext& other, void* buffer, size_t max_size)
```

## Status

```cpp
bool is_valid() const
```
**Description**  
Returns `true` if the buffer has been set through the constructor or `set_buffer`.  
For circular_buffer_ext only.  
From: `20.32.1`

## Element access

```cpp
reference operator[](size_t i)
const_reference operator[](size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.

---

```cpp
reference front()
const_reference front() const
```
**Description**  
Returns a reference or const reference to the first element.

---

```cpp
reference back()
const_reference back() const
```
**Description**  
Returns a reference or const reference to the last element.

---

```cpp
void fill(value_type value)
```
**Description**  
Fill the current size of the buffer with `value`.
From: `20.24.0`

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the `circular_buffer`.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the `circular_buffer`.

---

```cpp
iterator rbegin()
const_reverse_iterator rbegin() const
const_reverse_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the `circular_buffer`.

---

```cpp
iterator rend()
const_reverse_iterator rend() const
const_reverse_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the `circular_buffer`.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns `true` if the size of the circular_buffer is zero, otherwise `false`.

---

```cpp
bool full() const
```
**Description**  
Returns `true` if the size of the circular_buffer is `SIZE`, otherwise `false`.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the `circular_buffer`.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the `circular_buffer`.

---

```cpp
size_t capacity() const
```
**Description**  
Returns the maximum possible size of the `circular_buffer`.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the `circular_buffer`.

## Modifiers

```cpp
void set_buffer(void* buffer)
```
**Description**  
Sets the associated buffer.  
For circular_buffer_ext only.  
From: `20.32.1`

---

```cpp
void push(const_reference value)
void push(rvalue_reference value)
```
**Description**  
Pushes a value to the back of the `circular_buffer`.

---

```cpp
template <typename TIterator>
void push(TIterator first, const TIterator& last)
```
**Description**  
Pushes values to the back of the `circular_buffer`.

---

```cpp
void pop()
```
**Description**  
Pop a value from the front of the `circular_buffer`.

---

```cpp
void clear()
```
**Description**  
Clears the circular_buffer to a size of zero.

---

```cpp
void swap(circular_buffer<T, SIZE>& other)
```
**Description**  
Swaps the `circular_buffer` with `other`.

---

```cpp
void swap(circular_buffer_ext<T> other)
```
**Description**  
Swaps the `circular_buffer_ext` with `other`.  
The internal pointers to the external buffers are swapped.

## Non-member functions

```cpp
operator ==
```
**Description**  
`true` if the contents of the lists are equal, otherwise `false`.

---

```cpp
operator !=
```
**Description**  
`true` if the contents of the lists are not equal, otherwise `false`.

---

`swap` Swaps two circular buffers. A circular buffer with internal storage copies items, while one with an external buffer will swap pointers.
