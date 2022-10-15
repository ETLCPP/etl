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

#include <math.h>

#include "determine_compiler.h"

#ifdef __has_include
  #if __has_include(<version>)
    #include <version>
  #else
    #define ETL_NO_STD_FEATURE_TESTS
  #endif
#endif

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

#if !defined(ETL_NO_NULLPTR_SUPPORT)
  #define ETL_NO_NULLPTR_SUPPORT ETL_CPP11_NOT_SUPPORTED
#endif

#if !defined(ETL_NO_SMALL_CHAR_SUPPORT)
  #if __cpp_char8_t
    #define ETL_NO_SMALL_CHAR_SUPPORT 0
  #else
    #define ETL_NO_SMALL_CHAR_SUPPORT 1
  #endif
#endif

#if !defined(ETL_NO_LARGE_CHAR_SUPPORT)
#define ETL_NO_LARGE_CHAR_SUPPORT ETL_CPP11_NOT_SUPPORTED
#endif

#if !defined(ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED)
  #define ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED ETL_CPP14_SUPPORTED
#endif

// 'Using' macros
#define ETL_USING_CPP11 (ETL_CPP11_SUPPORTED == 1)
#define ETL_USING_CPP14 (ETL_CPP14_SUPPORTED == 1)
#define ETL_USING_CPP17 (ETL_CPP17_SUPPORTED == 1)
#define ETL_USING_CPP20 (ETL_CPP20_SUPPORTED == 1)
#define ETL_USING_CPP23 (ETL_CPP23_SUPPORTED == 1)

// Language standard
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

// NAN not defined or Rowley CrossWorks
#if !defined(NAN) || defined(__CROSSWORKS_ARM) || defined(ETL_COMPILER_ARM5) || defined(ARDUINO)
  #define ETL_NO_CPP_NAN_SUPPORT
#endif

#ifdef ETL_NO_STD_FEATURE_TESTS

// TODO: fill in any dependencies from https://en.cppreference.com/w/cpp/feature_test
// why testing the existence of a header is not sufficient: https://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations#example

#if (__cplusplus >= 201806L) && !defined(__cpp_lib_constexpr_algorithms)
  #define __cpp_lib_constexpr_algorithms 201806L
#endif

#if (__cplusplus >= 200704L) && !defined(__cpp_alias_templates)
  #define __cpp_alias_templates 200704L
#endif

#if (__cplusplus >= 200704L) && !defined(__cpp_variadic_templates)
  #define __cpp_variadic_templates 200704L
#endif

#if (__cplusplus >= 200610L) && !defined(__cpp_rvalue_references)
  #define __cpp_rvalue_references 200610L
#endif

#if defined(__cpp_deduction_guides)
#elif (__cplusplus >= 201907L)
  #define __cpp_deduction_guides 201907L
#elif (__cplusplus >= 201703L)
  #define __cpp_deduction_guides 201703L
#endif

#if (__cplusplus >= 200806L) && !defined(__cpp_initializer_lists)
  #define __cpp_initializer_lists 200806L
#endif

#if defined(__cpp_lib_array_constexpr)
#elif (__cplusplus >= 201811L)
  #define __cpp_lib_array_constexpr 201811L
#elif (__cplusplus >= 201603L)
  #define __cpp_lib_array_constexpr 201603L
#endif

#if defined(__cpp_constexpr)
#elif (__cplusplus >= 202207L)
  #define __cpp_constexpr 202207L
#elif (__cplusplus >= 202110L)
  #define __cpp_constexpr 202110L
#elif (__cplusplus >= 202002L)
  #define __cpp_constexpr 202002L
#elif (__cplusplus >= 201907L)
  #define __cpp_constexpr 201907L
#elif (__cplusplus >= 201603L)
  #define __cpp_constexpr 201603L
#elif (__cplusplus >= 201304L)
  #define __cpp_constexpr 201304L
#elif (__cplusplus >= 200704L)
  #define __cpp_constexpr 200704L
#endif

#if (__cplusplus >= 201907L) && !defined(__cpp_lib_bitops)
  #define __cpp_lib_bitops 201907L
#endif

#if (__cplusplus >= 201606L) && !defined(__cpp_inline_variables)
  #define __cpp_inline_variables 201606L
#endif

#if (__cplusplus >= 202110L) && !defined(__cpp_lib_byteswap)
  #define __cpp_lib_byteswap 202110L
#endif

#if (__cplusplus >= 201907L) && !defined(__cpp_lib_endian)
  #define __cpp_lib_endian 201907L
#endif

#if (__cplusplus >= 201505L) && !defined(__cpp_lib_bool_constant)
  #define __cpp_lib_bool_constant 201505L
#endif

#if (__cplusplus >= 201510L) && !defined(__cpp_lib_logical_traits)
  #define __cpp_lib_logical_traits 201510L
#endif

#if (__cplusplus >= 200710L) && !defined(__cpp_unicode_literals)
  #define __cpp_unicode_literals 200710L
#endif

#if (__cplusplus >= 201603L) && !defined(__cpp_fold_expressions)
  #define __cpp_fold_expressions 201603L
#endif

#if (__cplusplus >= 200410L) && !defined(__cpp_static_assert)
  #define __cpp_static_assert 200410L
#endif

// FIXME: no feature test macro for 'alignas' specifier

// FIXME: no feature test macro for std::array

// FIXME: no feature test macro for <atomic>

// FIXME: add ETL macro for delegate11 support -> (__cpp_variadic_templates)

// FIXME: no feature test macro for 'using'

// FIXME: no feature test macro for <type_traits>

// FIXME: no feature test macro for std::addressof

// FIXME: no feature test macro for std::construct_at / std::destroy_at

// FIXME: no feature test macro for nullptr

// FIXME: no feature test macro for std::thread and co.

#endif // ETL_NO_STD_FEATURE_TESTS

#endif
