---
title: "span"
---

{{< callout >}}
  Header: `span.h`  
  Similar to: `std::span`
{{< /callout >}}

This class implements a view in to a range of a C array, `etl::array`, `std::array`, `etl::vector` and `std::vector`.  
It will support construction from any class that supports `data()` and `size()` member functions as well as plain C arrays.  

From: `20.34.0`  
The ETL's span adds the ability to access circular iterators that will loop through the span when the beginning or end is reached.

```cpp
etl::span<typename T, size_t Extent = etl::dynamic_extent>
```

## Template deduction guides
C++17

```cpp
template <typename TIterator>
span(const TIterator begin, const TIterator end)
  ->span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;
```

```cpp
template <typename TIterator, typename TSize>
span(const TIterator begin, const TSize size)
  ->span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;
```

```cpp
template <typename T, size_t N>
span(T(&)[N])
  -> span<T, N>;
```

```cpp
template <typename T, size_t N>
span(etl::array<T, N>&)
  -> span<T, N>;
```

```cpp
template <typename T, size_t N>
span(const etl::array<T, N>&)
  -> span<const T, N>;
```

```cpp
template <typename T, size_t N>
span(std::array<T, N>&)
  ->span<T, N>;
```

```cpp
template <typename T, size_t N>
span(const std::array<T, N>&)
  ->span<const T, N>;
```

### Examples
```cpp
etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

etl::span span1{ data };
etl::span span2{ data.begin(), data.end() };
etl::span span3{ data.begin(), data.size() };
etl::span span4{ span1 };

int c_array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
etl::span span5{ c_array };
```

## Member types

```cpp
element_type               T
value_type                 remove_cv<T>::type
size_type                  size_t
difference_type            ptrdiff_t
reference                  value_type&
const_reference            const value_type&
pointer                    value_type*
const_pointer              const value_type*
iterator                   A random access iterator
reverse_iterator           A reverse random access iterator
circular_iterator          A circular random access iterator         20.34.0
circular_reverse_iterator  A reverse circular random access iterator 20.34.0
```

## Constructors

```cpp
ETL_CONSTEXPR span()
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR span(TIterator begin, TIterator end)
```
**Description**  
Construct from an iterator or pointer range.

---

```cpp
template <typename TIterator, typename TSize>
ETL_CONSTEXPR span(TIterator begin, TSize size)
```
**Description**  
Construct from a start and size

---

```cpp
template<const size_t ARRAY_SIZE>
ETL_CONSTEXPR explicit span(element_type(&begin)[ARRAY_SIZE])
```
**Description**  
Construct from a compile time sized C array.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR span(etl::array<U, N>& a) ETL_NOEXCEPT
```
**Description**  
Construct from an ETL array.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR span(const etl::array<U, N>& a) ETL_NOEXCEPT
```
**Description**  
Construct from a const ETL array.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR span(std::array<U, N>& a) ETL_NOEXCEPT
```
**Description**  
Enabled if `ETL_USING_CPP11` and `ETL_USING_STL` are `true`.  
Construct from an STL array.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR span(const std::array<U, N>& a) ETL_NOEXCEPT
```
**Description**  
Enabled if `ETL_USING_CPP11` and `ETL_USING_STL` are `true`.  
Construct from a const STL array.

---

```cpp
ETL_CONSTEXPR span(const span& other) ETL_NOEXCEPT
```
**Description**  
Copy constructor.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR span(const etl::span<U, N>& other) ETL_NOEXCEPT
```

## Access

```cpp
ETL_CONSTEXPR reference operator[](size_t i) const
```
**Description**  
Returns a reference to the indexed element.  
Index out of range results in undefined behaviour.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR reference front() const ETL_NOEXCEPT
```
**Description**  
Returns a reference to the first element.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR reference back() const ETL_NOEXCEPT
```
**Description**  
Returns a reference to the last element.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR pointer data() const ETL_NOEXCEPT
```
**Description**  
Returns a pointer to the first element.

