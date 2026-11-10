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
supported in all language modes. Floating-point types with runtime bounds are
supported in C++11 and later when `ETL_HAS_FLOATING_POINT_CLAMPED_VALUE` is
`1`. Floating-point compile-time bounds require C++20 support for
floating-point non-type template arguments.

## Availability

`ETL_HAS_FLOATING_POINT_CLAMPED_VALUE` is always defined by this header as
either `0` or `1`. It defaults to `1` in C++11 and later. The related
`ETL_HAS_COMPILE_TIME_FLOATING_POINT_CLAMPED_VALUE` macro defaults to `1` only
when C++20 is enabled and the compiler supports floating-point non-type
template arguments. A platform profile or build may define either macro before
including the header to override automatic detection.

When `ETL_HAS_FLOATING_POINT_CLAMPED_VALUE` is `0`, `T` must be an integral
type. When it is `1`, `float`, `double`, and `long double` may also be used with
runtime bounds. `clamped_value_range<T>` and `referenced_clamped_value<T>` are
available in C++11 and later.

```cpp
// C++11 and later
template <typename T, T... Limits>
class clamped_value;
```

```cpp
etl::clamped_value<int, 2, 7> value_ct; // Fixed range [2, 7].

etl::clamped_value<int> value_rt(2, 7, 5); // Runtime range and initial value.

#if ETL_HAS_FLOATING_POINT_CLAMPED_VALUE
etl::clamped_value<double> float_rt(-1.5, 2.5, 0.5);
#endif

#if ETL_HAS_COMPILE_TIME_FLOATING_POINT_CLAMPED_VALUE
etl::clamped_value<float, -1.5f, 2.5f> float_ct(0.5f);
#endif
```

In C++11 and later, multiple values can share one immutable runtime range:

```cpp
const etl::clamped_value_range<float> range(-1.5f, 2.5f);

etl::referenced_clamped_value<float> value1(range, 0.5f);
etl::referenced_clamped_value<float> value2(range, 1.5f);
```

`referenced_clamped_value<T>` stores an
`etl::reference_wrapper<const clamped_value_range<T>>`. The range must outlive
every value that refers to it. Construction from a temporary range is deleted.
Copying or swapping a referenced value also copies or swaps its range reference.

## Types

`difference_type` is the signed counterpart of `T` for integral values. For
floating-point values, `difference_type` is `T`, allowing fractional steps.
`limits_type` and `difference_limits_type` provide the corresponding
`etl::numeric_limits` specializations.

`referenced_clamped_value<T>` also defines `range_type` as
`clamped_value_range<T>`.

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

`min` must not be greater than `max`. Omitting the template bounds selects the
runtime-bound specialization. Supplying `Min` and `Max` selects fixed
compile-time bounds. NaN bounds and initial values are rejected. Infinite
bounds and values are permitted.

### Referenced ranges

```cpp
clamped_value_range<T> range(min, max);
referenced_clamped_value<T> value(range);
referenced_clamped_value<T> value(range, initial);
```

The one-argument value constructor initializes to `range.min()`. The
two-argument form clamps `initial` to the shared range. A range is immutable
after construction, so every referring value observes stable bounds. A
default-constructed range uses the full representable range of `T`.

`referenced_clamped_value<T>` is non-owning. The referenced range must outlive
the value and any copies of it. The constructors taking `range_type&&` are
deleted to prevent direct construction from a temporary range.

## Modifiers

```cpp
void set(T value);
void set(T min, T max);
void to_min();
void to_max();
void advance(difference_type n) ETL_NOEXCEPT;
```

`set(T)` clamps the current value. The runtime `set(min, max)` changes the
range and resets the value to the new minimum. `to_min()` and `to_max()` move
the value directly to a bound. For integral values, `advance` uses the signed
counterpart of `T` as its step type. For floating-point values, the step type
is `T` and may be fractional. Steps saturate in constant time at the minimum
or maximum rather than wrapping. NaN steps are rejected.

`referenced_clamped_value<T>` supports `set(T)`, `to_min()`, `to_max()`, and
`advance(difference_type)`. It does not provide `set(min, max)` because its
range is immutable and shared.

Positive steps move toward `max`; negative steps move toward `min`. Infinite
steps saturate at a finite bound. Infinite values and bounds remain valid.

```cpp
etl::clamped_value<float, -1.0f, 1.0f> value(0.25f);
value.advance(0.5f);  // 0.75f
value.advance(10.0f); // 1.0f
value.advance(-0.5f); // 0.5f
```

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

`referenced_clamped_value<T>` additionally provides:

```cpp
ETL_NODISCARD const range_type& range() const noexcept;
```

This returns the exact `clamped_value_range<T>` object referenced by the value.

## Operators

```cpp
ETL_NODISCARD operator T() const noexcept;
clamped_value& operator++() & noexcept;
clamped_value operator++(int) noexcept;
clamped_value& operator--() & noexcept;
clamped_value operator--(int) noexcept;
clamped_value& operator=(T value) & noexcept;
clamped_value& operator+=(difference_type n) &  noexcept;
clamped_value& operator-=(difference_type n) & noexcept;
```

