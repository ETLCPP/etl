---
title: "indirect_vector"
weight: 3
---

{{< callout type="info">}}
  Header: `indirect_vector.h`  
  Similar to: `std::vector`
{{< /callout >}}

A fixed capacity vector that uses indirection to access the elements.  
Can be more efficient for vectors of large objects, especially when inserting, erasing or sorting.  
Supplies function adaptors to allow efficient application of external algorithms.  

```cpp
etl::indirect_vector<typename T, size_t SIZE>
etl::indirect_vector_ext<typename T>
```

Inherits from `etl::iindirect_vector<T>`.  
`etl::iindirect_vector` may be used as a size independent pointer or reference type for any `etl::indirect_vector` instance.

## External buffer

```cpp
etl::indirect_vector_ext<typename T>
```
With this template, the constructor expects pointer and size parameters to the externally provided indirection lookup vector and pool.

## Template deduction guides
C++17 and above

```cpp
template <typename... T>
etl::indirect_vector(T...)
```

### Example

```cpp
etl::indirect_vector data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```
Defines data as an `indirect_vector` of `int`, of length `10`, containing the supplied data.

## Member types

```cpp
value_type              T
size_type               std::size_t
difference_type         std::ptrdiff_t
reference               value_type&
const_reference         const value_type&
pointer                 value_type*
const_pointer           const value_type*
indirect_iterator       Iterator to the internal lookup vector
indirect_const_iterator Constant iterator to the internal lookup vector
const_pointer           const value_type*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        std::reverse_iterator<iterator>
const_reverse_iterator  std::reverse_iterator<const_iterator>
```

## Iterators

Both `iterator` and `const_iterator` define `indirection()` member functions that return an `indirect_iterator` or `indirect_const_iterator` to the internal lookup vector.  

Access to these iterators allows an external algorithm to manipulate the `indirect_vector` without moving the objects themselves, thereby vastly increasing the efficiency for vectors of large objects.

## Function Object Adaptors

Unary and binary function object wrappers are defined to allow algorithms to manipulate the contents of an `indirect_vector` using a standard function object with indirect iterators.

```cpp
template <typename TUnaryFunction, typename TReturnType = void>
class unary_function_adaptor
```

---

```cpp
template <typename TBinaryFunction, typename TReturnType = void>
class binary_function_adaptor
```

### Example
Sorting without moving or copying  large objects.

```cpp
// The large data that we are storing in the vector.
struct Data
{
  std::array<int, 1000> values;
};

// How to compare Data objects.
bool operator <(const Data& lhs, const Data& rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

// The vector of data.
using Vector = etl::indirect_vector<Data, 10>;
Vector v;

// std::less wrapped in a binary_function_adaptor.
using LessThan = typename Vector::binary_function_adaptor<std::less<Data>, bool>;

// Sort the vector without moving or copying actual Data objects.
std::sort(v.begin().indirection(),
          v.end().indirection(),
          LessThan());
```

{{< callout type="warning">}}
Warning: This technique is not suitable for algorithms that may leave the container with multiple copies of an object, such as `std::remove_if`. This is because the algorithm may create multiple copies of the indirect iterator, but only one copy of the object that they point to.
{{< /callout >}}

## Constructor

### Internal buffer
```cpp
etl::indirect_vector<typename T, const size_t SIZE>()
etl::indirect_vector<typename T, const size_t SIZE>(size_t initialSize)
etl::indirect_vector<typename T, const size_t SIZE>(size_t initialSize, const T& value)

template <typename TIterator>
etl::indirect_vector<typename T, const size_t SIZE>(TIterator begin, TIterator end)

etl::indirect_vector<typename T, const size_t SIZE>(const etl::indirect_vector<typename T, const size_t SIZE>&)
etl::indirect_vector<typename T, const size_t SIZE>(etl::indirect_vector<typename T, const size_t SIZE>&&)
```
**Description**  
If the vector is full then emits an `etl::vector_full`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

