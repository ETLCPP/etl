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
void advance(int n);
```

`set(T)` clamps the current value. The runtime `set(min, max)` changes the
range and resets the value to the new minimum. `to_min()` and `to_max()` move
the value directly to a bound. `advance` increments or decrements repeatedly;
it saturates at the minimum or maximum rather than wrapping.

Increment and decrement operators also saturate:

```cpp
++value; // Remains Max when already at Max.
--value; // Remains Min when already at Min.
```

## Access

```cpp
T get() const noexcept;
T min() const noexcept;
T max() const noexcept;
```

Gets the current value and its bounds. For compile-time bounds, `min()` and
`max()` are static functions.

## Operators

```cpp
operator T() noexcept;
operator const T() const noexcept;
clamped_value& operator=(T value) & noexcept;
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
