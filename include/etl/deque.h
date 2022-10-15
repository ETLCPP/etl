///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_DEQUE_INCLUDED
#define ETL_DEQUE_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"
#include "utility.h"
#include "memory.h"
#include "exception.h"
#include "error_handler.h"
#include "debug_count.h"
#include "algorithm.h"
#include "type_traits.h"
#include "iterator.h"
#include "placement_new.h"
#include "initializer_list.h"

#include <stddef.h>
#include <stdint.h>

#include "private/minmax_push.h"

//*****************************************************************************
///\defgroup deque deque
/// A double ended queue with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Exception base for deques
  ///\ingroup deque
  //***************************************************************************
  class deque_exception : public etl::exception
  {
  public:

    deque_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Deque full exception.
  ///\ingroup deque
  //***************************************************************************
  class deque_full : public etl::deque_exception
  {
  public:

    deque_full(string_type file_name_, numeric_type line_number_)
      : etl::deque_exception(ETL_ERROR_TEXT("deque:full", ETL_DEQUE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Deque empty exception.
  ///\ingroup deque
  //***************************************************************************
  class deque_empty : public etl::deque_exception
  {
  public:

    deque_empty(string_type file_name_, numeric_type line_number_)
      : etl::deque_exception(ETL_ERROR_TEXT("deque:empty", ETL_DEQUE_FILE_ID"B"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Deque out of bounds exception.
  ///\ingroup deque
  //***************************************************************************
  class deque_out_of_bounds : public etl::deque_exception
  {
  public:

    deque_out_of_bounds(string_type file_name_, numeric_type line_number_)
      : etl::deque_exception(ETL_ERROR_TEXT("deque:bounds", ETL_DEQUE_FILE_ID"C"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup vector
  /// Deque incompatible type exception.
  //***************************************************************************
  class deque_incompatible_type : public deque_exception
  {
  public:

    deque_incompatible_type(string_type file_name_, numeric_type line_number_)
      : deque_exception(ETL_ERROR_TEXT("deque:type", ETL_DEQUE_FILE_ID"D"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The base class for all templated deque types.
  ///\ingroup deque
  //***************************************************************************
  class deque_base
  {
  public:

    typedef size_t size_type;

    //*************************************************************************
    /// Gets the current size of the deque.
    ///\return The current size of the deque.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Checks the 'empty' state of the deque.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Checks the 'full' state of the deque.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return current_size == CAPACITY;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the deque.
    ///\return The maximum size of the deque.
    //*************************************************************************
    size_type max_size() const
    {
      return CAPACITY;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the deque.
    ///\return The maximum size of the deque.
    //*************************************************************************
    size_type capacity() const
    {
      return CAPACITY;
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return max_size() - size();
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    deque_base(size_t max_size_, size_t buffer_size_)
      : current_size(0),
      CAPACITY(max_size_),
      BUFFER_SIZE(buffer_size_)
    {
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~deque_base()
    {
    }

    size_type       current_size; ///< The current number of elements in the deque.
    const size_type CAPACITY;     ///< The maximum number of elements in the deque.
    const size_type BUFFER_SIZE;  ///< The number of elements in the buffer.
    ETL_DECLARE_DEBUG_COUNT       ///< Internal debugging.
  };

  //***************************************************************************
  /// The base class for all etl::deque classes.
  ///\tparam T The type of values this deque should hold.
  ///\ingroup deque
  //***************************************************************************
  template <typename T>
  class ideque : public etl::deque_base
  {
  public:

    typedef T        value_type;
    typedef size_t   size_type;
    typedef T& reference;
    typedef const T& const_reference;
#if __cpp_rvalue_references
    typedef T&& rvalue_reference;
#endif
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef typename etl::iterator_traits<pointer>::difference_type difference_type;

    //*************************************************************************
    /// Iterator
    //*************************************************************************
    class iterator : public etl::iterator<ETL_OR_STD::random_access_iterator_tag, T>
    {
    public:

      friend class ideque;
      friend class const_iterator;

      //***************************************************
      iterator()
        : index(0)
        , p_deque(0)
        , p_buffer(0)
      {
      }

      //***************************************************
      iterator(const iterator& other)
        : index(other.index),
        p_deque(other.p_deque),
        p_buffer(other.p_buffer)
      {
      }

      //***************************************************
      iterator& operator =(const iterator& other)
      {
        index = other.index;
        p_deque = other.p_deque;
        p_buffer = other.p_buffer;

        return *this;
      }

      //***************************************************
      iterator& operator ++()
      {
        index = (static_cast<size_t>(index) == p_deque->BUFFER_SIZE - 1) ? 0 : index + 1;

        return *this;
      }

      //***************************************************
      iterator operator ++(int)
      {
        iterator previous(*this);
        index = (static_cast<size_t>(index) == p_deque->BUFFER_SIZE - 1) ? 0 : index + 1;

        return previous;
      }

      //***************************************************
      iterator& operator +=(difference_type offset)
      {
        if (offset > 0)
        {
          index += offset;
          index = (static_cast<size_t>(index) > p_deque->BUFFER_SIZE - 1) ? index - p_deque->BUFFER_SIZE : index;
        }
        else if (offset < 0)
        {
          operator -= (-offset);
        }

        return *this;
      }

      //***************************************************
      iterator& operator -=(difference_type offset)
      {
        if (offset > 0)
        {
          index -= offset;
          index = (index < 0) ? index + p_deque->BUFFER_SIZE : index;
        }
        else if (offset < 0)
        {
          operator += (-offset);
        }

        return *this;
      }

      //***************************************************
      iterator& operator --()
      {
        index = (index == 0) ? p_deque->BUFFER_SIZE - 1 : index - 1;

        return *this;
      }

      //***************************************************
      iterator operator --(int)
      {
        iterator previous(*this);
        index = (index == 0) ? p_deque->BUFFER_SIZE - 1 : index - 1;

        return previous;
      }

      //***************************************************
      reference operator *() const
      {
        return p_buffer[index];
      }

      //***************************************************
      pointer operator ->() const
      {
        return &p_buffer[index];
      }

      //***************************************************
      friend iterator operator +(const iterator& lhs, difference_type offset)
      {
        iterator result(lhs);
        result += offset;
        return result;
      }

      //***************************************************
      friend iterator operator -(const iterator& lhs, difference_type offset)
      {
        iterator result(lhs);
        result -= offset;
        return result;
      }

      //***************************************************
      friend bool operator == (const iterator& lhs, const iterator& rhs)
      {
        return lhs.index == rhs.index;
      }

      //***************************************************
      friend bool operator != (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs == rhs);
      }

      //***************************************************
      friend bool operator < (const iterator& lhs, const iterator& rhs)
      {
        const difference_type lhs_index = lhs.get_index();
        const difference_type rhs_index = rhs.get_index();
        const difference_type reference_index = lhs.container().begin().get_index();
        const size_t buffer_size = lhs.container().max_size() + 1;

        const difference_type lhs_distance = (lhs_index < reference_index) ? buffer_size + lhs_index - reference_index : lhs_index - reference_index;
        const difference_type rhs_distance = (rhs_index < reference_index) ? buffer_size + rhs_index - reference_index : rhs_index - reference_index;

        return lhs_distance < rhs_distance;
      }

      //***************************************************
      friend bool operator <= (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs > rhs);
      }

      //***************************************************
      friend bool operator > (const iterator& lhs, const iterator& rhs)
      {
        return (rhs < lhs);
      }

      //***************************************************
      friend bool operator >= (const iterator& lhs, const iterator& rhs)
      {
        return !(lhs < rhs);
      }

      //***************************************************
      difference_type get_index() const
      {
        return index;
      }

      //***************************************************
      ideque& container() const
      {
        return *p_deque;
      }

      //***************************************************
      pointer get_buffer() const
      {
        return p_buffer;
      }

      //***************************************************
      void swap(iterator& other)
      {
        using ETL_OR_STD::swap; // Allow ADL

        swap(index, other.index);
      }

    private:

      //***************************************************
      difference_type distance(difference_type firstIndex, difference_type index_) const
      {
        if (index_ < firstIndex)
        {
          return p_deque->BUFFER_SIZE + index_ - firstIndex;
        }
        else
        {
          return index_ - firstIndex;
        }
      }

      //***************************************************
      iterator(difference_type index_, ideque& the_deque, pointer p_buffer_)
        : index(index_)
        , p_deque(&the_deque)
        , p_buffer(p_buffer_)
      {
      }

      difference_type index;
      ideque* p_deque;
      pointer         p_buffer;
    };

    //*************************************************************************
    /// Const Iterator
    //*************************************************************************
    class const_iterator : public etl::iterator<ETL_OR_STD::random_access_iterator_tag, const T>
    {
    public:

      friend class ideque;

      //***************************************************
      const_iterator()
        : index(0)
        , p_deque(0)
        , p_buffer(0)
      {
      }

      //***************************************************
      const_iterator(const const_iterator& other)
        : index(other.index)
        , p_deque(other.p_deque)
        , p_buffer(other.p_buffer)
      {
      }

      //***************************************************
      const_iterator(const typename ideque::iterator& other)
        : index(other.index)
        , p_deque(other.p_deque)
        , p_buffer(other.p_buffer)
      {
      }

      //***************************************************
      const_iterator& operator =(const const_iterator& other)
      {
        index = other.index;
        p_deque = other.p_deque;
        p_buffer = other.p_buffer;

        return *this;
      }

      const_iterator& operator =(const typename ideque::iterator& other)
      {
        index = other.index;
        p_deque = other.p_deque;
        p_buffer = other.p_buffer;

        return *this;
      }

      //***************************************************
      const_iterator& operator ++()
      {
        index = (static_cast<size_t>(index) == p_deque->BUFFER_SIZE - 1) ? 0 : index + 1;

        return *this;
      }

      //***************************************************
      const_iterator operator ++(int)
      {
        const_iterator previous(*this);
        index = (static_cast<size_t>(index) == p_deque->BUFFER_SIZE - 1) ? 0 : index + 1;

        return previous;
      }

      //***************************************************
      const_iterator& operator +=(difference_type offset)
      {
        if (offset > 0)
        {
          index += offset;
          index = (static_cast<size_t>(index) > p_deque->BUFFER_SIZE - 1) ? index - p_deque->BUFFER_SIZE : index;
        }
        else if (offset < 0)
        {
          operator -= (-offset);
        }

        return *this;
      }

      //***************************************************
      const_iterator& operator -=(difference_type offset)
      {
        if (offset > 0)
        {
          index -= offset;
          index = (index < 0) ? static_cast<size_t>(index) + p_deque->BUFFER_SIZE : index;
        }
        else if (offset < 0)
        {
          operator += (-offset);
        }

        return *this;
      }

      //***************************************************
      const_iterator& operator --()
      {
        index = (index == 0) ? p_deque->BUFFER_SIZE - 1 : index - 1;

        return *this;
      }

      //***************************************************
      const_iterator operator --(int)
      {
        const_iterator previous(*this);
        index = (index == 0) ? p_deque->BUFFER_SIZE - 1 : index - 1;

        return previous;
      }

      //***************************************************
      const_reference operator *() const
      {
        return p_buffer[index];
      }

      //***************************************************
      const_pointer operator ->() const
      {
        return &p_buffer[index];
      }

      //***************************************************
      friend const_iterator operator +(const const_iterator& lhs, difference_type offset)
      {
        const_iterator result(lhs);
        result += offset;
        return result;
      }

      //***************************************************
      friend const_iterator operator -(const const_iterator& lhs, difference_type offset)
      {
        const_iterator result(lhs);
        result -= offset;
        return result;
      }

      //***************************************************
      friend bool operator == (const const_iterator& lhs, const const_iterator& rhs)
      {
        return lhs.index == rhs.index;
      }

      //***************************************************
      friend bool operator != (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs == rhs);
      }

      //***************************************************
      friend bool operator < (const const_iterator& lhs, const const_iterator& rhs)
      {
        const difference_type lhs_index = lhs.get_index();
        const difference_type rhs_index = rhs.get_index();
        const difference_type reference_index = lhs.container().begin().get_index();
        const size_t buffer_size = lhs.container().max_size() + 1UL;

        const difference_type lhs_distance = (lhs_index < reference_index) ? buffer_size + lhs_index - reference_index : lhs_index - reference_index;
        const difference_type rhs_distance = (rhs_index < reference_index) ? buffer_size + rhs_index - reference_index : rhs_index - reference_index;

        return lhs_distance < rhs_distance;
      }

      //***************************************************
      friend bool operator <= (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs > rhs);
      }

      //***************************************************
      friend bool operator > (const const_iterator& lhs, const const_iterator& rhs)
      {
        return (rhs < lhs);
      }

      //***************************************************
      friend bool operator >= (const const_iterator& lhs, const const_iterator& rhs)
      {
        return !(lhs < rhs);
      }

      //***************************************************
      difference_type get_index() const
      {
        return index;
      }

      //***************************************************
      ideque& container() const
      {
        return *p_deque;
      }

      //***************************************************
      pointer get_buffer() const
      {
        return p_buffer;
      }

      //***************************************************
      void swap(const_iterator& other)
      {
        ETL_OR_STD::swap(index, other.index);
      }

    private:

      //***************************************************
      difference_type distance(difference_type firstIndex, difference_type index_) const
      {
        if (index_ < firstIndex)
        {
          return p_deque->BUFFER_SIZE + index_ - firstIndex;
        }
        else
        {
          return index_ - firstIndex;
        }
      }

      //***************************************************
      const_iterator(difference_type index_, ideque& the_deque, pointer p_buffer_)
        : index(index_)
        , p_deque(&the_deque)
        , p_buffer(p_buffer_)
      {
      }

      difference_type index;
      ideque* p_deque;
      pointer         p_buffer;
    };

    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Assigns a range to the deque.
    //*************************************************************************
    template<typename TIterator>
    typename etl::enable_if<!etl::is_integral<TIterator>::value, void>::type
      assign(TIterator range_begin, TIterator range_end)
    {
      initialise();

      while (range_begin != range_end)
      {
        push_back(*range_begin);
        ++range_begin;
      }
    }

    //*************************************************************************
    /// Assigns 'n' copies of a value to the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full is 'n' is too large.
    ///\param n     The number of copies to assign.
    ///\param value The value to add.<
    //*************************************************************************
    void assign(size_type n, const value_type& value)
    {
      ETL_ASSERT(n <= CAPACITY, ETL_ERROR(deque_full));

      initialise();

      while (n > 0)
      {
        create_element_back(value);
        --n;
      }
    }

    //*************************************************************************
    /// Gets a reference to the item at the index.
    /// If asserts or exceptions are enabled, throws an etl::deque_out_of_bounds if the index is out of range.
    ///\return A reference to the item at the index.
    //*************************************************************************
    reference at(size_t index)
    {
      ETL_ASSERT(index < current_size, ETL_ERROR(deque_out_of_bounds));

      iterator result(_begin);
      result += index;

      return *result;
    }

    //*************************************************************************
    /// Gets a const reference to the item at the index.
    /// If asserts or exceptions are enabled, throws an etl::deque_out_of_bounds if the index is out of range.
    ///\return A const reference to the item at the index.
    //*************************************************************************
    const_reference at(size_t index) const
    {
      ETL_ASSERT(index < current_size, ETL_ERROR(deque_out_of_bounds));

      iterator result(_begin);
      result += index;

      return *result;
    }

    //*************************************************************************
    /// Gets a reference to the item at the index.
    ///\return A reference to the item at the index.
    //*************************************************************************
    reference operator [](size_t index)
    {
      iterator result(_begin);
      result += index;

      return *result;
    }

    //*************************************************************************
    /// Gets a const reference to the item at the index.
    ///\return A const reference to the item at the index.
    //*************************************************************************
    const_reference operator [](size_t index) const
    {
      iterator result(_begin);
      result += index;

      return *result;
    }

    //*************************************************************************
    /// Gets a reference to the item at the front of the deque.
    ///\return A reference to the item at the front of the deque.
    //*************************************************************************
    reference front()
    {
      return *_begin;
    }

    //*************************************************************************
    /// Gets a const reference to the item at the front of the deque.
    ///\return A const reference to the item at the front of the deque.
    //*************************************************************************
    const_reference front() const
    {
      return *_begin;
    }

    //*************************************************************************
    /// Gets a reference to the item at the back of the deque.
    ///\return A reference to the item at the back of the deque.
    //*************************************************************************
    reference back()
    {
      return *(_end - 1);
    }

    //*************************************************************************
    /// Gets a const reference to the item at the back of the deque.
    ///\return A const reference to the item at the back of the deque.
    //*************************************************************************
    const_reference back() const
    {
      return *(_end - 1);
    }

    //*************************************************************************
    /// Gets an iterator to the beginning of the deque.
    //*************************************************************************
    iterator begin()
    {
      return _begin;
    }

    //*************************************************************************
    /// Gets a const iterator to the beginning of the deque.
    //*************************************************************************
    const_iterator begin() const
    {
      return _begin;
    }

    //*************************************************************************
    /// Gets a const iterator to the beginning of the deque.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return _begin;
    }

    //*************************************************************************
    /// Gets an iterator to the end of the deque.
    //*************************************************************************
    iterator end()
    {
      return iterator(_end);
    }

    //*************************************************************************
    /// Gets a const iterator to the end of the deque.
    //*************************************************************************
    const_iterator end() const
    {
      return iterator(_end);
    }

    //*************************************************************************
    /// Gets a const iterator to the end of the deque.
    //*************************************************************************
    const_iterator cend() const
    {
      return const_iterator(_end);
    }

    //*************************************************************************
    /// Gets a reverse iterator to the end of the deque.
    //*************************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(end());
    }

    //*************************************************************************
    /// Gets a const reverse iterator to the end of the deque.
    //*************************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }

    //*************************************************************************
    /// Gets a const reverse iterator to the end of the deque.
    //*************************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(cend());
    }

    //*************************************************************************
    /// Gets a reverse iterator to the beginning of the deque.
    //*************************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(begin());
    }

    //*************************************************************************
    /// Gets a const reverse iterator to the beginning of the deque.
    //*************************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }

    //*************************************************************************
    /// Gets a const reverse iterator to the beginning of the deque.
    //*************************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(cbegin());
    }

    //*************************************************************************
    /// Clears the deque.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*************************************************************************
    /// Fills the deque.
    //*************************************************************************
    void fill(const T& value)
    {
      etl::fill(begin(), end(), value);
    }

    //*************************************************************************
    /// Inserts data into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is full.
    ///\param insert_position>The insert position.
    ///\param value>The value to insert.
    //*************************************************************************
    iterator insert(const_iterator insert_position, const value_type& value)
    {
      iterator position(to_iterator(insert_position));

      ETL_ASSERT(!full(), ETL_ERROR(deque_full));

      if (insert_position == begin())
      {
        create_element_front(value);
        position = _begin;
      }
      else if (insert_position == end())
      {
        create_element_back(value);
        position = _end - 1;
      }
      else
      {
        // Are we closer to the front?
        if (etl::distance(_begin, position) < etl::distance(position, _end - 1))
        {
          // Construct the _begin.
          create_element_front(*_begin);

          // Move the values.
          etl::move(_begin + 1, position, _begin);

          // Write the new value.
          *--position = value;
        }
        else
        {
          // Construct the _end.
          create_element_back(*(_end - 1));

          // Move the values.
          etl::move_backward(position, _end - 2, _end - 1);

          // Write the new value.
          *position = value;
        }
      }

      return position;
    }

#if __cpp_rvalue_references
    //*************************************************************************
    /// Inserts data into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is full.
    ///\param insert_position>The insert position.
    ///\param value>The value to insert.
    //*************************************************************************
    iterator insert(const_iterator insert_position, value_type&& value)
    {
      iterator position(insert_position.index, *this, p_buffer);

      ETL_ASSERT(!full(), ETL_ERROR(deque_full));

      if (insert_position == begin())
      {
        create_element_front(etl::move(value));
        position = _begin;
      }
      else if (insert_position == end())
      {
        create_element_back(etl::move(value));
        position = _end - 1;
      }
      else
      {
        // Are we closer to the front?
        if (etl::distance(_begin, position) < etl::distance(position, _end - 1))
        {
          // Construct the _begin.
          create_element_front(etl::move(*_begin));

          // Move the values.
          etl::move(_begin + 1, position, _begin);

          // Write the new value.
          *--position = etl::move(value);
        }
        else
        {
          // Construct the _end.
          create_element_back(etl::move(*(_end - 1)));

          // Move the values.
          etl::move_backward(position, _end - 2, _end - 1);

          // Write the new value.
          *position = etl::move(value);
        }
      }

      return position;
    }
#endif

    //*************************************************************************
    /// Emplaces data into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is full.
    ///\param insert_position>The insert position.
    //*************************************************************************
#if __cpp_rvalue_references && __cpp_variadic_templates && ETL_NOT_USING_STLPORT
    template <typename ... Args>
    iterator emplace(const_iterator insert_position, Args && ... args)
    {
      iterator position(insert_position.index, *this, p_buffer);

      ETL_ASSERT(!full(), ETL_ERROR(deque_full));

      void* p;

      if (insert_position == begin())
      {
        --_begin;
        p = etl::addressof(*_begin);
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _begin;
      }
      else if (insert_position == end())
      {
        p = etl::addressof(*_end);
        ++_end;
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _end - 1;
      }
      else
      {
        // Are we closer to the front?
        if (etl::distance(_begin, position) < etl::distance(position, _end - 1))
        {
          // Construct the _begin.
          create_element_front(*_begin);

          // Move the values.
          etl::move(_begin + 1, position, _begin);

          // Write the new value.
          --position;
          (*position).~T();
          p = etl::addressof(*position);
        }
        else
        {
          // Construct the _end.
          create_element_back(*(_end - 1));

          // Move the values.
          etl::move_backward(position, _end - 2, _end - 1);

          // Write the new value.
          (*position).~T();
          p = etl::addressof(*position);
        }
      }

      ::new (p) T(etl::forward<Args>(args)...);

      return position;
    }

#else

    //*************************************************************************
    /// Emplaces data into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is full.
    ///\param insert_position>The insert position.
    //*************************************************************************
    template <typename T1>
    iterator emplace(const_iterator insert_position, const T1& value1)
    {
      iterator position(insert_position.index, *this, p_buffer);

      ETL_ASSERT(!full(), ETL_ERROR(deque_full));

      void* p;

      if (insert_position == begin())
      {
        --_begin;
        p = etl::addressof(*_begin);
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _begin;
      }
      else if (insert_position == end())
      {
        p = etl::addressof(*_end);
        ++_end;
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _end - 1;
      }
      else
      {
        // Are we closer to the front?
        if (etl::distance(_begin, position) < etl::distance(position, _end - 1))
        {
          // Construct the _begin.
          create_element_front(*_begin);

          // Move the values.
          etl::move(_begin + 1, position, _begin);

          // Write the new value.
          --position;
          (*position).~T();
          p = etl::addressof(*position);
        }
        else
        {
          // Construct the _end.
          create_element_back(*(_end - 1));

          // Move the values.
          etl::move_backward(position, _end - 2, _end - 1);

          // Write the new value.
          (*position).~T();
          p = etl::addressof(*position);
        }
      }

      ::new (p) T(value1);

      return position;
    }

    //*************************************************************************
    /// Emplaces data into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is full.
    ///\param insert_position>The insert position.
    //*************************************************************************
    template <typename T1, typename T2>
    iterator emplace(const_iterator insert_position, const T1& value1, const T2& value2)
    {
      iterator position(insert_position.index, *this, p_buffer);

      ETL_ASSERT(!full(), ETL_ERROR(deque_full));

      void* p;

      if (insert_position == begin())
      {
        --_begin;
        p = etl::addressof(*_begin);
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _begin;
      }
      else if (insert_position == end())
      {
        p = etl::addressof(*_end);
        ++_end;
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _end - 1;
      }
      else
      {
        // Are we closer to the front?
        if (etl::distance(_begin, position) < etl::distance(position, _end - 1))
        {
          // Construct the _begin.
          create_element_front(*_begin);

          // Move the values.
          etl::move(_begin + 1, position, _begin);

          // Write the new value.
          --position;
          (*position).~T();
          p = etl::addressof(*position);
        }
        else
        {
          // Construct the _end.
          create_element_back(*(_end - 1));

          // Move the values.
          etl::move_backward(position, _end - 2, _end - 1);

          // Write the new value.
          (*position).~T();
          p = etl::addressof(*position);
        }
      }

      ::new (p) T(value1, value2);

      return position;
    }

    //*************************************************************************
    /// Emplaces data into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is full.
    ///\param insert_position>The insert position.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    iterator emplace(const_iterator insert_position, const T1& value1, const T2& value2, const T3& value3)
    {
      iterator position(insert_position.index, *this, p_buffer);

      ETL_ASSERT(!full(), ETL_ERROR(deque_full));

      void* p;

      if (insert_position == begin())
      {
        --_begin;
        p = etl::addressof(*_begin);
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _begin;
      }
      else if (insert_position == end())
      {
        p = etl::addressof(*_end);
        ++_end;
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _end - 1;
      }
      else
      {
        // Are we closer to the front?
        if (etl::distance(_begin, position) < etl::distance(position, _end - 1))
        {
          // Construct the _begin.
          create_element_front(*_begin);

          // Move the values.
          etl::move(_begin + 1, position, _begin);

          // Write the new value.
          --position;
          (*position).~T();
          p = etl::addressof(*position);
        }
        else
        {
          // Construct the _end.
          create_element_back(*(_end - 1));

          // Move the values.
          etl::move_backward(position, _end - 2, _end - 1);

          // Write the new value.
          (*position).~T();
          p = etl::addressof(*position);
        }
      }

      ::new (p) T(value1, value2, value3);

      return position;
    }

    //*************************************************************************
    /// Emplaces data into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is full.
    ///\param insert_position>The insert position.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    iterator emplace(const_iterator insert_position, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      iterator position(insert_position.index, *this, p_buffer);

      ETL_ASSERT(!full(), ETL_ERROR(deque_full));

      void* p;

      if (insert_position == begin())
      {
        --_begin;
        p = etl::addressof(*_begin);
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _begin;
      }
      else if (insert_position == end())
      {
        p = etl::addressof(*_end);
        ++_end;
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
          position = _end - 1;
      }
      else
      {
        // Are we closer to the front?
        if (etl::distance(_begin, position) < etl::distance(position, _end - 1))
        {
          // Construct the _begin.
          create_element_front(*_begin);

          // Move the values.
          etl::move(_begin + 1, position, _begin);

          // Write the new value.
          --position;
          (*position).~T();
          p = etl::addressof(*position);
        }
        else
        {
          // Construct the _end.
          create_element_back(*(_end - 1));

          // Move the values.
          etl::move_backward(position, _end - 2, _end - 1);

          // Write the new value.
          (*position).~T();
          p = etl::addressof(*position);
        }
      }

      ::new (p) T(value1, value2, value3, value4);

      return position;
    }
#endif

    //*************************************************************************
    /// Inserts 'n' copies of a value into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is full.
    ///\param insert_position The insert position.
    ///\param n               The number of values to insert.
    ///\param value           The value to insert.
    //*************************************************************************
    iterator insert(const_iterator insert_position, size_type n, const value_type& value)
    {
      iterator position;

      ETL_ASSERT((current_size + n) <= CAPACITY, ETL_ERROR(deque_full));

      if (insert_position == begin())
      {
        for (size_t i = 0UL; i < n; ++i)
        {
          create_element_front(value);
        }

        position = _begin;
      }
      else if (insert_position == end())
      {
        for (size_t i = 0UL; i < n; ++i)
        {
          create_element_back(value);
        }

        position = _end - n;
      }
      else
      {
        // Non-const insert iterator.
        position = iterator(insert_position.index, *this, p_buffer);

        // Are we closer to the front?
        if (distance(_begin, insert_position) <= difference_type(current_size / 2))
        {
          size_t n_insert = n;
          size_t n_move = etl::distance(begin(), position);
          size_t n_create_copy = etl::min(n_insert, n_move);
          size_t n_create_new = (n_insert > n_create_copy) ? n_insert - n_create_copy : 0;
          size_t n_copy_new = (n_insert > n_create_new) ? n_insert - n_create_new : 0;
          size_t n_copy_old = n_move - n_create_copy;

          // Remember the original start.
          iterator from = _begin + n_create_copy - 1;
          iterator to;

          // Create new.
          for (size_t i = 0UL; i < n_create_new; ++i)
          {
            create_element_front(value);
          }

          // Create copy.
          for (size_t i = 0UL; i < n_create_copy; ++i)
          {
            create_element_front(*from);
            --from;
          }

          // Move old.
          from = position - n_copy_old;
          to = _begin + n_create_copy;
          etl::move(from, from + n_copy_old, to);

          // Copy new.
          to = position - n_create_copy;
          etl::fill_n(to, n_copy_new, value);

          position = _begin + n_move;
        }
        else
        {
          size_t n_insert = n;
          size_t n_move = etl::distance(position, end());
          size_t n_create_copy = etl::min(n_insert, n_move);
          size_t n_create_new = (n_insert > n_create_copy) ? n_insert - n_create_copy : 0;
          size_t n_copy_new = (n_insert > n_create_new) ? n_insert - n_create_new : 0;
          size_t n_copy_old = n_move - n_create_copy;

          // Create new.
          for (size_t i = 0UL; i < n_create_new; ++i)
          {
            create_element_back(value);
          }

          // Create copy.
          const_iterator from = position + n_copy_old;

          for (size_t i = 0UL; i < n_create_copy; ++i)
          {
            create_element_back(*from);
            ++from;
          }

          // Move old.
          etl::move_backward(position, position + n_copy_old, position + n_insert + n_copy_old);

          // Copy new.
          etl::fill_n(position, n_copy_new, value);
        }
      }

      return position;
    }

    //*************************************************************************
    /// Inserts a range into the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_empty if the deque is full.
    ///\param insert_position>The insert position.
    ///\param range_begin The beginning of the range to insert.
    ///\param range_end   The end of the range to insert.
    //*************************************************************************
    template<typename TIterator>
    typename enable_if<!etl::is_integral<TIterator>::value, iterator>::type
      insert(const_iterator insert_position, TIterator range_begin, TIterator range_end)
    {
      iterator position;

      difference_type n = etl::distance(range_begin, range_end);

      ETL_ASSERT((current_size + n) <= CAPACITY, ETL_ERROR(deque_full));

      if (insert_position == begin())
      {
        create_element_front(n, range_begin);

        position = _begin;
      }
      else if (insert_position == end())
      {
        for (difference_type i = 0; i < n; ++i)
        {
          create_element_back(*range_begin);
          ++range_begin;
        }

        position = _end - n;
      }
      else
      {
        // Non-const insert iterator.
        position = iterator(insert_position.index, *this, p_buffer);

        // Are we closer to the front?
        if (distance(_begin, insert_position) < difference_type(current_size / 2))
        {
          size_t n_insert = n;
          size_t n_move = etl::distance(begin(), position);
          size_t n_create_copy = etl::min(n_insert, n_move);
          size_t n_create_new = (n_insert > n_create_copy) ? n_insert - n_create_copy : 0;
          size_t n_copy_new = (n_insert > n_create_new) ? n_insert - n_create_new : 0;
          size_t n_copy_old = n_move - n_create_copy;

          // Remember the original start.
          iterator from;
          iterator to;

          // Create new.
          create_element_front(n_create_new, range_begin);

          // Create copy.
          create_element_front(n_create_copy, _begin + n_create_new);

          // Move old.
          from = position - n_copy_old;
          to = _begin + n_create_copy;
          etl::move(from, from + n_copy_old, to);

          // Copy new.
          to = position - n_create_copy;
          range_begin += n_create_new;
          etl::copy(range_begin, range_begin + n_copy_new, to);

          position = _begin + n_move;
        }
        else
        {
          size_t n_insert = n;
          size_t n_move = etl::distance(position, end());
          size_t n_create_copy = etl::min(n_insert, n_move);
          size_t n_create_new = (n_insert > n_create_copy) ? n_insert - n_create_copy : 0;
          size_t n_copy_new = (n_insert > n_create_new) ? n_insert - n_create_new : 0;
          size_t n_copy_old = n_move - n_create_copy;

          // Create new.
          TIterator item = range_begin + (n - n_create_new);
          for (size_t i = 0UL; i < n_create_new; ++i)
          {
            create_element_back(*item);
            ++item;
          }

          // Create copy.
          const_iterator from = position + n_copy_old;

          for (size_t i = 0UL; i < n_create_copy; ++i)
          {
            create_element_back(*from);
            ++from;
          }

          // Move old.
          etl::move_backward(position, position + n_copy_old, position + n_insert + n_copy_old);

          // Copy new.
          item = range_begin;
          etl::copy(item, item + n_copy_new, position);
        }
      }

      return position;
    }

    //*************************************************************************
    /// Erase an item.
    /// If asserts or exceptions are enabled, throws an etl::deque_out_of_bounds if the position is out of range.
    ///\param erase_position The position to erase.
    //*************************************************************************
    iterator erase(const_iterator erase_position)
    {
      iterator position(to_iterator(erase_position));
      //iterator position(erase_position.index, *this, p_buffer);

      ETL_ASSERT(distance(position) <= difference_type(current_size), ETL_ERROR(deque_out_of_bounds));

      if (position == _begin)
      {
        destroy_element_front();
        position = begin();
      }
      else if (position == _end - 1)
      {
        destroy_element_back();
        position = end();
      }
      else
      {
        // Are we closer to the front?
        if (distance(_begin, position) < difference_type(current_size / 2))
        {
          etl::move_backward(_begin, position, position + 1);
          destroy_element_front();
          ++position;
        }
        else
        {
          etl::move(position + 1, _end, position);
          destroy_element_back();
        }
      }

      return position;
    }

    //*************************************************************************
    /// erase a range.
    /// If asserts or exceptions are enabled, throws an etl::deque_out_of_bounds if the iterators are out of range.
    ///\param range_begin The beginning of the range to erase.
    ///\param range_end   The end of the range to erase.
    //*************************************************************************
    iterator erase(const_iterator range_begin, const_iterator range_end)
    {
      iterator position(to_iterator(range_begin));

      ETL_ASSERT((distance(range_begin) <= difference_type(current_size)) && (distance(range_end) <= difference_type(current_size)), ETL_ERROR(deque_out_of_bounds));

      // How many to erase?
      size_t length = etl::distance(range_begin, range_end);

      // At the beginning?
      if (position == _begin)
      {
        for (size_t i = 0UL; i < length; ++i)
        {
          destroy_element_front();
        }

        position = begin();
      }
      // At the end?
      else if (position == _end - length)
      {
        for (size_t i = 0UL; i < length; ++i)
        {
          destroy_element_back();
        }

        position = end();
      }
      else
      {
        // Copy the smallest number of items.
        // Are we closer to the front?
        if (distance(_begin, position) < difference_type(current_size / 2))
        {
          // Move the items.
          etl::move_backward(_begin, position, position + length);

          for (size_t i = 0UL; i < length; ++i)
          {
            destroy_element_front();
          }

          position += length;
        }
        else
          // Must be closer to the back.
        {
          // Move the items.
          etl::move(position + length, _end, position);

          for (size_t i = 0UL; i < length; ++i)
          {
            destroy_element_back();
          }
        }
      }

      return position;
    }

    //*************************************************************************
    /// Adds an item to the back of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    ///\param item The item to push to the deque.
    //*************************************************************************
    void push_back(const_reference item)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif
      create_element_back(item);
    }

#if __cpp_rvalue_references
    //*************************************************************************
    /// Adds an item to the back of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    ///\param item The item to push to the deque.
    //*************************************************************************
    void push_back(rvalue_reference item)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif
      create_element_back(etl::move(item));
    }
#endif

#if __cpp_rvalue_references && __cpp_variadic_templates && ETL_NOT_USING_STLPORT
    //*************************************************************************
    /// Emplaces an item to the back of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename ... Args>
    void emplace_back(Args && ... args)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      ::new (&(*_end)) T(etl::forward<Args>(args)...);
      ++_end;
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

#else

    //*************************************************************************
    /// Emplaces an item to the back of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename T1>
    void emplace_back(const T1& value1)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      ::new (&(*_end)) T(value1);
      ++_end;
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// Emplaces an item to the back of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename T1, typename T2>
    void emplace_back(const T1& value1, const T2& value2)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      ::new (&(*_end)) T(value1, value2);
      ++_end;
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// Emplaces an item to the back of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    void emplace_back(const T1& value1, const T2& value2, const T3& value3)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      ::new (&(*_end)) T(value1, value2, value3);
      ++_end;
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// Emplaces an item to the back of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    void emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      ::new (&(*_end)) T(value1, value2, value3, value4);
      ++_end;
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }
#endif

    //*************************************************************************
    /// Removes the oldest item from the deque.
    //*************************************************************************
    void pop_back()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(deque_empty));
#endif
      destroy_element_back();
    }

    //*************************************************************************
    /// Adds an item to the front of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    ///\param item The item to push to the deque.
    //*************************************************************************
    void push_front(const_reference item)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif
      create_element_front(item);
    }

#if __cpp_rvalue_references
    //*************************************************************************
    /// Adds an item to the front of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    ///\param item The item to push to the deque.
    //*************************************************************************
    void push_front(rvalue_reference item)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif
      create_element_front(etl::move(item));
    }
