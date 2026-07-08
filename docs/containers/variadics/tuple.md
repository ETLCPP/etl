---
title: "tuple"
---

{{< callout type="info">}}
  Header: `tuple.h`  
  Similar to: `std::tuple`
{{< /callout >}}

C++11 and above.  
A fixed-size collection of heterogeneous values.  

```cpp
template <typename... TTypes>
class tuple
```

## Traits

```cpp
template <typename T>
struct is_tuple
```
Type trait to check if a type is an `etl::tuple`

## Types
```cpp
using value_type          = THead;                                                          ///< The type contained by this tuple.
using this_type           = tuple<THead, TTail...>;                                         ///< The type of this tuple.
using base_type           = tuple<TTail...>;                                                ///< The type of the base tuple.
using type_list           = etl::type_list<THead, TTail...>;                                ///< The type list for this tuple.
using index_sequence_type = etl::make_index_sequence< number_of_types< THead, TTail...>()>; ///< The index_sequence type for this tuple.
```

## Constants
  
```cpp
inline constexpr private_tuple::ignore_t ignore;
```
From: C++17

---

```cpp
static constexpr private_tuple::ignore_t ignore;
```
Before: C++17

## Constructors

```cpp
ETL_CONSTEXPR14 
tuple()
```
**Description**  
Default constructor.

---

```cpp
ETL_CONSTEXPR14
tuple(const tuple<THead, TTail...>& other) = default;
```
**Description**  
Copy constructor.

---

```cpp
ETL_CONSTEXPR14
tuple(tuple<THead, TTail...>&& other) = default;
```
**Description**  
Move constructor.

---

```cpp
ETL_CONSTEXPR14
tuple& operator=(const tuple<THead, TTail...>& other) = default;
```
**Description**  
Copy assignment.

---

```cpp
ETL_CONSTEXPR14
tuple& operator=(tuple<THead, TTail...>&& other) = default;
```
**Description**  
Move assignment.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
tuple(tuple<UTypes...>& other)
```
**Description**  
Copy construct from lvalue reference tuple type.  
Implicit conversion.  

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are convertible to `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
explicit tuple(tuple<UTypes...>& other)
```
**Description**  
Copy construct from lvalue reference tuple type.  
Explicit conversion.

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are **not** convertible to `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
tuple(const tuple<UTypes...>& other)
```
**Description**  
Copy construct from const lvalue reference tuple type.  
Implicit conversion.  

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are convertible to `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
explicit tuple(const tuple<UTypes...>& other)
```
**Description**  
Copy construct from const lvalue reference tuple type.  
Explicit conversion.

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are **not** convertible to `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
tuple(tuple<UHead, UTail...>&& other)
```
**Description**  
Move construct from rvalue reference tuple type.
Implicit conversion

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are convertible to `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
explicit tuple(tuple<UTypes...>&& other)
```
**Description**  
Move construct from rvalue reference tuple type.
Explicit conversion.

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are **not** convertible to `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
tuple(const tuple<UHead, UTail...>&& other)
```
**Description**  
Move construct from const rvalue reference tuple type.
Implicit conversion

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are convertible to `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
explicit tuple(const tuple<UTypes...>&& other)
```
**Description**  
Move construct from const rvalue reference tuple type.
Explicit conversion.

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are **not** convertible to `TTypes`.

---

```cpp
ETL_CONSTEXPR14
tuple(const TTypes&... args)
```
**Description**  
Construct from arguments.      

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
tuple(UHead&& head, UTail&&... tail) ETL_NOEXCEPT
```
**Description**  
Construct from arguments.
Implicit conversion.

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are not convertible to `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
explicit tuple(UHead&& head, UTail&&... tail) ETL_NOEXCEPT
```
**Description**  
Construct from arguments.
Explicit conversion.

Enabled if:-
- The number of `UTypes` == number of `TTypes`.
- The number of `TTYpes` >= 1.
- `UTypes` are **not** convertible to `TTypes`.

---

```cpp
template <typename U1, typename U2,
ETL_CONSTEXPR14
tuple(ETL_OR_STD::pair<U1, U2>& p) ETL_NOEXCEPT
```
**Description**  
Construct from lvalue reference pair.
Implicit conversion.

Enabled if:-
- The number of `TTYpes` == 2.
- `U1` and `U2` are convertible to `TTypes`.

---

```cpp
template <typename U1, typename U2,
ETL_CONSTEXPR14
explicit tuple(ETL_OR_STD::pair<U1, U2>& p) ETL_NOEXCEPT
```
**Description**  
Construct from lvalue reference pair.
Explicit conversion.

