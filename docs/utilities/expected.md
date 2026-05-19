---
title: "expected"
---

{{< callout type="info">}}
  Header: `expected.h`  
  From: `20.35.13`  
  Similar to: `std::expected`
{{< /callout >}}

A generic result type for returning either a result or an error.  
Replacement for `etl::result`.

```cpp
template <typename TValue, typename TError>
class expected
```

Specialisation for `void` result.
```cpp
template <typename TError>
class expected<void, TError>
```

## unexpected
template <typename TError>
class unexpected<TError>

## unexpect_t
A tag type for in-place construction of an unexpected value in an `etl::expected` object.

---

struct unexpect_t;

A constant of type `etl::unexpect_t` which is directly passed to a constructor of `etl::expected` to construct an unexpected value.  

```cpp
inline constexpr unexpect_t unexpect;
```
From: C++17.

```cpp
static const unexpect_t unexpect;
```
Before: C++17.

## unexpected

```cpp
template <typename TError>
class unexpected<TError>
```

### Types

`error_type = TError`

### Member functions

```cpp
ETL_CONSTEXPR
unexpected(const unexpected& other)
```
**Description**  
Copy constructor

---

```cpp
ETL_CONSTEXPR 
unexpected(unexpected&& other)
```
**Description**  
Move constructor.  
Since: C++11

---

```cpp
template <typename TError>
constexpr explicit unexpected(TError&& e)
```
**Description**  
Construct from an argument.  
Since: C++11

---

```cpp
template <typename TError>
explicit unexpected(const TError& e)
```
**Description**  
Construct from argument.
Before: C++11

---

```cpp
template <typename... TArgs>
constexpr explicit unexpected(etl::in_place_t, TArgs&&... args)
```
**Description**  
Construct from arguments.
Since: C++11

---

```cpp
template <typename U, typename... TArgs>
constexpr explicit unexpected(etl::in_place_t, std::initializer_list<U> init, TArgs&&... args)
```
**Description**  
Construct from initializer_list and arguments.
Since: C++11

---

```cpp
ETL_CONSTEXPR14
etl::unexpected<TError>& operator =(const etl::unexpected<TError>& rhs)
```
**Description**  
Assign from etl::unexpected

---

```cpp
ETL_CONSTEXPR14
etl::unexpected<TError>& operator =(etl::unexpected<TError>&& rhs)
```
**Description**  
Move assign from etl::unexpected
Since: C++11

---

```cpp
TError& error() & noexcept
```
**Description**  
Get the error.
Since: C++11

---

```cpp
constexpr const TError& error() const& noexcept
```
**Description**  
Get the error.
Since: C++11

---

```cpp
TError&& error() && noexcept
```
**Description**  
Get the error.
Since: C++11

---

```cpp
constexpr TError&& error() const&& noexcept
```
**Description**  
Get the error.
Since: C++11

---

```cpp
const TError& error() const
```
**Description**  
Get the error.
Before: C++11

---

```cpp
void swap(etl::unexpected<TError>& other)
```
**Description**  
Swap with another etl::unexpected

## expected

```cpp
template <typename TValue, typename TError>
class expected
```cpp

Specialisation for void value type.
```cpp
template <typename TError>
class expected<void, TError>
```cpp

### Types

```cpp
this_type       etl::expected<TValue, TError>
value_type      TValue
error_type      TError
unexpected_type etl::unexpected<TError>
```

### Member functions

```cpp
ETL_CONSTEXPR14 expected() ETL_NOEXCEPT
```
**Description**  
Default constructor

---

```cpp
ETL_CONSTEXPR14 expected(const value_type& value) ETL_NOEXCEPT
```
**Description**  
Constructor

---

```cpp
ETL_CONSTEXPR14 expected(value_type&& value) ETL_NOEXCEPT
```
**Description**  
Constructor
Since: C++11

---

```cpp
ETL_CONSTEXPR14 expected(const expected& other) ETL_NOEXCEPT
```
**Description**  
Copy constructor

---

```cpp
ETL_CONSTEXPR14 expected(expected&& other) ETL_NOEXCEPT
```
**Description**  
Move constructor
Since: C++11

---

```cpp
template <typename F>
ETL_CONSTEXPR14 explicit expected(const etl::unexpected<F>& ue)
```
**Description**  
Copy construct from unexpected type.

---

```cpp
template <typename F>
ETL_CONSTEXPR14 explicit expected(etl::unexpected<F>&& ue)
```
**Description**  
Move construct from unexpected type.
Since: C++11

---

```cpp
ETL_CONSTEXPR14 explicit expected(etl::in_place_t) ETL_NOEXCEPT
```
**Description**  
Construct with default value type.

---

