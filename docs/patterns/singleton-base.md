---
title: "singleton_base"
---

{{< callout type="info">}}
  Header: `singleton_base.h`  
{{< /callout >}}

Allows creation of a singleton base class.  

```cpp
etl::singleton_base<typename T>
```
`T` The type to use as a singleton.  

The class derived derived from this must call the constructor with the instance of itself.  
Multiple calls to the constructor will result in an `etl::singleton_base_already_created` assertion.  

Before the constructor Is called, the singleton is in the invalid state.  

See also `etl::singleton`

## Static functions

```cpp
static T& instance()
```
Returns a reference to the one instance of `T`.  
Asserts an `etl::singleton_base_not_created` if the instance has not been attached.

---

```cpp
static bool is_valid()
```
Returns `true` if the instance is has been attached, otherwise `false`.

## Example

```cpp
class MyType : public etl::singleton_base<MyType>
{
 MyType()
   : etl::singleton_base<MyType>(*this)
  {
  }
};

bool is_valid;
is_valid = MyType::is_valid();   // true

MyType& mt = MyType::instance(); // Get the instance
```
