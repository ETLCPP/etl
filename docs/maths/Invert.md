---
title: "Invert"
---

{{< callout type="info">}}
  Header: `invert.h`  
  Since: `20.9.0`  
{{< /callout >}}

Invert an input range.

```cpp
template <typename TInput>
class invert : public etl::unary_function<TInput, TInput>
```

``TInput```   The input data type.

```cpp
invert() 
```
**Description**  
`offset` = 0
`minuend` = `etl::numeric_limits<TInput>::is_signed) ? TInput(0)
                                                   : etl::numeric_limits<TInput>::max())`

If the type is signed then the input values have their sign changed.  
i.e. `10` => `-10`, `-10`  => `10`.  
If the type is unsigned then the input values are subtracted from the type's maximum value.

---

```cpp
invert(TInput offset, TInput minuend)
```
**Description**  
Sets the offset and minuend.

---

```cpp
TInput operator()(TInput value) const
```
**Description**  
Invert a value.
Calculates  `minuend - (value - offset)`

## Example
```cpp
std::array<int, 10> input = 
{ 
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19
};

std::array<int, 10> output;

etl::invert<int> invert(10, 100); // offset = 10, minuend = 100

std::transform(input.begin(), input.end(), output.begin(), invert);

// output = 100.0, 99.0, 98.0, 97.0, 96.0, 95.0, 94.0, 93.0, 92.0, 91.0
```
