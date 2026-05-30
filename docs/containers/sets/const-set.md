---
title: "const_set / const_set_ext"
---

{{< callout >}}
  Header: `const_set.h`  
  Similar to: `std::set`
{{< /callout >}}

A fixed capacity read-only set based on a sorted array.  
The containers are designed to be able to be `constexpr` for C++14 and up.  

Uses `etl::less` as the default key comparison method.  

**Internal storage**
```cpp
etl::const_set<typename TKey, size_t Size, TKeyCompare = etl::less>
```

**Enternal storage**
```cpp
etl::const_set_ext<typename TKey, TKeyCompare = etl::less>
```

Inherits from `etl::iconst_set<TKey, TMapped, TKeyCompare>`.
`etl::iconst_set` may be used as a Size independent pointer or reference type for any `etl::const_set` or `etl::const_set_ext` instance.
Both the key type must be default constructible.

## Template deduction guides
**C++17 and above.**  

### const_set
```cpp
template <typename... TElements>
etl::const_set(TElements...)
```

**Example**
```cpp
constexpr etl::const_set data{ 0, 1, 2, 3 };
```
Defines data as an `const_set` of `int`, of length 4, containing the supplied data.

### const_set_ext
```cpp
template <typename... TElements>
etl::const_set_ext(TElements...)
```

**Example**
```cpp
constexpr etl::pair<int, int> values{ 0, 1, 2, 3 };
constexpr etl::const_set_ext data{ values };
```
Defines data as an `const_set` of int, of length 4, containing the supplied data.

---

```cpp
constexpr values[]{ 0, 1, 2, 3 };
constexpr etl::span<int, 4> span{ values };
constexpr etl::const_set_ext data{ span };
```
Defines data as an `const_set` of `int` of length 4, containing the supplied data.

---

## Member types
```cpp
key_type                TKey    Must be default constructible.
value_type              key_type
size_type               std::size_t
difference_type         std::ptrdiff_t
const_reference         const T&
const_pointer           const T*
const_iterator          Constant random access iterator
```

## Constructors

### const_set
```cpp
template <typename... TElements>
ETL_CONSTEXPR14 explicit const_set(TElements&&... elements) ETL_NOEXCEPT
```
**Description**  
Construct a `const_set` from a variadic list of elements.  
Static asserts if the elements are not of type `value_type`.  
Static asserts if the number of elements is greater than the capacity of the `const_set`.

### const_set_ext

```cpp
ETL_CONSTEXPR14 const_set_ext() ETL_NOEXCEPT
```
**Description**  
Default construct a `const_set`.

---

```cpp
template <size_type Size>
ETL_CONSTEXPR14 explicit const_set_ext(const etl::span<const value_type, Size>& sp) ETL_NOEXCEPT
```
**Description**  
Construct a `const_set` from a variadic list of elements.

---

```cpp
template <size_type Size>
ETL_CONSTEXPR14 explicit const_set_ext(const value_type (&begin_)[Size]) ETL_NOEXCEPT
```
**Description**  
Construct a `const_set` from an array.

## Iterators

```cpp
const_iterator begin() const ETL_NOEXCEPT
const_iterator cbegin() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the beginning of the set.

---

```cpp
const_iterator end() const ETL_NOEXCEPT
const_iterator cend() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the end of the set.

## Capacity

```cpp
ETL_CONSTEXPR14 bool empty() const ETL_NOEXCEPT
```
**Description**  
Returns `true` if the size of the set is zero, otherwise `false`.

---

```cpp
ETL_CONSTEXPR14 bool full() const ETL_NOEXCEPT
```
**Description**  
Returns `true` if the size of the lookup is size, otherwise `false`.

---

```cpp
ETL_CONSTEXPR14 size_t size() const ETL_NOEXCEPT
```
**Description**  
Returns the size of the lookup.

---

```cpp
ETL_CONSTEXPR14 size_t max_size() const ETL_NOEXCEPT
```
**Description**  
Returns the maximum possible size of the set.

## Status

```cpp
ETL_CONSTEXPR14 bool is_valid() const ETL_NOEXCEPT
```
Check that the elements are valid for a set.  
The elements must be sorted and contain no duplicates.

## Search

```cpp
ETL_CONSTEXPR14 const_iterator find(const key_type& key) const ETL_NOEXCEPT
```
**Description**  
Find the `key` in the container.  

**Return**  
A `const_iterator` to the element, or `end()` if it could not be found.  

---

```cpp
ETL_CONSTEXPR14 const_iterator lower_bound(const key_type& key) const ETL_NOEXCEPT
```
**Description**   
Searches for the first element after the `key`.  

**Return**  
A `const_iterator` to the element, or `end()` if it could not be found.  

---

```cpp
ETL_CONSTEXPR14 const_iterator upper_bound(const key_type& key) const ETL_NOEXCEPT
```
**Description**  
Searches for the first element with the `key`.  

**Return**  
A `const_iterator` to the element, or `end()` if it could not be found.  

---

```cpp
ETL_CONSTEXPR14 pair<const_iterator, const_iterator> equal_range(const key_type& key) const ETL_NOEXCEPT
```
**Description**  
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)  

**Return**  
Returns a range containing all elements equivalent to `key`.

---

```cpp
ETL_CONSTEXPR14 bool contains(const key_type& k) const ETL_NOEXCEPT
```
**Description**  
Check if the container contains the key.  

**Return**  
`true` if the container contains the key, otherwise `false`.

## Search (transparent comparators)

```cpp
template <typename K>
ETL_CONSTEXPR14 const_iterator find(const K& key) const ETL_NOEXCEPT
```
**Description**  
Find the `key` in the container.  

**Return**  
A `const_iterator` to the element, or `end()` if it could not be found.  

---

```cpp
template <typename K>
ETL_CONSTEXPR14 const_iterator lower_bound(const K& key) const ETL_NOEXCEPT
```
**Description**   
Searches for the first element after the `key`.  

**Return**  
A `const_iterator` to the element, or `end()` if it could not be found.  

---

```cpp
template <typename K>
ETL_CONSTEXPR14 const_iterator upper_bound(const K& key) const ETL_NOEXCEPT
```
**Description**  
Searches for the first element with the `key`.  

**Return**  
A `const_iterator` to the element, or `end()` if it could not be found.  

---

```cpp
template <typename K>
ETL_CONSTEXPR14 pair<const_iterator, const_iterator> equal_range(const K& key) const ETL_NOEXCEPT
```
**Description**  
The return type is either `std::pair` (default) or `etl::pair` (`ETL_NO_STL`)  

**Return**  
Returns a range containing all elements equivalent to `key`.

---

```cpp
template <typename K>
ETL_CONSTEXPR14 bool contains(const K& k) const ETL_NOEXCEPT
```
**Description**  
Check if the container contains the key.  

**Return**  
`true` if the container contains the key, otherwise `false`.

## Non-member functions

**Lexicographically comparisons**

```cpp
operator ==
```
`true` if the contents of the sets are equal, otherwise `false`.

---

```cpp
operator != 
```
`true` if the contents of the sets are not equal, otherwise `false`.

---

```cpp
operator <  
```
`true` if the contents of the lhs is less-than the rhs, otherwise `false`.

---

```cpp
operator <=
```
`true` if the contents of the lhs is less-than-equal the rhs, otherwise `false`.

---

```cpp
operator >
```
`true` if the contents of the lhs is greater-than the rhs, otherwise `false`.

---

```cpp
operator >= 
```
`true` if the contents of the lhs is greater-than-equal the rhs, otherwise `false`.

## Technical stuff

The const set is implemented as a sorted array key/value pairs.
