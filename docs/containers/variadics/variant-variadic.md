---
title: "variant"
---

{{< callout type="info">}}
  Header: `variant.h`  
  Similar to: `std::variant`
{{< /callout >}}

**For C++11 and above.**  

```cpp
template <typename... TTypes>
class variant
```

A variadic class that can contain one a several specified types in a type safe manner.  
C++11 and C++14 support up to 32 types for visitor callback callbacks.  
C++17 and above supports any number of visitor callback types.  

Accepts functors, `etl::overload` and `etl::visitor` types as visitor callbacks.  

Accepts `etl::visit` since `20.28.0`.

## Error exception types

```cpp
class variant_exception
```
**Description**  
The base for all variant exceptions.  
Derived from `etl::exception`.

---

```cpp
class variant_incorrect_type_exception
```
**Description**  
The error types emitted when a non-supported type is requested from the variant.

## Construction

```cpp
ETL_CONSTEXPR14 variant()
```
**Description**  
Default constructor.

---

```cpp
template <typename T>
ETL_CONSTEXPR14 variant(T&& value)
```
**Description**  
Constructor from a value.

---

```cpp
template <typename T, typename... TArgs>
ETL_CONSTEXPR14 explicit variant(etl::in_place_type_t<T>, TArgs&&... args)
```
**Description**  
Construct T from arguments.

---

```cpp
template <size_t Index, typename... TArgs>
ETL_CONSTEXPR14 explicit variant(etl::in_place_index_t<Index>, TArgs&&... args)
```
**Description**  
Construct from Index using  args.

---

```cpp
template <typename T, typename U, typename... TArgs>
ETL_CONSTEXPR14 explicit variant(etl::in_place_type_t<T>, 
                                 std::initializer_list<U> init, 
                                 TArgs&&... args)
```
**Description**  
Construct from type, `initializer_list` and arguments.

---

```cpp
template <size_t Index, typename U, typename... TArgs>
ETL_CONSTEXPR14 explicit variant(etl::in_place_index_t<Index>, 
                                 std::initializer_list<U> init, 
                                 TArgs&&... args)
```
**Description**  
Construct from `Index`, initializer_list and arguments.

---

```cpp
ETL_CONSTEXPR14 variant(const variant& other)
```
**Description**  
Copy constructor.

---

```cpp
ETL_CONSTEXPR14 variant(variant&& other)
```
**Description**  
Move constructor.

## Operations

```cpp
template <typename T, typename... TArgs>
T& emplace(TArgs&&... args)
```
**Description**  
Set to type `T` using the construction parameters args.

---

```cpp
template <typename T>
variant& operator =(T&& value)
```
**Description**  

---    

```cpp
variant& operator =(const variant& other)
```
**Description**  
Copies other to the current variant.

---

```cpp
variant& operator =(variant&& other)
```
**Description**  
Moves other to the current variant.

---

```cpp
constexpr bool valueless_by_exception() const noexcept
```
**Description**  
Returns `true` if the type id is `etl::variant_npos`.

---

```cpp
constexpr size_t index() const noexcept
```
**Description**  
Returns the index of the type contained in the variant.  
Returns `etl::variant_npos` if the variant does not hold a valid type.

---

```cpp
void swap(variant& rhs) noexcept
```
**Description**  
Swaps rhs with the current variant.

## Access

**Non-member functions**  

### get

```cpp
template <size_t Index, typename... TTypes>
ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<TTypes...>>&
  get(etl::variant<TTypes...>& v)
```
**Description**  
Get the value based on `Index`.  
If `Index` matches the current index of the variant, returns a reference to the stored value, otherwise, asserts `etl::variant_incorrect_type_exception`.

**Parameters**  
A reference to a variant.

---

```cpp
template <size_t Index, typename... TTypes>
ETL_CONSTEXPR14 etl::variant_alternative_t<Index, etl::variant<TTypes...>>&&
  get(etl::variant<TTypes...>&& v)
```
**Description**  
Get the value based on `Index`.  
If `Index` matches the current index of the variant, returns a reference to the stored value, otherwise, asserts `etl::variant_incorrect_type_exception`.

