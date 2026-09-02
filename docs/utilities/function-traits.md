---
title: "function_traits"
---

{{< callout type="info">}}
  Header: `function_traits.h`  
  Since: `TBC`  
{{< /callout >}}

Allows the traits of functions to be determined.

```cpp
template <typename TReturn, typename... TArgs>
struct function_traits<TReturn(*)(TArgs...)>
```

## Member types

`function_type`  
The signature of the function.  
`TReturn(TArgs...)`  

---

`return_type`  
The return type.  
`TReturn`  

---

`object_type`  
The object type, if a member function, otherwise `void`.  

---

`argument_types`  
An `etl::type_list` of the arguments.  
`etl::type_list<TArgs...>`  

## Member constants

`is_function`  
`true` if a free or static member function.  
`bool`  

---

`is_member_function`  
`true` if a member function.  
`bool`  

---

`is_const`  
`true` if a const member function.  
`bool`  

---

`argument_count`  
The number of arguments.  
`size_t`  

---

**Example**

```cpp
int free_int(int i, int j);

class Object
{
public:
  int member_int(int i, int j);
  int member_int_const(int i, int j) const;
  static void member_static(int i, int j);
};

using traits1 = etl::function_traits<decltype(&free_int)>;
```

```cpp
function_type      int(int, int)
return_type        int
object_type        void
argument_types     etl::type_list<int, int>
is_function        true
is_member_function false
is_const           false
argument_count     2
```

---

```cpp
using traits2 = etl::function_traits<decltype(&Object::member_int)>;
```

```cpp
function_type      int(int, int)
return_type        int
object_type        Object
argument_types     etl::type_list<int, int>
is_function        false
is_member_function true
is_const           false
argument_count     2
```

---

```cpp
using traits3 = etl::function_traits<decltype(&Object::member_int_const)>;
```
```cpp
function_type      int(int, int) const
return_type        int
object_type        Object
argument_types     etl::type_list<int, int>
is_function        false
is_member_function true
is_const           true
argument_count     2
```

---

```cpp
using traits4 = etl::function_traits<decltype(&Object::member_static)>;
```
```cpp
function_type      void(int, int)
return_type        void
object_type        void
argument_types     etl::type_list<int, int>
is_function        true
is_member_function false
is_const           false
argument_count     2
```
