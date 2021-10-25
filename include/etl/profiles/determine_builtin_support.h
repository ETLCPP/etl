///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#if defined(__has_builtin)
  #if !defined(ETL_USE_BUILTIN_MEMCMP)
    #define ETL_USE_BUILTIN_MEMCMP  __has_builtin(__builtin_memcmp)
  #endif

  #if !defined(ETL_USE_BUILTIN_MEMMOVE)
    #define ETL_USE_BUILTIN_MEMMOVE __has_builtin(__builtin_memmove)
  #endif

  #if !defined(ETL_USE_BUILTIN_MEMSET)
    #define ETL_USE_BUILTIN_MEMSET  __has_builtin(__builtin_memset)
  #endif

  #if !defined(ETL_USE_BUILTIN_IS_ASSIGNABLE)
    #define ETL_USE_BUILTIN_IS_ASSIGNABLE __has_builtin(__is_assignable)
  #endif

#if !defined(ETL_USE_BUILTIN_IS_CONSTRUCTIBLE)
  #define ETL_USE_BUILTIN_IS_CONSTRUCTIBLE __has_builtin(__is_constructible)
#endif

#if !defined(ETL_USE_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE)
  #define ETL_USE_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE (__has_builtin(__has_trivial_constructor) || __has_builtin(__is_trivially_constructible))
#endif


__has_trivial_destructor
__is_trivially_destructible

__has_trivial_copy
__is_trivially_copyable

#else
  #if !defined(ETL_USE_BUILTIN_MEMCMP)
    #define ETL_USE_BUILTIN_MEMCMP  0
  #endif

  #if !defined(ETL_USE_BUILTIN_MEMMOVE)
    #define ETL_USE_BUILTIN_MEMMOVE 0
  #endif

  #if !defined(ETL_USE_BUILTIN_MEMSET)
    #define ETL_USE_BUILTIN_MEMSET  0
  #endif
#endif

#endif
