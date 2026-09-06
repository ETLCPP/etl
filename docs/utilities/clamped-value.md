---
title: "clamped_value"
---

{{< callout type="info">}}
  Header: `clamped_value.h`  
  Since: `TBC`  
{{< /callout >}}

Provides a value constrained between a minimum and maximum value. Values are
clamped using `etl::clamp`. The type supports compile-time bounds to minimise
storage and runtime bounds when the range must be changed. Integral types are
supported in all language modes. Floating-point types are supported in C++20
when `ETL_HAS_FLOATING_POINT_CLAMPED_VALUE` is `1`. By default, the macro is
enabled when `__cpp_nontype_template_args` is at least `201911L`.

```cpp
template <typename T,
          T Min = T(),
          T Max = T(),
          bool RuntimeSpecialisation = ((Min == T()) && (Max == T()))>
class clamped_value;
```

```cpp
etl::clamped_value<int, 2, 7> value_ct; // Fixed range [2, 7].

etl::clamped_value<int> value_rt(2, 7, 5); // Runtime range and initial value.

#if ETL_HAS_FLOATING_POINT_CLAMPED_VALUE
etl::clamped_value<float, -1.5f, 2.5f> float_ct(0.5f);
etl::clamped_value<double>              float_rt(-1.5, 2.5, 0.5);
#endif
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

`min` must not be greater than `max`. As with `cyclic_value`, zero-initialized
`Min` and `Max` select the runtime-bound specialization; use a non-zero
compile-time bound when a fixed compile-time range is needed. NaN bounds and
initial values are rejected. Infinite bounds and values are permitted.

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
the value directly to a bound. For integral values, `advance` uses the signed
counterpart of `T` as its step type. For floating-point values, the step type
is `T` and may be fractional. Steps saturate in constant time at the minimum
or maximum rather than wrapping. NaN steps are rejected.

Increment and decrement move by one and also saturate, including when a
floating-point bound is less than one unit away:

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
ETL_NODISCARD operator T() const noexcept;
clamped_value& operator=(T value) & noexcept;
clamped_value& operator+=(difference_type n) & noexcept;
clamped_value& operator-=(difference_type n) & noexcept;
```

Conversion and assignment to the underlying type are supported. Assignment is
clamped to the configured range. The lvalue reference qualification on
assignment prevents assignment through temporary values where supported.

## Operations

```cpp
ETL_CONSTEXPR14 void swap(clamped_value& other);
ETL_CONSTEXPR14 void swap(clamped_value& lhs, clamped_value& rhs);
```

Swaps clamped values. Runtime values also swap their bounds.

Equality, inequality, and relational comparisons are provided. Comparisons
against arithmetic values use the implicit conversion to `T`. Equality
compares the current values; runtime bounds do not affect equality. Runtime
ranges must satisfy `min <= max`; invalid ranges trigger an ETL assertion. The
default runtime specialization uses the full representable range of `T`.
