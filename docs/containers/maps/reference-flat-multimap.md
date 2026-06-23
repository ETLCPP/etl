---
title: "reference_flat_multimap"
---

{{< callout type="info">}}
  Header: `reference_flat_multimap.h`  
  Similar to: `std::reference_flat_multimap`
{{< /callout >}}

A fixed capacity reference map based on a sorted vector.  
The container stores references to objects, rather than the objects themselves.  
The container is an associative lookup table with O(N) insertion and erase, and O(log N) search.  
This container is best used for tables that are occasionally updated and spend most of their time being searched.  
Uses `etl::less` as the default key comparison method.

```cpp
etl::reference_flat_multimap<typename TKey, typename TMapped, const size_t SIZE, TKeyCompare = less>
```

Inherits from `etl::ireference_flat_multimap<TKey, TMapped, TKeyCompare>`.  
`etl::ireference_flat_multimap` may be used as a size independent pointer or reference type for any `etl::reference_flat_multimap` instance.

## Template deduction guides
C++17 and above

```cpp
template <typename... TPairs>
etl::reference_flat_multimap(TPairs...)
```

### Example

```cpp
etl::reference_flat_multimap data{ etl::pair{0, 1}, etl::pair{2, 3}, etl::pair{4, 5}, etl::pair{6, 7} };
```
Defines data as an reference_flat_multimap of int, of length 4, containing the supplied data.

## Make template
C++11 and above

```cpp
template <typename TKey, 
          typename TMapped, 
          typename TKeyCompare = etl::less<TKey>, 
          typename... TPairs>
constexpr auto make_reference_flat_map(TValues&&... values)
```

### Example
auto data = etl::make_reference_flat_multimap<int, int>(etl::pair{0, 1}, etl::pair{2, 3},
                                                   etl::pair{4, 5}, etl::pair{6, 7});

## Member types

```cpp
key_type                TKey
mapped_type             TMapped
value_type              pair<key_type, mapped_type>
size_type               size_t
difference_type         ptrdiff_t
reference               value_type&
const_reference         const value_type&
pointer                 value_type*
const_pointer           const value_type*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        ETL_OR_STD::reverse_iterator<iterator>
const_reverse_iterator  ETL_OR_STD::reverse_iterator<const_iterator>
```

## Constructor

```cpp
etl::reference_flat_multimap<Tkey, TMapped, SIZE, TKeyCompare>()
```
**Description**  
Default constructor.

---

```cpp
etl::reference_flat_multimap(const flat_map& other)
```
**Description**  
Copy constructor.

---

```cpp
template <typename TIterator>
etl::reference_flat_multimap<Tkey, TMapped, SIZE, TKeyCompare>(TIterator begin, TIterator end)
```
**Description**  
If the map is full then raises an `etl::reference_flat_map_full`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

## Element access

### at
```cpp
TMapped& at(const key_type& key)
const TMapped& at(const key_type& key) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Raises an `etl::flat_map_out_of_range` if the key is not in the table.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
template <typename K, typename KC = TKeyCompare>
mapped_type& at(const K& key)
const mapped_type& at(const K& key) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Raises an `etl::flat_map_out_of_range` if the key is not in the table.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

### find

```cpp
template <typename K>
iterator find(const key_type& key)
const_iterator find(const key_type& key) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Raises an `etl::flat_map_out_of_range` if the key is not in the table.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
template <typename K>
iterator find(const K& key)
const_iterator find(const K& key) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Raises an `etl::flat_map_out_of_range` if the key is not in the table.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

### lower_bound

```cpp
iterator lower_bound(const key_type& key)
const_iterator lower_bound(const key_type& key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is not considered to go before key (i.e., either it is equivalent or goes after).

---

```cpp
template <typename K, typename KC = TKeyCompare>
iterator lower_bound(const K& key)
const_iterator lower_bound(const K& key) const
```
Returns an iterator pointing to the first element in the container whose key is not considered to go before key (i.e., either it is equivalent or goes after).

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

### upper_bound

```cpp
iterator upper_bound(const key_type& key)
const_iterator upper_bound(const key_type& key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is considered to go after `key`.

---

```cpp
template <typename K, typename KC = TKeyCompare>
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
pair<iterator, iterator> equal_range(const key_type& key)
pair<const_iterator, const_iterator> equal_range(const key_type& key) const
```
**Description**  
Returns the bounds of a range that includes all the elements in the container which have a key equivalent to `key`.

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

---

```cpp
template <typename K, typename KC = TKeyCompare>
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
bool contains(const value_type& key) const
```
**Description**  
Check if the container contains the key.
Since: `20.21.0`

---

```cpp
template <typename K, typename KC = TKeyCompare>
bool contains(const K& k) const
```
**Description**  
Check if the container contains the key.

C++11 or above.  
For comparators that define `is_transparent`.  
Since: `20.21.0`

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the map.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the map.

---

```cpp
iterator rbegin()
const_iterator rbegin() const
const_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the map.

---

```cpp
iterator rend()
const_iterator rend() const
const_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the map.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns `true` if the size of the map is zero, otherwise `false`.

---

```cpp
bool full() const
```
**Description**  
Returns `true` if the size of the lookup is SIZE, otherwise `false`.

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
Returns the maximum possible size of the map.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the map.

## Modifiers

```cpp
reference_flat_multimap& operator = (const reference_flat_multimap& rhs)
```
**Description**  
Copies the data from another flat map.

---

```cpp
pair<iterator, bool> insert(const value_type& value)
```
**Description**  
Inserts `value` into the map.

---

```cpp
iterator insert(iterator position, const value_type& value)
```
**Description**  
Inserts `value` into the map at th esuggested `position`.

---

```cpp
template <typename TIterator>
void insert(TIterator first, TIterator last)
```
**Description**  
Inserts values in to the map. If the map is full then raises an `etl::flat_map_full`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
size_t erase(const key_type& key)
```
**Description**  
Erase the element with `key`.

---

```cpp
void erase(iterator i_element)
```
**Description**  
Erase the element at `i_element`.

---

```cpp
void erase(iterator first, iterator last)
```
**Description**  
Erase elements in the range [`first`, `last`).

---

```cpp
template <typename K>
size_t erase(K&& key)
```
**Description**  
Erases values in the map.  
Since: `20.21.0`

---

```cpp
void clear()
```
**Description**  
Clears the lookup to a size of zero.  
Iterator are not checked for validity.

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

## Technical stuff

Reference flat maps are different from the normal version in that the elements are not copied, but linked directly.  
This means that the lifetime of the element inserted must be as great as that of the map that contains it.  
Unlike most other reference containers, the map has a finite capacity.  

Flat maps are usually implemented internally as a sorted vector of key/value pairs. Whilst this makes searching fast, it can have a detrimental effect when items are inserted into a container that stores complex, non-trivial keys or values.  
As inserting requires that all of the items above the insert position must be shifted, this can become an expensive operation for larger containers.  

To improve insertion performance ETL reference flat maps are implemented as vectors of pointers to key/value pairs, sorted by key value. An insertion will involve a copy of a range of pointers; an operation that can be made very fast.  

The downside is that access to an item via an iterator will involve one indirection and the overhead of the container will be one pointer per item. A normal flat map implementation does not have this overhead.  

