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

#ifndef __ETL_IQUEUE__
#define __ETL_IQUEUE__
#define __ETL_IN_IQUEUE_H__

#include <stddef.h>

#include "queue_base.h"
#include "type_traits.h"
#include "parameter_type.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  ///\ingroup queue
  ///\brief This is the base for all queues that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived queue.
  ///\code
  /// etl::queue<int, 10> myQueue;
  /// etl::iqueue<int>& iQueue = myQueue;
  ///\endcode
  /// \warning This queue cannot be used for concurrent access from multiple threads.
  /// \tparam T The type of value that the queue holds.
  //***************************************************************************
  template <typename T>
  class iqueue : public queue_base
  {
  public:

    typedef T                     value_type;      ///< The type stored in the queue.
    typedef T&                    reference;       ///< A reference to the type used in the queue.
    typedef const T&              const_reference; ///< A const reference to the type used in the queue.
    typedef T*                    pointer;         ///< A pointer to the type used in the queue.
    typedef const T*              const_pointer;   ///< A const pointer to the type used in the queue.
    typedef queue_base::size_type size_type;       ///< The type used for determining the size of the queue.

  private:

    typedef typename parameter_type<T>::type parameter_t;

  public:

    //*************************************************************************
    /// Gets a reference to the value at the front of the queue.<br>
    /// \return A reference to the value at the front of the queue.
    //*************************************************************************
    reference front()
    {
      return p_buffer[out];
    }

    //*************************************************************************
    /// Gets a const reference to the value at the front of the queue.<br>
    /// \return A const reference to the value at the front of the queue.
    //*************************************************************************
    const_reference front() const
    {
      return p_buffer[out];
    }

    //*************************************************************************
    /// Gets a reference to the value at the back of the queue.<br>
    /// \return A reference to the value at the back of the queue.
    //*************************************************************************
    reference back()
    {
      return p_buffer[in == 0 ? MAX_SIZE - 1 : in - 1];
    }

    //*************************************************************************
    /// Gets a const reference to the value at the back of the queue.<br>
    /// \return A const reference to the value at the back of the queue.
    //*************************************************************************
    const_reference back() const
    {
      return p_buffer[in == 0 ? MAX_SIZE - 1 : in - 1];
    }

    //*************************************************************************
    /// Adds a value to the queue.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::queue_full is the queue is already full,
    /// otherwise does nothing if full.
    ///\param value The value to push to the queue.
    //*************************************************************************
    void push(parameter_t value)
    {
      if (!full())
      {
        new(&p_buffer[in]) T(value);
        in = (in == (MAX_SIZE - 1)) ? 0 : in + 1;
        ++current_size;
      }
      else
#ifdef ETL_THROW_EXCEPTIONS
      {
        throw queue_full();
      }
#else
      {
        error_handler::error(queue_full());
      }
#endif
    }

    //*************************************************************************
    /// Allows a possibly more efficient 'push' by moving to the next input value
    /// and returning a reference to it.
    /// This may eliminate a copy by allowing direct construction in-place.<br>
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::queue_full is the queue is already full,
    /// otherwise does nothing if full.
    /// \return A reference to the position to 'push' to.
    //*************************************************************************
    reference push()
    {
      const size_type next = in;

      if (!full())
      {
        new(&p_buffer[in]) T();
        in = (in == (MAX_SIZE - 1)) ? 0 : in + 1;
        ++current_size;
      }
      else
#ifdef ETL_THROW_EXCEPTIONS
      {
        throw queue_full();
      }
#else
      {
        error_handler::error(queue_full());
      }
#endif

      return p_buffer[next];
    }

    //*************************************************************************
    /// Clears the queue to the empty state.
    //*************************************************************************
    void clear()
    {
      while (current_size > 0)
      {
        p_buffer[out].~T();
        out = (out == (MAX_SIZE - 1)) ? 0 : out + 1;
        --current_size;
      }

      in = 0;
      out = 0;
    }

    //*************************************************************************
    /// Removes the oldest value from the back of the queue.
    /// Does nothing if the queue is already empty.
    //*************************************************************************
    void pop()
    {
      if (!empty())
      {
        p_buffer[out].~T();
        out = (out == (MAX_SIZE - 1)) ? 0 : out + 1;
        --current_size;
      }
    }

  protected:

    //*************************************************************************
    /// Make this a clone of the supplied queue
    //*************************************************************************
    void clone(const iqueue& other)
    {
      size_t index = other.out;

      for (size_t i = 0; i < other.size(); ++i)
      {
        push(other.p_buffer[index]);
        index = (index == (MAX_SIZE - 1)) ? 0 : index + 1;
      }
    }

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    iqueue(T* p_buffer, size_type max_size)
      : queue_base(max_size),
        p_buffer(p_buffer)
    {
    }

  private:

    T* p_buffer; ///< The internal buffer.
  };
}

#undef __ETL_IN_IQUEUE_H__
#endif
