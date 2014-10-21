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

#ifndef __etl_queue_base__
#define __etl_queue_base__

#include <cstddef>

#include "exception.h"

///\defgroup Queue Queue
///\ingroup Containers

namespace etl
{
#ifdef ETL_USE_EXCEPTIONS
  //***************************************************************************
  ///\ingroup Queue
  ///\brief This is the base for all queues.
  ///\detail This is the base for all queues. Instances or references to this type should never need to be created.
  /// The base class for queue exceptions.
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
  ///\ingroup Queue
  /// The exception thrown when the queue is full.
  //***************************************************************************
  class queue_full_exception : public queue_exception
  {
  public:

    queue_full_exception()
      : queue_exception("queue full exception")
    {
    }
  };

  //***************************************************************************
  ///\ingroup Queue
  /// The exception thrown when the queue is empty.
  //***************************************************************************
  class queue_empty_exception : public queue_exception
  {
  public:

    queue_empty_exception()
      : queue_exception("queue empty exception")
    {
    }
  };
#endif

  //***************************************************************************
  ///\ingroup Queue
  /// A fixed capacity queue written in the STL style.
  /// \warning This queue cannot be used for concurrent access from multiple threads.
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
      return size;
    }

    //*************************************************************************
    /// Returns the maximum number of items that can be queued.
    //*************************************************************************
    size_type capacity() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the maximum number of items that can be queued.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Clears the queue to the empty state.
    //*************************************************************************
    void clear()
    {
      in   = 0;
      out  = 0;
      size = 0;
    }

    //*************************************************************************
    /// Checks to see if the queue is empty.
    /// \return <b>true</b> if the queue is empty, otherwise <b>false</b>
    //*************************************************************************
    bool empty() const
    {
      return size == 0;
    }

    //*************************************************************************
    /// Checks to see if the queue is full.
    /// \return <b>true</b> if the queue is full, otherwise <b>false</b>
    //*************************************************************************
    bool full() const
    {
      return size == MAX_SIZE;
    }

    //*************************************************************************
    /// Removes the oldest item from the back of the queue.
    /// Does nothing if the queue is already empty.
    //*************************************************************************
    void pop()
    {
      if (!empty())
      {
        out = (out == (MAX_SIZE - 1)) ? 0 : ++out;
        --size;
      }
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    queue_base(size_type max_size)
      : MAX_SIZE(max_size)
    {
      clear();
    }

    size_type in;             ///< Where to input new data.
    size_type out;            ///< Where to get the oldest data.
    size_type size;           ///< The number of items in the queue.
    const size_type MAX_SIZE; ///< The maximum number of items in the queue.
  };
}

