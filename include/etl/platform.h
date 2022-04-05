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

#include "file_error_numbers.h"

//*************************************
// Include the user's profile definition.
#if !defined(ETL_NO_PROFILE_HEADER) && defined(__has_include)
  #if !__has_include("etl_profile.h")
    #define ETL_NO_PROFILE_HEADER
  #endif
#endif

#if !defined(ETL_NO_PROFILE_HEADER)
  #include "etl_profile.h"
#endif

// Determine the bit width of the platform.
#define ETL_PLATFORM_16BIT (UINT16_MAX == UINTPTR_MAX)
#define ETL_PLATFORM_32BIT (UINT32_MAX == UINTPTR_MAX)
#define ETL_PLATFORM_64BIT (UINT64_MAX == UINTPTR_MAX)

//*************************************
// Define debug macros.
#if (defined(_DEBUG) || defined(DEBUG)) && !defined(ETL_DEBUG) 
  #define ETL_DEBUG
#endif

#if defined(ETL_DEBUG)
  #define ETL_IS_DEBUG_BUILD 1
#else
  #define ETL_IS_DEBUG_BUILD 0
#endif

//*************************************
// Helper macros, so we don't have to use double negatives.
// The ETL will use the STL, unless ETL_NO_STL is defined.
// With this macro we can use '#if ETL_USING_STL' instead of '#if !ETL_NO_STL' in the code.
#if defined(ETL_NO_STL)
  #define ETL_USING_STL     0
  #define ETL_NOT_USING_STL 1
#else
  #define ETL_USING_STL     1
  #define ETL_NOT_USING_STL 0
#endif

//*************************************
// Helper macros for ETL_STLPORT.
#if defined(ETL_STLPORT)
  #define ETL_USING_STLPORT     1
  #define ETL_NOT_USING_STLPORT 0
#else
  #define ETL_USING_STLPORT     0
  #define ETL_NOT_USING_STLPORT 1
#endif

//*************************************
// Some targets do not support 8bit types.
#if (CHAR_BIT == 8)
  #define ETL_USING_8BIT_TYPES     1
  #define ETL_NOT_USING_8BIT_TYPES 0
#else
  #define ETL_USING_8BIT_TYPES     0
  #define ETL_NOT_USING_8BIT_TYPES 1
#endif

#define ETL_8BIT_SUPPORT (CHAR_BIT == 8) // Deprecated

//*************************************
// Helper macro for ETL_NO_64BIT_TYPES.
#if defined(ETL_NO_64BIT_TYPES)
  #define ETL_USING_64BIT_TYPES     0
  #define ETL_NOT_USING_64BIT_TYPES 1
#else
  #define ETL_USING_64BIT_TYPES     1
  #define ETL_NOT_USING_64BIT_TYPES 0
#endif

//*************************************
// Figure out things about the compiler, if haven't already done so in etl_profile.h
#include "profiles/determine_compiler_version.h"
#include "profiles/determine_compiler_language_support.h"

//*************************************
// See if we can determine the OS we're compiling on, if haven't already done so in etl_profile.h
#include "profiles/determine_development_os.h"

//*************************************
// Check WCHAR_MIN and WCHAR_MAX
#if !defined(WCHAR_MIN)
  #define WCHAR_MIN 0x0000
#endif

#if !defined(WCHAR_MAX)
  #define WCHAR_MAX 0xFFFF
#endif

//*************************************
// Option to force string construction from a character pointer to be explicit.
#if defined(ETL_FORCE_EXPLICIT_STRING_CONVERSION_FROM_CHAR)
  #define ETL_EXPLICIT_STRING_FROM_CHAR explicit
#else
  #define ETL_EXPLICIT_STRING_FROM_CHAR
#endif

//*************************************
// Option to disable truncation checks for strings.
#if defined(ETL_DISABLE_STRING_TRUNCATION_CHECKS)
  #define ETL_HAS_STRING_TRUNCATION_CHECKS 0
#else
  #define ETL_HAS_STRING_TRUNCATION_CHECKS 1
#endif

//*************************************
// Option to disable clear-after-use functionality for strings.
#if defined(ETL_DISABLE_STRING_CLEAR_AFTER_USE)
  #define ETL_HAS_STRING_CLEAR_AFTER_USE 0
#else
  #define ETL_HAS_STRING_CLEAR_AFTER_USE 1
#endif

//*************************************
// Option to make string truncation an error.
#if ETL_HAS_ERROR_ON_STRING_TRUNCATION
  #define ETL_HAS_ERROR_ON_STRING_TRUNCATION 1
#else
  #define ETL_HAS_ERROR_ON_STRING_TRUNCATION 0
#endif

//*************************************
// Option to enable repair-after-memcpy for istrings.
#if defined(ETL_ISTRING_REPAIR_ENABLE)
  #define ETL_HAS_ISTRING_REPAIR 1
#else
  #define ETL_HAS_ISTRING_REPAIR 0
#endif

//*************************************
// Option to enable repair-after-memcpy for ivector.
#if defined(ETL_IVECTOR_REPAIR_ENABLE)
  #define ETL_HAS_IVECTOR_REPAIR 1
