---
title: "type_list"
---

{{< callout type="info">}}
  Header: `type_list.h`  
  Since: `20.39.5`  
  From: `C++11`
{{< /callout >}}

## type_list
```cpp
template <typename... Typelists>
struct type_list
```
**Description**  
Defines a tuple of types, but unlike a tuple, does not contain any values.  
Also defines a range of compile time algorithms for use with `etl::type_list` template meta-programming.  
Valid for C++11 and above, except where specifically noted.  

## is_type_list
```cpp
template <typename T>
struct is_type_list
```
**Description**  
Check if a type is an `etl::type_list`.  

**Return**  
`etl::true_type` if `T` is an `etl::type_list`, otherwise `etl::false_type`.

---

```cpp
template <typename T>
constexpr bool is_type_list_v
```
**Return**  
`true` if `T` is an `etl::type_list`, otherwise `false`.  
From: `C++17`

## type_list_size
```cpp
template <typename TTypes>
struct type_list_size
```
**Description**  
'value' is defined as the number of types in the `type_list`.

---

```cpp
template <typename... TTypes>
constexpr size_t type_list_size_v
```
Defined as the number of types in the `type_list`.
From: C++17

## type_list_type_at_index
```cpp
template <typename TTypeList, size_t Index>
struct type_list_type_at_index
```
**Description**  
Defines `type` as the type found at `Index` in the `type_list`.  
Static asserts if `Index` is out of range.

---

```cpp
template <typename TTypeList, size_t Index>
type_list_type_at_index_t
```
**Description**  
Defined as the type found at `Index` in the `type_list`.  
Static asserts if `Index` is out of range.

## type_list_index_of_type
```cpp
template <typename TTypeList, typename T>
struct type_list_index_of_type
```
**Description**  
Defines an integral constant `value` that is the index of the *first* instance of `T` in the `type_list`.  
If the type is not in the `type_list`, then defined as `etl::type_list_npos`.  
Useful for type lists that do not contain duplicates, otherwise use `type_list_indices_of_type`.  
Static asserts if `TTypeList` is not an `etl::type_list`.

---

```cpp
template <typename TTypeList, typename T>
constexpr size_t type_list_index_of_type_v
```
**Description**  
`value` is defined as the index of the type.  
From: `C++17`

## type_list_indices_of_type
```cpp
template <typename TTypeList, typename T>
struct type_list_indices_of_type
```
**Description**  
Defines an `index_sequence` of indices where `T` appears in the `type_list`.  
If the type is not in the `type_list`, then defined as an empty `index_sequence`.  
Useful for type lists that contain duplicates, otherwise use `type_list_index_of_type`.  
Static asserts if `TTypeList` is not an `etl::type_list`.

---

```cpp
template <typename TTypeList, typename T>
type_list_indices_of_type_t
```
**Description**  
Defined as `index_sequence` of indices where `T` appears in the `type_list`.  
If the type is not in the `type_list`, then defined as an empty `index_sequence`.  
Useful for type lists that contain duplicates, otherwise use `type_list_index_of_type`.  
Static asserts if `TTypeList` is not an `etl::type_list`.

## type_list_contains
```cpp
template <typename TTypeList, typename T>
struct type_list_contains
```
**Description**  
Defines `value` as `true` if the `type_list` contains the specified type, otherwise `false`.
From: `C++17`

---

```cpp
template <typename TTypeList, typename T>
constexpr bool type_list_contains_v
```
**Description**  
Defined as `true` if the `type_list` contains the specified type, otherwise `false`.

## type_list_has_duplicates_of
```cpp
template <typename TTypeList, typename T>
struct type_list_has_duplicates_of
```
**Description**  
Defines `value` as `true` if the `type_list` has duplicates of the specified type, otherwise `false`.

---

```cpp
template <typename TTypeList, typename T>
constexpr bool type_list_has_duplicates_of_v
```
Defined as `true` if the `type_list` has duplicates of the specified type, otherwise `false`.  
From: `C++17`

