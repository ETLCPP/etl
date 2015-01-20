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

#ifndef __ETL_QUEUE__
#define __ETL_QUEUE__

#include <stddef.h>
#include <stdint.h>

#include "iqueue.h"
#include "container.h"
#include "alignment.h"
#include "array.h"

//*****************************************************************************
///\defgroup queue queue
/// A First-in / first-out queue with the capacity defined at compile time,
/// written in the STL style.
///\note Uses a predefined array, so MAX_SIZE_ elements will be always be constructed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup queue
  /// A fixed capacity queue.
  /// This queue does not support concurrent access by different threads.
  /// \tparam T    The type this queue should support.
  /// \tparam SIZE The maximum capacity of the queue.
  //***************************************************************************
  template <typename T, const size_t SIZE>
  class queue : public iqueue<T>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    queue()
      : iqueue<T>(reinterpret_cast<T*>(&buffer.value[0]), SIZE)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~queue()
    {
      clear();
    }

  private:

    /// The unititialised buffer of T used in the stack.
    etl::align_as<array<uint8_t, SIZE * sizeof(T)>, T> buffer;
  };
}

#endif
