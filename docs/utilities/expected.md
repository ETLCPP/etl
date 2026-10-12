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
```

Specialisation for void value type.
```cpp
template <typename TError>
class expected<void, TError>
```

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
template <typename U>
ETL_CONSTEXPR14 expected(U&& value)
```
**Description**  
Construct the value type from a value implicitly convertible to it.
Not selected when U is an etl::expected, etl::unexpected, etl::in_place_t, etl::unexpect_t or value_type itself.
Since: C++11

---

```cpp
template <typename U>
ETL_CONSTEXPR14 explicit expected(U&& value)
```
**Description**  
Construct the value type from a value that is only explicitly convertible to it.
Constrained as above.
Since: C++11

---

```cpp
template <typename U, typename G>
expected(const etl::expected<U, G>& other)
```
**Description**  
Copy construct from an etl::expected<U, G> whose value and error types are both implicitly convertible.
Not selected when value_type can be constructed from, or etl::unexpected<error_type> built from, the source etl::expected itself.
Since: C++11

---

```cpp
template <typename U, typename G>
explicit expected(const etl::expected<U, G>& other)
```
**Description**  
Copy construct from an etl::expected<U, G> whose value or error type is only explicitly convertible.
Constrained as above.
Since: C++11

---

```cpp
template <typename U, typename G>
expected(etl::expected<U, G>&& other)
```
**Description**  
Move construct from an etl::expected<U, G> whose value and error types are both implicitly convertible.
Constrained as above.
Since: C++11

---

```cpp
template <typename U, typename G>
explicit expected(etl::expected<U, G>&& other)
```
**Description**  
Move construct from an etl::expected<U, G> whose value or error type is only explicitly convertible.
Constrained as above.
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
template <typename U>
expected& operator =(U&& value)
```
**Description**  
Assign from a value convertible to value_type.
Constrained like the value constructor.
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
Before: C++11

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
template <typename G>
ETL_NODISCARD
ETL_CONSTEXPR14
error_type error_or(G&& default_error) const&
```
**Description**
Returns default_error if has_value() returns true; otherwise returns the stored error.
Since: C++11

---

```cpp
template <typename G>
ETL_NODISCARD
ETL_CONSTEXPR14
error_type error_or(G&& default_error)&&
```
**Description**
Returns default_error if has_value() returns true; otherwise returns the stored error.
Since: C++11

---

```cpp
template <typename G>
error_type error_or(const G& default_error) const
```
**Description**
Returns default_error if has_value() returns true; otherwise returns the stored error.
Before: C++11

---

```cpp
template <typename... TArgs>
ETL_CONSTEXPR14 value_type& emplace(TArgs&&... args) ETL_NOEXCEPT
```
**Description**  
Create from arguments.
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
value_type& operator *() &
```
**Description**
Dereference operator.

---

```cpp
const value_type& operator *() const&
```
**Description**
Dereference operator.

---

```cpp
value_type&& operator *() &&
```
**Description**
Dereference operator.
Since: C++11

---

```cpp
const value_type&& operator *() const&&
```
**Description**
Dereference operator.
Since: C++11

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
value_type* begin() ETL_NOEXCEPT
```
**Description**
Returns a pointer to the value if has_value() returns true, otherwise returns nullptr.
Allows expected to be used as a range of 0 or 1 elements. Not valid for void specialisation.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
const value_type* begin() const ETL_NOEXCEPT
```
**Description**
Returns a pointer to the value if has_value() returns true, otherwise returns nullptr. Not valid for void specialisation.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
value_type* end() ETL_NOEXCEPT
```
**Description**
Returns a pointer past the value if has_value() returns true, otherwise returns nullptr. Not valid for void specialisation.

---