**Parameters**  
An rvalue reference to a variant.

---

```cpp
template <size_t Index, typename... TTypes>
ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<TTypes...>>&
  get(const etl::variant<TTypes...>& v)
```
**Description**  
Get the value based on `Index`.  
If `Index` matches the current index of the variant, returns a reference to the stored value, otherwise, asserts `etl::variant_incorrect_type_exception`.

**Parameters**  
A const reference to a variant.

---

```cpp
template <size_t Index, typename... TTypes>
ETL_CONSTEXPR14 const etl::variant_alternative_t<Index, const etl::variant<TTypes...>>&&
  get(const etl::variant<TTypes...>&& v)
```
**Description**  
Get the value based on `Index`.  
If `Index` matches the current index of the variant, returns a reference to the stored value, otherwise, asserts `etl::variant_incorrect_type_exception`.

**Parameters**  
A const rvalue reference to a variant.

---

```cpp
template <typename T, typename... TTypes>
ETL_CONSTEXPR14 T& get(etl::variant<TTypes...>& v)
```
**Description**  
Get the value based on `T`.  
If `T` matches the current type stored in the variant, returns a reference to the stored value, otherwise, asserts `etl::variant_incorrect_type_exception`.

**Parameters**  
A reference to a variant.

---

```cpp
template <typename T, typename... TTypes>
ETL_CONSTEXPR14 T&& get(etl::variant<TTypes...>&& v)
```
**Description**  
Get the value based on `T`.  
If `T` matches the current type stored in the variant, returns a reference to the stored value, otherwise, asserts `etl::variant_incorrect_type_exception`.

**Parameters**  
An rvalue reference to a variant.

---

```cpp
template <typename T, typename... TTypes>
ETL_CONSTEXPR14 const T& get(const etl::variant<TTypes...>& v)
```
**Description**  
Get the value based on `T`.  
If `T` matches the current type stored in the variant, returns a reference to the stored value, otherwise, asserts `etl::variant_incorrect_type_exception`.

**Parameters**  
A const reference to a variant.

---

```cpp
template <typename T, typename... TTypes>
ETL_CONSTEXPR14 const T&& get(const etl::variant<TTypes...>&& v)
```
**Description**  
Get the value based on `T`.  
If `T` matches the current type stored in the variant, returns a reference to the stored value, otherwise, asserts `etl::variant_incorrect_type_exception`.

**Parameters**  
A const rvalue reference to a variant.

### get_if

```cpp
template <size_t  Index, typename... TTypes>
ETL_CONSTEXPR14 etl::add_pointer_t<etl::variant_alternative_t<Index, etl::variant<TTypes...>>>
  get_if(etl::variant<TTypes...>* pv) noexcept
```
**Description**  
Get the value based on `Index`.  
If `Index` matches the current index of the variant, returns a pointer to the stored value, otherwise returns `nullptr`.

**Parameters**  
A pointer to a variant.

---

```cpp
template <size_t Index, typename... TTypes>
ETL_CONSTEXPR14 etl::add_pointer_t<const etl::variant_alternative_t<Index, etl::variant<TTypes...>>>
  get_if(const etl::variant<TTypes...>* pv) noexcept
```
**Description**  
Get the value based on `Index`.  
If `Index` matches the current index of the variant, returns a const pointer to the stored value, otherwise returns `nullptr`.

**Parameters**  
A const pointer to a variant.

---

```cpp
template <typename T, typename... TTypes>
ETL_CONSTEXPR14 etl::add_pointer_t<T> get_if(etl::variant<TTypes...>* pv) noexcept
```
**Description**  
Get the value based on `T`.  
If `T` matches the current type stored in the variant, returns a pointer to the stored value, otherwise return `nullptr`.

**Parameters**  
An pointer to a variant.

