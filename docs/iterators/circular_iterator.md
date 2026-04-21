---
title: "circular_iterator"
---

{{< callout type="info">}}
  Header: `circular_iterator.h`  
  Since: `20.34.0`  
{{< /callout >}}

An iterator wrapper where increments and decrements will roll around the boundaries of an iterator range.  
It is constructed with the begin and end of the range, plus an optional start position.  
May be used with forward, bidirectional and random access iterators.  

```cpp
template <typename TIterator>
etl::circular_iterator final;
```

## Types

```cpp
value_type
difference_type
pointer
reference
iterator_category
```

## Constructors
```cpp
ETL_CONSTEXPR14 circular_iterator()
```
Default constructor.

---

```cpp
ETL_CONSTEXPR14 circular_iterator(TIterator itr_begin, TIterator itr_end)
```
Construct from iterators.

---

```cpp
ETL_CONSTEXPR14 circular_iterator(TIterator itr_begin, TIterator itr_end, TIterator itr_start)
```
Construct from iterators and start position

---

```cpp
ETL_CONSTEXPR14 circular_iterator(const circular_iterator& other)
```
Copy constructor

## Member functions
```cpp
ETL_CONSTEXPR14 circular_iterator& operator =(const circular_iterator& other)
```
Assignment operator

---

```cpp
ETL_CONSTEXPR14 size_t size() const
```
Returns the length of the range.

---

```cpp
ETL_CONSTEXPR14 bool empty() const
```
Returns true if the range is empty.

---

```cpp
ETL_CONSTEXPR14 value_type operator *()
```
Dereference operator

---

```cpp
ETL_CONSTEXPR14 const value_type operator *() const
```
Dereference operator

---

```cpp
ETL_CONSTEXPR14 TIterator operator ->()
```
-> operator

---

```cpp
ETL_CONSTEXPR14 const TIterator operator ->() const
```
-> operator

---

```cpp
ETL_CONSTEXPR14 operator TIterator() const
```
Conversion operator

---

```cpp
ETL_CONSTEXPR14 TIterator current() const
```
Conversion to the base iterator type

---

```cpp
ETL_CONSTEXPR14 circular_iterator& operator ++()
```
Increment  
Forward iterator  
Bidirectional iterator  
Random access iterator

---

```cpp
ETL_CONSTEXPR14 circular_iterator operator ++(int)
```
Increment  
Forward iterator  
Bidirectional iterator  
Random access iterator  

---

```cpp
ETL_CONSTEXPR14 circular_iterator& operator --()
``
Decrement  
Bidirectional iterator  
Random access iterator  

---

```cpp
ETL_CONSTEXPR14 circular_iterator operator --(int)
```
Decrement  
Bidirectional iterator  
Random access iterator  

---

```cpp
ETL_CONSTEXPR14 circular_iterator& operator +=(difference_type offset)
```
+= operator  
Random access iterator

---

```cpp
ETL_CONSTEXPR14 circular_iterator& operator -=(typename 
                                               etl::iterator_traits<TIterator>::difference_type 
                                               offset)
```
-= operator
Random access iterator

## Non-member functions
```cpp
template <typename TIterator>
ETL_CONSTEXPR14 
typename etl::iterator_traits<TIterator>::difference_type 
operator - (etl::circular_iterator<TIterator>& lhs, etl::circular_iterator<TIterator>& rhs)
```

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR14
bool operator ==(const etl::circular_iterator<TIterator>& lhs,
                 const etl::circular_iterator<TIterator>& rhs)
```

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR14 bool operator ==(const etl::circular_iterator<TIterator>& lhs,
                                 TIterator rhs)
```

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR14 bool operator ==(TIterator lhs,
                                 const etl::circular_iterator<TIterator>& rhs)
```

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR14 bool operator !=(const etl::circular_iterator<TIterator>& lhs,
                                 const etl::circular_iterator<TIterator>& rhs)
```

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR14 bool operator !=(const etl::circular_iterator<TIterator>& lhs,
                                 TIterator rhs)
```

---

```cpp
template <typename TIterator>
ETL_CONSTEXPR14 bool operator !=(TIterator& lhs,
                                 const etl::circular_iterator<TIterator>& rhs)
```

## Example
```cpp
std::list<int> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

// The start position to loop from.
std::list<int>::const_iterator> start = data.begin();
std::advance(start, 2);

// Create circular iterators.
etl::circular_iterator<std::list<int>::const_iterator>> cli(data.begin(), data.end(), start);
etl::circular_iterator<std::list<int>::const_reverse_iterator>> clri(data.rbegin(), data.rend);

// Loop 10 times, taking every third value, starting from the third element for the forward iterator and from the end for the reverse iterator.
for (int = 0; i < 10; ++i)
{
    std::cout << "(" << *cli << "," << *clrl << ")";
    std::advance(cli, 3);
    std::advance(clri, 3);
}

Prints the following
(2,9)(5,6)(8,3)(1,0)(4,7)(7,4)(0,1)(3,8)(6,5)(9,2)
```