---

```cpp
ETL_CONSTEXPR14 span& operator =(const span& other) ETL_NOEXCEPT
```
**Description**  
Assign from a other span

---

```cpp
template <size_t COUNT>
ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, COUNT> first() const
```
**Description**  
Returns a span consisting of the first `COUNT` elements of the current span

---

```cpp
template <size_t COUNT>
ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, COUNT> last() const
```
**Description**  
Returns a span consisting of the last `COUNT` elements of the current span

---

```cpp
template <size_t OFFSET, size_t COUNT = etl::dynamic_extent>
ETL_NODISCARD ETL_CONSTEXPR etl::span<element_type, E> subspan() const 
```
**Description**  
Returns a subspan consisting of the range starting at `OFFSET` for `COUNT` elements of the current span

## Iterators

```cpp
ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the beginning of the span.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR circular_iterator begin_circular() const ETL_NOEXCEPT
```
**Description**  
Returns a circular iterator to the beginning of the span.  
From: `20.34.0`

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the end of the span.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR iterator rbegin() const  ETL_NOEXCEPT
```
**Description**  
Returns a reverse iterator to the beginning of the span.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR reverse_circular_iterator rbegin_circular() const  ETL_NOEXCEPT
```
**Description**  
Returns a reverse circular iterator to the beginning of the span.  
From: `20.34.0`

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR iterator rend() const ETL_NOEXCEPT
```
**Description**  
Returns a reverse iterator to the end of the span.

## Capacity

```cpp
ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
```
**Description**  
Returns `true` if the size of the span is zero, otherwise `false`.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
```
**Description**  
Returns the size of the span.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
```
**Description**  
Returns the size of the span in bytes.

## Non-member functions

```cpp
template <typename T1, size_t N1, typename T2, size_t N2>
ETL_NODISCARD 
ETL_CONSTEXPR
bool operator ==(const etl::span<T1, N1>& lhs, const etl::span<T2, N2>& rhs) ETL_NOEXCEPT
```
**Description**  
Compare two spans for equality.  
Returns true if they both point to the same range of data.  
From: `20.35.12`

---

```cpp
template <typename T1, size_t N1, typename T2, size_t N2>
ETL_NODISCARD
ETL_CONSTEXPR
bool operator !=(const etl::span<T1, N1>& lhs, const etl::span<T2, N2>& rhs) ETL_NOEXCEPT
```
**Description**  
Compare two spans for inequality.  
Returns true if they don't both point to the same range of data.  
From: `20.35.12`

---

```cpp
template <typename T1, size_t N1, typename T2, size_t N2>
bool equal(const etl::span<T1, N1>& lhs, const etl::span<T2, N2>& rhs)
```
**Description**  
Equality function.  
Performs a comparision of the range values.  
Returns `true` if one of the following are `true`  
1. Both spans are empty.  
2. They both point to the same range of data. 
3. The values in the two ranges are equal.  

From: `20.35.12`

## Hash
There is a specialisation of etl::hash for etl::span

### Example

**Iterating through a span and subspan.**

```cpp
etl::array<int, 10> data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
using View = etl::span<int>;
View view(data.begin() + 2, data.end() - 2);

void Print(const View& view)
{
  for (size_t i = 0; i < view.size(); ++i)
  {
    std::cout << view[i] << " ";
  }
}

Print(view); // Prints "3 4 5 6 7 8"

auto subview = view.subspan<2, 3>();

Print(subview); // Prints "5 6 7"

size_t hashvalue = etl::hash<View>()(view);
```

**Looping a span.**  

```cpp
etl::array<int, 10> data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
using View = etl::span<int>;
View view(data.begin(), data.end());

View::circular_iterator itr = view.begin_circular();

// Loop through the data for 1000 iterations.
for (int i; i < 1000; ++i)
{
  Print(*itr++);
}
```
