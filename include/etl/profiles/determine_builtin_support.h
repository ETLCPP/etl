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

#if defined(ETL_USE_TYPE_TRAITS_BUILTINS) // Set all of them to be true if not already defined
  #if !defined(ETL_USING_BUILTIN_IS_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_ASSIGNABLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_CONSTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE 1
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE 1
  #endif
#endif

#if defined(__has_builtin) // Use __has_builtin to check for existence of builtin functions?
  #if !defined(ETL_USING_BUILTIN_IS_ASSIGNABLE)
    #define ETL_USING_BUILTIN_IS_ASSIGNABLE __has_builtin(__is_assignable)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_CONSTRUCTIBLE __has_builtin(__is_constructible)
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE (__has_builtin(__has_trivial_constructor) || __has_builtin(__is_trivially_constructible))
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE (__has_builtin(__has_trivial_destructor) || __has_builtin(__is_trivially_destructible))
  #endif

  #if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE)
    #define ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE (__has_builtin(__has_trivial_copy) || __has_builtin(__is_trivially_copyable))
  #endif
#endif

// The default. Set to 0, if not already set.
#if !defined(ETL_USING_BUILTIN_IS_ASSIGNABLE)
  #define ETL_USING_BUILTIN_IS_ASSIGNABLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_CONSTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_CONSTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE)
  #define ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE 0
#endif

#if !defined(ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE)
  #define ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE 0
#endif

namespace etl
{
  namespace traits
  {
    // Documentation: https://www.etlcpp.com/etl_traits.html

    static ETL_CONSTANT bool using_builtin_is_assignable              = (ETL_USING_BUILTIN_IS_ASSIGNABLE == 1);
    static ETL_CONSTANT bool using_builtin_is_constructible           = (ETL_USING_BUILTIN_IS_CONSTRUCTIBLE == 1);
    static ETL_CONSTANT bool using_builtin_is_trivially_constructible = (ETL_USING_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE == 1);
    static ETL_CONSTANT bool using_builtin_is_trivially_destructible  = (ETL_USING_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE == 1);
    static ETL_CONSTANT bool using_builtin_is_trivially_copyable      = (ETL_USING_BUILTIN_IS_TRIVIALLY_COPYABLE == 1);
  }
}

#endif