## type_list_count_of
```cpp
template <typename TTypeList, typename T>
struct type_list_count_of
```
**Description**  
Defines `value` that is the count of the number of times a type is in the `type_list`.

---

```cpp
template <typename TTypeList, typename T>
constexpr size_t type_list_count_of_v
```
**Description**  
Defined as the count of the number of times a type is in the `type_list`.
From: `C++17`

## type_list_max_size
```cpp
template <typename T>
struct type_list_max_size;
```
**Description**  
Defines `value` that is the maximum `sizeof` all types in the `type_list`.  
If the `type_list` is empty, then defined as `0`.

---

```cpp
template <typename TTypeList>
constexpr size_t type_list_max_size_v
```
**Description**  
Defined as the maximum `sizeof` all types in the `type_list`.  
If the `type_list` is empty, then defined as `0`.  
From: `C++17`

## type_list_max_alignment
```cpp
template <typename T>
struct type_list_max_alignment
```
**Description**  
Defines `value` as the maximum alignment all types in the `type_list`.  
If the `type_list` is empty, then defined as `1`.

---

```cpp
template <typename TTypeList>
constexpr size_t type_list_max_alignment_v
```
Defined as the maximum alignment all types in the `type_list`.  
If the `type_list` is empty, then defined as `1`.
From: `C++17`

## type_list_select
```cpp
template <typename TTypeList, size_t... Indices>
struct type_list_select
```
**Description**  
Selects types from a given `type_list`, according to a list of indices. 
Defines `type` as the modified `type_list`. 

---

```cpp
template <typename TTypeList, size_t... Indices>
type_list_select_t
```
**Description**  
Selects types from a given `type_list`, according to a list of indices. 
Defined as the modified `type_list`. 

## type_list_select_from_indexes
```cpp
template <typename TTypeList, size_t... Indices>
type_list_select_from_indexes
```
**Description**  
`type_list_select_from_indexes` is an alias of `type_list_select` to be more consistent with the naming of other type_list metafunctions.  
Selects types from a given `type_list`, according to a list of indices. 
Defines `type` as the modified `type_list`. 

## type_list_select_from_index_sequence
```cpp
template <typename TTypeList, typename TIndexSequence>
struct type_list_select_from_index_sequence
```
**Description**  
Defines a modified `type_list` by selecting types from a given `type_list`, according to an index sequence.  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList, typename TIndexSequence>
type_list_select_from_index_sequence_t
```

## type_list_select_common_types
```cpp
template <typename... TypeLists>
struct type_list_select_common_types
```
**Description**  
Defines `type` as a modified `type_list` of all of the types that are common to all of the `type_lists`.  
A type is included in the result if it is present in every `type_list`.  
*The result contains no duplicates*.  

---

```cpp
template <typename... TypeLists>
type_list_select_common_types_t
```
**Description**  
Defined as a modified `type_list` of all of the types that are common to all of the `type_lists`.  
A type is included in the result if it is present in every `type_list`.  
*The result contains no duplicates*.  

## type_list_select_not_common_types

```cpp
  template <typename... TypeLists>
  struct type_list_select_not_common_types
