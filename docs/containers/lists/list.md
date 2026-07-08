---
title: "list"
weight: 1
---

{{< callout >}}
  Header: `list.h`  
  Similar to: `std::list`
{{< /callout >}}

A fixed capacity bidirectional list.

```cpp
etl::list<typename T, size_t SIZE>
etl::list_ext<typename T>
```

Inherits from `etl::ilist<T>`.  
`etl::ilist` may be used as a size independent pointer or reference type for any `etl::list` instance.  

{{< callout type="warning">}}
  Does not support the member function `swap`.
{{< /callout >}}

## Shared Pools

```cpp
etl::list<typename T>
```
This template may share pools with another `etl::list` of the same type.  
The list is initialised with the pool either at construction time or a call to `set_pool(etl::ipool& pool)`.
When pools are shared there are a few side effects that must be noted.  

`size()` and `empty()` will have O(N) complexity.  
For a normal `etl::list` they are O(1).  

Destruction of the container will always be O(N) regardless of whether the type store is trivially destructible or not.  

`max_size()` will return the potential maximum size of the list. The actual maximum size will dependent of how many elements the other lists sharing the pool have allocated.  

The pool must be declared with the list's `pool_type`.  

### Example
```cpp
// The element type
struct Point { int x; int y; };

// The list type
using List = etl::list_ext<Point>;

// The shared pool
etl::pool<List::pool_type, 10> pool;
```

## Template deduction guides
**C++17 and above**  

```cpp
template <typename... T>
etl::list(T...)
```

### Example
```cpp
etl::list data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```
Defines data as an list of `int`, of length `10`, containing the supplied data.

## Make template
**C++11 and above**  
```cpp
template <typename T, typename... TValues>
constexpr auto make_list(TValues&&... values)
```

### Example
```cpp
auto data = etl::make_list<int>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
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
iterator                Bi-directional iterator
const_iterator          Constant bi-directional iterator
reverse_iterator        ETL_OR_STD::reverse_iterator<iterator>
const_reverse_iterator  ETL_OR_STD::reverse_iterator<const_iterator>
```

## Constructors

```cpp
etl::list<typename T, const size_t SIZE>()
```

---

```cpp
etl::list<typename T, const size_t SIZE>(size_t initialSize, parameter_t value = T())
```

---

```cpp
etl::list<typename T, const size_t SIZE>(const etl::list<typename T, const size_t SIZE>&)
```

---

```cpp
etl::list<typename T, const size_t SIZE>(etl::list<typename T, const size_t SIZE>&&)
```

---

```cpp
template <typename TIterator>
etl::list<typename T, const size_t SIZE>(TIterator begin, TIterator end)
```
**Description**  
Emits an `etl::list_iterator` if the iterators are invalid. If asserts or exceptions are disabled then undefined behaviour occurs.

## For shared pool lists

```cpp
etl::list<typename T, const size_t SIZE>(etl::pool& pool)
```

```cpp
etl::list<typename T, const size_t SIZE>(size_t initialSize, parameter_t value, etl::pool& pool)
```

```cpp
template <typename TIterator>
etl::list<typename T, const size_t SIZE>(TIterator begin, TIterator end, etl::pool& pool)
```
**Description**  
Emits an `etl::list_iterator` if the iterators are invalid. If asserts or exceptions are disabled then undefined behaviour occurs.

### Copy constructor
**Implicit pool**  
Uses the pool from other.
```cpp
etl::list<typename T, const size_t SIZE>(etl::list<typename T, size_t SIZE>& other)
```

**Explicit pool**  
```cpp
etl::list<typename T, const size_t SIZE>(etl::list<typename T, size_t SIZE>& other, etl::pool& pool)
```

### Initialisation

if the list is sharing pools then a `set_pool(etl::pool&)` is available.  
If a pool has already been set then the list is first cleared before updating to the new pool.  

The pool instance can be retrieved by call `get_pool()`  

## Element access

