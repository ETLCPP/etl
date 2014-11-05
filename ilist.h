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
#include <cstddef>

#include "nullptr.h"
#include "list_base.h"

namespace etl
{
  //***************************************************************************
  /// A templated base for all etl::list types.
  ///\ingroup list
  //***************************************************************************
  template <typename T>
  class ilist : public list_base
  {
  protected:

    //*************************************************************************
    /// The node element in the list.
    //*************************************************************************
    struct Node
    {
      Node()
        : previous(nullptr),
          next(nullptr)
      {
      }

      void mark_as_free()
      {
        previous = nullptr;
        next     = nullptr;
      }

      bool is_free() const
      {
        return next == nullptr;
      }

      void reverse()
      {
        std::swap(previous, next);
      }

      T     value;
      Node* previous;
      Node* next;
    };

  private:

    Node   terminal_node;  ///< The node that acts as the list start and end.
    Node*  node_pool;      ///< The pool of nodes used in the list.

  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T>
    {
    public:

      friend class ilist;

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

      iterator& operator --()
      {
        p_node = p_node->previous;
        return *this;
      }

      iterator operator --(int)
      {
        iterator temp(*this);
        p_node = p_node->previous;
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      reference operator *()
      {
        return p_node->value;
      }

      const_reference operator *() const
      {
        return p_node->value;
      }

      pointer operator &()
      {
        return &(p_node->value);
      }

      const_pointer operator &() const
      {
        return &(p_node->value);
      }

      pointer operator ->()
      {
        return &(p_node->value);
      }

      const_pointer operator ->() const
      {
        return &(p_node->value);
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
    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, const T>
    {
    public:

      friend class ilist;

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

      const_iterator(const typename ilist::iterator& other)
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

      const_iterator& operator --()
      {
        p_node = p_node->previous;
        return *this;
      }

      const_iterator operator --(int)
      {
        const_iterator temp(*this);
        p_node = p_node->previous;
        return temp;
      }

      const_iterator operator =(const const_iterator& other)
      {
        p_node = other.p_node;
        return *this;
      }

      const_reference operator *() const
      {
        return p_node->value;
      }

      const_pointer operator &() const
      {
        return &(p_node->value);
      }

      Node* operator ->()
      {
        return p_node;
      }

      const Node* operator ->() const
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

    private:

      const Node* p_node;
    };

		typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Gets the beginning of the list.
    //*************************************************************************
    iterator begin()
    {
      return iterator(get_head());
    }

    //*************************************************************************
    /// Gets the beginning of the list.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the beginning of the list.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the reverse beginning of the list.
    //*************************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the end of the list.
    //*************************************************************************
    iterator end()
    {
        return iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the end of the list.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the end of the list.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator(terminal_node);
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(get_head());
    }

    //*************************************************************************
    /// Gets the reverse end of the list.
    //*************************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(get_head());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ilist& operator = (const ilist& rhs)
    {
      assign(rhs.cbegin(), rhs.cend());

      return *this;
    }

    //*************************************************************************
    /// Clears the list.
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
      return get_head().value;
    }

    //*************************************************************************
    /// Gets a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return get_head().value;
    }

    //*************************************************************************
    /// Gets a reference to the last element.
    //*************************************************************************
    reference back()
    {
      return get_tail().value;
    }

    //*************************************************************************
    /// Gets a reference to the last element.
    //*************************************************************************
    const_reference back() const
    {
      return get_tail().value;
    }

    //*************************************************************************
    /// Assigns a range of values to the list.
		/// If ETL_USE_EXCEPTIONS is defined throws etl::list_full if the list does not have enough free space.
    /// If ETL_USE_EXCEPTIONS & _DEBUG are defined throws list_iterator if the iterators are reversed.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#ifdef _DEBUG
      if (std::distance(first, last) < 0)
      {
#ifdef ETL_USE_EXCEPTIONS
        throw list_iterator();
#endif
      }
      else
#endif
      {
        // Reset the links.
        join(terminal_node, terminal_node);

        size_t i = 0;

        // Add all of the elements.
        while (first != last)
        {
#ifdef ETL_USE_EXCEPTIONS
          if (i == MAX_SIZE)
          {
            throw list_full();
          }
#endif
          Node& node = node_pool[i];
          node.value = *first++;
          join(*terminal_node.previous, node);
          join(node, terminal_node);
          ++i;
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
    }

    //*************************************************************************
    /// Assigns 'n' copies of a value to the list.
    //*************************************************************************
    void assign(size_t n, const_reference value)
    {
      if (n <= MAX_SIZE)
      {
        // Reset the links.
        join(terminal_node, terminal_node);

        size_t i = 0;

        // Add all of the elements.
        while (i < n)
        {
          Node& node = node_pool[i];
          node.value = value;
          join(*terminal_node.previous, node);
          join(node, terminal_node);
          ++i;
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
#ifdef ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
      }
#endif
    }

    //*************************************************************************
    /// Adds a node to the front of the list so a new value can be assigned to front().
    //*************************************************************************
    void push_front()
    {
      if (!full())
      {
        Node& node = node_pool[next_free];

        insert_node(get_head(), node);
      }
#ifdef ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
      }
#endif
    }

    //*************************************************************************
    /// Pushes a value to the front of the list.
    //*************************************************************************
    void push_front(const_reference value)
    {
      if (!full())
      {
        Node& node  = node_pool[next_free];
        node.value  = value;

        insert_node(get_head(), node);
      }
#ifdef ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
      }
#endif
    }

    //*************************************************************************
    /// Adds a node to the back of the list so a new value can be assigned to back().
    //*************************************************************************
    void push_back()
    {
      if (!full())
      {
        Node& node = node_pool[next_free];

        insert_node(terminal_node, node);
      }
#if ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
      }
#endif
    }

