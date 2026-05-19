---
title: "type_list"
---

{{< callout type="info">}}
  Header: `type_list.h`  
  From: `20.39.5`  
{{< /callout >}}

Defines a tuple of types, but unlike a tuple, does not contain any values.  
This is an empty class.  
Valid for C++11 and above.  

## type_list

```cpp
etl::type_select<typename... TTypes>
```
Creates a tuple of types from a set of template type parameters.

### Member types
index_sequence_type
The index_sequence type for this type_list.

### Type list creation
**Using global types**  

```cpp
template <typename TTypeList, size_t... Indices>
type_list_select
```
Defines a new type_list by selecting types from a given type_list, according to an index sequence.

---

```cpp
template <typename... TTypeLists>
type_list_cat
```
Defines a new type list by concatenating a list of `etl::type_list` objects.  
`std::tuple` style access.

## Type list properties

### Using member types

```cpp
static constexpr size_t size();
```
Returns the number of types in the type list.

### Using global types

```cpp
template <typename TTypelist>
type_list_size
```
Defines value as the size of the type list.  
tuple style access.

```cpp
template <typename TTypelist>
type_list_size_v
```
C++17 and above.

---

```cpp
etl::nth_type<size_t N, typename TTypeList>
```
Defines the nth type in the type list.

---

```cpp
template <typename TFromList, typename TToList>
type_lists_are_convertible
```
Checks that types in a type_list are convertible to the type in another.  
Defines value as `true` or `false`.  
From: `20.43.0`

---

```cpp
template <typename TFromList, typename TToList>
type_lists_are_convertible_v
```
C++17 and above.  
From: `20.43.0`

## Examples

```cpp
using TypeList1 = etl::type_list<char, short, int, long>
using TypeList2 = etl::type_list<float, double>
using TypeList3 = etl::type_list<unsigned char, unsigned short>
```

### Using member types

```cpp
// Get the size of TypeList1
constexpr size_t typeList1Size = TypeList1::size();
```

### Using tuple style global types

```cpp
// Get the size of TypeList1
constexpr size_t typeList1Size = etl::type_list_size_v<TypeList1>;

// Define the type list 
// etl::type_list<char, short, int, long, float, double, unsigned char, unsigned short>
using Concatenated = etl::type_list_cat<TypeList1, TypeList2, TypeList3>
```