```
**Description**  
Defines `type` as a modified `type_list` of all of the types that are not common to all of the `type_lists`.  
A type is included in the result if it is not present in every `type_list`.  
*The result contains no duplicates*.  

---

```cpp
template <typename... TypeLists>
type_list_select_not_common_types_t
```
**Description**  
Defined as modified `type_list` of all of the types that are not common to all of the `type_lists`.  
A type is included in the result if it is not present in every `type_list`.  
*The result contains no duplicates*.  

## type_list_cat
```cpp
template <typename... TTypes>
struct type_list_cat
```
**Description**  
Defines `type` as the concatenation of two or more `type_lists`.  
*Does not remove duplicates.*

---

```cpp
template <typename... TypeLists>
type_list_cat_t
```
**Description**  
Defined as the concatenation of two or more `type_lists`.  
*Does not remove duplicates.*

## type_list_cat_unique
```cpp
template <typename... TypeLists>
struct type_list_cat_unique
```
**Description**  
Defines `type` as the concatenation of two or more `type_lists`.  
*Removes duplicates*.

```cpp
template <typename... TypeLists>
type_list_cat_unique_t
```
**Description**  
Defined as the concatenation of two or more `type_lists`.  
*Removes duplicates*.  

## type_list_push_front
```cpp
template <typename T, typename... TTypes>
struct type_list_push_front
```
**Description**  
Add a type to the beginning of a `type_list`.  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TypeList, typename T>
type_list_push_front_t
```
**Description**  
Add a type to the beginning of a `type_list`.  
Defined as the modified `type_list`.

## type_list_pop_front
```cpp
template <typename TTypeList>
struct type_list_pop_front
```
**Description**  
Removes the first type from a `type_list`.  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList>
type_list_pop_front_t
```
**Description**  
Removes the first type from a `type_list`.  
Defined as the modified `type_list`.

## type_list_push_back
```cpp
template <typename T, typename... TTypes>
struct type_list_push_back
```
**Description**  
Add a type to the end of a `type_list`.  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TypeList, typename T>
type_list_push_back_t
```
**Description**  
Add a type to the end of a `type_list`.  
Defined as the modified `type_list`.

## type_list_pop_back
```cpp
template <typename TTypeList>
struct type_list_pop_back
```
**Description**  
Removes the last type from a `type_list`.  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList>
type_list_pop_back_t
```
**Description**  
Removes the last type from a `type_list`.  
Defined as the modified `type_list`.

## type_list_insert
```cpp
template <typename TTypeList, typename T, size_t Index>
struct type_list_insert
```
**Description**  
Insert a type at an index in a `type_list`.  
Inserts before the type currently at Index.  
If `Index` == size of the `type_list`, the type is appended.  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList, typename T, size_t Index>
type_list_insert_t
```
**Description**  
Insert a type at an index in a `type_list`.  
Inserts before the type currently at Index.  
If `Index` == size of the `type_list`, the type is appended.  
Defined as the modified `type_list`

## type_list_remove
```cpp
template <typename TTypeList, size_t Index>
struct type_list_remove
```
**Description**  
Remove a type at an index in a `type_list`.  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList, size_t Index>
type_list_remove_t
```
**Description**  
Remove a type at an index in a `type_list`.  
Defined as the modified `type_list`

## type_list_remove_if
```cpp
template <typename TTypeList, template <typename> class TPredicate>
struct type_list_remove_if
```
**Description**  
Remove types that satisfy a predicate from a `type_list`.  
Predicate must be: `template <typename T> struct Pred : etl::bool_constant<...> {};`  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList, template <typename> class TPredicate>
type_list_remove_if_t
```
**Description**  
Remove types that satisfy a predicate from a `type_list`.  
Predicate must be: `template <typename T> struct Pred : etl::bool_constant<...> {};`  
Defined as the modified `type_list`

