---
title: "Algorithms"
---

A set of algorithms from the STL.  

If `ETL_NO_STL` is defined then the ETL implements its own reverse engineered versions.  
Otherwise, the ETL will  often be a wrapper around the STL's implementation.

Functions returning pair will either use `std::pair` or, if `ETL_NO_STL` is defined, `etl::pair`.

**Note:**  
Some algorithms will be `constexpr`, dependent on the compiler support and ETL setup.

## swap
Only defined if `ETL_NO_STL` is defined.  

```cpp
template <typename T>
void swap(T& a, T& b) ETL_NOEXCEPT
```
**Description**  
Swaps two values.

---

```cpp
template <typename T, size_t N>
void swap(T(&a)[N], T(&b)[N]) ETL_NOEXCEPT
```
**Description**  
Swaps two arrays.  

## iter_swap

```cpp
template <typename TIterator1, typename TIterator2>
void iter_swap(TIterator1 a, TIterator2 b)
```
**Description**  
Swaps the elements pointed to by two iterators.  

## swap_ranges

```cpp
template <typename TIterator1, typename TIterator2>
TIterator2 swap_ranges(TIterator1 first1,
                       TIterator1 last1,
                       TIterator2 first2)
```
**Description**  
Swaps two ranges of elements.  

## copy

```cpp
template <typename TInputIterator, typename TOutputIterator>
TOutputIterator copy(TInputIterator sb, TInputIterator se, TOutputIterator db)
```
**Description**  
Copies the elements in the range, defined by `sb`, `se`, to another range beginning at `db`.  

**constexpr**  
This function is `constexpr` under the following conditions.  
-	The STL is in use and C++20 is supported.  
-	The iterator is non-pointer or the type is not trivially copyable and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, built-in `memcpy` is supported, and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, `ETL_FORCE_CONSTEXPR_ALGORITHMS` is defined as `1`, and C++14 is supported.

## copy_s
**ETL extension**  

```cpp
template <typename TInputIterator, typename TOutputIterator>
TOutputIterator copy_s(TInputIterator  i_begin, TInputIterator  i_end,
                       TOutputIterator o_begin, TOutputIterator o_end)
```
**Description**  
A safer four parameter version that will stop copying when either iterator reaches the end of its range.

**constexpr**  
This function is `constexpr` under the following conditions.  
-	The STL is in use and C++20 is supported.  
-	The iterator is non-pointer or the type is not trivially copyable and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, built-in `memcpy` is supported, and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, `ETL_FORCE_CONSTEXPR_ALGORITHMS` is defined as `1`, and C++14 is supported.

## copy_n

```cpp
template <typename TInputIterator, typename TSize, typename TOutputIterator>
TOutputIterator copy_n(TInputIterator begin, TSize n, TOutputIterator result)
```
**Description**  
Copies exactly `n` values from the range beginning at `begin` to the range beginning at `result`.

**constexpr**  
This function is `constexpr` under the following conditions.  
-	The STL is in use and C++20 is supported.  
-	The iterator is non-pointer or the type is not trivially copyable and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, built-in `memcpy` is supported, and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, `ETL_FORCE_CONSTEXPR_ALGORITHMS` is defined as `1`, and C++14 is supported.  

## copy_n_s
**ETL extension**  
```cpp
template <typename TInputIterator, typename TSize, typename TOutputIterator>
TOutputIterator copy_n_s(TInputIterator i_begin,
                         TSize n,
                         TOutputIterator o_begin, TOutputIterator o_end)
```
**Description**  
A safer version that will stop copying when either `n` items have been  copied or the output iterator reaches the end of its range.  

---

```cpp
template <typename TInputIterator, typename TSize1, typename TOutputIterator, typename TSize2>
TOutputIterator copy_n_s(TInputIterator i_begin,
                         TSize1 n1,
                         TOutputIterator o_begin,
                         TSize2 n2)
```
**Description**  
A safer version that will stop copying when either n1 or n2 items have been  copied.

**constexpr**  
This function is constexpr under the following conditions.  
-	The STL is in use and C++20 is supported.  
-	The iterator is non-pointer or the type is not trivially copyable and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, built-in `memcpy` is supported, and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, `ETL_FORCE_CONSTEXPR_ALGORITHMS` is defined as `1`, and C++14 is supported.  

## copy_if

```cpp
template <typename TIterator, typename TOutputIterator, typename TUnaryPredicate>
ETL_CONSTEXPR14
TOutputIterator copy_if(TIterator begin, TIterator end, TOutputIterator out, TUnaryPredicate predicate)
```
**Description**  
Only copies the elements for which the predicate predicate returns `true`.

## copy_if_s
**ETL extension**  
```cpp
template <typename TIterator, typename TOutputIterator, typename TUnaryPredicate>
ETL_CONSTEXPR14
TOutputIterator copy_if_s(TIterator i_begin,        TIterator i_end,
                          TOutputIterator o_begin,  TOutputIterator o_end,
                          TUnaryPredicate predicate)
```
**Description**  
A safer version that will stop copying when either iterator reaches the end of its range.

## copy_n_if
**ETL extension**  

