---
title: "forward_list"
weight: 3
---

{{< callout >}}
  Header: `forward_list.h`  
  Similar to: `std::forward_list`
{{< /callout >}}

A fixed capacity forward list.

```cpp
etl::forward_list<typename T, size_t SIZE>
etl::forward_list_ext<typename T>
```

Inherits from `etl::iforward_list<T>`.  
`etl::iforward_list` may be used as a size independent pointer or reference type for any `etl::forward_list` instance.  

**Note:** Does not support the member function swap.

## Shared Pools

`etl::forward_list_ext<typename T>`  
This template may share pools with another `etl::forward_list` of the same type.  
The list is initialised with the pool either at construction time or a call to `set_pool(etl::ipool& pool)`.  
When pools are shared there are a few side effects that must be noted.  

`size()` and `empty()` will be O(N) complexity. For a normal `etl::forward_list` they are O(1).  

Destruction of the container will always be O(N) regardless of whether the type store is trivially destructible or not.  

`max_size()` will return the potential maximum size of the list. The actual maximum size will dependent of how many elements the other lists sharing the pool have allocated.  

Pool must be declared with the list's `pool_type`.  

### Example
```cpp
// The element type
struct Point { int x; int y; };

// The list type
using List = etl::forward_list_ext<Point>;

// The shared pool
etl::pool<List::pool_type, 10> pool;
```

## Template deduction guides
**C++17 and above**  

```cpp
template <typename T...>
etl::forward_list(T...)
```

### Example
```cpp
etl::forward_list data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```

Defines data as an `forward_list` of int, of length 10, containing the supplied data.

## Make template
**C++11 and above**  
```cpp
template <typename T, typename... TValues>
  constexpr auto make_forward_list(TValues&&... values)
```

### Example
```cpp
auto data = etl::make_forward_list<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
```

## Member types

```cpp
value_type              T
size_type               std::size_t
difference_type         std::ptrdiff_t
reference               value_type&
const_reference         const value_type&
pointer                 value_type*
const_pointer           const value_type*
iterator                Forward iterator
const_iterator          Constant forward iterator
```

## Constructors

```cpp
etl::forward_list<typename T, const size_t SIZE>()
```

---

```cpp
etl::forward_list<typename T, const size_t SIZE>(size_t initialSize, parameter_t value = T())
```

---

```cpp
template <typename TIterator>
etl::forward_list<typename T, const size_t SIZE>(TIterator begin, TIterator end)
```

Raises an `etl::forward_list_iterator` if the iterators are invalid. Raises an `etl::forward_list_full` if the list becomes full.  
If asserts or exceptions are disabled then undefined behaviour occurs.

## For shared pool lists
```cpp
etl::forward_list<typename T, const size_t SIZE>(etl::pool& pool)
```

---

```cpp
etl::forward_list<typename T, const size_t SIZE>(size_t initialSize, parameter_t value, etl::pool& pool)
```

---

```cpp
template <typename TIterator>
etl::forward_list<typename T, const size_t SIZE>(TIterator begin, TIterator end, etl::pool& pool)
```

Raises an `etl::forward_list_iterator` if the iterators are invalid. If asserts or exceptions are disabled then undefined behaviour occurs.  

## Copy constructor
**Implicit pool**  
Uses the pool from other.  
```cpp
etl::forward_list<typename T, const size_t SIZE>(etl::forward_list<typename T, const size_t SIZE>& other)
```

**Explicit pool**  
```cpp
etl::forward_list<typename T, const size_t SIZE>(etl::forward_list<typename T, const size_t SIZE>& other, etl::pool& pool)
```

## Element access

```cpp
T& front()
const T& front() const
```
**Description**  
Returns a reference or const reference to the first element.  
Undefined behaviour if the list is empty.

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the forward list.
Returns end() if the list is empty.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the forward list.

---

```cpp
iterator before_begin()
const_iterator before_begin() const
const_iterator cbefore_begin() const
```
**Description**  
Returns an iterator to before the beginning of the forward list.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns `true` if the size of the forward list is zero, otherwise `false`.

---

```cpp
bool full() const
```
**Description**  
Returns `true` if the size of the forward list is SIZE, otherwise `false`.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the forward list.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the forward list.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the forward list .

## Modifiers

```cpp
template <typename TIterator>
void assign(TIterator begin, TIterator end)

void assign(size_t n, parameter_t value)
```
**Description**  
Fills the forward list with the values.  
Raises an `etl::forward_list_iterator` if the iterators are invalid. 
Raises an `etl::forward_list_full` if the list becomes full. 
If asserts or exceptions are disabled then undefined behaviour occurs.

---

```cpp
void push_front(parameter_t value)
```
**Description**  
Pushes a value to the front of the forward list. If the forward list is full and ETL_CHECK_PUSH_POP is defined then raises an etl::forward_list_full error, otherwise undefined behaviour occurs.

---

