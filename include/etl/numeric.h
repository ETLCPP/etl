///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#ifndef ETL_NUMERIC_INCLUDED
#define ETL_NUMERIC_INCLUDED

#include "platform.h"

///\defgroup numeric numeric
///\ingroup utilities

namespace etl
{ 
  //***************************************************************************
  /// iota
  /// Reverse engineered version of std::iota for non C++ 0x11 compilers.
  /// Fills a range of elements with sequentially increasing values starting with <b>value</b>.
  ///\param first An iterator to the first position to fill.
  ///\param last  An iterator to the last + 1 position.
  ///\ingroup numeric
  //***************************************************************************
  template <typename TIterator, typename T>
  void iota(TIterator first, TIterator last, T value)
  {
    while (first != last)
    {
      *first++ = value++;
    }
  }
}

#endif

