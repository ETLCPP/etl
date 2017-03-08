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

#ifndef __ETL_IFORWARD_LIST__
#define __ETL_IFORWARD_LIST__
#define __ETL_IN_IFORWARD_LIST_H__

#include "platform.h"

#ifdef ETL_COMPILER_MICROSOFT
#undef min
#endif

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>

#include "pool.h"
#include "nullptr.h"
#include "private/forward_list_base.h"
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
    /// The data node element in the forward_list.
    //*************************************************************************
    struct data_node_t : public node_t
    {
      explicit data_node_t(parameter_t value)
        : value(value)
      {}

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

      node_t* p_node;
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

      const_iterator(node_t& node)
        : p_node(&node)
      {
      }

      const_iterator(const node_t& node)
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

      const node_t* p_node;
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
      return iterator(static_cast<data_node_t&>(start_node));
    }

    //*************************************************************************
    /// Gets before the beginning of the forward_list.
    //*************************************************************************
    const_iterator before_begin() const
    {
      return const_iterator(static_cast<const data_node_t&>(start_node));
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
      return iterator();
    }

    //*************************************************************************
    /// Gets the end of the forward_list.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator();
    }

    //*************************************************************************
    /// Gets the end of the forward_list.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator();
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
    /// If asserts or exceptions are enabled throws etl::forward_list_full if the forward_list does not have enough free space.
    /// If ETL_THROW_EXCEPTIONS & _DEBUG are defined throws forward_list_iterator if the iterators are reversed.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type count = std::distance(first, last);
      ETL_ASSERT(count >= 0, ETL_ERROR(forward_list_iterator));
#endif

      initialise();

      node_t* p_last_node = &start_node;

      // Add all of the elements.
      while (first != last)
      {
        ETL_ASSERT(!full(), ETL_ERROR(forward_list_iterator));

        data_node_t& data_node = allocate_data_node(*first++);
        join(p_last_node, &data_node);
        data_node.next = nullptr;
        p_last_node = &data_node;
      }
    }

    //*************************************************************************
    /// Assigns 'n' copies of a value to the forward_list.
    //*************************************************************************
    void assign(size_t n, parameter_t value)
    {
      ETL_ASSERT(n <= MAX_SIZE, ETL_ERROR(forward_list_full));

      initialise();

      node_t* p_last_node = &start_node;

      // Add all of the elements.
      while (size() < n)
      {
        data_node_t& data_node = allocate_data_node(value);
        join(p_last_node, &data_node);
        data_node.next = nullptr;
        p_last_node = &data_node;
      }
    }

    //*************************************************************************
    /// Adds a node to the front of the forward_list so a new value can be assigned to front().
    //*************************************************************************
    void push_front()
    {
      push_front(T());
    }

    //*************************************************************************
    /// Pushes a value to the front of the forward_list.
    //*************************************************************************
    void push_front(parameter_t value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));
#endif

      data_node_t& data_node = allocate_data_node(value);
      insert_node_after(start_node, data_node);
    }

    //*************************************************************************
    /// Removes a value from the front of the forward_list.
    //*************************************************************************
    void pop_front()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(forward_list_empty));
