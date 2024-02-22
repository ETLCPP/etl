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

#ifndef ETL_DETERMINE_COMPILER_LANGUAGE_SUPPORT_H_INCLUDED
#define ETL_DETERMINE_COMPILER_LANGUAGE_SUPPORT_H_INCLUDED

//#include <math.h>

#include "determine_compiler.h"

// Determine C++23 support
#if !defined(ETL_CPP23_SUPPORTED)
  #define ETL_CPP23_SUPPORTED 0
#endif

#if ETL_CPP23_SUPPORTED
  #define ETL_CPP11_SUPPORTED 1
  #define ETL_CPP14_SUPPORTED 1
  #define ETL_CPP17_SUPPORTED 1
  #define ETL_CPP20_SUPPORTED 1
#endif

// Determine C++20 support
#if !defined(ETL_CPP20_SUPPORTED)
  #if defined(__cplusplus)
    #if defined(ETL_COMPILER_MICROSOFT)
      #if defined(_MSVC_LANG)
        #define ETL_CPP20_SUPPORTED (_MSVC_LANG >= 202002L)
      #else
        #define ETL_CPP20_SUPPORTED (_MSC_VER >= 1929)
      #endif
    #elif defined(ETL_COMPILER_ARM5)
      #define ETL_CPP20_SUPPORTED 0
    #else
      #define ETL_CPP20_SUPPORTED (__cplusplus >= 202002L)
    #endif
  #else
    #define ETL_CPP20_SUPPORTED 0
  #endif
#endif

#if ETL_CPP20_SUPPORTED
  #define ETL_CPP11_SUPPORTED 1
  #define ETL_CPP14_SUPPORTED 1
  #define ETL_CPP17_SUPPORTED 1
#endif

// Determine C++17 support
#if !defined(ETL_CPP17_SUPPORTED)
  #if defined(__cplusplus)
    #if defined(ETL_COMPILER_MICROSOFT)
      #if defined(_MSVC_LANG)
        #define ETL_CPP17_SUPPORTED (_MSVC_LANG >= 201703L)
      #else
        #define ETL_CPP17_SUPPORTED (_MSC_VER >= 1914)
      #endif
    #elif defined(ETL_COMPILER_ARM5)
      #define ETL_CPP17_SUPPORTED 0
    #else
      #define ETL_CPP17_SUPPORTED (__cplusplus >= 201703L)
    #endif
  #else
    #define ETL_CPP17_SUPPORTED 0
  #endif
#endif

#if ETL_CPP17_SUPPORTED
  #define ETL_CPP11_SUPPORTED 1
  #define ETL_CPP14_SUPPORTED 1
#endif

// Determine C++14 support
#if !defined(ETL_CPP14_SUPPORTED)
  #if defined(__cplusplus)
    #if defined(ETL_COMPILER_MICROSOFT)
      #if defined(_MSVC_LANG)
        #define ETL_CPP14_SUPPORTED (_MSVC_LANG >= 201402L)
      #else
        #define ETL_CPP14_SUPPORTED (_MSC_VER >= 1900)
      #endif
    #elif defined(ETL_COMPILER_ARM5)
      #define ETL_CPP14_SUPPORTED 0
    #else
      #define ETL_CPP14_SUPPORTED (__cplusplus >= 201402L)
    #endif
  #else
    #define ETL_CPP14_SUPPORTED 0
  #endif
#endif

#if ETL_CPP14_SUPPORTED
  #define ETL_CPP11_SUPPORTED 1
#endif

// Determine C++11 support
#if !defined(ETL_CPP11_SUPPORTED)
  #if defined(__cplusplus)
    #if defined(ETL_COMPILER_MICROSOFT)
      #if defined(_MSVC_LANG)
        #define ETL_CPP11_SUPPORTED (_MSVC_LANG >= 201103L)
      #else
        #define ETL_CPP11_SUPPORTED (_MSC_VER >= 1700)
      #endif
    #elif defined(ETL_COMPILER_ARM5)
      #define ETL_CPP11_SUPPORTED 0
    #else
      #define ETL_CPP11_SUPPORTED (__cplusplus >= 201103L)
    #endif
  #else
    #define ETL_CPP11_SUPPORTED 0
  #endif
#endif

// Helper macros
#define ETL_CPP11_NOT_SUPPORTED (!ETL_CPP11_SUPPORTED)
#define ETL_CPP14_NOT_SUPPORTED (!ETL_CPP14_SUPPORTED)
#define ETL_CPP17_NOT_SUPPORTED (!ETL_CPP17_SUPPORTED)
#define ETL_CPP20_NOT_SUPPORTED (!ETL_CPP20_SUPPORTED)
#define ETL_CPP23_NOT_SUPPORTED (!ETL_CPP23_SUPPORTED)

// 'Using' macros
#define ETL_USING_CPP11 (ETL_CPP11_SUPPORTED == 1)
#define ETL_USING_CPP14 (ETL_CPP14_SUPPORTED == 1)
#define ETL_USING_CPP17 (ETL_CPP17_SUPPORTED == 1)
#define ETL_USING_CPP20 (ETL_CPP20_SUPPORTED == 1)
#define ETL_USING_CPP23 (ETL_CPP23_SUPPORTED == 1)

#define ETL_NOT_USING_CPP11 (ETL_CPP11_SUPPORTED == 0)
#define ETL_NOT_USING_CPP14 (ETL_CPP14_SUPPORTED == 0)
#define ETL_NOT_USING_CPP17 (ETL_CPP17_SUPPORTED == 0)
#define ETL_NOT_USING_CPP20 (ETL_CPP20_SUPPORTED == 0)
#define ETL_NOT_USING_CPP23 (ETL_CPP23_SUPPORTED == 0)

#if !defined(ETL_NO_NULLPTR_SUPPORT)
  #define ETL_NO_NULLPTR_SUPPORT ETL_NOT_USING_CPP11
#endif

#if !defined(ETL_NO_SMALL_CHAR_SUPPORT)
  #if ETL_USING_CPP20
    #define ETL_NO_SMALL_CHAR_SUPPORT 0
  #else
    #define ETL_NO_SMALL_CHAR_SUPPORT 1
  #endif
#endif

#if !defined(ETL_NO_LARGE_CHAR_SUPPORT)
  #define ETL_NO_LARGE_CHAR_SUPPORT ETL_NOT_USING_CPP11
#endif

#if !defined(ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED)
  #define ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED ETL_USING_CPP11
#endif

// Language standard
#if !defined(ETL_LANGUAGE_STANDARD)
  #if ETL_USING_CPP23
    #define ETL_LANGUAGE_STANDARD 23
  #elif ETL_USING_CPP20
    #define ETL_LANGUAGE_STANDARD 20
  #elif ETL_USING_CPP17
    #define ETL_LANGUAGE_STANDARD 17
  #elif ETL_USING_CPP14
    #define ETL_LANGUAGE_STANDARD 14
  #elif ETL_USING_CPP11
    #define ETL_LANGUAGE_STANDARD 11
  #else
    #define ETL_LANGUAGE_STANDARD 3
  #endif
#endif

// NAN not defined or Rowley CrossWorks
#if !defined(ETL_NO_CPP_NAN_SUPPORT) && (!defined(NAN) || defined(__CROSSWORKS_ARM) || defined(ETL_COMPILER_ARM5) || defined(ARDUINO))
  #define ETL_NO_CPP_NAN_SUPPORT
#endif

#endif
