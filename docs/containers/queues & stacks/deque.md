---
title: "deque"
---

{{< callout >}}
  Header: `deque.h`  
  Similar to: [std::deque](https://en.cppreference.com/w/cpp/container/deque.html)
{{< /callout >}}

A fixed capacity deque.

```cpp
etl::deque<typename T, const size_t SIZE>
```

Inherits from `etl::ideque<T>`.  
`etl::ideque` may be used as a size independent pointer or reference type for any `etl::deque` instance.  

Has the ability to be copied by low level functions such as `memcpy` by use of a `repair()` function.

## Template deduction guides
```cpp
template <typename... T>
etl::deque(T...)
```
C++17 and above.

### Example
```cpp
etl::deque data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```
Defines data as an `deque` of `int`, of length `10`, containing the supplied data.

## Make template
template <typename T, typename... TValues>
  constexpr auto make_deque(TValues&&... values)
  C++11 and above

### Example
```cpp
auto data = etl::make_deque<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
```

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
reverse_iterator        std::reverse_iterator<iterator>
const_reverse_iterator  std::reverse_iterator<const_iterator>
```

## Static Constants

`MAX_SIZE`  The maximum size of the `deque`.

## Constructor

```cpp
etl::deque<typename T, const size_t SIZE>()
etl::deque<typename T, const size_t SIZE>(size_t initialSize)
etl::deque<typename T, const size_t SIZE>(size_t initialSize, parameter_t value)

template <typename TIterator>
etl::deque<typename T, const size_t SIZE>(TIterator begin, TIterator end)

etl::deque<typename T, const size_t SIZE>(const etl::deque<typename T, const size_t SIZE>& other)
etl::deque<typename T, const size_t SIZE>(etl::deque<typename T, const size_t SIZE>&& other)
```cpp

## Element access

```cpp
T& at(size_t i)
const T& at(size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Emits an `etl::deque_out_of_range` if the index is out of range of the array. Undefined behaviour if asserts or exceptions are not enabled.

---

```cpp
T& operator[](size_t i)
const T& operator[](size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.

---

T& front()
const T& front() const
**Description**  
Returns a reference or const reference to the first element.

---

T& back()
const T& back() const
**Description**  
Returns a reference or const reference to the last element.

---

void fill(value_type value)
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
Returns an iterator to the beginning of the `deque`.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the deque.

---

```cpp
reverse_iterator rbegin()
const_reverse_iterator rbegin() const
const_reverse_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the deque.

---

```cpp
iterator rend()
const_reverse_iterator rend() const
const_reverse_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the deque.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns `true` if the size of the deque is zero, otherwise `false`.

---

```cpp
bool full() const
```
**Description**  
Returns `true` if the size of the deque is `SIZE`, otherwise `false`.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the deque.

---

```cpp
void resize(size_t new_size, T value = T())
```
**Description**  
Resizes the deque, up to the maximum capacity.  
Emits an `etl::deque_full` if the deque does not have the capacity.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the deque.

---

```cpp
size_t capacity() const
```
**Description**  
Returns the maximum possible size of the deque.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the deque.

## Modifiers

```cpp
template <typename TIterator>
void assign(TIterator begin, TIterator end)
```
**Description**  
Fills the deque with the range.  
The range is not checked for validity.

---

```cpp
void assign(size_t n, const_reference value)
```
**Description**  
Fills the deque with the values.  
Emits etl::deque_iterator if the distance between begin and end is illegal, otherwise undefined behaviour if asserts or exceptions are not enabled.

---

```cpp
void push_front(const_reference value)
void push_front(rvalue_reference value)
```
**Description**  
Pushes a value to the front of the deque.  
If the deque is full and `ETL_CHECK_PUSH_POP` is defined then emits an `etl::deque_full`, otherwise undefined behaviour if asserts or exceptions are not enabled.

---

```cpp
void push_back(const_reference value)
void push_back(rvalue_reference value)
```
**Description**  
Pushes a value to the back of the deque.  
If the deque is full and `ETL_CHECK_PUSH_POP` is defined then emits an `etl::deque_full`, otherwise undefined behaviour if asserts or exceptions are not enabled.

---

```cpp
void pop_front()
```
**Description**  
Pop a value from the front of the deque.  
If the deque is empty and `ETL_CHECK_PUSH_POP` is defined then emits an `etl::deque_empty`, otherwise undefined behaviour if asserts or exceptions are not enabled.

---

```cpp
void pop_back()
```
**Description**  
Pop a value from the back of the deque.  
If the deque is empty and `ETL_CHECK_PUSH_POP` is defined then emits an `etl::deque_empty`, otherwise undefined behaviour if asserts or exceptions are not enabled.

---

```cpp
void insert(iterator position, size_t n, parameter_t value)
```

---

```cpp
template <typename TIterator>
void insert(iterator position, TIterator begin, TIterator end)
iterator insert(iterator position, parameter_t value)
iterator insert(iterator position, rvalue_reference value)
```
Before: `20.20.0`

---

```cpp
template <typename TIterator>
iterator insert(const_iterator position, TIterator begin, TIterator end)
iterator insert(const_iterator position, parameter_t value)
iterator insert(const_iterator position, rvalue_reference value)
```
**Description**  
Inserts values in to the deque. If the deque is full then emits an `etl::deque_full exception`.  
Undefined behaviour if asserts or exceptions are not enabled.  
From: `20.20.0`

---

```cpp
template <typename TIterator>
iterator erase(TIterator begin, TIterator end)
```

```cpp
iterator erase(iterator position)
```
Before: `20.20.0`

---

```cpp
iterator erase(iterator position)
iterator erase(const_iterator position)
```
**Description**  
Erases values in the deque.  
Undefined behaviour if asserts or exceptions are not enabled and begin, end or position are invalid.
From: `20.20.0`

---

C++03  
Before: `20.20.0`

```cpp
template <typename T1>
iterator emplace(iterator insert_position, const T1& value1)

template <typename T1, typename T2>
iterator emplace(iterator insert_position, const T1& value1, const T2& value2)

template <typename T1, typename T2, typename T3>
iterator emplace(iterator insert_position, const T1& value1, const T2& value2,
                                           const T3& value3)

template <typename T1, typename T2, typename T3, typename T4>
iterator emplace(iterator insert_position, const T1& value1, const T2& value2,
                                           const T3& value3, const T4& value4)
```

---

```cpp
template <typename ... Args>
iterator emplace(iterator insert_position, Args&& ... args)
```

---

C++03  
From: `20.20.0`  

```cpp
template <typename T1>
iterator emplace(const_iterator insert_position, const T1& value1)

template <typename T1, typename T2>
iterator emplace(const_iterator insert_position, const T1& value1, const T2& value2)

template <typename T1, typename T2, typename T3>
iterator emplace(const_iterator insert_position, const T1& value1, const T2& value2,
                                                 const T3& value3)

template <typename T1, typename T2, typename T3, typename T4>
iterator emplace(const_iterator insert_position, const T1& value1, const T2& value2,
                                                 const T3& value3, const T4& value4)
```

---

C++11 and above

```cpp
template <typename ... Args>
iterator emplace(const_iterator insert_position, Args&& ... args)
```

---

C++03
Before: `20.35.8`

```cpp
template <typename T1>
void emplace_front(const T1& value1)

template <typename T1, typename T2>
void emplace_front(const T1& value1, const T2& value2)

template <typename T1, typename T2, typename T3>
void emplace_front(const T1& value1, const T2& value2,
                   const T3& value3)

template <typename T1, typename T2, typename T3, typename T4>
void emplace_front(const T1& value1, const T2& value2,
                   const T3& value3, const T4& value4)
```

---

C++11 and above

```cpp
template <typename ... Args>
void emplace_front(Args&& ... args)
```

---

C++03
Since `20.35.10`

```cpp
template <typename T1>
reference emplace_front(const T1& value1)

template <typename T1, typename T2>
reference emplace_front(const T1& value1, const T2& value2)

template <typename T1, typename T2, typename T3>
reference emplace_front(const T1& value1, const T2& value2,
                        const T3& value3)

template <typename T1, typename T2, typename T3, typename T4>
reference emplace_front(const T1& value1, const T2& value2,
                        const T3& value3, const T4& value4)
```

---

C++11 and above

```cpp
template <typename ... Args>
reference emplace_front(Args&& ... args)
```

---

C++03
Before: 20.35.8

```cpp
template <typename T1>
void emplace_back(const T1& value1)

template <typename T1, typename T2>
void emplace_back(const T1& value1, const T2& value2)

template <typename T1, typename T2, typename T3>
void emplace_back(const T1& value1, const T2& value2,
                  const T3& value3)

template <typename T1, typename T2, typename T3, typename T4>
void emplace_back(const T1& value1, const T2& value2,
                  const T3& value3, const T4& value4)
```

---

C++11 and above

```cpp
template <typename ... Args>
void emplace_back(Args&& ... args)
```

---

C++03
Since: 20.35.10

```cpp
template <typename T1>
reference emplace_back(const T1& value1)

template <typename T1, typename T2>
reference emplace_back(const T1& value1, const T2& value2)

template <typename T1, typename T2, typename T3>
reference emplace_back(const T1& value1, const T2& value2,
                       const T3& value3)

template <typename T1, typename T2, typename T3, typename T4>
reference emplace_back(const T1& value1, const T2& value2,
                       const T3& value3, const T4& value4)
```

---

C++11 and above

```cpp
template <typename ... Args>
reference emplace_back(Args&& ... args)
```

---

```cpp
void clear()
```
**Description**  
Clears the deque to a size of zero.

---

```cpp
void repair()
```
**Description**  
This function must be called if the deque has been copied via a low level method such as `memcpy`.  
This can only be called from an `etl::deque` instance, unless `ETL_IDEQUE_REPAIR_ENABLE` is defined. Be aware that doing this introduces a virtual function to the class.  

Has no effect if the object has not been copied in this way.  

**Note:**  
The contained type must be trivially copyable.  
Compilers that satisfy the C++11 type traits support check in `platform.h` will generate an assert if the type is incompatible.  

### Example
```cpp
typedef etl::deque<int, 10> Data;

Data data(8, 1);

char buffer[sizeof(Data)];

memcpy(&buffer, &data, sizeof(Data));

Data& rdata(*reinterpret_cast<Data*>(buffer));

// Do not access the copied object in any way until you have called this.
rdata.repair();
```

---

```cpp
etl::deque<typename T, const size_t SIZE>& 
    operator =(const etl::deque<typename T, const size_t SIZE>& other)

etl::deque<typename T, const size_t SIZE>& 
    operator =(etl::deque<typename T, const size_t SIZE>&& other)

etl::ideque<typename T>& 
    operator =(const etl::ideque<typename T>& other)

etl::deque<typename T>& 
    operator =(etl::deque<typename T>&& other)
```

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

```cpp
operator <
```
**Description**  
`true` if the contents of the lhs are lexicographically less than the contents of the rhs,  otherwise `false`.

---

```cpp
operator <=
```
**Description**  
`true` if the contents of the lhs are lexicographically less than or equal to the contents of the rhs, otherwise `false`.

---

```cpp
operator >
```
**Description**  
`true` if the contents of the lhs are lexicographically greater than the contents of the rhs,  otherwise `false`.

---

```cpp
operator >=
```
**Description**  
`true` if the contents of the lhs are lexicographically greater than or equal to the contents of the rhs, otherwise `false`.