```cpp
template <typename... TArgs>
ETL_CONSTEXPR14 explicit expected(etl::in_place_t, TArgs&&... args)
```
**Description**  
Construct value type from arguments.
Since: C++11

---

```cpp
template <typename U, typename... TArgs>
ETL_CONSTEXPR14 explicit expected(etl::in_place_t, std::initializer_list<U> il, TArgs&&... args)
```
**Description**  
Construct value type from initializer_list and arguments.

---

```cpp
template <typename... TArgs>
ETL_CONSTEXPR14 explicit expected(etl::unexpect_t, TArgs&&... args)
```
**Description**  
Construct error type from arguments.
Since: C++11

---

```cpp
template <typename U, typename... TArgs>
ETL_CONSTEXPR14 explicit expected(etl::unexpect_t, std::initializer_list<U> il, TArgs&&... args)
```
**Description**  
Construct error type from initializer_list and arguments.
Since: C++11

---

```cpp
this_type& operator =(const this_type& other)
```
**Description**  
Copy assign 

---

```cpp
this_type& operator =(this_type&& other)
```
**Description**  
Move assign
Since: C++11

---

```cpp
expected& operator =(const value_type& value)
```
**Description**  
Copy assign from value

---

```cpp
expected& operator =(value_type&& value)
```
**Description**  
Move assign from value
Since: C++11

---

```cpp
expected& operator =(const unexpected_type& error)
```
**Description**  
Copy assign from error

---

```cpp
expected& operator =(unexpected_type&& error)
```
**Description**  
Move assign from error
Since: C++11

---

```cpp
ETL_CONSTEXPR14 value_type& value()&
```
**Description**  
Get the value.
Undefined if has_value() returns false.
Not valid for void specialisation.
Since: C++11

---

```cpp
ETL_CONSTEXPR14 const value_type& value() const&
```
**Description**  
Get the value.
Undefined if has_value() returns false.
Not valid for void specialisation.
Since: C++11

---

```cpp
ETL_CONSTEXPR14 value_type&& value()&&
```
**Description**  
Get the value.
Undefined if has_value() returns false.
Not valid for void specialisation.
Since: C++11

---

```cpp
ETL_CONSTEXPR14 const value_type&& value() const&&
```
**Description**  
Get the value.
Undefined if has_value() returns false.
Not valid for void specialisation.
Since: C++11

---

```cpp
value_type& value() const
```
**Description**  
Get the value.
Undefined if has_value() returns false.
Not valid for void specialisation.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
bool has_value() const
```
**Description**  
Returns true if the class contains a value.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
operator bool() const
```
**Description**  
Returns true if the class contains a value.

---

```cpp
template <typename U>
ETL_NODISCARD
ETL_CONSTEXPR14
value_type value_or(U&& default_value) const&
```
**Description**  
Returns the value or default_value if has_value() returns false.
Since: C++11

---

```cpp
template <typename U>
ETL_NODISCARD
ETL_CONSTEXPR14
value_type value_or(U&& default_value)&&
```
**Description**  
Returns the value or default_value if has_value() returns false.
Since: C++11

---

```cpp
template <typename U>
value_type value_or(const U& default_value) const
```
**Description**  
Returns the value or default_value if has_value() returns false.
Before: C++11

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
error_type& error()& ETL_NOEXCEPT
```
**Description**  
Returns the error.
Undefined if has_value() returns true.
Since: C++11

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
const error_type& error() const& ETL_NOEXCEPT
```
**Description**  
Returns the error.
Undefined if has_value() returns true.
Since: C++11

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
error_type&& error() && ETL_NOEXCEPT
```
**Description**  
Returns the error.
Undefined if has_value() returns true.
Since: C++11

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
const error_type&& error() const&& ETL_NOEXCEPT
```
**Description**  
Returns the error.
Undefined if has_value() returns true.
Since: C++11

---

```cpp
error_type& error() const
```
**Description**  
Returns the error.
Undefined if has_value() returns true.
Before: C++11

---

```cpp
template <typename... TArgs>
ETL_CONSTEXPR14 value_type& emplace(TArgs&&... args) ETL_NOEXCEPT
```
**Description**  
Returns the error if has_value() returns false.
Undefined if has_value() returns true.
Since: C++11

---

```cpp
template <typename U, typename... TArgs>
ETL_CONSTEXPR14 value_type& emplace(std::initializer_list<U>& il, TArgs&&... args) ETL_NOEXCEPT
```
**Description**  
Create from arguments.
Since: C++11

---

```cpp
value_type* operator ->()
```
**Description**  
Class member access operator.

---

```cpp
const value_type* operator ->() const
```
**Description**  
Const class member access operator.

---

```cpp
value_type& operator *()
```
**Description**  
Dereference operator.

---

```cpp
const value_type& operator *() const
```
**Description**  
Dereference operator.
