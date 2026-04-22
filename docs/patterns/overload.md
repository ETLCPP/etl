---
title: "overload"
---

{{< callout type="info">}}
  Header: `overload.h`  
  Since: `20.14.0`  
{{< /callout >}}

**For C++17 and above**  

Allows creation of a functor of overloaded lambdas.  

```cpp
etl::overload<typename... TOverloads>
```

## Template deduction guide

template <typename... TOverloads> 
overload(TOverloads...) -> overload<TOverloads...>;

## Non-member functions

```cpp
template <typename... TOverloads>
constexpr overload<TOverloads...> make_overload(TOverloads&&... overloads)
```
Creates an `etl::overload` from a set of lambdas.

## Example

```cpp
int         result_int;
double      result_double;
std::string result_string;

auto overloaded = etl::make_overload([](int i) { result_int = i; },
                                     [](double d) { result_double = d; },
                                     [](const std::string& s) { result_string = s; });

template <typename T, typename TOverload>
void Function(T value, TOverload&& ol)
{
  ol(value);
}

Function(int(1), overloaded);           // result_int    == 1
Function(double(2.2), overloaded);      // result_double == 2.2
Function(std::string("3"), overloaded); // result_string == "3"
```

---

**Alternative syntax**  
```cpp
Function(int(1), etl::overload
                 {
                   [](int i) { result_int = i; },
                   [](double d) { result_double = d; },
                   [](const std::string& s) { result_string = s; }
                 });
```
