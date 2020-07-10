///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#include <math.h>

#include "determine_compiler.h"

#if !defined(ETL_CPP11_SUPPORTED) && !defined(ETL_CPP14_SUPPORTED) && !defined(ETL_CPP17_SUPPORTED)
  #if defined(__cplusplus)
    #if defined(ETL_COMPILER_MICROSOFT)
      #define ETL_CPP11_SUPPORTED (_MSC_VER >= 1600)
      #define ETL_CPP14_SUPPORTED (_MSC_VER >= 1900)
      #define ETL_CPP17_SUPPORTED (_MSC_VER >= 1914)
      #define ETL_CPP20_SUPPORTED 0
    #elif defined(ETL_COMPILER_ARM5)
      #define ETL_CPP11_SUPPORTED 0
      #define ETL_CPP14_SUPPORTED 0
      #define ETL_CPP17_SUPPORTED 0
      #define ETL_CPP20_SUPPORTED 0
    #else
      #define ETL_CPP11_SUPPORTED (__cplusplus >= 201103L)
      #define ETL_CPP14_SUPPORTED (__cplusplus >= 201402L)
      #define ETL_CPP17_SUPPORTED (__cplusplus >= 201703L)
      #define ETL_CPP20_SUPPORTED 0
    #endif
  #else
    #define ETL_CPP11_SUPPORTED 0
    #define ETL_CPP14_SUPPORTED 0
    #define ETL_CPP17_SUPPORTED 0
    #define ETL_CPP20_SUPPORTED 0
  #endif
#endif

// Helper macros
#define ETL_CPP11_NOT_SUPPORTED !ETL_CPP11_SUPPORTED
#define ETL_CPP14_NOT_SUPPORTED !ETL_CPP14_SUPPORTED
#define ETL_CPP17_NOT_SUPPORTED !ETL_CPP17_SUPPORTED

#if !defined(ETL_NO_NULLPTR_SUPPORT)
  #define ETL_NO_NULLPTR_SUPPORT ETL_CPP11_NOT_SUPPORTED
#endif

#if !defined(ETL_NO_LARGE_CHAR_SUPPORT)
  #define ETL_NO_LARGE_CHAR_SUPPORT ETL_CPP11_NOT_SUPPORTED
#endif

#if !defined(ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED)
  #define ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED ETL_CPP14_SUPPORTED
#endif

// NAN not defined or Rowley CrossWorks
#if !defined(NAN) || defined(__CROSSWORKS_ARM) || defined(ETL_COMPILER_ARM5)
  #define ETL_NO_CPP_NAN_SUPPORT
#endif

#endif
