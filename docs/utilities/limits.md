---
title: "limits"
---

An emulation of <limits> under the `etl` namespace.

See https://en.cppreference.com/w/cpp/types/numeric_limits

## is_iec559
When `ETL_NO_STL` is defined, `is_iec559` for `float`, `double` and `long double` is detected from the compiler's predefined macros (GCC and clang), using the same conditions as libstdc++.  
For MSVC it is `true`, as all of its floating point types are IEEE 754.  
If the compiler does not supply them, the value is `false`.  
It can be overridden by defining `ETL_FLOAT_IS_IEC559`, `ETL_DOUBLE_IS_IEC559` or `ETL_LONG_DOUBLE_IS_IEC559` as `true` or `false` in `etl_profile.h`.