#endif

#if __cpp_rvalue_references && __cpp_variadic_templates && ETL_NOT_USING_STLPORT
    //*************************************************************************
    /// Emplaces an item to the front of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename ... Args>
    void emplace_front(Args && ... args)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      --_begin;
      ::new (&(*_begin)) T(etl::forward<Args>(args)...);
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

#else

    //*************************************************************************
    /// Emplaces an item to the front of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename T1>
    void emplace_front(const T1& value1)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      --_begin;
      ::new (&(*_begin)) T(value1);
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// Emplaces an item to the front of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename T1, typename T2>
    void emplace_front(const T1& value1, const T2& value2)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      --_begin;
      ::new (&(*_begin)) T(value1, value2);
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// Emplaces an item to the front of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3>
    void emplace_front(const T1& value1, const T2& value2, const T3& value3)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      --_begin;
      ::new (&(*_begin)) T(value1, value2, value3);
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// Emplaces an item to the front of the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full if the deque is already full.
    //*************************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    void emplace_front(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!full(), ETL_ERROR(deque_full));
#endif

      --_begin;
      ::new (&(*_begin)) T(value1, value2, value3, value4);
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }
#endif

    //*************************************************************************
    /// Removes the oldest item from the deque.
    //*************************************************************************
    void pop_front()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(!empty(), ETL_ERROR(deque_empty));
