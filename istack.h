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

#ifndef __etl_istack__
#define __etl_istack__
#define __etl_in_istack_h__

#include <cstddef>

#include "stack_base.h"

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
  /// \tparam T The type of item that the stack holds.
  //***************************************************************************
  template <typename T>
  class istack : public stack_base
  {
  public:

    typedef stack_base::size_type size_type;       ///< The type used for determining the size of the stack.
    typedef T                     value_type;      ///< The type stored in the stack.
    typedef T&                    reference;       ///< A reference to the type used in the stack.
    typedef const T&              const_reference; ///< A const reference to the type used in the stack.
    typedef T*                    pointer;         ///< A pointer to the type used in the stack.
    typedef const T*              const_pointer;   ///< A const pointer to the type used in the stack.

    //*************************************************************************
    /// Adds an item to the stack.
    /// If ETL_USE_EXCEPTIONS is defined, throws an etl::stack_full is the stack is already full,
    /// otherwise does nothing if full.
    ///\param item The item to push to the stack.
    //*************************************************************************
    void push(const_reference item)
    {
      if (!full())
      {
        top_index = current_size++;
        buffer[top_index] = item;
      }
#ifdef ETL_USE_EXCEPTIONS
      else
      {
        throw stack_full();
      }
#endif
    }

    //*************************************************************************
    /// Allows a possibly more efficient 'push' by moving to the next input item
    /// and returning a reference to it.
    /// This may eliminate a copy by allowing direct construction in-place.<br>
    /// If ETL_USE_EXCEPTIONS is defined, throws an etl::stack_full is the stack is already full,
    /// otherwise does nothing if full.
    /// \return A reference to the position to 'push' to.
    //*************************************************************************
    reference push()
    {
      if (!full())
      {
        top_index = current_size++;
      }
#ifdef ETL_USE_EXCEPTIONS
      else
      {
        throw stack_full();
      }
#endif

      return buffer[top_index];
    }

    //*************************************************************************
    /// Gets a reference to the item at the top of the stack.<br>
    /// If ETL_USE_EXCEPTIONS is defined, throws an etl::stack_empty if the stack is empty.<br>
    /// If ETL_USE_EXCEPTIONS is not defined and the stack is empty, the return value is undefined.
    /// \return A reference to the item at the top of the stack.
    //*************************************************************************
    reference top()
    {
#ifdef ETL_USE_EXCEPTIONS
      if (empty())
      {
        throw stack_empty();
      }
#endif

      return buffer[top_index];
    }

    //*************************************************************************
    /// Gets a const reference to the item at the top of the stack.<br>
    /// If ETL_USE_EXCEPTIONS is defined, throws an etl::stack_empty if the stack is empty.<br>
    /// If ETL_USE_EXCEPTIONS is not defined and the stack is empty, the return value is undefined.
    /// \return A const reference to the item at the top of the stack.
    //*************************************************************************
    const_reference top() const
    {
#ifdef ETL_USE_EXCEPTIONS
      if (empty())
      {
        throw stack_empty();
      }
#endif

      return buffer[top_index];
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    istack(T* buffer, size_type max_size)
      : stack_base(max_size),
        buffer(buffer)
    {
    }

  private:

    T* buffer; ///< The internal buffer.
  };
}

#undef __etl_in_istack_h__
#endif
