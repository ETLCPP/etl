---
title: "Page Template"
draft: true
weight: 1
---

{{< callout type="info">}}
  Header: `my_class.h`  
  Supported: `xx.yy.zz`  
  Similar to: [Some similar in the STL](https://en.cppreference.com/w/cpp/stl_header.html)
{{< /callout >}}

A short description of the code this document is about.

```cpp
The signature of the class, struct, or function
```

## Template Parameters

```cpp
TParameter1
```
**Description**  
A description the the TParameter1 template parameter.  

---

```cpp
TParameter1
```
**Description**  
A description the the TParameter1 template parameter.  

## Exceptions

```cpp
etl::your_firt_exception_type
```
**Description**  
Description of what the exception indicates.

## Member Types

```cpp
`first_member_type`
```
**Description**  
The first member type

```cpp
return_type
```
**Description**
`The second class member type`

```cpp
argument_types
```
**Description**  

## Constructors

`my_class()`

**Description**  
Default constructor.

**Parameters**  
`void`

---

`my_class(const my_class&)`

**Description**  
Copy constructor.

**Parameters**  
Const reference to etl::my_class.

---

`my_class(my_class&&)`

**Description**  
Move constructor.

**Parameters**  
rvalue reference to etl::my_class.

## Invocation

```cpp
int operator(float f)
```
**Description**  
Invokes the function operator.

**Parameters**  
`f` The floating point parameter.

**Returns**  
An integer calculated from the float.

---

```cpp
int calculate(float f)
```
**Description**  
Calculates an integer from a float.

**Parameters**  
`f` The floating point parameter.

**Returns**  
An integer calculated from the float.

## Observers

```cpp
bool is_valid() const
```
**Returns**  
`true` if the object is valid.

## Modifiers

```cpp
void clear()
```
**Description**  
Clears the object.  

**Parameters**  
None  

**Returns**  
`void`

---

```cpp
void swap(my_class& other)
```
**Description**  
Swaps with another my_class object.

**Parameters**  
A reference to another my_class object.

**Returns**  
`void`

## Example

```cpp
#include "etl/my_class.h"

etl::my_class my_object1;
etl::my_class my_object2;

int main()
{
  my_object1.swap(my_object2);

  if (my_object.is_valid())
  {
    return my_object1(1.23f);
  }
  else
  {
    return 0;
  }
}
```

## Notes

Some other notes about `etl::my_class`.
