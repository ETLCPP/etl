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

#ifndef ETL_DETERMINE_COMPILER_VERSION_H_INCLUDED
#define ETL_DETERMINE_COMPILER_VERSION_H_INCLUDED

#include "determine_compiler.h"

#if !defined(ETL_COMPILER_VERSION) && !defined(ETL_COMPILER_FULL_VERSION)
  #if defined(ETL_COMPILER_GCC)
    #define ETL_COMPILER_VERSION      __GNUC__
    #define ETL_COMPILER_FULL_VERSION ((__GNUC__ * 10000) + (__GNUC_MINOR__ * 100) + __GNUC_PATCHLEVEL__)
  #elif defined(ETL_COMPILER_MICROSOFT)
    #define ETL_COMPILER_VERSION      _MSC_VER
    #define ETL_COMPILER_FULL_VERSION _MSC_FULL_VER
  #elif defined(ETL_COMPILER_ARM5)
    #define ETL_COMPILER_VERSION      __ARMCC_VERSION
    #define ETL_COMPILER_FULL_VERSION __ARMCC_VERSION
  #elif defined(ETL_COMPILER_ARM6)
    #define ETL_COMPILER_VERSION      __ARMCOMPILER_VERSION
    #define ETL_COMPILER_FULL_VERSION __ARMCOMPILER_VERSION
  #elif defined(ETL_COMPILER_ARM7)
    #define ETL_COMPILER_VERSION      __ARMCOMPILER_VERSION
    #define ETL_COMPILER_FULL_VERSION __ARMCOMPILER_VERSION
  #elif defined(ETL_COMPILER_CLANG)
    #define ETL_COMPILER_VERSION      __clang_version__
    #define ETL_COMPILER_FULL_VERSION ((__clang_major__ * 10000) + (__clang_minor__ * 100) + __clang_patchlevel__)
  #elif defined(ETL_COMPILER_GREEN_HILLS)
    #define ETL_COMPILER_VERSION      __GHS_VERSION_NUMBER__
    #define ETL_COMPILER_FULL_VERSION __GHS_VERSION_NUMBER__
  #elif defined(ETL_COMPILER_IAR)
    #define ETL_COMPILER_VERSION      __VER__
    #define ETL_COMPILER_FULL_VERSION __VER__
  #elif defined(ETL_COMPILER_INTEL)
    #define ETL_COMPILER_VERSION      __INTEL_COMPILER
    #define ETL_COMPILER_FULL_VERSION __INTEL_COMPILER
  #elif defined(ETL_COMPILER_TEXAS_INSTRUMENTS)
    #define ETL_COMPILER_VERSION      __TI_COMPILER_VERSION__
    #define ETL_COMPILER_FULL_VERSION __TI_COMPILER_VERSION__
  #elif defined(ETL_COMPILER_TASKING)
    #define ETL_COMPILER_VERSION      __REVISION__
    #define ETL_COMPILER_FULL_VERSION __VERSION__
  #else
    #define ETL_COMPILER_VERSION      0
    #define ETL_COMPILER_FULL_VERSION 0
  #endif
#endif

#endif
