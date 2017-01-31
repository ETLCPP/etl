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

#ifndef __ETL_IN_IFORWARD_LIST_H__
#error This header is a private element of etl::forward_list & etl::iforward_list
#endif

#ifndef __ETL_LIST_BASE__
#define __ETL_LIST_BASE__

#include <stddef.h>
#include "../exception.h"
#include "../error_handler.h"
#include "../debug_count.h"

#undef ETL_FILE
#define ETL_FILE "6"

namespace etl
{
  //***************************************************************************
  /// Exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_exception : public exception
  {
  public:

    forward_list_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_full : public forward_list_exception
  {
  public:

    forward_list_full(string_type file_name, numeric_type line_number)
      : forward_list_exception(ETL_ERROR_TEXT("forward_list:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Empty exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_empty : public forward_list_exception
  {
  public:

    forward_list_empty(string_type file_name, numeric_type line_number)
      : forward_list_exception(ETL_ERROR_TEXT("forward_list:empty", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_iterator : public forward_list_exception
  {
  public:

    forward_list_iterator(string_type file_name, numeric_type line_number)
      : forward_list_exception(ETL_ERROR_TEXT("forward_list:iterator", ETL_FILE"C"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// The base class for all forward_lists.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_base
  {
  protected:

    //*************************************************************************
    /// The node element in the forward_list.
    //*************************************************************************
    struct node_t
    {
      node_t()
        : next(nullptr)
      {
      }

      node_t* next;
    };

  public:

    typedef size_t size_type; ///< The type used for determining the size of forward_list.

    //*************************************************************************
    /// Gets the size of the forward_list.
    //*************************************************************************
    size_type size() const
    {
      return p_node_pool->size();
    }

    //*************************************************************************
    /// Gets the maximum possible size of the forward_list.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Checks to see if the forward_list is empty.
    //*************************************************************************
    bool empty() const
    {
      return p_node_pool->empty();
    }

    //*************************************************************************
    /// Checks to see if the forward_list is full.
    //*************************************************************************
    bool full() const
    {
      return p_node_pool->full();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return p_node_pool->available();
    }

    //*************************************************************************
    /// Reverses the forward_list.
    //*************************************************************************
    void reverse()
    {
      if (is_trivial_list())
      {
        return;
      }

      node_t* p_last    = &start_node;
      node_t* p_current = p_last->next;
      node_t* p_next    = p_current->next;

      p_current->next = nullptr;

      while (p_next != nullptr)
      {
        p_last    = p_current;
        p_current = p_next;
        p_next    = p_current->next;

        p_current->next = p_last;
      }

      join(&start_node, p_current);
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    forward_list_base(etl::ipool& node_pool, size_type max_size)
      : p_node_pool(&node_pool),
        MAX_SIZE(max_size)
    {
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    node_t& get_head()
    {
      return *start_node.next;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const node_t& get_head() const
    {
      return *start_node.next;
    }

    //*************************************************************************
    /// Insert a node.
    //*************************************************************************
    inline void insert_node_after(node_t& position, node_t& node)
    {
      // Connect to the forward_list.
      join(&node, position.next);
      join(&position, &node);
    }

    //*************************************************************************
    /// Is the forward_list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return (size() < 2);
    }

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(node_t* left, node_t* right)
    {
      left->next = right;
    }

    node_t           start_node;      ///< The node that acts as the forward_list start.
    etl::ipool*      p_node_pool;     ///< The pool of data nodes used in the list.
    const size_type  MAX_SIZE;        ///< The maximum size of the forward_list.
    etl::debug_count construct_count; ///< Internal debugging.
  };
}

#undef ETL_FILE

#endif
