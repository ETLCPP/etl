///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_INTRUSIVE_FORWARD_LIST__
#define __ETL_INTRUSIVE_FORWARD_LIST__

#if WIN32
#undef min
#endif

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>

#include "nullptr.h"
#include "type_traits.h"
#include "array.h"

namespace etl
{
  //***************************************************************************
  /// Exception for the intrusive_forward_list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  class intrusive_forward_list_exception : public exception
  {
  public:

    intrusive_forward_list_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the intrusive_forward_list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  class intrusive_forward_list_iterator_exception : public intrusive_forward_list_exception
  {
  public:

    intrusive_forward_list_iterator_exception()
      : intrusive_forward_list_exception("intrusive_forward_list: iterator problem")
    {
    }
  };

  //***************************************************************************
  /// Index exception for the intrusive_forward_list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  class intrusive_forward_list_index_exception : public intrusive_forward_list_exception
  {
  public:

    intrusive_forward_list_index_exception()
      : intrusive_forward_list_exception("intrusive_forward_list: index out of bounds")
    {
    }
  };

  namespace __private_intrusive_forward_list__
  {
    //***************************************************************************
    /// The node element in the intrusive_forward_list.
    //***************************************************************************
    class intrusive_forward_list_node_base
    {
    public:

      virtual intrusive_forward_list_node_base* get_next(size_t index) const
      {
        return base_next;
      }

      virtual void set_next(size_t index, intrusive_forward_list_node_base* pnext)
      {
        base_next = pnext;
      }

    private:

      intrusive_forward_list_node_base* base_next;
    };
  }

  //***************************************************************************
  /// The node element in the intrusive_forward_list.
  //***************************************************************************
  template <const size_t SIZE>
  struct intrusive_forward_list_node : public __private_intrusive_forward_list__::intrusive_forward_list_node_base
  {
  public:

    __private_intrusive_forward_list__::intrusive_forward_list_node_base* get_next(size_t index) const
    {
#ifdef _DEBUG
      if (index >= SIZE)
      {
        ETL_ERROR(intrusive_forward_list_index_exception());
      }
#endif
      return next[index];
    }

    void set_next(size_t index, __private_intrusive_forward_list__::intrusive_forward_list_node_base* pnext)
    {
#ifdef _DEBUG
      if (index >= SIZE)
      {
        ETL_ERROR(intrusive_forward_list_index_exception());
      }
#endif
      next[index] = pnext;
    }

  private:

    etl::array<__private_intrusive_forward_list__::intrusive_forward_list_node_base*, SIZE> next;
  };

  //***************************************************************************
  /// An intrusive forward list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  template <typename TValue>
  class intrusive_forward_list
  {
  public:

    // Node typedef.
    typedef __private_intrusive_forward_list__::intrusive_forward_list_node_base node_t;

    // STL style typedefs.
    typedef TValue        value_type;
    typedef TValue*       pointer;
    typedef const TValue* const_pointer;
    typedef TValue&       reference;
    typedef const TValue& const_reference;
    typedef size_t        size_type;

  public:

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public std::iterator<std::forward_iterator_tag, value_type>
    {
    public:

      friend class intrusive_forward_list;

      iterator()
        : p_node(nullptr),
          index(0)
      {
      }

      iterator(node_t& node, size_t index)
        : p_node(&node),
          index(index)
      {
      }

      iterator(const iterator& other)
        : p_node(other.p_node),
          index(other.index)
      {
      }

      iterator& operator ++()
      {
        p_node = p_node->get_next(index);
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        p_node = p_node->get_next(index);
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      reference operator *()
      {
        return static_cast<reference>(*p_node);
      }

      const_reference operator *() const
      {
        return static_cast<const_reference>(*p_node);
      }

      pointer operator &()
      {
        return static_cast<pointer>(p_node);
      }

      const_pointer operator &() const
      {
        return static_cast<const_pointer>(p_node);
      }

      pointer operator ->()
      {
        return static_cast<pointer>(p_node);
      }

      const_pointer operator ->() const
      {
        return static_cast<const_pointer>(p_node);
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      node_t* p_node;
      size_t  index;
    };

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public std::iterator<std::forward_iterator_tag, const value_type>
    {
    public:

      friend class intrusive_forward_list;

      const_iterator()
        : p_node(nullptr),
          index(0)
      {
      }

      const_iterator(node_t& node, size_t index)
        : p_node(&node),
          index(index)
      {
      }

      const_iterator(const node_t& node, size_t index)
        : p_node(&node),
          index(index)
      {
      }

      const_iterator(const typename intrusive_forward_list<TValue>::iterator& other)
        : p_node(other.p_node),
          index(other.index)
      {
      }

      const_iterator(const const_iterator& other)
        : p_node(other.p_node),
          index(other.index)
      {
      }

      const_iterator& operator ++()
      {
        p_node = p_node->get_next(index);
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        p_node = p_node->get_next(index);
        return temp;
      }

      const_iterator operator =(const const_iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return static_cast<const_reference>(*p_node);
      }

      const_pointer operator &() const
      {
        return static_cast<const_pointer>(p_node);
      }

      const_pointer operator ->() const
      {
        return static_cast<const_pointer>(p_node);
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_node == rhs.p_node;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      const node_t* p_node;
      size_t        index;
    };

		typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    intrusive_forward_list(size_t index)
      : index(index)
    {
      initialise();
    }

    //*************************************************************************
    /// Constructor from range
    //*************************************************************************
    template <typename TIterator>
    intrusive_forward_list(size_t index, TIterator first, TIterator last)
      : index(index)
    {
      assign(first, last);
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    iterator begin()
    {
      return iterator(get_head(), index);
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(get_head(), index);
    }

    //*************************************************************************
    /// Gets before the beginning of the intrusive_forward_list.
    //*************************************************************************
    iterator before_begin()
    {
      return iterator(start_node, index);
    }

    //*************************************************************************
    /// Gets before the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator before_begin() const
    {
      return const_iterator(start_node, index);
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(get_head(), index);
    }

    //*************************************************************************
    /// Gets the end of the intrusive_forward_list.
    //*************************************************************************
    iterator end()
    {
      return iterator();
    }

    //*************************************************************************
    /// Gets the end of the intrusive_forward_list.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator();
    }

    //*************************************************************************
    /// Gets the end of the intrusive_forward_list.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator();
    }

    //*************************************************************************
    /// Clears the intrusive_forward_list.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*************************************************************************
    /// Gets a reference to the first element.
    //*************************************************************************
    reference front()
    {
      return static_cast<reference>(get_head());
    }

    //*************************************************************************
    /// Gets a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return get_head();
    }

    //*************************************************************************
    /// Assigns a range of values to the intrusive_forward_list.
    /// If ETL_THROW_EXCEPTIONS & _DEBUG are defined throws forward_list_iterator if the iterators are reversed.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#ifdef _DEBUG
      difference_type count = std::distance(first, last);

      if (count < 0)
      {
        ETL_ERROR(intrusive_forward_list_iterator_exception());
      }
#endif

      initialise();

      node_t* p_last_node = &start_node;

      // Add all of the elements.
      while (first != last)
      {
        node_t& node = *first++;
        join(p_last_node, &node);
        join(&node, nullptr);
        p_last_node = &node;
        ++current_size;
      }
    }

    //*************************************************************************
    /// Pushes a value to the front of the intrusive_forward_list.
    //*************************************************************************
    void push_front(node_t& value)
    {
      insert_node_after(start_node, value);
    }

    //*************************************************************************
    /// Removes a value from the front of the intrusive_forward_list.
    //*************************************************************************
    void pop_front()
    {
      if (!empty())
      {
        remove_node_after(start_node);
      }
    }

    //*************************************************************************
    /// Reverses the intrusive_forward_list.
    //*************************************************************************
    void reverse()
    {
      if (is_trivial_list())
      {
        return;
      }
      
      node_t* p_last    = &start_node;
      node_t* p_current = p_last->get_next(index);
      node_t* p_next    = p_current->get_next(index);

      p_current->set_next(index, nullptr);

      while (p_next != nullptr)
      {
        p_last    = p_current;
        p_current = p_next;
        p_next    = p_current->get_next(index);

        p_current->set_next(index, p_last);
      }

      join(&start_node, p_current);
    }

    //*************************************************************************
    /// Inserts a value to the intrusive_forward_list after the specified position.
    //*************************************************************************
    iterator insert_after(iterator position, node_t& value)
    {
      insert_node_after(*position.p_node, value);

      return iterator(value, index);
    }

    //*************************************************************************
    /// Inserts a range of values to the intrusive_forward_list after the specified position.
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
      node_t* p_next  = p_first->get_next(index);

      // Join the ends.
      join(p_first, p_last);

      p_first = p_next;

      // Erase the ones in between.
      while (p_first != p_last)
      {
        // One less.
        --current_size;

        p_next  = p_first->get_next(index); // Remember the next node.
        p_first = p_next;                   // Move to the next node.
      }

      if (p_next == nullptr)
      {
        return end();
      }
      else
      {
        return iterator(*p_last, index);
      }
    }

    //*************************************************************************
    /// Removes all but the one element from every consecutive group of equal
    /// elements in the container.
    //*************************************************************************
    template <typename TIsEqual>
    void unique(TIsEqual isEqual)
    {
      if (empty())
      {
        return;
      }

      node_t* last    = &get_head();
      node_t* current = last->get_next(index);

      while (current != nullptr)
      {
        // Is this value the same as the last?
        if (isEqual(static_cast<reference>(*current), static_cast<reference>(*last)))
        {
          remove_node_after(*last);
        }
        else
        {
          // Move on one.
          last = current;
        }

        current = last->get_next(index);
      }
    }

    //*************************************************************************
    /// Sort using in-place merge sort algorithm.
    //*************************************************************************
    void sort()
    {
      sort(std::less<TValue>());
    }

    //*************************************************************************
    /// Sort using in-place merge sort algorithm.
    /// Uses a supplied predicate function or functor.
    /// This is not my algorithm. I got it off the web somewhere.
    //*************************************************************************
    template <typename TCompare>
    void sort(TCompare compare)
    {
      iterator i_left;
      iterator i_right;
      iterator i_node;
      iterator i_head;
      iterator i_tail;
      int      list_size = 1;
      int      number_of_merges;
      int      left_size;
      int      right_size;

      if (is_trivial_list())
      {
	      return;
      }

      while (true)
      {
        i_left = begin();
        i_head = before_begin();
        i_tail = before_begin();

        number_of_merges = 0;  // Count the number of merges we do in this pass.

        while (i_left != end())
        {
          ++number_of_merges;  // There exists a merge to be done.
          i_right = i_left;
          left_size = 0;

          // Step 'list_size' places along from left
          for (int i = 0; i < list_size; ++i)
          {
            ++left_size;

            ++i_right;

            if (i_right == end())
            {
              break;
            }
          }

          // If right hasn't fallen off end, we have two lists to merge.
          right_size = list_size;

          // Now we have two lists. Merge them.
          while (left_size > 0 || (right_size > 0 && i_right != end()))
          {
            // Decide whether the next node of merge comes from left or right.
            if (left_size == 0)
            {
		          // Left is empty. The node must come from right.
		          i_node = i_right;
              ++i_right;
              --right_size;
		        }
            else if (right_size == 0 || i_right == end())
            {
		          // Right is empty. The node must come from left.
		          i_node = i_left;
              ++i_left;
              --left_size;
		        }
            else if (compare(*i_left, *i_right))
            {
		          // First node of left is lower or same. The node must come from left.
		          i_node = i_left;
              ++i_left;
              --left_size;
		        }
            else
            {
		          // First node of right is lower. The node must come from right.
		          i_node  = i_right;
              ++i_right;
              --right_size;
		        }

            // Add the next node to the merged head.
            if (i_head == before_begin())
            {
                join(i_head.p_node, i_node.p_node);
                i_head = i_node;
                i_tail = i_node;
            }
            else
            {
                join(i_tail.p_node, i_node.p_node);
                i_tail = i_node;
            }

            i_tail.p_node->set_next(index, nullptr);
          }

          // Now left has stepped `list_size' places along, and right has too.
          i_left = i_right;
        }

        // If we have done only one merge, we're finished.
        if (number_of_merges <= 1)   // Allow for number_of_merges == 0, the empty head case
        {
            return;
        }

        // Otherwise repeat, merging lists twice the size
        list_size *= 2;
      }
    }

    //*************************************************************************
    // Removes the values specified.
    //*************************************************************************
    void remove(node_t& value)
    {
      iterator i_item = begin();
      iterator i_last_item = before_begin();

      while (i_item != end())
      {
        if (static_cast<reference>(*i_item) == static_cast<reference>(value))
        {
          i_item = erase_after(i_last_item);
        }
        else
        {
          ++i_item;
          ++i_last_item;
        }
      }
    }

    //*************************************************************************
    /// Removes according to a predicate.
    //*************************************************************************
    template <typename TPredicate>
    void remove_if(TPredicate predicate)
    {
      iterator i_item = begin();
      iterator i_last_item = before_begin();

      while (i_item != end())
      {
        if (predicate(*i_item))
        {
          i_item = erase_after(i_last_item);
        }
        else
        {
          ++i_item;
          ++i_last_item;
        }
      }
    }

    //*************************************************************************
    /// Returns true if the list has not elements.
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

    node_t start_node;   ///< The node that acts as the intrusive_forward_list start.
    size_t current_size; ///< The number of elements in the list.
    size_t index;        ///< The index level of the node that this list operates on.

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(node_t* left, node_t* right)
    {
      left->set_next(index, right);
    }

    //*************************************************************************
    /// Is the intrusive_forward_list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return current_size <= 1;
    }

    //*************************************************************************
    /// Insert a node.
    //*************************************************************************
    void insert_node_after(node_t& position, node_t& node)
    {
      // Connect to the intrusive_forward_list.
      join(&node,     position.get_next(index));
      join(&position, &node);
      ++current_size;
    }

    //*************************************************************************
    /// Remove a node.
    //*************************************************************************
    void remove_node_after(node_t& node)
    {
      // The node to erase.
      node_t* p_node = node.get_next(index);

      if (p_node != nullptr)
      {
        // Disconnect the node from the intrusive_forward_list.
        join(&node, p_node->get_next(index));
        --current_size;
      }
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    node_t& get_head()
    {
      return *start_node.get_next(index);
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const node_t& get_head() const
    {
      return *start_node.get_next(index);
    }

    //*************************************************************************
    /// Initialise the intrusive_forward_list.
    //*************************************************************************
    void initialise()
    {
      start_node.set_next(index, nullptr);
      current_size = 0;
    }

    // Disabled.
    intrusive_forward_list(const intrusive_forward_list& other);
    intrusive_forward_list& operator = (const intrusive_forward_list& rhs);
  };
}

#if WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif
