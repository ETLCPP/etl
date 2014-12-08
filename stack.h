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

#ifndef __ETL_STACK__
#define __ETL_STACK__

#include <stddef.h>
#include <algorithm>

#include "istack.h"
#include "container.h"

//*****************************************************************************
///\defgroup stack stack
/// A First-in / first-out stack with the capacity defined at compile time,
/// written in the STL style.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup stack
  /// A fixed capacity stack.
  /// This stack does not support concurrent access by different threads.
  ///\note Uses a predefined array, so MAX_SIZE_ elements will be always be constructed.
  /// \tparam T    The type this stack should support.
  /// \tparam SIZE The maximum capacity of the stack.
  //***************************************************************************
  template <typename T, const size_t SIZE>
  class stack : public istack<T>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    stack()
      : istack<T>(buffer, SIZE)
    {
    }

    //*************************************************************************
    /// Swap
    //*************************************************************************
    void swap(stack& other)
    {
      std::swap_ranges(etl::begin(buffer), etl::end(buffer), etl::begin(other.buffer));
      std::swap(this->top_index, other.top_index);
      std::swap(this->current_size, other.current_size);
    }

  private:

    T buffer[SIZE]; ///< The internal buffer.
  };

  //*************************************************************************
  /// Swap
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE>
  void swap(etl::stack<T, MAX_SIZE>& first, etl::stack<T, MAX_SIZE>& second)
  {
    first.swap(second);
  }
}

#endif
