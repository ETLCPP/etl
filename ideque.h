///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#ifndef __ETL_IDEQUE__
#define __ETL_IDEQUE__
#define __ETL_IN_IDEQUE_H__

#include <stddef.h>
#include <iterator>

#include "algorithm.h"
#include "type_traits.h"
#include "deque_base.h"
#include "parameter_type.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  /// The base class for all etl::deque classes.
  ///\tparam T The type of values this deque should hold.
  ///\ingroup deque
  //***************************************************************************
  template <typename T>
  class ideque : public deque_base
  {
  public:

    typedef T        value_type;
    typedef size_t   size_type;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef typename std::iterator_traits<pointer>::difference_type difference_type;

  protected:

    typedef typename parameter_type<T>::type parameter_t;

    //*************************************************************************
    /// Test for an iterator.
    //*************************************************************************
    template <typename TIterator>
    struct is_iterator : public etl::integral_constant<bool, !etl::is_integral<TIterator>::value && !etl::is_floating_point<TIterator>::value>
    {
    };

  public:

    //*************************************************************************
    /// Iterator
    //*************************************************************************
    struct iterator : public std::iterator<std::random_access_iterator_tag, T>
    {
      friend class ideque;

      //***************************************************
      iterator()
      : index(0),
        p_deque(0),
        p_buffer(0)
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
      iterator& operator ++()
      {
        index = (index == p_deque->BUFFER_SIZE - 1) ? 0 : index + 1;

        return *this;
      }

      //***************************************************
      iterator operator ++(int)
      {
        iterator previous(*this);
        index = (index == p_deque->BUFFER_SIZE - 1) ? 0 : index + 1;

        return previous;
      }

      //***************************************************
      iterator operator +=(difference_type offset)
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
      iterator operator -=(difference_type offset)
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
      reference operator *()
      {
        return p_buffer[index];
      }

      //***************************************************
      const_reference operator *() const
      {
        return p_buffer[index];
      }

      //***************************************************
      pointer operator ->()
      {
        return &p_buffer[index];
      }

      //***************************************************
      const_pointer operator ->() const
      {
        return &p_buffer[index];
      }

      //***************************************************
      bool operator <(const iterator& other) const
      {
        return ideque::distance(*this, other) > 0;
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
      difference_type get_index() const
      {
        return index;
      }

      //***************************************************
      ideque& get_deque() const
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
        std::swap(index, other.index);
      }

    private:

      //***************************************************
      iterator(difference_type index, ideque& the_deque, pointer p_buffer)
        : index(index),
          p_deque(&the_deque),
          p_buffer(p_buffer)
      {
      }

      difference_type index;
      ideque*         p_deque;
      pointer         p_buffer;
    };

    //*************************************************************************
    /// Const Iterator
    //*************************************************************************
    struct const_iterator : public std::iterator<std::random_access_iterator_tag, const T>
    {
      friend class ideque;

      //***************************************************
      const_iterator()
        : index(0),
          p_deque(0),        
          p_buffer(0)
      {
      }

      //***************************************************
      const_iterator(const const_iterator& other)
        : index(other.index),
          p_deque(other.p_deque),
          p_buffer(other.p_buffer)
      {
      }

      //***************************************************
      const_iterator(const typename ideque::iterator& other)
        : index(other.index),
          p_deque(other.p_deque),
          p_buffer(other.p_buffer)
      {
      }

      //***************************************************
      const_iterator& operator ++()
      {
        index = (index == p_deque->BUFFER_SIZE - 1) ? 0 : index + 1;

        return *this;
      }

      //***************************************************
      const_iterator operator ++(int)
      {
        const_iterator previous(*this);
        index = (index == p_deque->BUFFER_SIZE - 1) ? 0 : index + 1;

        return previous;
      }

      //***************************************************
      const_iterator operator +=(difference_type offset)
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
      const_iterator operator -=(difference_type offset)
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
      bool operator <(const const_iterator& other) const
      {
        return ideque::distance(*this, other) > 0;
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
      difference_type get_index() const
      {
        return index;
      }

      //***************************************************
      ideque& get_deque() const
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
        std::swap(index, other.index);
      }

    private:

      //***************************************************
      difference_type distance(difference_type firstIndex, difference_type index)
      {
        if (index < firstIndex)
        {
          return p_deque->BUFFER_SIZE + index - firstIndex;
        }
        else
        {
          return index - firstIndex;
        }
      }

      //***************************************************
      const_iterator(difference_type index, ideque& the_deque, pointer p_buffer)
        : index(index),
          p_deque(&the_deque),
          p_buffer(p_buffer)
      {
      }

      difference_type index;
      ideque*         p_deque;
      pointer         p_buffer;
    };

    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ideque& operator =(const ideque& other)
    {
      assign(other.begin(), other.end());

      return *this;
    }

    //*************************************************************************
    /// Assigns a range to the deque.
    //*************************************************************************
    template<typename TIterator>
    typename etl::enable_if<is_iterator<TIterator>::value, void>::type
      assign(TIterator range_begin, TIterator range_end)
    {
      initialise();

      while (range_begin != range_end)
      {
        push_back(*range_begin++);
      }
    }

    //*************************************************************************
    /// Assigns 'n' copies of a value to the deque.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_full is 'n' is too large.
    ///\param n     The number of copies to assign.
    ///\param value The value to add.<
    //*************************************************************************
    void assign(size_type n, const value_type& value)
    {
      if (n > MAX_SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS 
        throw deque_full();
#else
        error_handler::error(deque_full());
#endif
      }

      initialise();

      first.index = 0;
      last.index  = 0;

      while (n > 0)
      {
        create_element_back(value);
        --n;
      }
    }

    //*************************************************************************
    /// Gets a reference to the item at the index.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_out_of_bounds if the index is out of range.
    ///\return A reference to the item at the index.
    //*************************************************************************
    reference at(size_t index)
    {
      if (index >= current_size)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_out_of_bounds();
#else
        error_handler::error((deque_out_of_bounds()));
#endif
      }
      
      iterator result(first);
      result += index;

      return *result;
    }

    //*************************************************************************
    /// Gets a const reference to the item at the index.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_out_of_bounds if the index is out of range.
    ///\return A const reference to the item at the index.
    //*************************************************************************
    const_reference at(size_t index) const
    {
      if (index >= current_size)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_out_of_bounds();
#else
        error_handler::error((deque_out_of_bounds()));
#endif
      }

      iterator result(first);
      result += index;

      return *result;
    }
    
    //*************************************************************************
    /// Gets a reference to the item at the index.
    ///\return A reference to the item at the index.
    //*************************************************************************
    reference operator [](size_t index)
    {
      iterator result(first);
      result += index;

      return *result;
    }

    //*************************************************************************
    /// Gets a const reference to the item at the index.
    ///\return A const reference to the item at the index.
    //*************************************************************************
    const_reference operator [](size_t index) const
    {
      iterator result(first);
      result += index;

      return *result;
    }

    //*************************************************************************
    /// Gets a reference to the item at the front of the deque.
    ///\return A reference to the item at the front of the deque.
    //*************************************************************************
    reference front()
    {
      return *first;
    }

    //*************************************************************************
    /// Gets a const reference to the item at the front of the deque.
    ///\return A const reference to the item at the front of the deque.
    //*************************************************************************
    const_reference front() const
    {
      return *first;
    }

    //*************************************************************************
    /// Gets a reference to the item at the back of the deque.
    ///\return A reference to the item at the back of the deque.
    //*************************************************************************
    reference back()
    {
      return *last;
    }

    //*************************************************************************
    /// Gets a const reference to the item at the back of the deque.
    ///\return A const reference to the item at the back of the deque.
    //*************************************************************************
    const_reference back() const
    {
      return *last;
    }

    //*************************************************************************
    /// Gets an iterator to the beginning of the deque.
    //*************************************************************************
    iterator begin()
    {
      return first;
    }

    //*************************************************************************
    /// Gets a const iterator to the beginning of the deque.
    //*************************************************************************
    const_iterator begin() const
    {
      return first;
    }
    
    //*************************************************************************
    /// Gets a const iterator to the beginning of the deque.
    //*************************************************************************
    const_iterator cbegin() const
    {
      return first;
    }

    //*************************************************************************
    /// Gets an iterator to the end of the deque.
    //*************************************************************************
    iterator end()
    {
      return ++iterator(last);
    }

    //*************************************************************************
    /// Gets a const iterator to the end of the deque.
    //*************************************************************************
    const_iterator end() const
    {
      return ++iterator(last);
    }

    //*************************************************************************
    /// Gets a const iterator to the end of the deque.
    //*************************************************************************
    const_iterator cend() const
    {
      return ++const_iterator(last);
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
    /// Inserts data into the deque.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_full if the deque is full.
    ///\param insert_position>The insert position.
    ///\param value>The value to insert.
    //*************************************************************************
    iterator insert(const_iterator insert_position, const value_type& value)
    {
      iterator position(insert_position.index, *this, p_buffer);

      if (!full())
      {
        if (insert_position == begin())
        {
          create_element_front(value);
          position = first;
        }
        else if (insert_position == end())
        {
          create_element_back(value);
          position = last;
        }
        else
        {
          // Are we closer to the front?
          if (std::distance(first, position) < std::distance(position, last))
          {
            // Construct the first.
            create_element_front(*first);

            // Move the values.
            std::copy(first + 1, position, first);

            // Write the new value.
            *--position = value;
          }
          else
          {
            // Construct the last.
            create_element_back(*last);
            
            // Move the values.
            std::copy_backward(position, last - 1, last);

            // Write the new value.
            *position = value;
          }
        }
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_full();
#else
        error_handler::error(deque_full());
#endif
      }

      return position;
    }

    //*************************************************************************
    /// Inserts 'n' copies of a value into the deque.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_full if the deque is full.
    ///\param insert_position The insert position.
    ///\param n               The number of values to insert.
    ///\param value           The value to insert.
    //*************************************************************************
    iterator insert(const_iterator insert_position, size_type n, const value_type& value)
    {
      iterator position;

      if ((current_size + n) <= MAX_SIZE)
      {
        if (insert_position == begin())
        {
          for (size_t i = 0; i < n; ++i)
          {
            create_element_front(value);
          }
          
          position = first;
        }
        else if (insert_position == end())
        {
          for (size_t i = 0; i < n; ++i)
          {
            create_element_back(value);
          }

          position = last - (n - 1);
        }
        else
        {
          // Non-const insert iterator.
          position = iterator(insert_position.index, *this, p_buffer);

          // Are we closer to the front?
          if (distance(first, insert_position) <= difference_type(current_size / 2))
          {
            size_t insert_index  = std::distance(begin(), position);
            size_t n_insert      = n;
            size_t n_move        = std::distance(begin(), position);
            size_t n_create_copy = std::min(n_insert, n_move);
            size_t n_create_new  = (n_insert > n_create_copy) ? n_insert - n_create_copy : 0;
            size_t n_copy_new    = (n_insert > n_create_new) ? n_insert - n_create_new : 0;
            size_t n_copy_old    = n_move - n_create_copy;

            // Remember the original start.
            iterator from = first + n_create_copy - 1;
            iterator to;

            // Create new.
            for (size_t i = 0; i < n_create_new; ++i)
            {
              create_element_front(value);
            }

            // Create copy.
            for (size_t i = 0; i < n_create_copy; ++i)
            {
              create_element_front(*from--);
            }

            // Copy old.
            from = position - n_copy_old;
            to   = first + n_create_copy;
            etl::copy_n(from, n_copy_old, to);

            // Copy new.
            to = position - n_create_copy;
            std::fill_n(to, n_copy_new, value);

            position = first + n_move;
          }
          else
          {
            size_t insert_index  = std::distance(begin(), position);
            size_t n_insert      = n;
            size_t n_move        = std::distance(position, end());
            size_t n_create_copy = std::min(n_insert, n_move);
            size_t n_create_new  = (n_insert > n_create_copy) ? n_insert - n_create_copy : 0;
            size_t n_copy_new    = (n_insert > n_create_new) ? n_insert - n_create_new : 0;
            size_t n_copy_old    = n_move - n_create_copy;

            // Create new.
            for (size_t i = 0; i < n_create_new; ++i)
            {
              create_element_back(value);
            }

            // Create copy.
            const_iterator from = position + n_copy_old;

            for (size_t i = 0; i < n_create_copy; ++i)
            {
              create_element_back(*from++);
            }

            // Copy old.
            std::copy_backward(position, position + n_copy_old, position + n_insert + n_copy_old);

            // Copy new.
            std::fill_n(position, n_copy_new, value);
          }
        }
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS 
        throw deque_full();
#else
        error_handler::error(deque_full());
#endif
      }

      return position;
    }

    //*************************************************************************
    /// Inserts a range into the deque.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_empty if the deque is full.
    ///\param insert_position>The insert position.
    ///\param range_begin The beginning of the range to insert.
    ///\param range_end   The end of the range to insert.
    //*************************************************************************
    template<typename TIterator>
    typename enable_if<is_iterator<TIterator>::value, iterator>::type
    insert(const_iterator insert_position, TIterator range_begin, TIterator range_end)
    {
      iterator position;

      difference_type n = std::distance(range_begin, range_end);

      if ((current_size + n) <= MAX_SIZE)
      {
        if (insert_position == begin())
        {
          create_element_front(n, range_begin);

          position = first;
        }
        else if (insert_position == end())
        {
          for (difference_type i = 0; i < n; ++i)
          {
            create_element_back(*range_begin++);
          }

          position = last - (n - 1);
        }
        else
        {
          // Non-const insert iterator.
          position = iterator(insert_position.index, *this, p_buffer);

          // Are we closer to the front?
          if (distance(first, insert_position) < difference_type(current_size / 2))
          {
            size_t insert_index  = std::distance(begin(), position);
            size_t n_insert      = n;
            size_t n_move        = std::distance(begin(), position);
            size_t n_create_copy = std::min(n_insert, n_move);
            size_t n_create_new  = (n_insert > n_create_copy) ? n_insert - n_create_copy : 0;
            size_t n_copy_new    = (n_insert > n_create_new) ? n_insert - n_create_new : 0;
            size_t n_copy_old    = n_move - n_create_copy;

            // Remember the original start.
            iterator from;
            iterator to;

            // Create new.
            create_element_front(n_create_new, range_begin);

            // Create copy.
            create_element_front(n_create_copy, first + n_create_new);

            // Copy old.
            from = position - n_copy_old;
            to   = first + n_create_copy;
            etl::copy_n(from, n_copy_old, to);

            // Copy new.
            to = position - n_create_copy;
            range_begin += n_create_new;
            etl::copy_n(range_begin, n_copy_new, to);

            position = first + n_move;
          }
          else
          {
            size_t insert_index  = std::distance(begin(), position);
            size_t n_insert      = n;
            size_t n_move        = std::distance(position, end());
            size_t n_create_copy = std::min(n_insert, n_move);
            size_t n_create_new  = (n_insert > n_create_copy) ? n_insert - n_create_copy : 0;
            size_t n_copy_new    = (n_insert > n_create_new) ? n_insert - n_create_new : 0;
            size_t n_copy_old    = n_move - n_create_copy;

            // Create new.
            TIterator item = range_begin + (n - n_create_new);
            for (size_t i = 0; i < n_create_new; ++i)
            {
              create_element_back(*item++);
            }

            // Create copy.
            const_iterator from = position + n_copy_old;

            for (size_t i = 0; i < n_create_copy; ++i)
            {
              create_element_back(*from++);
            }

            // Copy old.
            std::copy_backward(position, position + n_copy_old, position + n_insert + n_copy_old);

            // Copy new.
            item = range_begin;
            etl::copy_n(item, n_copy_new, position);
          }
        }
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS 
        throw deque_full();
#else
        error_handler::error(deque_full());
#endif
      }

      return position;
    }

    //*************************************************************************
    /// Erase an item.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_out_of_bounds if the position is out of range.
    ///\param erase_position The position to erase.
    //*************************************************************************
    iterator erase(const_iterator erase_position)
    {
      iterator position(erase_position.index, *this, p_buffer);

      if (distance(position) <= difference_type(current_size))
      {
        if (position == first)
        {
          destroy_element_front();
          position = begin();
        }
        else if (position == last)
        {
          destroy_element_back();
          position = end();
        }
        else
        {
          // Are we closer to the front?
          if (distance(first, position) < difference_type(current_size / 2))
          {
            std::copy_backward(first, position, position + 1);
            destroy_element_front();
            ++position;
          }
          else
          {
            std::copy(position + 1, last + 1, position);
            destroy_element_back();
          }
        }
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_out_of_bounds();
#else
        error_handler::error(deque_out_of_bounds());
#endif
      }

      return position;
    }

    //*************************************************************************
    /// erase a range.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_out_of_bounds if the iterators are out of range.
    ///\param range_begin The beginning of the range to erase.
    ///\param range_end   The end of the range to erase.
    //*************************************************************************
    iterator erase(const_iterator range_begin, const_iterator range_end)
    {
      iterator position(range_begin.index, *this, p_buffer);

      if ((distance(range_begin) <= difference_type(current_size)) &&
          (distance(range_end) <= difference_type(current_size)))
      {
        // How many to erase?
        size_t length = std::distance(range_begin, range_end);

        // At the beginning?
        if (position == first)
        {
          for (size_t i = 0; i < length; ++i)
          {
            destroy_element_front();
          }

          position = begin();
        }
        // At the end?
        else if (position == last - length + 1)
        {
          for (size_t i = 0; i < length; ++i)
          {
            destroy_element_back();
          }

          position = end();
        }
        else
        {         
          // Copy the smallest number of items.
          // Are we closer to the front?
          if (distance(first, position) < difference_type(current_size / 2))
          {
            // Move the items.
            std::copy_backward(first, position, position + length);
            
            for (size_t i = 0; i < length; ++i)
            {
              destroy_element_front();
            }

            position += length;
          }
          else
          // Must be closer to the back.
          {
            // Move the items.
            std::copy(position + length, last + 1, position);
            
            for (size_t i = 0; i < length; ++i)
            {
              destroy_element_back();
            }
          }
        }
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_out_of_bounds();
#else
        error_handler::error(deque_out_of_bounds());
#endif
      }

      return position;
    }

    //*************************************************************************
    /// Adds an item to the back of the deque.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_full is the deque is already full.
    ///\param item The item to push to the deque.
    //*************************************************************************
    void push_back(parameter_t item)
    {
      if (!full())
      {
        create_element_back(item);
      } 
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_full();
#else
        error_handler::error(deque_full());
#endif
      }
    }

    //*************************************************************************
    /// Adds one to the front of the deque and returns a reference to the new element.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_full is the deque is already full.
    ///\return A reference to the item to assign to.
    //*************************************************************************
    reference push_back()
    {
      reference r = *last;

      if (!full())
      {
        create_element_back();
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_full();
#else
        error_handler::error(deque_full());
#endif
      }

      return *last;
    }

    //*************************************************************************
    /// Removes the oldest item from the deque.
    //*************************************************************************
    void pop_back()
    {
      if (!empty())
      {
        destroy_element_back();
      }
    }

    //*************************************************************************
    /// Adds an item to the front of the deque.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_full is the deque is already full.
    ///\param item The item to push to the deque.
    //*************************************************************************
    void push_front(parameter_t item)
    {
      if (!full())
      {
        create_element_front(item);
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_full();
#else
        error_handler::error(deque_full());
#endif
      }
    }

    //*************************************************************************
    /// Adds one to the front of the deque and returns a reference to the new element.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_full is the deque is already full.
    ///\return A reference to the item to assign to.
    //*************************************************************************
    reference push_front()
    {
      if (!full())
      {
        create_element_front();
      }
      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_full();
#else
        error_handler::error(deque_full());
#endif
      }

      return *first;
    }

    //*************************************************************************
    /// Removes the oldest item from the deque.
    //*************************************************************************
    void pop_front()
    {
      if (!empty())
      {
        destroy_element_front();
      }
    }

    //*************************************************************************
    /// Resizes the deque.
    /// If ETL_THROW_EXCEPTIONS is defined, throws an etl::deque_full is 'new_size' is too large.
    ///\param new_size The new size of the deque.
    ///\param value   The value to assign if the new size is larger. Default = Default constructed value.
    //*************************************************************************
    void resize(size_t new_size, const value_type& value = value_type())
    {
      if (new_size <= MAX_SIZE)
      {
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

          for (size_t i = 0; i < count; ++i)
          {
            create_element_back(value);
          }
        }
      }

      else
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw deque_out_of_bounds();
#else
        error_handler::error(deque_out_of_bounds());
