---
title: "Covariance"
---

{{< callout type="info">}}
  Header: `covariance.h`  
  Since: `20.9.0`  
{{< /callout >}}

```cpp
template <bool Covariance_Type, typename TInput, typename TCalc = TInput>
class covariance : public etl::binary_function<TInput, TInput, void>
```

`Covariance_Type` `Population` or `Sample`.  
`TInput`          The input data type.  
`TCalc`           The type to use for internal calculations. By default, equal to `TInput`.


## covariance_type  
`etl::covariance_type::Sample`  
`etl::covariance_type::Population`  

## Constructor
```cpp
covariance()
```
Default constructor.

---

```cpp
template <typename TIterator>
covariance(TIterator first1, TIterator last1, TIterator first2)
```
**Description**  
Construct from two iterator ranges.

## Member functions

```cpp
void add(TInput value1, TInput value2)
```
**Description**  
Add a pair of values.

```cpp
template <typename TIterator>
void add(TIterator first1, TIterator last1, TIterator first2)
```
**Description**  
Add a range of values.

---

```cpp
void operator()(TInput value1, TInput value2)
```
**Description**  
Add a pair of values.

---

```cpp
template <typename TIterator>
void operator()(TIterator first1, TIterator last1, TIterator first2)
```
**Description**  
Add a range of values.

---

```cpp
double get_covariance() const
```
**Return**  
The calculated covariance for the data.

---

```cpp
operator double() const
```
**Return**  
The calculated covariance for the data.

---

```cpp
size_t count() const
```
**Description**  
Get the total number added entries.

```cpp
void clear()
```
**Description**  
Clear the covariance.

## Example

```cpp
std::array<char, 10> input_c
{
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

std::array<char, 10> input_c_inv
{
  0, -1, -2, -3, -4, -5, -6, -7, -8, -9
};

etl::covariance<etl::covariance_type::Population, char, int32_t> covariance(input_c.begin(),     
                                                                            input_c.end(), 
                                                                            input_c_inv.begin());

double covariance_result;

covariance_result  = covariance; // covariance_result  == -8.25
```
