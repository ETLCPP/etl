---
title: "const_multimap / const_multimap_ext"
---

{{< callout >}}
  Header: `const_multimap.h`  
  Supported: TBC  
  Similar to: [std::multimap](https://en.cppreference.com/w/cpp/container/multimap.html)
{{< /callout >}}

A fixed capacity read-only multimap based on a sorted array.  
The containers are designed to be able to be constexpr for C++14 and up.

The interface is most similar to `std::multimap`.  
Uses `etl::less` as the default key comparison method.

```cpp
etl::const_multimap<typename TKey, typename TMapped, size_t Size, TKeyCompare = etl::less>
etl::const_multimap_ext<typename TKey, typename TMapped, TKeyCompare = etl::less>
```

Inherits from `etl::iconst_multimap<TKey, TMapped, TKeyCompare>`.  
`etl::iconst_multimap` may be used as a size independent pointer or reference type for any `etl::const_map` instance.
Both the key type and mapped type must be default constructible.

## Template deduction guides
**C++17 and above**
```cpp
template <typename... TPairs>
etl::const_multimap(TPairs...)
```

**Example**
```cpp
constexpr etl::const_multimap data{ etl::pair{0, 1}, etl::pair{2, 3},
                                    etl::pair{4, 5}, etl::pair{6, 7} };
```
Defines data as an const_multimap of `int`/`int` pairs, of length 4, containing the supplied data.

```cpp
template <typename... TPairs>
etl::const_multimap_ext(TPairs...)
```

**Example**
```cpp
constexpr etl::pair<int, int> values[]{ etl::pair{0, 1}, etl::pair{2, 3}, 
                                        etl::pair{4, 5}, etl::pair{6, 7} };
constexpr etl::const_multimap_ext data{ values };
```
Defines data as an const_multimap of `int`/`int` pairs, of length 4, containing the supplied data.
```cpp
constexpr values[]{ etl::pair{0, 1}, etl::pair{2, 3}, 
                    etl::pair{4, 5}, etl::pair{6, 7} };
constexpr etl::span<const etl::pair<`int`, `int`>, 4> span{ values };
constexpr etl::const_multimap_ext data{ span };
```
Defines data as an const_multimap of `int`/`int` pairs, of length 4, containing the supplied data.

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
ETL_CONSTEXPR14 explicit const_multimap(TElements&&... elements) ETL_NOEXCEPT
```
**Description**  
Constructs a const_multimap from a variadic list of elements.  
Static asserts if the elements are not of type `value_type`.  
Static asserts if the number of elements is greater than the capacity of the `const_multimap`.

## Iterators

```cpp
const_iterator begin() const ETL_NOEXCEPT
const_iterator cbegin() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the beginning of the multimap.

```cpp
const_iterator end() const ETL_NOEXCEPT
const_iterator cend() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the end of the multimap.

## Capacity

```cpp
ETL_CONSTEXPR14 bool empty() const ETL_NOEXCEPT
```
**Description**  
Returns true if the size of the multimap is zero, otherwise false.

```cpp
ETL_CONSTEXPR14 bool full() const ETL_NOEXCEPT
```
**Description**  
Returns true if the size of the lookup is size, otherwise false.

```cpp
ETL_CONSTEXPR14 size_t size() const ETL_NOEXCEPT
```
**Description**  
Returns the size of the lookup.

```cpp
ETL_CONSTEXPR14 size_t max_size() const ETL_NOEXCEPT
```
**Description**  
Returns the maximum possible Size of the multimap.

## Status
```cpp
ETL_CONSTEXPR14 bool is_valid() const ETL_NOEXCEPT
```
**Description**  
Check that the elements are valid for a multimap.  
The elements must be sorted.

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
The return type is either `std::pair` (default) or `etl::pair` (ETL_NO_STL)

```cpp
ETL_CONSTEXPR14 bool contains(const key_type& k) const ETL_NOEXCEPT
```
**Description**  
Checks if the container contains the key.

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
Checks if the container contains the key.

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

The const multimap is implemented as a sorted array key/value pairs.
