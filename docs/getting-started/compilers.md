---
title: "Compilers"
weight: 3
---

The library is intended to be used on multiple platforms and devices. Not every single combination ofcompiler/IDE/device/library template have been, or can realistically be, checked. A set of profiles have been created for the most popular setups.
If you have issues porting the library to your platform then contact me, and I will do my best to resolve anyincompatibilities.
If you have no issues, or have solved it by yourself then please [let me know](https://github.com/ETLCPP/etl/issues) so that I may add information about it to this page.

## Visual Studio
Has been compiled under VS2019 and VS2022.  
No known issues.

## GCC
Has been compiled under GNU GCC  on Windows & Linux.  
No known issues, apart from early versions of GCC that may need to define the macro `__STDC_LIMIT_MACROS` in the project properties.

## Keil 
**uVision ARM**  
Define the macro `__STDC_LIMIT_MACROS` in the project properties.  
Compiled with version 5 & 6 ARM compilers.  
No known issues.

## TI Code Composer
**MPS430**  
Define the macro `__STDC_LIMIT_MACROS` in the project properties.
In TI projects you have the choice between EABI and COFF file formats.
There is a known issue with the COFF format when creating static libraries that use templates. You may find that the linker will complain about 'undefined symbols'.  
The only known workaround is to declare a dummy type in the main application that uses the template with the same parameter types.

e.g.
If your static library uses `etl::vector<MyType, N>` then declare a dummy type `etl::vector` of the same type in the application.

## IAR Embedded Workbench
**AVR Compiler**  
**ARM**  

Define the macro `__STDC_LIMIT_MACROS` in the project properties.No known issues.

## Segger Embedded Studio
**ARM**  
If using STLPort then the macro `ETL_STLPORT` must be defined in the profile.

## Greenhills
**ARM**  
No known issues.

## Zephyr OS
The ETL uses 'placement new'.
To enable this from Zephyr you need to add the following to `proj.conf`.    
`CONFIG_LIB_CPLUSPLUS=yCONFIG_NEWLIB_LIBC=y`

## avr-gcc 9.2
If you are using `-std=c++2a` you will find the compiler complaining aboutredeclaration of C++ built-in type `char8_t` `[-fpermissive]`
Add this to your `etl_profile.h` or define it in the project properties.  
`#define ETL_NO_SMALL_CHAR_SUPPORT 0`  
You may also want to define `#define ETL_CPP20_SUPPORTED 0`

## Arduino
**Arduino IDE**  
**Visual Studio with Visual Micro extension**  
The Arduino programming platform is not supplied with an implementation of the STL. The ETL can use of somefeatures of the STL to compile, and therefore  either one must be acquired or the project is compiled with `ETL_NO_STL` defined.
A suitable STL implementation may be downloaded from the Arduino library resource; It is called **ArduinoSTL**.  

Add `#include <ArduinoSTL.h>` to the start of every file that uses the ETL.
Remember to add the directory to the include path.

The alternative option is to compile with `ETL_NO_STL` defined, if the STL is not used in your project.
Partially tested for Zero, Uno, Yún, Mega, Industrial 101 and Leonardo boards using `test_embedded_compile.cpp`

### Issues
**Zero**  
**Arduino101**  
**TinyTile**  
`<algorithm>` must be edited to move the template function iter_swap before the declaration of swap_ranges.The template class variant will not currently compile unless the header file is either renamed or the include path somehow altered, as the platform also defines a header named `variant.h`.

**Duo**  
Fails to find `ArduinoSTL.h` even though it is present.
If `ArduinoSTL.h` is removed, then fails with printf has not been declared.

**General**  
It may be necessary to add the following lines to the beginning of the file.
```cpp
#undef min
#undef max
```
