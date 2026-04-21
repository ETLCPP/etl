---
title: "type_id"
---

{{< callout type="info">}}
  Header: `type_id.h`  
  Since: `TBC`  
{{< /callout >}}

Below is a concise, technical overview of the `etl::type_id` facility, suitable for design documentation or code review context.

## Overview: 
`etl::type_id` is a lightweight, RTTI-free mechanism for uniquely identifying C++ types at runtime. It is designed for environments where standard RTTI (`typeid`, `std::type_info`) is unavailable, undesirable, or too costly—such as embedded systems or performance-constrained platforms.

The implementation guarantees uniqueness per type within a program image while remaining constexpr-friendly and requiring no dynamic allocation.

## Design Concept

The core idea is to associate each distinct type `T` with the address of a unique static object. That address serves as the type’s identifier.

This is achieved through:

```cpp
template <typename T>
struct type_id_anchor
{
  static char value;
};
```

Each instantiation of `type_id_anchor<T>` owns its own `static char value`, and the address of that variable is guaranteed to be unique for each distinct `T`. The address is stable for the lifetime of the program.

## Key Components

### Type Normalization

When requesting a type ID via `type_id::get<T>()`, the type is first normalised:

```cpp
using type = typename etl::remove_cvref<T>::type;
```

This ensures that:

* `T`, `const T`, `T&`, and `T&&` all map to the same `type_id`
* Only the underlying type identity matters

### Type ID Generation

```cpp
template <typename T>
static ETL_CONSTEXPR type_id get() ETL_NOEXCEPT;
```

* Returns a `type_id` that uniquely represents `T`
* Internally stores the address of `type_id_anchor<T>::value`
* Requires no RTTI, no dynamic memory, and no runtime registration

### Invalid Type ID

```cpp
static ETL_CONSTEXPR type_id invalid_id() ETL_NOEXCEPT;
```

* Represents an invalid or uninitialised type ID
* Implemented as a null pointer (`id == nullptr`)
* Default constructor produces this state

## `type_id` Class Semantics

### Storage

```cpp
const void* id;
```

* The identifier is stored as a pointer
* Comparison is purely pointer comparison (fast and deterministic)

### Supported Operations

| Operation          | Behavior                                           |
| ------------------ | -------------------------------------------------- |
| Equality (`==`)    | True if both type IDs refer to the same type       |
| Inequality (`!=`)  | Logical negation of equality                       |
| Ordering (`<`)     | Pointer comparison (useful for ordered containers) |
| Boolean conversion | `true` if valid, `false` if invalid                |
| Integer conversion | Explicit conversion to `intptr_t` / `uintptr_t`    |

### Copy and Assignment

* Copyable and assignable
* Copy semantics preserve identity (pointer value)

## Guarantees and Properties

* **Uniqueness**: Each distinct type has exactly one unique `type_id`
* **Stability**: Type IDs remain valid for the entire program lifetime
* **Zero overhead**: No dynamic allocation, no lookup tables
* **RTTI-free**: Suitable for builds with RTTI disabled
* **Comparable**: Can be used as keys in associative containers

## Typical Use Cases

* Type-safe containers (e.g., variant, any, message buses)
* Plugin or component systems
* Compile-time / runtime type discrimination in embedded systems
* Replacement for `std::type_index` in restricted environments

## Summary

`etl::type_id` provides a robust, minimal, and efficient alternative to C++ RTTI by encoding type identity as the address of a unique static object. Its design is particularly well-suited to embedded and low-level systems where determinism, performance, and binary size are critical.