#endif
      destroy_element_front();
    }

    //*************************************************************************
    /// Resizes the deque.
    /// If asserts or exceptions are enabled, throws an etl::deque_full is 'new_size' is too large.
    ///\param new_size The new size of the deque.
    ///\param value   The value to assign if the new size is larger. Default = Default constructed value.
    //*************************************************************************
    void resize(size_t new_size, const value_type& value = value_type())
    {
      ETL_ASSERT(new_size <= CAPACITY, ETL_ERROR(deque_full));

      // Make it smaller?
      if (new_size < current_size)
      {
        while (current_size > new_size)
        {
          destroy_element_back();
        }
      }
      // Make it larger?
      else if (new_size > current_size)
      {
        size_t count = new_size - current_size;

        for (size_t i = 0UL; i < count; ++i)
        {
          create_element_back(value);
        }
      }
    }

    //*************************************************************************
    /// - operator for iterator
    //*************************************************************************
    friend difference_type operator -(const iterator& lhs, const iterator& rhs)
    {
      return distance(rhs, lhs);
    }

    //*************************************************************************
    /// - operator for const_iterator
    //*************************************************************************
    friend difference_type operator -(const const_iterator& lhs, const const_iterator& rhs)
    {
      return distance(rhs, lhs);
    }

    //*************************************************************************
    /// - operator for reverse_iterator
    //*************************************************************************
    friend difference_type operator -(const reverse_iterator& lhs, const reverse_iterator& rhs)
    {
      return distance(lhs.base(), rhs.base());
    }

    //*************************************************************************
    /// - operator for const_reverse_iterator
    //*************************************************************************
    friend difference_type operator -(const const_reverse_iterator& lhs, const const_reverse_iterator& rhs)
    {
      return distance(lhs.base(), rhs.base());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ideque& operator =(const ideque& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.begin(), rhs.end());
      }

      return *this;
    }

