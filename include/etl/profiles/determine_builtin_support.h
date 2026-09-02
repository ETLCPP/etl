///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#ifndef ETL_DETERMINE_BUILTIN_SUPPORT_H_INCLUDED
#define ETL_DETERMINE_BUILTIN_SUPPORT_H_INCLUDED

#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) // Set all of them to be true if not
                                          // already defined
  #if !defined(ETL_USING_BUILTIN_IS_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_ASSIGNABLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_CONSTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_NOTHROW_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_NOTHROW_ASSIGNABLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_HAS_TRIVIAL_CONSTRUCTOR)
    #define ETL_USING_BUILTIN_HAS_TRIVIAL_CONSTRUCTOR 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_HAS_TRIVIAL_DESTRUCTOR)
    #define ETL_USING_BUILTIN_HAS_TRIVIAL_DESTRUCTOR 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_DESTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_DESTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_NOTHROW_DESTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_NOTHROW_DESTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_STANDARD_LAYOUT)
    #define ETL_USING_BUILTIN_IS_STANDARD_LAYOUT 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIAL)
    #define ETL_USING_BUILTIN_IS_TRIVIAL 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_LAYOUT_COMPATIBLE)
    #define ETL_USING_BUILTIN_IS_LAYOUT_COMPATIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_POINTER_INTERCONVERTIBLE_BASE_OF)
    #define ETL_USING_BUILTIN_IS_POINTER_INTERCONVERTIBLE_BASE_OF 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_UNION)
    #define ETL_USING_BUILTIN_IS_UNION 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_EMPTY)
    #define ETL_USING_BUILTIN_IS_EMPTY 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_POLYMORPHIC)
    #define ETL_USING_BUILTIN_IS_POLYMORPHIC 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_ABSTRACT)
    #define ETL_USING_BUILTIN_IS_ABSTRACT 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_FINAL)
    #define ETL_USING_BUILTIN_IS_FINAL 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_AGGREGATE)
    #define ETL_USING_BUILTIN_IS_AGGREGATE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR)
    #define ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR 1
  #endif

  #if !defined(ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS)
    #define ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS 1
  #endif

  #if !defined(ETL_USING_BUILTIN_UNDERLYING_TYPE)
    #define ETL_USING_BUILTIN_UNDERLYING_TYPE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_CONSTANT_EVALUATED)
    #define ETL_USING_BUILTIN_IS_CONSTANT_EVALUATED 1
  #endif

  #if !defined(ETL_USING_BUILTIN_BIT_CAST)
    #define ETL_USING_BUILTIN_BIT_CAST 1
  #endif
#endif

#if defined(ETL_USE_BUILTIN_MEM_FUNCTIONS) // Set all of them to be true if not
                                           // already defined
  #if !defined(ETL_USING_BUILTIN_MEMCPY)
    #define ETL_USING_BUILTIN_MEMCPY 1
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMMOVE)
    #define ETL_USING_BUILTIN_MEMMOVE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMSET)
    #define ETL_USING_BUILTIN_MEMSET 1
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMCMP)
    #define ETL_USING_BUILTIN_MEMCMP 1
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMCHR)
    #define ETL_USING_BUILTIN_MEMCHR 1
  #endif
#endif