Enabled if:-
- The number of `TTYpes` == 2.
- `U1` or `U2` are not convertible to `TTypes`.

---

```cpp
template <typename U1, typename U2,
ETL_CONSTEXPR14
tuple(const ETL_OR_STD::pair<U1, U2>& p) ETL_NOEXCEPT
```
**Description**  
Construct from const lvalue reference pair.
Implicit conversion.

Enabled if:-
- The number of `TTYpes` == 2.
- `U1` and `U2` are convertible to `TTypes`.

---

```cpp
template <typename U1, typename U2,
ETL_CONSTEXPR14
explicit tuple(const ETL_OR_STD::pair<U1, U2>& p) ETL_NOEXCEPT
```
**Description**  
Construct from const lvalue reference pair.
Explicit conversion.

Enabled if:-
- The number of `TTYpes` == 2.
- `U1` or `U2` are not convertible to `TTypes`.

---

```cpp
template <typename U1, typename U2,
ETL_CONSTEXPR14
tuple(ETL_OR_STD::pair<U1, U2>&& p) ETL_NOEXCEPT
```
**Description**  
Construct from rvalue reference pair.
Implicit conversion.

Enabled if:-
- The number of `TTYpes` == 2.
- `U1` and `U2` are convertible to `TTypes`.

---

```cpp
template <typename U1, typename U2,
ETL_CONSTEXPR14
explicit tuple(ETL_OR_STD::pair<U1, U2>&& p) ETL_NOEXCEPT
```
**Description**  
Construct from rvalue reference pair.
Explicit conversion.

Enabled if:-
- The number of `TTYpes` == 2.
- `U1` or `U2` are not convertible to `TTypes`.
    
---

```cpp
template <typename U1, typename U2,
ETL_CONSTEXPR14
tuple(const ETL_OR_STD::pair<U1, U2>&& p) ETL_NOEXCEPT
```
**Description**  
Construct from const rvalue reference pair.
Implicit conversion.

Enabled if:-
- The number of `TTYpes` == 2.
- `U1` and `U2` are convertible to `TTypes`.

---

```cpp
template <typename U1, typename U2,
ETL_CONSTEXPR14
explicit tuple(const ETL_OR_STD::pair<U1, U2>&& p) ETL_NOEXCEPT
```
**Description**  
Construct from const rvalue reference pair.
Explicit conversion.

Enabled if:-
- The number of `TTYpes` == 2.
- `U1` or `U2` are not convertible to `TTypes`.
    
## Assignment

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
tuple& operator=(const tuple<UTypes...>& other)
```
**Description**  
Copy assign from other tuple type.  

Enabled if:-
- The number of `UTypes` == number of `TTypes`.

---

```cpp
template <typename... UTypes>
ETL_CONSTEXPR14
tuple& operator=(tuple<UTypes...>&& other)
```
**Description**  
Move assign from other tuple type.  

Enabled if:-
- The number of `UTypes` == number of `TTypes`.

---

```cpp
template <typename U1, typename U2>
ETL_CONSTEXPR14
tuple& operator=(ETL_OR_STD::pair<U1, U2>& p)
```
**Description**  
Assign from lvalue pair tuple type.  

Enabled if:-
- The number of `TTYpes` == 2.

---

```cpp
template <typename U1, typename U2>
ETL_CONSTEXPR14
tuple& operator=(const ETL_OR_STD::pair<U1, U2>& p)
```
**Description**  
Assign from const lvalue pair tuple type.  

Enabled if:-
- The number of `TTYpes` == 2.

---

```cpp
template <typename U1, typename U2>
ETL_CONSTEXPR14
tuple& operator=(ETL_OR_STD::pair<U1, U2>&& p)
```
**Description**  
Assign from rvalue pair tuple type.  

Enabled if:-
- The number of `TTYpes` == 2.

---

```cpp
template <typename U1, typename U2>
ETL_CONSTEXPR14
tuple& operator=(const ETL_OR_STD::pair<U1, U2>&& p)
```
**Description**  
Assign from const rvalue pair tuple type.  

Enabled if:-
- The number of `TTYpes` == 2.

## Other

```cpp
ETL_CONSTEXPR14
void swap(this_type& other)
```
**Description**  
Swaps this tuple with another.  

---

```cpp
ETL_NODISCARD
constexpr static size_t size()
```
**Description**  
Returns the number of elements in the tuple.  

## Template deduction guides
For C++17 and above.

```cpp
template <typename... TArgs>
tuple(TArgs... args) -> tuple<TArgs...>;
```
**Description**  
From variadic arguments.  

---

```cpp
template <typename T1, typename T2>
tuple(ETL_OR_STD::pair<T1, T2>) -> tuple<T1, T2>;
```
From a `pair`.

## Utility classes

```cpp
  template <size_t Index, typename... TTypes>
  struct tuple_element<Index, etl::tuple<TTypes...>>
