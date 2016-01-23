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
#include "exception.h"
#include "error_handler.h"
#include "intrusive_links.h"

#undef ETL_FILE
#define ETL_FILE "20"

namespace etl
{
  //***************************************************************************
  /// Exception for the intrusive_forward_list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  class intrusive_forward_list_exception : public exception
  {
  public:

    intrusive_forward_list_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Empty exception for the intrusive_forward_list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  class intrusive_forward_list_empty : public intrusive_forward_list_exception
  {
  public:

    intrusive_forward_list_empty(string_type file_name, numeric_type line_number)
      : intrusive_forward_list_exception(ETL_ERROR_TEXT("intrusive_forward_list:empty", ETL_FILE"A"), file_name, line_number)
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

    intrusive_forward_list_iterator_exception(string_type file_name, numeric_type line_number)
      : intrusive_forward_list_exception(ETL_ERROR_TEXT("intrusive_forward_list:iterator", ETL_FILE"B"), file_name, line_number)
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

    intrusive_forward_list_index_exception(string_type file_name, numeric_type line_number)
      : intrusive_forward_list_exception(ETL_ERROR_TEXT("intrusive_forward_list:bounds", ETL_FILE"C"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// An intrusive forward list.
  ///\ingroup intrusive_forward_list
  ///\note TLink must be a base of TValue.
  //***************************************************************************
  template <typename TValue, typename TLink = etl::forward_link<0> >
  class intrusive_forward_list
  {
  public:

    // Node typedef.
    typedef TLink             link_type;

    // STL style typedefs.
    typedef TValue            value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef size_t            size_type;

  public:

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public std::iterator<std::forward_iterator_tag, value_type>
    {
    public:

      friend class intrusive_forward_list;

      iterator()
        : p_value(nullptr)
      {
      }

      iterator(value_type& value)
        : p_value(&value)
      {
      }

      iterator(const iterator& other)
        : p_value(other.p_value)
      {
      }

      iterator& operator ++()
      {
        // Read the appropriate 'etl_next'.
        p_value = static_cast<value_type*>(p_value->link_type::etl_next);
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        // Read the appropriate 'etl_next'.
        p_value = static_cast<value_type*>(p_value->link_type::etl_next);
        return temp;
      }

      iterator operator =(const iterator& other)
      {
        p_value = other.p_value;
        return *this;
      }

      reference operator *()
      {
        return *p_value;
      }

      const_reference operator *() const
      {
        return *p_value;
      }

      pointer operator &()
      {
        return p_value;
      }

      const_pointer operator &() const
      {
        return p_value;
      }

      pointer operator ->()
      {
        return p_value;
      }

      const_pointer operator ->() const
      {
        return p_value;
      }

      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.p_value == rhs.p_value;
      }

      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      value_type* p_value;
    };

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public std::iterator<std::forward_iterator_tag, const value_type>
    {
    public:

      friend class intrusive_forward_list;

      const_iterator()
        : p_value(nullptr)
      {
      }

      const_iterator(const value_type& value)
        : p_value(&value)
      {
      }

      const_iterator(const typename intrusive_forward_list<value_type, link_type>::iterator& other)
        : p_value(other.p_value)
      {
      }

      const_iterator(const const_iterator& other)
        : p_value(other.p_value)
      {
      }

      const_iterator& operator ++()
      {
        // Read the appropriate 'etl_next'.
        p_value = static_cast<value_type*>(p_value->link_type::etl_next);
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        // Read the appropriate 'etl_next'.
        p_value = static_cast<value_type*>(p_value->link_type::etl_next);
        return temp;
      }

      const_iterator operator =(const const_iterator& other)
      {
        p_value = other.p_value;
        return *this;
      }

      const_reference operator *() const
      {
        return *p_value;
      }

      const_pointer operator &() const
      {
        return p_value;
      }

      const_pointer operator ->() const
      {
        return p_value;
      }

      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.p_value == rhs.p_value;
      }

      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

    private:

      const value_type* p_value;
    };

		typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    intrusive_forward_list()
    {
      initialise();
    }

    //*************************************************************************
    /// Constructor from range
    //*************************************************************************
    template <typename TIterator>
    intrusive_forward_list(TIterator first, TIterator last)
    {
      assign(first, last);
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    iterator begin()
    {
      return iterator(static_cast<value_type&>(get_head()));
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(static_cast<const value_type&>(get_head()));
    }

    //*************************************************************************
    /// Gets before the beginning of the intrusive_forward_list.
    //*************************************************************************
    iterator before_begin()
    {
      return iterator(static_cast<value_type&>(start_link));
    }

    //*************************************************************************
    /// Gets before the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator before_begin() const
    {
      return const_iterator(static_cast<const value_type&>(start_link));
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(static_cast<const value_type&>(get_head()));
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
      return static_cast<value_type&>(get_head());
    }

    //*************************************************************************
    /// Gets a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return static_cast<const value_type&>(get_head());;
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
      ETL_ASSERT(count >= 0, ETL_ERROR(intrusive_forward_list_iterator_exception));
#endif

      initialise();

      link_type* p_last_link = &start_link;

      // Add all of the elements.
      while (first != last)
      {
        link_type& link = *first++;
        join(p_last_link, &link);
        join(&link, nullptr);
        p_last_link = &link;
        ++current_size;
      }
    }

    //*************************************************************************
    /// Pushes a value to the front of the intrusive_forward_list.
    //*************************************************************************
    void push_front(link_type& value)
    {
      insert_link_after(start_link, value);
    }

    //*************************************************************************
    /// Removes a value from the front of the intrusive_forward_list.
    //*************************************************************************
    void pop_front()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(intrusive_forward_list_empty));
#endif
      remove_link_after(start_link);
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

      link_type* first  = nullptr;             // To keep first link
      link_type* second = start_link.etl_next; // To keep second link
      link_type* track  = start_link.etl_next; // Track the list

      while (track != NULL)
      {
        track = track->etl_next;  // Track point to next link;
        second->etl_next = first; // Second link point to first
        first  = second;          // Move first link to next
        second = track;           // Move second link to next
      }

      join(&start_link, first);
    }

    //*************************************************************************
    /// Inserts a value to the intrusive_forward_list after the specified position.
    //*************************************************************************
    iterator insert_after(iterator position, value_type& value)
    {
      insert_link_after(*position.p_value, value);

      return iterator(value);
    }

    //*************************************************************************
    /// Inserts a range of values to the intrusive_forward_list after the specified position.
    //*************************************************************************
    template <typename TIterator>
    void insert_after(iterator position, TIterator first, TIterator last)
    {
      while (first != last)
      {
        // Set up the next free link.
        insert_link_after(*position.p_value, *first++);
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

      remove_link_after(*position.p_value);

      return next;
    }

    //*************************************************************************
    /// Erases a range of elements.
    //*************************************************************************
    iterator erase_after(iterator first, iterator last)
    {
      link_type* p_first = first.p_value;
      link_type* p_last  = last.p_value;
      link_type* p_next  = p_first->etl_next;

      // Join the ends.
      join(p_first, p_last);

      p_first = p_next;

      // Erase the ones in between.
      while (p_first != p_last)
      {
        // One less.
        --current_size;

        p_next  = p_first->etl_next; // Remember the next link.
        p_first = p_next;            // Move to the next link.
      }

      if (p_next == nullptr)
      {
        return end();
      }
      else
      {
        return iterator(*static_cast<value_type*>(p_last));
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

      link_type* last    = &get_head();
      link_type* current = last->etl_next;

      while (current != nullptr)
      {
        // Is this value the same as the last?
        if (isEqual(*static_cast<value_type*>(current), *static_cast<value_type*>(last)))
        {
          remove_link_after(*last);
        }
        else
        {
          // Move on one.
          last = current;
        }

        current = last->etl_next;
      }
    }

    //*************************************************************************
    /// Sort using in-place merge sort algorithm.
    //*************************************************************************
    void sort()
    {
      sort(std::less<value_type>());
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
      iterator i_link;
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
            // Decide whether the next link of merge comes from left or right.
            if (left_size == 0)
            {
		          // Left is empty. The link must come from right.
		          i_link = i_right;
              ++i_right;
              --right_size;
		        }
            else if (right_size == 0 || i_right == end())
            {
		          // Right is empty. The link must come from left.
		          i_link = i_left;
              ++i_left;
              --left_size;
		        }
            else if (compare(*i_left, *i_right))
            {
		          // First link of left is lower or same. The link must come from left.
		          i_link = i_left;
              ++i_left;
              --left_size;
		        }
            else
            {
		          // First link of right is lower. The link must come from right.
		          i_link  = i_right;
              ++i_right;
              --right_size;
		        }

            // Add the next link to the merged head.
            if (i_head == before_begin())
            {
                join(i_head.p_value, i_link.p_value);
                i_head = i_link;
                i_tail = i_link;
            }
            else
            {
                join(i_tail.p_value, i_link.p_value);
                i_tail = i_link;
            }

            i_tail.p_value->link_type::etl_next = nullptr;
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
    void remove(const_reference value)
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

    link_type start_link; ///< The link that acts as the intrusive_forward_list start.
    size_t current_size;  ///< The number of elements in the list.
    size_t index;         ///< The index level of the link that this list operates on.

    //*************************************************************************
    /// Join two links.
    //*************************************************************************
    void join(link_type* left, link_type* right)
    {
      left->etl_next = right;
    }

    //*************************************************************************
    /// Is the intrusive_forward_list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return current_size <= 1;
    }

    //*************************************************************************
    /// Insert a link.
    //*************************************************************************
    void insert_link_after(link_type& position, link_type& link)
    {
      // Connect to the intrusive_forward_list.
      join(&link,     position.etl_next);
      join(&position, &link);
      ++current_size;
    }

    //*************************************************************************
    /// Remove a link.
    //*************************************************************************
    void remove_link_after(link_type& link)
    {
      // The link to erase.
      link_type* p_link = link.etl_next;

      if (p_link != nullptr)
      {
        // Disconnect the link from the intrusive_forward_list.
        join(&link, p_link->etl_next);
        --current_size;
      }
    }

    //*************************************************************************
    /// Get the head link.
    //*************************************************************************
    link_type& get_head()
    {
      return *start_link.etl_next;
    }

    //*************************************************************************
    /// Get the head link.
    //*************************************************************************
    const link_type& get_head() const
    {
      return *start_link.etl_next;
    }

    //*************************************************************************
    /// Initialise the intrusive_forward_list.
    //*************************************************************************
    void initialise()
    {
      start_link.etl_next = nullptr;
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

#undef ETL_FILE

#endif