#if defined(__has_builtin) && !defined(ETL_COMPILER_MICROSOFT) // Use __has_builtin to check for
                                                               // existence of builtin functions?
                                                               // Fix VS2022 intellisense issue.
  #if !defined(ETL_USING_BUILTIN_IS_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_ASSIGNABLE __has_builtin(__is_assignable)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_CONSTRUCTIBLE __has_builtin(__is_constructible)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE __has_builtin(__is_nothrow_constructible)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_NOTHROW_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_NOTHROW_ASSIGNABLE __has_builtin(__is_nothrow_assignable)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE __has_builtin(__is_trivially_constructible)
  #endif

  // The legacy __has_trivial_constructor builtin. Deprecated by GCC 12 and
  // removed by GCC 15, so it must be detected separately from the standard
  // __is_trivially_constructible builtin.
  #if !defined(ETL_USING_BUILTIN_HAS_TRIVIAL_CONSTRUCTOR)
    #define ETL_USING_BUILTIN_HAS_TRIVIAL_CONSTRUCTOR __has_builtin(__has_trivial_constructor)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE __has_builtin(__is_trivially_assignable)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE __has_builtin(__is_trivially_destructible)
  #endif

  // The legacy __has_trivial_destructor builtin. Deprecated by GCC 12 and
  // removed by GCC 15, so it must be detected separately from the standard
  // __is_trivially_destructible builtin.
  #if !defined(ETL_USING_BUILTIN_HAS_TRIVIAL_DESTRUCTOR)
    #define ETL_USING_BUILTIN_HAS_TRIVIAL_DESTRUCTOR __has_builtin(__has_trivial_destructor)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_DESTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_DESTRUCTIBLE __has_builtin(__is_destructible)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_NOTHROW_DESTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_NOTHROW_DESTRUCTIBLE __has_builtin(__is_nothrow_destructible)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE (__has_builtin(__has_trivial_copy) || __has_builtin(__is_trivially_copyable))
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_STANDARD_LAYOUT)
    #define ETL_USING_BUILTIN_IS_STANDARD_LAYOUT __has_builtin(__is_standard_layout)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIAL)
    #define ETL_USING_BUILTIN_IS_TRIVIAL __has_builtin(__is_trivial)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_LAYOUT_COMPATIBLE)
    #define ETL_USING_BUILTIN_IS_LAYOUT_COMPATIBLE __has_builtin(__is_layout_compatible)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_POINTER_INTERCONVERTIBLE_BASE_OF)
    #define ETL_USING_BUILTIN_IS_POINTER_INTERCONVERTIBLE_BASE_OF __has_builtin(__is_pointer_interconvertible_base_of)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_UNION)
    #define ETL_USING_BUILTIN_IS_UNION __has_builtin(__is_union)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_EMPTY)
    #define ETL_USING_BUILTIN_IS_EMPTY __has_builtin(__is_empty)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_POLYMORPHIC)
    #define ETL_USING_BUILTIN_IS_POLYMORPHIC __has_builtin(__is_polymorphic)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_ABSTRACT)
    #define ETL_USING_BUILTIN_IS_ABSTRACT __has_builtin(__is_abstract)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_FINAL)
    #define ETL_USING_BUILTIN_IS_FINAL __has_builtin(__is_final)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_AGGREGATE)
    #define ETL_USING_BUILTIN_IS_AGGREGATE __has_builtin(__is_aggregate)
  #endif

  #if !defined(ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR)
    #define ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR __has_builtin(__has_virtual_destructor)
  #endif

  #if !defined(ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS)
    #define ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS __has_builtin(__has_unique_object_representations)
  #endif

  #if !defined(ETL_USING_BUILTIN_UNDERLYING_TYPE)
    #define ETL_USING_BUILTIN_UNDERLYING_TYPE __has_builtin(__underlying_type)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_CONSTANT_EVALUATED)
    #define ETL_USING_BUILTIN_IS_CONSTANT_EVALUATED __has_builtin(__builtin_is_constant_evaluated)
  #endif

  #if !defined(ETL_USING_BUILTIN_CONSTANT_P)
    #define ETL_USING_BUILTIN_CONSTANT_P __has_builtin(__builtin_constant_p)
  #endif

  #if !defined(ETL_USING_BUILTIN_BIT_CAST)
    #define ETL_USING_BUILTIN_BIT_CAST __has_builtin(__builtin_bit_cast)
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMCPY)
    #define ETL_USING_BUILTIN_MEMCPY __has_builtin(__builtin_memcpy)
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMMOVE)
    #define ETL_USING_BUILTIN_MEMMOVE __has_builtin(__builtin_memmove)
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMSET)
    #define ETL_USING_BUILTIN_MEMSET __has_builtin(__builtin_memset)
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMCMP)
    #define ETL_USING_BUILTIN_MEMCMP __has_builtin(__builtin_memcmp)
  #endif

  #if !defined(ETL_USING_BUILTIN_MEMCHR)
    #define ETL_USING_BUILTIN_MEMCHR __has_builtin(__builtin_memchr)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_VIRTUAL_BASE_OF)
    #define ETL_USING_BUILTIN_IS_VIRTUAL_BASE_OF __has_builtin(__is_virtual_base_of)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE __has_builtin(__is_trivially_relocatable)
  #endif

  // __builtin_is_cpp_trivially_relocatable replaces __is_trivially_relocatable by an
  // effort of standardization for newer C++ standards
  #if !defined(ETL_USING_BUILTIN_BUILTIN_IS_CPP_TRIVIALLY_RELOCATABLE)
    #define ETL_USING_BUILTIN_BUILTIN_IS_CPP_TRIVIALLY_RELOCATABLE __has_builtin(__builtin_is_cpp_trivially_relocatable)
  #endif
#endif