```cpp
T& front()
const T& front() const
```
**Description**  
Returns a reference or const reference to the first element.  
Undefined behaviour if the list is empty.

---

```cpp
T& back()
const T& back() const
```
**Description**  
Returns a reference or const reference to the last element.  
Undefined behaviour if the list is empty.

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the list.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the list.

---

```cpp
reverse_iterator rbegin()
const_reverse_iterator rbegin() const
const_reverse_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the list.

---

```cpp
reverse_iterator rend()
const_reverse_iterator rend() const
const_reverse_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the list.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the list is zero, otherwise false.

---

```cpp
bool full() const
```
**Description**  
Returns true if the size of the list is SIZE, otherwise false.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the list.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the list.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the list.

## Modifiers

```cpp
template <typename TIterator>
void assign(TIterator begin, TIterator end)
```
**Description**  
Assigns from a range of values.

---

```cpp
void assign(size_t n, parameter_t value)
```
**Description**  
Fills the list with the values.

---

```cpp
void push_front(const T& value)
void push_front(T&& value)
```
**Description**  
Pushes a value to the front of the list.  
If the list is full then emits an `etl::list_full error`.

---

```cpp
emplace_front
```

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
**Description**  
Emplaces a value at the front, constructed using the supplied arguments.  
Before: `20.35.10`  

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
**Description**  
Emplaces a value at the front, constructed using the supplied arguments.  
Since: `20.35.10`  

### C++11 and above
```cpp
template <typename ... Args>
reference emplace_front(Args&& …args)
```
**Description**  
Emplaces a value at the front, constructed using the supplied arguments.  

---

```cpp
void push_back(const T& value);
void push_back(T&& value);
```
**Description**  
Pushes a value to the back of the list.  
If the list is full and `ETL_CHECK_PUSH_POP` is defined then emits an `etl::list_full` error, otherwise undefined behaviour occurs.

---

```cpp
emplace_back
```
**Description**  
Emplaces a value at the back, constructed using the supplied arguments.  

**C++03**  
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
Before: `20.35.10`

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
Since: `20.35.10`  

### C++11 and above
```cpp
template <typename ... Args>
reference emplace_back(Args&& ... args)
```
**Description**  
Emplaces a value at the back, constructed using the supplied arguments.  
From: `20.35.10`  

---

**C++11 and above**  
```cpp
template <typename ... Args>
reference emplace_back(Args&& ... args)
```
**Description**  
Emplaces a value at the back, constructed using the supplied arguments.  
From: `20.35.10`  

---

```cpp
void pop_front()
```
**Description**  
Pop a value from the front of the list.  
If the list is empty and `ETL_CHECK_PUSH_POP` is defined then emits an `etl::list_empty` error, otherwise undefined behaviour occurs.

---

```cpp
void pop_back()
```
**Description**  
Pop a value from the back of the list.  
If the list is empty and `ETL_CHECK_PUSH_POP` is defined then emits an `etl::list_empty` error, otherwise undefined behaviour occurs.

---

```cpp
template <typename TIterator>
void insert(iterator position, TIterator begin, TIterator end)
```
**Description**  
Before: `20.20.0`

---

```cpp
iterator insert(iterator position, parameter_t value);
void insert(iterator position, size_t n, parameter_t value)
```

```cpp
template <typename TIterator>
iterator insert(const_iterator position, TIterator begin, TIterator end)
```
**Description**  
Since `20.20.0` 

---

```cpp
iterator insert(const_iterator position, parameter_t value)
iterator insert(const_iterator position, size_t n, parameter_t value)
```
**Description**  
Inserts values in to the list. If the list is full then emits an `etl::list_full` error.

---

```cpp
emplace
```
**Description**  
Constructs an item at the insert point in the list 'in place'.  

### C++03
```cpp
void emplace(iterator position, const T1& value1)

void emplace(iterator position, const T1& value1, const T2& value2)

void emplace(iterator position, const T1& value1, const T2& value2, const T3& value3)

void emplace(iterator position, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
Before: `20.20.0`  
```cpp
iterator emplace(const_iterator position, const T1& value1)