#endif
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

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    ideque(pointer p_buffer, size_t max_size, size_t buffer_size)
      : deque_base(max_size, buffer_size),
        p_buffer(p_buffer)
    {
      clear();
    }

    iterator first;    ///Iterator to the first item in the deque.
    iterator last;     ///Iterator to the last item in the deque.
    pointer  p_buffer; ///The buffer for the deque.

  private:

    //*********************************************************************
    /// Create a new element with a default value at the front.
    //*********************************************************************
    void create_element_front()
    {
      if (!empty())
      {
        --first;
      }

      new(&(*first)) T();
      ++current_size;
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

      if (!empty())
      {
        --first;
        --n;
      }

      if (n > 0)
      {
        first -= n;
      }

      iterator item = first;

      do
      {
        new(&(*item++)) T(*from);
        ++from;
        ++current_size;
      } while (n-- != 0);
    }

    //*********************************************************************
    /// Create a new element with a default value at the back.
    //*********************************************************************
    void create_element_back()
    {
      if (!empty())
      {
        ++last;
      }

      new(&(*last)) T();
      ++current_size;
    }

    //*********************************************************************
    /// Create a new element with a default value at the front.
    //*********************************************************************
    void create_element_front(parameter_t value)
    {
      if (!empty())
      {
        --first;
      }

      new(&(*first)) T(value);
      ++current_size;
    }

    //*********************************************************************
    /// Create a new element with a value at the back
    //*********************************************************************
    void create_element_back(parameter_t value)
    {
      if (!empty())
      {
        ++last;
      }

      new(&(*last)) T(value);
      ++current_size;
    }

    //*********************************************************************
    /// Destroy an element at the front.
    //*********************************************************************
    void destroy_element_front()
    {
      (*first).~T();
      --current_size;

      if (!empty())
      {
        ++first;
      }
    }

    //*********************************************************************
    /// Destroy an element at the back.
    //*********************************************************************
    void destroy_element_back()
    {
      (*last).~T();
      --last;
      --current_size;
    }

    //*********************************************************************
    /// Initialise the deque.
    //*********************************************************************
    void initialise()
    {
      while (current_size > 0)
      {
        destroy_element_back();
      }

      first = iterator(0, *this, p_buffer);
      last  = iterator(0, *this, p_buffer);
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
    /// Measures the distance from the first iterator to the specified iterator.
    //*************************************************************************
    template <typename TIterator>
    static difference_type distance(const TIterator& other)
    {
      const difference_type index           = other.get_index();
      const difference_type reference_index = other.get_deque().first.index;
      const size_t buffer_size              = other.get_deque().BUFFER_SIZE;

      if (index < reference_index)
      {
        return buffer_size + index - reference_index;
      }
      else
      {
        return index - reference_index;
      }
    }
  };
}

