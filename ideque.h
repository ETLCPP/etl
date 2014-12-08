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

#ifndef __ETL_IDEQUE__
#define __ETL_IDEQUE__
#define __ETL_IN_IDEQUE_H__

#include <stddef.h>
#include <iterator>

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
        index = (index == p_deque->MAX_SIZE) ? 0 : index + 1;

        return *this;
      }

      //***************************************************
      iterator operator ++(int)
      {
        iterator previous(*this);
        index = (index == p_deque->MAX_SIZE) ? 0 : index + 1;

        return previous;
      }

      //***************************************************
      iterator operator +=(difference_type offset)
      {
        if (offset > 0)
        {
          index += offset;
          index = (static_cast<size_t>(index) > p_deque->MAX_SIZE) ? index - p_deque->BUFFER_SIZE : index;
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
        index = (index == 0) ? p_deque->MAX_SIZE : index - 1;

        return *this;
      }

      //***************************************************
      iterator operator --(int)
      {
        iterator previous(*this);
        index = (index == 0) ? p_deque->MAX_SIZE : index - 1;

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
        index = (index == p_deque->MAX_SIZE) ? 0 : index + 1;

        return *this;
      }

      //***************************************************
      const_iterator operator ++(int)
      {
        const_iterator previous(*this);
        index = (index == p_deque->MAX_SIZE) ? 0 : index + 1;

        return previous;
      }

      //***************************************************
      const_iterator operator +=(difference_type offset)
      {
        if (offset > 0)
        {
          index += offset;
          index = (static_cast<size_t>(index) > p_deque->MAX_SIZE) ? index - p_deque->BUFFER_SIZE : index;
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
        index = (index == 0) ? p_deque->MAX_SIZE : index - 1;

        return *this;
      }

      //***************************************************
      const_iterator operator --(int)
      {
        iterator previous(*this);
        index = (index == 0) ? p_deque->MAX_SIZE : index - 1;

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
      clear();

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


      clear();

      std::fill_n(p_buffer, n, value);

      first.index = 0;
      last.index = n - 1;
      current_size = n;
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
      first = iterator(0, *this, p_buffer);
      last  = iterator(0, *this, p_buffer);
      current_size = 0;
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
          push_front(value);
          position = first;
        }
        else if (insert_position == end())
        {
          push_back(value);
          position = last;
        }
        else
        {
          // Are we closer to the front?
          if (std::distance(first, position) < std::distance(position, last))
          {
            // Move the values.
            std::copy(first, position, first - 1);

            // Write the new value.
            *(--position) = value;

            // Adjust the iterator
            --first;
          }
          else
          {
            // Adjust the iterator
            ++last;

            // Move the values.
            std::copy_backward(position, last, last + 1);

            // Write the new value.
            *position = value;
          }

          ++current_size;
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
          first -= n;
          std::fill_n(first, n, value);
          position = first;
        }
        else if (insert_position == end())
        {
          position = last + 1;
          std::fill_n(position, n, value);
          last += n;
        }
        else
        {
          // Non-const insert iterator.
          position = iterator(insert_position.index, *this, p_buffer);

          // Are we closer to the front?
          if (distance(first, insert_position) < difference_type(current_size / 2))
          {
            iterator new_first = first - n;
            iterator new_position = position - n;

            // Move the values.
            std::copy(first, position, new_first);

            // Write the new value.
            std::fill_n(new_position, n, value);

            // Adjust the iterators
            first = new_first;
            position = new_position;
          }
          else
          {
            iterator new_last = last + n;

            // Move the values.
            std::copy_backward(position, last + 1, new_last + 1);

            // Write the new value.
            std::fill_n(position, n, value);

            // Adjust the iterator
            last = new_last;
          }
        }

        current_size += n;
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

      // Find out how much room we need.
      size_t n = std::distance(range_begin, range_end);

      if ((current_size + n) <= MAX_SIZE)
      {
        if (insert_position == begin())
        {
          // Adjust the iterator.
          first -= n;
          iterator destination(first);

          // Copy them over.
          while (range_begin != range_end)
          {
            *destination++ = *range_begin++;
          }

          position = first;
        }
        else if (insert_position == end())
        {
          position = last + 1;

          while (range_begin != range_end)
          {
            *(++last) = *range_begin++;
          }
        }
        else
        {
          // Non-const insert iterator.
          position = iterator(insert_position.index, *this, p_buffer);

          // Are we closer to the front?
          if (distance(first, insert_position) < difference_type(current_size / 2))
          {
            iterator new_first = first - n;
            iterator new_position = position - n;

            // Move the values.
            std::copy(first, position, new_first);

            // Write the new values.
            std::copy(range_begin, range_end, new_position);

            // Adjust the iterators
            first = new_first;
            position = new_position;
          }
          else
          {
            iterator new_last = last + n;

            // Move the values.
            std::copy_backward(position, last + 1, new_last + 1);

            // Write the new values.
            std::copy(range_begin, range_end, position);

            // Adjust the iterator.
            last = new_last;
          }
        }

        current_size += n;
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
    /// erase an item.
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
          ++first;
          position = first;
        }
        else if (position == last)
        {
          --last;
        }
        else
        {
          // Are we closer to the front?
          if (distance(first, position) < difference_type(current_size / 2))
          {
            std::copy_backward(first, position, position + 1);
            ++first;
            ++position;
          }
          else
          {
            std::copy(position + 1, last + 1, position);
            --last;
          }
        }

        --current_size;
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
        difference_type length = std::distance(range_begin, range_end);

        // At the beginning?
        if (position == first)
        {
          first += length;
          position = first;
        }
        // At the end?
        else if (position == last - length + 1)
        {
          last -= length;
        }
        else
        {
          // Copy the smallest number of items.
          // Are we closer to the front?
          if (distance(first, position) < difference_type(current_size / 2))
          {
            // Move the items.
            std::copy_backward(first, position, position + length);
            first += length;
            position += length;
          }
          else
          // Must be closer to the back.
          {
            // Move the items.
            std::copy(position + length, last + 1, position);
            last -= length;
          }
        }

        current_size -= length;
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
        if (!empty())
        {
          ++last;
        }

        *last = item;
        ++current_size;
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
      if (!full())
      {
        if (!empty())
        {
          ++last;
        }

        ++current_size;
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
        --current_size;

        if (!empty())
        {
          --last;
        }
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
        if (!empty())
        {
          --first;
        }

        *first = item;
        ++current_size;
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
        if (!empty())
        {
          --first;
        }

        ++current_size;
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
        --current_size;

        if (!empty())
        {
          ++first;
        }
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
          last -= (current_size - new_size);
        }
        // Make it larger?
        else if (new_size > current_size)
        {
          size_t count = new_size - current_size;

          for (size_t i = 0; i < count; ++i)
          {
            *(++last) = value;
          }
        }

        current_size = new_size;
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

#endif
