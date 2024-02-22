///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_INTRUSIVE_FORWARD_LIST_INCLUDED
#define ETL_INTRUSIVE_FORWARD_LIST_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"
#include "functional.h"
#include "nullptr.h"
#include "type_traits.h"
#include "exception.h"
#include "error_handler.h"
#include "intrusive_links.h"

#include <stddef.h>

#include "private/minmax_push.h"

namespace etl
{
  //***************************************************************************
  /// Exception for the intrusive_forward_list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  class intrusive_forward_list_exception : public exception
  {
  public:

    intrusive_forward_list_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
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

    intrusive_forward_list_empty(string_type file_name_, numeric_type line_number_)
      : intrusive_forward_list_exception(ETL_ERROR_TEXT("intrusive_forward_list:empty", ETL_INTRUSIVE_FORWARD_LIST_FILE_ID"A"), file_name_, line_number_)
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

    intrusive_forward_list_iterator_exception(string_type file_name_, numeric_type line_number_)
      : intrusive_forward_list_exception(ETL_ERROR_TEXT("intrusive_forward_list:iterator", ETL_INTRUSIVE_FORWARD_LIST_FILE_ID"B"), file_name_, line_number_)
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

    intrusive_forward_list_index_exception(string_type file_name_, numeric_type line_number_)
      : intrusive_forward_list_exception(ETL_ERROR_TEXT("intrusive_forward_list:bounds", ETL_INTRUSIVE_FORWARD_LIST_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Unsorted exception for the intrusive_forward_list.
  ///\ingroup intrusive_list
  //***************************************************************************
  class intrusive_forward_list_unsorted : public intrusive_forward_list_exception
  {
  public:

    intrusive_forward_list_unsorted(string_type file_name_, numeric_type line_number_)
      : intrusive_forward_list_exception(ETL_ERROR_TEXT("intrusive_forward_list:unsorted", ETL_INTRUSIVE_FORWARD_LIST_FILE_ID"D"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// intrusive_stack_value_is_already_linked exception.
  ///\ingroup intrusive_stack
  //***************************************************************************
  class intrusive_forward_list_value_is_already_linked : public intrusive_forward_list_exception
  {
  public:

    intrusive_forward_list_value_is_already_linked(string_type file_name_, numeric_type line_number_)
      : intrusive_forward_list_exception(ETL_ERROR_TEXT("intrusive_forward_list:value is already linked", ETL_INTRUSIVE_FORWARD_LIST_FILE_ID"E"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Base for intrusive forward list.
  ///\ingroup intrusive_forward_list
  //***************************************************************************
  template <typename TLink>
  class intrusive_forward_list_base
  {
  public:

    // Node typedef.
    typedef TLink link_type;

    //*************************************************************************
    /// Clears the intrusive_forward_list.
    //*************************************************************************
    void clear()
    {
      // Unlink all of the items.
      link_type* p_unlink = start.etl_next;

      while (p_unlink != &terminator)
      {
        link_type* p_next = p_unlink->etl_next;
        p_unlink->clear();
        p_unlink = p_next;
      }

      initialise();
    }

    //*************************************************************************
    /// Assigns a range of values to the intrusive_forward_list.
    /// If ETL_THROW_EXCEPTIONS & ETL_DEBUG are defined throws forward_list_iterator if the iterators are reversed.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if ETL_IS_DEBUG_BUILD
      intmax_t d = etl::distance(first, last);
      ETL_ASSERT_OR_RETURN(d >= 0, ETL_ERROR(intrusive_forward_list_iterator_exception));
#endif

      clear();

      link_type* p_last = &start;

      // Add all of the elements.
      while (first != last)
      {
         link_type& value = *first++;

        ETL_ASSERT_OR_RETURN(!value.is_linked(), ETL_ERROR(intrusive_forward_list_value_is_already_linked));

        value.etl_next   = p_last->etl_next;
        p_last->etl_next = &value;
        p_last           = &value;
        ++current_size;
      }
    }

    //*************************************************************************
    /// Pushes a value to the front of the intrusive_forward_list.
    //*************************************************************************
    void push_front(link_type& value)
    {
      ETL_ASSERT_OR_RETURN(!value.is_linked(), ETL_ERROR(intrusive_forward_list_value_is_already_linked));

      insert_link_after(start, value);
    }

    //*************************************************************************
    /// Removes a value from the front of the intrusive_forward_list.
    //*************************************************************************
    void pop_front()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT_OR_RETURN(!empty(), ETL_ERROR(intrusive_forward_list_empty));
#endif
      remove_link_after(start);
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

      link_type* previous = &terminator;    // Point to the terminator of the linked list.
      link_type* current  = start.etl_next; // Point to the first item in the linked list (could be the terminator).
      link_type* next     = start.etl_next; // Point to the first item in the linked list (could be the terminator).
        
      while (next != &terminator)
      {
        next     = next->etl_next;    // Point to next link.
        current->etl_next = previous; // Reverse the current link.
        previous = current;           // Previous points to current.
        current  = next;              // Current points to next.
      }

      etl::link<link_type>(start, previous);
    }

    //*************************************************************************
    /// Returns true if the list has no elements.
    //*************************************************************************
    bool empty() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Returns the number of elements.
    //*************************************************************************
    size_t size() const
    {
      return current_size;
    }

  protected:

    link_type start;             ///< The link pointer that acts as the intrusive_forward_list start.
    static link_type terminator; ///< The link that acts as the intrusive_forward_list terminator.

    size_t current_size; ///< Counts the number of elements in the list.

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    intrusive_forward_list_base()
    {
      initialise();
    }

    //*************************************************************************
    /// Destructor
    //*************************************************************************
    ~intrusive_forward_list_base()
    {
      clear();
    }

    //*************************************************************************
    /// Is the intrusive_forward_list a trivial length?
    //*************************************************************************
    bool is_trivial_list() const
    {
      return (size() <= 1U);
    }

    //*************************************************************************
    /// Insert a link.
    //*************************************************************************
    void insert_link_after(link_type& position, link_type& link)
    {
      // Connect to the intrusive_forward_list.
      etl::link_splice<link_type>(position, link);
      ++current_size;
    }

    //*************************************************************************
    /// Remove a link.
    //*************************************************************************
    void remove_link_after(link_type& link)
    {
      link_type* p_next = link.etl_next;

      if (p_next != &this->terminator)
      {
        link_type* p_unlinked = etl::unlink_after<link_type>(link);
        p_unlinked->clear();
        --current_size;
      }
    }

    //*************************************************************************
    /// Get the head link.
    //*************************************************************************
    link_type* get_head()
    {
      return start.etl_next;
    }

    //*************************************************************************
    /// Get the head link.
    //*************************************************************************
    const link_type* get_head() const
    {
      return start.etl_next;
    }

    //*************************************************************************
    /// Initialise the intrusive_forward_list.
    //*************************************************************************
    void initialise()
    {
      start.etl_next = &terminator;
      current_size = 0;
    }
  };

  template <typename TLink>
  TLink etl::intrusive_forward_list_base<TLink>::terminator;

  //***************************************************************************
  /// An intrusive forward list.
  ///\ingroup intrusive_forward_list
  ///\note TLink must be a base of TValue.
  //***************************************************************************
  template <typename TValue, typename TLink>
  class intrusive_forward_list : public etl::intrusive_forward_list_base<TLink>
  {
  public:

    // Node typedef.
    typedef typename etl::intrusive_forward_list_base<TLink>::link_type link_type;

    // STL style typedefs.
    typedef TValue            value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef size_t            size_type;

    typedef intrusive_forward_list<TValue, TLink> list_type;

    //*************************************************************************
    /// iterator.
    //*************************************************************************
    class iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, value_type>
    {
    public:

      friend class intrusive_forward_list;
      friend class const_iterator;

      iterator()
        : p_value(ETL_NULLPTR)
      {
      }

      iterator(const iterator& other)
        : p_value(other.p_value)
      {
      }

      iterator& operator ++()
      {
        // Read the appropriate 'etl_next'.
        p_value = p_value->etl_next;
        return *this;
      }

      iterator operator ++(int)
      {
        iterator temp(*this);
        // Read the appropriate 'etl_next'.
        p_value = p_value->etl_next;
        return temp;
      }

      iterator& operator =(const iterator& other)
      {
        p_value = other.p_value;
        return *this;
      }

      reference operator *() const
      {
        return *static_cast<pointer>(p_value);
      }

      pointer operator &() const
      {
        return static_cast<pointer>(p_value);
      }

      pointer operator ->() const
      {
        return static_cast<pointer>(p_value);
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

      iterator(link_type* value)
        : p_value(value)
      {
      }

      link_type* p_value;
    };

    //*************************************************************************
    /// const_iterator
    //*************************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::forward_iterator_tag, const value_type>
    {
    public:

      friend class intrusive_forward_list;

      const_iterator()
        : p_value(ETL_NULLPTR)
      {
      }

      const_iterator(const typename intrusive_forward_list::iterator& other)
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
        p_value = p_value->etl_next;
        return *this;
      }

      const_iterator operator ++(int)
      {
        const_iterator temp(*this);
        // Read the appropriate 'etl_next'.
        p_value = p_value->etl_next;
        return temp;
      }

      const_iterator& operator =(const const_iterator& other)
      {
        p_value = other.p_value;
        return *this;
      }

      const_reference operator *() const
      {
        return *static_cast<const value_type*>(p_value);
      }

      const_pointer operator &() const
      {
        return static_cast<const value_type*>(p_value);
      }

      const_pointer operator ->() const
      {
        return static_cast<const value_type*>(p_value);
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

      const_iterator(const link_type* value)
        : p_value(value)
      {
      }

      const link_type* p_value;
    };

    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    intrusive_forward_list()
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~intrusive_forward_list()
    {
    }

    //*************************************************************************
    /// Constructor from range
    //*************************************************************************
    template <typename TIterator>
    intrusive_forward_list(TIterator first, TIterator last, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
    {
      this->assign(first, last);
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    iterator begin()
    {
      return iterator(this->get_head());
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator begin() const
    {
      return const_iterator(this->get_head());
    }

    //*************************************************************************
    /// Gets before the beginning of the intrusive_forward_list.
    //*************************************************************************
    iterator before_begin()
    {
      return iterator(&this->start);
    }

    //*************************************************************************
    /// Gets before the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator before_begin() const
    {
      return const_iterator(&this->start);
    }

    //*************************************************************************
    /// Gets the beginning of the intrusive_forward_list.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(this->get_head());
    }

    //*************************************************************************
    /// Gets the end of the intrusive_forward_list.
    //*************************************************************************
    iterator end()
    {
      return iterator(&this->terminator);
    }

    //*************************************************************************
    /// Gets the end of the intrusive_forward_list.
    //*************************************************************************
    const_iterator end() const
    {
      return const_iterator(&this->terminator);
    }

    //*************************************************************************
    /// Gets the end of the intrusive_forward_list.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator(&this->terminator);
    }

    //*************************************************************************
    /// Gets a reference to the first element.
    //*************************************************************************
    reference front()
    {
      return *static_cast<pointer>(this->get_head());
    }

    //*************************************************************************
    /// Gets a const reference to the first element.
    //*************************************************************************
    const_reference front() const
    {
      return *static_cast<const value_type*>(this->get_head());
    }

    //*************************************************************************
    /// Inserts a value to the intrusive_forward_list after the specified position.
    //*************************************************************************
    iterator insert_after(iterator position, value_type& value)
    {
      ETL_ASSERT_OR_RETURN_VALUE(!value.link_type::is_linked(), ETL_ERROR(intrusive_forward_list_value_is_already_linked), iterator(&value));

      this->insert_link_after(*position.p_value, value);
      return iterator(&value);
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
        ETL_ASSERT_OR_RETURN(!(*first).link_type::is_linked(), ETL_ERROR(intrusive_forward_list_value_is_already_linked));
        
        this->insert_link_after(*position.p_value, *first);
        ++first;
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
          this->remove_link_after(*position.p_value);
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
        this->current_size -= etl::distance(first, last) - 1;

        link_type* p_first = first.p_value;
        link_type* p_last  = last.p_value;
        link_type* p_after = p_first->etl_next;
        
        // Join the ends.
        etl::link<link_type>(p_first, p_last);

        // Unlink the erased range.
        link_type* p_unlink = p_after;

        while (p_unlink != p_last)
        {
          link_type* p_next = p_unlink->etl_next;
          p_unlink->clear();
          p_unlink = p_next;
        }

        if (p_after == &this->terminator)
        {
          return end();
        }
        else
        {
          return last;
        }
      }
      else
      {
        return last;
      }
    }

    //*************************************************************************
    /// Removes all but the one element from every consecutive group of equal
    /// elements in the container.
    //*************************************************************************
    template <typename TIsEqual>
    void unique(TIsEqual isEqual)
    {
      if (this->empty())
      {
        return;
      }

      link_type* last    = this->get_head();
      link_type* current = last->etl_next;

      while (current != &this->terminator)
      {
        // Is this value the same as the last?
        if (isEqual(*static_cast<pointer>(current), *static_cast<pointer>(last)))
        {
          this->remove_link_after(*last);
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
      sort(etl::less<value_type>());
    }

    //*************************************************************************
    /// Stable sort using in-place merge sort algorithm.
    /// Copyright 2001 Simon Tatham.
    ///
    /// Permission is hereby granted, free of charge, to any person
    /// obtaining a copy of this software and associated documentation
    /// files (the "Software"), to deal in the Software without
    /// restriction, including without limitation the rights to use,
    /// copy, modify, merge, publish, distribute, sublicense, and/or
    /// sell copies of the Software, and to permit persons to whom the
    /// Software is furnished to do so, subject to the following
    /// conditions:
    ///
    /// The above copyright notice and this permission notice shall be
    /// included in all copies or substantial portions of the Software.
    ///
    /// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    /// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    /// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    /// NONINFRINGEMENT.  IN NO EVENT SHALL SIMON TATHAM BE LIABLE FOR
    /// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    /// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    /// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    /// SOFTWARE.
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

      if (this->is_trivial_list())
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
            else if (!compare(*i_right, *i_left))
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
              etl::link<link_type>(i_head.p_value, i_link.p_value);
              i_head = i_link;
              i_tail = i_link;
            }
            else
            {
              etl::link<link_type>(i_tail.p_value, i_link.p_value);
              i_tail = i_link;
            }

            i_tail.p_value->etl_next = &this->terminator;
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
    /// Splice another list into this one.
    //*************************************************************************
    void splice_after(iterator position, etl::intrusive_forward_list<TValue, TLink>& other)
    {
      // No point splicing to ourself!
      if (&other != this)
      {
        if (!other.empty())
        {
          link_type& first = *other.get_head();

          if (&other != this)
          {
            this->current_size += other.size();
          }

          link_type& before = *position.p_value;
          link_type& after  = *position.p_value->etl_next;

          etl::link<link_type>(before, first);

          link_type* last = &before;
          while (last->etl_next != &other.terminator)
          {
            last = last->etl_next;
          }

          etl::link<link_type>(last, after);

          other.initialise();
        }
      }
    }

    //*************************************************************************
    /// Splice an element from another list into this one.
    //*************************************************************************
    void splice_after(iterator position, etl::intrusive_forward_list<TValue, TLink>& other, iterator isource)
    {
      link_type& before = *position.p_value;

      etl::unlink<link_type>(*isource.p_value);
      etl::link_splice<link_type>(before, *isource.p_value);

      if (&other != this)
      {
        ++this->current_size;
        --other.current_size;
      }
    }

    //*************************************************************************
    /// Splice a range of elements from another list into this one.
    //*************************************************************************
    void splice_after(iterator position, etl::intrusive_forward_list<TValue, TLink>& other, iterator begin_, iterator end_)
    {
      if (!other.empty())
      {
        if (&other != this)
        {
          size_t n = etl::distance(begin_, end_) - 1;
          this->current_size += n;
          other.current_size -= n;
        }

        link_type* first = begin_.p_value;
        link_type* last  = first;

        while (last->etl_next != end_.p_value)
        {
          last = last->etl_next;
        }

        // Unlink from the source list.
        link_type* first_next = first->etl_next;
        etl::unlink_after(*first, *last);

        // Fix our links.
        link_type* before = position.p_value;

        etl::link_splice<link_type>(*before, *first_next, *last);
      }
    }

    //*************************************************************************
    /// Merge another list into this one. Both lists should be sorted.
    //*************************************************************************
    void merge(list_type& other)
    {
      merge(other, etl::less<value_type>());
    }

    //*************************************************************************
    /// Merge another list into this one. Both lists should be sorted.
    //*************************************************************************
    template <typename TCompare>
    void merge(list_type& other, TCompare compare)
    {
      if ((this != &other) && !other.empty())
      {
#if ETL_IS_DEBUG_BUILD
        ETL_ASSERT(etl::is_sorted(other.begin(), other.end(), compare), ETL_ERROR(intrusive_forward_list_unsorted));
        ETL_ASSERT(etl::is_sorted(begin(), end(), compare), ETL_ERROR(intrusive_forward_list_unsorted));
#endif

        link_type* other_begin    = other.get_head();
        link_type* other_terminal = &other.terminator;

        link_type* before      = &this->start;
        link_type* before_next = get_next(before);
        link_type* terminal    = &this->terminator;;

        while ((before->etl_next != terminal) && (other_begin != other_terminal))
        {
          // Find the place to insert.
          while ((before_next != terminal) && !(compare(*static_cast<pointer>(other_begin), *static_cast<pointer>(before_next))))
          {
            before      = before_next;
            before_next = get_next(before_next);
          }

          // Insert.
          if (before_next != terminal)
          {
            while ((other_begin != other_terminal) && (compare(*static_cast<pointer>(other_begin), *static_cast<pointer>(before_next))))
            {
              link_type* value = other_begin;
              other_begin = get_next(other_begin);
              etl::link_splice<link_type>(*before, *value);
              before = get_next(before);
            }
          }
        }

        // Any left over?
        if (before_next == terminal)
        {
          while (other_begin != other_terminal)
          {
            link_type* value = other_begin;
            other_begin = get_next(other_begin);
            etl::link_splice<link_type>(*before, *value);
            before = get_next(before);
          }
        }

        this->current_size += other.size();

        other.initialise();
      }
    }

  private:

    //*************************************************************************
    /// Get the next value.
    //*************************************************************************
    link_type* get_next(link_type* link) const
    {
      return link->etl_next;
    }

    // Disabled.
    intrusive_forward_list(const intrusive_forward_list& other);
    intrusive_forward_list& operator = (const intrusive_forward_list& rhs);
  };
}

#include "private/minmax_pop.h"

#endif
