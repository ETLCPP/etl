///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_INTRUSIVE_STACK_INCLUDED
#define ETL_INTRUSIVE_STACK_INCLUDED

#include "platform.h"
#include "type_traits.h"
#include "error_handler.h"
#include "intrusive_links.h"

#include <stddef.h>

namespace etl
{
  //***************************************************************************
  /// Exception base for intrusive stack
  ///\ingroup intrusive_stack
  //***************************************************************************
  class intrusive_stack_exception : public etl::exception
  {
  public:

    ETL_CONSTEXPR
    intrusive_stack_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
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

    ETL_CONSTEXPR
    intrusive_stack_empty(string_type file_name_, numeric_type line_number_)
      : intrusive_stack_exception(ETL_ERROR_TEXT("intrusive_stack:empty", ETL_INTRUSIVE_STACK_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// intrusive_stack_value_is_already_linked exception.
  ///\ingroup intrusive_stack
  //***************************************************************************
  class intrusive_stack_value_is_already_linked : public intrusive_stack_exception
  {
  public:

    ETL_CONSTEXPR
    intrusive_stack_value_is_already_linked(string_type file_name_, numeric_type line_number_)
      : intrusive_stack_exception(ETL_ERROR_TEXT("intrusive_stack:value is already linked", ETL_INTRUSIVE_STACK_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// Base for intrusive stack. Stores elements derived any type that supports an 'etl_next' pointer member.
  /// \tparam TLink  The link type that the value is derived from.
  //***************************************************************************
  template <typename TLink>
  class intrusive_stack_base
  {
  public:

    // Node typedef.
    typedef TLink  link_type;

    //*************************************************************************
    /// Adds a value to the stack.
    ///\param value The value to push to the stack.
    //*************************************************************************
    ETL_CONSTEXPR14 void push(link_type& value)
    {
      ETL_ASSERT_OR_RETURN(!value.is_linked(), ETL_ERROR(intrusive_stack_value_is_already_linked));

      value.etl_next = p_top;
      p_top = &value;
      
      ++current_size;
    }

    //*************************************************************************
    /// Removes the oldest item from the top of the stack.
    /// Undefined behaviour if the stack is already empty.
    //*************************************************************************
    ETL_CONSTEXPR14 void pop()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT_OR_RETURN(!empty(), ETL_ERROR(intrusive_stack_empty));
#endif
      link_type* p_next = p_top->etl_next;
      p_top->clear();
      p_top = p_next;
      --current_size;
    }

    //*************************************************************************
    /// Removes the oldest item from the queue and pushes it to the destination.
    /// Undefined behaviour if the queue is already empty.
    /// NOTE: The destination must be an intrusive container that supports a push(TLink) member function.
    //*************************************************************************
    template <typename TContainer>
    ETL_CONSTEXPR14 void pop_into(TContainer& destination)
    {
      link_type* p_link = p_top;
      pop();
      destination.push(*p_link);
    }

    //*************************************************************************
    /// Reverses the stack order.
    //*************************************************************************
    ETL_CONSTEXPR14 void reverse()
    {
      link_type* previous = &terminator;
      link_type* current = p_top;
      link_type* next;

      while (current != &terminator)
      {
        next = current->etl_next;
        current->etl_next = previous;
        previous = current;
        current = next;
      }

      p_top = previous;
    }

    //*************************************************************************
    /// Clears the stack to the empty state.
    //*************************************************************************
    ETL_CONSTEXPR14 void clear()
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
    ETL_CONSTEXPR bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Returns the number of elements.
    //*************************************************************************
    ETL_CONSTEXPR size_t size() const
    {
      return current_size;
    }

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    ETL_CONSTEXPR14 intrusive_stack_base()
      : p_top(&terminator)
      , current_size(0)
    {      
    }

    //*************************************************************************
    /// Destructor
    //*************************************************************************
    ~intrusive_stack_base()
    {
    }

    link_type* p_top;      ///< The current top of the stack.
    link_type  terminator; ///< Terminator link of the queue.

    size_t current_size; ///< Counts the number of elements in the list.
  };

  //***************************************************************************
  ///\ingroup stack
  /// An intrusive stack. Stores elements derived from any type that supports an 'etl_next' pointer member.
  /// \warning This stack cannot be used for concurrent access from multiple threads.
  /// \tparam TValue The type of value that the stack holds.
  /// \tparam TLink  The link type that the value is derived from.
  //***************************************************************************
  template <typename TValue, typename TLink>
  class intrusive_stack : public etl::intrusive_stack_base<TLink>
  {
  public:

    // Node typedef.
    typedef typename etl::intrusive_stack_base<TLink>::link_type link_type;

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
    ETL_CONSTEXPR14 intrusive_stack()
    : intrusive_stack_base<TLink>()
    {
    }

    //*************************************************************************
    /// Gets a reference to the value at the top of the stack.
    /// Undefined behaviour if the stack is empty.
    /// \return A reference to the value at the top of the stack.
    //*************************************************************************
    ETL_CONSTEXPR14 reference top()
    {
      return *static_cast<TValue*>(this->p_top);
    }

    //*************************************************************************
    /// Gets a const reference to the value at the top of the stack.<br>
    /// \return A const reference to the value at the top of the stack.
    //*************************************************************************
    ETL_CONSTEXPR14 const_reference top() const
    {
      return *static_cast<const TValue*>(this->p_top);
    }

  private:

    // Disable copy construction and assignment.
    intrusive_stack(const intrusive_stack&);
    intrusive_stack& operator = (const intrusive_stack& rhs);
  };
}

#endif