### External buffer
```cpp
etl::indirect_vector<typename T, const size_t SIZE>(etl::ivector<T*>& lookup, etl::ipool& storage)
etl::indirect_vector<typename T, const size_t SIZE>(size_t initialSize, etl::ivector<T*>& lookup, etl::ipool& storage)
etl::indirect_vector<typename T, const size_t SIZE>(size_t initialSize, const T& value, etl::ivector<T*>& lookup, etl::ipool& storage)

template <typename TIterator>
etl::indirect_vector<typename T, const size_t SIZE>(TIterator begin, TIterator end, etl::ivector<T*>& lookup, etl::ipool& storage)

etl::indirect_vector<typename T, const size_t SIZE>(const etl::indirect_vector<typename T, const size_t SIZE>&, etl::ivector<T*>& lookup, etl::ipool& storage)

etl::indirect_vector<typename T, const size_t SIZE>(etl::indirect_vector<typename T, const size_t SIZE>&&, etl::ivector<T*>& lookup, etl::ipool& storage)
```
**Description**  
If the vector is full then emits an `etl::vector_full`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

## Element access

```cpp
reference at(size_t i)
const_reference at(size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Emits an `etl::vector_out_of_range` if the index is out of range of the array.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
reference operator[](size_t i)
const_reference operator[](size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.
if the index is out of range of the array then undefined behaviour occurs.

---

```cpp
reference front()
const_reference front() const
```
**Description**  
Returns a reference or const reference to the first element.
Undefined behaviour if the vector is empty.

---

```cpp
reference back()
const_reference back() const
```
**Description**  
Returns a reference or const reference to the last element.
Undefined behaviour if the vector is empty.

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the vector.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the vector.

---

```cpp
iterator rbegin()
const_reverse_iterator rbegin() const
const_reverse_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the vector.

---

```cpp
iterator rend()
const_reverse_iterator rend() const
const_reverse_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the vector.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the vector is zero, otherwise false.

---

```cpp
bool full() const
```
**Description**  
Returns true if the size of the vector is SIZE, otherwise false.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the vector.

---

```cpp
void resize(size_t new_size, T value = T())
```
**Description**  
Resizes the vector, up to the maximum capacity. Emits an etl::vector_full if the vector does not have the capacity.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the vector.

---

```cpp
size_t capacity() const
```
**Description**  
Returns the maximum possible size of the vector.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the vector.

## Modifiers

```cpp
template <typename TIterator>
void assign(TIterator begin, TIterator end)

void assign(size_t n, const T& value)
```
**Description**  
Fills the vector with the values. Emits etl::vector_iterator if the distance between begin and end is illegal. 
(debug mode only). If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
void push_back(const T& value)
void push_back(T&& value)
```
**Description**  
Pushes a value to the back of the vector.
If the vector is full then emits an etl::vector_full. If asserts or exceptions are not enabled undefined behaviour occurs.

---

**C++03**  
```cpp
void emplace() 
void emplace(const T1& value1)
void emplace(const T1& value1, const T2& value2)
void emplace(const T1& value1, const T2& value2, const T3& value3)
void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
Constructs an item at the back of the the vector 'in place'.  
Supports up to four constructor parameters.

---

**C++11**  

```cpp
template <typename… Args>
void emplace(Args&&… args)
```
**Description**  
Constructs an item at the back of the the vector 'in place'.
Pushes a value to the back of the vector.  
If the vector is full then emits an `etl::vector_full`.  
If asserts or exceptions are not enabled undefined behaviour occurs.

---

**C++03**

```cpp
void emplace_back(const T1& value1)
void emplace_back(const T1& value1, const T2& value2)
void emplace_back(const T1& value1, const T2& value2, const T3& value3)
void emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
Constructs an item at the back of the the vector 'in place'.  
If the vector is full then emits an `etl::vector_full`. If asserts or exceptions are not enabled undefined behaviour occurs.  
Supports up to four constructor parameters.  
Before: `20.35.10`

---

