---
title: "standard_deviation"
---

{{< callout type="info">}}
  Header: `standard_deviation.h`  
  Since: `20.9.0`  
{{< /callout >}}

```cpp
template <bool Standard_Deviation_Type, typename TInput, typename TCalc = TInput>
class standard_deviation  : public etl::unary_function<TInput, void> 
```

`Standard_Deviation_Type` `Population` or `Sample`.  
`TInput`                  The input data type.  
`TCalc`                  The type to use for internal calculations. By default, equal to `TInput`.  

**Enumeration**  
```cpp
standard_deviation_type
etl::standard_deviation_type::Sample     
etl::standard_deviation_type::Population 
```

---

```cp
standard_deviation()
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
standard_deviation(TIterator first, TIterator last)
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
double get_standard_deviation() const
```
**Return**  
The calculated standard deviation for the data.

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
The calculated standard deviation for the data.

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
Clear the standard deviation.

## Example
```cpp
std::array<char, 10> input
{
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

etl::standard_deviation<etl::standard_deviation_type::Population, char, int32_t> 
    standard_deviation(input.begin(), input.end());

double standard_deviation_result;
double variance_result;

standard_deviation_result = standard_deviation; // standard_deviation_result  == 2.87
variance_result = standard_deviation.get_variance(); // variance_result  == 8.25
```
