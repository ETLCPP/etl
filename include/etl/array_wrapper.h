///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#ifndef ETL_ARRAY_WRAPPER_INCLUDED
#define ETL_ARRAY_WRAPPER_INCLUDED

#include "platform.h"
#include "iterator.h"
#include "error_handler.h"
#include "exception.h"
#include "hash.h"
#include "container.h"
#include "parameter_type.h"

#include "algorithm.h"

///\defgroup array array
/// A wrapper for arrays
///\ingroup containers

#undef ETL_FILE
#define ETL_FILE "42"

namespace etl
{
  //***************************************************************************
  /// The base class for array_wrapper exceptions.
  //***************************************************************************
  class array_wrapper_exception : public exception
  {
  public:

    array_wrapper_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup stack
  /// The exception thrown when the index is out of bounds.
  //***************************************************************************
  class array_wrapper_bounds : public array_wrapper_exception
  {
  public:

    array_wrapper_bounds(string_type file_name_, numeric_type line_number_)
      : array_wrapper_exception(ETL_ERROR_TEXT("array_wrapper:bounds", ETL_FILE"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Array wrapper.
  //***************************************************************************
  template <typename T, size_t SIZE_, T(&ARRAY_)[SIZE_]>
  class array_wrapper
  {
  public:

    typedef T                                     value_type;
    typedef size_t                           size_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef typename etl::parameter_type<T>::type parameter_t;

    // Indexes for positions in the array.
    enum
    {
      SIZE     = SIZE_,
      MAX_SIZE = SIZE_,
      FRONT    = 0,
      BACK     = SIZE - 1,
      BEGIN    = 0,
      END      = SIZE,
      RBEGIN   = SIZE - 1,
      REND     = -1
    };

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    reference front()
    {
      return *&ARRAY_[FRONT];
    }

    //*************************************************************************
    /// Returns a const reference to the first element.
    //*************************************************************************
    ETL_CONSTEXPR const_reference front() const
    {
      return *&ARRAY_[FRONT];
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    reference back()
    {
      return *&ARRAY_[BACK];
    }

    //*************************************************************************
    /// Returns a const reference to the last element.
    //*************************************************************************
    ETL_CONSTEXPR const_reference back() const
    {
      return *&ARRAY_[BACK];
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal storage.
    //*************************************************************************
    pointer data()
    {
      return &ARRAY_[BEGIN];
    }

    //*************************************************************************
    /// Returns a const pointer to the first element of the internal storage.
    //*************************************************************************
    ETL_CONSTEXPR const_pointer data() const
    {
      return &ARRAY_[BEGIN];
    }

    //*************************************************************************
    /// Returns an iterator to the beginning of the array.
    //*************************************************************************
    iterator begin()
    {
      return &ARRAY_[BEGIN];
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator begin() const
    {
      return &ARRAY_[BEGIN];
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator cbegin() const
    {
      return &ARRAY_[BEGIN];
    }

    //*************************************************************************
    /// Returns an iterator to the end of the array.
    //*************************************************************************
    iterator end()
    {
      return &ARRAY_[END];
    }

    //*************************************************************************
    /// Returns a const iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator end() const
    {
      return &ARRAY_[END];
    }

    //*************************************************************************
    // Returns a const iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator cend() const
    {
      return &ARRAY_[END];
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    reverse_iterator rbegin()
    {
      return reverse_iterator(&ARRAY_[END]);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator rbegin() const
    {
      return const_reverse_iterator(&ARRAY_[END]);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator crbegin() const
    {
      return const_reverse_iterator(&ARRAY_[END]);
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the array.
    //*************************************************************************
    reverse_iterator rend()
    {
      return reverse_iterator(&ARRAY_[BEGIN]);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator rend() const
    {
      return const_reverse_iterator(&ARRAY_[BEGIN]);
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator crend() const
    {
      return const_reverse_iterator(&ARRAY_[BEGIN]);
    }

    //*************************************************************************
    /// Returns the size of the array.
    //*************************************************************************
    ETL_CONSTEXPR size_t size() const
    {
      return SIZE;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the array.
    //*************************************************************************
    ETL_CONSTEXPR size_t max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    reference operator[](size_t i)
    {
      return ARRAY_[i];
    }

    //*************************************************************************
    /// Returns a const reference to the indexed value.
    //*************************************************************************
    ETL_CONSTEXPR const_reference operator[](size_t i) const
    {
      return ARRAY_[i];
    }

    //*************************************************************************
    /// Returns a reference to the indexed value.
    //*************************************************************************
    reference at(size_t i)
    {
      ETL_ASSERT(i < SIZE, ETL_ERROR(etl::array_wrapper_bounds));
      return ARRAY_[i];
    }

    //*************************************************************************
    /// Returns a const reference to the indexed value.
    //*************************************************************************
    const_reference at(size_t i) const
    {
      ETL_ASSERT(i < SIZE, ETL_ERROR(etl::array_wrapper_bounds));
      return ARRAY_[i];
    }

    //*************************************************************************
    /// Fills the array.
    //*************************************************************************
    void fill(parameter_t value)
    {
      std::fill(begin(), end(), value);
    }

    //*************************************************************************
    /// Swaps the contents of arrays.
    //*************************************************************************
    template <typename U, U(&ARRAYOTHER)[SIZE_]>
    typename etl::enable_if<etl::is_same<T, U>::value, void>::type
     swap(etl::array_wrapper<U, SIZE_, ARRAYOTHER>& other)
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        std::swap(ARRAY_[i], other.begin()[i]);
      }
    }
  };

  //*************************************************************************
  /// Equality for array wrappers.
  //*************************************************************************
  template <typename TL, typename TR, size_t SIZEL, size_t SIZER, TL(&ARRAYL)[SIZEL], TR(&ARRAYR)[SIZER]>
  bool operator == (const etl::array_wrapper<TL, SIZEL, ARRAYL>& lhs,
                    const etl::array_wrapper<TR, SIZER, ARRAYR>& rhs)
  {
    return (SIZEL == SIZER) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  //*************************************************************************
  /// Inequality for array wrapper.
  //*************************************************************************
  template <typename TL, typename TR, size_t SIZEL, size_t SIZER, TL(&ARRAYL)[SIZEL], TR(&ARRAYR)[SIZER]>
  bool operator != (const etl::array_wrapper<TL, SIZEL, ARRAYL>& lhs,
                    const etl::array_wrapper<TR, SIZER, ARRAYR>& rhs)
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// Less-than for array wrapper.
  //*************************************************************************
  template <typename TL, typename TR, size_t SIZEL, size_t SIZER, TL(&ARRAYL)[SIZEL], TR(&ARRAYR)[SIZER]>
  bool operator < (const etl::array_wrapper<TL, SIZEL, ARRAYL>& lhs,
                   const etl::array_wrapper<TR, SIZER, ARRAYR>& rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //*************************************************************************
  /// Greater-than for array wrapper.
  //*************************************************************************
  template <typename TL, typename TR, size_t SIZEL, size_t SIZER, TL(&ARRAYL)[SIZEL], TR(&ARRAYR)[SIZER]>
  bool operator > (const etl::array_wrapper<TL, SIZEL, ARRAYL>& lhs,
                   const etl::array_wrapper<TR, SIZER, ARRAYR>& rhs)
  {
    return rhs < lhs;
  }

  //*************************************************************************
  /// Less-than-equal for array wrapper.
  //*************************************************************************
  template <typename TL, typename TR, size_t SIZEL, size_t SIZER, TL(&ARRAYL)[SIZEL], TR(&ARRAYR)[SIZER]>
  bool operator <= (const etl::array_wrapper<TL, SIZEL, ARRAYL>& lhs,
                    const etl::array_wrapper<TR, SIZER, ARRAYR>& rhs)
  {
    return !(lhs > rhs);
  }

  //*************************************************************************
  /// Greater-than-equal for array wrapper.
  //*************************************************************************
  template <typename TL, typename TR, size_t SIZEL, size_t SIZER, TL(&ARRAYL)[SIZEL], TR(&ARRAYR)[SIZER]>
  bool operator >= (const etl::array_wrapper<TL, SIZEL, ARRAYL>& lhs,
                    const etl::array_wrapper<TR, SIZER, ARRAYR>& rhs)
  {
    return !(lhs < rhs);
  }

  //*************************************************************************
  /// Hash function.
  //*************************************************************************
#if ETL_8BIT_SUPPORT
  template <typename T, size_t SIZE, T(&ARRAY)[SIZE]>
  struct hash<etl::array_wrapper<T, SIZE, ARRAY> >
  {
    size_t operator()(const etl::array_wrapper<T, SIZE, ARRAY>& aw) const
    {
      return etl::private_hash::generic_hash<size_t>(reinterpret_cast<const uint8_t*>(&aw[0]),
                                                         reinterpret_cast<const uint8_t*>(&aw[aw.size()]));
    }
  };
#endif
}

//*************************************************************************
/// Swap.
//*************************************************************************
template <typename T, size_t SIZE, T(&ARRAYL)[SIZE], T(&ARRAYR)[SIZE]>
void swap(etl::array_wrapper<T, SIZE, ARRAYL>& lhs,
          etl::array_wrapper<T, SIZE, ARRAYR>& rhs)
{
  lhs.swap(rhs);
}

#define ETL_ARRAY_WRAPPER(arraytype, arrayobject)  etl::array_wrapper<arraytype, ETL_ARRAY_SIZE(arrayobject), arrayobject>

#undef ETL_FILE

#endif