```cpp
emplace_front
```
**Description**  
Constructs an item at the front of the the list 'in place'.  
If the forward list is full and `ETL_CHECK_PUSH_POP` is defined then raises an `etl::forward_list_full` error, otherwise undefined behaviour occurs.

### C++03
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
Before: `20.35.0`

## C++11 and above

```cpp
template <typename ... Args>
void emplace_front(Args&& ... args)
```
**Description**  
Constructs an item at the front of the the list 'in place'.  
If the forward list is full and `ETL_CHECK_PUSH_POP` is defined then raises an `etl::forward_list_full` error, otherwise undefined behaviour occurs.

### C++03
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
From 20.35.10
---

### C++11 and above
```cpp
template <typename ... Args>
reference emplace_front(Args&& …args)
```

---

```cpp
void pop_front()
```
**Description**  
Pop a value from the front of the forward list.  
If the forward list is empty and `ETL_CHECK_PUSH_POP` is defined then raises an `etl::forward_list_empty` error, otherwise undefined behaviour occurs.

---

```cpp
template <typename TIterator>
void insert_after(iterator position, TIterator begin, TIterator end)
```
**Description**  
Inserts the range [`begin`, `end`) into the forward list after the specified position. 
Before: `20.20.0`

---

```cpp
iterator insert_after(iterator position, parameter_t value)
void insert_after(iterator position, size_t n, parameter_t value)
```
**Description**  
Inserts `value` in to the forward list after the specified position. 

---

```cpp
template <typename TIterator>
iterator insert_after(const_iterator position, TIterator begin, TIterator end)
```
**Description**  
Inserts the range [`begin`, `end`) into the forward list after the specified position. 
Since: `20.20.0`

---

```cpp
iterator insert_after(const_iterator position, parameter_t value)
```
**Description**  
Inserts `value` in to the forward list after the specified position. 

---

```cpp
iterator insert_after(const_iterator position, size_t n, parameter_t value)
```
Inserts values in to the forward list after the specified position.  
If the forward list is full then raises an `etl::forward_list_full error`.  
If asserts or exceptions are disabled then undefined behaviour occurs.

---

```cpp
iterator erase_after(iterator position)
```
Erases elements after the specified position.  
Before: `20.20.0`

---

```cpp
iterator erase_after(iterator position, const_iterator end)
```
**Description**  
Erases the range (`position`, `end`)
Before: `20.20.0`

---

```cpp
iterator erase_after(const_iterator position)
```
**Description**  
Erases elements after the specified position.
Since: `20.20.0`

---

```cpp
iterator erase_after(const_iterator position, const_iterator end)
```
**Description**  
Erases the range (`position`, `end`)
Since: `20.20.0`

---

```cpp
void resize(size_t n)
```
**Description**  
Resizes the forward list.  
If the new size is larger then the first assigns default constructed values.  
If n is larger than the capacity then raises an `etl::forward_list_full error`, if asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
void resize(size_t n, parameter_t value)
```
**Description**  
Resizes the forward list.  
If the new size is larger then the first assigns the supplied value.  
If n is larger than the capacity then raises an `etl::forward_list_full error`, if asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
void clear()
```
**Description**  
Clears the forward list to a size of zero.

## Operations

```cpp
void remove(const T& value)
```
**Description**  
Removes from the container all the elements that compare equal to value.

---

```cpp
template <typename TPredicate>
void remove_if(TPredicate predicate)
```
**Description**  
Removes from the container all the elements that satisfy predicate.

---

```cpp
void unique()
```
**Description**  
Removes all but the first element from every group of consecutive elements.

---

```cpp
template <typename TPredicate>
void unique(TPredicate predicate)
```
**Description**  
Removes all but the first element from every group of consecutive elements that satisfy the binary predicate.

---

```cpp
void sort()
```
**Description**  
Sorts using the `<` operator.  

---

```cpp
template <typename TCompare>
void sort(TCompare compare)
```
**Description**  
Sorts using the supplied `compare` function.

---

```cpp
void reverse()
```
**Description**  
Reverses the order of the container.

## Non-member functions

```cpp
operator ==
```
**Return**  
`true` if the contents of the lists are equal, otherwise `false`.  

---

```cpp
operator !=
```
**Return**  
`true` if the contents of the lists are not equal, otherwise `false`.  

---

```cpp
operator <
```
**Return**  
`true` if the contents of the lhs are lexicographically less than the contents of the rhs,  otherwise `false`.  

---

```cpp
operator <=
```
**Return**  
`true` if the contents of the lhs are lexicographically less than or equal to the contents of the rhs, otherwise `false`.  

---

```cpp
operator >
```
**Return**  
`true` if the contents of the lhs are lexicographically greater than the contents of the rhs,  otherwise `false`.  

---

```cpp
operator >=
```
**Return**  
`true` if the contents of the lhs are lexicographically greater than or equal to the contents of the rhs, otherwise `false`.  