#else
  #define ETL_HAS_IVECTOR_REPAIR 0
#endif

//*************************************
// Option to enable repair-after-memcpy for ideque.
#if defined(ETL_IDEQUE_REPAIR_ENABLE)
  #define ETL_HAS_IDEQUE_REPAIR 1
#else
  #define ETL_HAS_IDEQUE_REPAIR 0
#endif

//*************************************
// Indicate if C++ exceptions are enabled.
#if defined(ETL_THROW_EXCEPTIONS)
  #define ETL_USING_EXCEPTIONS 1
#else
  #define ETL_USING_EXCEPTIONS 0
#endif

//*************************************
// Indicate if nullptr is used.
#if ETL_NO_NULLPTR_SUPPORT
#define ETL_HAS_NULLPTR 0
#else
#define ETL_HAS_NULLPTR 1
#endif

//*************************************
// Indicate if there are large char types.
#if defined(ETL_NO_LARGE_CHAR_SUPPORT)
  #define ETL_HAS_LARGE_CHAR 0
#else
  #define ETL_HAS_LARGE_CHAR 1
#endif

//*************************************
// Indicate if array_view is mutable.
#if defined(ETL_ARRAY_VIEW_IS_MUTABLE)
  #define ETL_HAS_MUTABLE_ARRAY_VIEW 1
#else
  #define ETL_HAS_MUTABLE_ARRAY_VIEW 0
#endif

//*************************************
// The macros below are dependent on the profile.
// C++11
#if ETL_USING_CPP11 && !defined(ETL_FORCE_NO_ADVANCED_CPP)
  #define ETL_CONSTEXPR constexpr
  #define ETL_CONSTANT constexpr
  #define ETL_DELETE = delete
  #define ETL_EXPLICIT explicit
  #define ETL_OVERRIDE override
  #define ETL_FINAL final
  #define ETL_NORETURN [[noreturn]]
  #define ETL_MOVE(x) etl::move(x)

  #if ETL_USING_EXCEPTIONS
    #define ETL_NOEXCEPT  noexcept
    #define ETL_NOEXCEPT_EXPR(expression) noexcept(expression)
  #else
    #define ETL_NOEXCEPT
    #define ETL_NOEXCEPT_EXPR(expression)
  #endif
#else
  #define ETL_CONSTEXPR
  #define ETL_CONSTANT const
  #define ETL_DELETE
  #define ETL_EXPLICIT
  #define ETL_OVERRIDE
  #define ETL_FINAL
  #define ETL_NORETURN
  #define ETL_NOEXCEPT
  #define ETL_NOEXCEPT_EXPR(expression)
  #define ETL_MOVE(x) x
#endif

//*************************************
// C++14
#if ETL_USING_CPP14 && !defined(ETL_FORCE_NO_ADVANCED_CPP)
  #define ETL_CONSTEXPR14 constexpr
  #define ETL_DEPRECATED [[deprecated]]
  #define ETL_DEPRECATED_REASON(reason) [[deprecated(reason)]]
#else
  #define ETL_CONSTEXPR14
  #define ETL_DEPRECATED
  #define ETL_DEPRECATED_REASON(reason)
#endif

//*************************************
// C++17
#if ETL_USING_CPP17 && !defined(ETL_FORCE_NO_ADVANCED_CPP)
  #define ETL_CONSTEXPR17 constexpr
  #define ETL_IF_CONSTEXPR constexpr
  #define ETL_NODISCARD [[nodiscard]]
  #define ETL_MAYBE_UNUSED [[maybe_unused]]
  #define ETL_FALLTHROUGH [[fallthrough]]
  #define ETL_INLINE_VAR inline
#else
  #define ETL_CONSTEXPR17
  #define ETL_IF_CONSTEXPR
  #define ETL_NODISCARD
  #define ETL_MAYBE_UNUSED
  #define ETL_FALLTHROUGH
  #define ETL_INLINE_VAR
#endif

//*************************************
// C++20
#if ETL_USING_CPP20 && !defined(ETL_FORCE_NO_ADVANCED_CPP)
  #define ETL_LIKELY [[likely]]
  #define ETL_UNLIKELY [[unlikely]]
  #define ETL_CONSTEXPR20 constexpr
  #define ETL_CONSTEVAL consteval
  #define ETL_CONSTINIT constinit
  #define ETL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#else
  #define ETL_LIKELY
  #define ETL_UNLIKELY
  #define ETL_CONSTEXPR20
  #define ETL_CONSTEVAL
  #define ETL_CONSTINIT
  #define ETL_NO_UNIQUE_ADDRESS
#endif

//*************************************
// Determine if the ETL can use std::array
#if !defined(ETL_HAS_STD_ARRAY)
  #if ETL_USING_CPP11 && ETL_USING_STL
    #define ETL_HAS_STD_ARRAY 1
  #else
    #define ETL_HAS_STD_ARRAY 0
  #endif
#endif

//*************************************
// Determine if the ETL should support atomics.
#if defined(ETL_NO_ATOMICS) || \
    defined(ETL_TARGET_DEVICE_ARM_CORTEX_M0) || \
    defined(ETL_TARGET_DEVICE_ARM_CORTEX_M0_PLUS)
  #define ETL_HAS_ATOMIC 0