```cpp
ETL_NODISCARD
ETL_CONSTEXPR14
const value_type* end() const ETL_NOEXCEPT
```
**Description**
Returns a pointer past the value if has_value() returns true, otherwise returns nullptr. Not valid for void specialisation.

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TValue&>::type>::type>
auto transform(F&& f) & -> expected<U, TError>
```
**Description**
If has_value() returns true, it invokes f on the value and returns the result wrapped in an expected object; otherwise it returns an expected object that contains error(). For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void>::type>::type>
auto transform(F&& f) & -> expected<U, TError>
```
**Description**
For void specialisation.
If has_value() returns true, it invokes f and returns the result wrapped in an expected object; otherwise it returns an expected object that contains error().
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TValue&>::type>::type>
auto transform(F&& f) const& -> expected<U, TError>
```
**Description**
If has_value() returns true, it invokes f on the value and returns the result wrapped in an expected object; otherwise it returns an expected object that contains error(). For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void>::type>::type>
auto transform(F&& f) const& -> expected<U, TError>
```
**Description**
For void specialisation.
If has_value() returns true, it invokes f and returns the result wrapped in an expected object; otherwise it returns an expected object that contains error().
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TValue&&>::type>::type>
auto transform(F&& f) && -> expected<U, TError>
```
**Description**
If has_value() returns true, it invokes f on the value and returns the result wrapped in an expected object; otherwise it returns an expected object that contains error(). For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void>::type>::type>
auto transform(F&& f) && -> expected<U, TError>
```
**Description**
For void specialisation.
If has_value() returns true, it invokes f and returns the result wrapped in an expected object; otherwise it returns an expected object that contains error().
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TValue&&>::type>::type>
auto transform(F&& f) const&& -> expected<U, TError>
```
**Description**
If has_value() returns true, it invokes f and returns the result wrapped in an expected object; otherwise it returns an expected object that contains error(). For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void>::type>::type>
auto transform(F&& f) const&& -> expected<U, TError>
```
**Description**
For void specialisation.
If has_value() returns true, it invokes f and returns the result wrapped in an expected object; otherwise it returns an expected object that contains error().
Since: C++11

---

```cpp
template < typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TValue&>::type>::type>
auto and_then(F&& f) & -> U
```
**Description**
If has_value() returns true, it invokes f on the value and returns the result; otherwise it returns an expected object that contains error(). For void specialisation see below.
Since: C++11

---

```cpp
template < typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void>::type>
auto and_then(F&& f) & -> U
```
**Description**
For void specialisation.
If has_value() returns true, it invokes f and returns the result; otherwise it returns an expected object that contains error().
Since: C++11

---

```cpp
template < typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TValue&>::type>::type>
auto and_then(F&& f) const& -> U
```
**Description**
If has_value() returns true, it invokes f on the value and returns the result; otherwise it returns an expected object that contains error(). For void specialisation see below.
Since: C++11

---

```cpp
template < typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void>::type>::type>
auto and_then(F&& f) const& -> U
```
**Description**
For void specialisation.
If has_value() returns true, it invokes f and returns the result; otherwise it returns an expected object that contains error().
Since: C++11

---

```cpp
template < typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TValue&&>::type>::type>
auto and_then(F&& f) && -> U
```
**Description**
If has_value() returns true, it invokes f on the value and returns the result; otherwise it returns an expected object that contains error(). For void specialisation see below.
Since: C++11

---

```cpp
template < typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void>::type>::type>
auto and_then(F&& f) && -> U
```
**Description**
For void specialisation.
If has_value() returns true, it invokes f and returns the result; otherwise it returns an expected object that contains error().
Since: C++11

---

```cpp
template < typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TValue&&>::type>::type>
auto and_then(F&& f) const&& -> U
```
**Description**
If has_value() returns true, it invokes f on the value and returns the result; otherwise it returns an expected object that contains error(). For void specialisation see below.
Since: C++11

---

