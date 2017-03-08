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

#ifndef __ETL_IVECTOR__
#define __ETL_IVECTOR__
#define __ETL_IN_IVECTOR_H__

#include <iterator>
#include <algorithm>
#include <functional>
#include <stddef.h>

#include "platform.h"
#include "algorithm.h"
#include "private/vector_base.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "error_handler.h"
#include "memory.h"

#ifdef ETL_COMPILER_GCC
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

namespace etl
{
  //***************************************************************************
  /// The base class for specifically sized vectors.
  /// Can be used as a reference type for all vectors containing a specific type.
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  class ivector : public vector_base
  {
  public:

    typedef T                                     value_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

  protected:

    typedef typename parameter_type<T>::type parameter_t;

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
      return p_buffer;
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
      return p_end;
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return p_buffer;
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator cend() const
    {
      return p_end;
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
      resize(new_size, T());
    }

    //*********************************************************************
    /// Resizes the vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    ///\param value   The value to fill new elements with. Default = default constructed value.
    //*********************************************************************
    template <typename U = T>
    typename etl::enable_if<etl::has_trivial_constructor<U>::value, void>::type
      resize(size_t new_size, T value)
    {
      ETL_ASSERT(new_size <= MAX_SIZE, ETL_ERROR(vector_full));

      const size_t current_size = size();
      size_t delta = (current_size < new_size) ? new_size - current_size : current_size - new_size;

      if (current_size < new_size)
      {
        std::fill_n(p_end, delta, value);
#if defined(ETL_DEBUG)
        construct_count += delta;
#endif
      }
      else
      {
        p_end -= delta;
#if defined(ETL_DEBUG)
        construct_count -= delta;
#endif
      }

      p_end = p_buffer + new_size;
    }

    //*********************************************************************
    /// Resizes the vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    ///\param value   The value to fill new elements with. Default = default constructed value.
    //*********************************************************************
    template <typename U = T>
    typename etl::enable_if<!etl::has_trivial_constructor<U>::value, void>::type
     resize(size_t new_size, T value)
    {
      ETL_ASSERT(new_size <= MAX_SIZE, ETL_ERROR(vector_full));

      const size_t current_size = size();
      size_t delta = (current_size < new_size) ? new_size - current_size : current_size - new_size;

      if (current_size < new_size)
      {
        etl::uninitialized_fill_n(p_end, delta, value);
#if defined(ETL_DEBUG)
        construct_count += delta;
#endif
      }
      else
      {
        etl::destroy_n(p_end - delta, delta);
#if defined(ETL_DEBUG)
        construct_count -= delta;
#endif
      }

      p_end = p_buffer + new_size;
    }

    //*********************************************************************
    /// Does nothing.
    //*********************************************************************
    void reserve(size_t)
    {
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
      return *p_buffer;
    }

    //*********************************************************************
    /// Returns a const reference to the first element.
    ///\return A const reference to the first element.
    //*********************************************************************
    const_reference front() const
    {
      return *p_buffer;
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
    /// Assigns values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    /// If asserts or exceptions are enabled, emits vector_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
#if defined(ETL_DEBUG)
      difference_type count = std::distance(first, last);
      ETL_ASSERT(static_cast<size_t>(count) <= MAX_SIZE, ETL_ERROR(vector_full));
#endif

      initialise();

#if defined(ETL_DEBUG)        
      p_end = etl::uninitialized_copy(first, last, p_buffer, construct_count);
#else
      p_end = etl::uninitialized_copy(first, last, p_buffer);
#endif
    }

    //*********************************************************************
    /// Assigns values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, parameter_t value)
    {
      ETL_ASSERT(n <= MAX_SIZE, ETL_ERROR(vector_full));

      initialise();

#if defined(ETL_DEBUG)        
      p_end = etl::uninitialized_fill_n(p_buffer, n, value, construct_count);
#else
      p_end = etl::uninitialized_fill_n(p_buffer, n, value);
#endif
    }

    //*************************************************************************
    /// Clears the vector.
    //*************************************************************************
    void clear()
    {
      initialise();
    }

    //*************************************************************************
    /// Increases the size of the vector by one, but does not initialise the new element.
    /// If asserts or exceptions are enabled, throws a vector_full if the vector is already full.
    //*************************************************************************
    void push_back()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != MAX_SIZE, ETL_ERROR(vector_full));
#endif

      create_back();
    }

    //*********************************************************************
    /// Inserts a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(parameter_t value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != MAX_SIZE, ETL_ERROR(vector_full));