#endif
      remove_node_after(start_node);
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
    /// If asserts or exceptions are enabled, will throw an etl::forward_list_full
    /// if <b>n</b> is larger than the maximum size.
    //*************************************************************************
    void resize(size_t n, T value)
    {
      ETL_ASSERT(n <= MAX_SIZE, ETL_ERROR(forward_list_full));

      size_t i = 0;
      iterator i_node = begin();
      iterator i_last_node;

      // Find where we're currently at.
      while ((i < n) && (i_node != end()))
      {
        ++i;
        i_last_node = i_node;
        ++i_node;
      }

      if (i_node != end())
      {
        // Reduce.
        erase_after(i_last_node, end());
      }
      else if (i_node == end())
      {
        // Increase.
        while (i < n)
        {
          i_last_node = insert_after(i_last_node, value);
          ++i;
        }
      }
    }

    //*************************************************************************
    /// Inserts a value to the forward_list after the specified position.
    //*************************************************************************
    iterator insert_after(iterator position, parameter_t value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      data_node_t& data_node = allocate_data_node(value);
      insert_node_after(*position.p_node, data_node);

      return iterator(data_node);
    }

    //*************************************************************************
    /// Inserts 'n' copies of a value to the forward_list after the specified position.
    //*************************************************************************
    void insert_after(iterator position, size_t n, parameter_t value)
    {
      ETL_ASSERT(!full(), ETL_ERROR(forward_list_full));

      for (size_t i = 0; !full() && (i < n); ++i)
      {
        // Set up the next free node.
        data_node_t& data_node = allocate_data_node(value);
        insert_node_after(*position.p_node, data_node);
      }
    }

    //*************************************************************************
    /// Inserts a range of values to the forward_list after the specified position.
    //*************************************************************************
    template <typename TIterator>
    void insert_after(iterator position, TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type count = std::distance(first, last);
      ETL_ASSERT((count + size()) <= MAX_SIZE, ETL_ERROR(forward_list_full));
#endif

      while (first != last)
      {
        // Set up the next free node.
        data_node_t& data_node = allocate_data_node(*first++);
        insert_node_after(*position.p_node, data_node);
        ++position;
      }
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    iterator erase_after(iterator position)
    {
      iterator next(position);
      if (next != end())
      {
        ++next;
        if (next != end())
        {
          ++next;
          remove_node_after(*position.p_node);
        }
      }

      return next;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase_after(iterator first, iterator last)
    {
      if (first != end() && (first != last))
      {
        node_t* p_first = first.p_node;
        node_t* p_last = last.p_node;
        node_t* p_next = p_first->next;

        // Join the ends.
        join(p_first, p_last);

        p_first = p_next;

        // Erase the ones in between.
        while (p_first != p_last)
        {
          p_next = p_first->next;                               // Remember the next node.
          destroy_data_node(static_cast<data_node_t&>(*p_first)); // Destroy the pool object.
          p_first = p_next;                                     // Move to the next node.
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
      else
      {
        return end();
      }
    }

    //*************************************************************************
    /// Erases the value at the specified position.
    //*************************************************************************
    void move_after(const_iterator from_before, const_iterator to_before)
    {
      if (from_before == to_before) // Can't move to after yourself!
      {
        return;
      }

      node_t* p_from_before = const_cast<node_t*>(from_before.p_node); // We're not changing the value, just it's position.
      node_t* p_to_before   = const_cast<node_t*>(to_before.p_node);   // We're not changing the value, just it's position.

      node_t* p_from = p_from_before->next;

      // Disconnect from the list.
      join(p_from_before, p_from->next);

      // Attach it to the new position.
      join(p_from,        p_to_before->next);
      join(p_to_before,   p_from);
    }

    //*************************************************************************
    /// Moves a range from one position to another within the list.
    /// Moves a range at position 'first_before'/'last' to the position before 'to_before'.
    //*************************************************************************
    void move_after(const_iterator first_before, const_iterator last, const_iterator to_before)
    {
      if ((first_before == to_before) || (last == to_before))
      {
        return; // Can't more to before yourself!
      }

#if defined(ETL_DEBUG)
      // Check that we are not doing an illegal move!
      for (const_iterator item = first_before; item != last; ++item)
      {
        ETL_ASSERT(item != to_before, ETL_ERROR(forward_list_iterator));
      }
#endif

      node_t* p_first_before = const_cast<node_t*>(first_before.p_node); // We're not changing the value, just it's position.
      node_t* p_last         = const_cast<node_t*>(last.p_node);         // We're not changing the value, just it's position.
      node_t* p_to_before    = const_cast<node_t*>(to_before.p_node);    // We're not changing the value, just it's position.
      node_t* p_first        = p_first_before->next;
      node_t* p_final        = p_first_before;

      // Find the last node that will be moved.
      while (p_final->next != p_last)
      {
        p_final = p_final->next;
      }

      // Disconnect from the list.
      join(p_first_before, p_final->next);

      // Attach it to the new position.
      join(p_final, p_to_before->next);
      join(p_to_before, p_first);
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

      node_t* last    = &get_head();
      node_t* current = last->next;

      while (current != nullptr)
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
                join(p_head.p_node, p_node.p_node);
                p_head = p_node;
                p_tail = p_node;
            }
            else
            {
                join(p_tail.p_node, p_node.p_node);
                p_tail = p_node;
            }

            p_tail.p_node->next = nullptr;
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

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    iforward_list& operator = (const iforward_list& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    iforward_list(etl::ipool& node_pool, size_t max_size_)
      : forward_list_base(node_pool, max_size_)
    {
    }

    //*************************************************************************
    /// Initialise the forward_list.
    //*************************************************************************
    void initialise()
    {
      if (!empty())
      {
        node_t* p_first = start_node.next;
        node_t* p_next;

        // Erase the ones in between.
        while (p_first != nullptr)
        {
          p_next = p_first->next;                                 // Remember the next node.
          destroy_data_node(static_cast<data_node_t&>(*p_first)); // Destroy the pool object.
          p_first = p_next;                                       // Move to the next node.
        }
      }

      start_node.next = nullptr;
    }

  private:

    //*************************************************************************
    /// Downcast a node_t* to a data_node_t*
    //*************************************************************************
    static data_node_t* data_cast(node_t* p_node)
    {
      return static_cast<data_node_t*>(p_node);
    }

    //*************************************************************************
    /// Downcast a node_t& to a data_node_t&
    //*************************************************************************
    static data_node_t& data_cast(node_t& node)
    {
      return static_cast<data_node_t&>(node);
    }

    //*************************************************************************
    /// Downcast a const node_t* to a const data_node_t*
    //*************************************************************************
    static const data_node_t* data_cast(const node_t* p_node)
    {
      return static_cast<const data_node_t*>(p_node);
    }

    //*************************************************************************
    /// Downcast a const node_t& to a const data_node_t&
    //*************************************************************************
    static const data_node_t& data_cast(const node_t& node)
    {
      return static_cast<const data_node_t&>(node);
    }

    //*************************************************************************
    /// Remove a node.
    //*************************************************************************
    void remove_node_after(node_t& node)
    {
      // The node to erase.
      node_t* p_node = node.next;

      if (p_node != nullptr)
      {
        // Disconnect the node from the forward_list.
        join(&node, p_node->next);

        // Destroy the pool object.
        destroy_data_node(static_cast<data_node_t&>(*p_node));
      }
    }

    //*************************************************************************
    /// Allocate a data_node_t.
    //*************************************************************************
    data_node_t& allocate_data_node(parameter_t value)
    {
      data_node_t* p_node = p_node_pool->allocate<data_node_t>();
      ::new (&(p_node->value)) T(value);
      ++construct_count;

      return *p_node;
    }

    //*************************************************************************
    /// Destroy a data_node_t.
    //*************************************************************************
    void destroy_data_node(data_node_t& node)
    {
      node.value.~T();
      p_node_pool->release(&node);
      --construct_count;
    }

    // Disable copy construction.
    iforward_list(const iforward_list&);
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
///\return <b>true</b> if the first forward_list is lexicographically less than the
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
///\return <b>true</b> if the first forward_list is lexicographically greater than the
/// second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return (rhs < lhs);
}

//*************************************************************************
/// Less than or equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexicographically less than or equal
/// to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator <=(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return !(lhs > rhs);
}

//*************************************************************************
/// Greater than or equal operator.
///\param lhs Reference to the first forward_list.
///\param rhs Reference to the second forward_list.
///\return <b>true</b> if the first forward_list is lexicographically greater than or
/// equal to the second, otherwise <b>false</b>.
//*************************************************************************
template <typename T>
bool operator >=(const etl::iforward_list<T>& lhs, const etl::iforward_list<T>& rhs)
{
  return !(lhs < rhs);
}

#ifdef ETL_COMPILER_MICROSOFT
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#undef __ETL_IN_IFORWARD_LIST_H__

#endif
