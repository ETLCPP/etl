///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2015 jwellbelove, rlindeman

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

#ifndef __ETL_IN_IPRIORITY_QUEUE_H__
#error This header is a private element of etl::priority_queue & etl::ipriority_queue
#endif

#ifndef __ETL_PRIORITY_QUEUE_BASE__
#define __ETL_PRIORITY_QUEUE_BASE__

#include <stddef.h>

#include "exception.h"

namespace etl
{
  //***************************************************************************
  /// The base class for priority_queue exceptions.
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_exception : public exception
  {
  public:

    priority_queue_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  /// The exception thrown when the queue is full.
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_full : public priority_queue_exception
  {
  public:

    priority_queue_full()
      : priority_queue_exception("priority_queue: full")
    {
    }
  };

  //***************************************************************************
  /// The priority queue iterator exception on reversed iterators
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_iterator : public priority_queue_exception
  {
  public:

    priority_queue_iterator()
      : priority_queue_exception("priority_queue: iterator error")
    {
    }
  };

  //***************************************************************************
  /// The base class for all priority queues.
  ///\ingroup queue
  //***************************************************************************
  class priority_queue_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of queue.

    //*************************************************************************
    /// Returns the current number of items in the priority queue.
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
    /// Checks to see if the priority queue is empty.
    /// \return <b>true</b> if the queue is empty, otherwise <b>false</b>
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the priority queue is full.
    /// \return <b>true</b> if the priority queue is full, otherwise <b>false</b>
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
    priority_queue_base(size_type max_size)
      : current_size(0),
        MAX_SIZE(max_size)
    {
    }

    size_type current_size;   ///< The number of items in the priority queue.
    const size_type MAX_SIZE; ///< The maximum number of items in the priority queue.
  };
}

#endif
