---
title: "array_wrapper"
---

{{< callout >}}
  Header: `array_wrapper.h`  
  Since: All versions  
{{< /callout >}}

This class implements a zero cost C++ interface around a const or non-const C array. It presents the same API as std::array.
It allows legacy C data to be more easily integrated with C++ interfaces and be accessed in a less error prone fashion.
If C++11 or above is supported then most functions can be 'constexpr'.

---

## Member types

`value_type`  
`size_type`  
`difference_type`  
`reference`  
`value_type`  
`reference`  
`const_reference`  
`pointer`  
`const_pointer`  
`iterator`  
`const_iterator`  
`reverse_iterator`  
`const_reverse_iterator`  

## Member constants
**May be used as indexes in to the array.**  

`SIZE     =` Size of the array  
`MAX_SIZE = SIZE`  
`FRONT    = 0`  
`BACK     = SIZE - 1`  
`BEGIN    = 0`  
`END      = SIZE`  
`RBEGIN   = SIZE - 1`  
`REND     = -1`  

## Element access
```cpp
T& at(size_t i)
```
```cpp
ETL_CONSTEXPR const T& at(size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Emits an `etl::array_wrapper_bounds` if the index isout of range of the array.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---
```cpp
T& operator[](size_t i)
```
```cpp
ETL_CONSTEXPR const T& operator[](size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.if the index is out of range of the array then undefined behaviour occurs.---

---

```cpp
T& front()
```
```cpp
ETL_CONSTEXPR const T& front() const
```
**Description**  
Returns a reference or const reference to the first element.---

---

```cpp
T& back()
```
```cpp
ETL_CONSTEXPR const T& back() const
```
**Description**  
Returns a reference or const reference to the last element.---

---

```cpp
T* data()
```
```cpp
ETL_CONSTEXPR const T* data() const
```
**Description**  
Returns a pointer or const pointer to the array.---Modifiers

---
```cpp
void fill(parameter_t value)
```
**Description**  
Fill a non-cost array with the value.---

---

```cpp
template <typename T, T(&ARRAYOTHER)[SIZE_]>
void swap(etl::array_wrapper<T, SIZE_, ARRAYOTHER>& other)
```
**Description**  
Swaps the array contents with another.---Iterators

---

```cpp
ETL_CONSTEXPR iterator begin()
```
```cpp
ETL_CONSTEXPR const_iterator begin() constETL_CONSTEXPR const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the array.---

---

```cpp
ETL_CONSTEXPR iterator end()
```
```cpp
ETL_CONSTEXPR const_iterator end() constETL_CONSTEXPR const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the array.---

---

```cpp
ETL_CONSTEXPR iterator rbegin()
```
```cpp
ETL_CONSTEXPR const_reverse_iterator rbegin() const
```
```cpp
ETL_CONSTEXPR const_reverse_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the array.---

---

```cpp
ETL_CONSTEXPR iterator rend()
```
```cpp
ETL_CONSTEXPR const_reverse_iterator rend() constETL_CONSTEXPR const_reverse_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the array.---Capacity

---

```cpp
ETL_CONSTEXPR size_t size() const
```
**Description**  
Returns the size of the view.---

---

```cpp
ETL_CONSTEXPR size_t max_size() const
```
**Description**  
Returns the maximum possible size of the view.---Non-member functions

## Lexicographically comparisons

`operator ==`  
**Returns**  
`true` if the contents of the array views are equal, otherwise `false`.

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
contents of the rhs, otherwise `false`

## Swap
```cpp
template <typename T, std::size_t SIZE, T(&ARRAYL)[SIZE], T(&ARRAYR)[SIZE]>
void swap(etl::array_wrapper<T, SIZE, ARRAYL>& lhs,          
          etl::array_wrapper<T, SIZE, ARRAYR>& rhs)
```
**Description**            
Swaps the array contents of two arrays.

## Hash
There is a specialisation for `etl::hash` for `array_wrapper`.

## Macros
A macro is defined to ease the use of the class.

```cpp
ETL_ARRAY_WRAPPER(type, array);
```

**Example**  

```cpp
// The address of the array MUST be deducible at compile time.  
int cdata = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };  

typedef ETL_ARRAY_WRAPPER(int, cdata) Data;  

// All instances of 'Data' access the SAME array.Data data;  
void Print(Data data)  
{  
    Data::const_iterator itr = data.begin();  
    
    while (itr != data.end())  
    {  
        std::cout << *itr++ << " ";  
    }
}  

Print(data); // Prints "1 2 3 4 5 6 7 8 9 10"  

size_t hashvalue = etl::hash<Data>()(data);  

// Using indexes  
int firstElement = data[Data::FRONT];  

int lastElement = data[Data::BACK];  

// Using forward indexes  
for (int i = Data::BEGIN; i < Data::END; ++i)  
{  
  std::cout << data[i] << " ";  
}

// Using reverse indexes  
for (int i = Data::RBEGIN; i < Data::REND; --i)  
{  
  std::cout << data[i] << " ";  
}  
```
