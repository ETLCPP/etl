///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#ifndef ETL_VECTOR_INCLUDED
#define ETL_VECTOR_INCLUDED

#define ETL_IN_VECTOR_H

#include <stddef.h>
#include <stdint.h>
#include <stddef.h>

#include <new>

#include "platform.h"
#include "algorithm.h"
#include "type_traits.h"
#include "error_handler.h"
#include "memory.h"
#include "container.h"
#include "alignment.h"
#include "array.h"
#include "exception.h"
#include "debug_count.h"
#include "private/vector_base.h"

#include "algorithm.h"
#include "iterator.h"
#include "functional.h"

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
  #include <initializer_list>
#endif

#ifdef ETL_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

//*****************************************************************************
///\defgroup vector vector
/// A vector with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// The base class for specifically sized vectors.
  /// Can be used as a reference type for all vectors containing a specific type.
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  class ivector : public etl::vector_base
  {
  public:

    typedef T                                     value_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
#if ETL_CPP11_SUPPORTED
    typedef T&&                                   rvalue_reference;
#endif
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;
    typedef typename etl::iterator_traits<iterator>::difference_type difference_type;

  protected:

    typedef typename etl::parameter_type<T>::type parameter_t;

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
    void resize(size_t new_size, T value)
    {
      ETL_ASSERT(new_size <= CAPACITY, ETL_ERROR(vector_full));

      const size_t current_size = size();
      size_t delta = (current_size < new_size) ? new_size - current_size : current_size - new_size;

      if (current_size < new_size)
      {
        etl::uninitialized_fill_n(p_end, delta, value);
        ETL_ADD_DEBUG_COUNT(delta)
      }
      else
      {
        etl::destroy_n(p_end - delta, delta);
        ETL_SUBTRACT_DEBUG_COUNT(delta)
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
      ETL_STATIC_ASSERT((etl::is_same<typename etl::remove_cv<T>::type, typename etl::remove_cv<typename etl::iterator_traits<TIterator>::value_type>::type>::value), "Iterator type does not match container type");

#if defined(ETL_DEBUG)
      difference_type d = etl::distance(first, last);
      ETL_ASSERT(static_cast<size_t>(d) <= CAPACITY, ETL_ERROR(vector_full));
#endif

      initialise();

      p_end = etl::uninitialized_copy(first, last, p_buffer);
      ETL_ADD_DEBUG_COUNT(uint32_t(etl::distance(first, last)))
    }

    //*********************************************************************
    /// Assigns values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, parameter_t value)
    {
      ETL_ASSERT(n <= CAPACITY, ETL_ERROR(vector_full));

      initialise();

      p_end = etl::uninitialized_fill_n(p_buffer, n, value);
      ETL_ADD_DEBUG_COUNT(uint32_t(n))
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
    void push_back(const_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      create_back(value);
    }

#if ETL_CPP11_SUPPORTED
    //*********************************************************************
    /// Inserts a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(rvalue_reference value)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      create_back(etl::move(value));
    }
#endif

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && !defined(ETL_VECTOR_FORCE_CPP03)
    //*********************************************************************
    /// Constructs a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename ... Args>
    void emplace_back(Args && ... args)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      ::new (p_end) T(etl::forward<Args>(args)...);
      ++p_end;
      ETL_INCREMENT_DEBUG_COUNT
    }
#else
    //*********************************************************************
    /// Constructs a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1>
    void emplace_back(const T1& value1)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      ::new (p_end) T(value1);
      ++p_end;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*********************************************************************
    /// Constructs a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2>
    void emplace_back(const T1& value1, const T2& value2)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      ::new (p_end) T(value1, value2);
      ++p_end;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*********************************************************************
    /// Constructs a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2, typename T3>
    void emplace_back(const T1& value1, const T2& value2, const T3& value3)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      ::new (p_end) T(value1, value2, value3);
      ++p_end;
      ETL_INCREMENT_DEBUG_COUNT
    }

    //*********************************************************************
    /// Constructs a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    template <typename T1, typename T2, typename T3, typename T4>
    void emplace_back(const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
#if defined(ETL_CHECK_PUSH_POP)
      ETL_ASSERT(size() != CAPACITY, ETL_ERROR(vector_full));
#endif
      ::new (p_end) T(value1, value2, value3, value4);
      ++p_end;
      ETL_INCREMENT_DEBUG_COUNT
    }
