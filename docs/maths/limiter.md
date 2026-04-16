---
title: "limiter"
---

{{< callout type="info">}}
  Header: `limiter.h`  
  Since: `20.9.0`  
{{< /callout >}}

Limits an input range.

```cpp
template <typename TInput, typename TCompare = etl::less<TInput> >
class limit : public etl::unary_function<TInput, TInput>
```

`TInput`   The input data type.
`TCompare` The functor type used to compare values to the threshold. The default is `etl::less`.

```cpp
limit(TInput lowest, TInput highest)
```
**Description**  
`lowest`  The lowest limit.
`highest` The highest limit.

---

```cpp
TInput operator()(TInput value) const
```
**Return**  
The limited value.

```cpp
Example

std::array<int, 10> input = 
{ 
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19
};

std::array<int, 10> output;

etl::limiter<int> limiter(13, 16);

std::transform(input.begin(), input.end(), output.begin(), limiter);

// output = 13, 13, 13, 13, 14, 15, 16, 16, 16, 16
```
