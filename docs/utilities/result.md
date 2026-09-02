---
title: "result"
---

{{< callout type="info">}}
  Header: `result.h`  
  From: `20.17.0`  
  Similar to: `std::expected`
{{< /callout >}}

{{< callout type="warning">}}
**This class is deprecated.**  
Use `etl::expected` as a replacement.
{{< /callout >}}

A generic result type for returning either a result or an error.
Deprecated. Use etl::expected

## result

```cpp
template <typename TValue, typename TError>
class result
```

---

```cpp
template <typename TError>
class result<void, TError>
```
**Description**  
Specialisation for `void` `result`.  

---

```c++
result() = delete;
```
**Description**  
Cannot be default constructed.

---

```cpp
result(const result& other)
```
**Description**  
Copy constructor.

---

```cpp
result(result&& other)
```
**Description**  
Move constructor.

---

```cpp
result(const TValue& value)
```
**Description**  
Construct from a value.  
Not valid for void specialisation.

---

```cpp
result(TValue&& value)
```
**Description**  
Move construct from a value.  
Not valid for void specialisation.

---

```cpp
result(const TError& err)
```
**Description**  
Construct from error.

---

```cpp
result(TError&& err)
```
**Description**  
Move construct from error.

---

```cpp
result& operator =(const result& other)
```
**Description**  
Copy assign.

---

```cpp
result& operator =(result&& other)
```
**Description**  
Move assign.

---

```cpp
result& operator =(const TValue& value)
```
**Description**  
Copy assign from value.  
Not valid for void specialisation.

---

```cpp
result& operator =(TValue&& value)
```
**Description**  
Move assign from value.  
Not valid for void specialisation.

---

```cpp
result& operator =(const TError& err)
```
**Description**  
Copy assign from error.

---

```cpp
result& operator =(TError&& err)
```
**Description**  
Move assign from error.

---

```cpp
bool is_value() const
```
**Description**  
`true` if result contains a value.

---

```cpp
bool is_error() const
```
**Description**  
`true` if result contains an error.

---

```cpp
const TValue& value() const
```
**Description**  
Returns a const reference to the value.  
Undefined if the result does not contain an value.  
Not valid for void specialisation.  

---

```cpp
TValue&& value()
```
**Description**  
Returns an rvalue reference to the value.  
Undefined if the result does not contain an value.  
Not valid for void specialisation.  

---

```cpp
const TError& error() const
```
**Description**  
Returns a const reference to the error.  
Undefined if the result does not contain an error.

---

```cpp
TError&& error()
```
**Description**  
Returns an rvalue reference to the error.  
Undefined if the result does not contain an error.  
