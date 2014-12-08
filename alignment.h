///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_ALIGNEMENT__
#define __ETL_ALIGNEMENT__

#include "type_traits.h"

///\defgroup align_at align_at
/// Creates a variable of the specified type at the specified alignment.
/// \ingroup utilities

#if !defined(COMPILER_IAR)

#if defined(COMPILER_MICROSOFT)
#define ETL_ALIGNMENT_PRE(n) __declspec(align(n))
#define ETL_ALIGNMENT_POST(n)
#endif

#if defined(COMPILER_GCC)
#define ETL_ALIGNMENT_PRE(n)
#define ETL_ALIGNMENT_POST(n) __attribute__((aligned(n)))
#endif

#if defined (COMPILER_KEIL)
#define ETL_ALIGNMENT_PRE(n)
#define ETL_ALIGNMENT_POST(n) __attribute__((aligned(n)))
#endif

namespace etl
{ 
  /// Template declaration.
  template <typename T, const size_t ALIGNMENT>
  struct align_at;
	
  /// Align 1
  template <typename T>
  struct align_at<T, 1>
  {
    ETL_ALIGNMENT_PRE(1) T value ETL_ALIGNMENT_POST(1);
  };

  /// Align 2
  template <typename T>
  struct align_at<T, 2>
  {
    ETL_ALIGNMENT_PRE(2) T value ETL_ALIGNMENT_POST(2);
  };

  /// Align 4
  template <typename T>
  struct align_at<T, 4>
  {
    ETL_ALIGNMENT_PRE(4) T value ETL_ALIGNMENT_POST(4);
  };

  /// Align 8
  template <typename T>
  struct align_at<T, 8>
  {
    ETL_ALIGNMENT_PRE(8) T value ETL_ALIGNMENT_POST(8);
  };

  /// Align 16
  template <typename T>
  struct align_at<T, 16>
  {
    ETL_ALIGNMENT_PRE(16) T value ETL_ALIGNMENT_POST(16);
  };
  
  /// Align As
  template <typename TValue, typename TAlign>
  struct align_as : public align_at<TValue, etl::alignment_of<TAlign>::value>
  {
  };
}
#endif

#undef ETL_ALIGNMENT_PRE
#undef ETL_ALIGNMENT_POST

#endif