## type_list_unique
```cpp
template <typename TTypeList>
struct type_list_unique
```
**Description**  
Removes duplicate types from a given `type_list`, preserving the first occurrence.  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList>
type_list_unique_t
```
**Description**  
Removes duplicate types from a given `type_list`, preserving the first occurrence.  
Defined as the modified `type_list`

## type_list_is_unique
```cpp
template <typename TTypeList>
struct type_list_is_unique
```
**Description**  
Checks that all of the types in a `type_list` are unique.  
Inherits from `etl::true_type` or `etl::false_type`.  

---

```cpp
template <typename TTypeList>
constexpr bool type_list_is_unique_v
```
**Description**  
Checks that all of the types in a `type_list` are unique.
Defined as `true` or `false`.  
From: `C++17`

## type_list_is_empty
```cpp
template <typename T>
struct type_list_is_empty
```
**Description**  
Checks if the `type_list` is empty.
Inherits from `etl::true_type` or `etl::false_type`.  

---

```cpp
template <typename... TTypes>
constexpr bool type_list_is_empty_v
```
**Description**  
Checks if the `type_list` is empty.
Defined as `true` or `false`.
From: `C++17`

## type_list_all_of
```cpp
template <typename TTypeList, template <typename> class TPredicate>
struct type_list_all_of;
```
**Description**  
Checks that all types in a `type_list` satisfy a unary predicate.  
Predicate must be: `template <typename T> struct Pred : etl::bool_constant<...> {};`  
Inherits from `etl::true_type` or `etl::false_type`.  

---

```cpp
template <typename TTypeList, template <typename> class TPredicate>
constexpr bool type_list_all_of_v
```
**Description**  
Checks that all types in a `type_list` satisfy a unary predicate.  
Predicate must be: `template <typename T> struct Pred : etl::bool_constant<...> {};`  
Defined `true` or `false`.  
From: `C++17`

## type_list_any_of
```cpp
template <typename TTypeList, template <typename> class TPredicate>
struct type_list_any_of
```
**Description**  
Checks that any type in a `type_list` satisfies a unary predicate.  
Predicate must be: `template <typename T> struct Pred : etl::bool_constant<...> {};`  
Inherits from `etl::true_type` or `etl::false_type`.  

---

```cpp
template <typename TTypeList, template <typename> class TPredicate>
constexpr bool type_list_any_of_v
```
**Description**  
Checks that any type in a `type_list` satisfies a unary predicate.  
Predicate must be: `template <typename T> struct Pred : etl::bool_constant<...> {};`  
Defined as `true` or `false`.  
From: `C++17`

## type_list_none_of
```cpp
template <typename TTypeList, template <typename> class TPredicate>
struct type_list_none_of
```
**Description**  
Checks that no types in a `type_list` satisfy a unary predicate.  
Predicate must be: `template <typename T> struct Pred : etl::bool_constant<...> {};`  
Inherits from `etl::true_type` or `etl::false_type`.  

---

```cpp
template <typename TTypeList, template <typename> class TPredicate>
constexpr bool type_list_none_of_v
```
**Description**  
Checks that no types in a `type_list` satisfy a unary predicate.  
Predicate must be: `template <typename T> struct Pred : etl::bool_constant<...> {};`  
Defined as `true` or `false`.  
From: `C++17`

## type_lists_are_convertible
```cpp
template <typename TFromList, typename TToList>
struct type_lists_are_convertible
```
**Description**  
Checks that two type lists are convertible.
Static asserts if the type lists are not the same length.  
Inherits from `etl::true_type` or `etl::false_type`.  

---

```cpp
template <typename TFromList, typename TToList>
constexpr bool type_lists_are_convertible_v
```
**Description**  
Checks that two type lists are convertible.
Static asserts if the type lists are not the same length.  
Defined as `true` or `false`.  
From: `C++17`

## type_list_is_sorted
```cpp
  template <typename TTypeList, template <typename, typename> class TCompare>
  struct type_list_is_sorted
