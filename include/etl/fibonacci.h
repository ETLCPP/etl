///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_FIBONACCI_INCLUDED
#define ETL_FIBONACCI_INCLUDED

#include "platform.h"

#include <stddef.h>

///\defgroup fibonacci fibonacci
/// fibonacci<N> : Calculates the Nth Fibonacci value.
///\ingroup maths

namespace etl
{
  //***************************************************************************
  ///\ingroup fibonacci
  /// Defines <b>value</b> as the Nth Fibonacci number.
  ///\tparam N The number to find the Fibonacci value of.
  //***************************************************************************
  template <size_t N>
  struct fibonacci
  {
    static ETL_CONSTANT size_t value = fibonacci<N - 1>::value + fibonacci<N - 2>::value;
  };

  //***************************************************************************
  // Specialisation for N = 1
  //***************************************************************************
  template <>
  struct fibonacci<1>
  {
    static ETL_CONSTANT size_t value = 1UL;
  };

  //***************************************************************************
  // Specialisation for N = 0
  //***************************************************************************
  template <>
  struct fibonacci<0>
  {
    static ETL_CONSTANT size_t value = 0UL;
  };

  template <size_t N>
  ETL_CONSTANT size_t fibonacci<N>::value;

#if ETL_USING_CPP17
  template <size_t N>
  inline constexpr size_t fibonacci_v = fibonacci<N>::value;
#endif
}

#endif