```cpp
template <typename TIterator, typename TSize, typename TOutputIterator, typename TUnaryPredicate>
ETL_CONSTEXPR14
TOutputIterator copy_n_if(TIterator begin, TSize n, TOutputIterator out, TUnaryPredicate predicate)
```
**Description**  
A combination of `copy_if` and `copy_n`.

## reverse_copy
```cpp
template <typename TIterator1, typename TIterator2>
TIterator2 reverse_copy(TIterator1 sb, TIterator1 se, TIterator2 db)
```
**Description**  
Copies the elements from the range `sb`, `se` to another range beginning at `db` in such a way that the elements in the new range are in reverse order.

**constexpr**  
This function is `constexpr` under the following conditions.
-	The STL is in use and C++20 is supported.
-	The STL is not in use.

## copy_backward

```cpp
template <typename TIterator1, typename TIterator2>
TIterator2 copy_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
```
**Description**  
Copies the elements from the range, defined by `sb`, `se`  to another range ending at `db`.
The elements are copied in reverse order.

**constexpr**  
This function is constexpr under the following conditions.
-	The STL is in use and C++20 is supported.
-	The STL is not in use.

## count

```cpp
template <typename TIterator, typename T>
typename iterator_traits<TIterator>::difference_type count(TIterator first, 
                                                           TIterator last, 
                                                           const T& value)
```
**Description**  
Counts the elements that are equal to value.  

## count_if

```cpp
template <typename TIterator, typename TUnaryPredicate>
typename iterator_traits<TIterator>::difference_type count_if(TIterator first, 
                                                              TIterator last, 
                                                              TUnaryPredicate predicate)
```
**Description**  
Counts the elements where predicate returns `true`.

## equal
```cpp
template <typename TIterator1, typename TIterator2>
bool equal(TIterator1 first1, TIterator1 last1, TIterator2 first2)
```
**Description**  
Returns `true` if the range [`first1`, `last1`) is equal to the range []`first2`, `first2 + distance(first2, last2)`).

## lexicographical_compare
```cpp
template <typename TIterator1, typename TIterator2>
bool lexicographical_compare(TIterator1 first1, TIterator1 last1,
                             TIterator2 first2, TIterator2 last2)
```
**Description**  
Checks if the range [`first1`, `last1`) is lexicographically less than the range [`first2`, `last2`), using the 'less-than' operator.

---

```cpp
template <typename TIterator1, typename TIterator2, typename TCompare>
bool lexicographical_compare(TIterator1 first1, TIterator1 last1,
                             TIterator2 first2, TIterator2 last2,
                             TCompare compare)
```
**Description**  
Checks if the range [`first1`, `last1`) is lexicographically less than the range [`first2`, `last2`), using `compare`.
 
## make_heap
```cpp
template <typename TIterator>
void make_heap(TIterator first, TIterator last)
```
**Description**  
Makes a heap from the range [`first`, `last`).

---

```cpp
template <typename TIterator, typename TCompare>
void make_heap(TIterator first, TIterator last, TCompare compare)
```
**Description**  
Makes a heap from the range [`first`, `last`).  
Uses `compare` to compare elements.

## is_heap
```cpp
template <typename TIterator>
bool is_heap(TIterator first, TIterator last)
```
**Description**  
Returns `true` is the range [`first`, `last`) is a heap, otherwise `false`.

---

```cpp
template <typename TIterator, typename TCompare>
bool is_heap(TIterator first, TIterator last, TCompare compare)
```
**Description**  
Returns `true` is the range [`first`, `last`) is a heap, otherwise `false`.  
Uses `compare` to compare elements.

## pop_heap
```cpp
template <typename TIterator>
void pop_heap(TIterator first, TIterator last)
```
**Description**  
Removes the first element from the heap.

---

```cpp
template <typename TIterator, typename TCompare>
void pop_heap(TIterator first, TIterator last, TCompare compare)
```
**Description**  
Removes the first element from the heap.  
Uses `compare` to compare elements.

## push_heap
```cpp
template <typename TIterator>
void push_heap(TIterator first, TIterator last)
```
**Description**  
Inserts the element at the position `last - 1` into the heap.  

---

```cpp
template <typename TIterator, typename TCompare>
void push_heap(TIterator first, TIterator last, TCompare compare)
```
**Description**  
Inserts the element at the position `last - 1` into the heap.  
Uses `compare` to compare elements.

## min
```cpp
template <typename T>
ETL_CONSTEXPR const T& min(const T& a, const T& b)
```
**Description**  
Returns the minimum of the two values.

---

