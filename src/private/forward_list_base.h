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
    struct Node
    {
      Node()
        : next(nullptr)
      {
      }

      Node* next;
    };

  public:

    typedef size_t size_type; ///< The type used for determining the size of forward_list.

    //*************************************************************************
    /// Gets the size of the forward_list.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
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
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the forward_list is full.
    //*************************************************************************
    bool full() const
    {
      return current_size == MAX_SIZE;
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
    /// Reverses the forward_list.
    //*************************************************************************
    void reverse()
    {
      if (is_trivial_list())
      {
        return;
      }

      Node* p_last    = &start_node;
      Node* p_current = p_last->next;
      Node* p_next    = p_current->next;

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
    forward_list_base(size_type max_size)
      : next_free(0),
        current_size(0),
        MAX_SIZE(max_size)
    {
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    Node& get_head()
    {
      return *start_node.next;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const Node& get_head() const
    {
      return *start_node.next;
    }

    //*************************************************************************
    /// Insert a node.
    //*************************************************************************
    void insert_node_after(Node& position, Node& node)
    {
      // Connect to the forward_list.
      node.next = position.next;

      join(&position, &node);

      // One more.
      ++current_size;
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
    void join(Node* left, Node* right)
    {
      left->next = right;
    }

    Node start_node;          ///< The node that acts as the forward_list start.
    size_type next_free;      ///< The index of the next free node.
    size_type current_size;   ///< The number of items in the list.
    const size_type MAX_SIZE; ///< The maximum size of the forward_list.
  };
}

#undef ETL_FILE

#endif
