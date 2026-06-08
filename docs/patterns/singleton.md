---
title: "singleton"
---

{{< callout type="info">}}
  Header: `singleton.h`  
{{< /callout >}}

Allows creation of a singleton, with deterministic construction and destruction.  

```cpp
etl::singleton<typename T>
```
`T` The type to use as a singleton.

Before create Is called, the singleton is in the invalid state.  

See also `etl::singleton_base`.

## Static functions

**C++03 and below.**  
```cpp
static void create()
```
Creates the singleton. Default constructs the instance.  
Sets the singleton to the valid state.  
Only the first call to create is effective.  
Must be called before any calls to `instance()`.

---

```cpp
template <typename T1>
static void create(const T1& p1)
```
Construct from 1 parameter.

---

```cpp
template <typename T1, typename T2>
static void create(const T1& p1, const T2& p2)
```
Construct from 2 parameters.

---

```cpp
template <typename T1, typename T2, typename T3>
static void create(const T1& p1, const T2& p2, const T3& p3)
```
Construct from 3 parameters.

---

```cpp
template <typename T1, typename T2, typename T3, typename T4>
static void create(const T1& p1, const T2& p2, const T3& p3, const T4& p4)
```
Construct from 4 parameters.

---

**C++11 and above.**  
```cpp
template <typename... TArgs>
static void create(TArgs&&... args)
```
Creates the singleton and constructs the type using the supplied parameters.  
Sets the singleton to the valid state.  
Only the first successful call to create is effective. Does nothing if the singleton is already valid.  
Must be called before any calls to `instance()`.

---

```cpp
static T& instance()
```
Returns a reference to the one instance of `T`.  
Asserts an `etl::singleton_not_created` if create has not been called.

---

```cpp
static void destroy()
```
Calls the destructor of the instance.  
Sets the singleton to the invalid state.  
Only the first successful call to destroy is effective. Does nothing if the singleton is already invalid.

---

```cpp
static bool is_valid()
```
Returns `true` if the instance is valid.  
Returns `false` before `create` and after `destroy` are called.

---

## Example

```cpp
class MyType;
using MySingleton = etl::singleton<MyType>;
bool is_valid;

is_valid = MySingleton::is_valid();    // false

MyType& mt1 = MySingleton::instance(); // Raises ETL_ASSERT etl::singleton_not_created

MySingleton::create(1, "Hello World"); // Construct with parameters
is_valid = MySingleton::is_valid();    // true

MyType& mt2 = MySingleton::instance(); // Get the instance
is_valid = MySingleton::is_valid();    // true

MySingleton::destroy();                // Destruct the instance
is_valid = MySingleton::is_valid();    // false
```