    //*************************************************************************
    /// Pushes a value to the back of the list..
    //*************************************************************************
    void push_back(const_reference value)
    {
      if (!full())
      {
        Node& node  = node_pool[next_free];
        node.value  = value;

        insert_node(terminal_node, node);
      }
#if ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
      }
#endif
    }

    //*************************************************************************
    /// Removes a value from the front of the list.
    //*************************************************************************
    void pop_front()
    {
      if (!empty())
      {
        remove_node(get_head());
      }
    }

    //*************************************************************************
    /// Removes a value from the back of the list.
    //*************************************************************************
    void pop_back()
    {
      if (!empty())
      {
        remove_node(get_tail());
      }
    }

    //*************************************************************************
    /// Resizes the list.
    //*************************************************************************
    void resize(size_t n)
    {
        resize(n, T());
    }

    //*************************************************************************
    /// Resizes the list.
    //*************************************************************************
    void resize(size_t n, T value)
    {
      if (n <= MAX_SIZE)
      {
        // Reduce if necessary.
        while (size() > n)
        {
          pop_back();
        }

        // Increase if necessary.
        while (size() < n)
        {
          push_back(value);
        }
      }
#if ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
      }
#endif
    }

    //*************************************************************************
    /// Reverses the list.
    //*************************************************************************
    void reverse()
    {
      if (is_trivial_list())
      {
        return;
      }

      Node* p_current = &get_head();

      for (size_t i = 0; i < size(); ++i)
      {
        p_current->reverse();
        p_current = p_current->previous;
      }

      terminal_node.reverse();
    }

    //*************************************************************************
    /// Inserts a value to the list at the specified position.
    //*************************************************************************
    iterator insert(iterator position, const value_type& value)
    {
      if (!full())
      {
        Node& node = node_pool[next_free];
        node.value = value;

        insert_node(*position.p_node, node);

        return iterator(node);
      }
#if ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
        return end();
      }
#else
      {
        return end();
      }
#endif
    }

    //*************************************************************************
    /// Inserts 'n' copies of a value to the list at the specified position.
    //*************************************************************************
    void insert(iterator position, size_t n, const value_type& value)
    {
      if (!full())
      {
        for (size_t i = 0; !full() && (i < n); ++i)
        {
          // Set up the next free node.
          Node& node  = node_pool[next_free];
          node.value  = value;

          insert_node(*position.p_node, node);
        }
      }
#if ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
      }
#endif
    }

    //*************************************************************************
    /// Inserts a range of values to the list at the specified position.
    //*************************************************************************
    template <typename TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      if ((size() + std::distance(first, last)) <= MAX_SIZE)
      {
        while (first != last)
        {
          // Set up the next free node.
          Node& node = node_pool[next_free];
          node.value = *first;

          insert_node(*position.p_node, node);
          ++first;
        }
      }
#if ETL_USE_EXCEPTIONS
      else
      {
        throw list_full();
      }
