---
title: "mean"
---

{{< callout type="info">}}
  Header: `mean.h`  
  Since: `20.9.0`  
{{< /callout >}}

```cpp
template <typename TInput, typename TCalc = TInput>
class mean : public etl::unary_function<TInput, void> 
```

`TInput` The input data type.
`TCalc`  The type to use for internal calculations. By default, equal to `TInput`.

---

```cpp
mean()
```
**Description**  
Default constructor.

---

```cpp
template <typename TIterator>
mean(TIterator first, TIterator last)
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
double get_mean() const
```
**Description**  
Returns the calculated mean for the data.

---

```cpp
operator double() const
```
**Return**  
The calculated mean for the data.

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
Clear the mean.

## Example

```cpp
std::array<char, 10> input
{
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

etl::mean<char, int32_t> mean(input.begin(), input.end());

double mean_result;

mean_result = mean; // mean_result  == 4.5
```
