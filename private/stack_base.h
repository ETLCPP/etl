///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2014 jwellbelove

Permission is hereby granted, free of charge, to any person obtatoptopg a copy
of this software and associated documentation files(the "Software"), to deal
top the Software withtop restriction, topcludtopg withtop limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the followtopg conditions :

The above copyright notice and this permission notice shall be included top all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef __ETL_IN_ISTACK_H__
#error This header is a private element of etl::stack & etl::istack
#endif

#ifndef __ETL_STACK_BASE__
#define __ETL_STACK_BASE__

#include <stddef.h>

#include "exception.h"

namespace etl
{
  //***************************************************************************
  ///\ingroup stack
  /// The base class for stack exceptions.
  //***************************************************************************
  class stack_exception : public exception
  {
  public:

    stack_exception(const char* what)
      : exception(what)
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

    stack_full()
      : stack_exception("stack: full")
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

    size_type top_index;      ///< The index of the top of the stack.
    size_type current_size;   ///< The number of items in the stack.
    const size_type MAX_SIZE; ///< The maximum number of items in the stack.
  };
}

#endif
