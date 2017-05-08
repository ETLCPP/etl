///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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
#include <stdint.h>
#include <algorithm>

#include "container.h"
#include "alignment.h"
#include "array.h"
#include "exception.h"
#include "error_handler.h"
#include "debug_count.h"
#include "type_traits.h"
#include "parameter_type.h"

#define ETL_FILE "15"

//*****************************************************************************
///\defgroup stack stack
/// A Last-in / first-out stack with the capacity defined at compile time,
/// written in the STL style.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup stack
  /// The base class for stack exceptions.
  //***************************************************************************
  class stack_exception : public exception
  {
  public:

    stack_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the stack is full.
  //***************************************************************************
  class stack_full : public stack_exception
  {
  public:

    stack_full(string_type file_name, numeric_type line_number)
      : stack_exception(ETL_ERROR_TEXT("stack:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the stack is empty.
  //***************************************************************************
  class stack_empty : public stack_exception
  {
  public:

    stack_empty(string_type file_name, numeric_type line_number)
      : stack_exception(ETL_ERROR_TEXT("stack:empty", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// A fixed capacity stack written in the STL style.
  /// \warntopg This stack cannot be used for concurrent access from multiple threads.
  //***************************************************************************
  class stack_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of stack.

    //*************************************************************************
    /// Checks to see if the stack is empty.
    /// \return <b>true</b> if the stack is empty, otherwise <b>false</b>
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the stack is full.
    /// \return <b>true</b> if the stack is full, otherwise <b>false</b>
    //*************************************************************************
    bool full() const
    {
      return current_size == MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the current number of items top the stack.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Returns the maximum number of items that can be stacked.
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
    /// The constructor that is called from derived classes.
    //*************************************************************************
    stack_base(size_type max_size)
      : top_index(0),
      current_size(0),
      MAX_SIZE(max_size)
    {
    }

    size_type top_index;              ///< The index of the top of the stack.
    size_type current_size;           ///< The number of items in the stack.
    const size_type MAX_SIZE;         ///< The maximum number of items in the stack.
    etl::debug_count construct_count; ///< For internal debugging purposes.
  };

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
  class istack : public etl::stack_base
  {
  public:

    typedef T                     value_type;      ///< The type stored in the stack.
    typedef T&                    reference;       ///< A reference to the type used in the stack.
    typedef const T&              const_reference; ///< A const reference to the type used in the stack.
    typedef T*                    pointer;         ///< A pointer to the type used in the stack.
    typedef const T*              const_pointer;   ///< A const pointer to the type used in the stack.
    typedef stack_base::size_type size_type;       ///< The type used for determining the size of the stack.

  private:

    typedef typename etl::parameter_type<T>::type parameter_t;

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
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    ///\param value The value to push to the stack.
    //*************************************************************************
    void push(parameter_t value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      top_index = current_size++;
      ::new (&p_buffer[top_index]) T(value);
      ++construct_count;
    }

    //*************************************************************************
    /// Allows a possibly more efficient 'push' by moving to the next input value
    /// and returning a reference to it.
    /// This may eliminate a copy by allowing direct construction in-place.<br>
    /// If asserts or exceptions are enabled, throws an etl::stack_full if the stack is already full.
    /// \return A reference to the position to 'push' to.
    //*************************************************************************
    reference push()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(stack_full));
#endif
      top_index = current_size++;
      ::new (&p_buffer[top_index]) T();
      ++construct_count;

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
        --construct_count;
      }
    }

    //*************************************************************************
    /// Removes the oldest item from the top of the stack.
    /// Does nothing if the stack is already empty.
    //*************************************************************************
    void pop()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(stack_empty));
#endif
      p_buffer[top_index].~T();
      --top_index;
      --current_size;
      --construct_count;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    istack& operator = (const istack& rhs)
    {
      if (&rhs != this)
      {
        clone(rhs);
      }

      return *this;
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

    // Disable copy construction.
    istack(const istack&);

    T* p_buffer; ///< The internal buffer.
  };

  //***************************************************************************
  ///\ingroup stack
  /// A fixed capacity stack.
  /// This stack does not suppo7rt concurrent access by different threads.
  /// \tparam T        The type this stack should support.
  /// \tparam MAX_SIZE The maximum capacity of the stack.
  //***************************************************************************
  template <typename T, const size_t SIZE>
  class stack : public etl::istack<T>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    stack()
      : etl::istack<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    stack(const stack& rhs)
      : etl::istack<T>(reinterpret_cast<T*>(&buffer[0]), SIZE)
    {
      etl::istack<T>::clone(rhs);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~stack()
    {
      etl::istack<T>::clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    stack& operator = (const stack& rhs)
    {
      if (&rhs != this)
      {
        etl::istack<T>::clone(rhs);
      }

      return *this;
    }

  private:

    /// The unintitialised buffer of T used in the stack.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[SIZE];
  };
}

#undef ETL_FILE

#endif
