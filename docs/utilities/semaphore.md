---
title: "semaphore"
---

{{< callout type="info" >}}
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


## counting_semaphore

```cpp
class etl::counting_semaphore<ptrdiff_t LeastMaxValue>
```

A counting semaphore is guaranteed to have a maximum count of at least `LeastMaxValue`.
If it cannot honor it, then it will fail to compile.

{{< callout type="info" >}}
On FreeRTOS, `LeastMaxValue` must be `1` or greater and not `0` or greater as
the standard specifies. This is a limitation of the FreeRTOS APIs.
{{< /callout >}}

## binary_semaphore

```cpp
using etl::binary_semaphore = etl::counting_semaphore<1>;
```

A binary semaphore is simply a counting semaphore where `LeastMaxValue` is 1.

{{< callout type="info" >}}
It is important to point out the caveats of this system: a binary semaphore is
guaranteed to have a maximum count of *at least 1*. It is not *guaranteed* to be
exactly 1, even though most implementations will honor this maximum.

Maximum count may also not behave as you would expect, which is explained in the
[`release`](#release) documentation
{{< /callout >}}

## Methods

### acquire

```cpp
void acquire()
```

Blocking. Decrements the semaphore. Waits forever if it needs to.

### try_acquire

```cpp
bool try_acquire()
```

Non-blocking. Tries to decrement the semaphore.
Fails if the semaphore is already at 0 count.
Returns `true` if it was successful, `false` otherwise.

### try_acquire_for

```cpp
template <class Rep, class Period>
bool try_acquire_for(const etl::chrono::duration<Rep, Period>& rel_time)
```

Blocking. Tries to acquire the semaphore for a specific durations.
Fails if the semaphore is at 0 count for the entire duration.
Returns `true` if it was successful, `false` otherwise.

{{< callout type="info" >}}
The GCC/clang atomics implementation uses `etl::chrono::steady_clock`. Make sure
to provide the methods associated with this clock: see [the relevant documentation](/docs/chrono/clocks/#platform-clock-access).
{{< /callout >}}

### try_acquire_until

```cpp
template <class Clock, class Duration>
bool try_acquire_until(const etl::chrono::time_point<Clock, Duration>& abs_time)
```

Blocking. Tries to acquire the semaphore until a deadline expires.
Fails if the semaphore is always at 0 count until the deadline expires.
Returns `true` if it was successful, `false` otherwise.


### release

```cpp
void release(ptrdiff_t update = 1)
```

Non-blocking. Adds `update` count to the semaphore.

{{< callout type="info" >}}
This may not behave as you would expect.

The standard specifies the following pre-conditions:

- `update >= 0`
- `update <= max() - counter`

Popular implementations including GCC's `libstdc++` will check these 
pre-conditions *after* incrementing the semaphore, with `asserts` that may be
disabled at runtime.

This is due to the implementation complexity and performance costs of atomics:
checking the pre-conditions *before* incrementing involves a compare-and-swap
loop which is costly and may block for an indeterminate amount of time.

In effect, this means using a `binary_semaphore` (or a `counting_semaphore` with
a low `LeastMaxValue`) may allow you to overflow its maximum without runtime error.

While implementing the GCC/clang atomics version of `release`, the decision was
made to follow GCC and check the pre-conditions *after* incrementing the counter.

This caveat may also be valid for the STL/STD (C++20) implementation, depending
on your compiler and standard library.

On FreeRTOS the semaphore maximum is enforced by the kernel, and any
incrementation above the maximum will simply not work and be discarded.

{{< /callout >}}

### max

```cpp
static ETL_CONSTEXPR ptrdiff_t max() ETL_NOEXCEPT
```

Returns `LeastMaxValue`.