#if defined(ETL_COMPILER_MICROSOFT) // The Microsoft compiler supports the __is_trivially_copyable
                                    // intrinsic directly. __has_builtin is not used for Microsoft
                                    // (see above) to avoid a VS2022 intellisense issue, so the
                                    // intrinsic is enabled here for the no-STL configuration.
  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_STANDARD_LAYOUT)
    #define ETL_USING_BUILTIN_IS_STANDARD_LAYOUT 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIAL)
    #define ETL_USING_BUILTIN_IS_TRIVIAL 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_UNION)
    #define ETL_USING_BUILTIN_IS_UNION 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_EMPTY)
    #define ETL_USING_BUILTIN_IS_EMPTY 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_POLYMORPHIC)
    #define ETL_USING_BUILTIN_IS_POLYMORPHIC 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_ABSTRACT)
    #define ETL_USING_BUILTIN_IS_ABSTRACT 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_FINAL)
    #define ETL_USING_BUILTIN_IS_FINAL 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_AGGREGATE)
    #define ETL_USING_BUILTIN_IS_AGGREGATE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR)
    #define ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR 1
  #endif

  #if !defined(ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS)
    #define ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS 1
  #endif

  // MSVC has supported __builtin_bit_cast since VS2019 16.6. __has_builtin is not
  // used for Microsoft (see above) to avoid a VS2022 intellisense issue, so the
  // intrinsic is enabled here directly for the no-STL configuration.
  #if !defined(ETL_USING_BUILTIN_BIT_CAST) && (_MSC_VER >= 1926)
    #define ETL_USING_BUILTIN_BIT_CAST 1
  #endif
#endif

// The default. Set to 0, if not already set.
#if !defined(ETL_USING_BUILTIN_IS_ASSIGNABLE)
  #define ETL_USING_BUILTIN_IS_ASSIGNABLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_CONSTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_CONSTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_NOTHROW_ASSIGNABLE)
  #define ETL_USING_BUILTIN_IS_NOTHROW_ASSIGNABLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE)
  #define ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_DESTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_DESTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_NOTHROW_DESTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_NOTHROW_DESTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE)
  #define ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_STANDARD_LAYOUT)
  #define ETL_USING_BUILTIN_IS_STANDARD_LAYOUT 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIAL)
  #define ETL_USING_BUILTIN_IS_TRIVIAL 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_LAYOUT_COMPATIBLE)
  #define ETL_USING_BUILTIN_IS_LAYOUT_COMPATIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_POINTER_INTERCONVERTIBLE_BASE_OF)
  #define ETL_USING_BUILTIN_IS_POINTER_INTERCONVERTIBLE_BASE_OF 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_UNION)
  #define ETL_USING_BUILTIN_IS_UNION 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_EMPTY)
  #define ETL_USING_BUILTIN_IS_EMPTY 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_POLYMORPHIC)
  #define ETL_USING_BUILTIN_IS_POLYMORPHIC 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_ABSTRACT)
  #define ETL_USING_BUILTIN_IS_ABSTRACT 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_FINAL)
  #define ETL_USING_BUILTIN_IS_FINAL 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_AGGREGATE)
  #define ETL_USING_BUILTIN_IS_AGGREGATE 0
#endif

#if !defined(ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR)
  #define ETL_USING_BUILTIN_HAS_VIRTUAL_DESTRUCTOR 0
#endif

#if !defined(ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS)
  #define ETL_USING_BUILTIN_HAS_UNIQUE_OBJECT_REPRESENTATIONS 0
#endif

#if !defined(ETL_USING_BUILTIN_UNDERLYING_TYPE)
  #define ETL_USING_BUILTIN_UNDERLYING_TYPE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_CONSTANT_EVALUATED)
  #define ETL_USING_BUILTIN_IS_CONSTANT_EVALUATED 0
#endif

#if !defined(ETL_USING_BUILTIN_MEMCPY)
  #define ETL_USING_BUILTIN_MEMCPY 0
#endif

#if !defined(ETL_USING_BUILTIN_MEMMOVE)
  #define ETL_USING_BUILTIN_MEMMOVE 0
#endif

#if !defined(ETL_USING_BUILTIN_MEMSET)
  #define ETL_USING_BUILTIN_MEMSET 0
#endif

#if !defined(ETL_USING_BUILTIN_MEMCMP)
  #define ETL_USING_BUILTIN_MEMCMP 0
#endif

#if !defined(ETL_USING_BUILTIN_MEMCHR)
  #define ETL_USING_BUILTIN_MEMCHR 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_VIRTUAL_BASE_OF)
  #define ETL_USING_BUILTIN_IS_VIRTUAL_BASE_OF 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE)
  #define ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE 0
