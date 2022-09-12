.. etl documentation master file, created by
   sphinx-quickstart on Mon Sep 12 11:23:29 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Embedded Template Library (ETL)
===============================

Motivation
------------

C++ is a great language to use for embedded applications and templates are a powerful aspect. The standard library can offer a great deal of well tested functionality,  but there are some parts of the standard library that do not fit well with deterministic behaviour and limited resource requirements. These limitations usually preclude the use of dynamically allocated memory and containers with open ended sizes.

What is needed is a template library where the user can declare the size, or maximum size of any object upfront. Most embedded compilers do not currently support the standard beyond C++ 03, therefore excluding the programmer from using the enhanced features of the later library.

This is what the ETL attempts to achieve.

Main features
-----------------

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


.. toctree::
   :maxdepth: 2
   :caption: Contents:



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
