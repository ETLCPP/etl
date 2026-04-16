---
title: "quantize"
---

{{< callout type="info">}}
  Header: `quantize.h`  
  Since: `20.9.0`  
{{< /callout >}}

Quantizes an input range.

```cpp
template <typename TInput, typename TCompare = etl::less<TInput> >
class quantize : public etl::unary_function<TInput, TInput>
```

`TInput`   The input data type.
`TCompare` The functor type used to compare values to the threshold. The default is `etl::less`.

---

```cpp
quantize(const TInput* p_thresholds, 
         const TInput* p_quantizations, 
         size_t        n_quantizations, 
         TCompare      compare = TCompare())
```
**Description**  
`p_thresholds`    A pointer to the list of threshold values.  
`p_quantizations` A pointer to the list of quantization values. One more than the number of thresholds.  
`n_quantizations` The number of quantization values.  

---

```cpp
TInput operator()(TInput value) const
```
**Description**  
Quantatize a value.

---

## Example
```cpp
constexpr size_t Size        = 20U;
constexpr size_t NThresholds = 4U;

std::array<int, NThresholds> thresholds = 
{ 
  14, 18, 22, 25
};

std::array<int, NThresholds + 1> quantizations = 
{ 
  1, 2, 3, 4, 5
};

std::array<int, Size> input = 
{ 
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29
};

std::array<int, Size> output;

etl::quantize<int> quantize(thresholds.data(), quantizations.data(), quantizations.size());

std::transform(input.begin(), input.end(), output.begin(), quantize);

// output = 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5
```