iterator emplace(const_iterator position, const T1& value1, const T2& value2)

iterator emplace(const_iterator position, const T1& value1, const T2& value2, const T3& value3)

iterator emplace(const_iterator position, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
From: `20.20.0`  

### C++11
```cpp
void emplace(iterator position, Args&& ... args)
```
**Description**  
Constructs an item at the insert point in the the list 'in place'.  
Before: `20.20.0`  

```cpp
void emplace(const_iterator position, Args&& ... args)
```
**Description**  
Constructs an item at the insert point in the the list 'in place'.  
From: `20.20.0`  

---

```cpp
template <typename TIterator>
iterator erase(TIterator begin, TIterator end)
```

---

```cpp
iterator erase(iterator position)
```
**Description**  
Before: `20.20.0`

---

```cpp
iterator erase(iterator position)
iterator erase(const_iterator position)
```
**Description**  
Erases values in the list.
Before: `20.21.0`  

---

```cpp
void resize(size_t n);
void resize(size_t n, parameter_t value)
```
**Description**  
Resizes the list. If the new size is larger then the first assigns default constructed values, the second assigns the supplied value.  
If `n` is larger than the capacity then emits an `etl::list_full` error, if asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
void clear();
```
**Description**  
Clears the list to a size of zero.

---

```cpp
void splice(iterator to, etl::ilist& other)
void splice(iterator to, etl::ilist&& other)
```
**Description**  
Moves the elements in list other to before the position to.  
The operation performs copies between the different lists.

---

```cpp
void splice(iterator to, etl::ilist& other, iterator from)
void splice(iterator to, etl::ilist&& other, iterator from)
```
**Description**  
Moves the element at position from in list other to before the position to.  
The operation is fast when spicing within the same list, otherwise performs copies between different lists.  
If from is not valid then undefined behaviour occurs.

---

```cpp
void splice(iterator to, etl::ilist& other, iterator first, iterator last)
void splice(iterator to, etl::ilist&& other, iterator first, iterator last)
```
**Description**  
Moves the elements in the range first to one before last in list other to before the position to.  
The operation is fast when spicing within the same list, otherwise performs copies between different lists.

---

```cpp
void merge(etl::ilist& other)
void merge(etl::ilist&& other)
```
**Description**  
Merges the elements in list other to this list.  
The lists must be sorted. If a debug compile and asserts or exceptions are enabled than an etl::list_unsorted is emitted if either list is unsorted, otherwise undefined behaviour occurs.

---

```cpp
template <typename TCompare>
void merge(etl::ilist& other, TCompare compare)
```

---

```cpp
template <typename TCompare>
void merge(etl::ilist&& other, TCompare compare)
```
**Description**  
Merges the elements in list other to this list using the supplied comparison function to determine order.
The lists must already be sorted according to the compare function.
If a debug compile and asserts or exceptions are enabled than an etl::list_unsorted is emitted if either list is unsorted, otherwise undefined behaviour occurs.

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

```cpp
template <typename TCompare>
void sort(TCompare compare)
```
**Description**  
Sorts using the supplied compare function.

---

```cpp
void reverse()
```
**Description**  
Reverses the order of the list.

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


## Shared pool example

```cpp
// The element type.
struct Point { int x; int y; };

// The list type
using List = etl::list_ext<Point>;

// The shared pool. Maximum of 10 items.
etl::pool<List::pool_type, 10> pool;

// The vector of lists with shared pools.
etl::vector<List, 3> vector_of_lists(3, List(pool));

// Make some data.
Point point = { 1, 2 };

// Push one to each list.
vector_of_lists[0].push_back(point);
vector_of_lists[1].push_back(point);
vector_of_lists[2].push_back(point);

size_t available = 0;

available = vector_of_lists[0].available(); // Reports '7'
available = vector_of_lists[1].available(); // Reports '7'
available = vector_of_lists[2].available(); // Reports '7'
```
