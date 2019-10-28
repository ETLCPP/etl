///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#ifndef ETL_FACTORIAL_INCLUDED
#define ETL_FACTORIAL_INCLUDED

#include <stddef.h>

#include "platform.h"

///\defgroup factorial factorial
/// fibonacci<N> : Calculates the Nth factorial value.
///\ingroup maths

namespace etl
{
  //***************************************************************************
  ///\ingroup fibonacci
  /// Defines <b>value</b> as the Nth factorial number.
  ///\tparam N The number to find the factorial value of.
  //***************************************************************************
  template <size_t N>
  struct factorial
  {
    static const size_t value = N * factorial<N - 1>::value;
  };

  //***************************************************************************
  // Specialisation for N = 0
  //***************************************************************************
  template <>
  struct factorial<0>
  {
    static const size_t value = 1;
  };
}

#endif
