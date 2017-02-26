///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include <stdint.h>
#include <limits.h>

// Define the compiler.
#if defined(__IAR_SYSTEMS_ICC__)
  #define ETL_COMPILER_IAR
#elif defined(__KEIL__) && !defined(__GNUC__)
  #define ETL_COMPILER_KEIL
#elif defined(__ghs__)
  #define ETL_COMPILER_GREEN_HILLS
#elif defined(__INTEL_COMPILER)
  #define ETL_COMPILER_INTEL
#elif defined(_MSC_VER)
  #define ETL_COMPILER_MICROSOFT
#elif defined(__GNUC__)
  #define ETL_COMPILER_GCC
#elif defined(__TI_COMPILER_VERSION__) && defined(__MSP430__)
  #define ETL_COMPILER_TI_MSP430
#elif defined(_MRI)
  #define ETL_COMPILER_MICROTEC
#elif defined(__HIGHC__)
  #define ETL_COMPILER_METAWARE_HIGH
#elif defined(__llvm__)
  #define ETL_COMPILER_LLVM
#elif defined(__KCC_VERSION)
  #define ETL_COMPILER_KAI
#elif defined(_COMO__)
  #define ETL_COMPILER_COMEAU
#elif defined(__BORLANDC__)
  #define ETL_COMPILER_BORLAND
#elif defined(__CC_ARM)
  #define ETL_COMPILER_ARM
#elif defined(__MRC__)
  #define ETL_COMPILER_MPW
#else
  #define ETL_COMPILER_GENERIC
#endif

// Check to see if the compiler supports nullptr and large character types.
#if (defined(ETL_COMPILER_MICROSOFT) && (_MSC_VER < 1600)) || \
     defined(ETL_COMPILER_KEIL) || \
     defined(ETL_COMPILER_TI_MSP430) || \
     defined(ETL_COMPILER_IAR) || \
     (defined(ETL_COMPILER_GCC) && (__cplusplus < 201103L))
  #define ETL_NO_NULLPTR_SUPPORT
  #define ETL_NO_LARGE_CHAR_SUPPORT
#endif

// Check to see if the compiler supports static_assert.
#if (defined(ETL_COMPILER_MICROSOFT) && (_MSC_VER >= 1600)) || \
    (defined(ETL_COMPILER_GCC) && (__cplusplus >= 201103L))
#define ETL_STATIC_ASSERT_SUPPORTED
#endif

// Some targets do not support 8bit types.
#define ETL_8BIT_SUPPORT (CHAR_BIT == 8)

#if defined(_DEBUG) || defined(DEBUG)
  #define ETL_DEBUG
#endif