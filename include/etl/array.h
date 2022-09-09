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

#ifndef ETL_ARRAY_INCLUDED
#define ETL_ARRAY_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "iterator.h"
#include "functional.h"
#include "exception.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "static_assert.h"
#include "error_handler.h"
#include "nth_type.h"
#include "initializer_list.h"

#include <stddef.h>

///\defgroup array array
/// A replacement for std::array if you haven't got C++0x11.
///\ingroup containers

namespace etl
{
  //***************************************************************************
  ///\ingroup array
  /// The base class for array exceptions.
  //***************************************************************************
  class array_exception : public exception
  {
  public:

    array_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup array
  /// The out of range exceptions.
  //***************************************************************************
  class array_out_of_range : public array_exception
  {
  public:

    array_out_of_range(string_type file_name_, numeric_type line_number_)
      : array_exception("array:range", file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup array
  /// A replacement for std::array if you haven't got C++0x11.
  //***************************************************************************
  template <typename T, const size_t SIZE_>
  class array
  {
  private:

    typedef typename etl::parameter_type<T>::type parameter_t;

  public:

    enum
    {
      SIZE = SIZE_
    };

    typedef T                                     value_type;
    typedef size_t                                size_type;
    typedef ptrdiff_t                             difference_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef ETL_OR_STD::reverse_iterator<iterator>       reverse_iterator;
    typedef ETL_OR_STD::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    // Element access
    //*************************************************************************

    //*************************************************************************
    /// Returns a reference to the value at index 'i'.
    ///\param i The index of the element to access.
    //*************************************************************************
    reference at(size_t i)
    {
      ETL_ASSERT(i < SIZE, ETL_ERROR(array_out_of_range));

      return _buffer[i];
    }

    //*************************************************************************
    /// Returns a const reference to the value at index 'i'.
    ///\param i The index of the element to access.
    //*************************************************************************
    const_reference at(size_t i) const
    {
      ETL_ASSERT(i < SIZE, ETL_ERROR(array_out_of_range));

      return _buffer[i];
    }

    //*************************************************************************
    /// [] operator.
    /// Returns a reference to the value at index 'i'.
    ///\param i The index of the element to access.
    //*************************************************************************
    reference operator[](size_t i)
    {
      return _buffer[i];
    }

    //*************************************************************************
    /// [] operator.
    /// Returns a const reference to the value at index 'i'.
    ///\param i The index of the element to access.
    //*************************************************************************
    ETL_CONSTEXPR const_reference operator[](size_t i) const
    {
      return _buffer[i];
    }

    //*************************************************************************
    /// Returns a reference to the first element.
    //*************************************************************************
    reference front()
    {
      return _buffer[0];
    }

    //*************************************************************************
    /// Returns a const reference to the first element.
    //*************************************************************************
    ETL_CONSTEXPR const_reference front() const
    {
      return _buffer[0];
    }

    //*************************************************************************
    /// Returns a reference to the last element.
    //*************************************************************************
    reference back()
    {
      return _buffer[SIZE - 1];
    }

    //*************************************************************************
    /// Returns a const reference to the last element.
    //*************************************************************************
    ETL_CONSTEXPR const_reference back() const
    {
      return _buffer[SIZE - 1];
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal buffer.
    //*************************************************************************
    pointer data() ETL_NOEXCEPT
    {
      return &_buffer[0];
    }

    //*************************************************************************
    /// Returns a const pointer to the first element of the internal buffer.
    //*************************************************************************
    ETL_CONSTEXPR const_pointer data() const ETL_NOEXCEPT
    {
      return &_buffer[0];
    }

    //*************************************************************************
    // Iterators
    //*************************************************************************

    //*************************************************************************
    /// Returns an iterator to the beginning of the array.
    //*************************************************************************
    iterator begin() ETL_NOEXCEPT
    {
      return &_buffer[0];
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator begin() const ETL_NOEXCEPT
    {
      return &_buffer[0];
    }

    //*************************************************************************
    /// Returns a const iterator to the beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator cbegin() const ETL_NOEXCEPT
    {
      return begin();
    }

    //*************************************************************************
    /// Returns an iterator to the end of the array.
    //*************************************************************************
    iterator end() ETL_NOEXCEPT
    {
      return &_buffer[SIZE];
    }

    //*************************************************************************
    /// Returns a const iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator end() const ETL_NOEXCEPT
    {
      return &_buffer[SIZE];
    }

    //*************************************************************************
    // Returns a const iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_iterator cend() const ETL_NOEXCEPT
    {
      return &_buffer[SIZE];
    }

    //*************************************************************************
    // Returns an reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    reverse_iterator rbegin() ETL_NOEXCEPT
    {
      return reverse_iterator(end());
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator rbegin() const ETL_NOEXCEPT
    {
      return const_reverse_iterator(end());
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the reverse beginning of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator crbegin() const ETL_NOEXCEPT
    {
      return const_reverse_iterator(end());
    }

    //*************************************************************************
    /// Returns a reverse iterator to the end of the array.
    //*************************************************************************
    reverse_iterator rend() ETL_NOEXCEPT
    {
      return reverse_iterator(begin());
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator rend() const ETL_NOEXCEPT
    {
      return const_reverse_iterator(begin());
    }

    //*************************************************************************
    /// Returns a const reverse iterator to the end of the array.
    //*************************************************************************
    ETL_CONSTEXPR const_reverse_iterator crend() const ETL_NOEXCEPT
    {
      return const_reverse_iterator(begin());
    }

    //*************************************************************************
    // Capacity
    //*************************************************************************

    //*************************************************************************
    /// Returns <b>true</b> if the array size is zero.
    //*************************************************************************
    ETL_CONSTEXPR bool empty() const ETL_NOEXCEPT
    {
      return (SIZE == 0);
    }

    //*************************************************************************
    /// Returns the size of the array.
    //*************************************************************************
    ETL_CONSTEXPR size_t size() const ETL_NOEXCEPT
    {
      return SIZE;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the array.
    //*************************************************************************
    ETL_CONSTEXPR size_t max_size() const ETL_NOEXCEPT
    {
      return SIZE;
    }

    //*************************************************************************
    // Operations
    //*************************************************************************

    //*************************************************************************
    /// Fills the array with the specified value.
    ///\param value The value to fill the array with.
    //*************************************************************************
    void fill(parameter_t value)
    {
      etl::fill(begin(), end(), value);
    }

    //*************************************************************************
    /// Swaps the contents of this array and another.
    ///\param other A reference to the other array.
    //*************************************************************************
    void swap(array& other) ETL_NOEXCEPT
    {
      using ETL_OR_STD::swap; // Allow ADL

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        swap(_buffer[i], other._buffer[i]);
      }
    }

    //*************************************************************************
    /// Fills the array from the range.
    /// If the range is smaller than the array then the unused array elements are left unmodified.
    ///\param first The iterator to the first item in the range.
    ///\param last  The iterator to one past the final item in the range.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, const TIterator last)
    {
      etl::copy_s(first, last, begin(), end());
    }

    //*************************************************************************
    /// Fills the array from the range.
    /// If the range is smaller than the array then the unused array elements are initialised with the supplied value.
    ///\param first The iterator to the first item in the range.
    ///\param last  The iterator to one past the final item in the range.
    //*************************************************************************
    template <typename TIterator>
    void assign(TIterator first, const TIterator last, parameter_t value)
    {
      // Copy from the range.
      iterator p = etl::copy(first, last, begin());

      // Initialise any that are left.
      etl::fill(p, end(), value);
    }

    //*************************************************************************
    /// Inserts a value into the array.
    ///\param position The index of the position to insert at.
    ///\param value    The value to insert.
    //*************************************************************************
    inline iterator insert_at(size_t position, parameter_t value)
    {
      return insert(begin() + position, value);
    }

    //*************************************************************************
    /// Inserts a value into the array.
    ///\param position The iterator to the position to insert at.
    ///\param value    The value to insert.
    //*************************************************************************
    iterator insert(const_iterator position, parameter_t value)
    {
      iterator p = to_iterator(position);

      etl::move_backward(p, end() - 1, end());
      *p = value;

      return p;
    }

    //*************************************************************************
    /// Insert into the array from the range.
    ///\param position The position to insert at.
    ///\param first    The iterator to the first item in the range.
    ///\param last     The iterator to one past the final item in the range.
    //*************************************************************************
    template <typename TIterator>
    inline iterator insert_at(size_t position, TIterator first, const TIterator last)
    {
      return insert(begin() + position, first, last);
    }

    //*************************************************************************
    /// Insert into the array from the range.
    ///\param position The position to insert at.
    ///\param first    The iterator to the first item in the range.
    ///\param last     The iterator to one past the final item in the range.
    //*************************************************************************
    template <typename TIterator>
    iterator insert(const_iterator position, TIterator first, const TIterator last)
    {
      iterator p = to_iterator(position);
      iterator result(p);

      size_t source_size       = etl::distance(first, last);
      size_t destination_space = etl::distance(position, cend());

      // Do we need to move anything?
      if (source_size < destination_space)
      {
        size_t length = SIZE - (etl::distance(begin(), p) + source_size);
        etl::move_backward(p, p + length, end());
      }

      // Copy from the range.
      etl::copy_s(first, last, p, end());

      return result;
    }

    //*************************************************************************
    /// Erases a value from the array.
    /// After erase, the last value in the array will be unmodified.
    ///\param position The index of the position to erase at.
    //*************************************************************************
    inline iterator erase_at(size_t position)
    {
      return erase(begin() + position);
    }

    //*************************************************************************
    /// Erases a value from the array.
    /// After erase, the last value in the array will be unmodified.
    ///\param position The iterator to the position to erase at.
    //*************************************************************************
    iterator erase(const_iterator position)
    {
      iterator p = to_iterator(position);
      etl::move(p + 1, end(), p);

      return p;
    }

    //*************************************************************************
    /// Erases a range of values from the array.
    /// After erase, the last values in the array will be unmodified.
    ///\param first The first item to erase.
    ///\param last  The one past the last item to erase.
    //*************************************************************************
    iterator erase_range(size_t first, size_t last)
    {
      return erase(begin() + first, begin() + last);
    }

    //*************************************************************************
    /// Erases a range of values from the array.
    /// After erase, the last values in the array will be unmodified.
    ///\param first The first item to erase.
    ///\param last  The one past the last item to erase.
    //*************************************************************************
    iterator erase(const_iterator first, const_iterator last)
    {
      iterator p = to_iterator(first);
      etl::move(last, cend(), p);
      return p;
    }

    //*************************************************************************
    /// Erases a value from the array.
    ///\param position The index of the position to erase at.
    ///\param value    The value to use to overwrite the last element in the array.
    //*************************************************************************
    inline iterator erase_at(size_t position, parameter_t value)
    {
      return erase(begin() + position, value);
    }

    //*************************************************************************
    /// Erases a value from the array.
    ///\param position The iterator to the position to erase at.
    ///\param value    The value to use to overwrite the last element in the array.
    //*************************************************************************
    iterator erase(const_iterator position, parameter_t value)
    {
      iterator p = to_iterator(position);

      etl::move(p + 1, end(), p);
      back() = value;

      return p;
    }

    //*************************************************************************
    /// Erases a range of values from the array.
    ///\param first The first item to erase.
    ///\param last  The one past the last item to erase.
    ///\param value The value to use to overwrite the last elements in the array.
    //*************************************************************************
    iterator erase_range(size_t first, size_t last, parameter_t value)
    {
      return erase(begin() + first, begin() + last, value);
    }

    //*************************************************************************
    /// Erases a range of values from the array.
    ///\param position The iterator to the position to erase at.
    ///\param value    The value to use to overwrite the last elements in the array.
    //*************************************************************************
    iterator erase(const_iterator first, const_iterator last, parameter_t value)
    {
      iterator p = to_iterator(first);

      p = etl::move(last, cend(), p);
      etl::fill(p, end(), value);

      return to_iterator(first);
    }

    /// The array data.
    T _buffer[SIZE];

  private:

    //*************************************************************************
    /// Convert from const_iterator to iterator
    //*************************************************************************
    iterator to_iterator(const_iterator itr) const
    {
      return const_cast<iterator>(itr);
    }
  };

  //*************************************************************************
  /// Template deduction guides.
  //*************************************************************************
#if ETL_USING_CPP17
  template <typename... T>
  array(T...) -> array<typename etl::common_type<T...>::type, sizeof...(T)>;
#endif  

  //*************************************************************************
  /// Make
  //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
  template <typename T, typename... TValues>
  constexpr auto make_array(TValues&&... values) -> etl::array<T, sizeof...(TValues)>
  {
    return { { etl::forward<T>(values)... } };
  }
#endif

  //*************************************************************************
  /// Overloaded swap for etl::array<T, SIZE>
  ///\param lhs The first array.
  ///\param rhs The second array.
  //*************************************************************************
  template <typename T, const size_t SIZE>
  void swap(etl::array<T, SIZE> &lhs, etl::array<T, SIZE> &rhs)
  {
    lhs.swap(rhs);
  }

  //*************************************************************************
  /// Equal operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, size_t SIZE>
  bool operator ==(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return etl::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
  }

  //*************************************************************************
  /// Not equal operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, size_t SIZE>
  bool operator !=(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return !(lhs == rhs);
  }

  //*************************************************************************
  /// Less than operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the first array is lexicographically less than the second, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, size_t SIZE>
  bool operator <(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return etl::lexicographical_compare(lhs.cbegin(),
                                        lhs.cend(),
                                        rhs.cbegin(),
                                        rhs.cend());
  }

  //*************************************************************************
  /// Less than or equal operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the first array is lexicographically less than or equal to the second, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, size_t SIZE>
  bool operator <=(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return !(lhs > rhs);
  }

  //*************************************************************************
  /// Greater than operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the first array is lexicographically greater than the second, otherwise <b>false</b>
  template <typename T, size_t SIZE>
  //*************************************************************************
  bool operator >(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return (rhs < lhs);
  }

  //*************************************************************************
  /// Greater than or equal operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the first array is lexicographically greater than or equal to the second, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, size_t SIZE>
  bool operator >=(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return !(lhs < rhs);
  }

  //*************************************************************************
  /// Gets a reference to an element in the array.
  ///\tparam I The index.
  ///\tparam T The type.
  ///\tparam MAXN The array size.
  ///\param a The array.
  ///\return A reference to the element
  //*************************************************************************
  template <size_t I, typename T, size_t MAXN>
  inline T& get(array<T, MAXN>& a)
  {
    ETL_STATIC_ASSERT(I < MAXN, "Index out of bounds");
    return a[I];
  }

  //*************************************************************************
  /// Gets a const reference to an element in the array.
  ///\tparam I The index.
  ///\tparam T The type.
  ///\tparam MAXN The array size.
  ///\param a The array.
  ///\return A const reference to the element
  //*************************************************************************
  template <size_t I, typename T, size_t MAXN>
  inline const T& get(const array<T, MAXN>& a)
  {
    ETL_STATIC_ASSERT(I < MAXN, "Index out of bounds");
    return a[I];
  }
}

#endif
