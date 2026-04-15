---
title: "Correlation"
---

{{< callout type="info">}}
  Header: `correlation.h`  
  Since: `20.9.0`  
{{< /callout >}}

Calculates the correlation of two sets of data.

```cpp
template <bool Correlation_Type, typename TInput, typename TCalc = TInput>
class correlation : public etl::binary_function<TInput, TInput, void>
```
`Correlation_Type` `Population` or `Sample`.  
`TInput`           The input data type.  
`TCalc`            The type to use for internal calculations. By default, equal to `TInput`.  

**correlation_type**  
`etl::correlation_type::Sample`
`etl::correlation_type::Population`

## Construction
```cpp
correlation()
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
correlation(TIterator first1, TIterator last1, TIterator first2)
```
**Decsription**  
Construct from two iterator ranges.

```cpp
void add(TInput value1, TInput value2)
```
**Description**  
Add a pair of values.

## Member functions

```cpp
template <typename TIterator>
void add(TIterator first1, TIterator last1, TIterator first2)
```
**Description** 
Add a range of values.

```cpp
void operator()(TInput value1, TInput value2)
```
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
**Decsription**  
Returns the calculated covariance for the data.

---

```cpp
double get_correlation() const
```
**Description**  
Returns the calculated correlation for the data.

---

```cpp
operator double() const
```
**Description**  
Returns the calculated correlation for the data.

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
Clear the correlation.

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

etl::correlation<etl::correlation_type::Population, char, int32_t> correlation(input_c.begin(),     
                                                                               input_c.end(), 
                                                                               input_c_inv.begin());

double correlation_result;
double covariance_result;

correlation_result = correlation;                  // correlation_result == -1.0
covariance_result  = correlation.get_covariance(); // covariance_result  == -8.25
```
