---
title: "compare"
---

{{< callout type="info">}}
  Header: `compare.h`  
  Since: `TBC`  
{{< /callout >}}

A helper class that defines `<=`, `>`, `>=` in terms of `etl::less` or a user supplied compare type. 

```cpp
template <typename T, typename TLess = etl::less<T> >
struct compare
```

## Types
```cpp
first_argument_type;
second_argument_type;
result_type;
```

## Members
```cpp
static result_type lt(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs < rhs`, otherwise `false`.

---

```cpp
static result_type lte(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs <= rhs`, otherwise `false`.

---

```cpp
static result_type gt(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs > rhs`, otherwise `false`.

---

```cpp
static result_type gte(first_argument_type lhs, second_argument_type rhs)
```
**Return**  
`true` if `lhs >= rhs`, otherwise `false`.

## Example 1
```cpp
using std::less
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
