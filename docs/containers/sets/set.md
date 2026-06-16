---
title: "set"
---

{{< callout type="info">}}
  Header: `set.h`  
  Similar to: `std::set`
{{< /callout >}}

A fixed capacity set.  
Uses etl or `std::less` as the default key comparison method.

```cpp
etl::set<typename T, const size_t SIZE, TKeyCompare = etl::less>
```

Inherits from `etl::iset<T, TKeyCompare>`.  
`etl::iset` may be used as a size independent pointer or reference type for any `etl::set` instance.

## Template deduction guides
C++17 and above

```cpp
template <typename... TPairs>
etl::set(TPairs...)
```

### Example
```cpp
etl::set data{ 0, 1, 2, 3, 4, 5, 6, 7 };
```
Defines data as an set of `int`, of length 8, containing the supplied data.

## Make template
**C++11 and above**  

```cpp
template <typename TKey, 
          typename TKeyCompare = etl::less<TKey>, 
          typename... TPairs>
constexpr auto make_set(TValues&&... values)
```

### Example
```cpp
auto data = etl::make_set<int>(0, 1, 2, 3, 4, 5, 6, 7);
```

## Member types

```cpp
value_type              T
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

## Constructors

```cpp
etl::set<T, SIZE, TKeyCompare>()
```
**Description**  

---

```cpp
template <typename TIterator>
etl::set<T, SIZE, TKeyCompare>(TIterator begin, TIterator end)
```
**Description**  
If the map is full then emits an `etl::reference_flat_set_full`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

## Element access

```cpp
iterator find(const value_type& v)
const_iterator find(const value_type& v) const
```
**Description**  
Searches the container for an element with a value of `v` and returns an iterator to it if found, otherwise it returns an iterator to `etl::set::end()`.

---

```cpp
size_type count(const value_type& v) const
```
**Description**  
Count elements with a specific key.
Searches the container for elements with a value of `v` and returns the number of matches

---

```cpp
iterator lower_bound(const value_type& v)
const_iterator lower_bound(const value_type& v) const
```
**Description**  
Searches for an element with the key not less than `v`.  
Returns an iterator to the element, or `end()` if not found.

---

```cpp
iterator upper_bound(const value_type& v)
const_iterator upper_bound (const value_type& v) const
```
**Description**  
Searches for an element with the key greater than `v`.  
Returns an iterator to the element, or `end()` if not found.

---

```cpp
pair<const_iterator, const_iterator> equal_range(const value_type& v) const
pair<iterator, iterator> equal_range(const value_type& v)
```
**Description**  
Returns the range of elements with a key equal to `v`.
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)

---

**For comparators that define `is_transparent`.**

```cpp
template <typename K>
iterator find(const K& key)
```
**Description**  
Since: `20.21.0`  
C++11 or above.


---

```cpp
template <typename K>
const_iterator find(const K& key) const
```
**Description**  
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
iterator lower_bound(const K& key)
```
**Description**  
Searches for an element with the key not less than `key`.  
Returns an iterator to the element, or `end()` if not found.
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
const_iterator lower_bound(const K& key) const
```
**Description**  
Searches for an element with the key not less than `key`.  
Returns a const iterator to the element, or `end()` if not found.
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
iterator upper_bound(const K& key)
```
**Description**  
Searches for an element with the key greater than `key`.  
Returns an iterator to the element, or `end()` if not found.
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
const_iterator upper_bound(const K& key) const
```
**Description**  
Searches for an element with the key greater than `key`.  
Returns a const iterator to the element, or `end()` if not found.
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
pair<iterator, iterator> equal_range(const K& key)
```
**Description**  
Returns the range of elements with a key equal to `key`.
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
pair<const_iterator, const_iterator> equal_range(const K& key) const
```
**Description**  
Returns the range of elements with a key equal to `key`.
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)
Since: `20.21.0`  
C++11 or above.

---

```cpp
bool contains(key_value_parameter_t key) const
```
**Description**  
Check if the container contains the key.
Since: `20.21.0`  

---

For comparators that define is_transparent.
```cpp
template <typename K>
```
**Description**  
Since: `20.21.0`  
C++11 or above.

---

```cpp
bool contains(const K& k) const
```
Check if the container contains the key.
Since: `20.21.0`  
C++11 or above.

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
iterator rbegin()
const_iterator rbegin() const
const_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the map.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the map is zero, otherwise false.

---

```cpp
bool full() const
```
**Description**  
Returns true if the size of the map is SIZE, otherwise false.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the map.

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

<=20.19.0
```cpp
template <typename TIterator>
void insert(TIterator begin, TIterator end)
```
**Description**  

---

>=20.20.0
```cpp
template <typename TIterator>
iterator insert(TIterator begin, TIterator end)
```
**Description**  

---

```cpp
iterator insert(parameter_t value)
```
**Description**  
If the set is full then emits an `etl::set_full`. 
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
template <typename TIterator>
iterator erase(TIterator begin, TIterator end)
```
**Description**  

---

```cpp
iterator erase(TIterator element)
```
**Description**  
Erases values in the set.
Iterators are not checked for validity.

---

```cpp
size_t erase(const key_type& key)
```
**Description**  

----

20.21.0
```cpp
template <typename K>
size_t erase(K&& key)
```
**Description**  

---

```cpp
void clear()
```
**Description**  
Clears the map to a size of zero.

## Non-member functions

```cpp
operator ==
```
**Description**  
`true` if the contents of the sets are equal, otherwise `false`.

---

```cpp
operator !=
```
**Description**  
`true` if the contents of the sets are not equal, otherwise `false`.
