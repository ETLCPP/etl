---
title: "Containers"
weight: 100
---

## Containers
The library defines a set of containers that have been specially tailored for embedded systems.They have a maximum capacity fixed at compile time and make no calls to malloc/free or  new/delete.They are completely deterministic.
Most container types have been designed to mimic, as far as possible, those found in the STL. Some do not havedirect STL equivalents.

As the storage for all of the container types is allocated as a contiguous block, they are extremely cache friendly.
Note: Unlike the STL, the ETL's vector<bool> really is a container and actually does contains bool.  

If you require a compact Boolean container then etl::bitset may be more appropriate.
Intrusive containersThere are intrusive versions of certain containers. These do not store copies of the inserted values, but link to theoriginal objects. Certain types of intrusive container require that the stored object derives from an intrusive link.  

Most intrusive containers do not have a maximum fixed capacity.See here for more information.
To eliminate code bloat, most container templates utilise 'hoisting' where functionality, independent of the sizeand/or type, is separated out in to base classes.  

For example, `vector<int, 5>` and `vector<int, 10>` will share code from `ivector<int>`.  
`vector<int, 5>`, `vector<float, 7>` and `vector<int, 10>` will all share code from `vector_base`.  

The `i` prefixed container types may be used as size independent pointer or reference types for all sizes of thederived type.

```C++
etl::vector<int, 5>  vector1;  
etl::vector<int, 10> vector2;

etl::ivector<int>* pvector;

pvector = &vector1;pvector->push_back(3);
pvector = &vector2;pvector->push_back(4);
```

## Move semantics and rvalue references
The following containers support rvalue references and move semantics.

`vector`  
`deque`  
`list`  
`forward_list`  
`flat_set`  
`flat_multiset`  
`flat_map`  
`flat_multimap`  

*This list may be out of date*.

## Differences from the STL containers
As the containers have a fixed capacity, most also implement full() and available() member functions.  

Most of the containers allocate their storage from an internal instance of `etl::pool`.
Because of this the containers have a certain set of attributes that differ from the standard library.
- The storage for all containers is contiguous, thereby enhancing cache hits.
- No heap memory is used; no OS or user supplied memory management is required.
- Non-static containers declared locally within functions will store their contents on the stack.
- Copying or swapping a container is not a low cost action, as all of the contents will be copied  
(except for externally buffered vectors which will use a simple pointer copy).

## Notes
Although the containers utilise inheritance, like the STL containers, they are not intended to be used  
polymorphically, unless enabled by a compile time macros `ETL_POLYMORPHIC_XXX`.
