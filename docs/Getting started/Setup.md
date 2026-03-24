---
title: "Setup"
---

---

This page describes the steps needed to integrate the ETL with your project.
The ETL requires no special installation. Just copy or clone the GitHub project into an appropriate directory.
For additional information pertaining to compilers, see this page.

The ETL does not depend on the STL, but can use the algorithms and definitions from it, if the project does.
This is controlled by the ETL_NO_STL macro. If this macro is defined in the profile then the ETL will use its own reverse engineered versions.
See No STL

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

## Missing <new>
The Arduino is often not supplied with a definition of the header `<new>` causing compilation to fail.  
One option, if you have this problem, is to define your own empty header `new`.

## STL
The Arduino programming platform is not supplied with an implementation of the STL. Probably the best option for Arduino users to define ETL_NO_STL which will allow the ETL to be compiled without the standard library.

If `ETL_NO_STL` is not defined then one must be acquired. 

A suitable STL implementation may be downloaded from the Arduino library resource. It is called ArduinoSTL.  
Add `#include <ArduinoSTL.h>` to the start of every file that uses the ETL.  
Remember to add the directory to the include path.

## PlatformIO
Many people are now using PlatformIO in conjunction with Visual Studio Code, which supports many platforms, including Arduino.

## Compatibility with CppUTest
There are some issues with using the ETL with a project that uses CppUTest for unit testing.
The unit test library redefines the new operator. The ETL uses 'placement new' in many of its classes and the CppUTest redefinition breaks this functionality.

To allow CppUTest to work with the ETL these macros must be disabled, either by commenting them out in the source files or removing the header files from the Makefile. A recompilation of CppUTest will be necessary.

