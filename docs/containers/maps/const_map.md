---
title: "const_map / const_map_ext"
---

{{< callout >}}
  Header: `const_map.h`  
  Supported: TBC  
  Similar to: [std::map](https://en.cppreference.com/w/cpp/container/map.html)
{{< /callout >}}

A fixed capacity read-only map based on a sorted array.
The containers are designed to be able to be constexpr for C++14 and up.

The interface is most similar to `std::map`.
Uses etl::less as the default key comparison method.
```cpp
etl::const_map<typename TKey, typename TMapped, size_t Size, TKeyCompare = etl::less>
etl::const_map_ext<typename TKey, typename TMapped, TKeyCompare = etl::less>
```

Inherits from `etl::iconst_map<TKey, TMapped, TKeyCompare>`  
`etl::iconst_map` may be used as a Size independent pointer or reference type for any `etl::const_map` instance.  
Both the key type and mapped type must be default constructible.

## Template deduction guides
**C++17 and above**

```cpp
template <typename... TPairs>
etl::const_map(TPairs...)
```

**Example**
```cpp
constexpr etl::const_map data{ etl::pair{0, 1}, etl::pair{2, 3},
                               etl::pair{4, 5}, etl::pair{6, 7} };
```
Defines data as an `const_map` of `int`/`int` pairs, of length 4, containing the supplied data.

---

```cpp
template <typename... TPairs>
etl::const_map_ext(TPairs...)
```

**Example**
```cpp
constexpr etl::pair<int, int> values[]{ etl::pair{0, 1}, etl::pair{2, 3}, 
                                        etl::pair{4, 5}, etl::pair{6, 7} };

constexpr etl::const_map_ext data{ values };
```
Defines data as an `const_map` of `int`/`int` pairs, of length 4, containing the supplied data.

```cpp
constexpr values[]{ etl::pair{0, 1}, etl::pair{2, 3}, 
                    etl::pair{4, 5}, etl::pair{6, 7} };
constexpr etl::span<const etl::pair<int, int>, 4> span{ values };
constexpr etl::const_map_ext data{ span };
```
Defines data as an `const_map` of `int`/`int` pairs, of length 4, containing the supplied data.

## Member types

|  Type             | Maps to                           | Description                                                      |
| ----------------- | --------------------------------- | ---------------------------------------------------------------- | 
| `key_type`        | TKey                              | Must be default constructible.                                   |
| `mapped_type`     | TMapped                           | Must be default constructible.                                   |
| `value_type`      | pair<const key_type, mapped_type> | The type is either std::pair (default) or etl::pair (ETL_NO_STL) |
| `size_type`       | std::size_t                       |                                                                  |
| `difference_type` | std::ptrdiff_t                    |                                                                  |
| `const_reference` | const T&                          |                                                                  |
| `const_pointer`   | const T*                          |                                                                  |
|` const_iterator`  |                                   | Constant random access iterator                                  |

## Constructor
```cpp
template <typename... TElements>
ETL_CONSTEXPR14 explicit const_map(TElements&&... elements) ETL_NOEXCEPT
```
Construct a const_map from a variadic list of elements.  
Static asserts if the elements are not of type `value_type`.  
Static asserts if the number of elements is greater than the capacity of the `const_map`.  

## Element access
```cpp
ETL_CONSTEXPR14 const TMapped& at(key_parameter_t key) const ETL_NOEXCEPT
```
**Description**  
Returns a const reference to the indexed element. 

```cpp
ETL_CONSTEXPR14 const TMapped& operator[](key_parameter_t key) const ETL_NOEXCEPT
```
**Description**  
Returns a const reference to the indexed element.

## Iterators
```cpp
const_iterator begin() const ETL_NOEXCEPT
const_iterator cbegin() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the beginning of the map.

```cpp
const_iterator end() const ETL_NOEXCEPT
const_iterator cend() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the end of the map.

## Capacity
```cpp
ETL_CONSTEXPR14 bool empty() const ETL_NOEXCEPT
```
**Description**  
Returns `true` if the size of the map is zero, otherwise `false`.

```cpp
ETL_CONSTEXPR14 bool full() const ETL_NOEXCEPT
```
**Description**  
Returns `true` if the size of the lookup is size, otherwise `false`.

```cpp
ETL_CONSTEXPR14 size_t size() const ETL_NOEXCEPT
```
**Description**  
Returns the size of the lookup.

```cpp
ETL_CONSTEXPR14 size_t max_size() const ETL_NOEXCEPT
```
**Description**  
Returns the maximum possible Size of the map.

## Status
```cpp
ETL_CONSTEXPR14 bool is_valid() const ETL_NOEXCEPT
```
**Description**  
Check that the elements are valid for a map.  
The elements must be sorted and contain no duplicates.

## Search
```cpp
ETL_CONSTEXPR14 const_iterator find(const key_type& key) const ETL_NOEXCEPT
```
```cpp
ETL_CONSTEXPR14 const_iterator lower_bound(const key_type& key) const ETL_NOEXCEPT
```
```cpp
ETL_CONSTEXPR14 const_iterator upper_bound(const key_type& key) const ETL_NOEXCEPT
```
```cpp
ETL_CONSTEXPR14 pair<const_iterator, const_iterator> equal_range(const key_type& key) const ETL_NOEXCEPT
```
**Description**  
The return type is either std::pair (default) or etl::pair (ETL_NO_STL)

```cpp
ETL_CONSTEXPR14 bool contains(const key_type& k) const ETL_NOEXCEPT
```
**Description**  
Check if the container contains the key.

---

**For comparators that define is_transparent.**

```cpp
template <typename K>
ETL_CONSTEXPR14 const_iterator find(const K& key) const ETL_NOEXCEPT
```
```cpp
template <typename K>
ETL_CONSTEXPR14 const_iterator lower_bound(const K& key) const ETL_NOEXCEPT
```
```cpp
template <typename K>
ETL_CONSTEXPR14 const_iterator upper_bound(const K& key) const ETL_NOEXCEPT
```
```cpp
template <typename K>
ETL_CONSTEXPR14 pair<const_iterator, const_iterator> equal_range(const K& key) const ETL_NOEXCEPT
```
```cpp
template <typename K>
ETL_CONSTEXPR14 bool contains(const K& k) const ETL_NOEXCEPT
```
**Description**  
Check if the container contains the key.

## Non-member functions

**Lexicographically comparisons**
| Operator | Description                                                                         |
| -------- | ----------------------------------------------------------------------------------- |
| `==`     | `true` if the contents of the maps are equal, otherwise `false`.                    |
| `!=`     | `true` if the contents of the maps are not equal, otherwise `false`.                |
| `<`      | `true` if the contents of the lhs is less-than the rhs, otherwise `false`.          |
| `<=`     | `true` if the contents of the lhs is less-than-equal the rhs, otherwise `false`.    |
| `>`      | `true` if the contents of the lhs is greater-than the rhs, otherwise `false`.       |
| `>=`     | `true` if the contents of the lhs is greater-than-equal the rhs, otherwise `false`. |

## Technical stuff

The const map is implemented as a sorted array key/value pairs.

