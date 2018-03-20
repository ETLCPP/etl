/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef __ETL_ATOMIC__
#define __ETL_ATOMIC__

#include "platform.h"

#if ETL_ATOMIC_SUPPORTED == 1
  #include <atomic>

  namespace etl
  {
    typedef std::atomic<uint32_t> atomic_uint32_t;
  }
  #define ETL_CPP11_ATOMIC_AVAILABLE 1
#elif defined(ETL_COMPILER_ARM)
  #include "atomic/atomic_arm.h"
  #define ETL_ETL_ATOMIC_AVAILABLE 1
#elif defined(ETL_COMPILER_GCC)
  #include "atomic/atomic_gcc.h"
  #define ETL_ATOMIC_AVAILABLE 1
#elif defined(ETL_COMPILER_MSVC)
  #include "atomic/atomic_windows.h"
  #define ETL_ATOMIC_AVAILABLE 1
#else
  #warning NO ATOMIC SUPPORT DEFINED!
  #define ETL_ATOMIC_AVAILABLE 0
#endif

#endif
