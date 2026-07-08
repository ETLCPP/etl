---
title: "poly_span"
---

{{< callout >}}
  Header: `poly_span.h`  
  From: `20.31.0`
{{< /callout >}}

Polymorphic span.  
This class implements a view in to a contiguous range through a base type.  

```cpp
etl::poly_span<typename TBase, size_t Extent = etl::dynamic_extent>
```

## Template deduction guides
C++17

```cpp
template <typename TIterator>
poly_span(const TIterator begin, const TIterator end)
  ->poly_span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;
```

```cpp
template <typename TIterator, typename TSize>
poly_span(const TIterator begin, const TSize size)
  ->poly_span<etl::remove_pointer_t<TIterator>, etl::dynamic_extent>;
```

```cpp
template <typename T, size_t N>
poly_span(T(&)[N])
  -> poly_span<T, N>;
```

```cpp
template <typename T, size_t N>
poly_span(etl::array<T, N>&)
  -> poly_span<T, N>;
```

```cpp
template <typename T, size_t N>
poly_span(const etl::array<T, N>&)
  -> poly_span<const T, N>;
```

```cpp
template <typename T, size_t N>
poly_span(std::array<T, N>&)
  ->poly_span<T, N>;
```

```cpp
template <typename T, size_t N>
poly_span(const std::array<T, N>&)
  ->poly_span<const T, N>;
```

## Member types

```cpp
element_type            T
value_type              remove_cv<T>::type
size_type               size_t
difference_type         ptrdiff_t
reference               value_type&
const_reference         const value_type&
pointer                 value_type*
const_pointer           const value_type*
iterator                Random access iterator
reverse_iterator        ETL_OR_STD::reverse_iterator<iterator>
```

## Constructors

```cpp
ETL_CONSTEXPR poly_span() ETL_NOEXCEPT
```

---

```cpp
template <typename TIterator, typename TSize>
ETL_CONSTEXPR poly_span(const TIterator begin, const TSize size) ETL_NOEXCEPT
```
**Description**  
Static asserts if `etl::iterator_traits<TIterator>::iterator_category` is not `random_access_iterator_tag`  
or `etl::iterator_traits<TIterator>::value_type` is not the same as, or not derived from `TBase`.

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR poly_span(const TIterator begin, const TIterator end)
```
**Description**  
Static asserts if `etl::iterator_traits<TIterator>::iterator_category` is not `random_access_iterator_tag`  
or `etl::iterator_traits<TIterator>::value_type` is not the same as, or not derived from `TBase`.

---

```cpp
template<typename U, size_t N>
ETL_CONSTEXPR poly_span(U(&begin_)[N]) ETL_NOEXCEPT
```
**Description**  
Static asserts if `N` is greater than `Extent` or `etl::iterator_traits<TIterator>::value_type` is not the same as, or not derived from `TBase`.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR poly_span(etl::array<U, N>& a) ETL_NOEXCEPT
```
**Description**  
Static asserts if `N` is greater than `Extent` or `etl::iterator_traits<TIterator>::value_type` is not the same as, or not derived from `TBase`.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR poly_span(const etl::array<U, N>& a) ETL_NOEXCEPT
```
**Description**  
Static asserts if `N` is greater than `Extent` or `etl::iterator_traits<TIterator>::value_type` is not the same as, or not derived from `TBase`.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR poly_span(std::array<U, N>& a) ETL_NOEXCEPT
```
**Description**  
Enabled if `ETL_USING_CPP11` and `ETL_USING_STL` are `true`.  
Static asserts if `N` is greater than `Extent` or `etl::iterator_traits<TIterator>::value_type` is not the same as, or not derived from `TBase`.

---

```cpp
template <typename U, size_t N>
ETL_CONSTEXPR poly_span(const std::array<U, N>& a) ETL_NOEXCEPT
```
**Description**  
Enabled if `ETL_USING_CPP11` and `ETL_USING_STL` are `true`.  
Static asserts if `N` is greater than `Extent` or `etl::iterator_traits<TIterator>::value_type` is not the same as, or not derived from `TBase`.

---

```cpp
ETL_CONSTEXPR poly_span(const poly_span& other)
```
**Description**  
Copy constructor.

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
ETL_CONSTEXPR14 poly_span& operator =(const poly_span& other) ETL_NOEXCEPT
```
**Description**  
Assign from a other span

---

```cpp
template <typename UBase>
ETL_CONSTEXPR14 poly_span& operator =(const poly_span<UBase, Extent>& other) ETL_NOEXCEPT
```
**Description**  
Assign from a other span.

---

```cpp
template <size_t COUNT>
ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT> first() const
```
**Description**  
Returns a span consisting of the first `COUNT` elements of the current span.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> last(size_t count) const
```
**Description**  
Returns a span consisting of the last count elements of the current span.

---

```cpp
template <size_t COUNT>
ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT> last() const
```
**Description**  
Returns a span consisting of the last `COUNT` elements of the current span

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> first(size_t count) const
```
**Description**  
Returns a span consisting of the first count elements of the current span.

---

```cpp
template <const size_t OFFSET, size_t COUNT = etl::dynamic_extent>
ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, COUNT != etl::dynamic_extent ? COUNT : Extent - OFFSET> subspan() const
```
**Description**  
Returns a subspan consisting of the range starting at `OFFSET` for `COUNT` elements of the current span.

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR etl::poly_span<element_type, etl::dynamic_extent> subspan(size_t offset, size_t count = etl::dynamic_extent) const
```
**Description**  
Returns a subspan consisting of the range starting at `offset` for `count` elements of the current span.

## Iterators

```cpp
ETL_NODISCARD ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the beginning of the span.

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
ETL_NODISCARD ETL_CONSTEXPR iterator rend() const ETL_NOEXCEPT
```
**Description**  
Returns a reverse iterator to the end of the span.

## Capacity

```cpp
ETL_NODISCARD ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
```
**Description**  
Returns true if the size of the span is zero, otherwise false.

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

---

```cpp
ETL_NODISCARD ETL_CONSTEXPR size_t size_of_element() const ETL_NOEXCEPT
```
**Description**  
Returns the size of the elements pointed to by the span, in bytes.

## Non-member functions

### Hash
There is a specialisation of `etl::hash` for `etl::poly_span`.

---

### Example
```cpp
struct Base
{
  virtual ~Base() {}
  virtual void Print() const = 0;
};

struct Derived : Base
{
  Derived(int i_)
    : i(i_)
  {
  }

  void Print() const override
  {
    std::cout << "Derived " << i << "\n";
  }

  int i;
};

etl::array data{ Derived(1), Derived(2), Derived(3), Derived(4) };

etl::poly_span<Base> ps{ data };

for (const Base& b : ps)
{
  b.Print();
}
```
