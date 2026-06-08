---
title: "packet"
---

A homogeneous container that can contain one of several types that are all inherited from the same base class.  

```cpp
etl::packet<typename TBase, size_t SIZE, size_t ALIGNMENT>
```

## Template parameters
`TBase`  
&emsp;The base class for all objects. The destructor must be virtual.  

`SIZE`  
&emsp;The size of the largest type.  

`ALIGNMENT`  
&emsp;The largest alignment of all of the types.  

## Member types

`base_t` = `TBase`

## Contructors

**C++03**  
```cpp
template <typename T>
explicit packet(const T& value)
```

**C++11**  
```cpp
template <typename T>
explicit packet(T&& value)
```
**Description**  
Constructs an object of type `T` with the supplied value.  
Static asserts on any type that does not derive from `TBase`.  
Static asserts on any type that does not conform to the maximum size and alignment.  

## Destructor
```cpp
~packet()
```
**Description**  
Destructs the contained object

## Operators

**C++03**  
```cpp
template <typename T>
packet& operator =(const T& value)
```

**C++11**  
```cpp
template <typename T>
packet& operator =(T&& value)
```
**Description**  
Assigns a new object to the packet.
The previous object is destructed.

## Access

```cpp
const TBase& get() const
```
**Description**  
Returns a const reference to the contained object.
