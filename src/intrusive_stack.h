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
#include "parameter_type.h"
#include "error_handler.h"

namespace etl
{
  //***************************************************************************
  ///\ingroup stack
  /// An intrusive stack. Stores elements derived from etl::forward_link
  /// \warning This stack cannot be used for concurrent access from multiple threads.
  /// \tparam TValue The type of value that the stack holds.
  /// \tparam TLink  The link type that the value is derived from.
  //***************************************************************************
  template <typename TValue, typename TLink = etl::bidirectional_link<0> >
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

  public:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    intrusive_stack()
    : p_top(&base)
    {

    }

    //*************************************************************************
    /// Gets a reference to the value at the top of the stack.
    /// Undefined behaviour if the stack is empty.
    /// \return A reference to the value at the top of the stack.
    //*************************************************************************
    reference top()
    {
      return *p_top;
    }

    //*************************************************************************
    /// Adds a value to the stack.
    ///\param value The value to push to the stack.
    //*************************************************************************
    void push(link_type& value)
    {
      etl::link(p_top, value);
      p_top = &value;
    }

    //*************************************************************************
    /// Gets a const reference to the value at the top of the stack.<br>
    /// \return A const reference to the value at the top of the stack.
    //*************************************************************************
    const_reference top() const
    {
      return *p_top;
    }

    //*************************************************************************
    /// Clears the stack to the empty state.
    //*************************************************************************
    void clear()
    {
        unlink(base, *p_top);
    }

    //*************************************************************************
    /// Removes the oldest item from the top of the stack.
    /// Does nothing if the stack is already empty.
    //*************************************************************************
    void pop()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(p_top != &base, ETL_ERROR(intrusive_stack_empty));
#endif
      link_type* p_previous = p_top->etl_previous;
      unlink(p_top);
      p_top = p_previous;
    }

  private:

    // Disable copy construction and assignment.
    intrusive_stack(const intrusive_stack&);
    intrusive_stack& operator = (const intrusive_stack& rhs)

    link_type  base;  // The base of the stack.
    link_type* p_top; // The current top of the stack.
  };
}

#endif
