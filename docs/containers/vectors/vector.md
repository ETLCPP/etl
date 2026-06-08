---
title: "vector"
weight: 1
---

{{< callout >}}
  Header: `vector.h`  
  Similar to: [std::vector](https://en.cppreference.com/w/cpp/container/vector.html)
{{< /callout >}}

A fixed capacity vector.

```cpp
etl::vector<typename T, size_t SIZE>
etl::vector_ext<typename T>
```

## ivector
Inherits from `etl::ivector<T>`  
`etl::ivector` may be used as a size independent pointer or reference type for any `etl::vector` instance.  

There is a specialisation for pointers that means that just one instantiation of code for all pointer types.  
The one caveat is that `etl::vector` cannot directly store pointers to member functions. They must be wrapped in either a custom struct, one of the `etl::function` or `etl::deletgate` templates, or `std::function`.  

Has the ability to be copied by low level functions such as `memcpy` by use of a `repair()` function.  
See the function reference for an example of use.  

Unlike `std::vector`, An iterator to an `etl::vector` element is never invalidated by a call to `resize()`.  

The size of the instance will be `(SIZE * sizeof(T)) + (2 * sizeof(size_t)) + sizeof(T*)`.  
For a 32 bit environment the overhead (compared to an array) will usually be 12 bytes.  

---

## External buffer

```cpp
etl::vector_ext<typename T>
```
With this template the constructor expects pointer and size parameters to the externally provided buffer. This buffer must not be shared concurrently with any other vector.  
When a vector with an external buffer is moved, the data is moved, not the pointer to the buffer.

## Template deduction guides
C++17 and above

template <typename T, typename... Ts>
etl::vector(T...)

template <typename T, typename... Ts>
etl::vector(T*...)

### Example
etl::vector data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

Defines data as an vector of int, of length 10, containing the supplied data.

## Member types

```cpp
value_type              T
size_type               size_t
difference_type         ptrdiff_t
reference               value_type&
const_reference         const value_type&
rvalue_reference        value_type&&
pointer                 value_type*
const_pointer           const value_type*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        ETL_OR_STD::reverse_iterator<iterator>
const_reverse_iterator  ETL_OR_STD::reverse_iterator<const_iterator>
```

## Constructors

**Internal buffer**  
```cpp
etl::vector<typename T, const size_t SIZE>();
etl::vector<typename T, const size_t SIZE>(size_t initialSize);
etl::vector<typename T, const size_t SIZE>(size_t initialSize, const T& value);

template <typename TIterator>
etl::vector<typename T, const size_t SIZE>(TIterator begin, TIterator end);

etl::vector<typename T, const size_t SIZE>(const etl::vector<typename T, const size_t SIZE>&);
etl::vector<typename T, const size_t SIZE>(etl::vector<typename T, const size_t SIZE>&&);

**External buffer**  
etl::vector<typename T, const size_t SIZE>(void* buffer, size_t max_size);
etl::vector<typename T, const size_t SIZE>(size_t initialSize, void* buffer, size_t max_size);
etl::vector<typename T, const size_t SIZE>(size_t initialSize, const T& value, void* buffer, size_t max_size);

template <typename TIterator>
etl::vector<typename T, const size_t SIZE>(TIterator begin, TIterator end, void* buffer, size_t max_size);

etl::vector<typename T, const size_t SIZE>(const etl::vector<typename T, const size_t SIZE>&, void* buffer, size_t max_size);

etl::vector<typename T, const size_t SIZE>(etl::vector<typename T, const size_t SIZE>&&, void* buffer, size_t max_size);
```
If the vector is full then emits an `etl::vector_full`. If asserts or exceptions are not enabled then undefined behaviour occurs.

## Element access

```cpp
T& at(size_t i)
const T& at(size_t i) const
```
Returns a reference or const reference to the indexed element. Emits an etl::vector_out_of_range if the index is out of range of the array. If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
T& operator[](size_t i)
const T& operator[](size_t i) const
```
Returns a reference or const reference to the indexed element.  
if the index is out of range of the array then undefined behaviour occurs.  

---

```cpp
T& front()
const T& front() const
```
Returns a reference or const reference to the first element.  
Undefined behaviour if the vector is empty.  

---

```cpp
T& back()
const T& back() const
```
Returns a reference or const reference to the last element.  
Undefined behaviour if the vector is empty.

---

```cpp
T* data()
const T* data() const
```
Returns a pointer or const pointer to the internal buffer.  

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
Returns an iterator to the beginning of the vector.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
Returns an iterator to the end of the vector.

---

```cpp
iterator rbegin()
const_reverse_iterator rbegin() const
const_reverse_iterator crbegin() const
```
Returns a reverse iterator to the beginning of the vector.

---

```cpp
iterator rend()
const_reverse_iterator rend() const
const_reverse_iterator crend() const
```
Returns a reverse iterator to the end of the vector.

## Capacity

```cpp
bool empty() const
```
Returns `true` if the size of the vector is zero, otherwise `false`.

---

```cpp
bool full() const
```
Returns `true` if the size of the vector is `SIZE`, otherwise `false`.

---

```cpp
size_t size() const
```
Returns the size of the vector.

---

```cpp
void resize(size_t new_size, const_reference value = T())
```
Resizes the vector, up to the maximum capacity.  
Emits an `etl::vector_full` if the vector does not have the capacity.

---

```cpp
void uninitialized_resize(size_t new_size)
```
Resizes the vector, up to the maximum capacity, without initialising the new elements.  
From: `20.4.0`

---

```cpp
size_t max_size() const
```
Returns the maximum possible size of the vector.

---

```cpp
size_t capacity() const
```
Returns the maximum possible size of the vector.

---

```cpp
size_t available() const
```
Returns the remaining available capacity in the vector.

## Modifiers

```cpp
template <typename TIterator>
void assign(TIterator begin, TIterator end)
```

```cpp
void assign(size_t n, const T& value)
```
Fills the vector with the values. Emits `etl::vector_iterator` if the distance between begin and end is illegal. 
(debug mode only). If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
void push_back(const T& value)
void push_back(T&& value)
```
Pushes a value to the back of the vector.  
If the vector is full then emits an `etl::vector_full`. If asserts or exceptions are not enabled undefined behaviour occurs.

---

**C++03**  
From: `20.38.0`
```cpp
void emplace();
void emplace(const T1& value1)
void emplace(const T1& value1, const T2& value2)
void emplace(const T1& value1, const T2& value2, const T3& value3)
void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```

**C++11**  
```cpp
template <typename… Args>
void emplace(Args&&… args);
```
Constructs an item at the back of the the vector 'in place'.  
Supports up to four constructor parameters.  
Pushes a value to the back of the vector. The first pushes a value, the second allocates the new element but does not initialise it.  
If the vector is full then emits an `etl::vector_full`. If asserts or exceptions are not enabled undefined behaviour occurs.  

---

**C++03**  
Before `20.35.10`
```cpp
void emplace_back(const T1& value1)
void emplace_back(const T1& value1, const T2& value2)
void emplace_back(const T1& value1, const T2& value2, const T3& value3)
void emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```

From: `20.35.10`
```cpp
reference emplace_back(); 20.38.0
reference emplace_back(const T1& value1)
reference emplace_back(const T1& value1, const T2& value2)
reference emplace_back(const T1& value1, const T2& value2, const T3& value3)
reference emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```

**C++11**  
Before: `20.35.10`  
```cpp
template <typename Args...>
void emplace_back(Args&&... args)
```

From: `20.35.10`  
```cpp
template <typename Args...>
reference emplace_back(Args&&... args)
```
Constructs an item at the back of the the vector 'in place'.  
Supports up to four constructor parameters.  
Pushes a value to the back of the vector.  
If the vector is full then emits an `etl::vector_full`. If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
void pop_back()
```
Pop a value from the back of the vector.
If the vector is empty and ETL_CHECK_PUSH_POP is defined then emits an etl::vector_empty. If asserts or exceptions are not enabled undefined behaviour occurs.

---

Before: `20.20.0`
```cpp
template <typename TIterator>
void insert(iterator position, TIterator begin, TIterator end)
```

```cpp
iterator insert(iterator position, const T& value)
iterator insert(iterator position, T&& value)
void insert(iterator position, size_t n, const T& value)
```

From: `20.20.0`
```cpp
template <typename TIterator>
iterator insert(const_iterator position, TIterator begin, TIterator end)
```

```cpp
iterator insert(const_iterator position, const T& value)
iterator insert(const_iterator position, T&& value)
iterator insert(const_iterator position, size_t n, const T& value)
```
Inserts values in to the vector.  
If the vector is full then emits an `etl::vector_full` exception. If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
template <typename TIterator>
iterator erase(TIterator begin, TIterator end)
```

```cpp
iterator erase(iterator position)
```
Erases values in the vector.  
Iterators are not checked.

---

```cpp
void clear()
```
Clears the vector to a size of zero.

---

```cpp
void fill(value_type value)
```
Fill the current size of the buffer with `value`.
From: `20.24.0`

---

```cpp
void repair()
```
This function must be called if the vector has been copied via a low level method such as `memcpy`.  
This can only be called from an `etl::vector` instance, unless `ETL_IVECTOR_REPAIR_ENABLE` is defined. Be aware that doing so introduces a virtual function to the class.  

Has no effect if the object has not been copied in this way.  

**Note:**  
The contained type must be trivially copyable.  
Compilers that satisfy the C++11 type traits support check in `platform.h` will generate an assert if the type is incompatible.

### Example:

```cpp
typedef etl::vector<int, 10> Data;

Data data(8, 1);
char buffer[sizeof(Data)];

memcpy(&buffer, &data, sizeof(Data));

Data& rdata(*reinterpret_cast<Data*>(buffer));

// Do not access the copied object in any way until you have called this.
rdata.repair();
```

## Non-member functions

```cpp
template <typename T, typename U>
typename etl::ivector<T>::difference_type erase(etl::ivector<T>& v, const U& value)
```
Erases all elements that compare equal to value from the vector.

---

```cpp
template <typename T, typename TPredicate>
typename etl::ivector<T>::difference_type erase_if(etl::ivector<T>& v, TPredicate predicate)
```
Erases all elements that satisfy the predicate from the vector.

## Operators

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

