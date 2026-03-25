---
title: "array_wrapper"
---

---

{{< callout >}}
  Header: `array_wrapper.h`  
  Supported: All versions  
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
```C++
T& at(size_t i)
```
```C++
ETL_CONSTEXPR const T& at(size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.  
Emits an `etl::array_wrapper_bounds` if the index isout of range of the array.  
If asserts or exceptions are not enabled then undefined behaviour occurs.

---
```C++
T& operator[](size_t i)
```
```C++
ETL_CONSTEXPR const T& operator[](size_t i) const
```
**Description**  
Returns a reference or const reference to the indexed element.if the index is out of range of the array then undefined behaviour occurs.---

---

```C++
T& front()
```
```C++
ETL_CONSTEXPR const T& front() const
```
**Description**  
Returns a reference or const reference to the first element.---

---

```C++
T& back()
```
```C++
ETL_CONSTEXPR const T& back() const
```
**Description**  
Returns a reference or const reference to the last element.---

---

```C++
T* data()
```
```C++
ETL_CONSTEXPR const T* data() const
```
**Description**  
Returns a pointer or const pointer to the array.---Modifiers

---
```C++
void fill(parameter_t value)
```
**Description**  
Fill a non-cost array with the value.---

---

```C++
template <typename T, T(&ARRAYOTHER)[SIZE_]>
void swap(etl::array_wrapper<T, SIZE_, ARRAYOTHER>& other)
```
**Description**  
Swaps the array contents with another.---Iterators

---

```C++
ETL_CONSTEXPR iterator begin()
```
```C++
ETL_CONSTEXPR const_iterator begin() constETL_CONSTEXPR const_iterator cbegin() const
```
**Description**  
Returns an iterator to the beginning of the array.---

---

```C++
ETL_CONSTEXPR iterator end()
```
```C++
ETL_CONSTEXPR const_iterator end() constETL_CONSTEXPR const_iterator cend() const
```
**Description**  
Returns an iterator to the end of the array.---

---

```C++
ETL_CONSTEXPR iterator rbegin()
```
```C++
ETL_CONSTEXPR const_reverse_iterator rbegin() const
```
```C++
ETL_CONSTEXPR const_reverse_iterator crbegin() const
```
**Description**  
Returns a reverse iterator to the beginning of the array.---

---

```C++
ETL_CONSTEXPR iterator rend()
```
```C++
ETL_CONSTEXPR const_reverse_iterator rend() constETL_CONSTEXPR const_reverse_iterator crend() const
```
**Description**  
Returns a reverse iterator to the end of the array.---Capacity

---

```C++
ETL_CONSTEXPR size_t size() const
```
**Description**  
Returns the size of the view.---

---

```C++
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
```C++
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

```C++
ETL_ARRAY_WRAPPER(type, array);
```

**Example**  

```C++
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
