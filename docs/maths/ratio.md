---
title: "ratio"
---

{{< callout type="info">}}
  Header: `ratio.h`  
  Since: `TBC`  
  Similar to: [std::ratio](https://en.cppreference.com/w/cpp/numeric/ratio/ratio)    
{{< /callout >}}

A template constant to encode ratios.

```cpp
template <const size_t NUM, const size_t DEN = 1>
struct ratio
{
  static ETL_CONSTANT intmax_t num = NUM;
  static ETL_CONSTANT intmax_t den = DEN;
};
```

`num` : Numerator  
`den` : Denominator  

## Predefined ratios

If `INT_MAX` > `INT32_MAX`
```cpp
typedef ratio<1, 1000000000000000000LL * 1000000000000LL> quecto;
typedef ratio<1, 1000000000000000000LL * 1000000000LL>    ronto;
typedef ratio<1, 1000000000000000000000000LL> yocto;
typedef ratio<1, 1000000000000000000000>      zepto;
typedef ratio<1, 1000000000000000000>         atto;
typedef ratio<1, 1000000000000000>            femto;
typedef ratio<1, 1000000000000>               pico;
```

If `INT_MAX` >= `INT32_MAX`
```cpp
typedef ratio<1, 1000000000>                  nano;
typedef ratio<1, 1000000>                     micro;
```

If `INT_MAX` >= `INT16_MAX`
```cpp
typedef ratio<1, 1000>                        milli;
typedef ratio<1, 100>                         centi;
typedef ratio<1, 10>                          deci;
typedef ratio<10, 1>                          deca;
typedef ratio<100, 1>                         hecto;
typedef ratio<1000, 1>                        kilo;
```

If `INT_MAX` >= `INT32_MAX`
```cpp
typedef ratio<1000000, 1>                     mega;
typedef ratio<1000000000, 1>                  giga;
```

If `INT_MAX` > `INT32_MAX`
```cpp
typedef ratio<1000000000000, 1>               tera;
typedef ratio<1000000000000000, 1>            peta;
typedef ratio<1000000000000000000, 1>         exa;
typedef ratio<1000000000000000000000, 1>      zetta;
typedef ratio<1000000000000000000000000LL, 1> yotta;
typedef ratio<1000000000000000000LL * 1000000000LL, 1>    ronna;
typedef ratio<1000000000000000000LL * 1000000000000LL, 1> quetta;
```

An approximation of PI to 6 digits.
```cpp
typedef ratio<355, 113> ratio_pi;
```

An approximation of root 2.
```cpp
typedef ratio<239, 169> ratio_root2;
```

An approximation of 1 over root 2.
```cpp
typedef ratio<169, 239> ratio_1_over_root2;
```

An approximation of e.
```cpp
typedef ratio<326, 120> ratio_e;
```
