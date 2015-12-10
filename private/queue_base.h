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

#ifndef __ETL_IN_IQUEUE_H__
#error This header is a private element of etl::queue & etl::iqueue
#endif

#ifndef __ETL_QUEUE_BASE__
#define __ETL_QUEUE_BASE__

#include <stddef.h>

#include "exception.h"

namespace etl
{
  //***************************************************************************
  /// The base class for queue exceptions.
  ///\ingroup queue
  //***************************************************************************
  class queue_exception : public exception
  {
  public:

    queue_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  /// The exception thrown when the queue is full.
  ///\ingroup queue
  //***************************************************************************
  class queue_full : public queue_exception
  {
  public:

    queue_full()
      : queue_exception("queue: full")
    {
    }
  };

  //***************************************************************************
  /// The base class for all queues.
  ///\ingroup queue
  //***************************************************************************
  class queue_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of queue.

    //*************************************************************************
    /// Returns the current number of items in the queue.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Returns the maximum number of items that can be queued.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Checks to see if the queue is empty.
    /// \return <b>true</b> if the queue is empty, otherwise <b>false</b>
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the queue is full.
    /// \return <b>true</b> if the queue is full, otherwise <b>false</b>
    //*************************************************************************
    bool full() const
    {
      return current_size == MAX_SIZE;
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
    /// The constructor that is called from derived classes.
    //*************************************************************************
    queue_base(size_type max_size)
      : in(0),
        out(0),
        current_size(0),
        MAX_SIZE(max_size)
    {
    }

    size_type in;             ///< Where to input new data.
    size_type out;            ///< Where to get the oldest data.
    size_type current_size;   ///< The number of items in the queue.
    const size_type MAX_SIZE; ///< The maximum number of items in the queue.
  };
}

#endif
