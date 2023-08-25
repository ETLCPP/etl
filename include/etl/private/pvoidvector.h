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

#ifndef ETL_PVOIDVECTOR_INCLUDED
#define ETL_PVOIDVECTOR_INCLUDED

#define ETL_IN_PVOIDVECTOR

#include "../platform.h"
#include "../algorithm.h"
#include "vector_base.h"
#include "../type_traits.h"
#include "../error_handler.h"
#include "../functional.h"
#include "../iterator.h"

#include <stddef.h>

#include "minmax_push.h"

namespace etl
{
  //***************************************************************************
  /// The base class for void* vectors.
  ///\ingroup vector
  //***************************************************************************
  class pvoidvector : public vector_base
  {
  public:

    typedef void*                                 value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef value_type*                           pointer;
    typedef const value_type*                     const_pointer;
    typedef value_type*                           iterator;
    typedef const value_type*                     const_iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;
    typedef etl::iterator_traits<iterator>::difference_type difference_type;

  public:

    //*********************************************************************
    /// Returns an iterator to the beginning of the vector.
    ///\return An iterator to the beginning of the vector.
    //*********************************************************************
    iterator begin()
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator(p_buffer);
    }

    //*********************************************************************
    /// Returns an iterator to the end of the vector.
    ///\return An iterator to the end of the vector.
    //*********************************************************************
    iterator end()
    {
      return p_end;
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator(p_end);
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(p_buffer);
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator(p_end);
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the vector.
    ///\return Iterator to the reverse beginning of the vector.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(end());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the vector.
    ///\return Const iterator to the reverse beginning of the vector.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(end());
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the vector.
    ///\return Reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the vector.
    ///\return Const reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(begin());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the vector.
    ///\return Const reverse iterator to the reverse beginning of the vector.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(cend());
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the vector.
    ///\return Const reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(cbegin());
    }

    //*********************************************************************
    /// Resizes the vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    //*********************************************************************
    void resize(size_t new_size)
    {
      ETL_ASSERT_OR_RETURN(new_size <= CAPACITY, ETL_ERROR(vector_full));

      p_end = p_buffer + new_size;
    }

    //*********************************************************************
    /// Resizes the vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    ///\param value   The value to fill new elements with. Default = default constructed value.
    //*********************************************************************
    void resize(size_t new_size, value_type value)
    {
      ETL_ASSERT_OR_RETURN(new_size <= CAPACITY, ETL_ERROR(vector_full));

      pointer p_new_end = p_buffer + new_size;

      // Size up if necessary.
      if (p_end < p_new_end)
      {
        etl::fill(p_end, p_new_end, value);       
      }

      p_end = p_new_end;
    }