```
**Description**  
Checks if a `type_list` is sorted according to `TCompare`.  
Static asserts if `TTypeList` is not an `etl::type_list`.  
Comparator must be: template <typename A, typename B> struct Compare : etl::bool_constant<...> {};  
Inherits from `etl::true_type` or `etl::false_type`.  

---

```cpp
template <typename TTypeList, template <typename, typename> class TCompare>
constexpr bool type_list_is_sorted_v
```
**Description**  
Checks if a `type_list` is sorted according to `TCompare`.  
Static asserts if `TTypeList` is not an `etl::type_list`.  
Comparator must be: template <typename A, typename B> struct Compare : etl::bool_constant<...> {};  
Defined as `true` or `false`.  
From: `C++17`

## type_list_insert_sorted
```cpp
template <typename TTypeList, typename T, template <typename, typename> class TCompare>
struct type_list_insert_sorted
```
**Description**  
Insert `T` into the correct position in the sorted list, as determined by `TCompare`.
Static asserts if `TTypeList` is not sorted according to `TCompare`.  
Comparator must be: `template <typename A, typename B> struct Compare : etl::bool_constant<...> {};`  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList, typename T, template <typename, typename> class TCompare>
type_list_insert_sorted_t
```
**Description**  
Insert `T` into the correct position in the sorted list, as determined by `TCompare`.
Static asserts if `TTypeList` is not sorted according to `TCompare`.  
Comparator must be: `template <typename A, typename B> struct Compare : etl::bool_constant<...> {};`  
Defined as the modified `type_list`.

## type_list_sort
```cpp
template <typename TTypeList, template <typename, typename> class TCompare>
struct type_list_sort
```
**Description**  
`etl::type_list` sorting by a user supplied type comparator.  
Comparator must be: `template <typename A, typename B> struct Compare : etl::bool_constant<...> {};`  
Defines `type` as the modified `type_list`.

---

```cpp
template <typename TTypeList, template <typename, typename> class TCompare>
type_list_sort_t
```
**Description**  
`etl::type_list` sorting by a user supplied type comparator.  
Comparator must be: `template <typename A, typename B> struct Compare : etl::bool_constant<...> {};`  
Defined as the modified `type_list`.

## type_list_in_all_lists
```cpp
template <typename T, typename... TypeLists>
struct type_list_in_all_lists
```
**Description**  
Checks if a type `T` is present in all provided `type_lists`.  
Inherits from `etl::true_type` if `T` is in every list, otherwise `etl::false_type`.  
Since: `20.48.0`

```cpp
template <typename T, typename... TypeLists>
constexpr bool type_list_in_all_lists_v
```
**Description**  
Checks if a type `T` is present in all provided `type_lists`.  
Defined as `true` if `T` is in every list, otherwise `false`.  
Since: `20.48.0`  
From: `C++17`

## type_list_in_any_list
```cpp
template <typename T, typename... TypeLists>
struct type_list_in_any_list
```
**Description**  
Checks if a type `T` is present in at least one of the provided `type_lists`.  
Inherits from `etl::true_type` if `T` is in any list, otherwise `etl::false_type`.  
Since: `20.48.0`  

---

```cpp
template <typename T, typename... TypeLists>
constexpr bool type_list_in_any_list_v
```
**Description**  
Checks if a type `T` is present in at least one of the provided `type_lists`.  
Defined as `true` if `T` is in any list, otherwise `false`.    
Since: `20.48.0`  
From: `C++17`

## type_list_in_no_lists

```cpp
template <typename T, typename... TypeLists>
struct type_list_in_no_lists
```
**Description**  
Checks if a type `T` is present none of the provided `type_lists`.   
Inherits from  `etl::true_type` if `T` is in no list, otherwise `etl::false_type`.  
Since: `20.48.0`

---

```cpp
template <typename T, typename... TypeLists>
constexpr bool type_list_in_no_lists_v
```
**Description**  
Checks if a type `T` is present in none of the provided `type_lists`.   
Defined as `true` if `T` is in no list, otherwise `false`.    
Since: `20.48.0`  
From: `C++17`

## type_list_remove_from
```cpp
template <typename... TypeLists>
struct type_list_remove_from
```
**Description**  
Defines a modified `type_list` that contains the types in the first list that are not in any of the following lists.  
*Does not remove duplicates*.  
`type` is defined as the modified `type_list`.

---

```cpp
template <typename... TypeLists>
type_list_remove_from_t
```
**Description**  
Defines a modified `type_list` that contains the types in the first list that are not in any of the following lists.  
*Does not remove duplicates*.  
Defined as the modified `type_list`.