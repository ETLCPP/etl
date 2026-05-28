---
title: "reference_flat_set"
---

{{< callout type="info">}}
  Header: `reference_flat_set.h`  
  Similar to: `std::set`
{{< /callout >}}

A fixed capacity set based on a sorted vector.  
The container stores references to objects, rather than the objects themselves.  
The container is an associative lookup table with O(N) insertion and erase, and O(log N) search.  
This container is best used for tables that are occasionally updated and spend most of their time being searched.  
The interface is most similar to `std::set`.  
Uses `etl::less` as the default key comparison method.  

```cpp
etl::reference_flat_set<typename T, const size_t SIZE, TCompare = etl::less>
```

Inherits from `etl::ireference_flat_set<T, TCompare>`.  
`etl::ireference_flat_set` may be used as a size independent pointer or reference type for any `etl::reference_flat_set` instance.

## Template deduction guides
C++17 and above

template <typename... T>
etl::reference_flat_set(T...)

### Example
etl::reference_flat_setdata{ 0, 1, 2, 3, 4, 5, 6, 7 };
Defines data as a set of int/int pairs, of length 8, containing the supplied data.

## Make template
C++11 and above
template <typename TKey
          typename... T>
constexpr auto make_reference_flat_set(T&&... values)

### Example
auto data = etl::make_reference_flat_set<int>(0, 1, 2, 3, 4, 5, 6, 7);

## Member types

```cpp
key_type                T
value_type              T
size_type               size_t
difference_type         ptrdiff_t
reference               value_type&
const_reference         const value_type&
pointer                 value_type*
const_pointer           const value_type*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        ETL_OR_STD:reverse_iterator<iterator>
const_reverse_iterator  ETL_OR_STD::reverse_iterator<const_iterator>
```

## Constructor

```cpp
etl::flat_set<T, SIZE, TCompare>();
```
**Description**  

---

```cpp
etl::flat_set(const flat_set& other)
```
**Description**  

---

```cpp
template <typename TIterator>
etl::flat_set<T, SIZE, TCompare>(TIterator begin, TIterator end);
```
**Description**  
If the set is full then raises an `etl::reference_flat_set_full`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

## Iterators
```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the set.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the set.

---

```cpp
iterator rbegin()
const_iterator rbegin() const
const_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the set.

---

```cpp
iterator rend()
const_iterator rend() const
const_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the set.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns `true` if the size of the set is zero, otherwise `false`.

---

```cpp
bool full() const
```
**Description**  
Returns `true` if the size of the lookup is `SIZE`, otherwise `false`.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the lookup.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the set.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the set.

## Modifiers

```cpp
flat_set& operator = (const flat_set& rhs)
```
**Description**  
Copies the data from another flat set.

---

```cpp
ETL_OR_STD::pair<iterator, bool> insert(const value_type& value)
iterator insert(iterator position, const value_type& value)
```
**Description**  

---

```cpp
template <typename TIterator>
void insert(TIterator first, TIterator last)
```
**Description**  
Inserts values in to the set.  
If the set is full then raises an `etl::reference_flat_set_full`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
size_t erase(const value_type& key)
void erase(iterator i_element)
void erase(iterator first, iterator last)
```
**Description**  
Erases values in the set.  
Iterators are not checked for validity.

Since: 20.21.0
```cpp
template <typename K>
size_t erase(K&& key)
```
**Description**  
Erases values in the map.

---

```cpp
void clear()
```
**Description**  
Clears the lookup to a size of zero.

## Search

### find

```cpp
iterator find(const value_type& key)
const_iterator find(const value_type& key) const
```
**Description**  
Finds the element that contains `key`, or `end()` if not found,


```cpp
template <typename K>
iterator find(const K& key)
const_iterator find(const K& key) const
```
**Description**  
Finds the element that contains `key`, or `end()` if not found,

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

### lower_bound

```cpp
iterator lower_bound(const value_type& key)
const_iterator lower_bound(const value_type& key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is not considered to go before `key` (i.e., either it is equivalent or goes after).

---

Since: 20.21.0
C++11 or above
```cpp
template <typename K>
iterator lower_bound(const K& key)
const_iterator lower_bound(const K& key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is not considered to go before `key` (i.e., either it is equivalent or goes after).

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

### upper_bound

```cpp
iterator upper_bound(const value_type& key)
const_iterator upper_bound(const value_type& key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is considered to go after `key`.

---

```cpp
template <typename K>
iterator upper_bound(const K& key)
const_iterator upper_bound(const K& key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is considered to go after `key`.

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

### equal_range

```cpp
ETL_OR_STD::pair<iterator, iterator> equal_range(const value_type& key)
ETL_OR_STD::pair<const_iterator, const_iterator> equal_range(const value_type& key) const
```
**Description**  
Returns the bounds of a range that includes all the elements in the container which have a key equivalent to `key`.

---

```cpp
template <typename K>
pair<iterator, iterator> equal_range(const K& key)
pair<const_iterator, const_iterator> equal_range(const K& key) const
```
**Description**  
Returns the bounds of a range that includes all the elements in the container which have a key equivalent to `key`.

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

### contains

```cpp
bool contains(const value_type&  key) const
```
**Description**  
Check if the container contains the key.

Since: 20.21.0
C++11 or above
For comparators that define is_transparent.

```cpp
template <typename K>
bool contains(const K& k) const
```
**Description**  
Check if the container contains the key.

Since: 20.21.0
C++11 or above
For comparators that define is_transparent.

## Non-member functions
Lexicographically comparisons

```cpp
operator ==
```
**Description**  
`true` if the contents of the maps are equal, otherwise `false`.

---

```cpp
operator !=
```
**Description**  
`true` if the contents of the maps are not equal, otherwise `false`.

## How it works

Reference flat sets are different from the normal version in that the elements are not copied, but linked directly.  
This means that the lifetime of the element inserted must be as great as that of the set that contains it.  
Unlike most other reference containers, the set has a finite capacity.  

Flat sets are usually implemented internally as a sorted vector of values. Whilst this makes searching fast, it can have a detrimental effect when items are inserted into a container that stores complex, non-trivial values.  
As Inserting requires that all of the items above the insert position must be shifted, this can become an expensive operation for larger containers.  

To improve insertion performance ETL reference flat sets are implemented as vectors of pointers to values, sorted by value. An insertion will involve a copy of a range of pointers; an operation that can be made very fast.  

The downside is that access to an item via an iterator will involve one indirection and the overhead of the container will be one pointer per item. A normal flat set implementation does not have this overhead.  