    //*********************************************************************
    /// Resizes the vector, but does not initialise new entries.
    ///\param new_size The new size.
    //*********************************************************************
    void uninitialized_resize(size_t new_size)
    {
      ETL_ASSERT_OR_RETURN(new_size <= CAPACITY, ETL_ERROR(vector_full));

      p_end = p_buffer + new_size;
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference operator [](size_t i)
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference operator [](size_t i) const
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference at(size_t i)
    {
      ETL_ASSERT(i < size(), ETL_ERROR(vector_out_of_bounds));
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference at(size_t i) const
    {
      ETL_ASSERT(i < size(), ETL_ERROR(vector_out_of_bounds));
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a reference to the first element.
    ///\return A reference to the first element.
    //*********************************************************************
    reference front()
    {
      return p_buffer[0];
    }

    //*********************************************************************
    /// Returns a const reference to the first element.
    ///\return A const reference to the first element.
    //*********************************************************************
    const_reference front() const
    {
      return p_buffer[0];
    }

    //*********************************************************************
    /// Returns a reference to the last element.
    ///\return A reference to the last element.
    //*********************************************************************
    reference back()
    {
      return *(p_end - 1);
    }

    //*********************************************************************
    /// Returns a const reference to the last element.
    ///\return A const reference to the last element.
    //*********************************************************************
    const_reference back() const
    {
      return *(p_end - 1);
    }

    //*********************************************************************
    /// Returns a pointer to the beginning of the vector data.
    ///\return A pointer to the beginning of the vector data.
    //*********************************************************************
    pointer data()
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Returns a const pointer to the beginning of the vector data.
    ///\return A const pointer to the beginning of the vector data.
    //*********************************************************************
    const_pointer data() const
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Assigns values to the vector. Non-pointer
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    /// If asserts or exceptions are enabled, emits vector_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    typename etl::enable_if<!etl::is_pointer<TIterator>::value, void>::type
      assign(TIterator first, TIterator last)
    {
#if ETL_IS_DEBUG_BUILD
      difference_type d = etl::distance(first, last);
      ETL_ASSERT_OR_RETURN(static_cast<size_t>(d) <= CAPACITY, ETL_ERROR(vector_full));
#endif

      initialise();

      while (first != last)
      {
        *p_end++ = (void*)(*first);
        ++first;
      }
    }

    //*********************************************************************
    /// Assigns values to the vector. Pointer
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    /// If asserts or exceptions are enabled, emits vector_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    typename etl::enable_if<etl::is_pointer<TIterator>::value, void>::type
      assign(TIterator first, TIterator last)
    {
#if ETL_IS_DEBUG_BUILD     
      difference_type d = etl::distance(first, last);
      ETL_ASSERT_OR_RETURN(static_cast<size_t>(d) <= CAPACITY, ETL_ERROR(vector_full));
#endif

      initialise();

      void** p_first = (void**)(first);
      void** p_last  = (void**)(last);

      p_end = etl::copy(p_first, p_last, p_buffer);
    }

    //*********************************************************************
    /// Assigns values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, value_type value)
    {
      ETL_ASSERT_OR_RETURN(n <= CAPACITY, ETL_ERROR(vector_full));

      initialise();

      p_end = etl::fill_n(p_buffer, n, value);
    }

    //*************************************************************************
    /// Clears the vector.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*********************************************************************
    /// Inserts a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(value_type value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT_OR_RETURN(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      *p_end++ = value;
    }

    //*********************************************************************
    /// Emplaces a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void emplace_back(value_type value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT_OR_RETURN(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      * p_end++ = value;
    }

    //*************************************************************************
    /// Removes an element from the end of the vector.
    /// Does nothing if the vector is empty.
    //*************************************************************************
    void pop_back()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT_OR_RETURN(size() > 0, ETL_ERROR(vector_empty));
#endif
      --p_end;
    }

