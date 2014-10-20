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

#ifndef __etl_iqueue__
#define __etl_iqueue__

#include <cstddef>

#include "exception.h"

namespace etl
{
#ifdef USE_ETL_EXCEPTIONS
  //***************************************************************************
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
  /// A fixed capacity queue written in the STL style.
  /// This queue cannot be used for concurrent access from multiple threads.
  /// \tparam T The type of item that the queue holds.
  //***************************************************************************
  template <typename T>
  class iqueue
  {
  public:

    typedef size_t   size_type;
    typedef T        value_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;

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
    /// Adds an item to the queue.
    /// If USE_ETL_EXCEPTIONS is defined, throws a queue_full_exception is the queue is already full,
    /// otherwise does nothing if full.
    ///\param item The item to push to the queue.
    //*************************************************************************
    void push(const_reference item)
    {
      if (!full())
      {
        buffer[in++] = item;
        in %= MAX_SIZE;
        ++size;
      }
#ifdef USE_ETL_EXCEPTIONS
      else
      {
        throw queue_full_exception();
      }
#endif
    }

    //*************************************************************************
    /// Allows a possibly more efficient 'push' by moving to the next input item
    /// and returning a reference to it.
    /// This may eliminate a copy by allowing direct construction in-place.
    /// If USE_ETL_EXCEPTIONS is defined, throws a queue_full_exception is the queue is already full,
    /// otherwise does nothing if full.
    /// \return A reference to the position to 'push' to.
    //*************************************************************************
    reference push()
    {
      size_type next = in;

      if (!full())
      {
        ++in;
        in %= MAX_SIZE;
        ++size;
      }
#ifdef USE_ETL_EXCEPTIONS
      else
      {
        throw queue_full_exception();
      }
#endif

      return buffer[next];
    }

    //*************************************************************************
    /// Removes the oldest item from the back of the queue.
    /// Does nothing if the queue is already empty.
    //*************************************************************************
    void pop()
    {
      if (!empty())
      {
        out = (out + 1) % MAX_SIZE;
        --size;
      }
    }

    //*************************************************************************
    /// Gets a reference to the item at the front of the queue.
    /// If USE_ETL_EXCEPTIONS is defined, throws a queue_empty_exception if the queue is empty.
    /// If USE_ETL_EXCEPTIONS is not defined and the queue is empty, the return value is undefined.
    /// \return A reference to the item at the front of the queue.
    //*************************************************************************
    reference front()
    {
#ifdef USE_ETL_EXCEPTIONS
      if (empty())
      {
        throw queue_empty_exception();
      }
#endif

      return buffer[out];
    }

    //*************************************************************************
    /// Gets a const reference to the item at the front of the queue.
    /// If USE_ETL_EXCEPTIONS is defined, throws a queue_empty_exception if the queue is empty.
    /// If USE_ETL_EXCEPTIONS is not defined and the queue is empty, the return value is undefined.
    /// \return A const reference to the item at the front of the queue.
    //*************************************************************************
    const_reference front() const
    {
#ifdef USE_ETL_EXCEPTIONS
      if (empty())
      {
        throw queue_empty_exception();
      }
#endif

      return buffer[out];
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    queue(T* buffer, size_type max_size)
      : buffer(buffer),
        MAX_SIZE(max_size)
    {
      clear();
    }

  private:

    size_type in;             ///< Where to input new data.
    size_type out;            ///< Where to get the oldest data.
    size_type size;           ///< The number of items in the queue.
    const size_type MAX_SIZE; ///< The maximum number of items in the queue.
    T* buffer;                ///< The internal buffer.
  };
}

