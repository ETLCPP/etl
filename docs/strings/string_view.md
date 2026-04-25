---
title: "basic_string_view"
---

{{< callout type="info">}}
  Header: `string_view.h`  
  Similar to:  
  `std::basic_string_view`  
  `std::string_view`  
  `std::wstring_view`  
  `std::u8string_view`  
  `std::u16string_view`  
  `std::u32string_view`
{{< /callout >}}

This class implements a view in to a range of a `C` string, `etl::string` (+ variants), `std::string` (+ variants).

## Classes

```cpp
etl::basic_string_view<typename T, typename TTraits = etl::char_traits<T>>
```

## Types

```cpp
etl::string_view
etl::wstring_view
etl::u8string_view;
etl::u16string_view
etl::u32string_view
```

## Member types

```cpp
value_type              T
size_type               std::size_t
difference_type         std::ptrdiff_t
reference               value_type&
const_reference         const value_type&
pointer                 value_type*
const_pointer           const value_type*
iterator                Random access iterator
const_iterator          Constant random access iterator
reverse_iterator        std::reverse_iterator<iterator>
const_reverse_iterator  std::reverse_iterator<const_iterator>
```

## Constants

```cpp
size_t npos
```
**Description**  
An end of view indicator by the functions that expect a view index.  
An error indicator for functions that return a view index.

## Constructors

```cpp
ETL_CONSTEXPR basic_string_view()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR basic_string_view(const T* begin, size_t size)
```
**Description**  
Construct from a start and size.

---

```cpp
ETL_CONSTEXPR basic_string_view(const T* begin)
```
**Description**  
Construct from a zero terminated string.

---

```cpp
ETL_CONSTEXPR basic_string_view(const etl::basic_string_view& other)
```
**Description**  
Copy constructor.

## Modifiers

```cpp
void remove_prefix(size_t n)
```
**Description**  
Shrinks the view from the front.  
Undefined behaviour if n is larger than the string.

---

```cpp
void remove_suffix(size_t n)
```
**Description**  
Shrinks the view from the back.  
Undefined behaviour if n is larger than the string.

---

```cpp
void swap(etl::basic_string_view<T, TTraits>& view)
```
**Description**  
Swaps with another view.

## Element access

```cpp
const T& at(size_t i) const
```
**Description**  
Returns a const reference to the indexed element.  
Emits an `etl::string_view_uninitialised` if the view is not initialised.  
Emits an `etl::string_view_bounds` if the index is not in range.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---

```cpp
const_reference operator[](size_t i) const
```
**Description**  
Returns a const reference to the indexed element.  
Undefined behaviour if the string is empty.

---

```cpp
const_reference front() const
```
**Description**  
Returns a const reference to the first element.
Undefined behaviour if the string is empty.

---

```cpp
const_reference back() const
```
**Description**  
Returns a const reference to the last element.
Undefined behaviour if the string is empty.

---

```cpp
const_pointer data() const
```
**Description**  
Returns a const pointer to the first element.
Undefined behaviour if the string is empty.

## Iterators

```cpp
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the string view.

---

```cpp
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the string view.

---

```cpp
const_iterator rbegin() const
const_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the string view.

---

```cpp
const_iterator rend() const
const_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the string view.

## Operations

```cpp
size_t copy(T* destination, size_t count, size_t position = 0) const
```
**Description**  
Copies the sub-string at position for count characters or size() - count, whichever smaller,  to the string pointed to by destination.

---

```cpp
basic_string_view substr(size_t position = 0, size_t count = npos) const
```
**Description**  
Returns a view which is the sub-string at position for count characters or size() - count, whichever smaller.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the string view is zero, otherwise false.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the view.

---

```cpp
size_t length() const
```
**Description**  
Alternative for size().

---

```cpp
size_t max_size() const
```
**Description**  
Returns the maximum possible size of the view.

## Compare

```cpp
int compare(basic_string_view v) const
int compare(size_t position1, size_t count1, basic_string_view view) const
int compare(size_t position1, size_t count1, 
            basic_string_view view, 
            size_t position2, size_t count2) const
int compare(const T* text) const
int compare(size_t position1, size_t count, const T* text) const
int compare(size_t position1, size_t count1, const T* text, size_t count2) const
```
**Description**  
Compares two character sequences.