```cpp
template <typename T, typename TCompare>
ETL_CONSTEXPR const T& min(const T& a, const T& b, TCompare compare)
```
**Description**  
Returns the value where compare returns `true` if `a is less than `b`.

## max
```cpp
template <typename T>
ETL_CONSTEXPR const T& max(const T& a, const T& b)
```
**Description**  
Returns the maximum of the two values.

---

```cpp
template <typename T, typename TCompare>
ETL_CONSTEXPR const T& max(const T& a, const T& b, TCompare compare)
```
**Description**  
Returns the value where `compare` returns `true` if `a` is less than `b`.

## minmax
```cpp
template <typename T> 
pair<const T&, const T&> minmax(const T& a, const T& b)
```
**Description**  
Returns the lesser and greater values.

---

```cpp
template <typename T, typename TCompare> 
pair<const T&, const T&> minmax(const T& a, const T& b, TCompare compare)
```
**Description**  
Returns the lesser and greater values using `compare`.

## min_element
```cpp
template <typename TIterator>
TIterator min_element(TIterator begin, TIterator end)
```
**Description**  
Finds the smallest element in the range [`begin`, `end`).

---

```cpp
template <typename TIterator, typename TCompare>
TIterator min_element(TIterator begin, TIterator end, TCompare compare)
```
**Description**  
Finds the smallest element in the range [`begin`, `end`) using `compare`.

## max_element
```cpp
template <typename TIterator>
TIterator max_element(TIterator begin, TIterator end)
```
**Description**  
Finds the greatest element in the range [`begin`, `end`).

---

```cpp
template <typename TIterator, typename TCompare>
TIterator max_element(TIterator begin, TIterator end, TCompare compare)
```
**Description**  
Finds the greatest element in the range [`begin`, `end`) using `compare`.

## minmax_element
```cpp
template <typename TIterator>
pair<TIterator, TIterator> minmax_element(TIterator begin, TIterator end)
```
**Description**  
Finds the smallest and greatest element in the range [`begin`, `end`).

---

```cpp
template <typename TIterator, typename TCompare>
pair<TIterator, TIterator> minmax_element(TIterator begin, TIterator end, TCompare compare)
```
**Description**  
Finds the smallest and greatest element in the range [`begin`, `end`) using `compare`.

## multimin
**ETL extension**  
C++11 only  
Variadic functions to return the minimum value, or iterator to value, from a variable length parameter list.

```cpp
template <typename T, typename... Tx>
constexpr const T& multimin(const T& t, const Tx&... tx)
```
**Description**  
Returns the minimum of the supplied arguments.

---

```cpp
template <typename TCompare, typename T, typename... Tx>
constexpr const T& multimin_compare(TCompare compare, const T& t, const Tx&... tx)
```
**Description**  
Returns the minimum of the supplied arguments.
Uses `compare` to compare values.

---

```cpp
template <typename TIterator, typename... TIteratorx>
constexpr const TIterator& multimin_iter(const TIterator& t, const TIteratorx&... tx)
```
**Description**  
Returns the minimum of the dereferenced supplied arguments.

---

```cpp
template <typename TCompare, typename TIterator, typename... TIteratorx>
constexpr const TIterator& multimin_iter_compare(TCompare compare, 
                                                 const TIterator& t, const TIteratorx&... tx)
```
**Description**  
Returns the minimum of the dereferenced supplied arguments.
Uses `compare` to compare values.

### Example
```cpp
int minimum;

minimum = etl::multimin(1, 2, 3, 4, 5, 6, 7, 8));
minimum = etl::multimin_compare(std::less<int>(), 1, 2, 3, 4, 5, 6, 7, 8));
minimum = etl::multimin_compare(std::greater<int>(), 1, 2, 3, 4, 5, 6, 7, 8));

int i[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

int* p_minimum;

p_minimum = etl::multimin_iter(&i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
p_minimum = etl::multimin_iter_compare(std::less<int>(), 
                                       &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
p_minimum = etl::multimin_iter_compare(std::greater<int>(), 
                                       &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));

```

---

## multimax
**ETL extension**  
C++11 only  
Variadic functions to return the maximum value, or iterator to value, from a variable length parameter list.

```cpp
template <typename T, typename... Tx>
constexpr const T& multimax(const T& t, const Tx&... tx)
```
**Description**  
Returns the maximum of the supplied arguments.

---

```cpp
template <typename TCompare, typename T, typename... Tx>
constexpr const T& multimax_compare(TCompare compare, const T& t, const Tx&... tx)
```
**Description**  
Returns the maximum of the supplied arguments.  
Uses `compare` to compare values.

---

```cpp
template <typename TIterator, typename... TIteratorx>
constexpr const TIterator& multimax_iter(const TIterator& t, const TIteratorx&... tx)
```
**Description**  
Returns the maximum of the dereferenced supplied arguments.

---

```cpp
template <typename TCompare, typename TIterator, typename... TIteratorx>
constexpr const TIterator& multimax_iter_compare(TCompare             compare, 
                                                 const TIterator&     t, 
                                                 const TIteratorx&... tx)
```
**Description**  
Returns the maximum of the dereferenced supplied arguments.  
Uses `compare` to compare values.

### Example
```cpp
int maximum;

maximum = etl::multimax(1, 2, 3, 4, 5, 6, 7, 8));
maximum = etl::multimax_compare(std::less<int>(), 1, 2, 3, 4, 5, 6, 7, 8));
maximum = etl::multimax_compare(std::greater<int>(), 1, 2, 3, 4, 5, 6, 7, 8));

