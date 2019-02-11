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

#include <stdint.h>
#include <limits.h>

#ifndef ETL_PLATFORM_INCLUDED
#define ETL_PLATFORM_INCLUDED

// Some targets do not support 8bit types.
#define ETL_8BIT_SUPPORT (CHAR_BIT == 8)

// Define a debug macro
#if defined(_DEBUG) || defined(DEBUG)
#define ETL_DEBUG
#endif

// Undefine all of the macros.
#undef ETL_PLATFORM_16BIT 
#undef ETL_PLATFORM_32BIT
#undef ETL_PLATFORM_64BIT
#undef ETL_CPP11_SUPPORTED
#undef ETL_CPP14_SUPPORTED
#undef ETL_CPP17_SUPPORTED
#undef ETL_NO_NULLPTR_SUPPORT
#undef ETL_NO_LARGE_CHAR_SUPPORT
#undef ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED
#undef ETL_STD_ATOMIC_SUPPORTED

// Determine the bit width of the platform.
#define ETL_PLATFORM_16BIT (UINT16_MAX == UINTPTR_MAX)
#define ETL_PLATFORM_32BIT (UINT32_MAX == UINTPTR_MAX)
#define ETL_PLATFORM_64BIT (UINT64_MAX == UINTPTR_MAX)

#include "etl_profile.h"

// The macros below are dependent on the profile.

#if defined(ETL_COMPILER_MICROSOFT)
  // Disable warning of deprecated std::iterator.
  #pragma warning(disable : 4996)
#endif

#if defined(ETL_COMPILER_GCC)
  #define ETL_COMPILER_VERSION      __GNUC__
  #define ETL_COMPILER_FULL_VERSION ((__GNUC__ * 10000) + (__GNUC_MINOR__ * 100) + __GNUC_PATCHLEVEL__)
#elif defined ETL_COMPILER_MICROSOFT
  #define ETL_COMPILER_VERSION      _MSC_VER
  #define ETL_COMPILER_FULL_VERSION _MSC_FULL_VER
#endif

#if ETL_CPP11_SUPPORTED
  #define ETL_CONSTEXPR constexpr
#else
  #define ETL_CONSTEXPR
#endif

#if ETL_CPP17_SUPPORTED
  #define ETL_IF_CONSTEXPR constexpr
#else
  #define ETL_IF_CONSTEXPR
#endif

#if ETL_CPP11_SUPPORTED
  #define ETL_DELETE = delete
#else
  #define ETL_DELETE
#endif

#if ETL_CPP11_SUPPORTED
  #define ETL_NOEXCEPT noexcept
  #define ETL_NOEXCEPT_EXPR(expression) noexcept(expression)
#else
  #define ETL_NOEXCEPT
  #define ETL_NOEXCEPT_EXPR(expression)
#endif

#endif