#else
  #if ((ETL_USING_CPP11 && (ETL_USING_STL || defined(ETL_IN_UNIT_TEST))) || \
        defined(ETL_COMPILER_ARM5)  || \
        defined(ETL_COMPILER_ARM6)  || \
        defined(ETL_COMPILER_GCC)   || \
        defined(ETL_COMPILER_CLANG))
    #define ETL_HAS_ATOMIC 1
  #else
    #define ETL_HAS_ATOMIC 0
  #endif
#endif

//*************************************
// Set force flag to 0 if not already set.
#if !defined(ETL_FORCE_CONSTEXPR_ALGORITHMS)
  #define ETL_FORCE_CONSTEXPR_ALGORITHMS 0
#endif

//*************************************
// Check for availability of certain builtins
#include "profiles/determine_builtin_support.h"

//*************************************
// Sort out namespaces for STL/No STL options.
#include "private/choose_namespace.h"

namespace etl
{
  namespace traits
  {
    // Documentation: https://www.etlcpp.com/etl_traits.html

    static ETL_CONSTANT bool using_stl                        = (ETL_USING_STL == 1);
    static ETL_CONSTANT bool using_stlport                    = (ETL_USING_STLPORT == 1);
    static ETL_CONSTANT bool using_cpp11                      = (ETL_USING_CPP11 == 1);
    static ETL_CONSTANT bool using_cpp14                      = (ETL_USING_CPP14 == 1);
    static ETL_CONSTANT bool using_cpp17                      = (ETL_USING_CPP17 == 1);
    static ETL_CONSTANT bool using_cpp20                      = (ETL_USING_CPP20 == 1);
    static ETL_CONSTANT bool using_cpp23                      = (ETL_USING_CPP23 == 1);
    static ETL_CONSTANT bool using_exceptions                 = (ETL_USING_EXCEPTIONS == 1);
    static ETL_CONSTANT bool using_gcc_compiler               = (ETL_USING_GCC_COMPILER == 1);
    static ETL_CONSTANT bool using_microsoft_compiler         = (ETL_USING_MICROSOFT_COMPILER == 1);
    static ETL_CONSTANT bool using_arm5_compiler              = (ETL_USING_ARM5_COMPILER == 1);
    static ETL_CONSTANT bool using_arm6_compiler              = (ETL_USING_ARM6_COMPILER == 1);
    static ETL_CONSTANT bool using_arm7_compiler              = (ETL_USING_ARM7_COMPILER == 1);
    static ETL_CONSTANT bool using_clang_compiler             = (ETL_USING_CLANG_COMPILER == 1);
    static ETL_CONSTANT bool using_green_hills_compiler       = (ETL_USING_GREEN_HILLS_COMPILER == 1);
    static ETL_CONSTANT bool using_iar_compiler               = (ETL_USING_IAR_COMPILER == 1);
    static ETL_CONSTANT bool using_intel_compiler             = (ETL_USING_INTEL_COMPILER == 1);
    static ETL_CONSTANT bool using_texas_instruments_compiler = (ETL_USING_TEXAS_INSTRUMENTS_COMPILER == 1);
    static ETL_CONSTANT bool using_generic_compiler           = (ETL_USING_GENERIC_COMPILER == 1);
    static ETL_CONSTANT bool has_8bit_types                   = (ETL_USING_8BIT_TYPES == 1);
    static ETL_CONSTANT bool has_64bit_types                  = (ETL_USING_64BIT_TYPES == 1);
    static ETL_CONSTANT bool has_atomic                       = (ETL_HAS_ATOMIC == 1);
    static ETL_CONSTANT bool has_nullptr                      = (ETL_HAS_NULLPTR == 1);
    static ETL_CONSTANT bool has_large_char                   = (ETL_HAS_LARGE_CHAR == 1);
    static ETL_CONSTANT bool has_string_truncation_checks     = (ETL_HAS_STRING_TRUNCATION_CHECKS == 1);
    static ETL_CONSTANT bool has_error_on_string_truncation   = (ETL_HAS_ERROR_ON_STRING_TRUNCATION == 1);
    static ETL_CONSTANT bool has_string_clear_after_use       = (ETL_HAS_STRING_CLEAR_AFTER_USE == 1);
    static ETL_CONSTANT bool has_istring_repair               = (ETL_HAS_ISTRING_REPAIR == 1);
    static ETL_CONSTANT bool has_ivector_repair               = (ETL_HAS_IVECTOR_REPAIR == 1);
    static ETL_CONSTANT bool has_mutable_array_view           = (ETL_HAS_MUTABLE_ARRAY_VIEW == 1);
    static ETL_CONSTANT bool has_ideque_repair                = (ETL_HAS_IDEQUE_REPAIR == 1);
    static ETL_CONSTANT bool is_debug_build                   = (ETL_IS_DEBUG_BUILD == 1);
    static ETL_CONSTANT long cplusplus                        = __cplusplus;
  }
}

#endif