#endif
      create_back(value);
    }

    //*************************************************************************
    /// Removes an element from the end of the vector.
    /// Does nothing if the vector is empty.
    //*************************************************************************
    void pop_back()
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() > 0, ETL_ERROR(vector_empty));
#endif
      destroy_back();
    }

    //*********************************************************************
    /// Inserts a value to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, parameter_t value)
    {
      ETL_ASSERT(size() + 1 <= MAX_SIZE, ETL_ERROR(vector_full));

      if (position == end())
      {
        create_back(value);
      }
      else
      {
        create_back(back());
        std::copy_backward(position, p_end - 1, p_end);
        *position = value;
      }

      return position;
    }

    //*********************************************************************
    /// Inserts 'n' values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    template <typename U = T>
    typename etl::enable_if<etl::has_trivial_constructor<U>::value, void>::type
      insert(iterator position, size_t n, parameter_t value)
    {
      ETL_ASSERT((size() + n) <= MAX_SIZE, ETL_ERROR(vector_full));

      std::copy_backward(position, p_end, p_end + n);
      std::fill_n(position, n, value);

      construct_count += n;
      p_end += n;
    }

    //*********************************************************************
    /// Inserts 'n' values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    template <typename U = T>
    typename etl::enable_if<!etl::has_trivial_constructor<U>::value, void>::type
      insert(iterator position, size_t n, parameter_t value)
    {
      ETL_ASSERT((size() + n) <= MAX_SIZE, ETL_ERROR(vector_full));

      size_t insert_n = n;
      size_t insert_begin = std::distance(begin(), position);
      size_t insert_end = insert_begin + insert_n;

      // Copy old data.
      size_t copy_old_n;
      size_t construct_old_n;
      iterator p_construct_old;

      if (insert_end > size())
      {
        copy_old_n = 0;
        construct_old_n = size() - insert_begin;
        p_construct_old = p_buffer + insert_end;
      }
      else
      {
        copy_old_n = size() - insert_begin - insert_n;
        construct_old_n = insert_n;
        p_construct_old = p_end;
      }

      size_t copy_new_n = construct_old_n;
      size_t construct_new_n = insert_n - copy_new_n;

#if defined(ETL_DEBUG)
      // Construct old.
      etl::uninitialized_copy_n(p_end - construct_old_n, construct_old_n, p_construct_old, construct_count);

      // Copy old.
      etl::copy_n(p_buffer + insert_begin, copy_old_n, p_buffer + insert_end);

      // Construct new.
      etl::uninitialized_fill_n(p_end, construct_new_n, value, construct_count);

      // Copy new.
      std::fill_n(p_buffer + insert_begin, copy_new_n, value);
#else
      // Construct old.
      etl::uninitialized_copy_n(p_end - construct_old_n, construct_old_n, p_construct_old);

      // Copy old.
      etl::copy_n(p_buffer + insert_begin, copy_old_n, p_buffer + insert_end);

      // Construct new.
      etl::uninitialized_fill_n(p_end, construct_new_n, value);

      // Copy new.
      std::fill_n(p_buffer + insert_begin, copy_new_n, value);
#endif

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
    template <class TIterator, typename U = T>
    typename etl::enable_if<etl::has_trivial_constructor<U>::value, void>::type
      insert(iterator position, TIterator first, TIterator last)
    {
      size_t count = std::distance(first, last);

      ETL_ASSERT((size() + count) <= MAX_SIZE, ETL_ERROR(vector_full));

      construct_count += count;

      if (position == end())
      {
        p_end = std::copy(first, last, p_end);
      }
      else
      {
        std::copy_backward(position, p_end, p_end + count);
        std::copy(first, last, position);
        p_end += count;
      }
    }

    //*********************************************************************
    /// Inserts a range of values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    /// For fundamental and pointer types.
    ///\param position The position to insert before.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator, typename U = T>
    typename etl::enable_if<!etl::has_trivial_constructor<U>::value, void>::type
     insert(iterator position, TIterator first, TIterator last)
    {
      size_t count = std::distance(first, last);

      ETL_ASSERT((size() + count) <= MAX_SIZE, ETL_ERROR(vector_full));

      size_t insert_n     = count;
      size_t insert_begin = std::distance(begin(), position);
      size_t insert_end   = insert_begin + insert_n;

      // Copy old data.
      size_t copy_old_n;
      size_t construct_old_n;
      iterator p_construct_old;

      if (insert_end > size())
      {
        copy_old_n      = 0;
        construct_old_n = size() - insert_begin;
        p_construct_old = p_buffer + insert_end;
      }
      else
      {
        copy_old_n      = size() - insert_begin - insert_n;
        construct_old_n = insert_n;
        p_construct_old = p_end;
      }

      size_t copy_new_n      = construct_old_n;
      size_t construct_new_n = insert_n - copy_new_n;
      
#if defined(ETL_DEBUG)
      // Construct old.
      etl::uninitialized_copy_n(p_end - construct_old_n, construct_old_n, p_construct_old, construct_count);

      // Copy old.
      etl::copy_n(p_buffer + insert_begin, copy_old_n, p_buffer + insert_end);

      // Construct new.
      etl::uninitialized_copy_n(first + copy_new_n, construct_new_n, p_end, construct_count);

      // Copy new.
      etl::copy_n(first, copy_new_n, p_buffer + insert_begin);
#else
      // Construct old.
      etl::uninitialized_copy_n(p_end - construct_old_n, construct_old_n, p_construct_old);

      // Copy old.
      etl::copy_n(p_buffer + insert_begin, copy_old_n, p_buffer + insert_end);

      // Construct new.
      etl::uninitialized_copy_n(first + copy_new_n, construct_new_n, p_end);

      // Copy new.
      etl::copy_n(first, copy_new_n, p_buffer + insert_begin);
#endif

      p_end += count;
    }
    
    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      std::copy(i_element + 1, end(), i_element);
      destroy_back();
      
      return i_element;
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    template <typename U = T>
    typename etl::enable_if<etl::has_trivial_constructor<U>::value, iterator>::type
     erase(iterator first, iterator last)
    {
      if (first == begin() && last == end())
      {
        clear();
      }
      else
      {
        std::copy(last, end(), first);
        size_t n_delete = std::distance(first, last);
        construct_count -= n_delete;
        p_end -= n_delete;
      }

      return first;
    }

    //*********************************************************************
    /// Erases a range of elements.
    /// The range includes all the elements between first and last, including the
    /// element pointed by first, but not the one pointed by last.
    ///\param first Iterator to the first element.
    ///\param last  Iterator to the last element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    template <typename U = T>
    typename etl::enable_if<!etl::has_trivial_constructor<U>::value, iterator>::type
     erase(iterator first, iterator last)
    {
      if (first == begin() && last == end())
      {
        clear();
      }
      else
      {
        std::copy(last, end(), first);
        size_t n_delete = std::distance(first, last);

        // Destroy the elements left over at the end.
#if defined(ETL_DEBUG)
        etl::destroy(p_end - n_delete, p_end, construct_count);
#else
        etl::destroy(p_end - n_delete, p_end);
#endif          
        p_end -= n_delete;
      }

      return first;
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    ivector& operator = (const ivector& rhs)
    {
      if (&rhs != this)
      {
        assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

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
      return size() == MAX_SIZE;
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
    ivector(T* p_buffer_, size_t MAX_SIZE)
      : vector_base(MAX_SIZE),
        p_buffer(p_buffer_),
        p_end(p_buffer_)
    {
    }

    //*********************************************************************
    /// Initialise the vector.
    //*********************************************************************
    void initialise()
    {
#if defined(ETL_DEBUG)
      etl::destroy(p_buffer, p_end, construct_count);
#else
      etl::destroy(p_buffer, p_end);
#endif

      p_end = p_buffer;
    }

  private:

    pointer p_buffer; ///< Pointer to the start of the buffer.
    pointer p_end;    ///< Pointer to one past the last element in the buffer.

    //*********************************************************************
    /// Create a new element with a default value at the back.
    //*********************************************************************
    inline void create_back()
    {
#if defined(ETL_DEBUG)
      etl::create_value_at(p_end, construct_count);
#else
      etl::create_value_at(p_end);
#endif
      ++p_end;
    }

    //*********************************************************************
    /// Create a new element with a value at the back
    //*********************************************************************
    inline void create_back(parameter_t value)
    {
#if defined(ETL_DEBUG)
      etl::create_copy_at(p_end, value, construct_count);
#else
      etl::create_copy_at(p_end, value);
#endif
      ++p_end;
    }

    //*********************************************************************
    /// Destroy an element at the back.
    //*********************************************************************
    inline void destroy_back()
    {
      --p_end;

#if defined(ETL_DEBUG)
      etl::destroy_at(p_end, construct_count);
#else
      etl::destroy_at(p_end);
#endif
    }

    // Disable copy construction.
    ivector(const ivector&);
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator ==(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator !=(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
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
  template <typename T>
  bool operator <(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
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
  template <typename T>
  bool operator <=(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
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
  template <typename T>
  bool operator >=(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return !(lhs < rhs);
  }
}

#include "private/ivectorpointer.h"

#undef __ETL_IN_IVECTOR_H__
#endif
