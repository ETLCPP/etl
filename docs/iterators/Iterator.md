---
title: iterator
---

{{< callout type="info">}}
  Header: `iterator.h`  
  Since: `TBC`  
{{< /callout >}}


A set of templates to more easily determine the properties of iterator types.

## Input iterators
```cpp
is_input_iterator<T>::value
```
**Description**  
Is `T` an input iterator?

---

```cpp
is_input_iterator_concept<T>::value
```
**Description**  
Can `T` be used as an input iterator?

## Output iterators
```cpp
is_output_iterator<T>::value
```
**Description**  
Is `T` an output iterator?

---

```cpp
is_output_iterator_concept<T>::value
```
**Description**  
Can `T` be used as an output iterator?

## Forward iterators
```cpp
is_forward_iterator<T>::value
```
**Description**  
Is `T` a forward iterator?

---

```cpp
is_forward_iterator_concept<T>::value
```
**Description**  
Can `T` be used as an forward iterator?

## Bidirectional iterators
```cpp
is_bidirectional_iterator<T>::value
```
**Description**  
Is `T` a bidirectional iterator?

---

```cpp
is_bidirectional_iterator_concept<T>::value
```
**Description**  
Can `T` be used as a bidirectional iterator?

## Random iterators
```cpp
is_random_iterator<T>::value
```
**Description**  
Is `T` a random iterator?

```cpp
is_random_iterator_concept<T>::value
```
**Description**  
Can `T` be used as a random iterator?

## Iterator tags
```cpp
struct input_iterator_tag
struct output_iterator_tag
struct forward_iterator_tag
struct bidirectional_iterator_tag
struct random_access_iterator_tag
```

## Iterator traits
```cpp
template <typename TIterator>
struct iterator_traits
```
**Defined types**  
```cpp
iterator_category
value_type
difference_type
pointer
reference
```

## advance
```cpp
template <typename TIterator, typename TDistance>
ETL_CONSTEXPR17 void advance(TIterator& itr, TDistance n)
```

## prev
```cpp
template<typename TIterator>
ETL_CONSTEXPR17 TIterator prev(TIterator itr, 
                               typename etl::iterator_traits<TIterator>::difference_type n = 1)
```

## next
```cpp
template<typename TIterator>
ETL_CONSTEXPR17 TIterator next(TIterator itr, 
                               typename etl::iterator_traits<TIterator>::difference_type n = 1)
```

## distance
```cpp
template<typename TIterator>
ETL_CONSTEXPR17 typename std::iterator_traits<TIterator>::difference_type 
 distance(TIterator first, TIterator last)
```

## iterator
A base class provided to simplify definitions of the required types for iterators.
https://en.cppreference.com/w/cpp/iterator/iterator

```cpp
template <typename TCategory,
          typename T, 
          typename TDistance = ptrdiff_t, 
          typename TPointer = T* , 
          typename TReference = T&>
struct iterator
```

**Defined types**  
```cpp
value_type
difference_type
pointer
reference
iterator_category
```

## reverse_iterator
An iterator adaptor that reverses the direction of a given iterator

```cpp
template <typename TIterator>
class reverse_iterator
```

**Defined types**  
```cpp
iterator_category
value_type
iterator_type
difference_type
pointer
reference
```

## move_iterator
An iterator adaptor that converts the value returned by the underlying iterator into an rvalue.
C++11 or above.

```cpp
template <typename TIterator>
class move_iterator
```

**Defined types**  
```cpp
iterator_category
value_type
iterator_type
difference_type
pointer
reference
```

## back_insert_iterator
Inserts using `push_back`.
```cpp
template <typename TContainer>
class back_insert_iterator
```

---

```cpp
template <typename TContainer>
ETL_NODISCARD 
ETL_CONSTEXPR14 
etl::back_insert_iterator<TContainer> back_inserter(TContainer& container)
```

## front_insert_iterator
Inserts using `push_front`.
```cpp
template <typename TContainer>
class front_insert_iterator
```

---

