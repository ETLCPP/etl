---
title: "instance_count"
---

{{< callout type="info">}}
  Header: `instance_count.h`  
  Since: `TBC`  
{{< /callout >}}

Inherit from this class to count instances of a class.

Before: `20.25.0`
```cpp
etl::instance_count<typename T>
```
This class is not thread safe.

From: `20.25.0`
```cpp
etl::instance_count<typename T, typename TCounter = uint32_t>
```
This class is not thread safe unless `TCounter` is an atomic type.

## Types

`type`         The type being counted.
`counter_type` The type used for the counter.

## Member functions
```cpp
static const counter_type& get_instance_count()
```
Returns a const reference to the current count.

---

```cpp
static void reset_instance_count()
```
Resets the count to zero.

## Example
Instances of `My_Class` will be counted and can be accessed by the inherited member function `get_instance_count()`.

```cpp
class My_Class : public etl::instance_count<My_Class, std::atomic_uint8_t>
{
};
```
