///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_IN_IDEQUE_H__
#error This header is a private element of etl::deque & etl::ideque
#endif

#ifndef __ETL_DEQUE_BASE__
#define __ETL_DEQUE_BASE__

#include <stddef.h>

#include "exception.h"

namespace etl
{
  //***************************************************************************
  /// Exception base for deques
  ///\ingroup deque
  //***************************************************************************
  class deque_exception : public exception
  {
  public:

    deque_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  /// Deque full exception.
  ///\ingroup deque
  //***************************************************************************
  class deque_full : public deque_exception
  {
  public:

    deque_full()
      : deque_exception("deque: full")
    {
    }
  };

  //***************************************************************************
  /// Deque empty exception.
  ///\ingroup deque
  //***************************************************************************
  class deque_empty : public deque_exception
  {
  public:

    deque_empty()
      : deque_exception("deque: empty")
    {
    }
  };

  //***************************************************************************
  /// Deque out of bounds exception.
  ///\ingroup deque
  //***************************************************************************
  class deque_out_of_bounds : public deque_exception
  {
  public:

    deque_out_of_bounds()
      : deque_exception("deque: out of bounds")
    {
    }
  };

  //***************************************************************************
  /// The base class for all templated deque types.
  ///\ingroup deque
  //***************************************************************************
  class deque_base
  {
  public:

    typedef size_t size_type;

    //*************************************************************************
    /// Gets the current size of the deque.
    ///\return The current size of the deque.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Checks the 'empty' state of the deque.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Checks the 'full' state of the deque.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return current_size == MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the deque.
    ///\return The maximum size of the deque.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return max_size() - size();
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    deque_base(size_t max_size, size_t buffer_size)
      : current_size(0),
        MAX_SIZE(max_size),
        BUFFER_SIZE(buffer_size)
    {
    }

    size_type       current_size; ///< The current number of elements in the deque.
    const size_type MAX_SIZE;     ///< The maximum number of elements in the deque.
    const size_type BUFFER_SIZE;  ///< The number of elements in the buffer.
  };
}

#endif
