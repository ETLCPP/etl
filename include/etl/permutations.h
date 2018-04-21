///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#ifndef ETL_PERMUTATIONS_INCLUDED
#define ETL_PERMUTATIONS_INCLUDED

#include "platform.h"

///\defgroup permutations permutations
/// permutations<N, K> : Calculates K permutations from N.
///\ingroup maths

namespace etl 
{
  //***************************************************************************
  ///\ingroup permutations
  /// Calculates permutaions.
  //***************************************************************************
  template <const size_t NV, const size_t KV>
  struct permutations
  {
    static const size_t value = NV * permutations<NV - 1, KV - 1>::value;
  };

  //***************************************************************************
  /// Calculates permutations.
  /// Specialisation for KV == 0.
  //***************************************************************************
  template <const size_t NV>
  struct permutations<NV, 0>
  {
    static const size_t value = 1;
  };
}

#endif