```cpp
template <typename TContainer>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::front_insert_iterator<TContainer> front_inserter(TContainer& container)
```

## push_insert_iterator
Inserts using `push`.
```cpp
template <typename TContainer>
class push_insert_iterator
```

---

```cpp
template <typename TContainer>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::push_insert_iterator<TContainer> push_inserter(TContainer& container)
```

## begin
```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::iterator begin(TContainer& container)
Get the 'begin' iterator of `container`.
```
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::const_iterator begin(const TContainer& container)
```
Get the `begin` `const_iterator` of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::const_iterator cbegin(const TContainer& container)
```
Get the `begin` `const_iterator` of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR TValue* begin(TValue(&data)[Array_Size])
```
Get the `begin` `pointer` for an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR const TValue* begin(const TValue(&data)[Array_Size])
```
Get the `begin` `const pointer` for an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR const TValue* cbegin(const TValue(&data)[Array_Size])
```
Get the `begin` `const pointer` for an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::reverse_iterator rbegin(TContainer& container)
```
Get the `begin` `reverse_iterator` of `container`.  
Enabled if ETL_NOT_USING_STL or ETL_CPP14_NOT_SUPPORTED.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::const_reverse_iterator rbegin(const TContainer& container)
```
Get the `begin` `const_reverse_iterator` of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::const_reverse_iterator crbegin(const TContainer& container)
```
Get the `begin`' `const_reverse_iterator` of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_OR_STD::reverse_iterator<TValue*> rbegin(TValue(&data)[Array_Size])
```
Get the `begin` `reverse_iterator` for an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR ETL_OR_STD::reverse_iterator<const TValue*> crbegin(const TValue(&data)[Array_Size])
```
Get the `begin` `const_reverse_iterator` for an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

## end
```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::iterator end(TContainer& container)
```
Get the `end` iterator of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::const_iterator end(const TContainer& container)
```
Get the `end` `const_iterator` of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::const_iterator cend(const TContainer& container)
```
Get the `end` `const_iterator` of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR TValue* end(TValue(&data)[Array_Size])
```
Get the `end` `pointer` for an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR const TValue* end(const TValue(&data)[Array_Size])
```
Get the `end` `const pointer` of an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR const TValue* cend(const TValue(&data)[Array_Size])
```
Get the `end` `const pointer` of an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP11_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::reverse_iterator rend(TContainer& container)
```
Get the `end` `reverse_iterator` of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::const_reverse_iterator rend(TContainer& container)
```
Get the `end` `const_reverse_iterator` of `container`.
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

---

```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::const_reverse_iterator crend(const TContainer& container)
```
Get the `end` `const_reverse_iterator` of `container`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR ETL_OR_STD::reverse_iterator<TValue*> rend(TValue(&data)[Array_Size])
```
Get the `end` `reverse_iterator` for an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR ETL_OR_STD::reverse_iterator<const TValue*> crend(const TValue(&data)[Array_Size])
```
Get the `end` `const_reverse_iterator` for an array.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP14_NOT_SUPPORTED`.

## size
```cpp
template <typename TContainer>
ETL_CONSTEXPR typename TContainer::size_type size(const TContainer& container)
```
Get the size of a container.  
Expects the container to have defined `size_type`.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP17_NOT_SUPPORTED`.

---

```cpp
template <typename TValue, size_t Array_Size>
ETL_CONSTEXPR size_t size(TValue(&)[Array_Size])
```
Get the size of an array in elements at run time, or compile time if C++11 or above.  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP17_NOT_SUPPORTED`.

---

```cpp
template <typename T, size_t Array_Size>
char(&array_size(T(&array)[Array_Size]))[Array_Size];
```
Get the size of an array in elements at compile time for C++03  
Usage:- `sizeof(array_size(array))`  
Enabled if `ETL_NOT_USING_STL` or `ETL_CPP17_NOT_SUPPORTED`.

---

`ETL_ARRAY_SIZE(a)` calls `sizeof(etl::array_size(a))`
