///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 BMW AG

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

#ifndef ETL_META_INCLUDED
#define ETL_META_INCLUDED

#include "platform.h"

//*****************************************************************************
/// \defgroup meta meta
/// C++26 static reflection (P2996).
/// \ingroup etl
//*****************************************************************************

//*****************************************************************************
// This is the one and only place where reflection support is detected.
//
// GCC 16 is currently the only compiler that implements enough of P2996 for
// etl::meta, and it does so for both STL and non-STL builds.
// See .devcontainer/gcc16 for the reference toolchain.
//
// Clang (including the Bloomberg p2996 fork) and MSVC are not supported yet.
//
// ETL_HAS_REFLECTION is always defined, as 1 or 0.
// When it is 0 this header defines nothing else, so user code can include
// etl/meta.h unconditionally and test ETL_HAS_REFLECTION.
//*****************************************************************************
#if defined(ETL_COMPILER_GCC) && defined(__cpp_impl_reflection) && (__cpp_impl_reflection >= 202506L)
  #define ETL_HAS_REFLECTION 1
#else
  #define ETL_HAS_REFLECTION 0
#endif

#if ETL_HAS_REFLECTION
  #if ETL_USING_STL
    // The full standard library is available, so etl::meta is a thin
    // using-declaration layer over std::meta.
    #include "private/meta/stl.h"
  #else
    // No standard library: etl::meta supplies the minimal std declarations
    // that the reflection builtins require.
    #include "private/meta/gcc.h"
  #endif
#endif

#endif // ETL_META_INCLUDED
