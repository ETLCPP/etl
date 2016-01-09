///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove

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

//*****************************************************************************
// This forward list is intended for internal library use.
// It may be used as a very low cost intrusive forward list.
// Elements in the list are accessed as basic_intrusive_forward_list_node.
// Code using this list will be required to cast to the real data type.
// etl::basic_intrusive_forward_list will throw no exceptions or errors.
//*****************************************************************************

#ifndef __ETL_BASIC_INTRUSIVE_FORWARD_LIST__
#define __ETL_BASIC_INTRUSIVE_FORWARD_LIST__

#include <iterator>
#include <stddef.h>

#include "nullptr.h"
#include "type_traits.h"
#include "basic_intrusive_forward_list_node.h"
#include "error_handler.h"

namespace etl
{
  //***************************************************************************
  /// Exception for the intrusive_forward_list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  class basic_intrusive_forward_list_exception : public exception
  {
  public:

    basic_intrusive_forward_list_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Empty exception for the intrusive_forward_list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  class basic_intrusive_forward_list_empty : public basic_intrusive_forward_list_exception
  {
  public:

    basic_intrusive_forward_list_empty(string_type file_name, numeric_type line_number)
      : basic_intrusive_forward_list_exception(ETL_ERROR_TEXT("basic_intrusive_forward_list:empty", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// An intrusive forward list.
  ///\ingroup basic_intrusive_forward_list
  //***************************************************************************
  class basic_intrusive_forward_list
  {
  public:

    // Node typedef.
    typedef basic_intrusive_forward_list_node node_t;

    // STL style typedefs.
    typedef node_t        value_type;
    typedef node_t*       pointer;
    typedef const node_t* const_pointer;
    typedef node_t&       reference;
    typedef const node_t& const_reference;
    typedef size_t        size_type;

  public:

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public std::iterator<std::forward_iterator_tag, value_type>
    {
    public:

      friend class basic_intrusive_forward_list;

      iterator()
        : p_node(nullptr)
      {
      }

      iterator(node_t& node)
        : p_node(&node)
      {
      }

      iterator(const iterator& other)
        : p_node(other.p_node)
      {
      }

      iterator& operator ++()
      {
        p_node = p_node->bifln_next;
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        p_node = p_node->bifln_next;
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      reference operator *()
      {
        return *p_node;
      }

      const_reference operator *() const
      {
        return *p_node;
      }

      pointer operator &()
      {
        return p_node;
      }

      const_pointer operator &() const
      {
        return p_node;
      }

      pointer operator ->()
      {
        return p_node;
      }

      const_pointer operator ->() const
      {
        return p_node;
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

      template<typename T>
      T& ref_cast()
      {
        return static_cast<T&>(*p_node);
      }

      template<typename T>
      const T& ref_cast() const
      {
        return static_cast<const T&>(*p_node);
      }

    private:

      node_t* p_node;
    };

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public std::iterator<std::forward_iterator_tag, const value_type>
    {
    public:

      friend class basic_intrusive_forward_list;

      const_iterator()
        : p_node(nullptr)
      {
      }

      const_iterator(node_t& node)
        : p_node(&node)
      {
      }

      const_iterator(const node_t& node)
        : p_node(&node)
      {
      }

      const_iterator(const basic_intrusive_forward_list::iterator& other)
        : p_node(other.p_node)
      {
      }

      const_iterator(const const_iterator& other)
        : p_node(other.p_node)
      {
      }

      const_iterator& operator ++()
      {
        p_node = p_node->bifln_next;
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        p_node = p_node->bifln_next;
        return temp;
      }

      const_iterator operator =(const const_iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return *p_node;
      }

      const_pointer operator &() const
      {
        return p_node;
      }

      const_pointer operator ->() const
      {
        return p_node;
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

      template<typename T>
      const T& ref_cast() const
      {
        return static_cast<const T&>(*p_node);
      }

    private:

      const node_t* p_node;
    };

		typedef std::iterator_traits<iterator>::difference_type difference_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    basic_intrusive_forward_list()
    {
      initialise();
    }

    //*************************************************************************
    /// Constructor from range
    //*************************************************************************
    template <typename TIterator>
    basic_intrusive_forward_list(TIterator first, TIterator last)
    {
      assign(first, last);
    }

    //*************************************************************************
    /// Gets the beginning of the basic_intrusive_forward_list.
    //*************************************************************************
    iterator begin()
    {
      return iterator(get_head());
    }

    //*************************************************************************
    /// Gets the beginning of the basic_intrusive_forward_list.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets before the beginning of the basic_intrusive_forward_list.
    //*************************************************************************
    iterator before_begin()
    {
      return iterator(start_node);
    }

    //*************************************************************************
    /// Gets before the beginning of the basic_intrusive_forward_list.
    //*************************************************************************
    const_iterator before_begin() const
    {
      return const_iterator(start_node);
    }

    //*************************************************************************
    /// Gets the beginning of the basic_intrusive_forward_list.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the end of the basic_intrusive_forward_list.
    //*************************************************************************
    iterator end()
    {
      return iterator();
    }

    //*************************************************************************
    /// Gets the end of the basic_intrusive_forward_list.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator();
    }

    //*************************************************************************
    /// Gets the end of the basic_intrusive_forward_list.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator();
    }

    //*************************************************************************
    /// Clears the basic_intrusive_forward_list.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*************************************************************************
    /// Gets a reference to the first element.
    //*************************************************************************
    template <typename T>
    T& front()
    {
      return static_cast<T&>(get_head());
    }

    //*************************************************************************
    /// Gets a const reference to the first element.
    //*************************************************************************
    template <typename T>
    const_reference front() const
    {
      return static_cast<const T&>(get_head());
    }

    //*************************************************************************
    /// Assigns a range of values to the basic_intrusive_forward_list.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      initialise();

      node_t* p_last_node = &start_node;

      // Add all of the elements.
      while (first != last)
      {
        node_t& node = *first++;
        join(p_last_node, &node);
        join(&node, nullptr);
        p_last_node = &node;
      }
    }

    //*************************************************************************
    /// Pushes a value to the front of the basic_intrusive_forward_list.
    //*************************************************************************
    void push_front(node_t& value)
    {
      insert_node_after(start_node, value);
    }

    //*************************************************************************
    /// Removes a value from the front of the basic_intrusive_forward_list.
    //*************************************************************************
    void pop_front()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(basic_intrusive_forward_list_empty));
#endif
      remove_node_after(start_node);
    }

    //*************************************************************************
    /// Reverses the basic_intrusive_forward_list.
    //*************************************************************************
    void reverse()
    {
      if ((start_node.bifln_next == nullptr) || (start_node.bifln_next->bifln_next == nullptr))
      {
        return;
      }
      
      node_t* first  = nullptr;         // To keep first node
      node_t* second = start_node.bifln_next; // To keep second node
      node_t* track  = start_node.bifln_next; // Track the list

      while (track != NULL)
      {
        track = track->bifln_next;  // Track point to next node;
        second->bifln_next = first; // Second node point to first
        first  = second;            // Move first node to next
        second = track;             // Move second node to next
      }

      join(&start_node, first);
    }

    //*************************************************************************
    /// Inserts a value to the basic_intrusive_forward_list after the specified position.
    //*************************************************************************
    iterator insert_after(iterator position, node_t& value)
    {
      insert_node_after(*position.p_node, value);

      return iterator(value);
    }

    //*************************************************************************
    /// Inserts a range of values to the basic_intrusive_forward_list after the specified position.
    //*************************************************************************
    template <typename TIterator>
    void insert_after(iterator position, TIterator first, TIterator last)
    {
      while (first != last)
      {
        // Set up the next free node.
        insert_node_after(*position.p_node, *first++);
        ++position;
      }
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase_after(iterator position)
    {
      iterator next(position);
      ++next;
      ++next;

      remove_node_after(*position.p_node);

      return next;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase_after(iterator first, iterator last)
    {
      node_t* p_first = first.p_node;
      node_t* p_last  = last.p_node;
      node_t* p_next  = p_first->bifln_next;

      // Join the ends.
      join(p_first, p_last);

      p_first = p_next;

      // Erase the ones in between.
      while (p_first != p_last)
      {
        p_next  = p_first->bifln_next; // Remember the next node.
        p_first = p_next;              // Move to the next node.
      }

      if (p_next == nullptr)
      {
        return end();
      }
      else
      {
        return iterator(*p_last);
      }
    }
    
    //*************************************************************************
    /// Returns true if the list has no elements.
    //*************************************************************************
    bool empty() const
    {
      return start_node.bifln_next == nullptr;
    }

  private:

    node_t start_node; ///< The node that acts as the basic_intrusive_forward_list start.

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(node_t* left, node_t* right)
    {
      left->bifln_next = right;
    }

    //*************************************************************************
    /// Insert a node.
    //*************************************************************************
    void insert_node_after(node_t& position, node_t& node)
    {
      // Connect to the basic_intrusive_forward_list.
      join(&node,     position.bifln_next);
      join(&position, &node);
    }

    //*************************************************************************
    /// Remove a node.
    //*************************************************************************
    void remove_node_after(node_t& node)
    {
      // The node to erase.
      node_t* p_node = node.bifln_next;

      if (p_node != nullptr)
      {
        // Disconnect the node from the basic_intrusive_forward_list.
        join(&node, p_node->bifln_next);
      }
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    node_t& get_head()
    {
      return *start_node.bifln_next;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const node_t& get_head() const
    {
      return *start_node.bifln_next;
    }

    //*************************************************************************
    /// Initialise the basic_intrusive_forward_list.
    //*************************************************************************
    void initialise()
    {
      start_node.bifln_next = nullptr;
    }

    // Disabled.
    basic_intrusive_forward_list(const basic_intrusive_forward_list& other);
    basic_intrusive_forward_list& operator = (const basic_intrusive_forward_list& rhs);
  };
}

#endif
