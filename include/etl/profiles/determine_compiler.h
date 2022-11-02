///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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

#ifndef ETL_DETERMINE_COMPILER_H_INCLUDED
#define ETL_DETERMINE_COMPILER_H_INCLUDED

//*****************************************************************************
// Macros that are conditionally defined.
//*****************************************************************************
#if !defined(ETL_COMPILER_GCC) && \
    !defined(ETL_COMPILER_MICROSOFT) && \
    !defined(ETL_COMPILER_ARM5) && \
    !defined(ETL_COMPILER_ARM6) && \
    !defined(ETL_COMPILER_ARM7) && \
    !defined(ETL_COMPILER_CLANG) && \
    !defined(ETL_COMPILER_GREEN_HILLS) && \
    !defined(ETL_COMPILER_IAR) && \
    !defined(ETL_COMPILER_INTEL) && \
    !defined(ETL_COMPILER_TEXAS_INSTRUMENTS) && \
    !defined(ETL_COMPILER_GENERIC)

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_ARM5)
    #if defined(__CC_ARM)
      #define ETL_COMPILER_ARM5
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_ARM6)
    #if defined(__ARMCOMPILER_VERSION) && (__ARMCOMPILER_VERSION >= 6000000L) && (__ARMCOMPILER_VERSION < 7000000L)
      #define ETL_COMPILER_ARM6
      #define ETL_COMPILER_TYPE_DETECTED 1
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_ARM7)
    #if defined(__ARMCOMPILER_VERSION) && (__ARMCOMPILER_VERSION >= 7000000L) && (__ARMCOMPILER_VERSION < 8000000L)
      #define ETL_COMPILER_ARM7
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_GCC)
    #if defined(__GNUC__) && !defined(__clang__) && !defined(__llvm__)
      #define ETL_COMPILER_GCC
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_CLANG)
    #if defined(__clang__) || defined(__llvm__)
      #define ETL_COMPILER_CLANG
      #define ETL_COMPILER_TYPE_DETECTED
      #if __AVR__ == 1
        #define ETL_CROSS_COMPILING_TO_AVR
      #endif
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_GREEN_HILLS)
    #if defined(__ghs__)
      #define ETL_COMPILER_GREEN_HILLS
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_IAR)
    #if defined(__IAR_SYSTEMS_ICC__)
      #define ETL_COMPILER_IAR
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_INTEL)
    #if defined(__INTEL_COMPILER)
      #define ETL_COMPILER_INTEL
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_TEXAS_INSTRUMENTS)
    #if defined(__TI_COMPILER_VERSION__)
      #define ETL_COMPILER_TEXAS_INSTRUMENTS
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_MICROSOFT)
    #if defined(_MSC_VER)
      #define ETL_COMPILER_MICROSOFT
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED) && !defined(ETL_COMPILER_TASKING)
    #if defined(__TASKING__)
      #define ETL_COMPILER_TASKING
      #define ETL_COMPILER_TYPE_DETECTED
    #endif
  #endif

  #if !defined(ETL_COMPILER_TYPE_DETECTED)
    #define ETL_COMPILER_GENERIC
  #endif
#endif

//*****************************************************************************
// 'Using' macros that are always defined.
//*****************************************************************************
#if defined(ETL_COMPILER_GCC)
  #define ETL_USING_GCC_COMPILER 1
#else
  #define ETL_USING_GCC_COMPILER 0
#endif

#if defined(ETL_COMPILER_MICROSOFT)
  #define ETL_USING_MICROSOFT_COMPILER 1
#else
  #define ETL_USING_MICROSOFT_COMPILER 0
#endif

#if defined(ETL_COMPILER_ARM5)
  #define ETL_USING_ARM5_COMPILER 1
#else
  #define ETL_USING_ARM5_COMPILER 0
#endif

#if defined(ETL_COMPILER_ARM6)
  #define ETL_USING_ARM6_COMPILER 1
#else
  #define ETL_USING_ARM6_COMPILER 0
#endif

#if defined(ETL_COMPILER_ARM7)
  #define ETL_USING_ARM7_COMPILER 1
#else
  #define ETL_USING_ARM7_COMPILER 0
#endif

#if defined(ETL_COMPILER_CLANG)
  #define ETL_USING_CLANG_COMPILER 1
#else
  #define ETL_USING_CLANG_COMPILER 0
#endif

#if defined(ETL_COMPILER_GREEN_HILLS)
  #define ETL_USING_GREEN_HILLS_COMPILER 1
#else
  #define ETL_USING_GREEN_HILLS_COMPILER 0
#endif

#if defined(ETL_COMPILER_IAR)
  #define ETL_USING_IAR_COMPILER 1
#else
  #define ETL_USING_IAR_COMPILER 0
#endif

#if defined(ETL_COMPILER_INTEL)
  #define ETL_USING_INTEL_COMPILER 1
#else
  #define ETL_USING_INTEL_COMPILER 0
#endif

#if defined(ETL_COMPILER_TEXAS_INSTRUMENTS)
  #define ETL_USING_TEXAS_INSTRUMENTS_COMPILER 1
#else
  #define ETL_USING_TEXAS_INSTRUMENTS_COMPILER 0
#endif

#if defined(ETL_COMPILER_TASKING)
  #define ETL_USING_TASKING_COMPILER 1
#else
  #define ETL_USING_TASKING_COMPILER 0
#endif

#if defined(ETL_COMPILER_GENERIC)
  #define ETL_USING_GENERIC_COMPILER 1
#else
  #define ETL_USING_GENERIC_COMPILER 0
#endif

#endif
