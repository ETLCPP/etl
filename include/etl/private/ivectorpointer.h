///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef __ETL_IVECTOR_POINTER__
#define __ETL_IVECTOR_POINTER__

#ifndef __ETL_IN_VECTOR_H__
#error  This header is a private element of etl::ivector
#endif

#include "pvoidvector.h"

namespace etl
{
  //***************************************************************************
  /// The base class for specifically sized vectors.
  /// Can be used as a reference type for all vectors containing a specific pointer type.
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  class ivector<T*> : public pvoidvector
  {
  public:

    typedef T*                                    value_type;
    typedef T*&                                   reference;
    typedef const T* const &                      const_reference;
    typedef T**                                   pointer;
    typedef const T* const *                      const_pointer;
    typedef T**                                   iterator;
    typedef const T* const *                      const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

  protected:

    typedef value_type parameter_t;

  private:

    typedef pvoidvector base_t;

  public:

    //*********************************************************************
    /// Returns an iterator to the beginning of the vector.
    ///\return An iterator to the beginning of the vector.
    //*********************************************************************
    iterator begin()
    {
      return iterator(base_t::begin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator begin() const
    {
      return const_iterator(base_t::begin());
    }

    //*********************************************************************
    /// Returns an iterator to the end of the vector.
    ///\return An iterator to the end of the vector.
    //*********************************************************************
    iterator end()
    {
      return iterator(base_t::end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator end() const
    {
      return const_iterator(base_t::end());
    }

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return const_iterator(base_t::cbegin());
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator cend() const
    {
      return const_iterator(base_t::cend());
    }

    //*********************************************************************
    /// Returns an reverse iterator to the reverse beginning of the vector.
    ///\return Iterator to the reverse beginning of the vector.
    //*********************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(iterator(base_t::end()));
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the vector.
    ///\return Const iterator to the reverse beginning of the vector.
    //*********************************************************************
    const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(const_iterator(base_t::end()));
    }

    //*********************************************************************
    /// Returns a reverse iterator to the end + 1 of the vector.
    ///\return Reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(iterator(base_t::begin()));
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the vector.
    ///\return Const reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    const_reverse_iterator rend() const
    {
      return const_reverse_iterator(const_iterator(base_t::begin()));
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the vector.
    ///\return Const reverse iterator to the reverse beginning of the vector.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(const_iterator(base_t::cend()));
    }

    //*********************************************************************
    /// Returns a const reverse iterator to the end + 1 of the vector.
    ///\return Const reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
      return const_reverse_iterator(const_iterator(base_t::cbegin()));
    }

    //*********************************************************************
    /// Resizes the vector.
    /// If asserts or exceptions are enabled and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param new_size The new size.
    //*********************************************************************
    void resize(size_t new_size)
    {
      base_t::resize(new_size);
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
      base_t::resize(new_size, value);
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference operator [](size_t i)
    {
      return reference(base_t::operator[](i));
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference operator [](size_t i) const
    {
      return const_reference(base_t::operator[](i));
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference at(size_t i)
    {
      return reference(base_t::at(i));
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    /// If asserts or exceptions are enabled, emits an etl::vector_out_of_bounds if the index is out of range.
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference at(size_t i) const
    {
      return const_reference(base_t::at(i));
    }

    //*********************************************************************
    /// Returns a reference to the first element.
    ///\return A reference to the first element.
    //*********************************************************************
    reference front()
    {
      return reference(base_t::front());
    }

    //*********************************************************************
    /// Returns a const reference to the first element.
    ///\return A const reference to the first element.
    //*********************************************************************
    const_reference front() const
    {
      return const_reference(base_t::front());
    }

    //*********************************************************************
    /// Returns a reference to the last element.
    ///\return A reference to the last element.
    //*********************************************************************
    reference back()
    {
      return reference(base_t::back());
    }

    //*********************************************************************
    /// Returns a const reference to the last element.
    ///\return A const reference to the last element.
    //*********************************************************************
    const_reference back() const
    {
      return const_reference(base_t::back());
    }

    //*********************************************************************
    /// Returns a pointer to the beginning of the vector data.
    ///\return A pointer to the beginning of the vector data.
    //*********************************************************************
    pointer data()
    {
      return pointer(base_t::data());
    }

    //*********************************************************************
    /// Returns a const pointer to the beginning of the vector data.
    ///\return A const pointer to the beginning of the vector data.
    //*********************************************************************
    const_pointer data() const
    {
      return const_pointer(base_t::data());
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
      base_t::initialise();

      while (first != last)
      {
        *p_end++ = (void*)*first++;
      }
    }

    //*********************************************************************
    /// Assigns values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, parameter_t value)
    {
      base_t::assign(n, value);
    }

    //*************************************************************************
    /// Clears the vector.
    //*************************************************************************
    void clear()
    {
      base_t::clear();
    }

    //*************************************************************************
    /// Increases the size of the vector by one, but does not initialise the new element.
    /// If asserts or exceptions are enabled, throws a vector_full if the vector is already full.
    //*************************************************************************
    void push_back()
    {
      base_t::push_back();
    }

    //*********************************************************************
    /// Inserts a value at the end of the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(parameter_t value)
    {
      base_t::push_back(value);
    }

    //*************************************************************************
    /// Removes an element from the end of the vector.
    /// Does nothing if the vector is empty.
    //*************************************************************************
    void pop_back()
    {
      base_t::pop_back();
    }

    //*********************************************************************
    /// Inserts a value to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector is already full.
    ///\param position The position to insert before.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, parameter_t value)
    {
      return iterator(base_t::insert(base_t::iterator(position), value));
    }

    //*********************************************************************
    /// Inserts 'n' values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    void insert(iterator position, size_t n, parameter_t value)
    {
      base_t::insert(base_t::iterator(position), n, value);
    }

    //*********************************************************************
    /// Inserts a range of values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    ///\param position The position to insert before.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      base_t::insert(base_t::iterator(position), first, last);
    }

    //*********************************************************************
    /// Erases an element.
    ///\param i_element Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator i_element)
    {
      return iterator(base_t::erase(base_t::iterator(i_element)));
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
      return iterator(base_t::erase(base_t::iterator(first), base_t::iterator(last)));
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

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    ivector(T** p_buffer_, size_t MAX_SIZE_)
      : pvoidvector(reinterpret_cast<void**>(p_buffer_), MAX_SIZE_)
    {
    }
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator ==(const etl::ivector<T*>& lhs, const etl::ivector<T*>& rhs)
  {
    return pvoidvector_equal(lhs, rhs);
  }

  //***************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator !=(const etl::ivector<T*>& lhs, const etl::ivector<T*>& rhs)
  {
    return pvoidvector_not_equal(lhs, rhs);
  }

  //***************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexicographically less than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator <(const etl::ivector<T*>& lhs, const etl::ivector<T*>& rhs)
  {
    return pvoidvector_less_than(lhs, rhs);
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexicographically greater than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::ivector<T*>& lhs, const etl::ivector<T*>& rhs)
  {
    return pvoidvector_greater_than(lhs, rhs);
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexigraphically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator <=(const etl::ivector<T*>& lhs, const etl::ivector<T*>& rhs)
  {
    return pvoidvector_less_than_equal(lhs, rhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first vector.
  ///\param rhs Reference to the second vector.
  ///\return <b>true</b> if the first vector is lexigraphically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator >=(const etl::ivector<T*>& lhs, const etl::ivector<T*>& rhs)
  {
    return pvoidvector_greater_than_equal(lhs, rhs);
  }

  //***************************************************************************
  // Helper functions
  //***************************************************************************
  inline bool pvoidvector_equal(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return operator ==(lhs, rhs);
  }

  inline bool pvoidvector_not_equal(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return operator !=(lhs, rhs);
  }

  inline bool pvoidvector_less_than(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return operator <(lhs, rhs);
  }

  inline bool pvoidvector_greater_than(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return operator >(lhs, rhs);
  }

  inline bool pvoidvector_less_than_equal(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return operator <=(lhs, rhs);
  }

  inline bool pvoidvector_greater_than_equal(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs)
  {
    return operator >=(lhs, rhs);
  }
}

#endif