```cpp
template < typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void>::type>::type>
auto and_then(F&& f) const&& -> U
```
**Description**
For void specialisation.
If has_value() returns true, it invokes f and returns the result; otherwise it returns an expected object that contains error().
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TError&>::type>::type>
auto or_else(F&& f) & -> U
```
**Description**
If has_value() returns true, it returns an expected object that contains value(); otherwise it invokes f on error() and returns the result. For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TError&>::type>::type>
auto or_else(F&& f) & -> U
```
**Description**
For void specialisation.
If has_value() returns true, it returns a default constructed expected object of the appropriate type; otherwise it invokes f on error() and returns the result.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TError&>::type>::type>
auto or_else(F&& f) const& -> U
```
**Description**
If has_value() returns true, it returns an expected object that contains value(); otherwise it invokes f on error() and returns the result. For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TError&>::type>::type>
auto or_else(F&& f) const& -> U
```
**Description**
For void specialisation.
If has_value() returns true, it returns a default constructed expected object of the appropriate type; otherwise it invokes f on error() and returns the result.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TError&&>::type>::type>
auto or_else(F&& f) && -> U
```
**Description**
If has_value() returns true, it returns an expected object that contains value(); otherwise it invokes f on error() and returns the result. For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TError&&>::type>::type>
auto or_else(F&& f) && -> U
```
**Description**
For void specialisation.
If has_value() returns true, it returns a default constructed expected object of the appropriate type; otherwise it invokes f on error() and returns the result.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TError&&>::type>::type>
auto or_else(F&& f) const&& -> U
```
**Description**
If has_value() returns true, it returns an expected object that contains value(); otherwise it invokes f on error() and returns the result. For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TError&&>::type>::type>
auto or_else(F&& f) const&& -> U
```
**Description**
For void specialisation.
If has_value() returns true, it returns a default constructed expected object of the appropriate type; otherwise it invokes f on error() and returns the result.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TError&>::type>::type>
auto transform_error(F&& f) & -> expected<TValue, U>
```
**Description**
If has_value() returns true, it returns an expected that contains value(); otherwise it invokes f on error() and returns the result wrapped in an expected object. For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TError&>::type>::type>
auto transform_error(F&& f) & -> expected<TValue, U>
```
**Description**
For void specialisation.
If has_value() returns true, it returns a default constructed expected object of the appropriate type; otherwise it invokes f on error() and returns the result wrapped in an expected object.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TError&>::type>::type>
auto transform_error(F&& f) const& -> expected<TValue, U>
```
**Description**
If has_value() returns true, it returns an expected that contains value(); otherwise it invokes f on error() and returns the result wrapped in an expected object. For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TError&>::type>::type>
auto transform_error(F&& f) const& -> expected<TValue, U>
```
**Description**
For void specialisation.
If has_value() returns true, it returns a default constructed expected object of the appropriate type; otherwise it invokes f on error() and returns the result wrapped in an expected object.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TError&&>::type>::type>
auto transform_error(F&& f) && -> expected<TValue, U>
```
**Description**
If has_value() returns true, it returns an expected that contains value(); otherwise it invokes f on error() and returns the result wrapped in an expected object. For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, TError&&>::type>::type>
auto transform_error(F&& f) && -> expected<TValue, U>
```
**Description**
For void specialisation.
If has_value() returns true, it returns a default constructed expected object of the appropriate type; otherwise it invokes f on error() and returns the result wrapped in an expected object.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TError&&>::type>::type>
auto transform_error(F&& f) const&& -> expected<TValue, U>
```
**Description**
If has_value() returns true, it returns an expected that contains value(); otherwise it invokes f on error() and returns the result wrapped in an expected object. For void specialisation see below.
Since: C++11

---

```cpp
template <typename F, typename U = typename etl::remove_cvref< typename etl::invoke_result<F, void, const TError&&>::type>::type>
auto transform_error(F&& f) const&& -> expected<TValue, U>
```
**Description**
For void specialisation.
If has_value() returns true, it returns a default constructed expected object of the appropriate type; otherwise it invokes f on error() and returns the result wrapped in an expected object.
Since: C++11