//***************************************************************************
/// Equal operator.
///\param lhs  Reference to the first deque.
///\param rhs  Reference to the second deque.
///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
///\ingroup deque
//***************************************************************************
template <typename T>
bool operator ==(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
{
  return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

//***************************************************************************
/// Not equal operator.
///\param lhs  Reference to the first deque.
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
///\param lhs  Reference to the first deque.
///\param rhs  Reference to the second deque.
///\return <b>true</b> if the first deque is lexigraphically less than the second, otherwise <b>false</b>
///\ingroup deque
//***************************************************************************
template <typename T>
bool operator <(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end());
}

//***************************************************************************
/// Less than or equal operator.
///\param lhs  Reference to the first deque.
///\param rhs  Reference to the second deque.
///\return <b>true</b> if the first deque is lexigraphically less than or equal to the second, otherwise <b>false</b>
///\ingroup deque
//***************************************************************************
template <typename T>
bool operator <=(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
{
  return !operator >(lhs, rhs);
}

//***************************************************************************
/// Greater than operator.
///\param lhs  Reference to the first deque.
///\param rhs  Reference to the second deque.
///\return <b>true</b> if the first deque is lexigraphically greater than the second, otherwise <b>false</b>
///\ingroup deque
//***************************************************************************
template <typename T>
bool operator >(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
{
  return std::lexicographical_compare(lhs.begin(),
                                      lhs.end(),
                                      rhs.begin(),
                                      rhs.end(),
                                      std::greater<T>());
}

//***************************************************************************
/// Greater than or equal operator.
///\param "lhs  Reference to the first deque.
///\param "rhs  Reference to the second deque.
///\return <b>true</b> if the first deque is lexigraphically greater than or equal to the second, otherwise <b>false</b>
///\ingroup deque
//***************************************************************************
template <typename T>
bool operator >=(const etl::ideque<T>& lhs, const etl::ideque<T>& rhs)
{
  return !operator <(lhs, rhs);
}

#undef __ETL_IN_IDEQUE_H__

#endif
