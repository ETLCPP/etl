---
title: "tuple"
---

{{< callout type="info">}}
  Header: `tuple.h`  
  From: `20.41.0`  
  Similar to: `std::tuple`
{{< /callout >}}

A fixed-size collection of heterogeneous values.  

Adds additional functionality to std::tuple.

```cpp
etl::tuple<typename... Types>
```

## Template deduction guides
C++17 and above.  

**Template deduction guide from variadic arguments.**  
```cpp
template <typename... TArgs>
tuple(TArgs... args) -> tuple<TArgs...>;
```

**Template deduction guide from pair.**  
```cpp
template <typename T1, typename T2>
tuple(ETL_OR_STD::pair<T1, T2>) -> tuple<T1, T2>;
```

**Example**  
```cpp
etl::tuple tp(1, 2.2, 3, std::string("4"));
```

Equivalent to:  
```cpp
etl::tuple<int, double, int, std::string> t(1, 2.2, 3, std::string("4"));
```

## Make tuple

```cpp
template <typename... Types>
ETL_NODISCARD
ETL_CONSTEXPR14
etl::tuple<etl::unwrap_ref_decay_t<Types>...> make_tuple(Types&&... args)
```

**Example**  
```cpp
auto t = etl::make_tuple(1, 2.2, 3, std::string("4"));
```

Equivalent to:  
```cpp
etl::tuple<int, double, int, std::string> t(1, 2.2, 3, std::string("4"));
```

---

## Member types

`value_type`  
&emsp; The first type contained by this tuple.  
`this_type`  
&emsp; The type of this tuple.  
`base_type`  
&emsp; The type of the base tuple to this tuple. This is a tuple of all types except for the first.  
`index_sequence_type`  
&emsp; The index_sequence type for this tuple.  

## Constructor

Default or copy constructs each element.  
Can be initialised like a C array.  

```cpp
etl::array<int, 10> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
```

### Access

```cpp
template <size_t Index, typename Tuple>
etl::tuple_element;
```

## Capacity

```cpp
template <typename Tuple>
struct tuple_size;
```

## ETL Extensions

```cpp
template <typename T>
struct is_tuple
```
If `T` is a `tuple` then `etl::is_tuple` is defined as `etl::true_type`, otherwise `etl::false_type`.

### Specialisations of STL to allow the use of C++ structured bindings.

```cpp
template <typename... Types>
struct tuple_size<etl::tuple<Types...>>
```
Specialisation of tuple_size to allow the use of C++ structured bindings.  
Declared in namespace `std`.

```cpp
template <size_t Index, typename... Types>
struct tuple_element<Index, etl::tuple<Types...>>
```
Specialisation of `tuple_element` to allow the use of C++ structured bindings.  
Declared in namespace `std`.

### Convert an `etl::tuple` to a `std::tuple`

```cpp
template <typename... Types>
ETL_NODISCARD
ETL_CONSTEXPR14
auto to_std(const etl::tuple<Types...>& etl_tuple)
```
Converts an `etl::tuple` to a `std::tuple`.  
Enabled when STL is available.

---

```cpp
template <typename... Types>
ETL_NODISCARD
ETL_CONSTEXPR14
auto to_std(etl::tuple<Types...>&& etl_tuple)
```
Converts an `etl::tuple` to a `std::tuple`.  
Enabled when STL is available.

### Convert an std::tuple to a etl::tuple

```cpp
template <typename... Types>
ETL_NODISCARD
ETL_CONSTEXPR14
auto to_etl(const std::tuple<Types...>& std_tuple)
```
Converts a `std::tuple` to an `etl::tuple`.  
Enabled when STL is available.

---

```cpp
template <typename... Types>
ETL_NODISCARD
ETL_CONSTEXPR14
auto to_etl(std::tuple<Types...>&& std_tuple)
```
Converts a `std::tuple` to an `etl::tuple`.  
Enabled when STL is available.

### Create a tuple from selected elements of another

```cpp
template <typename Tuple, size_t... Indices>
ETL_NODISCARD
ETL_CONSTEXPR14
auto select_from_tuple(Tuple&& tuple, etl::index_sequence<Indices...>)
```
Creates a new tuple by selecting elements from another, given a run time index sequence.  
Static asserts if the number of indices does not match the tuple size.

---

```cpp
template <size_t... Indices, typename Tuple>
ETL_NODISCARD
ETL_CONSTEXPR14
auto select_from_tuple(Tuple&& tuple)
```
Creates a new tuple by selecting elements from another, given a template parameter index sequence.  
Static asserts if the number of indices does not match the tuple size.

## Non-member functions

```cpp
template <size_t Index, typename... Types>
ETL_CONSTEXPR14
etl::tuple_element_t<Index, etl::tuple<Types...>>& get(tuple<Types...>&);
```

---

```cpp
template <size_t Index, typename... Types>
ETL_CONSTEXPR14 
etl::tuple_element_t<Index, etl::tuple<Types...>>&& get(tuple<Types...>&&);
```

---

```cpp
template <size_t Index, typename... Types>
ETL_CONSTEXPR14 
const etl::tuple_element_t<Index, etl::tuple<Types...>>& get(const tuple<Types...>&);
```

---

```cpp 
template <size_t Index, typename... Types>
ETL_CONSTEXPR14 
const etl::tuple_element_t<Index, etl::tuple<Types...>>&& get(const tuple<Types...>&&);
```

---

```cpp
template <typename T, typename... Types>
ETL_CONSTEXPR14
T& get(tuple<Types...>&);
```

---

```cpp
template <typename T, typename... Types>
ETL_CONSTEXPR14
T&& get(tuple<Types...>&&);
```

---

```cpp
template <typename T, typename... Types>
ETL_CONSTEXPR14
const T& get(const tuple<Types...>&);
```

---

```cpp
template <typename T, typename... Types>
ETL_CONSTEXPR14
const T&& get(const tuple<Types...>&&);
```