#endif

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
    iterator insert(iterator position, const_reference value)
    {
      ETL_ASSERT(size() + 1 <= CAPACITY, ETL_ERROR(vector_full));

      if (position == end())
      {
        create_back(value);
      }
      else
      {
        create_back(back());
        etl::move_backward(position, p_end - 2, p_end - 1);
        *position = value;
      }

      return position;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************************************************
    /// Inserts a value to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, rvalue_reference value)
    {
      ETL_ASSERT(size() + 1 <= CAPACITY, ETL_ERROR(vector_full));

      if (position == end())
      {
        create_back(etl::move(value));
      }
      else
      {
        create_back(etl::move(back()));
        etl::move_backward(position, p_end - 2, p_end - 1);
        *position = etl::move(value);
      }

      return position;
    }
#endif

    //*************************************************************************
    /// Emplaces a value to the vector at the specified position.
    //*************************************************************************
#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT
    template <typename ... Args>
    iterator emplace(iterator position, Args && ... args)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      void* p;

      if (position == end())
      {
        p = p_end++;
        ETL_INCREMENT_DEBUG_COUNT
      }
      else
      {
        p = etl::addressof(*position);
        create_back(back());
        etl::move_backward(position, p_end - 2, p_end - 1);
        (*position).~T();
      }

      ::new (p) T(etl::forward<Args>(args)...);

      return position;
    }
#else
    template <typename T1>
    iterator emplace(iterator position, const T1& value1)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      void* p;

      if (position == end())
      {
        p = p_end++;
        ETL_INCREMENT_DEBUG_COUNT
      }
      else
      {
        p = etl::addressof(*position);
        create_back(back());
        etl::move_backward(position, p_end - 2, p_end - 1);
        (*position).~T();
      }

      ::new (p) T(value1);

      return position;
    }

    template <typename T1, typename T2>
    iterator emplace(iterator position, const T1& value1, const T2& value2)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      void* p;

      if (position == end())
      {
        p = p_end++;
        ETL_INCREMENT_DEBUG_COUNT
      }
      else
      {
        p = etl::addressof(*position);
        create_back(back());
        etl::move_backward(position, p_end - 2, p_end - 1);
        (*position).~T();
      }

      ::new (p) T(value1, value2);

      return position;
    }

    template <typename T1, typename T2, typename T3>
    iterator emplace(iterator position, const T1& value1, const T2& value2, const T3& value3)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      void* p;

      if (position == end())
      {
        p = p_end++;
        ETL_INCREMENT_DEBUG_COUNT
      }
      else
      {
        p = etl::addressof(*position);
        create_back(back());
        etl::move_backward(position, p_end - 2, p_end - 1);
        (*position).~T();
      }

      ::new (p) T(value1, value2, value3);

      return position;
    }

    template <typename T1, typename T2, typename T3, typename T4>
    iterator emplace(iterator position, const T1& value1, const T2& value2, const T3& value3, const T4& value4)
    {
      ETL_ASSERT(!full(), ETL_ERROR(vector_full));

      void* p;

      if (position == end())
      {
        p = p_end++;
        ETL_INCREMENT_DEBUG_COUNT
      }
      else
      {
        p = etl::addressof(*position);
        create_back(back());
        etl::move_backward(position, p_end - 2, p_end - 1);
        (*position).~T();
      }

      ::new (p) T(value1, value2, value3, value4);

      return position;
    }
