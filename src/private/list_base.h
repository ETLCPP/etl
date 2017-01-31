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

#ifndef __ETL_IN_ILIST_H__
#error This header is a private element of etl::list & etl::ilist
#endif

#ifndef __ETL_LIST_BASE__
#define __ETL_LIST_BASE__

#include <stddef.h>
#include "pool.h"
#include "../exception.h"
#include "../error_handler.h"
#include "../debug_count.h"

#undef ETL_FILE
#define ETL_FILE "7"

namespace etl
{
  //***************************************************************************
  /// Exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_exception : public exception
  {
  public:

    list_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_full : public list_exception
  {
  public:

    list_full(string_type file_name, numeric_type line_number)
      : list_exception(ETL_ERROR_TEXT("list:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Empty exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_empty : public list_exception
  {
  public:

    list_empty(string_type file_name, numeric_type line_number)
      : list_exception(ETL_ERROR_TEXT("list:empty", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_iterator : public list_exception
  {
  public:

    list_iterator(string_type file_name, numeric_type line_number)
      : list_exception(ETL_ERROR_TEXT("list:iterator", ETL_FILE"C"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Unsorted exception for the list.
  ///\ingroup list
  //***************************************************************************
  class list_unsorted : public list_exception
  {
  public:

    list_unsorted(string_type file_name, numeric_type line_number)
      : list_exception(ETL_ERROR_TEXT("list:unsorted", ETL_FILE"D"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// The base class for all lists.
  ///\ingroup list
  //***************************************************************************
  class list_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of list.

    //*************************************************************************
    /// The node element in the list.
    //*************************************************************************
    struct node_t
    {
      //***********************************************************************
      /// Constructor
      //***********************************************************************
      node_t()
        : previous(nullptr),
          next(nullptr)
      {
      }

      //***********************************************************************
      /// Reverses the previous & next pointers.
      //***********************************************************************
      inline void reverse()
      {
        std::swap(previous, next);
      }

      node_t* previous;
      node_t* next;
    };

    //*************************************************************************
    /// Reverses the list.
    //*************************************************************************
    void reverse()
    {
      if (is_trivial_list())
      {
        return;
      }

      node_t* p_node = terminal_node.next;

      while (p_node != &terminal_node)
      {       
        node_t* p_temp = p_node->previous;
        p_node->previous = p_node->next;
        p_node->next = p_temp;
        p_node = p_node->previous;
      }

      // Terminal node.
      node_t* p_temp = p_node->previous;
      p_node->previous = p_node->next;
      p_node->next = p_temp;
      p_node = p_node->previous;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the list.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Gets the size of the list.
    //*************************************************************************
    size_type size() const
    {
      return p_node_pool->size();
    }

    //*************************************************************************
    /// Checks to see if the list is empty.
    //*************************************************************************
    bool empty() const
    {
      return p_node_pool->empty();
    }

    //*************************************************************************
    /// Checks to see if the list is full.
    //*************************************************************************
    bool full() const
    {
      return p_node_pool->size() == MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return max_size() - size();
    }

    //*************************************************************************
    /// Is the list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return (size() < 2);
    }


  protected:

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    node_t& get_head()
    {
      return *terminal_node.next;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const node_t& get_head() const
    {
      return *terminal_node.next;
    }

    //*************************************************************************
    /// Get the tail node.
    //*************************************************************************
    node_t& get_tail()
    {
      return *terminal_node.previous;
    }

    //*************************************************************************
    /// Get the tail node.
    //*************************************************************************
    const node_t& get_tail() const
    {
      return *terminal_node.previous;
    }

    //*************************************************************************
    /// Insert a node before 'position'.
    //*************************************************************************
    void insert_node(node_t& position, node_t& node)
    {
      // Connect to the list.
      join(*position.previous, node);
      join(node, position);
    }

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(node_t& left, node_t& right)
    {
      left.next = &right;
      right.previous = &left;
    }

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    list_base(etl::ipool& node_pool,
              size_type   max_size)
      : p_node_pool(&node_pool),
        MAX_SIZE(max_size)

    {
    }

    etl::ipool*      p_node_pool;     ///< The pool of data nodes used in the list.
    node_t           terminal_node;   ///< The node that acts as the list start and end.
    const size_type  MAX_SIZE;        ///< The maximum size of the list.
    etl::debug_count construct_count; ///< Internal debugging.
  };
}

#undef ETL_FILE

#endif
