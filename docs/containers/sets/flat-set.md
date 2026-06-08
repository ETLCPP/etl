---
title: "flat_set"
---

{{< callout type="info">}}
  Header: `flat_set.h`  
  Similar to: `std::set`
{{< /callout >}}

A fixed capacity set based on a sorted vector.  
The container is an associative lookup table with O(N) insertion and erase, and O(log N) search.  
This container is best used for tables that are occasionally updated and spend most of their time being searched.  
Uses `etl::less` as the default key comparison method.  

```cpp
etl::flat_set<typename T, size_t SIZE, TKeyCompare = etl::less>
```

Inherits from `etl::iflat_set<T, TKeyCompare>`.
`etl::iflat_set` may be used as a size independent pointer or reference type for any `etl::flat_set` instance.

## Template deduction guides
C++17 and above

```cpp
template <typename... T>
etl::flat_set(T...)
```

### Example
```cpp
etl::flat_set data{ 0, 1, 2, 3 };
```
Defines data as an `flat_set` of `int`, of length 4, containing the supplied data.

## make_flat_set
C++11 and above
```cpp
template <typename T,
          typename TKeyCompare = etl::less<T>>
constexpr auto make_flat_set(TValues&&... values)
```

### Example
```cpp
auto data = etl::make_flat_set<int>({0, 1, 2, 3 });
```

## Member types

```cpp
key_type                T
value_type              T
size_type               std::size_t
difference_type         std::ptrdiff_t
reference               reference
const_reference         const_reference
rvalue_reference        reference&
pointer                 T*
const_pointer           const T*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        reverse_iterator<iterator>
const_reverse_iterator  reverse_iterator<const_iterator>
```

## Static Constants

`MAX_SIZE`  The maximum size of the flat set.

## Constructors

```cpp
etl::flat_set<Tkey, SIZE, TKeyCompare>()
```
**Description**  
Default constructor.

---

```cpp
etl::flat_set(const flat_set& other)
```
**Description**  
Copy constructor.

---

```cpp
etl::flat_set(flat_set&& other)
```
**Description**  
Move constructor.

---

```cpp
template <typename TIterator>
etl::flat_set<Tkey, SIZE, TKeyCompare>(TIterator begin, TIterator end);
```
**Description**  
Construct from the range [`begin`, `end`).

## Element access

```cpp
reference at(const_reference key)
const_reference at(const_reference key) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Emits an `etl::flat_set_out_of_range` if the key is not in the table.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
reference operator[](const_reference key)
const_reference operator[](const_reference key) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
If the key is not in the table then a new entry is created.

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
flat_set& operator = (flat_set&& rhs)
```
**Description**  
Copies or moves the data from another flat set.

---

```cpp
pair<iterator, bool> insert(const_reference value)
pair<iterator, bool> insert(rvalue_reference value)
iterator insert(iterator position, const_reference value)
iterator insert(iterator position, rvalue_reference value)
```
**Description**  
Inserts a value into the set.

```cpp
template <typename TIterator>
void insert(TIterator first, TIterator last)
```
**Description**  
Inserts values in to the set.  
If the set is full then emits an `etl::flat_set_full`. If asserts or exceptions are not enabled then undefined behaviour occurs.  
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)

---

```cpp
pair<iterator, bool> emplace((const_reference value))
pair<iterator, bool> emplace(const key_type& key, const mapped_type& value)
```
**Description**  
Inserts key/value pairs into the set by constructing directly into storage.  
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)

---

**C++03**  
The emplace functions differ from that of `std::set` due to C++03 not supporting 'perfect forwarding'.

```cpp
template <typename T1>
pair<iterator, bool> emplace(const key_type& key, const T1& value1)
```
**Description**  

Emplaces a value constructed from `key` and 1 argument into the set.

---

```cpp
template <typename T1, typename T2>
pair<iterator, bool> emplace(const key_type& key, const T1& value1, const T2& value2)
```
**Description**  
Emplaces a value constructed from `key` and 2 arguments into the set.

---

```cpp
template <typename T1, typename T2, typename T3>
pair<iterator, bool> emplace(const key_type& key, const T1& value1, const T2& value2, const T3& value3)
```
**Description**  
Emplaces a value constructed from `key` and 3 arguments into the set.

---

```cpp
template <typename T1, typename T2, typename T3, typename T4>
pair<iterator, bool> emplace(const key_type& key, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```
**Description**  
Emplaces a value constructed from `key` and 4 arguments into the set.

---

**C++11**  

```cpp
template <typename ... Args>
pair<iterator, bool> emplace(const key_type& key, Args&& ... args)
```
**Description**  
Emplaces a value constructed from the `key` and arguments into the set.

---

