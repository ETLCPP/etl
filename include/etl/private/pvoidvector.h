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

#ifndef ETL_PVOIDVECTOR_INCLUDED
#define ETL_PVOIDVECTOR_INCLUDED

#define ETL_IN_PVOIDVECTOR

#include <stddef.h>

#include "../platform.h"
#include "../algorithm.h"
#include "vector_base.h"
#include "../type_traits.h"
#include "../error_handler.h"

#include "../stl/functional.h"
#include "../stl/iterator.h"

#ifdef ETL_COMPILER_GCC
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

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
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef size_t                                size_type;
    typedef std::iterator_traits<iterator>::difference_type difference_type;

  public:

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;

    reverse_iterator rend();
    const_reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    void resize(size_t new_size);
    void resize(size_t new_size, value_type value);

    reference operator [](size_t i);
    const_reference operator [](size_t i) const;

    reference at(size_t i);
    const_reference at(size_t i) const;

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

    pointer data();
    const_pointer data() const;

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
      difference_type d = std::distance(first, last);
      ETL_ASSERT(static_cast<size_t>(d) <= CAPACITY, ETL_ERROR(vector_full));
#endif

      initialise();

      while (first != last)
      {
        *p_end++ = const_cast<void*>(*first++);
      }
    }

    void assign(size_t n, value_type value);

    void clear();

    void push_back(value_type value);

    void pop_back();

    iterator insert(iterator position, value_type value);
    void insert(iterator position, size_t n, value_type value);

    //*********************************************************************
    /// Inserts a range of values to the vector.
    /// If asserts or exceptions are enabled, emits vector_full if the vector does not have enough free space.
    /// For fundamental and pointer types.
    ///\param position The position to insert before.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <typename TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      size_t count = std::distance(first, last);

      ETL_ASSERT((size() + count) <= CAPACITY, ETL_ERROR(vector_full));

      std::copy_backward(position, p_end, p_end + count);
      std::copy(first, last, position);
      p_end += count;
    }

    iterator erase(iterator i_element);
    iterator erase(iterator first, iterator last);

    pvoidvector& operator = (const pvoidvector& rhs);

    size_type size() const;

    bool empty() const;

    bool full() const;

    size_t available() const;

  protected:

    pvoidvector(void** p_buffer_, size_t MAX_SIZE);

    void initialise();

    void repair_buffer(void** p_buffer_);

    void** p_buffer;
    void** p_end;

  private:

    // Disable copy construction.
    pvoidvector(const pvoidvector&);
  };

  bool operator ==(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs);
  bool operator !=(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs);
  bool operator  <(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs);
  bool operator  >(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs);
  bool operator <=(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs);
  bool operator >=(const etl::pvoidvector& lhs, const etl::pvoidvector& rhs);
}

#include "minmax_pop.h"

#undef ETL_IN_PVOIDVECTOR

#endif
