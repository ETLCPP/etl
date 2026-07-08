---
title: "multimap"
---

{{< callout type="info">}}
  Header: `multimap.h`  
  Similar to: `std::multimap`
{{< /callout >}}

A fixed capacity multimap.  
Uses `std::less` as the default key comparison method.  

```cpp
etl::multimap<typename TKey, typename TMapped, const size_t SIZE, TKeyCompare = etl::less>
```

Inherits from `etl::imap<TKey, TMapped, TKeyCompare>`.
`etl::imap` may be used as a size independent pointer or reference type for any etl::multimap instance.

## Template deduction guides
C++17 and above

```cpp
template <typename... TPairs>
etl::multimap(TPairs...)
```

### Example
```cpp
etl::multimap data{ etl::pair{0, 1}, etl::pair{2, 3}, etl::pair{4, 5}, etl::pair{6, 7} }
```
Defines data as an multimap of `int`/`int` pairs, of length 4, containing the supplied data.

## Make template
**C++11 and above**  
```cpp
template <typename TKey, 
          typename TMapped, 
          typename TKeyCompare = etl::less<TKey>, 
          typename... TPairs>
constexpr auto make_flat_map(TValues&&... values)
```

### Example
```cpp
auto data = etl::make_multimap<int, int>(etl::pair{0, 1}, etl::pair{2, 3},
                                    etl::pair{4, 5}, etl::pair{6, 7});
```

## Member types

```cpp
key_type                TKey
mapped_type             TMapped
value_type              etl or std::pair<key_type, mapped_type>
size_type               size_t
difference_type         ptrdiff_t
reference               value_type&
const_reference         const value_type&
pointer                 value_type*
const_pointer           const value_type*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        etl or std::reverse_iterator<iterator>
const_reverse_iterator  etl or std::reverse_iterator<const_iterator>
```

## Constructor

```cpp
etl::multimap<Tkey, TMapped, SIZE, TKeyCompare>();
```
**Description**  
Default constructor.


```cpp
template <typename TIterator>
etl::multimap<Tkey, TMapped, SIZE, TKeyCompare>(TIterator begin, TIterator end);
```
**Description**  
Construct from the range [`begin`, `end`).

## Element access

```cpp
TMapped& at(key_parameter_t key)
const TMapped& at(key_parameter_t key) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Asserts an `etl::map_out_of_range` if the key is not in the table.  
If assert or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
TMapped& operator[](key_parameter_t key)
const TMapped& operator[](key_parameter_t key) const
```
**Description**  
Returns a reference or const reference to the indexed element.
If the key does not exist then one is created using the default constructor.  
If the multimap is full then asserts an `etl::map_full`. If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
iterator find(key_parameter_t key);
const_iterator find(key_parameter_t key) const
```
**Description**  
Searches the container for an element with a key equivalent to key and returns an iterator to it if found, otherwise it returns an iterator to `etl::multimap::end()`.

---

```cpp
size_type count(key_parameter_t key) const
```
**Description**  
Count elements with a specific key.  
Searches the container for elements with a key equivalent to key and returns the number of matches

---

```cpp
iterator lower_bound(key_parameter_t key);
const_iterator lower_bound(key_parameter_t key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose `key` is not considered to go before key (i.e., either it is equivalent or goes after).

---

```cpp
iterator upper_bound(key_parameter_t key);
const_iterator upper_bound(key_parameter_t key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is considered to go after `key`.

---

```cpp
pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
pair<iterator, iterator> equal_range(key_parameter_t key)
```
**Description**  
Returns the bounds of a range that includes all the elements in the container which have a key equivalent to `key`.  
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)

---

**For comparators that define `is_transparent`.**  

```cpp
template <typename K>
iterator find(const K& key)
```
**Description**  
Searches the container for an element with a key equivalent to key and returns an iterator to it if found, otherwise it returns an iterator to `etl::multimap::end()`.
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
const_iterator find(const K& key) const
```
**Description**  
Searches the container for an element with a key equivalent to `key` and returns a const iterator to it if found, otherwise it returns a const iterator to `etl::multimap::end()`.

---

```cpp
template <typename K>
iterator lower_bound(const K& key)
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is not considered to go before `key` (i.e., either it is equivalent or goes after).
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
const_iterator lower_bound(const K& key) const
```
**Description**  
Returns a const iterator pointing to the first element in the container whose key is not considered to go before `key` (i.e., either it is equivalent or goes after).
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
iterator upper_bound(const K& key)
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is considered to go after `key`.
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
const_iterator upper_bound(const K& key) const
```
**Description**  
Returns a const iterator pointing to the first element in the container whose key is considered to go after `key`.  
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
pair<iterator, iterator> equal_range(const K& key)
```
**Description**  
Returns the bounds of a range that includes all the elements in the container which have a key equivalent to `key`.  
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)
Since: `20.21.0`  
C++11 or above.

---

```cpp
template <typename K>
pair<const_iterator, const_iterator> equal_range(const K& key) const
```
**Description**  
Returns the bounds of a range that includes all the elements in the container which have a key equivalent to `key`.  
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)

---

```cpp
bool contains(key_value_parameter_t  key) const
```
**Description**  
Check if the container contains the key.
Since: `20.21.0`

---

```cpp
template <typename K>
bool contains(const K& k) const
```
**Description**  
Check if the container contains the key.  
Since: `20.21.0`  
C++11 or above.  
For comparators that define is_transparent.

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the multimap.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the multimap.

---

```cpp
iterator rbegin()
const_iterator rbegin() const
const_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the multimap.

---

```cpp
iterator rend()
const_iterator rend() const
const_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the multimap.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the multimap is zero, otherwise false.

---

```cpp
bool full() const
```
**Description**  
Returns true if the size of the multimap is SIZE, otherwise false.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the multimap.

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the multimap.

---

```cpp
size_t available() const
```
**Description**  
Returns the remaining available capacity in the multimap.

---

## Modifiers

```cpp
template <typename TIterator>
void insert(TIterator begin, TIterator end)
```
**Description**  
Before: `20.20.0`
---


```cpp
template <typename TIterator>
iterator insert(TIterator begin, TIterator end)
```
**Description**  
Since: `20.20.0`

---

```cpp
iterator insert(mapped_parameter_t value)
```
**Description**  
Inserts values in to the multimap. 
If the multimap is full then asserts an etl::map_full error. If asserts or exceptions are not enabled then undefined behaviour occurs.

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
Erases values in the multimap.
Iterator are not checked for validity.

---

```cpp
size_t erase(const key_type& key)
```
**Description**  

---

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
Clears the multimap to a size of zero.

## Non-member functions

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
