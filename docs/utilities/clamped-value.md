---
title: "clamped_value"
---

{{< callout type="info">}}
  Header: `clamped_value.h`  
  Since: `TBC`  
{{< /callout >}}

Provides a value constrained between a minimum and maximum value. Values are
clamped using `etl::clamp`. The type supports compile-time bounds to minimise
storage and runtime bounds when the range must be changed.

```cpp
template <typename T, const T Min = 0, const T Max = 0>
class clamped_value;
```

```cpp
etl::clamped_value<int, 2, 7> value_ct; // Fixed range [2, 7].

etl::clamped_value<int> value_rt(2, 7, 5); // Runtime range and initial value.
```

## Constructors

```cpp
clamped_value<int, 2, 7> value;
```

Creates a compile-time clamped value initialized to `Min`.

```cpp
clamped_value<int> value(min, max);
clamped_value<int> value(min, max, initial);
```

Creates a runtime clamped value. The two-argument form initializes the value
to `min`; the three-argument form clamps `initial` to the range.

`min` must not be greater than `max`. As with `cyclic_value`, the template
arguments `Min = 0` and `Max = 0` select the runtime-bound specialization;
use a non-zero compile-time bound when a fixed compile-time range is needed.

## Modifiers

```cpp
void set(T value);
void set(T min, T max);
void to_min();
void to_max();
void advance(difference_type n) noexcept;
```

`set(T)` clamps the current value. The runtime `set(min, max)` changes the
range and resets the value to the new minimum. `to_min()` and `to_max()` move
the value directly to a bound. `advance` uses the signed counterpart of `T`
as its step type and saturates in constant time at the minimum or maximum
rather than wrapping.

Increment and decrement operators also saturate:

```cpp
++value; // Remains Max when already at Max.
--value; // Remains Min when already at Min.
```

## Access

```cpp
ETL_NODISCARD T get() const noexcept;
ETL_NODISCARD T min() const noexcept;
ETL_NODISCARD T max() const noexcept;
```

Gets the current value and its bounds. Both compile-time and runtime
specializations provide these accessors as const-qualified member functions.

## Operators

```cpp
operator T() const noexcept;
clamped_value& operator=(T value) & noexcept;
clamped_value& operator+=(difference_type n) & noexcept;
clamped_value& operator-=(difference_type n) & noexcept;
```

Conversion and assignment to the underlying type are supported. Assignment is
clamped to the configured range. The lvalue reference qualification on
assignment prevents assignment through temporary values where supported.

## Operations

```cpp
void swap(clamped_value& other);
void swap(clamped_value& lhs, clamped_value& rhs);
```

Swaps clamped values. Runtime values also swap their bounds.

Equality, inequality, relational comparisons, and comparisons against `T` are
provided. Equality compares the current values; runtime bounds do not affect
equality. Runtime ranges must satisfy `min <= max`; invalid ranges trigger an
ETL assertion. The default runtime specialization uses the full representable
range of `T`.