Conversion and assignment to the underlying type are supported. Assignment is
clamped to the configured range. The lvalue reference qualification on
assignment prevents assignment through temporary values where supported. For
floating-point values, assignment and compound-assignment reject NaN.

Prefix increment and decrement return a reference to the updated object.
Postfix increment and decrement return a copy of the value before it was
updated. All four operations move by one and saturate at the applicable bound.
`referenced_clamped_value<T>` provides the same conversion, assignment,
increment, decrement, and compound-assignment operators.

## Operations

```cpp
ETL_CONSTEXPR14 void swap(clamped_value& other);
ETL_CONSTEXPR14 void swap(clamped_value& lhs, clamped_value& rhs);
```

Swaps clamped values. Runtime values also swap their bounds.

Swapping two `referenced_clamped_value<T>` objects swaps both their current
values and range references. Copy construction and copy assignment preserve
the source's range identity; they do not copy the range itself.

Equality, inequality, and relational comparisons are provided. Comparisons
against arithmetic values use the implicit conversion to `T`. Equality
compares the current values; runtime bounds do not affect equality. Runtime
ranges must satisfy `min <= max`; invalid ranges trigger an ETL assertion. The
default runtime specialization uses the full representable range of `T`.

Floating-point comparisons use the underlying values directly; no approximate
or tolerance-based comparison is applied.

## Error Handling

Runtime ranges with `min > max` trigger an ETL assertion. NaN values, bounds,
assignments, and steps also trigger an ETL assertion. Depending on the ETL
error-handler configuration, an assertion may throw `etl::exception`, invoke a
configured handler, or terminate. Operations that validate input are only
`noexcept` when exceptions are disabled.

The same validation applies when constructing `clamped_value_range<T>` and
when assigning or advancing a `referenced_clamped_value<T>`.

## Shared Referenced Ranges

`clamped_value_range<T>` and `referenced_clamped_value<T>` are available in
C++11 and later. They let independent values share immutable runtime bounds.

### `clamped_value_range<T>`

```cpp
template <typename T>
class clamped_value_range;
```

#### Constructors

```cpp
clamped_value_range();
clamped_value_range(T min, T max);
```

The default constructor uses the full representable range of `T`. The
two-argument constructor stores immutable bounds and rejects reversed or NaN
bounds.

#### Accessors

```cpp
ETL_NODISCARD T min() const noexcept;
ETL_NODISCARD T max() const noexcept;
```

### `referenced_clamped_value<T>`

```cpp
template <typename T>
class referenced_clamped_value;
```

The class stores a current value and an
`etl::reference_wrapper<const clamped_value_range<T>>`. It does not own the
range, which must outlive the value and all copies of it.

Member types are `range_type`, `difference_type`, `limits_type`, and
`difference_limits_type`, with the same numeric meanings as `clamped_value<T>`.

#### Constructors

```cpp
explicit referenced_clamped_value(const range_type& range);
referenced_clamped_value(const range_type& range, T initial);
referenced_clamped_value(const referenced_clamped_value& other);

referenced_clamped_value(range_type&&) = delete;
referenced_clamped_value(range_type&&, T) = delete;
```

The one-argument constructor initializes to `range.min()`. The two-argument
constructor clamps `initial`. Temporary ranges are rejected to prevent an
immediately dangling reference. Copy construction preserves the source value
and range identity.

#### Modifiers

```cpp
void set(T value);
void to_min() noexcept;
void to_max() noexcept;
void advance(difference_type n);
```

These operations mirror `clamped_value<T>`. There is no `set(min, max)` because
the referenced range is immutable and shared.

#### Accessors

```cpp
ETL_NODISCARD T get() const noexcept;
ETL_NODISCARD T min() const noexcept;
ETL_NODISCARD T max() const noexcept;
ETL_NODISCARD const range_type& range() const noexcept;
ETL_NODISCARD operator T() const noexcept;
```

`range()` returns the exact range object held by reference.

#### Operators

```cpp
referenced_clamped_value& operator=(const referenced_clamped_value& other) & noexcept;
referenced_clamped_value& operator=(T value) &;
referenced_clamped_value& operator++() & noexcept;
referenced_clamped_value operator++(int) noexcept;
referenced_clamped_value& operator--() & noexcept;
referenced_clamped_value operator--(int) noexcept;
referenced_clamped_value& operator+=(difference_type n) &;
referenced_clamped_value& operator-=(difference_type n) &;
```

Copy assignment copies both the current value and range reference. Arithmetic
and value assignment clamp to the referenced bounds. Comparisons compare only
current values, not range identity or bounds.

#### Swap

```cpp
void swap(referenced_clamped_value& other) noexcept;
void swap(referenced_clamped_value& lhs,
          referenced_clamped_value& rhs) noexcept;
```

Swap exchanges both current values and range references, keeping each value
associated with its original bounds.

#### Errors

Invalid or NaN range bounds trigger an ETL assertion. NaN initial values,
assignments, and steps are also rejected. Infinite values follow the same
behavior as `clamped_value<T>`.
