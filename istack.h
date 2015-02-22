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

#ifndef __ETL_ISTACK__
#define __ETL_ISTACK__
#define __ETL_IN_ISTACK_H__

#include <stddef.h>

#include "stack_base.h"
#include "type_traits.h"
#include "parameter_type.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  ///\ingroup stack
  ///\brief This is the base for all stacks that contain a particular type.
  ///\details Normally a reference to this type will be taken from a derived stack.
  ///\code
  /// etl::stack<int, 10> myQueue;
  /// etl::istack<int>& iQueue = myQueue;
  ///\endcode
  /// \warning This stack cannot be used for concurrent access from multiple threads.
  /// \tparam T The type of value that the stack holds.
  //***************************************************************************
  template <typename T>
  class istack : public stack_base
  {
  public:

    typedef T                     value_type;      ///< The type stored in the stack.
    typedef T&                    reference;       ///< A reference to the type used in the stack.
    typedef const T&              const_reference; ///< A const reference to the type used in the stack.
    typedef T*                    pointer;         ///< A pointer to the type used in the stack.
    typedef const T*              const_pointer;   ///< A const pointer to the type used in the stack.
    typedef stack_base::size_type size_type;       ///< The type used for determining the size of the stack.

  private:

    typedef typename parameter_type<T>::type parameter_t;

  public:

    //*************************************************************************
    /// Gets a reference to the value at the top of the stack.<br>
    /// \return A reference to the value at the top of the stack.
    //*************************************************************************
    reference top()
    {
      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Adds a value to the stack.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::stack_full is the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    void push(parameter_t value)
    {
      if (!full())
      {
        top_index = current_size++;
        new(&p_buffer[top_index]) T(value);
      }
      else
#ifdef ETL_THROW_EXCEPTIONS     
      {
        throw stack_full();
      }
#else
      {
        error_handler::error(stack_full());
      }
#endif
    }

    //*************************************************************************
    /// Allows a possibly more efficient 'push' by moving to the next input value
    /// and returning a reference to it.
    /// This may eliminate a copy by allowing direct construction in-place.<br>
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::stack_full is the stack is already full.
    /// \return A reference to the position to 'push' to.
    //*************************************************************************
    reference push()
    {
      if (!full())
      {
        top_index = current_size++;
        new(&p_buffer[top_index]) T();
      }
      else
#ifdef ETL_THROW_EXCEPTIONS     
      {
        throw stack_full();
      }
#else
      {
        error_handler::error(stack_full());
      }
#endif

      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Gets a const reference to the value at the top of the stack.<br>
    /// \return A const reference to the value at the top of the stack.
    //*************************************************************************
    const_reference top() const
    {
      return p_buffer[top_index];
    }

    //*************************************************************************
    /// Clears the stack to the empty state.
    //*************************************************************************
    void clear()
    {
      while (current_size > 0)
      {
        p_buffer[top_index].~T();
        --top_index;
        --current_size;
      }
    }

    //*************************************************************************
    /// Removes the oldest item from the top of the stack.
    /// Does nothing if the stack is already empty.
    //*************************************************************************
    void pop()
    {
      if (!empty())
      {
        p_buffer[top_index].~T();
        --top_index;
        --current_size;
      }
    }

  protected:

    //*************************************************************************
    /// Make this a clone of the supplied stack
    //*************************************************************************
    void clone(const istack& other)
    {
      size_t index = 0;

      for (size_t i = 0; i < other.size(); ++i)
      {
        push(other.p_buffer[index++]);
      }
    }

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    istack(T* p_buffer, size_type max_size)
      : stack_base(max_size),
        p_buffer(p_buffer)
    {
    }

  private:

    T* p_buffer; ///< The internal buffer.
  };
}

#undef __ETL_IN_ISTACK_H__
#endif