```
**Description**  
Gets the element type at `Index` in the tuple.

---

```cpp
template <typename... TTypes>
struct tuple_size<etl::tuple<TTypes...>>
```
**Description**  
Gets the size of the tuple.  
Derived from `etl::integral_constant<size_t, sizeof...(TTypes)>`.

---

```cpp
template <typename... TTypes>
struct common_type<etl::tuple<TTypes...>>
```
**Description**  
Gets the common type of a tuple.

## Get an element

```cpp
template <size_t Index, typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::tuple_element_t<Index, etl::tuple<TTypes...>>& get(tuple<TTypes...>& t)
```
**Description**  
Extracts the element at `Index` from the tuple.  
`Index` must be an integer value in [`0`, `sizeof...(TTypes)`).  
Returns a reference to the value.  

---

```cpp
template <size_t Index, typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14 
const etl::tuple_element_t<Index, etl::tuple<TTypes...>>& get(const tuple<TTypes...>& t)
```
**Description**  
Extracts the element at `Index` from the tuple.  
`Index` must be an integer value in [`0`, `sizeof...(TTypes)`).  
Returns a const reference to the value.

---

```cpp
template <size_t Index, typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::tuple_element_t<Index, etl::tuple<TTypes...>>&& get(tuple<TTypes...>&& t)
```
**Description**  
Extracts the element at `Index` from the tuple.  
`Index` must be an integer value in [`0`, `sizeof...(TTypes)`).  
Returns an rvalue reference.

---

```cpp
template <size_t Index, typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
const etl::tuple_element_t<Index, etl::tuple<TTypes...>>&& get(const tuple<TTypes...>&& t)
```
**Description**  
Extracts the element at `Index` from the tuple.  
`Index` must be an integer value in [`0`, `sizeof...(TTypes)`).  
Returns a const rvalue reference.

---

```cpp
template <typename T, typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
T& get(tuple<TTypes...>& t)
```
**Description**  
Extracts the element with type `T` from the tuple.  
Static asserts if the tuple contains more than one `T`, or does not contain a `T` element.  
Returns a reference to the value.

---

```cpp
template <typename T, typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
const T& get(const tuple<TTypes...>& t)
```
**Description**  
Extracts the element with type `T` from the tuple.  
Static asserts if the tuple contains more than one `T`, or does not contain a `T` element.  
Returns a const reference to the value.

---

```cpp
template <typename T, typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
T&& get(tuple<TTypes...>&& t)
```
**Description**  
Extracts the element with type `T` from the tuple.  
Static asserts if the tuple contains more than one `T`, or does not contain a `T` element.  
Returns an rvalue reference to the value.

---

```cpp
template <typename T, typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
const T&& get(const tuple<TTypes...>&& t)
```
**Description**  
Extracts the element with type `T` from the tuple.
Static asserts if the tuple contain more than one `T`, or does not contain a `T` element.  
Returns a const rvalue reference to the value.

---

```cpp
template <typename... TTypes>
ETL_CONSTEXPR
etl::tuple<TTypes&...> tie(TTypes&... args)
```
**Description**  
Create a tuple of references to the provided arguments.

---

```cpp
template <typename... TTypes>
ETL_NODISCARD  
ETL_CONSTEXPR14  
etl::tuple<etl::unwrap_ref_decay_t<TTypes>...> make_tuple(TTypes&&... args)
```
**Description**  
Create a tuple from the provided arguments.

---

```cpp
template <typename TTuple, size_t... Indices>
ETL_NODISCARD  
ETL_CONSTEXPR14  
auto select_from_tuple(TTuple&& tuple,
                       etl::index_sequence<Indices...>) 
                       -> etl::tuple<etl::tuple_element_t<Indices, etl::decay_t<TTuple>>...>