#endif

    //*********************************************************************
    /// Inserts 'n' values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    void insert(iterator position, size_t n, parameter_t value)
    {
      ETL_ASSERT((size() + n) <= CAPACITY, ETL_ERROR(vector_full));

      size_t insert_n = n;
      size_t insert_begin = etl::distance(begin(), position);
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

      // Construct old.
      etl::uninitialized_move(p_end - construct_old_n, p_end, p_construct_old);
      ETL_ADD_DEBUG_COUNT(construct_old_n)

      // Copy old.
      etl::move_backward(p_buffer + insert_begin, p_buffer + insert_begin + copy_old_n, p_buffer + insert_end + copy_old_n);

      // Construct new.
      etl::uninitialized_fill_n(p_end, construct_new_n, value);
      ETL_ADD_DEBUG_COUNT(construct_new_n)

        // Copy new.
        etl::fill_n(p_buffer + insert_begin, copy_new_n, value);

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
    template <class TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      size_t count = etl::distance(first, last);

      ETL_ASSERT((size() + count) <= CAPACITY, ETL_ERROR(vector_full));

      size_t insert_n = count;
      size_t insert_begin = etl::distance(begin(), position);
      size_t insert_end = insert_begin + insert_n;

      // Move old data.
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

      // Move construct old.
      etl::uninitialized_move(p_end - construct_old_n, p_end, p_construct_old);
      ETL_ADD_DEBUG_COUNT(construct_old_n)

      // Move old.
      etl::move_backward(p_buffer + insert_begin, p_buffer + insert_begin + copy_old_n, p_buffer + insert_end + copy_old_n);

      // Copy construct new.
      etl::uninitialized_copy(first + copy_new_n, first + copy_new_n + construct_new_n, p_end);
      ETL_ADD_DEBUG_COUNT(construct_new_n)

      // Copy new.
      etl::copy(first, first + copy_new_n, p_buffer + insert_begin);

      p_end += count;
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      etl::move(i_element + 1, end(), i_element);
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
    iterator erase(iterator first, iterator last)
    {
      if (first == begin() && last == end())
      {
        clear();
      }
      else
      {
        etl::move(last, end(), first);
        size_t n_delete = etl::distance(first, last);

        // Destroy the elements left over at the end.
        etl::destroy(p_end - n_delete, p_end);
        ETL_SUBTRACT_DEBUG_COUNT(n_delete)
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

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    ivector& operator = (ivector&& rhs)
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

#ifdef ETL_IVECTOR_REPAIR_ENABLE
    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    virtual void repair() = 0;
#endif

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    ivector(T* p_buffer_, size_t MAX_SIZE)
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
      etl::destroy(p_buffer, p_end);
      ETL_SUBTRACT_DEBUG_COUNT(int32_t(etl::distance(p_buffer, p_end)))

      p_end = p_buffer;
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair_buffer(T* p_buffer_)
    {
      uintptr_t length = p_end - p_buffer;
      p_buffer = p_buffer_;
      p_end    = p_buffer_ + length;
    }

    pointer p_buffer; ///< Pointer to the start of the buffer.
    pointer p_end;    ///< Pointer to one past the last element in the buffer.

  private:

    //*********************************************************************
    /// Create a new element with a default value at the back.
    //*********************************************************************
    inline void create_back()
    {
      etl::create_value_at(p_end);
      ETL_INCREMENT_DEBUG_COUNT

      ++p_end;
    }

    //*********************************************************************
    /// Create a new element with a value at the back
    //*********************************************************************
    inline void create_back(const_reference value)
    {
      etl::create_copy_at(p_end, value);
      ETL_INCREMENT_DEBUG_COUNT

      ++p_end;
    }

#if ETL_CPP11_SUPPORTED
    //*********************************************************************
    /// Create a new element with a value at the back
    //*********************************************************************
    inline void create_back(rvalue_reference value)
    {
      etl::create_copy_at(p_end, etl::move(value));
      ETL_INCREMENT_DEBUG_COUNT

      ++p_end;
    }
#endif

    //*********************************************************************
    /// Destroy an element at the back.
    //*********************************************************************
    inline void destroy_back()
    {
      --p_end;

      etl::destroy_at(p_end);
      ETL_DECREMENT_DEBUG_COUNT
    }

    // Disable copy construction.
    ivector(const ivector&);

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
#if defined(ETL_POLYMORPHIC_VECTOR) || defined(ETL_POLYMORPHIC_CONTAINERS)
  public:
    virtual ~ivector()
    {
    }
#else
  protected:
    ~ivector()
    {
    }
#endif
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
    return (lhs.size() == rhs.size()) && etl::equal(lhs.begin(), lhs.end(), rhs.begin());
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
    return etl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
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

namespace etl
{
  //***************************************************************************
  /// A vector implementation that uses a fixed size buffer.
  ///\tparam T The element type.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup vector
  //***************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class vector : public etl::ivector<T>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    vector()
      : etl::ivector<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initial_size The initial size of the vector.
    //*************************************************************************
    explicit vector(size_t initial_size)
      : etl::ivector<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      this->initialise();
      this->resize(initial_size);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initial_size  The initial size of the vector.
    ///\param value        The value to fill the vector with.
    //*************************************************************************
    vector(size_t initial_size, typename etl::ivector<T>::parameter_t value)
      : etl::ivector<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      this->initialise();
      this->resize(initial_size, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    vector(TIterator first, TIterator last)
      : etl::ivector<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    vector(std::initializer_list<T> init)
      : etl::ivector<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    vector(const vector& other)
      : etl::ivector<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      this->assign(other.begin(), other.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    vector& operator = (const vector& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    vector(vector&& other)
      : etl::ivector<T>(reinterpret_cast<T*>(&buffer), MAX_SIZE)
    {
      if (this != &other)
      {
        this->initialise();

        typename etl::ivector<T>::iterator itr = other.begin();
        while (itr != other.end())
        {
          this->push_back(etl::move(*itr));
          ++itr;
        }

        other.initialise();
      }
    }

    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    vector& operator = (vector&& rhs)
    {
      if (&rhs != this)
      {
        this->clear();
        typename etl::ivector<T>::iterator itr = rhs.begin();
        while (itr != rhs.end())
        {
          this->push_back(etl::move(*itr));
          ++itr;
        }

        rhs.initialise();
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~vector()
    {
      this->clear();
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
#ifdef ETL_IVECTOR_REPAIR_ENABLE
    virtual
#endif
    void repair()
#ifdef ETL_IVECTOR_REPAIR_ENABLE
      ETL_OVERRIDE
#endif
    {
      #if ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED
      ETL_ASSERT(etl::is_trivially_copyable<T>::value, ETL_ERROR(etl::vector_incompatible_type));
      #endif

      etl::ivector<T>::repair_buffer(buffer);
    }

  private:

    typename etl::aligned_storage<sizeof(T) * MAX_SIZE, etl::alignment_of<T>::value>::type buffer;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_CPP17_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
  template <typename T, typename... Ts>
  vector(T, Ts...)
    ->vector<etl::enable_if_t<(etl::is_same_v<T, Ts> && ...), T>, 1U + sizeof...(Ts)>;
#endif

  //***************************************************************************
  /// A vector implementation that uses a fixed size buffer.
  /// The buffer is supplied on construction.
  ///\tparam T The element type.
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  class vector<T, 0> : public etl::ivector<T>
  {
  public:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    vector(void* buffer, size_t max_size)
      : etl::ivector<T>(reinterpret_cast<T*>(buffer), max_size)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initial_size The initial size of the vector.
    //*************************************************************************
    explicit vector(size_t initial_size, void* buffer, size_t max_size)
      : etl::ivector<T>(reinterpret_cast<T*>(buffer), max_size)
    {
      this->initialise();
      this->resize(initial_size);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initial_size  The initial size of the vector.
    ///\param value        The value to fill the vector with.
    //*************************************************************************
    vector(size_t initial_size, typename etl::ivector<T>::parameter_t value, void* buffer, size_t max_size)
      : etl::ivector<T>(reinterpret_cast<T*>(buffer), max_size)
    {
      this->initialise();
      this->resize(initial_size, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    vector(TIterator first, TIterator last, void* buffer, size_t max_size)
      : etl::ivector<T>(reinterpret_cast<T*>(buffer), max_size)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    vector(std::initializer_list<T> init, void* buffer, size_t max_size)
      : etl::ivector<T>(reinterpret_cast<T*>(buffer), max_size)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    vector(const vector& other, void* buffer, size_t max_size)
      : etl::ivector<T>(reinterpret_cast<T*>(buffer), max_size)
    {
      this->assign(other.begin(), other.end());
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    vector& operator = (const vector& rhs)
    {
      if (&rhs != this)
      {
        this->assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    vector(vector&& other, void* buffer, size_t max_size)
      : etl::ivector<T>(reinterpret_cast<T*>(buffer), max_size)
    {
      if (this != &other)
      {
        this->initialise();

        typename etl::ivector<T>::iterator itr = other.begin();
        while (itr != other.end())
        {
          this->push_back(etl::move(*itr));
          ++itr;
        }

        other.initialise();
      }
    }

    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    vector& operator = (vector&& rhs)
    {
      if (&rhs != this)
      {
        this->clear();

        typename etl::ivector<T>::iterator itr = rhs.begin();
        while (itr != rhs.end())
        {
          this->push_back(etl::move(*itr));
          ++itr;
        }

        rhs.initialise();
      }

      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~vector()
    {
      this->clear();
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair()
#ifdef ETL_IVECTOR_REPAIR_ENABLE
      ETL_OVERRIDE
#endif
    {
#if ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED
      ETL_ASSERT(etl::is_trivially_copyable<T>::value, ETL_ERROR(etl::vector_incompatible_type));
#endif

      etl::ivector<T>::repair_buffer(this->p_buffer);
    }
  };

  //***************************************************************************
  /// A vector implementation that uses a fixed size buffer.
  ///\tparam T The element type.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup vector
  //***************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class vector<T*, MAX_SIZE_> : public etl::ivector<T*>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    vector()
      : etl::ivector<T*>(reinterpret_cast<T**>(&buffer), MAX_SIZE)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initial_size The initial size of the vector.
    //*************************************************************************
    explicit vector(size_t initial_size)
      : etl::ivector<T*>(reinterpret_cast<T**>(&buffer), MAX_SIZE)
    {
      this->initialise();
      this->resize(initial_size);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initial_size  The initial size of the vector.
    ///\param value        The value to fill the vector with.
    //*************************************************************************
    vector(size_t initial_size, typename etl::ivector<T*>::parameter_t value)
      : etl::ivector<T*>(reinterpret_cast<T**>(&buffer), MAX_SIZE)
    {
      this->initialise();
      this->resize(initial_size, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    vector(TIterator first, TIterator last)
      : etl::ivector<T*>(reinterpret_cast<T**>(&buffer), MAX_SIZE)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    vector(std::initializer_list<T*> init)
      : etl::ivector<T*>(reinterpret_cast<T**>(&buffer), MAX_SIZE)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    vector(const vector& other)
      : etl::ivector<T*>(reinterpret_cast<T**>(&buffer), MAX_SIZE)
    {
      (void)etl::ivector<T*>::operator = (other);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    vector& operator = (const vector& rhs)
    {
      (void)etl::ivector<T*>::operator = (rhs);

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    vector(vector&& other)
      : etl::ivector<T*>(reinterpret_cast<T**>(&buffer), MAX_SIZE)
    {
      (void)etl::ivector<T*>::operator = (etl::move(other));
    }

    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    vector& operator = (vector&& rhs)
    {
      (void)etl::ivector<T*>::operator = (etl::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair()
#ifdef ETL_IVECTOR_REPAIR_ENABLE
      ETL_OVERRIDE
#endif
    {
      etl::ivector<T*>::repair_buffer(buffer);
    }

  private:

    typename etl::aligned_storage<sizeof(T*) * MAX_SIZE, etl::alignment_of<T*>::value>::type buffer;
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_CPP17_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
  template <typename T, typename... Ts>
  vector(T*, Ts*...)
    ->vector<etl::enable_if_t<(etl::is_same_v<T, Ts> && ...), T*>, 1U + sizeof...(Ts)>;
#endif

  //***************************************************************************
  /// A vector implementation that uses a fixed size buffer.
  /// The buffer is supplied on construction.
  ///\tparam T The element type that is pointed to.
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  class vector<T*, 0> : public etl::ivector<T*>
  {
  public:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    vector(void* buffer, size_t max_size)
      : etl::ivector<T*>(reinterpret_cast<T**>(buffer), max_size)
    {
      this->initialise();
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initial_size The initial size of the vector.
    //*************************************************************************
    vector(size_t initial_size, void* buffer, size_t max_size)
      : etl::ivector<T*>(reinterpret_cast<T**>(buffer), max_size)
    {
      this->initialise();
      this->resize(initial_size);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initial_size  The initial size of the vector.
    ///\param value        The value to fill the vector with.
    //*************************************************************************
    vector(size_t initial_size, typename etl::ivector<T*>::parameter_t value, void* buffer, size_t max_size)
      : etl::ivector<T*>(reinterpret_cast<T**>(buffer), max_size)
    {
      this->initialise();
      this->resize(initial_size, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    vector(TIterator first, TIterator last, void* buffer, size_t max_size)
      : etl::ivector<T*>(reinterpret_cast<T**>(buffer), max_size)
    {
      this->assign(first, last);
    }

#if ETL_CPP11_SUPPORTED && ETL_NOT_USING_STLPORT && ETL_USING_STL
    //*************************************************************************
    /// Constructor, from an initializer_list.
    //*************************************************************************
    vector(std::initializer_list<T*> init, void* buffer, size_t max_size)
      : etl::ivector<T*>(reinterpret_cast<T**>(buffer), max_size)
    {
      this->assign(init.begin(), init.end());
    }
#endif

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    vector(const vector& other, void* buffer, size_t max_size)
      : etl::ivector<T*>(reinterpret_cast<T**>(buffer), max_size)
    {
      (void)etl::ivector<T*>::operator = (other);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    vector& operator = (const vector& rhs)
    {
      (void)etl::ivector<T*>::operator = (rhs);

      return *this;
    }

#if ETL_CPP11_SUPPORTED
    //*************************************************************************
    /// Move constructor.
    //*************************************************************************
    vector(vector&& other, void* buffer, size_t max_size)
      : etl::ivector<T*>(reinterpret_cast<T**>(buffer), max_size)
    {
      (void)etl::ivector<T*>::operator = (etl::move(other));
    }

    //*************************************************************************
    /// Move assignment operator.
    //*************************************************************************
    vector& operator = (vector&& rhs)
    {
      (void)etl::ivector<T*>::operator = (etl::move(rhs));

      return *this;
    }
#endif

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~vector()
    {
      this->clear();
    }

    //*************************************************************************
    /// Fix the internal pointers after a low level memory copy.
    //*************************************************************************
    void repair()
#ifdef ETL_IVECTOR_REPAIR_ENABLE
      ETL_OVERRIDE
#endif
    {
      etl::ivector<T*>::repair_buffer(this->p_buffer);
    }
  };
}

#ifdef ETL_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

#endif
