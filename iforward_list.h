///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#ifndef __ETL_ILIST__
#define __ETL_ILIST__
#define __ETL_IN_ILIST_H__

#if WIN32
#undef min
#endif

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>

#include "nullptr.h"
#include "forward_list_base.h"
#include "type_traits.h"
#include "parameter_type.h"

namespace etl
{
  //***************************************************************************
  /// A templated base for all etl::forward_list types.
  ///\ingroup forward_list
  //***************************************************************************
  template <typename T>
  class iforward_list : public forward_list_base
  {
  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

  protected:

    typedef typename parameter_type<T, is_fundamental<T>::value || is_pointer<T>::value>::type parameter_t;

    //*************************************************************************
    /// The node element in the forward_list.
    //*************************************************************************
    struct Data_Node;

    struct Node
    {
      Node()
        : next(nullptr)
      {
      }

      void mark_as_free()
      {
        next = nullptr;
      }

      bool is_free() const
      {
        return next == nullptr;
      }

      Node* next;
    };

    //*************************************************************************
    /// The data node element in the forward_list.
    //*************************************************************************
    struct Data_Node : public Node
    {
      T value;
    };

  public:

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public std::iterator<std::forward_iterator_tag, T>
    {
    public:

      friend class iforward_list;

      iterator()
        : p_node(nullptr)
      {
      }

      iterator(Node& node)
        : p_node(&node)
      {
      }

      iterator(const iterator& other)
        : p_node(other.p_node)
      {
      }

      iterator& operator ++()
      {
        p_node = p_node->next;
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        p_node = p_node->next;
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      reference operator *()
      {
        return iforward_list::data_cast(p_node)->value;
      }

      const_reference operator *() const
      {
        return iforward_list::data_cast(p_node)->value;
      }

      pointer operator &()
      {
        return &(iforward_list::data_cast(p_node)->value);
      }

      const_pointer operator &() const
      {
        return &(iforward_list::data_cast(p_node)->value);
      }

      pointer operator ->()
      {
        return &(iforward_list::data_cast(p_node)->value);
      }

      const_pointer operator ->() const
      {
        return &(iforward_list::data_cast(p_node)->value);
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

      Node* p_node;
    };

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public std::iterator<std::forward_iterator_tag, const T>
    {
    public:

      friend class iforward_list;

      const_iterator()
        : p_node(nullptr)
      {
      }

      const_iterator(Node& node)
        : p_node(&node)
      {
      }

      const_iterator(const Node& node)
        : p_node(&node)
      {
      }

      const_iterator(const typename iforward_list::iterator& other)
        : p_node(other.p_node)
      {
      }

      const_iterator(const const_iterator& other)
        : p_node(other.p_node)
      {
      }

      const_iterator& operator ++()
      {
        p_node = p_node->next;
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        p_node = p_node->next;
        return temp;
      }

      const_iterator operator =(const const_iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return iforward_list::data_cast(p_node)->value;
      }

      const_pointer operator &() const
      {
        return iforward_list::data_cast(p_node)->value;
      }

      const_pointer operator ->() const
      {
        return &(iforward_list::data_cast(p_node)->value);
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

      const Node* p_node;
    };

		typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    //*************************************************************************
    /// Gets the beginning of the forward_list.
    //*************************************************************************
    iterator begin()
    {
      return iterator(data_cast(get_head()));
    }

    //*************************************************************************
    /// Gets the beginning of the forward_list.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(data_cast(get_head()));
    }

    //*************************************************************************
    /// Gets before the beginning of the forward_list.
    //*************************************************************************
    iterator before_begin()
    {
      return iterator(static_cast<Data_Node&>(start_node));
    }

    //*************************************************************************
    /// Gets before the beginning of the forward_list.
    //*************************************************************************
    const_iterator before_begin() const
    {
      return const_iterator(static_cast<const Data_Node&>(start_node));
    }

    //*************************************************************************
    /// Gets the beginning of the forward_list.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the end of the forward_list.
    //*************************************************************************
    iterator end()
    {
      return iterator(end_node);
    }

    //*************************************************************************
    /// Gets the end of the forward_list.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(static_cast<const Data_Node&>(end_node));
    }

    //*************************************************************************
    /// Gets the end of the forward_list.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator(static_cast<const Data_Node&>(end_node));
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iforward_list& operator = (const iforward_list& rhs)
    {
      assign(rhs.cbegin(), rhs.cend());

      return *this;
    }

    //*************************************************************************
    /// Clears the forward_list.
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
      return data_cast(get_head()).value;
    }

    //*************************************************************************
    /// Gets a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return data_cast(get_head()).value;
    }

