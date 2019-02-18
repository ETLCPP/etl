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

#ifndef ETL_INTRUSIVE_QUEUE_INCLUDED
#define ETL_INTRUSIVE_QUEUE_INCLUDED

#include <stddef.h>

#include "platform.h"
#include "type_traits.h"
#include "error_handler.h"
#include "intrusive_links.h"

#undef ETL_FILE
#define ETL_FILE "29"

namespace etl
{
  //***************************************************************************
  /// Exception base for intrusive queue
  ///\ingroup intrusive_queue
  //***************************************************************************
  class intrusive_queue_exception : public etl::exception
  {
  public:

    intrusive_queue_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// intrusive_queue empty exception.
  ///\ingroup intrusive_queue
  //***************************************************************************
  class intrusive_queue_empty : public intrusive_queue_exception
  {
  public:

    intrusive_queue_empty(string_type file_name_, numeric_type line_number_)
      : intrusive_queue_exception(ETL_ERROR_TEXT("intrusive_queue:empty", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup queue
  /// Base for intrusive queue. Stores elements derived any type that supports an 'etl_next' pointer member.
  /// \tparam TLink  The link type that the value is derived from.
  //***************************************************************************
  template <typename TLink>
  class intrusive_queue_base
  {
  public:

    // Node typedef.
    typedef TLink link_type;

    //*************************************************************************
    /// Adds a value to the queue.
    ///\param value The value to push to the queue.
    //*************************************************************************
    void push(link_type& value)
    {
      value.clear();

      if (p_back != nullptr)
      {
        etl::link(p_back, value);
      }
      else
      {
        p_front = &value;
      }

      p_back = &value;

      ++current_size;
    }

    //*************************************************************************
    /// Removes the oldest item from the queue.
    /// Undefined behaviour if the queue is already empty.
    //*************************************************************************
    void pop()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(intrusive_queue_empty));
#endif
      link_type* p_next = p_front->etl_next;

      p_front = p_next;

      // Now empty?
      if (p_front == nullptr)
      {
        p_back = nullptr;
      }

      --current_size;
    }

    //*************************************************************************
    /// Removes the oldest item from the queue and pushes it to the destination.
    /// Undefined behaviour if the queue is already empty.
    /// NOTE: The destination must be an intrusize container that supports a push(TLink) member function.
    //*************************************************************************
    template <typename TContainer>
    void pop_into(TContainer& destination)
    {
      link_type* p_link = p_front;
      pop();
      destination.push(*p_link);
    }

    //*************************************************************************
    /// Clears the queue to the empty state.
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
    /// Checks if the queue is in the empty state.
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

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    intrusive_queue_base()
      : p_front(nullptr),
        p_back(nullptr),
        current_size(0)
    {
    }

    //*************************************************************************
    /// Destructor
    //*************************************************************************
    ~intrusive_queue_base()
    {
    }

    link_type* p_front; ///< The current front of the queue.
    link_type* p_back;  ///< The current back of the queue.

    size_t current_size; ///< Counts the number of elements in the list.
  };

  //***************************************************************************
  ///\ingroup queue
  /// An intrusive queue. Stores elements derived from any type that supports an 'etl_next' pointer member.
  /// \warning This queue cannot be used for concurrent access from multiple threads.
  /// \tparam TValue The type of value that the queue holds.
  /// \tparam TLink  The link type that the value is derived from.
  //***************************************************************************
  template <typename TValue, typename TLink>
  class intrusive_queue : public etl::intrusive_queue_base<TLink>
  {
  public:

    // Node typedef.
    typedef typename etl::intrusive_queue_base<TLink> link_type;

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
    intrusive_queue()
      : intrusive_queue_base<TLink>()
    {
    }

    //*************************************************************************
    /// Gets a reference to the value at the front of the queue.
    /// Undefined behaviour if the queue is empty.
    /// \return A reference to the value at the front of the queue.
    //*************************************************************************
    reference front()
    {
      return *static_cast<TValue*>(this->p_front);
    }

    //*************************************************************************
    /// Gets a reference to the value at the back of the queue.
    /// Undefined behaviour if the queue is empty.
    /// \return A reference to the value at the back of the queue.
    //*************************************************************************
    reference back()
    {
      return *static_cast<TValue*>(this->p_back);
    }

    //*************************************************************************
    /// Gets a const reference to the value at the front of the queue.
    /// Undefined behaviour if the queue is empty.
    /// \return A const reference to the value at the front of the queue.
    //*************************************************************************
    const_reference front() const
    {
      return *static_cast<const TValue*>(this->p_front);
    }

    //*************************************************************************
    /// Gets a reference to the value at the back of the queue.
    /// Undefined behaviour if the queue is empty.
    /// \return A reference to the value at the back of the queue.
    //*************************************************************************
    const_reference back() const
    {
      return *static_cast<const TValue*>(this->p_back);
    }

  private:

    // Disable copy construction and assignment.
    intrusive_queue(const intrusive_queue&);
    intrusive_queue& operator = (const intrusive_queue& rhs);
  };
}

#undef ETL_FILE

#endif
