---
title: "compare"
---

{{< callout type="info">}}
  Header: `compare.h`  
  Since: `TBC`  
{{< /callout >}}

A helper for deriving comparison operations from a strict weak ordering.
`etl::compare<T, TLess>` exposes boolean predicates and a three-state comparison result.
`etl::three_way_compare` returns `-1`, `0`, or `1` using the same comparison policy.

```cpp
template <typename T, typename TLess = etl::less<T> >
struct compare
```

```cpp
template <typename T, typename TLess = etl::less<T> >
ETL_CONSTEXPR14 int three_way_compare(const T& lhs, const T& rhs)
```

## Types
```cpp
first_argument_type;
second_argument_type;
cmp_result;
```

`cmp_result` has the values `Less`, `Equal`, and `Greater`.

## Members
```cpp
static bool lt(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs < rhs`, otherwise `false`.

---

```cpp
static bool lte(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs <= rhs`, otherwise `false`.

---

```cpp
static bool gt(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs > rhs`, otherwise `false`.

---

```cpp
static bool gte(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs >= rhs`, otherwise `false`.

---

```cpp
static bool eq(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs == rhs`, otherwise `false`.

---

```cpp
static bool ne(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs != rhs`, otherwise `false`.

---

```cpp
static cmp_result cmp(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`Less`, `Equal`, or `Greater`.

## Three-way comparison

`etl::three_way_compare(lhs, rhs)` returns the same ordering as `compare<T, TLess>::cmp(lhs, rhs)`,
but as an `int` with the values `-1`, `0`, or `1`.

## C++20 comparison categories

When compiling as C++20, `compare.h` makes the comparison categories available in the `etl`
namespace, so ETL and user code can name them regardless of the STL configuration.

```cpp
class strong_ordering;
class weak_ordering;
class partial_ordering;
```

* When the STL is available these are aliases for `std::strong_ordering`, `std::weak_ordering`
  and `std::partial_ordering`.
* When the STL is not available, `compare.h` provides its own minimal implementations that
  follow the standard semantics: the named values `less`, `equivalent`, `greater`
  (plus `equal` for `strong_ordering` and `unordered` for `partial_ordering`), comparison
  against the literal `0` only, and the implicit conversions
  `strong_ordering` &rarr; `weak_ordering` &rarr; `partial_ordering`.

An `unordered` result compares `false` against `0` for all of `<`, `<=`, `==`, `>=` and `>`.

```cpp
ETL_CONSTEXPR strong_ordering  make_strong_ordering(int value) ETL_NOEXCEPT;
ETL_CONSTEXPR weak_ordering    make_weak_ordering(int value) ETL_NOEXCEPT;
ETL_CONSTEXPR partial_ordering make_partial_ordering(int value) ETL_NOEXCEPT;
```

These helpers convert a three-way comparison result, such as the one returned by
`etl::three_way_compare`, into a comparison category. Negative values map to `less`, positive
values to `greater`, and zero to `equal`/`equivalent`. `unordered` cannot be expressed as an
integer result, so use `etl::partial_ordering::unordered` directly for that case.

## Example 1
```cpp
struct Test
{
  int a;
  int b;
};

using Compare = etl::compare<Test>;

// Define the 'less-than' operator.
bool operator <(const Test& lhs, const Test& rhs)
{
  return (lhs.a + lhs.b) < (rhs.a + rhs.b);
}

// Define the rest in terms of Compare.
bool operator <=(const Test& lhs, const Test& rhs)
{
  return Compare::lte(lhs, rhs);
}

bool operator >(const Test& lhs, const Test& rhs)
{
  return Compare::gt(lhs, rhs);
}

bool operator >=(const Test& lhs, const Test& rhs)
{
  return Compare::gte(lhs, rhs);
}

int operator <=>(const Test& lhs, const Test& rhs)
{
  return etl::three_way_compare(lhs, rhs);
}
```

## Example 2
Separate 'less-than' class.
```cpp
struct Test
{
  int a;
  int b;
};

struct LessThan
{
  bool operator()(const Test& lhs, const Test& rhs) const
  {
    return (lhs.a + lhs.b) < (rhs.a + rhs.b);
  }
};

using Compare = etl::compare<Test, LessThan>;

bool operator <(const Test& lhs, const Test& rhs)
{
  return Compare::lt(lhs, rhs);
}

bool operator <=(const Test& lhs, const Test& rhs)
{
  return Compare::lte(lhs, rhs);
}

bool operator >(const Test& lhs, const Test& rhs)
{
  return Compare::gt(lhs, rhs);
}

bool operator >=(const Test& lhs, const Test& rhs)
{
  return Compare::gte(lhs, rhs);
}

int operator <=>(const Test& lhs, const Test& rhs)
{
  return etl::three_way_compare<Test, LessThan>(lhs, rhs);
}
```

## Example 3
Inheritance

```cpp
struct Test : public etl::compare<Test>
{
  int a;
  int b;
};

// Define the 'less-than' operator.
bool operator <(const Test& lhs, const Test& rhs)
{
  return (lhs.a + lhs.b) < (rhs.a + rhs.b);
}

// Define the rest.
bool operator <=(const Test& lhs, const Test& rhs)
{
  return Test::lte(lhs, rhs);
}

bool operator >(const Test& lhs, const Test& rhs)
{
  return Test::gt(lhs, rhs);
}

bool operator >=(const Test& lhs, const Test& rhs)
{
  return Test::gte(lhs, rhs);
}
```

## Example 4
C++20, with or without the STL.

```cpp
etl::strong_ordering operator <=>(const Test& lhs, const Test& rhs)
{
  return etl::make_strong_ordering(etl::three_way_compare(lhs, rhs));
}
```
