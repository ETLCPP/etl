---
title: "array_view"
---

{{< callout >}}
  Header: `array_view.h`  
  Supported: All versions  
  Similar to: [gsl::array_view](https://learn.microsoft.com/en-us/previous-versions/cpp/parallel/amp/reference/array-view-class)
{{< /callout >}}

This class implements a view in to a range of a C array, `etl::array`, `std::array`, `etl::vector` and `std::vector`.  
It will support construction from any class that supports data() and size() member functions as well as plain C arrays. 

If `ETL_ARRAY_VIEW_IS_MUTABLE` is defined, then the contents of the view are mutable.
The view is non-mutable by default, which is the opposite to the behaviour in 17.5.0 or earlier.

```cpp
etl::array_view<typename T>
```

## Template deduction guides
**C++17 and above**
```cpp
template <typename TArray>
etl::array_view(TArray& a) 
  -> etl::array_view<typename TArray::value_type>;
```
```cpp
template <typename TIterator>
etl::array_view(const TIterator begin_, const TIterator end_)
  -> etl::array_view<etl::remove_pointer_t<TIterator>>;
```  
```cpp
template <typename TIterator, typename TSize>
etl::array_view(const TIterator begin_, const TSize size_)
  -> etl::array_view<etl::remove_pointer_t<TIterator>>;
```

## Examples

```cpp
etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      
etl::array_view view1{ data };
etl::array_view view2{ data.begin(), data.end() };
etl::array_view view3{ data.begin(), data.size() };
etl::array_view view4{ view1 };
```
## Member types

`value_type`
`size_type`
`difference_type`
`reference`
`const_reference`
`pointer`
`const_pointer`
`iterator`
`const_iterator`
`reverse_iterator`
`const_reverse_iterator`

## Constructors

```cpp
ETL_CONSTEXPR array_view()
```
**Description**  
Default constructor.

---

```cpp
template <typename TArray>
ETL_CONSTEXPR explicit array_view(TArray& a)
```
**Description**  
Construct from array or vector like class.

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR array_view(TIterator begin_, TIterator end_)
```
**Description**  
Construct from an iterator or pointer range.

---

```cpp
template <typename TIterator, typename TSize>
ETL_CONSTEXPR array_view(TIterator begin_, TSize size_)
```
**Description**  
Construct from a start and size

---

```cpp
template<const size_t ARRAY_SIZE>
ETL_CONSTEXPR explicit array_view(T(&begin_)[ARRAY_SIZE])
```
**Description**  
Construct from a compile time sized C array.

---

```cpp
ETL_CONSTEXPR array_view(const array_view& other)
```
**Description**  
Copy constructor.

## Modifiers

```cpp
template <typename TIterator>
void assign(TIterator begin_, TIterator end_)
```
**Description**  
Assigns begin and end of the supplied range.  
Does not copy the elements in the range.

---

```cpp
template <typename TIterator, typename TSize>
void assign(TIterator begin_, TSize size_)
```
**Description**  
Assign from a start iterator and length.  
Does not copy the elements in the range.

---

```cpp
void remove_prefix(size_type n)
```
**Description**
Shrinks the view from the front.  
A value of `n` larger than the array will result in undefined behaviour.

---

```cpp
void remove_suffix(size_type n)
```
**Description**  
Shrinks the view from the back.  
A value of `n` larger than the array will result in undefined behaviour.

---
```cpp
void fill(value_type value)
```
*Introduced:* `20.24.0`  

**Description** 
Fill the underlying array with value.

## Element access

Mutable access is only possible by defining the macro `ETL_ARRAY_VIEW_IS_MUTABLE`.
In this case, `reference`, `pointer`, `iterator` and `reverse_iterator` are defined as their `const` counterparts.

```cpp
reference at(size_t i)
const_reference at(size_t i) const
```
**Description**  
Gets a reference or const reference to the indexed element.  
Emits an `etl::array_view_uninitialised` if the view is not initialised.  
Emits an `etl::array_view_bounds` if the index is not in range.  
If asserts or exceptions are not enabled then undefined behaviour occurs.  

**Parameters**  
`i` The index.

**Returns**  
A reference or const reference to the indexed element

---

```cpp
reference operator[](size_t i)
const_reference operator[](size_t i) const
```
**Description**  
Gets a reference or const reference to the indexed element.  

**Returns**  
A reference or const reference to the indexed element.  
Undefined behaviour if the `array_view` is empty.

---

```cpp
reference front()
const_reference front() const
```
**Description**  
Gets a reference or const reference to the first element.

**Returns**  
A reference or const reference to the first element.  
Undefined behaviour if the `array_view` is empty.

---

```cpp
reference back()
const_reference back() const
```
**Description**  
Gets a reference or const reference to the last element.

**Returns**  
A reference or const reference to the last element.  
Undefined behaviour if the `array_view` is empty.

---

```cpp
pointer data()
const_pointer data() const
```
**Description**  
Gets a pointer or const pointer to the first element.

**Returns**  
A pointer or const pointer to the first element.  
Equal to `end()` if the array_view is empty.

## Iterators
```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the array view.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the array view.

---

```cpp
iterator rbegin()
const_iterator rbegin() const
const_iterator crbegin() const
```
**Description**  
Gets a reverse iterator to the beginning of the array view.

---

```cpp
iterator rend()
const_iterator rend() const
const_iterator crend() const
```
**Description**  
Gets a reverse iterator to the end of the array view.

---

Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the array view is zero, otherwise false.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the view.

---

```
C++size_t max_size() const
```
**Description**  
Returns the maximum possible size of the view.

---

## Non-member functions
**Lexicographically comparisons**

`operator ==`  
**Returns**  
Returns `true` if the contents of the array views are equal, otherwise `false`.  

---

`operator !=`  
**Returns**  
`true` if the contents of the array views are not equal, otherwise `false`.  

`operator <`  
**Returns**  
`true` if the contents of the lhs are lexicographically less than the  
contents of the rhs,  otherwise `false`.  

`operator <=`  
**Returns**  
`true` if the contents of the lhs are lexicographically less than or equal to the  
contents of the rhs, otherwise `false`.  

`operator >`  
**Returns**  
`true` if the contents of the lhs are lexicographically greater than the  
contents of the rhs,  otherwise `false`.  

`operator >=`  
**Returns**  
`true` if the contents of the lhs are lexicographically greater than or equal to the  
contents of the rhs, otherwise `false`.

## Hash
There are specialisations of `etl::hash` for `array_view`.

## Example
```cpp
etl::array<int, 10> data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

typedef etl::array_view<int> View;

View view(data.begin() + 2, data.end() - 2);

void Print(const View& view)
{
  for (size_t i = 0; i < view.size(); ++i)
  {
    std::cout << view[i] << " ";
  }
}

Print(view); // Prints "3 4 5 6 7 8"

size_t hashvalue = etl::hash<View>()(view);
```