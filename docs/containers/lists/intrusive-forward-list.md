---
title: "intrusive_forward_list"
weight: 4
---

{{< callout type="info">}}
  Header: `intrusive_forward_list.h`  
  Similar to: `std::forward_list`
{{< /callout >}}

An intrusive forward list.https://www.messenger.com/t/837634551/

```cpp
template <typename TValue, typename TLink>
etl::intrusive_forward_list
```

`TValue` is the type that contains the actual values. It is derived from `Tlink`. `Tlink` is the link type for this list.  
See Intrusive links.  

Before `20.37.0` the default link type was `etl::forward_link<0>`.    

## Member types

```cpp
link_type           TLink             
value_type          TValue            
pointer             value_type*       
const_pointer const value_type* 
reference           value_type&       
const_reference     const value_type& 
size_type           size_t            
```

## Constructors

```pcc
etl::intrusive_forward_list<typename TValue, typename TLink>();
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
etl::intrusive_forward_list<typename TValue, typename TLink>(TIterator begin, TIterator end);
```
**Description**  
Creates the list from the range [`begin`, `end`) of node links.

---

```cpp
template <typename... TLinks>
intrusive_forward_list(TLink& first, TLinks&... links);
```cpp
**Description**  
Creates the list from node link references.

## Element access

```cpp
TValue& front()
const T& front() const
```
**Description**  
Returns a reference or const reference to the first element.

---

```cpp
TValue& back()
const T& back() const
```
**Description**  
Returns a reference or const reference to the last element.

## Iterators

```cpp
iterator begin()
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the list.

---

```cpp
iterator end()
const_iterator end() const
const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the list.

## Capacity

```cpp
bool empty() const
```
**Description**  
Returns true if the size of the list is zero, otherwise false.

---

```cpp
size_t size() const
```
**Description**  
Returns the size of the list.

## Modifiers

```cpp
template <typename TIterator>
void assign(TIterator begin, TIterator end);
```
**Description**  
Fills the list with the values.

---

```cpp
void push_front(value_type& value);
```
**Description**  
Pushes a value to the front of the list. 

---

```cpp
void pop_front();
```
**Description**  
Pop a value from the front of the list.  
Emits an `etl::intrusive_forward_list_empty` if the list is empty.  
If asserts or exceptions are disabled then undefined behaviour occurs.

---

```cpp
template <typename TIterator>
void insert_after(iterator position, TIterator begin, TIterator end);
```
**Description**  

---

```cpp
iterator insert_after(iterator position, value_type& value);
```
**Description**  
Inserts values in to the list.  
position is not checked for validity.

---

```cpp
template <typename TIterator>
iterator erase_after(TIterator begin, TIterator end);
```
**Description**  
Erases values in the range [`begin`, `end`).  
Iterators are not checked for validity.

---

```cpp
iterator erase_after(iterator position);
```
**Description**  
Erases values in the list.  
Iterators are not checked for validity.

---

```cpp
void clear()
```
**Description**  
Clears the list to a size of zero. No elements are destructed.

---

```cpp
void splice_after(iterator position, list_type& list);
void splice_after(iterator position, list_type& list, iterator isource);
void splice_after(iterator position, list_type& list, iterator begin_, iterator end_);
```
**Description**  
Splices elements from a list into this list.  
Iterators are not checked for validity.

---

```cpp
void merge(list_type& list);
```
**Description**  
Merges the sorted elements of 'list' into this list. Merges are stable.  
If a debug compile and asserts or exceptions are enabled than an etl::intrusive_list_unsorted is emitted if either list is unsorted, otherwise undefined behaviour occurs.

---

```cpp
template <typename TCompare>
void merge(list_type& list, Tcompare compare)
```
**Description**  
Merges the sorted elements of list into this list. Comparison functor is supplied in compare. Merges are stable.  
If a debug compile and asserts or exceptions are enabled than an etl::intrusive_list_unsorted is emitted if either list is unsorted, otherwise undefined behaviour occurs.

## Operations

```cpp
void remove(const T& value);
```
**Description**  
Removes from the container all the elements that compare equal to value.

---

```cpp
template <typename TPredicate>
void remove_if(TPredicate predicate);
```
**Description**  
Removes from the container all the elements that satisfy predicate.

---

```cpp
void unique();
```
**Description**  
Removes all but the first element from every group of consecutive elements.

---

```cpp
template <typename TPredicate>
void unique(TPredicate predicate);
```
**Description**  
Removes all but the first element from every group of consecutive elements that satisfy the binary `predicate`.

---

```cpp
void sort();
```
**Description**  
Sorts using the < operator.

---

```cpp
template <typename TCompare>
void sort(TCompare compare);
```
**Description**  
Sorts using the supplied `compare` function.

---

```cpp
void reverse();
```
**Description**  
Reverses the order of the list.

## Non-member functions

```cpp
operator ==
```
**Description**  
`true` if the contents of the lists are equal, otherwise `false`.

---

```cpp
operator !=
```
**Description**  
`true` if the contents of the lists are not equal, otherwise `false`.

---

```cpp
operator <
```
**Description**  
`true` if the contents of the lhs are lexicographically less than the contents of the rhs,  otherwise `false`.

---

```cpp
operator <=
```
**Description**  
`true` if the contents of the lhs are lexicographically less than or equal to the contents of the rhs, otherwise `false`.

---

```cpp
operator >
```
**Description**  
`true` if the contents of the lhs are lexicographically greater than the contents of the rhs,  otherwise `false`.

---

```cpp
operator >=
```
**Description**  
`true` if the contents of the lhs are lexicographically greater than or equal to the contents of the rhs, otherwise `false`.