    //*********************************************************************
    /// Inserts a value to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(const_iterator position, value_type value)
    {
      

      iterator position_ = to_iterator(position);

      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));

      if (size() != CAPACITY)
      {
        if (position_ != end())
        {
          ++p_end;
          etl::copy_backward(position_, end() - 1, end());
          *position_ = value;
        }
        else
        {
          *p_end++ = value;
        }
      }

      return position_;
    }

    //*************************************************************************
    /// Emplaces a value to the vector at the specified position.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    //*************************************************************************
    iterator emplace(const_iterator position)
    {
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));

      iterator position_ = to_iterator(position);

      if (position_ != end())
      {
        ++p_end;
        etl::copy_backward(position_, end() - 1, end());
        *position_ = ETL_NULLPTR;
      }
      else
      {
        *p_end++ = ETL_NULLPTR;
      }

      return position_;
    }

    //*************************************************************************
    /// Emplaces a value to the vector at the specified position.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    //*************************************************************************
    iterator emplace(const_iterator position, value_type value)
    {
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));

      iterator position_ = to_iterator(position);

      if (position_ != end())
      {
        ++p_end;
        etl::copy_backward(position_, end() - 1, end());
        *position_ = value;
      }
      else
      {
        *p_end++ = value;
      }

      return position_;
    }

    //*********************************************************************
    /// Inserts 'n' values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    void insert(const_iterator position, size_t n, value_type value)
    {
      ETL_ASSERT_OR_RETURN((size() + n) <= CAPACITY, ETL_ERROR(vector_full));

      iterator position_ = to_iterator(position);

      etl::copy_backward(position_, p_end, p_end + n);
      etl::fill_n(position_, n, value);

      p_end += n;
    }

    //*********************************************************************
    /// Inserts a range of values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    /// For fundamental and pointer types.
    ///\param position The position to insert before.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <typename TIterator>
    void insert(const_iterator position, TIterator first, TIterator last)
    {
      size_t count = etl::distance(first, last);

      iterator position_ = to_iterator(position);

      ETL_ASSERT_OR_RETURN((size() + count) <= CAPACITY, ETL_ERROR(vector_full));

      etl::copy_backward(position_, p_end, p_end + count);
      etl::copy(first, last, position_);
      p_end += count;
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      etl::copy(i_element + 1, end(), i_element);
      --p_end;

      return i_element;
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(const_iterator i_element)
    {
      iterator i_element_ = to_iterator(i_element);

      etl::copy(i_element_ + 1, end(), i_element_);
      --p_end;

      return i_element_;
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(const_iterator first, const_iterator last)
    {
      iterator first_ = to_iterator(first);
      iterator last_  = to_iterator(last);

      etl::copy(last_, end(), first_);
      size_t n_delete = etl::distance(first, last);

      // Just adjust the count.
      p_end -= n_delete;

      return first_;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    etl::pvoidvector& operator = (const etl::pvoidvector& rhs)
    {
      if (&rhs != this)
      {
        this->initialise();
        this->resize(rhs.size());
        etl::copy_n(rhs.data(), rhs.size(), this->data());
      }

      return *this;
    }

#if ETL_USING_CPP11
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    etl::pvoidvector& operator = (etl::pvoidvector&& rhs)
    {
      if (&rhs != this)
      {
        this->initialise();
        this->resize(rhs.size());
        etl::copy_n(rhs.data(), rhs.size(), this->data());
        rhs.initialise();
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// Gets the current size of the vector.
    ///\return The current size of the vector.
    //*************************************************************************
    size_type size() const
    {
      return size_t(p_end - p_buffer);
    }

    //*************************************************************************
    /// Checks the 'empty' state of the vector.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return (p_end == p_buffer);
    }

    //*************************************************************************
    /// Checks the 'full' state of the vector.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return size() == CAPACITY;
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

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    pvoidvector(void** p_buffer_, size_t MAX_SIZE)
      : vector_base(MAX_SIZE)
      , p_buffer(p_buffer_)
      , p_end(p_buffer_)
    {
    }

    //*********************************************************************
    /// Initialise the vector.
    //*********************************************************************
    void initialise()
    {
      p_end = p_buffer;
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair_buffer(void** p_buffer_)
    {
      uintptr_t length = p_end - p_buffer;

      p_buffer = p_buffer_;
      p_end = p_buffer_ + length;
    }

    void** p_buffer;
    void** p_end;

  private:

    //*************************************************************************
    /// Convert from const_iterator to iterator
    //*************************************************************************
    iterator to_iterator(const_iterator itr) const
    {
      return const_cast<iterator>(itr);
    }

    // Disable copy construction.
    pvoidvector(const pvoidvector&);
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  inline bool operator ==(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  inline bool operator !=(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return !(lhs == rhs);
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexicographically less than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  inline bool operator <(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return etl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  inline bool operator >(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return (rhs < lhs);
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexicographically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  inline bool operator <=(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return !(lhs > rhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexicographically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  inline bool operator >=(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return !(lhs < rhs);
  }
}

#include "minmax_pop.h"

#undef ETL_IN_PVOIDVECTOR

#endif
