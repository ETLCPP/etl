///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef __ETL_INTRUSIVE_STACK__
#define __ETL_INTRUSIVE_STACK__

#include <stddef.h>

#include "type_traits.h"
#include "error_handler.h"
#include "intrusive_links.h"

#define ETL_FILE "28"

namespace etl
{
  //***************************************************************************
  /// Exception base for intrusive stack
  ///\ingroup intrusive_stack
  //***************************************************************************
  class intrusive_stack_exception : public etl::exception
  {
  public:

    intrusive_stack_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// intrusive_stack empty exception.
  ///\ingroup intrusive_stack
  //***************************************************************************
  class intrusive_stack_empty : public intrusive_stack_exception
  {
  public:

    intrusive_stack_empty(string_type file_name, numeric_type line_number)
      : intrusive_stack_exception(ETL_ERROR_TEXT("intrusive_stack:empty", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// An intrusive stack. Stores elements derived from etl::forward_link
  /// \warning This stack cannot be used for concurrent access from multiple threads.
  /// \tparam TValue The type of value that the stack holds.
  /// \tparam TLink  The link type that the value is derived from.
  //***************************************************************************
  template <typename TValue, typename TLink>
  class intrusive_stack
  {
  public:

    // Node typedef.
    typedef TLink             link_type;

    // STL style typedefs.
    typedef TValue            value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef size_t            size_type;

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    intrusive_stack()
    : p_top(nullptr),
      current_size(0)
    {
    }

    //*************************************************************************
    /// Gets a reference to the value at the top of the stack.
    /// Undefined behaviour if the stack is empty.
    /// \return A reference to the value at the top of the stack.
    //*************************************************************************
    reference top()
    {
      return *static_cast<TValue*>(p_top);
    }

    //*************************************************************************
    /// Gets a const reference to the value at the top of the stack.<br>
    /// \return A const reference to the value at the top of the stack.
    //*************************************************************************
    const_reference top() const
    {
      return *static_cast<const TValue*>(p_top);
    }

    //*************************************************************************
    /// Adds a value to the stack.
    ///\param value The value to push to the stack.
    //*************************************************************************
    void push(link_type& value)
    {
      value.clear();

      if (p_top != nullptr)
      {
        etl::link(value, p_top);
      }

      p_top = &value;

      ++current_size;
    }

    //*************************************************************************
    /// Removes the oldest item from the top of the stack.
    /// Undefined behaviour if the stack is already empty.
    //*************************************************************************
    void pop()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(intrusive_stack_empty));
#endif
      link_type* p_next = p_top->etl_next;
      p_top = p_next;
      --current_size;
    }

    //*************************************************************************
    /// Clears the stack to the empty state.
    //*************************************************************************
    void clear()
    {
      while (!empty())
      {
        pop();
      }

      current_size = 0;
    }

    //*************************************************************************
    /// Checks if the stack is in the empty state.
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Returns the number of elements.
    //*************************************************************************
    size_t size() const
    {
      return current_size;
    }

  private:

    // Disable copy construction and assignment.
    intrusive_stack(const intrusive_stack&);
    intrusive_stack& operator = (const intrusive_stack& rhs);

    link_type* p_top; // The current top of the stack.

    size_t current_size; ///< Counts the number of elements in the list.
  };
}

#undef ETL_FILE

#endif