```cpp
size_t erase(key_value_parameter_t key)
void erase(iterator i_element)
void erase(iterator first, iterator last)
```
**Description**  
Erase elements from the set.

---

```cpp
iterator erase(const_iterator i_element)
iterator erase(const_iteratorfirst, const_iteratorlast)
```
**Description**  
Erase elements from the set.
From: `20.20.0`

---

```cpp
template <typename K>
size_t erase(K&& key)
```
**Description**  
Erases values in the set.  
Returns an iterator to the next element in the set.  
Iterator parameters are not checked for validity.  
From: `20.21.0`

---

```cpp
void clear();
```
**Description**  
Clears the set to a size of zero.

## Search

```cpp
iterator find(key_value_parameter_t key)
const_iterator find(key_value_parameter_t key) const
```
**Description**  
Searches for an element with the key `key`.  
Returns an iterator to the element, or `end()` if not found.

---

```cpp
iterator lower_bound(key_value_parameter_t key)
const_iterator lower_bound(key_value_parameter_t key) const
```
**Description**  
Searches for an element with the key not less than `key`.  
Returns an iterator to the element, or `end()` if not found.

---

```cpp
iterator upper_bound(key_value_parameter_t key)
const_iterator upper_bound(key_value_parameter_t key) const
```
**Description**  
Searches for an element with the key greater than `key`.  
Returns an iterator to the element, or `end()` if not found.

---

```cpp
pair<iterator, iterator> equal_range(key_value_parameter_t key)
pair<const_iterator, const_iterator> equal_range(key_value_parameter_t key) const
```
**Description**  
Returns the range of elements with a key equal to `key`.
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)

---

**For comparators that define is_transparent.**  

```cpp
template <typename K>
iterator find(const K& key)
```
**Description**  
Searches for an element with the key `key`.  
Returns an iterator to the element, or `end()` if not found.
From: `20.21.0`  
Since: C++11

---

```cpp
template <typename K>
const_iterator find(const K& key) const
```
**Description**  
Searches for an element with the key `key`.  
Returns an iterator to the element, or `end()` if not found.
From: `20.21.0`  
Since: C++11

---

```cpp
template <typename K>
iterator lower_bound(const K& key)
```
**Description**  
Searches for an element with the key not less than `key`.  
Returns an iterator to the element, or `end()` if not found.  
From: `20.21.0`  
Since: C++11

---

```cpp
template <typename K>
const_iterator lower_bound(const K& key) const
```
**Description**  
Searches for an element with the key not less than `key`.  
Returns an iterator to the element, or `end()` if not found.  
From: `20.21.0`  
Since: C++11

---

```cpp
template <typename K>
iterator upper_bound(const K& key)
```
**Description**  
Searches for an element with the key greater than `key`.  
Returns an iterator to the element, or `end()` if not found.
From: `20.21.0`  
Since: C++11

---

```cpp
template <typename K>
const_iterator upper_bound(const K& key) const
```
**Description**  
Searches for an element with the key greater than `key`.  
Returns an iterator to the element, or `end()` if not found.
From: `20.21.0`  
Since: C++11

---

```cpp
template <typename K>
pair<iterator, iterator> equal_range(const K& key)
```
**Description**  
Returns the range of elements with a key equal to `key`.
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)  
From: `20.21.0`  
Since: C++11

---

```cpp
template <typename K>
pair<const_iterator, const_iterator> equal_range(const K& key) const
```
**Description**  
Returns the range of elements with a key equal to `key`.
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)  
From: `20.21.0`  
Since: C++11

---

```cpp
bool contains(key_value_parameter_t  key) const
```
Check if the container contains the key.  
From: `20.21.0`


---

```cpp
template <typename K>
bool contains(const K& k) const
```
Check if the container contains the key.  
For comparators that define `is_transparent`.
From: `20.21.0`  
Since: C++11  

## Non-member functions

**Lexicographically comparisons**  

```cpp
operator ==
```
`true` if the contents of the maps are equal, otherwise `false`.

---

```cpp
operator !=  
```
`true` if the contents of the maps are not equal, otherwise `false`.

## Technical stuff

Flat maps are usually implemented internally as a sorted vector of key/value pairs. Whilst this makes searching fast, it can have a detrimental effect when items are inserted into a container that stores complex, non-trivial keys or values.  
As inserting requires that all of the items to the right of the insert position must be shifted this can become an expensive operation for larger containers.  

To improve insertion performance ETL flat maps are implemented as vectors of pointers to key/value pairs, sorted by key value. An insertion will involve a copy of a range of pointers; an operation that can be  very fast.  

The downside is that access to an item via an iterator will involve one indirection and the overhead of the container will be one pointer per item. A normal flat set implementation does not have this overhead.
