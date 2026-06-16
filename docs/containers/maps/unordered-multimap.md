---
title: "unordered_multimap"
---

{{< callout type="info">}}
  Header: `unordered_multimap.h`  
  Similar to: `std::unordered_multimap`
{{< /callout >}}

A fixed capacity unordered map.
Uses `std::less` as the default key comparison method.

```cpp
template <typename TKey, 
          typename TValue, 
          size_t   MAX_SIZE, 
          size_t   MAX_BUCKETS = MAX_SIZE, 
          typename THash = etl::hash<TKey>, 
          typename TKeyEqual = etl::equal_to<TKey>>
class unordered_multimap
```

Inherits from  `iunordered_map<TKey, TValue, THash, TKeyEqual>`.  
`etl::iunordered_map` may be used as a size independent pointer or reference type for any `etl::unordered_multimap` instance.

## Template deduction guides
C++17 and above

```cpp
template <typename... TPairs>
etl::unordered_multimap(TPairs...)
```

### Example
```cpp
etl::unordered_multimap data{ etl::pair{0, 1}, etl::pair{2, 3}, etl::pair{4, 5}, etl::pair{6, 7} };
```
Defines data as an unordered map of `int`/`int` pairs, of length 4, containing the supplied data.

## Make template
C++11 and above

```cpp
template <typename TKey, 
          typename TValue, 
          typename THash       = etl::hash<TKey>, 
          typename TKeyEqual   = etl::equal_to<TKey>
          typename... TPairs>
constexpr auto make_unordered_map(TValues&&... values)
```

### Example
```cpp
auto data = etl::make_unordered_map<int, int>(etl::pair{0, 1}, etl::pair{2, 3}, 
                                              etl::pair{4, 5}, etl::pair{6, 7});
```

## Member types

```cpp
value_type              pair<const TKey, T>
key_type                The type of the key value
mapped_type             The type of the mapped value
hasher                  The type used to hash the key
key_equal               The functor used to compare keys
reference               A reference to a value_type
const_reference         A reference to a const value_type
pointer                 A pointer to a value_type
const_pointer           A pointer to a const value_type
size_type               The type used for size information
iterator                Random access iterator
const_iterator          Constant random access iterator
local_iterator          Iterator to a bucket
local_const_iterator    A const iterator to a bucket
```

## Constructor

```cpp
unordered_multimap()
```
**Description**  
Default constructor

---

```cpp
unordered_multimap(const unordered_multimap& other)
```
**Description**  
Copy constructor

---

```cpp
template <typename TIterator>
unordered_multimap(TIterator begin, TIterator end)
```
**Description**  
Construct from a range

## Element access

```cpp
mapped_type& at(key_parameter_t key)
const mapped_type& at(key_parameter_t key) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Raises an `etl::unordered_map_out_of_range` if the key is not in the table.  
If assert or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
mapped_type& operator[](key_parameter_t key)
```
**Description**  
Returns a reference to the indexed element.  
If the key does not exist then one is created using the default constructor.  
If the map is full then emits an `etl::unordered_map_full`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
iterator find(key_parameter_t key);
const_iterator find (key_parameter_t key) const
```
**Description**  
Searches the container for an element with a key equivalent to key and returns an iterator to it if found, otherwise it returns an iterator to `etl::unordered_multimap::end()`.

---

```cpp
bool contains(const_key_reference key) const
```
**Description**  
Check if the map contains the key.

---

```cpp
template <typename K, typename KE = TKeyEqual>
bool contains(const K& key) const
```
**Description**  
Check if the map contains the key.  
Enabled if the comparator is transparent.

---

```cpp
size_type count(key_parameter_t key) const
```
**Description**  
Count elements with a specific key.  
Searches the container for elements with a key equivalent to key and returns the number of matches

---

```cpp
iterator lower_bound(key_parameter_t key)
const_iterator lower_bound(key_parameter_t key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is not considered to go before key (i.e., either it is equivalent or goes after).

---

```cpp
iterator upper_bound(key_parameter_t key)
const_iterator upper_bound(key_parameter_t key) const
```
**Description**  
Returns an iterator pointing to the first element in the container whose key is considered to go after `key`.

---

```cpp
pair<iterator, iterator> equal_range(key_parameter_t key)
pair<const_iterator, const_iterator> equal_range(key_parameter_t key) const
```
**Description**  
Returns the bounds of a range that includes all the elements in the container which have a key equivalent to `key`.

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
Returns `true` if the size of the map is `SIZE`, otherwise `false`.

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

---

```cpp
size_type get_bucket_index(key_parameter_t key) const
```
**Description**  
Returns the bucket index for the key.

---

```cpp
size_type bucket_size(key_parameter_t key) const
```
**Description**  
Returns the size of the bucket key.

---

```cpp
size_type max_bucket_count() const
```
**Description**  
Returns the maximum number of the buckets the container can hold.

---

```cpp
float load_factor() const
```
**Description**  
Returns the load factor = size / bucket_count

---

```cpp
hasher hash_function() const
```
**Description**  
Returns the function that hashes the keys.

---

```cpp
key_equal key_eq() const
```
**Description**  
Returns the function that compares the keys.

## Modifiers

```cpp
template <typename TIterator>
void assign(TIterator first, TIterator last)
```
**Description**  
Assigns elements from the range [`first`, `last`)].  
Clears the container before assigning.

---

```cpp
pair<iterator, bool> insert(const value_type& key_value_pair)
iterator insert(const_iterator, const value_type& key_value_pair)
```
**Description**  
Assigns elements from the range [`first`, `last`)].  
Clears the container before assigning.

---

```cpp
template <template TIterator>
void insert(TIterator first, TIterator last)
```
**Description**  
Inserts the range [`first`, `last`)] into the map.  
If the map is full then emits an `etl::unordered_map_full error`.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
size_t erase(key_parameter_t key)
```
**Description**  
Removes the element with the key `key`.

---

```cpp
iterator erase(const_iterator ielement)
```
**Description**  
Removes the element with the key `key`.

---

```cpp
iterator erase(const_iterator first, const_iterator last)
```
**Description**  
Erases values in the map.  
Iterator are not checked for validity.

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
`true` if the contents of the maps are equal, otherwise `false`.

---

```cpp
operator !=
```
**Description**  
`true` if the contents of the maps are not equal, otherwise `false`.