    //*************************************************************************
    /// Assigns a range of values to the forward_list.
		/// If ETL_THROW_EXCEPTIONS is defined throws etl::forward_list_full if the forward_list does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS & _DEBUG are defined throws forward_list_iterator if the iterators are reversed.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      // Reset the links.
      join(start_node, end_node);
      join(end_node, start_node);

      size_t i = 0;
      Node* p_last_node = &start_node;

      // Add all of the elements.
      while (first != last)
      {
        if (i < MAX_SIZE)
        {
          Data_Node& data_node = node_pool[i++];
          data_node.value = *first++;
          join(*p_last_node, data_node);
          join(data_node, end_node);
          p_last_node = &data_node;
        }
        else
#ifdef ETL_THROW_EXCEPTIONS
        {
          throw forward_list_full();
        }
#else
        {
          error_handler::error(forward_list_full());
        }
#endif
      }

      next_free = i;
      count     = i;

      // Clear the remaining elements in the node pool.
      while (i < MAX_SIZE)
      {
        node_pool[i].mark_as_free();
        ++i;
      }
    }

    //*************************************************************************
    /// Assigns 'n' copies of a value to the forward_list.
    //*************************************************************************
    void assign(size_t n, parameter_t value)
    {
      // Reset the links.
      join(start_node, end_node);
      join(end_node, start_node);

      size_t i = 0;
      Node* p_last_node = &start_node;

      // Add all of the elements.
      while (i < n)
      {
        if (i < MAX_SIZE)
        {
          Data_Node& data_node = node_pool[i++];
          data_node.value = value;
          join(*p_last_node, data_node);
          join(data_node, end_node);
          p_last_node = &data_node;
        }
        else
#ifdef ETL_THROW_EXCEPTIONS
        {
          throw forward_list_full();
        }
#else
        {
          error_handler::error(forward_list_full());
        }
#endif
      }

      next_free = i;
      count     = i;

      // Clear the remaining elements in the node pool.
      while (i < MAX_SIZE)
      {
        node_pool[i].mark_as_free();
        ++i;
      }
    }

    //*************************************************************************
    /// Adds a node to the front of the forward_list so a new value can be assigned to front().
    //*************************************************************************
    void push_front()
    {
      if (!full())
      {
        Data_Node& data_node = node_pool[next_free];

        insert_node(get_head(), data_node);
      }
      else
#ifdef ETL_THROW_EXCEPTIONS
      {
        throw forward_list_full();
      }
#else
      {
        error_handler::error(forward_list_full());
      }
#endif
    }

    //*************************************************************************
    /// Pushes a value to the front of the forward_list.
    //*************************************************************************
    void push_front(parameter_t value)
    {
      if (!full())
      {
        Data_Node& data_node = node_pool[next_free];
        data_node.value = value;

        insert_node_after(get_head(), data_node);
      }
      else
#ifdef ETL_THROW_EXCEPTIONS
      {
        throw forward_list_full();
      }
#else
      {
        error_handler::error(forward_list_full());
      }
#endif
    }

    //*************************************************************************
    /// Removes a value from the front of the forward_list.
    //*************************************************************************
    void pop_front()
    {
      if (!empty())
      {
        remove_node_after(start_node);
      }
    }

    //*************************************************************************
    /// Resizes the forward_list.
    //*************************************************************************
    void resize(size_t n)
    {
        resize(n, T());
    }

    //*************************************************************************
    /// Resizes the forward_list.
    /// If ETL_THROW_EXCEPTIONS is defined, will throw an etl::forward_list_full
    /// if <b>n</b> is larger than the maximum size.
    //*************************************************************************
    void resize(size_t n, T value)
    {
      if (n <= MAX_SIZE)
      {
        size_t i = 0;
        iterator i_node = begin();

        // Find where we're currently at.
        while ((i < n) && (i_node != end()))
        {
          ++i;
          ++i_node;
        }

        if (i_node != end())
        {
          // Reduce.
          erase_after(i_node, end());
        }
        else if (i_node == end())
        {
          while (i < n)
          {
            i_node = insert_after(i_node, value);
            ++i;
          }
        }
      }
      else
#ifdef ETL_THROW_EXCEPTIONS
      {
        throw forward_list_full();
      }
#else
      {
        error_handler::error(forward_list_full());
      }
#endif
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

      join(*p_current, end_node);

      while (p_next != &end_node)
      {
        p_last = p_current;
        p_current = p_next;
        p_next = p_current->next;

        p_current->next = static_cast<Data_Node*>(p_last);
      }

      join(start_node, *p_current);
    }

    //*************************************************************************
    /// Inserts a value to the forward_list after the specified position.
    //*************************************************************************
    iterator insert_after(iterator position, parameter_t value)
    {
      if (!full())
      {
        Data_Node& data_node = node_pool[next_free];
        data_node.value = value;

        insert_node_after(*position.p_node, data_node);

        return iterator(data_node);
      }
      else
#ifdef ETL_THROW_EXCEPTIONS    
      {
        throw forward_list_full();
        return end();
      }
#else
      {
        error_handler::error(forward_list_full());
        return end();
      }
#endif
    }

    //*************************************************************************
    /// Inserts 'n' copies of a value to the forward_list after the specified position.
    //*************************************************************************
    void insert_after(iterator position, size_t n, parameter_t value)
    {
      if (!full())
      {
        for (size_t i = 0; !full() && (i < n); ++i)
        {
          // Set up the next free node.
          Data_Node& data_node = node_pool[next_free];
          data_node.value = value;

          insert_node(*position.p_node, data_node);
        }
      }
      else
#ifdef ETL_THROW_EXCEPTIONS
      {
        throw forward_list_full();
      }
#else
      {
        error_handler::error(forward_list_full());
      }
#endif
    }

    //*************************************************************************
    /// Inserts a range of values to the forward_list after the specified position.
    //*************************************************************************
    template <typename TIterator>
    void insert_after(iterator position, TIterator first, TIterator last)
    {
      while (first != last)
      {
        if (!full())
        {
          // Set up the next free node.
          Data_Node& data_node = node_pool[next_free];
          data_node.value = *first;

          insert_node_after(*position.p_node, data_node);
          ++first;
          ++position;
        }
        else
#ifdef ETL_THROW_EXCEPTIONS
        {
          throw forward_list_full();
        }
#else
        {
          error_handler::error(forward_list_full());
        }
#endif
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
      Node* p_first = first.p_node;
      Node* p_last  = last.p_node;
      Node* p_next  = p_first->next;

      // Join the ends.
      join(*p_first, *p_last);

      p_first = p_next;

      // Erase the ones in between.
      while (p_first != p_last)
      {
        // Update the position of the earliest free node in the pool.
        size_t new_free = std::distance(&node_pool[0], static_cast<Data_Node*>(p_first));
        next_free       = std::min(next_free, new_free);

        // One less.
        --count;

        p_next = p_first->next;  // Remember the next node.
        p_first->mark_as_free(); // Free the current node.
        p_first = p_next;        // Move to the next node.
      }

      return ++last;
    }

    //*************************************************************************
    /// Removes all but the first element from every consecutive group of equal
    /// elements in the container.
    //*************************************************************************
    void unique()
    {
        unique(std::equal_to<T>());
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

      Node* last    = &get_head();
      Node* current = last->next;

      while (current != &end_node)
      {
        // Is this value the same as the last?
        if (isEqual(data_cast(current)->value, data_cast(last)->value))
        {
          remove_node_after(*last);
        }
        else
        {
          // Move on one.
          last = current;
        }

        current = last->next;
      }
    }

    //*************************************************************************
    /// Sort using in-place merge sort algorithm.
    /// Uses 'less-than operator as the predicate.
    //*************************************************************************
    void sort()
    {
      sort(std::less<T>());
    }

    //*************************************************************************
    /// Sort using in-place merge sort algorithm.
    /// Uses a supplied predicate function or functor.
    /// This is not my algorithm. I got it off the web somewhere.
    //*************************************************************************
    template <typename TCompare>
    void sort(TCompare compare)
    {
      iterator p_left;
      iterator p_right;
      iterator p_node;
      iterator p_head;
      iterator p_tail;
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
        p_left = begin();
        p_head = before_begin();
        p_tail = before_begin();

        number_of_merges = 0;  // Count the number of merges we do in this pass.

        while (p_left != end())
        {
          ++number_of_merges;  // There exists a merge to be done.
          p_right = p_left;
          left_size = 0;

          // Step 'list_size' places along from left
          for (int i = 0; i < list_size; ++i)
          {
            ++left_size;

            ++p_right;

            if (p_right == end())
            {
              break;
            }
          }

          // If right hasn't fallen off end, we have two lists to merge.
          right_size = list_size;

          // Now we have two lists. Merge them.
          while (left_size > 0 || (right_size > 0 && p_right != end()))
          {
            // Decide whether the next node of merge comes from left or right.
            if (left_size == 0)
            {
		          // Left is empty. The node must come from right.
		          p_node = p_right;
              ++p_right;
              --right_size;
		        }
            else if (right_size == 0 || p_right == end())
            {
		          // Right is empty. The node must come from left.
		          p_node = p_left;
              ++p_left;
              --left_size;
		        }
            else if (compare(*p_left, *p_right))
            {
		          // First node of left is lower or same. The node must come from left.
		          p_node = p_left;
              ++p_left;
              --left_size;
		        }
            else
            {
		          // First node of right is lower. The node must come from right.
		          p_node  = p_right;
              ++p_right;
              --right_size;
		        }

            // Add the next node to the merged head.
            if (p_head == before_begin())
            {
                join(*p_head.p_node, *p_node.p_node);
                p_head = p_node;
                p_tail = p_node;
            }
            else
            {
                join(*p_tail.p_node, *p_node.p_node);
                p_tail = p_node;
            }

            join(*p_tail.p_node, end_node);
          }

          // Now left has stepped `list_size' places along, and right has too.
          p_left = p_right;
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
    void remove(parameter_t value)
    {
      iterator i_item = begin();
      iterator i_last_item = before_begin();

      while (i_item != end())
      {
        if (*i_item == value)
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

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    iforward_list(Data_Node* node_pool, size_t max_size_)
      : forward_list_base(max_size_),
        node_pool(node_pool)
    {
      initialise();
    }

    Node start_node; ///< The node that acts as the forward_list start.
    Node end_node;   ///< The node that acts as the forward_list end.

  private:

    Data_Node* node_pool; ///< The pool of data nodes used in the forward_list.

    //*************************************************************************
    /// Downcast a Node* to a Data_Node*
    //*************************************************************************
    static Data_Node* data_cast(Node* p_node)
    {
      return static_cast<Data_Node*>(p_node);
    }

    //*************************************************************************
    /// Downcast a Node& to a Data_Node&
    //*************************************************************************
    static Data_Node& data_cast(Node& node)
    {
      return static_cast<Data_Node&>(node);
    }

    //*************************************************************************
    /// Downcast a const Node* to a const Data_Node*
    //*************************************************************************
    static const Data_Node* data_cast(const Node* p_node)
    {
      return static_cast<const Data_Node*>(p_node);
    }

    //*************************************************************************
    /// Downcast a const Node& to a const Data_Node&
    //*************************************************************************
    static const Data_Node& data_cast(const Node& node)
    {
      return static_cast<const Data_Node&>(node);
    }

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(Node& left, Node& right)
    {
      left.next = &static_cast<Data_Node&>(right);
    }

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(Data_Node& left, Data_Node& right)
    {
      left.next = &right;
    }

    //*************************************************************************
    /// Is the forward_list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return (size() < 2);
    }

    //*************************************************************************
    /// Finds the next free node.
    //*************************************************************************
    void find_next_free()
    {
      while (next_free != MAX_SIZE)
      {
        if (node_pool[next_free].is_free())
        {
          return;
        }
        else
        {
          ++next_free;
        }
      }
    }

    //*************************************************************************
    /// Insert a node.
    //*************************************************************************
    void insert_node_after(Node& position, Node& node)
    {
      // Connect to the forward_list.
      join(node, *position.next);
      join(position, node);

      // One more.
      ++count;

      // Update the position of the next free node in the pool.
      find_next_free();
    }

    //*************************************************************************
    /// Remove a node.
    //*************************************************************************
    void remove_node_after(Node& node)
    {
      // The node to erase.
      Node* p_node = node.next;

      // One less.
      --count;

      // Disconnect the node from the forward_list.
      join(node, *p_node->next);
      p_node->mark_as_free();

      // Update the position of the next free node in the pool.
      size_t new_free = std::distance(&node_pool[0], static_cast<Data_Node*>(p_node));
      next_free = std::min(next_free, new_free);
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    Node& get_head()
    {
      return static_cast<Data_Node&>(*start_node.next);
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const Node& get_head() const
    {
      return *start_node.next;
    }

    //*************************************************************************
    /// Initialise the forward_list.
    //*************************************************************************
    void initialise()
    {
      // Reset the node pool.
      for (size_t i = 0; i < max_size(); ++i)
      {
        node_pool[i].mark_as_free();
      }

      next_free = 0;
      count     = 0;
      join(start_node, end_node);
      join(end_node,   start_node);
    }
  };
}

//*************************************************************************
/// Equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator ==(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return (lhs.size() == rhs.size()) &&
    std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//*************************************************************************
/// Not equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator !=(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return !(lhs == rhs);
}

//*************************************************************************
/// Less than operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexigraphically less than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end());
}

//*************************************************************************
/// Greater than operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexigraphically greater than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end(),
                                      std::greater<T>());
}

//*************************************************************************
/// Less than or equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexigraphically less than or equal
/// to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <=(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return !operator >(lhs, rhs);
}

//*************************************************************************
/// Greater than or equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexigraphically greater than or
/// equal to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >=(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return !operator <(lhs, rhs);
}

#if WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif
