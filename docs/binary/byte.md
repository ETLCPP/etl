---
title: "byte"
---

{{< callout type="info">}}
  Header: `byte.h`  
  Since: `20.24.0`  
  Similar to: [std::byte](https://en.cppreference.com/cpp/types/byte)
{{< /callout >}}

A type that implements the concept of byte

**C++03** 
Implemented as a class.  
Cannot be cast using `static_cast`.  

**C++11 or above**  
Implemented as enum class.  
All functions are `constexpr`.  

## Constructors
```cpp
byte()
```
**Description**
Constructs a default initialised byte.  
C++03

---

```cpp
template <typename T>
explicit byte(T v)
```
Constructs a byte initialised to `v`.

## Non-member functions
```cpp
template <typename TInteger>
constexpr TInteger to_integer(etl::byte b) noexcept
```
**Description**  
Converts to an integral type.  
`constexpr` and `noexcept` for C++11 and above.

---

```cpp
template <typename TInteger>
constexpr etl::byte operator <<(etl::byte b, TInteger shift) noexcept
```
**Description**  
Shifts the value of the byte to the left and returns the new byte.  
`constexpr` and `noexcept` for C++11 and above.

---

```cpp
template <typename TInteger>
constexpr etl::byte operator >>(etl::byte b, TInteger shift) noexcept
```
**Description**  
Shifts the value of the byte to the right and returns the new byte.  
`constexpr` and `noexcept` for C++11 and above.

---

```cpp
template <typename TInteger>
constexpr etl::byte& operator <<=(etl::byte& b, TInteger shift) noexcept
```
**Description**  
Shifts the value of the byte to the left and returns a reference to the byte.  
`constexpr` and `noexcept` for C++11 and above.

---

```cpp
template <typename TInteger>
constexpr etl::byte& operator >>=(etl::byte& b, TInteger shift) noexcept
```
**Description**  
Shifts the value of the byte to the right and returns a reference to the byte.  
`constexpr` and `noexcept` for C++11 and above.

---

```cpp
constexpr etl::byte operator |(etl::byte lhs, etl::byte rhs) noexcept
```
ORs the two bytes returns the new byte.  
`constexpr` and `noexcept` for C++11 and above.

---

```cpp
constexpr etl::byte operator &(etl::byte lhs, etl::byte rhs) noexcept
```
**Description**  
ANDs the two bytes returns the new byte.  
`constexpr` and `noexcept` for C++11 and above.

---

```cpp
constexpr etl::byte operator ^(etl::byte lhs, etl::byte rhs) noexcept
```
**Description**  
Exclusive ORs the two bytes returns the new byte.  
`constexpr` and `noexcept` for C++11 and above.

---

```cpp
constexpr etl::byte& operator |=(etl::byte& lhs, etl::byte rhs) noexcept
```
**Description**  
ORs the two bytes returns and a reference to the first parameter.  
`constexpr` for C++14 and above.  
`noexcept` for C++11 and above.

---

```cpp
constexpr etl::byte& operator &=(etl::byte& lhs, etl::byte rhs) noexcept
```
**Description**  
ANDs the two bytes returns and a reference to the first parameter.  
`constexpr` for C++14 and above.  
`noexcept` for C++11 and above.

---

```cpp
constexpr etl::byte& operator ^=(etl::byte& lhs, etl::byte rhs) noexcept
```
**Description**  
Exclusive ORs the two bytes and returns a reference to the first parameter.  
`constexpr` for C++14 and above.  
`noexcept` for C++11 and above.

---

```cpp
constexpr etl::byte operator ~(etl::byte b) noexcept
```
**Description**  
Negates the value of the byte and returns the new value.  
`constexpr` and `noexcept` for C++11 and above.
