Embedded Template Library (ETL)
-------------------------

[![Release date](https://img.shields.io/github/release-date/jwellbelove/etl?color=%231182c3)](https://img.shields.io/github/release-date/jwellbelove/etl?color=%231182c3)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/jwellbelove/etl)
[![Standard](https://img.shields.io/badge/c%2B%2B-98/03/11/14/17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Sponsors](https://img.shields.io/github/sponsors/ETLCPP)](https://img.shields.io/github/sponsors/ETLCPP)

![CI](https://github.com/ETLCPP/etl/workflows/vs2019/badge.svg)
![CI](https://github.com/ETLCPP/etl/workflows/gcc/badge.svg)
![CI](https://github.com/ETLCPP/etl/workflows/clang/badge.svg)
[![Build status](https://ci.appveyor.com/api/projects/status/b7jgecv7unqjw4u0/branch/master?svg=true)](https://ci.appveyor.com/project/jwellbelove/etl/branch/master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/3c14cd918ccf40008d0bcd7b083d5946)](https://www.codacy.com/manual/jwellbelove/etl?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ETLCPP/etl&amp;utm_campaign=Badge_Grade)

**Motivation**

C++ is a great language to use for embedded applications and templates are a powerful aspect. The standard library can offer a great deal of well tested functionality,  but there are some parts of the standard library that do not fit well with deterministic behaviour and limited resource requirements. These limitations usually preclude the use of dynamically allocated memory and containers with open ended sizes.

What is needed is a template library where the user can declare the size, or maximum size of any object upfront. Most embedded compilers do not currently support the standard beyond C++ 03, therefore excluding the programmer from using the enhanced features of the later library.

This is what the ETL attempts to achieve.

**Summary**

The ETL is not designed to completely replace the STL, but complement it.
Its design objective covers three areas.

- Create a set of containers where the size or maximum size is determined at compile time. These containers are direct equivalents of those supplied in the STL.
- Be compatible with C++ 03 but implement as many of the C++ 11 additions as possible.
- Add other useful components that are not present in the standard library.

The embedded template library has been designed for lower resource embedded applications.
It contains a set of containers, algorithms and utilities, some of which emulate parts of the STL.
There is no dynamic memory allocation. The library makes no use of the heap. All of the containers have a fixed capacity allowing all memory allocation to be determined at compile time.
The library is intended for any compiler that supports C++ 03.

**Main features:**

- Cross platform. This library is not specific to any processor type.
- No dynamic memory allocation
- No RTTI required
- Very little use of virtual functions. They are used only when they are absolutely necessary for the required functionality
- A set of fixed capacity containers. (array, bitset, deque, forward_list, list, queue,  stack, vector, map, set, etc.)
- As the storage for all of the container types is allocated as a contiguous block, they are extremely cache friendly
- Templated compile time constants
- Templated design pattern base classes (Visitor, Observer)
- Reverse engineered C++ 0x11 features (type traits, algorithms, containers etc.)
- Type-safe enumerations
- Type-safe typedefs
- 8, 16, 32 & 64 bit CRC calculations
- Checksums & hash functions
- Variants (a type that can store many types in a type-safe interface)
- Choice of asserts, exceptions, error handler or no checks on errors
- Unit tested (currently over 6480 tests), using VS2019, GCC 8.1.0, , GCC 9.3.0, Clang 9.0.0 & 10.0.0
- Many utilities for template support.
- Easy to read and documented source.
- Free email support

Any help porting the library to work under different platforms and compilers would be gratefully received.
I am especially interested in people who are using Keil, IAR, Green Hills, TI Code Composer etc, bare metal or RTOS, and DSPs.

See (https://www.etlcpp.com) for up-to-date information.

**Arduino library:**

The content of this repo is available as a library in the Arduino IDE (search for the "Embedded Template Library" in the IDE library manager). The Arduino library repository is available at ```https://github.com/ETLCPP/etl-arduino```, see there for more details.
