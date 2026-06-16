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

#ifndef ETL_INFINITE_LOOP_INCLUDED
#define ETL_INFINITE_LOOP_INCLUDED

#include "platform.h"

namespace etl
{
  //*****************************************************************************
  /// An infinite loop that will not be optimised out
  ///
  /// Before C++26, an empty infinite loop without side effects is considered
  /// undefined behavior, and may be optimised away by the compiler.
  ///
  /// See also
  /// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2809r1.html
  //*****************************************************************************
  ETL_NORETURN
  inline void infinite_loop()
  {
    while (true)
    {
#if ETL_NOT_USING_CPP26
  #if defined(ETL_COMPILER_GCC) || defined(ETL_COMPILER_CLANG)
      __asm__ __volatile__("" : : : "memory");
  #elif defined(ETL_COMPILER_MICROSOFT)
      _ReadWriteBarrier();
  #else
    #error "Infinite loop not supported for this compiler and platform"
  #endif
#endif
    }
  }
} // namespace etl

#endif
