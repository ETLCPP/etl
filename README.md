Embedded Template Library (ETL)
-------------------------

![GitHub release (latest by date)](https://img.shields.io/github/v/release/jwellbelove/etl)
[![Release date](https://img.shields.io/github/release-date/jwellbelove/etl?color=%231182c3)](https://img.shields.io/github/release-date/jwellbelove/etl?color=%231182c3)
[![Standard](https://img.shields.io/badge/c%2B%2B-98/03/11/14/17/20/23-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
![GitHub contributors](https://img.shields.io/github/contributors-anon/ETLCPP/etl)
![GitHub forks](https://img.shields.io/github/forks/ETLCPP/etl?style=flat)
![GitHub Repo stars](https://img.shields.io/github/stars/ETLCPP/etl?style=flat)

![CI](https://github.com/ETLCPP/etl/actions/workflows/msvc.yml/badge.svg?branch=master)
[![Build status](https://ci.appveyor.com/api/projects/status/b7jgecv7unqjw4u0/branch/master?svg=true)](https://ci.appveyor.com/project/jwellbelove/etl/branch/master)

![CI](https://github.com/ETLCPP/etl/actions/workflows/gcc-c++11.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/gcc-c++14.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/gcc-c++17.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/gcc-c++20.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/gcc-c++23.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/gcc-syntax-checks.yml/badge.svg?branch=master)

![CI](https://github.com/ETLCPP/etl/actions/workflows/clang-c++11.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/clang-c++14.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/clang-c++17.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/clang-c++20.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/clang-c++23.yml/badge.svg?branch=master)
![CI](https://github.com/ETLCPP/etl/actions/workflows/clang-syntax-checks.yml/badge.svg?branch=master)

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/3c14cd918ccf40008d0bcd7b083d5946)](https://www.codacy.com/manual/jwellbelove/etl?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=ETLCPP/etl&amp;utm_campaign=Badge_Grade)

[![](https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&color=%23fe8e86)](https://www.etlcpp.com/sponsor.html)

[Project documentation](https://www.etlcpp.com/)

## Motivation

C++ is a powerful language for embedded systems development, with templates offering a great deal of flexibility and type safety. While the C++ Standard Library provides a wealth of well-tested functionality, it’s often not well suited to environments with strict deterministic behavior and limited resources.

In many embedded applications, dynamic memory allocation is discouraged or outright prohibited, making standard STL containers and many other components impractical or unusable.

What’s needed is a template library specifically designed for embedded systems — one that allows developers to define fixed or maximum sizes for containers and other objects at compile time. Additionally, since many embedded toolchains still lack full support for standards beyond C++03, it's valuable to have access to a library that backports select features from later versions of the C++ Standard Library.

## About the ETL

The Embedded Template Library (ETL) is not intended as a full replacement for the C++ Standard Template Library (STL), but rather as a complementary solution tailored specifically for embedded systems.

Its design goals include:

-	Providing a set of containers with fixed or maximum sizes defined at compile-time.

-	Offering APIs that closely resemble those of the STL, enabling familiar and consistent usage.

-	Maintaining compatibility with C++98 while implementing many features introduced in later standards 
(C++11/14/17/20/23) where possible.

-	Ensuring deterministic behavior, which is critical in real-time and resource-constrained environments.

-	Introducing additional components and utilities useful in embedded contexts but absent from the STL.

The ETL avoids dynamic memory allocation entirely; the heap is never used. All non-intrusive containers have a fixed capacity, allowing memory requirements to be fully determined at compile-time. This makes the ETL ideal for lower-resource embedded applications where predictability, performance, and memory control are essential.

The library is compatible with any compiler that supports C++03 or later.

Help on integrating the ETL with your project may be found here.


## Key features of the ETL

-	Actively Maintained: Developed and maintained on GitHub since 2014.

-	Open Source: MIT licensed.

-	No STL Dependency: Designed to operate independently of the C++ Standard Template Library.

-	No Dynamic Memory Allocation: All storage is allocated either at compile-time or on the stack; heap usage is entirely avoided.

-	RTTI and Virtual Functions: No runtime type information (RTTI) is required. Virtual functions are used sparingly and only when strictly necessary.

-	Header-Only Library: All functionality is provided via header files; No separate compilation needed.

-	Fixed-Capacity Containers: Offers STL-like containers with fixed or maximum capacity, plus additional non-standard container types.

-	Cache Efficiency: Containers use contiguous memory layouts for optimal cache performance.

-	Compact Codebase: Shared base classes (based on type) help reduce overall container code size.

-	Compile-Time Features:
    -	Templated compile-time constants
    -	Template-based design pattern base classes (e.g., Visitor, Observer)
    -	Type-safe smart enumerations
    -	Type-safe typedefs and constants

-	Embedded System Frameworks:
    -	Message routing
    -	Finite state machines
    -	Task scheduling

-	C++11 Backports: Implements many C++11 features (type traits, algorithms, containers) for use in C++03 environments.

-	Utilities:
    -	CRC calculations (8, 16, 32 & 64-bit)
    -	Checksums and hash functions
    -	Variants (type-safe unions)
    -	Extensive template support utilities

-	Robust Error Handling: Configurable error checking using asserts, exceptions, error handlers, or no checks; user’s choice.

-	Thoroughly Tested:
    -	Over 10,000 unit tests
    -	Tested with Visual Studio 2022, GCC 12, and Clang 14
    -	Continuous integration via GitHub Actions.

-	Readable and Well-Documented: Clean, maintainable source code with clear documentation.

-	Support: Free email support available. A Slack group is available. Paid support on request.

-	Archived: A snapshot of the ETL is preserved in the Arctic Code Vault for long-term digital preservation.

Any help porting the library to work under different platforms and compilers would be gratefully received.
I am especially interested in people who are using Keil, IAR, Green Hills, TI Code Composer etc, bare metal or RTOS, and DSPs.

See (https://www.etlcpp.com) for up-to-date information.

## Installing this library

You can find the setup steps [here](https://www.etlcpp.com/setup.html).

### CMake

One way to use this library is to drop it somewhere in your project directory
and then make the library available by using `add_subdirectory`

```cmake
add_subdirectory(etl)
add_executable(foo main.cpp)
target_link_libraries(foo PRIVATE etl::etl)
```

If ETL library is used as a Git submodule it may require additional configuration for proper ETL version resolution by allowing the lookup for Git folder outside of the library root directory.

```cmake
set(GIT_DIR_LOOKUP_POLICY ALLOW_LOOKING_ABOVE_CMAKE_SOURCE_DIR)
add_subdirectory(etl)
```

If you want to install this library with CMake, you can perform the following steps. On Linux,
super user rights might be required to install the library, so it might be necessary to add
`sudo` before the last command:

```sh
git clone https://github.com/ETLCPP/etl.git
cd etl
git checkout <targetVersion>
cmake -B build .
cmake --install build/
```

After the library has been installed, you can use
[find_package](https://cmake.org/cmake/help/latest/command/find_package.html) to use the library.
Replace `<majorVersionRequirement>` with your desired major version:

```cmake
find_package(etl <majorVersionRequirement>)
add_executable(foo main.cpp)
target_link_libraries(foo PRIVATE etl::etl)
```


Alternatively you can use [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html),
replacing `<targetVersion>` with the version to install based on a git tag:

```sh
Include(FetchContent)

FetchContent_Declare(
  etl
  GIT_REPOSITORY https://github.com/ETLCPP/etl
  GIT_TAG        <targetVersion>
)

FetchContent_MakeAvailable(etl)

add_executable(foo main.cpp)
target_link_libraries(foo PRIVATE etl::etl)
```

## Arduino library

The content of this repo is available as a library in the Arduino IDE (search for the "Embedded Template Library" in the IDE library manager). The Arduino library repository is available at ```https://github.com/ETLCPP/etl-arduino```, see there for more details.
