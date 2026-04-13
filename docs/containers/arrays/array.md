---
title: "array"
weight: 1
---

{{< callout >}}
  Header: `array.h`  
  Since: All versions  
  Similar to: [std::array](https://en.cppreference.com/w/cpp/container/array.html)
{{< /callout >}}

A fixed capacity array.
Adds additional members functions, `assign`, `insert` & `erase`.

```cpp
etl::array<typename T, const size_t SIZE>
```

See also
[array_view]()

## Template deduction guides
**C++17 and above**

```cpp
template <typename... T>
etl::array(T...)
```

### Example
```cpp
etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```
Defines data as an array of 'int', of length '10', containing the supplied data.

## Make template
**C++11 and above**

```cpp
template <typename T, typename... TValues>
constexpr auto make_array(TValues&&... values)
```

### Example
```cpp
auto data = etl::make_array<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
```

## Member types

`value_type`  
`size_type`  
`difference_type`  
`reference`  
`const_reference`  
`pointer`  
`const_pointer`  
`iterator`  
`const_iterator`  
`reverse_iterator`  
`const_reverse_iterator`

'etl::array' iterators are random access.

## Static Constants

```cpp
SIZE
```
**Description**  
The size of the array.

## Constructor

```cpp
etl::array<int, 10> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```
**Description**  
Default or copy constructs each element.
Can be initialised like a C array.

## Element access
```cpp
T& at(size_t i)
const T& at(size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element. Emits an etl::array_out_of_range if the index is not in range.
If asserts or exceptions are not enabled then an out of range index results in undefined behaviour.

---

```cpp
T& operator[](size_t i)
const T& operator[](size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.
An out of range index results in undefined behaviour.

---

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

## Iterators

```cpp
iterator begin()  
const_iterator begin() const  
const_iterator cbegin() const  
```
**Description**  
Returns an iterator to the beginning of the array.

---

```cpp
iterator end()  
const_iterator end() const  
const_iterator cend() const  
```
**Description**  
Returns an iterator to the end of the array.

---

```cpp
iterator rbegin()  
const_iterator rbegin() const  
const_iterator crbegin() const  
```
**Description**  
Returns a reverse iterator to the beginning of the array.

---

```cpp
iterator rend()  
const_iterator rend() const  
const_iterator crend() const  
```
**Description**  
Returns a reverse iterator to the end of the array.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the array is zero, otherwise false.

---

```cpp
size_t size() const  
```
**Description**  
Returns the size of the array.

---

```cpp
size_t max_size() const  
```
**Description**  
Returns the maximum possible size of the array.

## Modifiers

```cpp
void fill(T value)  
```
**Description**  
Fills the array with value.

## ETL Extensions

```cpp
template <typename TIterator>
void assign(TIterator first, const TIterator last)
```

```cpp
template <typename TIterator>
void assign(TIterator first, const TIterator last, parameter_t value)  
```

**Description**  
Assigns a range of values to the array. The second form set uninitialised elements to value.  
If the range is larger than the array then the extra data is ignored.  
If the range is smaller than the array then the unused array elements are left unmodified.

---

```cpp
iterator insert_at(size_t position, parameter_t value)  
iterator insert(const_iterator position, parameter_t value)  
```

**Description**  
Inserts a value into the array at the specified position, shifting elements to make room.
Elements may be truncated if they are shifted off the end of the array.

---

```cpp
template <typename TIterator>
iterator insert_at(size_t position, TIterator first, const TIterator last)
```

**Description**  
`position` is not checked for validity.

---

```cpp
template <typename TIterator>
iterator insert(const_iterator position, TIterator first, const TIterator last)
```

**Description**  
Inserts a range of values into the array at the specified position, shifting elements to make room.  
Elements may be truncated if they are shifted off the end of the array.  
The range may be larger than the capacity of the array. Excess elements will be ignored.  

---

```cpp
iterator erase_at(size_t position)
iterator erase_at(size_t position, parameter_t value)
iterator erase(const_iterator position)
iterator erase(const_iterator position, parameter_t value)
```

**Description**  
Erases an element at the specified position, shifting elements into the space.  
The value parameter variants initialise the unused elements with value, while the others leave them unchanged.  
position is not checked for valid range.  

---

```cpp
iterator erase_range(size_t first, size_t last)
iterator erase_range(size_t first, size_t last, parameter_t value)
iterator erase(const_iterator first, const_iterator last)
iterator erase(const_iterator first, const_iterator last, parameter_t value)
```

**Description**  
Erases a range, shifting elements into the space.
The value parameter variants initialise the unused elements with value, while the others leave them unchanged.
first/last are not checked for valid range.

## Non-member functions
**Lexicographically comparisons**

`operator ==`  
**Returns**  
`true` if the contents of the arrays are equal, otherwise `false`.  

`operator !=`  
**Returns**  
`true` if the contents of the arrays are not equal, otherwise `false`.  

`operator <`  
**Returns**    
`true` if the contents of the lhs are lexicographically less than the  
contents of the rhs,  otherwise `false`.  

`operator <=`  
**Returns**  
`true` if the contents of the lhs are lexicographically less than or equal to the  
contents of the rhs, otherwise `false`.  

`operator >`  
**Returns**  
`true` if the contents of the lhs are lexicographically greater than the  
contents of the rhs,  otherwise `false`.  

`operator >=`  
**Returns**  
`true` if the contents of the lhs are lexicographically greater than or equal to the  
contents of the rhs, otherwise `false`.  