int i[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

int* p_maximum;

p_maximum = etl::multimax_iter(&i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
p_maximum = etl::multimax_iter_compare(std::less<int>(), 
                                       &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
p_maximum = etl::multimax_iter_compare(std::greater<int>(), 
                                       &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
```

## move

```cpp
template <typename TIterator1, typename TIterator2>
TIterator2 move(TIterator1 sb, TIterator1 se, TIterator2 db)
```
**Description**  
Moves the elements in the range `sb`, `se` to another range beginning at `db`.  

**constexpr**  
This function is constexpr under the following conditions.  
-	The STL is in use and C++20 is supported.  
-	The iterator is non-pointer or the type is not trivially copyable and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, built-in `memcpy` is supported, and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, `ETL_FORCE_CONSTEXPR_ALGORITHMS` is defined as 1, and C++14 is supported.  

## move_s
**ETL extension**  

```cpp
template <typename TInputIterator, typename TOutputIterator>
TOutputIterator move_s(TInputIterator  i_begin, TInputIterator  i_end,
                       TOutputIterator o_begin, TOutputIterator o_end)
```
**Description**  
A safer four parameter version that will stop moving when either iterator reaches the end of its range.

**constexpr**  
This function is constexpr under the following conditions.  
-	The STL is in use and C++20 is supported.  
-	The iterator is non-pointer or the type is not trivially copyable and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, built-in `memcpy` is supported, and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, `ETL_FORCE_CONSTEXPR_ALGORITHMS` is defined as `1`, and C++14 is supported.  

## move_backward

```cpp
template <typename TIterator1, typename TIterator2>
TIterator2 move_backward(TIterator1 sb, TIterator1 se, TIterator2 de)
```
**Description**  
Moves the elements in the range `sb`, `se` to another range ending at `de`.  
The elements are moved in reverse order.  

**constexpr**  
This function is constexpr under the following conditions.  
-	The STL is in use and C++20 is supported.  
-	The iterator is non-pointer or the type is not trivially copyable and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, built-in `memcpy` is supported, and C++14 is supported.  
-	The iterator is a pointer, the type is trivially copyable, `ETL_FORCE_CONSTEXPR_ALGORITHMS` is defined as 1, and C++14 is supported.  

## reverse

```cpp
template <typename TIterator>
void reverse(TIterator b, TIterator e)
```
**Description**  
Reverses the order of the elements in the range `b`, `e`.  

## for_each_if
**ETL extension**  

```cpp
template <typename TIterator, typename TUnaryFunction, typename TUnaryPredicate>
TUnaryFunction for_each_if(TIterator       begin,
                           const TIterator end,
                           TUnaryFunction  function,
                           TUnaryPredicate predicate)
```
**Description**  
Applies function to each element if `predicate` returns `true`.

## for_each_n
**ETL extension**  

```cpp
template <typename TIterator, typename TSize, typename TUnaryFunction>
TIterator for_each_n(TIterator       begin,
                     TSize           n,
                     TUnaryFunction  function)
```
**Description**  
Applies function to `n` elements, starting from `begin`.

## for_each_n_if
**ETL extension**  

```cpp
template <typename TIterator, typename TSize, typename TUnaryFunction, typename TUnaryPredicate>
TIterator for_each_n_if(TIterator       begin,
                        TSize           n,
                        TUnaryFunction  function,
                        TUnaryPredicate predicate)
```
**Description**  
Combination of `for_each_if` and `for_each_n`.

## fill

```cpp
template <typename TIterator, typename TValue>
void fill(TIterator first, TIterator last, const TValue& value)
```
**Description**  
Assigns the given `value` to the elements in the range `first`, `last`.  

## fill_n

```cpp
template <typename TIterator, typename TSize, typename TValue>
TIterator fill_n(TIterator first, TSize count, const TValue& value)
```
**Description**  
Assigns the given `value` to the first `count` elements in the range beginning at `first`.  

## find

```cpp
template <typename TIterator, typename T>
TIterator find(TIterator first, TIterator last, const T& value)
```
**Description**  
Searches for an element equal to `value`.

## find_end

```cpp
template <typename TIterator1, typename TIterator2>
TIterator1 find_end(TIterator1 b, TIterator1 e,
                    TIterator2 sb, TIterator2 se)
```
**Description**  
Searches for the last occurrence of the sequence `b`, `e` in the range `sb`, `be`.

---

```cpp
template <typename TIterator1, typename TIterator2, typename TPredicate>
TIterator1 find_end(TIterator1 b, TIterator1 e,
                    TIterator2 sb, TIterator2 se,
                    TPredicate predicate)
```
**Description**  
Searches for the last occurrence of the sequence `b`, `e` in the range `sb`, `be`, using `predicate`.

## find_if

```cpp
template <typename TIterator, typename TUnaryPredicate>
TIterator find_if(TIterator first, TIterator last, TUnaryPredicate predicate)
```
**Description**  
Searches for the first element that satisfies the `predicate`.  

## find_if_not

```cpp
template <typename TIterator, typename TUnaryPredicate>
TIterator find_if_not(TIterator begin, TIterator end, TUnaryPredicate predicate)
```
**Description**  
Searches for the first element that does not satisfy the `predicate`.  

## lower_bound

```cpp
template <typename TIterator, typename TValue>
TIterator lower_bound(TIterator first, TIterator last, const TValue& value)
```
**Description**  
Searches for the first element in the range [`first`, `last`) which is **not* ordered before `value`.

**Return**  
An iterator to the element, or `last` if not found.

---

```cpp
template <typename TIterator, typename TValue, typename TCompare>
TIterator lower_bound(TIterator first, TIterator last, const TValue& value, TCompare compare)
```
**Description**  
Searches for the first element in the range [`first`, `last`) which is **not* ordered before `value`.  
Uses `compare` to compare elements.


**Return**  
An iterator to the element, or `last` if not found.

## upper_bound

```cpp
template <typename TIterator, typename TValue>
TIterator upper_bound(TIterator first, TIterator last, const TValue& value)
```
**Description**  
Searches for the first element in the range [`first`, `last`) which is ordered after `value`.

**Return**  
An iterator to the element, or `last` if not found.

---

```cpp
template <typename TIterator, typename TValue, typename TCompare>
TIterator upper_bound(TIterator first, TIterator last, const TValue& value, TCompare compare)
```
**Description**  
Searches for the first element in the range [`first`, `last`) which is ordered after `value`.  
Uses `compare` to compare elements.

**Return**  
An iterator to the element, or `last` if not found.

## equal_range

```cpp
template <typename TIterator, typename TValue>
pair<TIterator, TIterator> equal_range(TIterator first, TIterator last, const TValue& value)
```
**Description**  
Returns a range containing all elements equivalent to `value` in the range [`first`, `last`).

**Return**  
A `pair` containing a pair of iterators.

`first` is is the result of `lower_bound(first, last)`.
`last` is is the result of `upper_bound(first, last)`.

---

```cpp
template <typename TIterator, typename TValue, typename TCompare>
pair<TIterator, TIterator> equal_range(TIterator first, TIterator last, 
                                       const TValue& value, TCompare compare)
```
**Description**  
Returns a range containing all elements equivalent to `value` in the range [`first`, `last`).  
Uses compare to `compare` elements.


**Return**  
A `pair` containing a pair of iterators.

`first` is is the result of `lower_bound(first, last)`.
`last` is is the result of `upper_bound(first, last)`.

## binary_find
**ETL extension**  

```cpp
template <typename TIterator, typename TValue>
TIterator binary_find(TIterator     begin,
                      TIterator     end,
                      const TValue& value)
```
**Description**  
Finds the first item in a sorted container that matches the `value`.  
Returns an iterator to the `value` or `end`.

---

```cpp
template <typename TIterator, typename TValue, typename TBinaryPredicate, typename TBinaryEquality>
TIterator binary_find(TIterator        begin,
                      TIterator        end,
                      const TValue&    value,
                      TBinaryPredicate predicate,
                      TBinaryEquality  equality)
```
**Description**  
Finds the first item in a sorted container that matches the value.  
Returns an iterator to the `value` or `end`.  
Comparison and equality predicates are supplied.

---

## search

```cpp
template <typename TIterator1, typename TIterator2>
TIterator1 search(TIterator1 first, TIterator1 last, 
                  TIterator2 search_first, TIterator2 search_last)
```
**Description**  
Searches for the first occurrence of the sequence of elements `first`, `last` in the range `search_first`, `search_last`.

---

```cpp
template <typename TIterator1, typename TIterator2, typename TCompare>
TIterator1 search(TIterator1 first, TIterator1 last, 
                  TIterator2 search_first, TIterator2 search_last,
                  TCompare compare)
```
**Description**  
Searches for the first occurrence of the sequence of elements `first`, `last` in the range `search_first`, `search_last`  
Uses `compare` to check equality of the values.  

## binary_search

```cpp
template <typename TIterator, typename T>
bool binary_search(TIterator first, TIterator last, const T& value)
```
**Description**  
Checks if an element equivalent to value appears within the range [`first`, `last`).  

```cpp
template <typename TIterator, typename T, typename Compare>
bool binary_search(TIterator first, TIterator last, const T& value, Compare compare)
```
**Description**  
Checks if an element equivalent to value appears within the range [`first`, `last`).
Uses `compare` to check equality of the values.

## all_of

```cpp
template <typename TIterator, typename TUnaryPredicate>
bool all_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
```
**Description**  
Checks if `predicate` returns `true` for all elements in the range [`begin`, `end`).

## any_of

```cpp
template <typename TIterator, typename TUnaryPredicate>
bool any_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
```
**Description**  
Checks if `predicate` returns `true` for at least one element in the range [`begin`, `end`).

## none_of

```cpp
template <typename TIterator, typename TUnaryPredicate>
bool none_of(TIterator begin, TIterator end, TUnaryPredicate predicate)
```
**Description**  
Checks if `predicate` returns `true` for no elements in the range [`begin`, `end`).

## next_permutation
```cpp
template <typename TIterator, typename TCompare>
ETL_CONSTEXPR14 bool next_permutation(TIterator first, TIterator last, TCompare compare)
```
**Description**  
Creates the range [`first`, `last`) into the next permutation.  
`compare` should return `true` if its first argument is logically less that its second.

**Complexity**  
O(N)  
At most N/2 swaps.

**Return**  
`true` if there is a next permutation.

## is_permutation

```cpp
template <typename TIterator1, typename TIterator2>
bool is_permutation(TIterator1 begin1, 
                    TIterator1 end1, 
                    TIterator2 begin2)
```
**Description**  
Returns `true` if there exists a permutation of the elements in the range [`begin1`, `end1`) that makes that range equal to the second range.

---

```cpp
template <typename TIterator1, typename TIterator2>
bool is_permutation(TIterator1 begin1, 
                    TIterator1 end1, 
                    TIterator2 begin2, 
                    TIterator2 end2)
```
**Description**  
Returns `true` if there exists a permutation of the elements in the range [`begin1`, `end1`) that makes that range equal to the second range.

---

```cpp
template <typename TIterator1, 
          typename TIterator2, 
          typename TBinaryPredicate>
bool is_permutation(TIterator1       begin1, 
                    TIterator1       end1, 
                    TIterator2       begin2, 
                    TBinaryPredicate predicate)
```
**Description**  
Returns `true` if there exists a permutation of the elements in the range [`begin1`, `end1`) that makes that range equal to the second range.  
Uses `predicate` to checks elements in the range.

---

```cpp
template <typename TIterator1, 
          typename TIterator2, 
          typename TBinaryPredicate>
bool is_permutation(TIterator1       begin1, 
                    TIterator1       end1, 
                    TIterator2       begin2, 
                    TIterator2       end2, 
                    TBinaryPredicate predicate)
```
**Description**  
Returns `true` if there exists a permutation of the elements in the range [`begin1`, `end1`) that makes that range equal to the second range.  
Uses `predicate` to checks elements in the range.

## partition
```cpp
ETL_CONSTEXPR14 TIterator partition(TIterator first, TIterator last, TPredicate predicate)
```
**Description**  
Reorders elements in a range based on a predicate.  
It moves all elements that satisfy the predicate to the front, and those that do not to the back.  
Does not guarantee to keep the original relative order.  

**Complexity**  
O(N)  

**Return**  
An iterator to the start of the group that do not satisfy the predicate.

## stable_partition

### In-place
```cpp
template <typename TIterator, typename TPredicate>
ETL_CONSTEXPR14 TIterator stable_partition(TIterator first, 
                                           TIterator last, 
                                           TPredicate predicate)
```
**Description**  
Reorders elements in a range based on a predicate.  
It moves all elements that satisfy the predicate to the front, and those that do not to the back.  
Keeps the original relative order.  
In-place  

**Complexity**  
Recursive function.
O(NlogN) time.  
O(NlogN) space.

**Return**  
An iterator to the start of the group that do not satisfy the predicate.

### External buffer
```cpp
template <typename TIterator, typename TPredicate>
ETL_CONSTEXPR14 TIterator stable_partition(TIterator first, 
                                           TIterator last, 
                                           TIterator buffer_first, 
                                           TIterator buffer_last, 
                                           TPredicate predicate)
```
**Description**  
Reorders elements in a range based on a predicate.  
It moves all elements that satisfy the predicate to the front, and those that do not to the back.  
Keeps the original relative order.  

Requires a user supplied buffer that must be at least the same size as the range [`first`, `last`).  

**Complexity**  
Non-recursive function.
O(N) time.  
O(N) space.  

Raises an `etl::stable_partition_buffer_too_small` is the buffer is not large enough to accommodate the range [`first`, `last`).

**Return**  
An iterator to the start of the group that do not satisfy the predicate.

## is_partitioned

```cpp
template <typename TIterator, typename TUnaryPredicate>
bool is_partitioned(TIterator begin, 
                    TIterator end, 
                    TUnaryPredicate predicate)
```
**Description**  
Returns `true` if all elements in the range  [`begin`, `end`) that satisfy the predicate appear before all elements that don't. Also returns `true` if [`begin`, `end`) is empty.

## partition_point
```cpp
template <typename TIterator, typename TUnaryPredicate>
TIterator partition_point(TIterator begin, 
                          TIterator end,  
                          TUnaryPredicate predicate)
```
**Description**  
Examines the partitioned range [`begin`, `end`) and locates the end of the first partition according to `predicate`.

## partition_copy

```cpp
template <typename TSource, 
          typename TDestinationTrue, 
          typename TDestinationFalse, 
          typename TUnaryPredicate>
pair<TDestinationTrue, TDestinationFalse> partition_copy(TSource           begin,
                                                         TSource           end, 
                                                         TDestinationTrue  destination_true,
                                                         TDestinationFalse destination_false,
                                                         TUnaryPredicate   predicate)
```
**Description**  
Copies the elements from the range [`begin`, `end`) to two different ranges depending on the value returned by the `predicate`.  
The elements that satisfy `predicate` are copied to the range beginning at `destination_true`.  
The rest of the elements are copied to the range beginning at `destination_false`.  
This partition is stable.

**Complexity**  
O(N)  

## partition_move

```cpp
template <typename TSource, 
          typename TDestinationTrue, 
          typename TDestinationFalse, 
          typename TUnaryPredicate>
pair<TDestinationTrue, TDestinationFalse> partition_move(TSource           begin,
                                                         TSource           end, 
                                                         TDestinationTrue  destination_true,
                                                         TDestinationFalse destination_false,
                                                         TUnaryPredicate   predicate)
```
**Description**  
Moves the elements from the range [`begin`, `end`) to two different ranges depending on the value returned by the `predicate`.  
The elements that satisfy `predicate` are moved to the range beginning at `destination_true`.  
The rest of the elements are moved to the range beginning at `destination_false`.  
This partition is stable.

**Complexity**  
O(N)  

## partition_transform
**ETL extension**  

### One input range
```cpp
template <typename TSource, 
          typename TDestinationTrue, 
          typename TDestinationFalse,
          typename TUnaryFunctionTrue, 
          typename TUnaryFunctionFalse, 
          typename TUnaryPredicate>
pair<TDestinationTrue, TDestinationFalse>
  partition_transform(TSource             begin,
                      TSource             end,
                      TDestinationTrue    destination_true,
                      TDestinationFalse   destination_false,
                      TUnaryFunctionTrue  function_true,
                      TUnaryFunctionFalse function_false,
                      TUnaryPredicate     predicate)
```
**Description**  
Transforms and copies data from the source to one of two destinations.  
If `predicate` returns `true` then the source data is modified by `function_true` and stored in `destination_true`.  
If `predicate` returns `false` then the source data is modified by `function_false` and stored in `destination_false`.  
This partition is stable.

---

### Two input ranges
```cpp
template <typename TSource1,
          typename TSource2, 
          typename TDestinationTrue, 
          typename TDestinationFalse,
          typename TUnaryFunctionTrue, 
          typename TUnaryFunctionFalse, 
          typename TUnaryPredicate>
pair<TDestinationTrue, TDestinationFalse>
  partition_transform(TSource1             begin1,
                      TSource1             end1,
                      TSource2             begin2,
                      TDestinationTrue     destination_true,
                      TDestinationFalse    destination_false,
                      TBinaryFunctionTrue  function_true,
                      TBinaryFunctionFalse function_false,
                      TBinaryPredicate     predicate)
```
**Description**  
Transforms and copies data from the source to one of two destinations.  
If `predicate` returns `true` then the source data is modified by `function_true` and stored in `destination_true`.  
If `predicate` returns `false` then the source data is modified by `function_false` and stored in `destination_false`.  
This partition is stable.

## rotate

```cpp
template <typename TIterator>
TIterator rotate(TIterator first, TIterator middle, TIterator last)
```
**Description**  
Performs a left rotation on the range of elements.  

## transform_s
**ETL extension**  

```cpp
template <typename TIterator, 
          typename TOutputIterator, 
          typename TUnaryPredicate>
void transform_s(TIterator       i_begin,        
                 TIterator       i_end,
                 TOutputIterator o_begin,  
                 TOutputIterator o_end,
                 TUnaryFunction  function)
```
**Description**  
A safer version that will stop transforming when either iterator reaches the end of its range.  

## transform_n
**ETL extension**  

### One input range
```cpp
template <typename TInputIterator, 
          typename TSize, 
          typename TOutputIterator, 
          typename TUnaryFunction>
void transform_n(TInputIterator  i_begin,
                 TSize           n,
                 TOutputIterator o_begin,
                 TUnaryFunction  function)
```
**Description**  
Transform over `n` elements.

---

### Two input ranges
```cpp
template <typename TInputIterator1,
          typename TInputIterator2, 
          typename TSize, 
          typename TOutputIterator, 
          typename TBinaryFunction>
void transform_n(TInputIterator1 i_begin1,
                 TInputIterator2 i_begin2,
                 TSize           n,
                 TOutputIterator o_begin,
                 TBinaryFunction function)
```
**Description**  
Transform over `n` elements.

## transform_if
**ETL extension**  

### One input range
```cpp
template <typename TInputIterator, 
          typename TOutputIterator, 
          typename TUnaryFunction, 
          typename TUnaryPredicate>
TOutputIterator transform_if(TInputIterator  i_begin,  
                             TInputIterator  i_end,
                             TOutputIterator o_begin, 
                             TUnaryFunction  function,
                             TUnaryPredicate predicate)
```
**Description**  
Transforms data from the source based on a predicate.  
If `predicate` returns `true` then the source data is modified by `function` and stored in the output range.  
If `predicate` returns `false` then the source data is ignored.  

---

### Two input ranges
```cpp
template <typename TInputIterator1, 
          typename TInputIterator2,
          typename TOutputIterator, 
          typename TBinaryFunction, 
          typename TBinaryPredicate>
TOutputIterator transform_if(TInputIterator1  i_begin1,
                             TInputIterator1  i_end1,
                             TInputIterator2  i_begin2, 
                             TOutputIterator  o_begin,
                             TBinaryFunction  function,
                             TBinaryPredicate predicate)
```
**Description**  
Transforms data from the source based on a predicate.  
If `predicate` returns `true` then the source data is modified by `function` and stored in the output range.  
If `predicate` returns `false` then the source data is ignored.  

## transform_n_if
**ETL extension**  

### One input range
```cpp
template <typename TInputIterator,
          typename TSize,
          typename TOutputIterator, 
          typename TUnaryFunction, 
          typename TUnaryPredicate>
TOutputIterator transform_if(TInputIterator  i_begin,  
                             TSize           n,
                             TOutputIterator o_begin, 
                             TUnaryFunction  function,
                             TUnaryPredicate predicate)
```
**Description**  
Transforms data from the source based on a predicate for `n` items.  
If `predicate` returns `true` then the source data is modified by `function` and stored in the output range.  
If `predicate` returns `false` then the source data is ignored.

---

### Two input ranges
```cpp
template <typename TInputIterator1, 
          typename TInputIterator2,
          typename TSize,
          typename TOutputIterator, 
          typename TBinaryFunction, 
          typename TBinaryPredicate>
TOutputIterator transform_if(TInputIterator1  i_begin1,
                             TSize            n,
                             TInputIterator2  i_begin2, 
                             TOutputIterator  o_begin,
                             TBinaryFunction  function,
                             TBinaryPredicate predicate)
```
**Description**  
Transforms data from the source based on a predicate for `n` items.  
If `predicate` returns `true` then the source data is modified by `function` and stored in the output range.  
If `predicate` returns `false` then the source data is ignored.

## shell_sort
**ETL extension**  
https://en.wikipedia.org/wiki/Shellsort

```cpp
template <typename TIterator>
void shell_sort(TIterator first, TIterator last)
```
**Description**  
Sorts a range using the shell sort algorithm.

---

```cpp
template <typename TIterator, typename TCompare>
void shell_sort(TIterator first, TIterator last, TCompare compare)
```
**Description**  
Sorts a range using the shell sort algorithm, using `compare`.

## insertion_sort
**ETL extension**  
https://en.wikipedia.org/wiki/Insertion_sort

```cpp
template <typename TIterator>
void insertion_sort(TIterator first, TIterator last)
```
**Description**  
Sorts a range using the insertion sort algorithm.  

---

```cpp
template <typename TIterator, typename TCompare>
void insertion_sort(TIterator first, TIterator last, TCompare compare)
```
**Description**  
Sorts a range using the insertion sort algorithm.

## heap_sort
**ETL extension**  
https://en.wikipedia.org/wiki/Heapsort

```cpp
template <typename TIterator>
void heap_sort(TIterator first, TIterator last)
```
Sorts a range using the heap sort algorithm.  
Since: `16.2.0`

---

```cpp
template <typename TIterator, typename TCompare>
void heap_sort(TIterator first, TIterator last, TCompare compare)
```
**Description**
Sorts a range using the heap sort algorithm, using `compare`.
Since: `16.2.0`  

## selection_sort
**ETL extension**  
https://en.wikipedia.org/wiki/Selectionsort


```cpp
template <typename TIterator>
void selection_sort(TIterator first, TIterator last)
```
**Description**  
Sorts a range using the selection sort algorithm.  
From: `20.7.0`

---

```cpp
template <typename TIterator, typename TCompare>
void selection_sort(TIterator first, TIterator last, TCompare compare)
```
**Description**  
Sorts a range using the selection sort algorithm.  
From: `20.7.0`

## sort

```cpp
template <typename TIterator>
void sort(TIterator first, TIterator last)
```
**Description**  
If `ETL_NO_STL` is defined then uses `etl::shell_sort`, otherwise calls `std::sort`.

---

```cpp
template <typename TIterator, typename TCompare>
void sort(TIterator first, TIterator last, TCompare compare)
```
**Description**  
If `ETL_NO_STL` is defined then uses `etl::shell_sort`, otherwise calls `std::sort`.

## stable_sort

```cpp
template <typename TIterator>
void stable_sort(TIterator first, TIterator last)
```
**Description**  
If `ETL_NO_STL` is defined then uses `etl::insertion_sort`, otherwise calls `std::stable_sort`.

---

```cpp
template <typename TIterator, typename TCompare>
void stable_sort(TIterator first, TIterator last, TCompare compare)
```
**Description**  
If `ETL_NO_STL` is defined then uses `etl::insertion_sort`, otherwise calls `std::stable_sort`.

## is_sorted

```cpp
template <typename TIterator>
bool is_sorted(TIterator begin, TIterator end)
```
**Description**  
Returns true is the range is sorted.

---

```cpp
template <typename TIterator, typename TCompare>
bool is_sorted(TIterator begin, TIterator end, TCompare compare)
```
**Description**  
Returns true is the range is sorted.

## is_sorted_until

```cpp
template <typename TIterator>
TIterator is_sorted_until(TIterator begin, TIterator end)
```
**Description**  
Returns an iterator to the first element that is not sorted

---

```cpp
template <typename TIterator, typename TCompare>
TIterator is_sorted_until(TIterator begin, TIterator end, TCompare compare)
```
**Description**  
Returns an iterator to the first element that is not sorted

## clamp

```cpp
template <typename T>
ETL_CONSTEXPR const T& clamp(const T& value, const T& low, const T& high )
```
**Description**  
Clamps `value` between two limits.

---

```cpp
template <typename T, typename TCompare>
ETL_CONSTEXPR const T& clamp(const T& value, const T& low, const T& high, TCompare compare)
```
**Description**  
Clamps `value` between two limits, using `compare`.

## accumulate

```cpp
template <typename TIterator, typename T>
ETL_CONSTEXPR14 T accumulate(TIterator first, TIterator last, T sum)
```
Computes the sum of the given `sum` and the elements in the range `first`, `last`.

```cpp
template <typename TIterator, typename T, typename TBinaryOperation>
ETL_CONSTEXPR14 T accumulate(TIterator first, TIterator last, T init, TBinaryOperation operation)
```
Computes the sum of the given `init` and the elements in the range `first`, `last`.
Uses `operation` to compute the final value.

## remove

```cpp
template <typename TIterator, typename T>
TIterator remove(TIterator first, TIterator last, const T& value)
```
**Description**  
Removes all elements that are equal to value.

## remove_if

```cpp
template <typename TIterator, typename TUnaryPredicate>
TIterator remove_if(TIterator first, TIterator last, TPredicate predicate)
```
**Description**  
Removes all elements that satisfy `predicate`.
