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

Installing the library
-------------------------

You can find the setup steps `here <https://www.etlcpp.com/setup.html>`_.

CMake
~~~~~~~

One way to use this library is to drop it somewhere in your project directory
and then make the library available by using `add_subdirectory`


.. code:: cmake

   add_subdirectory(etl)
   add_executable(foo main.cpp)
   target_link_libraries(foo PRIVATE etl::etl)


If ETL library is used as a Git submodule it may require additional configuration for proper ETL version resolution by allowing the lookup for Git folder outside of the library root directory.


.. code:: cmake

   set(GIT_DIR_LOOKUP_POLICY ALLOW_LOOKING_ABOVE_CMAKE_SOURCE_DIR)
   add_subdirectory(etl)


If you want to install this library with CMake, you can perform the following steps. On Linux,
super user rights might be required to install the library, so it might be necessary to add
``sudo`` before the last command:

.. code:: shell

   git clone https://github.com/ETLCPP/etl.git
   cd etl
   git checkout <targetVersion>
   cmake -B build .
   cmake --install build/

After the library has been installed, you can use
`find_package <https://cmake.org/cmake/help/latest/command/find_package.html>`_ to use the library.
Replace ``<majorVersionRequirement>`` with your desired major version:

.. code:: cmake

   find_package(etl <majorVersionRequirement>)
   add_executable(foo main.cpp)
   target_link_libraries(foo PRIVATE etl::etl)


Alternatively you can use `FetchContent <https://cmake.org/cmake/help/latest/module/FetchContent.html>`_,
replacing ``<targetVersion>`` with the version to install based on a git tag:

.. code:: cmake

   Include(FetchContent)
   FetchContent_Declare(
     etl
     GIT_REPOSITORY https://github.com/ETLCPP/etl
     GIT_TAG        <targetVersion>
   )
   FetchContent_MakeAvailable(etl)
   add_executable(foo main.cpp)
   target_link_libraries(foo PRIVATE etl::etl)

Table of Contents
===================

.. toctree::
   :maxdepth: 2

   api

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