```
**Description**  
Create a new tuple by selecting elements from another, given a run time index sequence.  
Static asserts if the number of indices does not match the tuple size.

---

```cpp
template <size_t... Indices, typename TTuple>
ETL_NODISCARD  
ETL_CONSTEXPR14 
auto select_from_tuple(TTuple&& tuple) 
                       -> etl::tuple<etl::tuple_element_t<Indices, etl::decay_t<TTuple>>...>
```
**Description**  
Create a new tuple by selecting elements from another, given a template parameter index sequence.  
Static asserts if the number of indices does not match the tuple size.

---

```cpp
template <typename... TTypes>
ETL_NODISCARD  
ETL_CONSTEXPR14  
etl::tuple<TTypes&&...> forward_as_tuple(TTypes&&... args)
```
**Description**  
Forward the arguments as a tuple.

---

```cpp
template <typename... Tuples>
ETL_NODISCARD
ETL_CONSTEXPR14
auto tuple_cat(Tuples&&... t) -> Tuple
```
**Description**  
Concatenate the list of tuples.

---

 ## If using the STL

Converts an etl::tuple to a std::tuple.
  
```cpp
template <typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
auto to_std(const etl::tuple<TTypes...>& etl_tuple) -> std::tuple<etl::decay_t<TTypes>...>
```
**Description**  
Converts a `const etl::tuple` to a `std::tuple`.

---

```cpp
template <typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
auto to_std(etl::tuple<TTypes...>&& etl_tuple) -> std::tuple<etl::decay_t<TTypes>...>
```
**Description**  
Converts an `etl::tuple` to a `std::tuple`.

---

```cpp
template <typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
auto to_etl(const std::tuple<TTypes...>& std_tuple) -> etl::tuple<etl::decay_t<TTypes>...>
```
**Description**  
Converts a `const std::tuple` to an `etl::tuple`.

---

```cpp
template <typename... TTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
auto to_etl(std::tuple<TTypes...>&& std_tuple) -> etl::tuple<etl::decay_t<TTypes>...>
```
**Description**  
Converts a `std::tuple` to an `etl::tuple`.

## Comparisons

```cpp
template <typename... TTypes, typename... UTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
bool operator==(const etl::tuple<TTypes...>& lhs, const etl::tuple<UTypes...>& rhs)
```
**Description**  
Equality operator.

---

```cpp
template <typename... TTypes, typename... UTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
bool operator!=(const etl::tuple<TTypes...>& lhs, const etl::tuple<UTypes...>& rhs)
```
**Description**  
Inequality operator.

---

```cpp
template <typename... TTypes, typename... UTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
bool operator<(const etl::tuple<TTypes...>& lhs, const etl::tuple<UTypes...>& rhs)
```
**Description**  
Less than operator.

---

```cpp
template <typename... TTypes, typename... UTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
bool operator<=(const etl::tuple<TTypes...>& lhs, const etl::tuple<UTypes...>& rhs)
```
**Description**  
Less than or equals operator.

---

```cpp
template <typename... TTypes, typename... UTypes>
ETL_NODISCARD
ETL_CONSTEXPR14
bool operator>(const etl::tuple<TTypes...>& lhs, const etl::tuple<UTypes...>& rhs)
```
**Description**  
Greater than operator.

---

```cpp
template <typename... TTypes, typename... UTypes>
ETL_NODISCARD ETL_CONSTEXPR14 bool operator>=(const etl::tuple<TTypes...>& lhs, const etl::tuple<UTypes...>& rhs)
```
**Description**  
Greater than or equals operator.

## Swap

```cpp
template <typename... TTypes>
ETL_CONSTEXPR14
void swap(etl::tuple<TTypes...>& lhs, etl::tuple<TTypes...>& rhs)
```
**Description**  
Swap two tuples.

## Helpers

```cpp
template <typename... TTypes>
struct tuple_from_type_list<etl::type_list<TTypes...>>
```
**Description**  
Helper to turn `etl::type_list<TList>` into `etl::tuple<TList>`

---

```cpp
template <typename TTypeList>
using tuple_from_type_list_t = typename tuple_from_type_list<TTypeList>::type
```

## STL namespace definitions

```cpp
template <typename... Types>
struct tuple_size<etl::tuple<Types...>> : etl::integral_constant<size_t, sizeof...(Types)>
```
**Description**  
Specialisation of `tuple_size` to allow the use of C++ structured bindings.

---

```cpp
template <size_t Index, typename... Types>
struct tuple_element<Index, etl::tuple<Types...>>
```
**Description**  
Specialisation of `tuple_element` to allow the use of C++ structured bindings.
