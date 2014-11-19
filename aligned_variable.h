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

#ifndef __ETL_ALIGNED_VARIABLE__
#define __ETL_ALIGNED_VARIABLE__

///\defgroup aligned_variable aligned_variable
/// Creates a variable of the specified type at the specified alignment.
/// \ingroup utilities

namespace etl
{
  template <typename T, const size_t ALIGNMENT>
  struct aligned_variable;

#if defined(COMPILER_MICROSOFT)
  /// Align 1
  template <typename T>
  struct aligned_variable<T, 1>
  {
    __declspec(align(1)) T value;
  };

  /// Align 2
  template <typename T>
  struct aligned_variable<T, 2>
  {
    __declspec(align(2)) T value;
  };

  /// Align 4
  template <typename T>
  struct aligned_variable<T, 4>
  {
    __declspec(align(4)) T value;
  };

  /// Align 8
  template <typename T>
  struct aligned_variable<T, 8>
  {
    __declspec(align(8)) T value;
  };

  /// Align 16
  template <typename T>
  struct aligned_variable<T, 16>
  {
    __declspec(align(16)) T value;
  };
#endif

#if defined(COMPILER_GCC)
  /// Align 1
  template <typename T>
  struct aligned_variable<T, 1>
  {
    T value __attribute__((aligned(1)));
  };

  /// Align 2
  template <typename T>
  struct aligned_variable<T, 2>
  {
    T value __attribute__((aligned(2)));
  };

  /// Align 4
  template <typename T>
  struct aligned_variable<T, 4>
  {
    T value __attribute__((aligned(4)));
  };

  /// Align 8
  template <typename T>
  struct aligned_variable<T, 8>
  {
    T value __attribute__((aligned(8)));
  };
  
  /// Align 16
  template <typename T>
  struct aligned_variable<T, 16>
  {
    T value __attribute__((aligned(16)));
  };
#endif

#if defined(COMPILER_KEIL_ARM)
  /// Align 1
  template <typename T>
  struct aligned_variable<T, 1>
  {
    __align(1) T value;
  };

  /// Align 2
  template <typename T>
  struct aligned_variable<T, 2>
  {
    __align(2) T value;
  };

  /// Align 4
  template <typename T>
  struct aligned_variable<T, 4>
  {
    __align(4) T value;
};

  /// Align 8
  template <typename T>
  struct aligned_variable<T, 8>
  {
    __align(8) T value;
  };

  /// Align 16
  template <typename T>
  struct aligned_variable<T, 16>
  {
    __align(16) T value;
  };
#endif

#if defined (COMPILER_IAR_ARM)
  /// Align 1
  template <typename T>
  struct aligned_variable<T, 1>
  {
#pragma data_alignment=1
    T value;
  };

  /// Align 2
  template <typename T>
  struct aligned_variable<T, 2>
  {
#pragma data_alignment=2 
    T value;
  };

  /// Align 4
  template <typename T>
  struct aligned_variable<T, 4>
  {
#pragma data_alignment=4 
    T value;
  };

  /// Align 8
  template <typename T>
  struct aligned_variable<T, 8>
  {
#pragma data_alignment=8
    T value;
  };

  /// Align 16
  template <typename T>
  struct aligned_variable<T, 16>
  {
#pragma data_alignment=16
    T value;
  };
#endif
}

#endif