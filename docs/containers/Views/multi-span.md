---
title: "multi_span"
---

{{< callout >}}
  Header: `multi_span.h`  
{{< /callout >}}

This class implements a span of a range of spans.  
Allows implementation of Scatter/Gather to multiple buffers or ports.  
Iterating through the `multi_span` will iterate through the collection of spans consecutively.  
The list of spans must be in a contiguous list, such as an array or vector.

```cpp
etl::multi_span<typename T>
```
Where `T` is the element type of the spans.

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
iterator                Forward iterator <20.39.5 
                        Bidirectional iterator >=20.39.5
const_iterator          Constant Forward iterator <20.39.5 
                        Constant bidirectional iterator >=20.39.5
reverse_iterator        ETL_OR_STD::reverse_iterator<iterator> 
const_reverse_iterator  ETL_OR_STD::reverse_iterator<const_iterator>

span_type               etl::span<T>
span_list_type          etl::span<const span_type>
```

## Constructors

ETL_CONSTEXPR multi_span(span_list_type span_list)
**Description**  
Construct from a span of spans.

---

template <typename TContainer>
ETL_CONSTEXPR multi_span(TContainer& a) ETL_NOEXCEPT
**Description**  
Construct from a container of spans.

---

template <typename TContainer>
ETL_CONSTEXPR multi_span(const TContainer& a) ETL_NOEXCEPT
**Description**  
Construct from a const container of spans.

---

template <typename TIterator>
ETL_CONSTEXPR multi_span(TIterator begin, TIterator end)
**Description**  
Construct from a range of spans.

---

template <typename TIterator>
ETL_CONSTEXPR multi_span(TIterator begin, size_t length)
**Description**  
Construct from a range of spans.

---

ETL_CONSTEXPR multi_span(const multi_span& other)
**Description**  
Copy constructor.

## Access

```cpp
ETL_CONSTEXPR14 multi_span& operator =(const multi_span& other) ETL_NOEXCEPT
```
**Description**  
Assign from a other `multi_span`.

## Iterators

```cpp
ETL_CONSTEXPR iterator begin() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the beginning of the span.

---

```cpp
ETL_CONSTEXPR iterator end() const ETL_NOEXCEPT
```
**Description**  
Returns an iterator to the end of the span.

---

```cpp
ETL_CONSTEXPR iterator cbegin() const ETL_NOEXCEPT
```
**Description**  
Returns a const iterator to the beginning of the span.  
From: `20.39.5`

---

```cpp
ETL_CONSTEXPR iterator cend() const ETL_NOEXCEPT
```
**Description**  
Returns a const iterator to the end of the span.  
From: `20.39.5`

---

```cpp
ETL_CONSTEXPR iterator rbegin() const ETL_NOEXCEPT
```
**Description**  
Returns a reverse iterator to the beginning of the span.  
From: `20.39.5`

---

```cpp
ETL_CONSTEXPR iterator rend() const ETL_NOEXCEPT
```
**Description**  
Returns a reverse  iterator to the end of the span.  
From: `20.39.5`

---

```cpp
ETL_CONSTEXPR iterator crbegin() const ETL_NOEXCEPT
```
**Description**  
Returns a const reverse iterator to the beginning of the span.  
From: `20.39.5`

---

```cpp
ETL_CONSTEXPR iterator crend() const ETL_NOEXCEPT
```
**Description**  
Returns a const reverse iterator to the end of the span.  
From: `20.39.5`

## Capacity

```cpp
ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
```
**Description**  
Returns true if the size of the multi_span is zero, otherwise false.

---

```cpp
ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
```
**Description**  
Returns the size of the multi_span.

---

```cpp
ETL_CONSTEXPR size_t size_bytes() const ETL_NOEXCEPT
```
**Description**  
Returns the size of the multi_span in bytes.

---

```cpp
ETL_CONSTEXPR size_t size_spans() const ETL_NOEXCEPT
```
**Description**  
Returns the number of the spans.