## Search

```cpp
bool starts_with(etl::basic_string_view view) const
bool starts_with(T x) const
bool starts_with(const T* x) const
```
**Description**  
Checks if the string view begins with the given prefix.

---

```cpp
bool ends_with(etl::basic_string_view view) const
bool ends_with(T x) const
bool ends_with(const T* x) const
```
**Description**  
Checks if the string view ends with the given prefix.

---

```cpp
size_t find(basic_string_view view, size_type position = 0) const
size_t find(T c, size_t position = 0) const
size_t find(const T* text, size_t position, size_t count) const
size_t find(const T* text, size_t position = 0) const;
```
**Description**  
Finds the first sub-string equal to the given character sequence.

---

```cpp
size_t rfind(basic_string_view view, size_t position = npos) const
size_t rfind(T c, size_t position = npos) const
size_t rfind(const T* text, size_t position, size_t count) const
size_t rfind(const T* text, size_t position = npos) const
```
**Description**  
Finds the last sub-string equal to the given character sequence.

---

```cpp
size_t find_first_of(basic_string_view view, size_t position = 0) const
size_t find_first_of(T c, size_t position = 0) const
size_t find_first_of(const T* text, size_t position, size_t count) const
size_t find_first_of(const T* text, size_t position = 0) const
```
**Description**  
Finds the first character equal to any of the characters in the given character sequence.

---

```cpp
size_t find_last_of(basic_string_view view, size_t position = npos) const
size_t find_last_of(T c, size_t position = npos) const
size_t find_last_of(const T* text, size_t position, size_type count) const
size_t find_last_of(const T* text, size_t position = npos) const
```
**Description**  
Finds the last character equal to any of the characters in the given character sequence.

---

```cpp
size_t find_first_not_of(basic_string_view view, size_t position = 0) const
size_t find_first_not_of(T c, size_t position = 0) const
size_t find_first_not_of(const T* text, size_t position, size_t count) const
size_t find_first_not_of(const T* text, size_t position = 0) const
```
**Description**  
Finds the first character not equal to any of the characters in the given character sequence.

---

```cpp
size_t find_last_not_of(basic_string_view view, size_t position = npos) const
size_t find_last_not_of(T c, size_t position = npos) const
size_t find_last_not_of(const T* text, size_t position, size_t count) const
size_t find_last_not_of(const T* text, size_t position = npos) const
```
**Description**  
Finds the first character not equal to any of the characters in the given character sequence.

## Non-member functions
**Lexicographically comparisons**  

`==` `true` if the contents of the string views are equal,  
otherwise `false`.  

`!=` `true` if the contents of the string views are not equal,  
otherwise `false`.  

`< ` `true` if the contents of the lhs are lexicographically less than the contents of the rhs,  
otherwise `false`.  

`<=` `true` if the contents of the lhs are lexicographically less than or equal to the contents of the rhs,  
otherwise `false`.  

`> ` `true` if the contents of the lhs are lexicographically greater than the contents of the rhs,  
otherwise `false`.  

`>=` `true` if the contents of the lhs are lexicographically greater than or equal to the contents of the rhs,  
otherwise `false`.  

---

```cpp
void swap(etl::basic_string_view<T, TTraits> lhs,
          etl::basic_string_view<T, TTraits> rhs)
```
Swaps two views.

## Hash

```cpp
etl::hash<etl::string_view>
etl::hash<etl::wstring_view>
etl::hash<etl::u8string_view>
etl::hash<etl::u16string_view>
etl::hash<etl::u32string_view>
---

**Example**  

```cpp
etl::string<10> greeting("Hello World");

using View = etl::string_view;

View view(greeting.begin() + 2, greeting.size() - 4);

void Print(const View& view)
{
  std::cout << std::string(view.begin(), view.end());
}

Print(view); // Prints "llo Wo"

size_t hashvalue = etl::hash<View>()(view);
```