#endif
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase(iterator position)
    {
      iterator next(position);
      ++next;

      remove_node(*position.p_node);

      return next;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase(iterator first, iterator last)
    {
      Node* p_first = first.p_node;
      Node* p_last  = last.p_node;
      Node* p_next;

      // Join the ends.
      join(*(p_first->previous), *p_last);

      // Erase the ones in between.
      while (p_first != p_last)
      {
        // Update the position of the earliest free node in the pool.
        size_t new_free = std::distance(&node_pool[0], p_first);
        next_free       = std::min(next_free, new_free);

        // One less.
        --count;

        p_next = p_first->next;  // Remember the next node.
        p_first->mark_as_free(); // Free the current node.
        p_first = p_next;        // Move to the next node.
      }

      return last;
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
    /// Removes all but the first element from every consecutive group of equal
    /// elements in the container.
    //*************************************************************************
    template <typename TIsEqual>
    void unique(TIsEqual isEqual)
    {
      Node* current = get_head().next;

      while (current != &terminal_node)
      {
        Node* next = current->next;
        if (isEqual(current->previous->value, current->value))
        {
          remove_node(*current);
        }

        current = next;
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
      Node* p_left;
      Node* p_right;
      Node* p_node;
      Node* p_head;
      Node* p_tail;
      int   list_size = 1;
      int   number_of_merges;
      int   left_size;
      int   right_size;

      if (is_trivial_list())
      {
	      return;
      }

      while (true)
      {
        p_left = &get_head();
        p_head = &terminal_node;
        p_tail = &terminal_node;

        number_of_merges = 0;  // Count the number of merges we do in this pass.

        while (p_left != &terminal_node)
        {
          ++number_of_merges;  // There exists a merge to be done.
          p_right = p_left;
          left_size = 0;

          // Step 'list_size' places along from left
          for (int i = 0; i < list_size; ++i)
          {
            ++left_size;

            p_right = p_right->next;

            if (p_right == &terminal_node)
            {
              break;
            }
          }

          // If right hasn't fallen off end, we have two lists to merge.
          right_size = list_size;

          // Now we have two lists. Merge them.
          while (left_size > 0 || (right_size > 0 && p_right != &terminal_node))
          {
            // Decide whether the next node of merge comes from left or right.
            if (left_size == 0)
            {
		          // Left is empty. The node must come from right.
		          p_node = p_right;
              p_right = p_right->next;
              --right_size;
		        }
            else if (right_size == 0 || p_right == &terminal_node)
            {
		          // Right is empty. The node must come from left.
		          p_node = p_left;
              p_left = p_left->next;
              --left_size;
		        }
            else if (compare(p_left->value, p_right->value))
            {
		          // First node of left is lower or same. The node must come from left.
		          p_node = p_left;
              p_left = p_left->next;
              --left_size;
		        }
            else
            {
		          // First node of right is lower. The node must come from right.
		          p_node  = p_right;
              p_right = p_right->next;
              --right_size;
		        }

            // Add the next node to the merged head.
            if (p_head == &terminal_node)
            {
                join(*p_head, *p_node);
                p_head = p_node;
                p_tail = p_node;
            }
            else
            {
                join(*p_tail, *p_node);
                p_tail = p_node;
            }

            join(*p_tail, terminal_node);
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
    void remove(const value_type& value)
    {
      iterator iValue = begin();

      while (iValue != end())
      {
        if (value == *iValue)
        {
          iValue = erase(iValue);
        }
        else
        {
          ++iValue;
        }
      }
    }

    //*************************************************************************
    /// Removes according to a predicate.
    //*************************************************************************
    template <typename TPredicate>
    void remove_if(TPredicate predicate)
    {
      iterator iValue = begin();

      while (iValue != end())
      {
        if (predicate(*iValue))
        {
          iValue = erase(iValue);
        }
        else
        {
          ++iValue;
        }
      }
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    ilist(Node* node_pool, size_t max_size_)
      : list_base(max_size_),
        node_pool(node_pool)
    {
      initialise();
    }

  private:

    //*************************************************************************
    /// Join two nodes.
    //*************************************************************************
    void join(Node& left, Node& right)
    {
      left.next      = &right;
      right.previous = &left;
    }

    //*************************************************************************
    /// Is the list a trivial length?
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
    void insert_node(Node& position, Node& node)
    {
      // Connect to the list.
      join(*position.previous, node);
      join(node,               position);

      // One more.
      ++count;

      // Update the position of the next free node in the pool.
      find_next_free();
    }

    //*************************************************************************
    /// Remove a node.
    //*************************************************************************
    void remove_node(Node& node)
    {
      // Update the position of the next free node in the pool.
      size_t new_free = std::distance(&node_pool[0], &node);
      next_free       = std::min(next_free, new_free);

      // One less.
      --count;

      // Disconnect the node from the list.
      join(*node.previous, *node.next);
      node.mark_as_free();
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    Node& get_head()
    {
      return *terminal_node.next;
    }

    //*************************************************************************
    /// Get the head node.
    //*************************************************************************
    const Node& get_head() const
    {
      return *terminal_node.next;
    }

    //*************************************************************************
    /// Get the tail node.
    //*************************************************************************
    Node& get_tail()
    {
      return *terminal_node.previous;
    }

    //*************************************************************************
    /// Get the tail node.
    //*************************************************************************
    const Node& get_tail() const
    {
      return *terminal_node.previous;
    }

    //*************************************************************************
    /// initialise the list.
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
      join(terminal_node, terminal_node);
    }
  };
}

//*************************************************************************
/// Equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator ==(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return (lhs.size() == rhs.size()) &&
    std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//*************************************************************************
/// Not equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator !=(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return !(lhs == rhs);
}

//*************************************************************************
/// Less than operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexigraphically less than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end());
}

//*************************************************************************
/// Greater than operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexigraphically greater than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end(),
                                      std::greater<T>());
}

//*************************************************************************
/// Less than or equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexigraphically less than or equal
/// to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <=(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return !operator >(lhs, rhs);
}

//*************************************************************************
/// Greater than or equal operator.
///\param lhs Reference to the first list.
///\param rhs Reference to the second list.
///\return <b>true</b> if the first list is lexigraphically greater than or
/// equal to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >=(const etl::ilist<T>& lhs, const etl::ilist<T>& rhs)
{
  return !operator <(lhs, rhs);
}

#if WIN32
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif
