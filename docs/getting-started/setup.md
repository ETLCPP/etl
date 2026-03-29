---
title: "Setup"
weight: 1
---

---

This page describes the steps needed to integrate the ETL with your project.

## CMake

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

# Profile definition

When using ETL in a project, there is typically an `etl_profile.h` defined to
adjust ETL to the project needs. ETL will automatically find `etl_profile.h`
if it is available in the include path(s). If it's not available, ETL will
work with default values.

## Example

```C++
#ifndef __ETL_PROFILE_H__
#define __ETL_PROFILE_H__

#define ETL_TARGET_DEVICE_GENERIC
#define ETL_TARGET_OS_NONE

#define ETL_NO_STL

#define ETL_THROW_EXCEPTIONS
#define ETL_VERBOSE_ERRORS
#define ETL_CHECK_PUSH_POP

#endif
```

## Platform specific implementation

Although ETL is generally a self-contained header-only library, some interfaces need to be
implemented in every project or platform, at least if those interfaces are actually being
used, due to project specifics:

| ETL header | Platform specific API to be implemented | Needed when using                   |
|------------|-----------------------------------------|-------------------------------------|
| `chrono.h` | `etl_get_high_resolution_clock()`       | `etl::high_resolution_clock::now()` |
|            | `etl_get_system_clock()`                | `etl::system_clock::now()`          |
|            | `etl_get_steady_clock()`                | `etl::steady_clock::now()`          |
| `print.h`  | `etl_putchar()`                         | `etl::print()`                      |
|            |                                         | `etl::println()`                    |

### Example

```C++
#include <etl/chrono.h>
#include <etl/print.h>

extern "C"
{
  etl::chrono::high_resolution_clock::rep etl_get_high_resolution_clock()
  {
    return etl::chrono::high_resolution_clock::rep(static_cast<int64_t>(getSystemTimeNs()));
  }

  etl::chrono::system_clock::rep etl_get_system_clock()
  {
    return etl::chrono::system_clock::rep(static_cast<int64_t>(getSystemTimeNs()));
  }

  etl::chrono::system_clock::rep etl_get_steady_clock()
  {
    return etl::chrono::system_clock::rep(static_cast<int64_t>(getSystemTimeNs()));
  }

  void etl_putchar(int c)
  {
    putByteToStdout(static_cast<uint8_t>(c));
  }
}
```

The following default values apply if the respective macros are not defined
(e.g. in `etl_profile.h`):

| Macro                                         | Default                    |
|-----------------------------------------------|----------------------------|
| `ETL_CHRONO_SYSTEM_CLOCK_DURATION`            | `etl::chrono::nanoseconds` |
| `ETL_CHRONO_SYSTEM_CLOCK_IS_STEADY`           | `true`                     |
| `ETL_CHRONO_HIGH_RESOLUTION_CLOCK_DURATION`   | `etl::chrono::nanoseconds` |
| `ETL_CHRONO_HIGH_RESOLUTION_CLOCK_IS_STEADY`  | `true`                     |
| `ETL_CHRONO_STEADY_CLOCK_DURATION`            | `etl::chrono::nanoseconds` |

## Arduino library

The content of this repo is available as a library in the Arduino IDE (search for the "Embedded Template Library" in the IDE library manager). The Arduino library repository is available at ```https://github.com/ETLCPP/etl-arduino```, see there for more details.

## Compilers
For additional information pertaining to compilers, see [this page](https://www.etlcpp.com/compilers.html).

The ETL does not depend on the STL, but can use the algorithms and definitions from it, if the project does.

This is controlled by the ETL_NO_STL macro. If this macro is defined in the profile then the ETL will use its own reverse engineered versions.  
See [No STL]().

## User defined files
The user may  create a file named `etl_profile.h` that exists in the include path for the project.
The file will define any ETL configuration macros. See here for further information.
This header is included from ETL's `platform.h`

An example of `etl_profile.h` could be:-

```C++
#ifndef ETL_PROFILE_H
#define ETL_PROFILE_H

#define ETL_THROW_EXCEPTIONS
#define ETL_VERBOSE_ERRORS
#define ETL_CHECK_PUSH_POP
#endif
```

There are several example profiles available in the ETL's `profiles/` directory. They may be used as a base for your own project specific variant.

If `etl_profile.h` does not exist then `platform.h` will attempt to automatically deduce the development OS, compiler  
type, version and language support level.

## platform.h
This header is defined by the ETL and is included in all ETL headers. It will attempt to identify the development OS, compiler and language support level. Using this information it will define various ETL configuration macros.  
The user does not need to modify this file.

## Additional include directories
The compiler will normally expect paths to additional include directories to be specified  
A path to `etl/include` would normally be set, allowing header files to be specified as follows:-
```C++
#include "etl/vector.h"
#include "etl/fsm.h"
```

A path to a user generated `etl_profile.h` must be defined.

## WSL
There is a WSL Ubuntu image for running unit tests on Windows Subsystem for Linux.

## Arduino
The ETL is available through the Arduino library manager.

Missing `new`.

The Arduino is often not supplied with a definition of the header `<new>` causing compilation to fail.  
One option, if you have this problem, is to define your own empty header `new`.

## STL
The Arduino programming platform is not supplied with an implementation of the STL. Probably the best option for Arduino users to define `ETL_NO_STL` which will allow the ETL to be compiled without the standard library.

If `ETL_NO_STL` is not defined then one must be acquired. 

A suitable STL implementation may be downloaded from the Arduino library resource. It is called **ArduinoSTL**.  
Add `#include <ArduinoSTL.h>` to the start of every file that uses the ETL.  
Remember to add the directory to the include path.

## PlatformIO
Many people are now using PlatformIO in conjunction with Visual Studio Code, which supports many platforms, including Arduino.

## Compatibility with CppUTest
There are some issues with using the ETL with a project that uses CppUTest for unit testing.  

The unit test library redefines the `new` operator. The ETL uses 'placement' `new` in many of its classes and the CppUTest redefinition breaks this functionality.  

To allow CppUTest to work with the ETL these macros must be disabled, either by commenting them out in the source files or removing the header files from the Makefile. A recompilation of CppUTest will be necessary.