#endif

#if !defined(ETL_USING_BUILTIN_BUILTIN_IS_CPP_TRIVIALLY_RELOCATABLE)
  #define ETL_USING_BUILTIN_BUILTIN_IS_CPP_TRIVIALLY_RELOCATABLE 0
#endif

// __builtin_constant_p predates __has_builtin on GCC and Clang, so enable it for
// those compilers when __has_builtin was not available to detect it.
#if !defined(ETL_USING_BUILTIN_CONSTANT_P) && (defined(__GNUC__) || defined(__clang__))
  #define ETL_USING_BUILTIN_CONSTANT_P 1
#endif

#if !defined(ETL_USING_BUILTIN_CONSTANT_P)
  #define ETL_USING_BUILTIN_CONSTANT_P 0
#endif

#if !defined(ETL_USING_BUILTIN_BIT_CAST)
  #define ETL_USING_BUILTIN_BIT_CAST 0
#endif

// __builtin_bit_cast is only of use to the ETL from C++11 onwards.
// Disable it for older standards, so that the generated code for those users is unchanged.
#if ETL_USING_BUILTIN_BIT_CAST && !ETL_USING_CPP11
  #undef ETL_USING_BUILTIN_BIT_CAST
  #define ETL_USING_BUILTIN_BIT_CAST 0
#endif

namespace etl
{
  namespace traits
  {
    // Documentation: https://www.etlcpp.com/etl_traits.html

    static ETL_CONSTANT bool using_builtin_is_assignable                        = (ETL_USING_BUILTIN_IS_ASSIGNABLE == 1);
    static ETL_CONSTANT bool using_builtin_is_constructible                     = (ETL_USING_BUILTIN_IS_CONSTRUCTIBLE == 1);
    static ETL_CONSTANT bool using_builtin_is_nothrow_constructible             = (ETL_USING_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE == 1);
    static ETL_CONSTANT bool using_builtin_is_nothrow_assignable                = (ETL_USING_BUILTIN_IS_NOTHROW_ASSIGNABLE == 1);
    static ETL_CONSTANT bool using_builtin_is_trivially_constructible           = (ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE == 1);
    static ETL_CONSTANT bool using_builtin_is_trivially_assignable              = (ETL_USING_BUILTIN_IS_TRIVIALLY_ASSIGNABLE == 1);
    static ETL_CONSTANT bool using_builtin_is_trivially_destructible            = (ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE == 1);
    static ETL_CONSTANT bool using_builtin_is_trivially_copyable                = (ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE == 1);
    static ETL_CONSTANT bool using_builtin_underlying_type                      = (ETL_USING_BUILTIN_UNDERLYING_TYPE == 1);
    static ETL_CONSTANT bool using_builtin_is_constant_evaluated                = (ETL_USING_BUILTIN_IS_CONSTANT_EVALUATED == 1);
    static ETL_CONSTANT bool using_builtin_constant_p                           = (ETL_USING_BUILTIN_CONSTANT_P == 1);
    static ETL_CONSTANT bool using_builtin_memcpy                               = (ETL_USING_BUILTIN_MEMCPY == 1);
    static ETL_CONSTANT bool using_builtin_memmove                              = (ETL_USING_BUILTIN_MEMMOVE == 1);
    static ETL_CONSTANT bool using_builtin_memset                               = (ETL_USING_BUILTIN_MEMSET == 1);
    static ETL_CONSTANT bool using_builtin_memcmp                               = (ETL_USING_BUILTIN_MEMCMP == 1);
    static ETL_CONSTANT bool using_builtin_memchr                               = (ETL_USING_BUILTIN_MEMCHR == 1);
    static ETL_CONSTANT bool using_builtin_is_virtual_base_of                   = (ETL_USING_BUILTIN_IS_VIRTUAL_BASE_OF == 1);
    static ETL_CONSTANT bool using_builtin_is_trivially_relocatable             = (ETL_USING_BUILTIN_IS_TRIVIALLY_RELOCATABLE == 1);
    static ETL_CONSTANT bool using_builtin_builtin_is_cpp_trivially_relocatable = (ETL_USING_BUILTIN_BUILTIN_IS_CPP_TRIVIALLY_RELOCATABLE == 1);
    static ETL_CONSTANT bool using_builtin_bit_cast                             = (ETL_USING_BUILTIN_BIT_CAST == 1);
  } // namespace traits
} // namespace etl

#endif
