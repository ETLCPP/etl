**etl**
===
Embedded Template Library
-------------------------
The embedded template library has been designed for lower resource embedded applications.
It contains a set of containers, algorithms and utilities, some of which emulate parts of the STL.
There is no dynamic memory allocation. The library makes no use of the heap. All of the containers have a fixed capacity allowing all memory allocation to be determined at compile time.
The library is intended for any compiler that supports C++ 03.

**Main features:**

 - A set of fixed capacity containers. (stack, queue, list, forward_list, vector, deque)
 - Templated compile time constants.
 - Templated design pattern base classes (Visitor, Observer)
 - Reverse engineered C++ 0x11 features (type traits, algorithms, containers etc.)
 - Smart enumerations
 - 8, 16, 32 & 64 bit CRC calculations
 - Many utilities for template support.
 - Variants (a type that can store many types in a type-safe interface)
 - Optional exceptions on errors.

