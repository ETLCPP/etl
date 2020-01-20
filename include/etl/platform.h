///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#ifndef ETL_PLATFORM_INCLUDED
#define ETL_PLATFORM_INCLUDED

#include <stdint.h>
#include <limits.h>

// Some targets do not support 8bit types.
#define ETL_8BIT_SUPPORT (CHAR_BIT == 8)

// Define a debug macro
#if !defined(ETL_DEBUG)
  #if defined(_DEBUG) || defined(DEBUG)
    #define ETL_DEBUG
  #endif
#endif

// Determine the bit width of the platform.
#define ETL_PLATFORM_16BIT (UINT16_MAX == UINTPTR_MAX)
#define ETL_PLATFORM_32BIT (UINT32_MAX == UINTPTR_MAX)
#define ETL_PLATFORM_64BIT (UINT64_MAX == UINTPTR_MAX)

// Include the user's profile definition.
#include "etl_profile.h"

#if defined(ETL_AUTO_DETERMINE_COMPILER_INFO)
  // Figure out things about the compiler.
  #include "profiles/determine_compiler.h"
  #include "profiles/determine_compiler_version.h"
  #include "profiles/determine_compiler_language_support.h"
#endif

#if defined(ETL_FORCE_EXPLICIT_STRING_CONVERSION_FROM_CHAR)
#define ETL_EXPLICIT_STRING_FROM_CHAR explicit
#else
#define ETL_EXPLICIT_STRING_FROM_CHAR
#endif

// The macros below are dependent on the profile.
// C++11
#if ETL_CPP11_SUPPORTED && !defined(ETL_FORCE_NO_ADVANCED_CPP)
  #define ETL_CONSTEXPR constexpr
  #define ETL_CONST_OR_CONSTEXPR constexpr
  #define ETL_DELETE    = delete
  #define ETL_NOEXCEPT  noexcept
  #define ETL_NOEXCEPT_EXPR(expression) noexcept(expression)
  #define ETL_EXPLICIT explicit
#else
  #define ETL_CONSTEXPR
  #define ETL_CONST_OR_CONSTEXPR const
  #define ETL_DELETE
  #define ETL_NOEXCEPT
  #define ETL_NOEXCEPT_EXPR(expression)
  #define ETL_EXPLICIT
#endif

// C++14
#if ETL_CPP14_SUPPORTED && !defined(ETL_FORCE_NO_ADVANCED_CPP)
  #define ETL_CONSTEXPR14 constexpr
#else
  #define ETL_CONSTEXPR14
#endif

// C++17
#if ETL_CPP17_SUPPORTED && !defined(ETL_FORCE_NO_ADVANCED_CPP)
  #define ETL_CONSTEXPR17 constexpr
  #define ETL_IF_CONSTEXPR constexpr
  #define ETL_NODISCARD [[nodiscard]]
#else
  #define ETL_CONSTEXPR17
  #define ETL_IF_CONSTEXPR
  #define ETL_NODISCARD
#endif

// Sort out namespaces for STL/No STL options.
#include "private/choose_namespace.h"

#endif
