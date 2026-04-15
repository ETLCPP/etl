---
title: "Gamma"
---

{{< callout type="info">}}
  Header: `gamma.h`  
  Since: `20.9.0`  
{{< /callout >}}

There are two gamma functors, `gamma_encode` and `gamma_decode`.

```cpp
template <typename TInput>
class gamma_encode : public etl::unary_function<TInput, TInput>
```
```cpp
template <typename TInput>
class gamma_decode : public etl::unary_function<TInput, TInput>
```

`TInput` The input data type.

```cpp
gamma_encode(double gamma, TInput maximum)
```
**Description**  
Constructor.

---

```cpp
TInput operator()(TInput value) const
```
**Description**  
Gamma a value.

---

```cpp
gamma_decode(double gamma, TInput maximum)
```
**Decsription**  
Constructor.

---

```cpp
TInput operator()(TInput value) const
```
**Description**  
Gamma a value.

## Example
```cpp
std::array<double, 10> input
{
  0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
};

std::array<double, 10> output_encode;

etl::gamma_encode<double> gamma_encode(0.5, 9.0);

std::transform(input.begin(), input.end(), output_encode.begin(), gamma_encode);

// output_encode == 0.0, 0.11, 0.44, 1.00, 1.78, 2.78, 4.00, 5.44, 7.11, 9.00

std::array<double, 10> output_decode;

etl::gamma_decode<double> gamma_decode(0.5, 9.0);

std::transform(output_encode.begin(), output_encode.end(), output_decode.begin(), gamma_decode);

// output_decode == 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
```
