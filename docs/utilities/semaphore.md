---
title: "semaphore"
---

{{< callout type="info">}}
  Header: `semaphore.h`  
  Since: TBC  
  Similar to: [std::counting_semaphore, std::binary_semaphore](https://en.cppreference.com/cpp/thread/counting_semaphore)
{{< /callout >}}

This header attempts to replicate standard counting and binary semaphores.

## Supported implementations

| Implementation    | Associated definitions                                        |
|-------------------|---------------------------------------------------------------|
| FreeRTOS          | `ETL_TARGET_OS_FREERTOS`                                        |
| STL/STD (C++20)   | `ETL_USING_STL && ETL_USING_CPP20 && ETL_USING_STD_SEMAPHORE`   |
| GCC/clang atomics | `(ETL_COMPILER_GCC \|\| ETL_COMPILER_CLANG) && ETL_USING_CPP11` |


## Implementation quirks and deviations from standard

### FreeRTOS

On FreeRTOS, `LeastMaxValue` must be `1` or greater and not `0` or greater as the standard specifies.

### GCC/clang sync

The GCC/clang sync implementation uses `etl::chrono::steady_clock`. Make sure to
provide the methods associated with this clock: see [the relevant documentation](/docs/chrono/clocks/#platform-clock-access).
