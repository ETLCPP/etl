---
title: "optional"
---

{{< callout type="info">}}
  Header: `optional.h`  
  Similar to: `std::optional`
{{< /callout >}}

A class where the value is optional.  
If a value is not assigned then an object of the contained type is not constructed.

```cpp
template <typename T>
etl::optional<T>
```

If `_DEBUG` is defined then accesses to invalid data is detected either by throwing (`ETL_THROW_EXCEPTIONS` defined) or calling the error handler.

## Template deduction guides
C++17 and above

```cpp
template <typename T>
etl::optional(T)
  -> etl::optional<T>
```

### Example

```cpp
std::string text = "Hello";
etl::optional opt{ text };
```

Defines `opt` as containing a `std::string`, containing the supplied text.

## Constructor

```cpp
optional()
```
**Description**  
Default constructor.

---

```cpp
optional(const T& value)
optional(T&& value)
```
**Description**  
Construct from value. Constructs a `T`.

---

```cpp
optional(const optional<T>& other)
optional(optional<T>&& other)
```
**Description**  
Construct copy.  Constructs a `T`.

---

```cpp
optional(nullopt)
```
**Description**  
Construct empty value (same as default constructor).

---

```cpp
template <typename... TArgs>
ETL_CONSTEXPR14
optional_impl(etl::in_place_t, TArgs&&... args)
```
**Description**  
Constructor from variadic args.
20.43.0

---

```cpp
template <typename U, typename... TArgs>
ETL_CONSTEXPR14
optional_impl(etl::in_place_t, std::initializer_list<U> ilist, TArgs&&... args)
```
**Description**  
Constructor from variadic args.
20.43.0

---

```cpp
~optional()
```
**Description**  
Destructor.  
Destructs a `T` if a value has been assigned.

## Operations

```cpp
optional& operator =(etl::nullopt)
```
**Description**  
Clears the optional.

```cpp
optional& operator =(const optional<T>& value)
optional& operator =(optional<T>&& value)
```
**Description**  
Copy constructor.

```cpp
optional& operator =(const T& value)
optional& operator =(T&& value)
```
**Description**  
Assigns a value to the optional.

### Note
Unlike `std::optional`, assigning from `{ }` will not clear the optional item to 'empty'.
It will be assigned a default constructed 'T'.

```cpp
etl::optional<int> opt;
opt = { }; // opt will contain 0.
```

---

```cpp
void swap(optional& other)
```
**Description**  
Swaps with the `other` optional

---

```cpp
void reset()
```
**Description**  
Resets the optional back to default.

---

```cpp
emplace
```
**Description**  
Construct in-place from arguments.

### C++03
```cpp
template <typename T1>
void emplace(const T1& value1)

template <typename T1, typename T2>
void emplace(const T1& value1, const T2& value2)

template <typename T1, typename T2, typename T3>
void emplace(const T1& value1, const T2& value2, const T3& value3)

template <typename T1, typename T2, typename T3, typename T4>
void emplace(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
```

### C++11
```cpp
template <typename ... Args>
void emplace(Args && ... args)
```

## Observers

```cpp
const T& value() const
T& value()
```
**Description**  
Returns the contained value. 

---

```cpp
template <typename T>
T value_or(T value)
```
**Description**  
Returns the contained value if `*this` is valid, otherwise returns `value`. 

---

```cpp
const T* operator->() const
T* operator->()
```
**Description**  
Returns a pointer to the contained value.

---

```cpp
const T& operator *() const
T* operator *()
```
**Description**  
Returns a reference to the contained value.

## Tests

```cpp
explicit operator bool() const
bool has_value() const
```
**Description**  
Checks whether *this is in a valid state, i.e. whether the contained value is initialised.
Returns `true` if valid, otherwise `false`.

## Non-member functions

```cpp
operator ==
```
**Description**  
`true` if the contents of the lists are equal, otherwise `false`.

---

```cpp
operator !=
```
**Description**  
`true` if the contents of the lists are not equal, otherwise `false`.

---

```cpp
operator <
```
**Description**  
`true` if the contents of the lhs are lexicographically less than the contents of the rhs,  otherwise `false`.

---

```cpp
operator <=
```
**Description**  
`true` if the contents of the lhs are lexicographically less than or equal to the contents of the rhs, otherwise `false`.

---

```cpp
operator >
```
**Description**  
`true` if the contents of the lhs are lexicographically greater than the contents of the rhs,  otherwise `false`.

---

```cpp
operator >=
```
**Description**  
`true` if the contents of the lhs are lexicographically greater than or equal to the contents of the rhs, otherwise `false`.