```cpp
reference emplace_back() 20.38.0
reference emplace_back(const T1& value1)
reference emplace_back(const T1& value1, const T2& value2)
reference emplace_back(const T1& value1, const T2& value2, const T3& value3)
reference emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
Constructs an item at the back of the the vector 'in place'.  
If the vector is full then emits an `etl::vector_full`. If asserts or exceptions are not enabled undefined behaviour occurs.  
Supports up to four constructor parameters.  
Since: `20.35.10`

---

**C++11**  

```cpp
template <typename Args...>
void emplace_back(Args&&... args)
```
**Description**  
Constructs an item at the back of the the vector 'in place'.  
If the vector is full then emits an `etl::vector_full`. If asserts or exceptions are not enabled undefined behaviour occurs.  
Before: `20.35.10`

---

```cpp
template <typename Args...>
reference emplace_back(Args&&... args)
```
**Description**  
Constructs an item at the back of the the vector 'in place'.  
Supports up to four constructor parameters.  
If the vector is full then emits an `etl::vector_full`. If asserts or exceptions are not enabled undefined behaviour occurs.  
Since: `20.35.10`

---

```cpp
void pop_back()
```
**Description**  
Pop a value from the back of the vector.
If the vector is empty and `ETL_CHECK_PUSH_POP` is defined then emits an `etl::vector_empty`.  
If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
template <typename TIterator>
void insert(iterator position, TIterator begin, TIterator end)
```
**Description**  
Insert the range [`begin`, `end`) at `position`.  
Before: `20.19.0`

---

```cpp
template <typename TIterator>
iterator insert(const_iterator position, TIterator begin, TIterator end)
```
**Description**  
Insert the range [`begin`, `end`) at `position`.  
Since: `20.20.0`

**Return** 
`position`

---

```cpp
iterator insert(iterator position, const T& value)
iterator insert(iterator position, T&& value)
```
**Description**  
Insert `value` at `position`.  

**Return** 
`position`

---

```cpp
iterator insert(const_iterator position, const T& value)
iterator insert(const_iterator position, T&& value)
```
**Description**  
Insert `value` at `position`.  

**Return** 
`position`

---

```cpp
void insert(iterator position, size_t n, const T& value)
```
**Description**  
Inserts `n` copies of `value` at `position`.

---

```cpp
iterator insert(const_iterator position, size_t n, const T& value)
```
**Description**  
Inserts values in to the vector. 
If the vector is full then emits an `etl::vector_full` exception. If asserts or exceptions are not enabled undefined behaviour occurs.

---

```cpp
template <typename TIterator>
iterator erase(TIterator begin, TIterator end)
```
**Description**  
Erase elements in the range [`begin`, `end`).  

**Return**  
An iterator to the element after the erased range.

---

<=20.19.0
```cpp
iterator erase(iterator position)
```
**Description**  
Erases values in the vector.    
Iterators are not checked.  
Before: `20.20.0`

---

```cpp
iterator erase(iterator position)
iterator erase(const_iterator position)
```
**Description**  
Erases values in the vector.    
Iterators are not checked.  
Since: `20.20.0`

**Return**  
An iterator to the element after the erased element.

---

```cpp
void clear()
```
**Description**  
Clears the vector to a size of zero.

---

```cpp
void sort()
```
**Description**  
Sorts the elements using the 'less than' operator.

---

```cpp
template <typename TCompare>
void sort(TCompare compare)
```
**Description**  
Sorts the elements using the supplied compare.

---

```cpp
void sort(iterator first, iterator last)
```
**Description**  
Sorts a range of elements using the supplied 'less than' operator.

---

```cpp
template <typename TCompare>
void sort(iterator first, iterator last, TCompare compare)
```
**Description**  
Sorts a range of elements using the supplied `compare`.

---

```cpp
void stable_sort()
```
**Description**  
Stable sorts the elements using the 'less than' operator.

---

```cpp
template <typename TCompare>
void stable_sort(TCompare compare)
```
**Description**  
Stable sorts the elements using the supplied compare.

---

```cpp
void stable_sort(iterator first, iterator last)
```
**Description**  
Stable sorts a range of elements using the supplied 'less than' operator.

---

```cpp
template <typename TCompare>
void stable_sort(iterator first, iterator last, TCompare compare)
```
**Description**  
Stable sorts a range of elements using the supplied compare.

## Non-member functions
```cpp
operator ==  
```
**Description**  
`true` if the contents of the vectors are equal, otherwise `false`.

---

```cpp
operator !=  
```
**Description**  
`true` if the contents of the vectors are not equal, otherwise `false`.


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
