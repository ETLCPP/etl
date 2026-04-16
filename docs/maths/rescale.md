---
title: "rescale"
---

{{< callout type="info">}}
  Header: `rescale.h`  
  Since: `20.9.0`  
{{< /callout >}}

Rescales an input range.

```cpp
template <typename TInput, typename TOutput>
class rescale : public etl::unary_function<TOutput, TInput>
```

`TInput`   The input data type.
`TCompare` The functor type used to compare values to the threshold. The default is `etl::less`.

---

```cpp
rescale (TInput  input_min_value,
         TInput  input_max_value,
         TOutput output_min_value,
         TOutput output_max_value) 
```
**Description**  
Sets the rescale limits.

---

```cpp
TOutput operator()(TInput value) const
```
**Description**  
Rescale a value.

## Example
```cpp
std::array<char, 10> input = 
{ 
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19
};

std::array<int, 10> output;

etl::rescale<char, int> rescale(10, 19, 40000, 41900);

std::transform(input.begin(), input.end(), output.begin(), rescale);

// output = 40000, 40211, 40422, 40633, 40844, 41055, 41266, 41477, 41688, 41900
```
