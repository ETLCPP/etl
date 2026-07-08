---
title: "variance"
---

```cpp
template <bool Variance_Type, typename TInput, typename TCalc = TInput>
class variance : public etl::unary_function<TInput, void> 
```

`Variance_Type` `Population` or `Sample`.  
`TInput`        The input data type.  
`TCalc`         The type to use for internal calculations. By default, equal to `TInput`.

**Enumeration**  
```cpp
variance_type
etl::variance_type::Sample     
etl::variance_type::Population 
```

---

```cpp
variance()
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
variance(TIterator first, TIterator last)
```
**Description**  
Construct from an iterator range.

---

```cpp
void add(TInput value1)
```
**Description**  
Add a value.

---

```cpp
template <typename TIterator>
void add(TIterator first, TIterator last)
```
**Description**  
Add a range of values.

---

```cpp
void operator()(TInput value)
```
**Description**  
Add a values.

---

```cpp
template <typename TIterator>
void operator()(TIterator first, TIterator last)
```
**Description**  
Add a range of values.

---

```cpp
double get_variance() const
```
**Return**  
The calculated variance for the data.

---

```cpp
operator double() const
```
**Return**  
The calculated variance for the data.

---

```cpp
size_t count() const
```
**Description**  
Get the total number added entries.

---

```cpp
void clear()
```
**Description**  
Clear the variance.

---

## Example
```cpp
std::array<char, 10> input
{
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

etl::variance<etl::variance_type::Population, char, int32_t> variance(input.begin(),
                                                                      input.end());

double variance_result;

variance_result = variance; // variance_result  == 8.25
```