---


```cpp
template <typename T, typename... TTypes>
ETL_CONSTEXPR14 etl::add_pointer_t<const T> get_if(const etl::variant<TTypes...>* pv) noexcept
```
**Description**  
Get the value based on `T`.  
If `T` matches the current type stored in the variant, returns a const pointer to the stored value, otherwise return `nullptr`.

**Parameters**  
An const pointer to a variant.

## Other

```cpp
struct monostate
```
**Description**  
A default constructible type.  
For use as the first state to turn a non-default constructible variant into a default constructible variant.

---

```cpp
template <typename... TTypes>
void swap(etl::variant<TTypes...>& lhs, etl::variant<TTypes...>& rhs)
```
**Description**  
Swaps lhs and rhs.

---

```cpp
template <typename... TTypes>
struct variant_size<etl::variant<TTypes...>>
```
**Description**  
The member constant value contains the number of types.

For C++17 and above.
```cpp
template <typename... TTypes>
inline constexpr size_t variant_size_v = variant_size<TTypes...>::value;
```
**Description**  
Has the value equal to the  number of types.

## Tests
### Non-member functions
```cpp
template <typename T, typename... TTypes>
ETL_CONSTEXPR14 bool holds_alternative(const etl::variant<TTypes...>& v) noexcept
```
**Description**  
Checks if the variant v holds the alternative T.

---

```cpp
template <size_t Index, typename... TTypes>
ETL_CONSTEXPR14 bool holds_alternative(const etl::variant<TTypes...>& v) noexcept
```
**Description**  
Checks if the variant v holds the alternative Index.

---

```cpp
template <typename... TTypes>
ETL_CONSTEXPR14 bool holds_alternative(size_t index, const etl::variant<TTypes...>& v) noexcept
```
**Description**  
Checks if the variant v holds the alternative index.

## Visitation
For `etl::visitor`

For C++11 and C++14 the maximum number of visitor types is 32.
Code size may be minimised if one of the following macros are defined.

```cpp
ETL_VARIANT_CPP11_MAX_8_TYPES
ETL_VARIANT_CPP11_MAX_16_TYPES
ETL_VARIANT_CPP11_MAX_24_TYPES
```
**Description**  
C++17 and above support any number of visitor callback types.

`etl::variant` provides member functions that support two methods of visitation.

---

```cpp
void accept_visitor(etl::visitor<TTypes...>& v)
```
**Description**  
Accepts an `etl::visitor` derived type.  
Deprecated from `20.36.0`

```cpp
void accept_visitor(etl::visitor<TTypes...>& v) const
```
**Description**  
Accepts `an etl::visitor` derived type.  
Deprecated  
Since: `20.36.0`

---

```cpp
void accept(etl::visitor<TTypes...>& v)
```
**Description**  
Accepts an `etl::visitor` derived type.
Since: `20.36.0`

---

```cpp
void accept(etl::visitor<TTypes...>& v) const
```
**Description**  
Accepts an `etl::visitor` derived type.  
Since: `20.36.0`

See `visitor`

---

```cpp
template <typename TVisitor>
void accept_functor(TVisitor& v)
```
**Description**  
Accepts a functor that overrides `operator()` for each type.  
Deprecated since `20.36.0`

---

```cpp
template <typename TVisitor>
void accept_functor(TVisitor& v) const
```
**Description**  
Accepts a functor that overrides `operator()` for each type.  
Deprecated since `20.36.0`

---

```cpp
template <typename TVisitor>
void accept(TVisitor& v)
```
**Description**  
Accepts a functor that overrides `operator()` for each type.
Since: `20.36.0`

---

```cpp
template <typename TVisitor>
void accept(TVisitor& v) const
```
**Description**  
Accepts a functor that overrides `operator()` for each type.
Since: `20.36.0`

See overload

## Visitation
For STL style `etl::visit`.
Since: `20.28.0`

The ETL implements an STL style visit function.
