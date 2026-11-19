---
title: "meta"
---

{{< callout type="info">}}
  Header: `meta.h`  
  From: `20.50.0`  
  Similar to: `std::meta`
{{< /callout >}}

C++26 static reflection (P2996) in namespace `etl::meta`.

`etl::meta` mirrors `std::meta`, but is usable in both STL and non-STL builds.
In an STL build it is a thin layer of using-declarations over `std::meta`.
In a non-STL build ETL supplies the minimal declarations that the compiler's
reflection builtins require, so no standard library is needed.

```cpp
#include "etl/meta.h"
```

## Compiler support

Static reflection is a very new language feature, and at the time of writing
**GCC 16** is the only compiler that implements enough of P2996 to be usable.
Clang (including the Bloomberg `clang-p2996` fork) and MSVC are not supported.

`etl/meta.h` may always be included. It defines a single macro,
`ETL_HAS_REFLECTION`, as `1` or `0`; when it is `0` the header defines nothing
else. This is the only reflection feature switch in the ETL, and user code
should test it before using anything in `etl::meta`.

```cpp
#include "etl/meta.h"

#if ETL_HAS_REFLECTION
  static_assert(etl::meta::identifier_of(^^my_type) == "my_type");
#endif
```

GCC requires the `-freflection` option in addition to `-std=c++26`.
The reference toolchain is defined in `.devcontainer/gcc16`.

## Usage

Names are used exactly as their `std::meta` equivalents.

```cpp
struct point
{
  int x;
  int y;
};

constexpr auto members =
  etl::meta::nonstatic_data_members_of(^^point,
                                       etl::meta::access_context::unchecked());

static_assert(etl::meta::identifier_of(members[0]) == "x");
static_assert(etl::meta::type_of(members[0])       == ^^int);
static_assert(etl::meta::size_of(^^point)          == 2 * sizeof(int));
```

`identifier_of`, `display_string_of` and `symbol_of` return `std::string_view`
in an STL build and `etl::string_view` in a non-STL build.

## info_array

GCC cannot keep the heap allocation of a `std::vector<info>` alive across a
constant evaluation, so a `template for` over the result of `members_of` does
not compile. `etl::meta::info_array` is a fixed-size, allocation-free array of
`info` that can be iterated instead.

```cpp
constexpr auto ctx = etl::meta::access_context::unchecked();
constexpr auto count = etl::meta::nonstatic_data_members_of(^^point, ctx).size();
constexpr auto members =
  etl::meta::to_info_array<count>(etl::meta::nonstatic_data_members_of(^^point, ctx));

template for (constexpr auto member : members)
{
  // ...
}
```

| Member | Description |
|--------|-------------|
| `begin()` | Pointer to the first element. |
| `end()` | Pointer past the last element. |
| `size()` | The number of elements. |
| `operator[](size_t)` | The element at the index. |

All members are `consteval`.

## Tests

`test_meta.cpp`
