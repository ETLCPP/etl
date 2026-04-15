---
title: "Histgram"
---

{{< callout type="info">}}
  Header: `histogram.h`  
  Since: `20.9.0`  
{{< /callout >}}

## Member types
`const_iterator` The iterator used to traverse the histogram data.  
`key_type`       The index type for the histogram.  
`count_type`     The type used for the histogram counter.  
`value_type`     The type returned from the histogram.  

## Constants
`Max_Size` The maximum number of elements in the histogram.

## histogram
```cpp
template <typename TKey, typename TCount, size_t Max_Keys>
class histogram
```
**Description**  
A histogram where the start index of the  keys is defined in the constructor.

## Member functions
```cpp
histogram()
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
histogram(TIterator first, TIterator last)
```
**Description**  
Construct from an iterator range.

---

## histogram

```cpp
template <typename TKey, typename TCount, size_t Max_Keys, size_t Start_Index>
class histogram
```
**Description**  
A histogram where the start index of the  keys is defined as a template parameter.

## Member functions

```cpp
histogram()
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
histogram(TIterator first, TIterator last)
```
**Description**  
Construct from an iterator range.

---

```cpp
histogram(const histogram& other)
```
**Description**  
Copy constructor.

---

```cpp
histogram(histogram&& other)
```
**Description**  
Move constructor.

---

```cpp
histogram& operator =(const histogram& rhs)
```
**Description**  
Assignment operator.

---

```cpp
histogram& operator =(histogram&& rhs)
```
**Description**  
Move assignment operator.

---

```cpp
void add(key_type key)
```
**Description**  
Increment the count for the key.

---

```cpp
template <typename TIterator>
void add(TIterator first, TIterator last)
```
**Description**  
Increment the counts for the keys from an iterator range.

---

```cpp
void operator ()(key_type key)
```
**Description**  
Increment the count for the key.

---

```cpp
template <typename TIterator>
void operator ()(TIterator first, TIterator last)
```
**Description**  
Increment the counts for the keys from an iterator range.

---

```cpp
value_type operator [](key_type key) const
```
**Description**  
Get the count for a key.

---

```cpp
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Get the iterator to the first histogram entry.

---

```cpp
const_iterator end() const
const_iterator cend() const
```
**Description**  
Get the iterator to the last + 1 histogram entry.

---

```cpp
void clear()
```
**Description**  
Clear the counts to zero.

---

```cpp
ETL_CONSTEXPR size_t size() const
```
Get the number of items in the histogram.
**Description**  
Always equal to max_size().

---

```cpp
ETL_CONSTEXPR size_t max_size() const
```
**Description**  
Get the number of items in the histogram.

---

```cpp
size_t count() const
```
**Description**  
Get the total of all the counts.

## sparse_histogram

```cpp
template <typename TKey, typename TCount, size_t Max_Keys>
class sparse_histogram
```
**Description**  
A histogram where the keys are sparse or non-integral types.

## Types
`const_iterator` The iterator used to traverse the histogram data.
`key_type`       The index type for the histogram.
`count_type`     The type used for the histogram counter.
`value_type`     The type returned from the histogram. A pair containing the key and count.

## Constants
`Max_Size` The maximum number of elements in the histogram.

## Member functions
```cpp
sparse_histogram()
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
sparse_histogram(TIterator first, TIterator last)
```
**Description**  
Construct from an iterator range.

---

```cpp
sparse_histogram(const sparse_histogram& other)
```
**Description**  
Copy constructor.

---

```cpp
sparse_histogram(sparse_histogram&& other)
```
**Description**  
Move constructor.

---

```cpp
sparse_histogram& operator =(const sparse_histogram& rhs)
```
**Description**  
Assignment operator.

---

```cpp
sparse_histogram& operator =(sparse_histogram&& rhs)
```
**Description**  
Move assignment operator.

---

```cpp
void add(key_type key)
```
**Description**  
Increment the count for the key.

---

```cpp
template <typename TIterator>
void add(TIterator first, TIterator last)
```
**Description**  
Increment the counts for the keys from an iterator range.

---

```cpp
void operator ()(key_type key)
```
**Description**  
Increment the count for the key.

---

```cpp
template <typename TIterator>
void operator ()(TIterator first, TIterator last)
```
**Description**  
Increment the counts for the keys from an iterator range.

---

```cpp
const value_type& operator [](key_type key) const
```
**Description**  
Get the count for a key.

---

```cpp
const_iterator begin() const
const_iterator cbegin() const
```
**Description**  
Get the iterator to the first histogram entry.

---

```cpp
const_iterator end() const
const_iterator cend() const
```
**Description**  
Get the iterator to the last + 1 histogram entry.

---

```cpp
void clear()
```
**Description**  
Clear the counts to zero.

---

```cpp
ETL_CONSTEXPR size_t size() const
```
**Description**  
Get the number of items in the histogram.

---

```cpp
ETL_CONSTEXPR size_t max_size() const
```
**Description**  
Get the number of items in the histogram.

---

```cpp
size_t count() const
```
**Description**  
Get the total of all the counts.