#if __cpp_rvalue_references
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    ideque& operator =(ideque&& rhs)
    {
      if (&rhs != this)
      {
        clear();
        iterator itr = rhs.begin();
        while (itr != rhs.end())
        {
          push_back(etl::move(*itr));
          ++itr;
        }

        rhs.initialise();
      }

      return *this;
    }
#endif

#ifdef ETL_IDEQUE_REPAIR_ENABLE
    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    virtual void repair() = 0;
#endif

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    ideque(pointer p_buffer_, size_t max_size_, size_t buffer_size_)
      : deque_base(max_size_, buffer_size_),
      p_buffer(p_buffer_)
    {
    }

    //*********************************************************************
    /// Initialise the deque.
    //*********************************************************************
    void initialise()
    {
      if ETL_IF_CONSTEXPR(etl::is_trivially_destructible<T>::value)
      {
        current_size = 0;
        ETL_RESET_DEBUG_COUNT
      }
      else
      {
        while (current_size > 0)
        {
          destroy_element_back();
        }
      }

      _begin = iterator(0, *this, p_buffer);
      _end = iterator(0, *this, p_buffer);
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair_buffer(pointer p_buffer_)
    {
      p_buffer = p_buffer_;

      _begin = iterator(_begin.index, *this, p_buffer);
      _end = iterator(_end.index, *this, p_buffer);
    }

    iterator _begin;   ///Iterator to the _begin item in the deque.
    iterator _end;     ///Iterator to the _end item in the deque.
    pointer  p_buffer; ///The buffer for the deque.

  private:

    //*********************************************************************
    /// Create a new element with a default value at the front.
    //*********************************************************************
    void create_element_front()
    {
      --_begin;
      ::new (&(*_begin)) T();
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*********************************************************************
    /// Create a new elements from a range at the front.
    //*********************************************************************
    template <typename TIterator>
    void create_element_front(size_t n, TIterator from)
    {
      if (n == 0)
      {
        return;
      }

      _begin -= n;

      iterator item = _begin;

      do
      {
        ::new (&(*item)) T(*from);
        ++item;
        ++from;
        ++current_size;
        ETL_INCREMENT_DEBUG_COUNT
      } while (--n != 0);
    }

    //*********************************************************************
    /// Create a new element with a default value at the back.
    //*********************************************************************
    void create_element_back()
    {
      ::new (&(*_end)) T();
      ++_end;
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*********************************************************************
    /// Create a new element with a default value at the front.
    //*********************************************************************
    void create_element_front(const_reference value)
    {
      --_begin;
      ::new (&(*_begin)) T(value);
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*********************************************************************
    /// Create a new element with a value at the back
    //*********************************************************************
    void create_element_back(const_reference value)
    {
      ::new (&(*_end)) T(value);
      ++_end;
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

#if __cpp_rvalue_references
    //*********************************************************************
    /// Create a new element with a default value at the front.
    //*********************************************************************
    void create_element_front(rvalue_reference value)
    {
      --_begin;
      ::new (&(*_begin)) T(etl::move(value));
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*********************************************************************
    /// Create a new element with a value at the back
    //*********************************************************************
    void create_element_back(rvalue_reference value)
    {
      ::new (&(*_end)) T(etl::move(value));
      ++_end;
      ++current_size;
      ETL_INCREMENT_DEBUG_COUNT
    }
#endif

    //*********************************************************************
    /// Destroy an element at the front.
    //*********************************************************************
    void destroy_element_front()
    {
      (*_begin).~T();
      --current_size;
      ETL_DECREMENT_DEBUG_COUNT
        ++_begin;
    }

    //*********************************************************************
    /// Destroy an element at the back.
    //*********************************************************************
    void destroy_element_back()
    {
      --_end;
      (*_end).~T();
      --current_size;
      ETL_DECREMENT_DEBUG_COUNT
    }

    //*************************************************************************
    /// Measures the distance between two iterators.
    //*************************************************************************
    template <typename TIterator1, typename TIterator2>
    static difference_type distance(const TIterator1& range_begin, const TIterator2& range_end)
    {
      difference_type distance1 = distance(range_begin);
      difference_type distance2 = distance(range_end);

      return distance2 - distance1;
    }

    //*************************************************************************
    /// Measures the distance from the _begin iterator to the specified iterator.
    //*************************************************************************
    template <typename TIterator>
    static difference_type distance(const TIterator& other)
    {
      const difference_type index = other.get_index();
      const difference_type reference_index = other.container()._begin.index;
      const size_t buffer_size = other.container().BUFFER_SIZE;

      if (index < reference_index)
      {
        return buffer_size + index - reference_index;
      }
      else
      {
        return index - reference_index;
      }
    }

    //*************************************************************************
    /// Converts from const_iterator to iterator.
    //*************************************************************************
    iterator to_iterator(const_iterator itr) const
    {
      return iterator(itr.index, const_cast<ideque&>(*this), p_buffer);
    }

    // Disable copy construction.
    ideque(const ideque&);

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_DEQUE) || defined(ETL_POLYMORPHIC_CONTAINERS) || defined(ETL_IDEQUE_REPAIR_ENABLE)
  public:
    virtual ~ideque()
    {
    }
#else
  protected:
    ~ideque()
    {
    }
#endif
  };

  //***************************************************************************
  /// A fixed capacity double ended queue.
  ///\note The deque allocates one more element than the specified maximum size.
  ///\tparam T         The type of items this deque holds.
  ///\tparam MAX_SIZE_ The capacity of the deque
  ///\ingroup deque
  //***************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class deque : public etl::ideque<T>
  {
  public:

    static ETL_CONSTANT size_t MAX_SIZE = MAX_SIZE_;

  private:

    static ETL_CONSTANT size_t BUFFER_SIZE = MAX_SIZE + 1;

  public:

    typedef T        value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t   size_type;
    typedef typename etl::iterator_traits<pointer>::difference_type difference_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    deque()
      : etl::ideque<T>(reinterpret_cast<T*>(buffer.raw), MAX_SIZE, BUFFER_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~deque()
    {
      this->initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    deque(const deque& other)
      : etl::ideque<T>(reinterpret_cast<T*>(buffer.raw), MAX_SIZE, BUFFER_SIZE)
    {
      if (this != &other)
      {
        this->assign(other.begin(), other.end());
      }
    }

#if __cpp_rvalue_references
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    deque(deque&& other)
      : etl::ideque<T>(reinterpret_cast<T*>(buffer.raw), MAX_SIZE, BUFFER_SIZE)
    {
      if (this != &other)
      {
        this->initialise();

        typename etl::ideque<T>::iterator itr = other.begin();
        while (itr != other.end())
        {
          this->push_back(etl::move(*itr));
          ++itr;
        }
      }
    }
#endif

    //*************************************************************************
    /// Assigns data to the deque.
    //*************************************************************************
    template <typename TIterator>
    deque(TIterator begin_, TIterator end_, typename etl::enable_if<!etl::is_integral<TIterator>::value, int>::type = 0)
      : etl::ideque<T>(reinterpret_cast<T*>(buffer.raw), MAX_SIZE, BUFFER_SIZE)
    {
      this->assign(begin_, end_);
    }

    //*************************************************************************
    /// Assigns data to the deque.
    //*************************************************************************
    explicit deque(size_t n, const_reference value = value_type())
      : etl::ideque<T>(reinterpret_cast<T*>(buffer.raw), MAX_SIZE, BUFFER_SIZE)
    {
      this->assign(n, value);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    /// Construct from initializer_list.
    //*************************************************************************
    deque(std::initializer_list<T> init)
      : ideque<T>(reinterpret_cast<T*>(buffer.raw), MAX_SIZE, BUFFER_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    deque& operator =(const deque& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.begin(), rhs.end());
      }

      return *this;
    }

#if __cpp_rvalue_references
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    deque& operator =(deque&& rhs)
    {
      if (&rhs != this)
      {
        this->clear();
        typename etl::ideque<T>::iterator itr = rhs.begin();
        while (itr != rhs.end())
        {
          this->push_back(etl::move(*itr));
          ++itr;
        }
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
#ifdef ETL_IDEQUE_REPAIR_ENABLE
      virtual
#endif
      void repair()
#ifdef ETL_IDEQUE_REPAIR_ENABLE
      ETL_OVERRIDE
#endif
    {
#if ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED
      ETL_ASSERT(etl::is_trivially_copyable<T>::value, ETL_ERROR(etl::deque_incompatible_type));
#endif

      etl::ideque<T>::repair_buffer(reinterpret_cast<T*>(buffer.raw));
    }

  private:

    /// The uninitialised buffer of T used in the deque.
    etl::uninitialized_buffer_of<T, BUFFER_SIZE> buffer;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if __cpp_deduction_guides && ETL_HAS_INITIALIZER_LIST
  template <typename... T>
  deque(T...) -> deque<typename etl::common_type_t<T...>, sizeof...(T)>;
#endif

  //*************************************************************************
  /// Make
  //*************************************************************************
#if __cpp_rvalue_references && ETL_HAS_INITIALIZER_LIST
  template <typename T, typename... TValues>
  constexpr auto make_deque(TValues&&... values) -> etl::deque<T, sizeof...(TValues)>
  {
    return { { etl::forward<T>(values)... } };
  }
#endif

  //***************************************************************************
  /// Equal operator.
  ///\param lhs  Reference to the _begin deque.
  ///\param rhs  Reference to the second deque.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup deque
  //***************************************************************************
  template <typename T>
  bool operator ==(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs  Reference to the _begin deque.
  ///\param rhs  Reference to the second deque.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup deque
  //***************************************************************************
  template <typename T>
  bool operator !=(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs  Reference to the _begin deque.
  ///\param rhs  Reference to the second deque.
  ///\return <b>true</b> if the _begin deque is lexicographically less than the second, otherwise <b>false</b>
  ///\ingroup deque
  //***************************************************************************
  template <typename T>
  bool operator <(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
  {
    return etl::lexicographical_compare(lhs.begin(),
      lhs.end(),
      rhs.begin(),
      rhs.end());
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs  Reference to the _begin deque.
  ///\param rhs  Reference to the second deque.
  ///\return <b>true</b> if the _begin deque is lexicographically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup deque
  //***************************************************************************
  template <typename T>
  bool operator <=(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
  {
    return !(lhs > rhs);
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs  Reference to the _begin deque.
  ///\param rhs  Reference to the second deque.
  ///\return <b>true</b> if the _begin deque is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup deque
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param "lhs  Reference to the _begin deque.
  ///\param "rhs  Reference to the second deque.
  ///\return <b>true</b> if the _begin deque is lexicographically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup deque
  //***************************************************************************
  template <typename T>
  bool operator >=(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
  {
    return !(lhs < rhs);
  }
}

#include "private/minmax_pop.h"

#endif
