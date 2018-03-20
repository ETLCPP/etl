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

#ifndef __ETL_PLATFORM__
#define __ETL_PLATFORM__

// Some targets do not support 8bit types.
#define ETL_8BIT_SUPPORT (CHAR_BIT == 8)

#if defined(_DEBUG) || defined(DEBUG)
#define ETL_DEBUG
#endif

#undef ETL_CPP11_SUPPORTED
#undef ETL_CPP14_SUPPORTED
#undef ETL_CPP17_SUPPORTED
#undef ETL_NO_NULLPTR_SUPPORT
#undef ETL_NO_LARGE_CHAR_SUPPORT
#undef ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED
#undef ETL_ATOMIC_SUPPORTED

#include "etl_profile.h"

#if defined(ETL_COMPILER_MICROSOFT)
  // Disable warning of deprecated std::iterator.
  #pragma warning(disable : 4996)
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

#endif
