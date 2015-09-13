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

#ifndef __ETL_ARRAY__
#define __ETL_ARRAY__

#include <iterator>
#include <functional>
#include <algorithm>
#include <stddef.h>

#include "exception.h"
#include "type_traits.h"
#include "parameter_type.h"
#include "static_assert.h"
#include "error_handler.h"

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

    array_exception(const char* what)
      : exception(what)
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

    array_out_of_range()
      : array_exception("array: out of range")
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

    typedef typename parameter_type<T>::type parameter_t;

	public:

    enum
    {
      SIZE = SIZE_
    };

    typedef T                                     value_type;
    typedef std::size_t                           size_type;
    typedef std::ptrdiff_t                        difference_type;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T*                                    iterator;
		typedef const T*                              const_iterator;
		typedef std::reverse_iterator<iterator>       reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    //*************************************************************************
    // Element access
    //*************************************************************************

    //*************************************************************************
    /// Returns a reference to the value at index 'i'.
    /// If ETL_THROW_EXCEPTIONS is defined then am array_out_of_range is
    /// thown if the index is out of range.
    ///\param i The index of the element to access.
    //*************************************************************************
    reference at(size_t i)
    {

      if (i >= SIZE)
      {
#ifdef ETL_THROW_EXCEPTIONS
        throw array_out_of_range();
#else
        error_handler::error(array_out_of_range());
#endif
      }

      return _buffer[i];
    }

    //*************************************************************************
    /// Returns a const reference to the value at index 'i'.
    /// If ETL_THROW_EXCEPTIONS is defined then am array_out_of_range is
    /// thown if the index is out of range.
    ///\param i The index of the element to access.
    //*************************************************************************
    const_reference at(size_t i) const
    {
      if (i >= SIZE)
      {
        ETL_ERROR(array_out_of_range());
      }

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
    const_reference operator[](size_t i) const
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
    const_reference front() const
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
    const_reference back() const
    {
      return _buffer[SIZE - 1];
    }

    //*************************************************************************
    /// Returns a pointer to the first element of the internal buffer.
    //*************************************************************************
    pointer data()
    {
      return &_buffer[0];
    }

    //*************************************************************************
    /// Returns a const pointer to the first element of the internal buffer.
    //*************************************************************************
    const_pointer data() const
    {
      return &_buffer[0];
    }

    //*************************************************************************
    // Iterators
    //*************************************************************************

		//*************************************************************************
		/// Returns an iterator to the beginning of the array.
		//*************************************************************************
		iterator begin()
		{
			return &_buffer[0];
		}
    
		//*************************************************************************
		/// Returns a const iterator to the beginning of the array.
		//*************************************************************************
		const_iterator begin() const
		{
			return &_buffer[0];
		}        

		//*************************************************************************
		/// Returns a const iterator to the beginning of the array.
		//*************************************************************************
		const_iterator cbegin() const
		{
			return begin();
		} 

    //*************************************************************************
    /// Returns an iterator to the end of the array.
    //*************************************************************************
    iterator end()
    {
      return &_buffer[SIZE];
    }

    //*************************************************************************
    /// Returns a const iterator to the end of the array.
    //*************************************************************************
    const_iterator end() const
    {
      return &_buffer[SIZE];
    }

    //*************************************************************************
    // Returns a const iterator to the end of the array.
    //*************************************************************************
    const_iterator cend() const
    {
      return &_buffer[SIZE];
    }

		//*************************************************************************
		// Returns an reverse iterator to the reverse beginning of the array.
		//*************************************************************************
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}
    
		//*************************************************************************
		/// Returns a const reverse iterator to the reverse beginning of the array.
		//*************************************************************************
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}        

		//*************************************************************************
		/// Returns a const reverse iterator to the reverse beginning of the array.
		//*************************************************************************
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(end());
		} 

		//*************************************************************************
		/// Returns a reverse iterator to the end of the array.
		//*************************************************************************
		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}
  
		//*************************************************************************
		/// Returns a const reverse iterator to the end of the array.
		//*************************************************************************
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		//*************************************************************************
		/// Returns a const reverse iterator to the end of the array.
		//*************************************************************************
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(begin());
		} 

    //*************************************************************************
    // Capacity
    //*************************************************************************

    //*************************************************************************
    /// Returns <b>true</b> if the array size is zero.
    //*************************************************************************
    bool empty() const
    {
      return (SIZE == 0);
    }

		//*************************************************************************
		/// Returns the size of the array.
		//*************************************************************************
		size_t size() const
		{
			return SIZE;
		}

		//*************************************************************************
		/// Returns the maximum possible size of the array.
		//*************************************************************************
		size_t max_size() const
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
      std::fill(begin(), end(), value);
    }

    //*************************************************************************
    /// Swaps the contents of this array and another.
    ///\param other A reference to the other array.
    //*************************************************************************
    void swap(array& other)
    {
      for (size_t i = 0; i < SIZE; ++i)
      {
        std::swap(_buffer[i], other._buffer[i]);
      }
    }

    /// The array data.
    T _buffer[SIZE];
  };

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
  template <typename T, std::size_t SIZE>
  bool operator ==(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
  }

  //*************************************************************************
  /// Not equal operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, std::size_t SIZE>
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
  template <typename T, std::size_t SIZE>
  bool operator <(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return std::lexicographical_compare(lhs.cbegin(),
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
  template <typename T, std::size_t SIZE>
  bool operator <=(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return !std::lexicographical_compare(lhs.cbegin(),
                                         lhs.cend(), 
                                         rhs.cbegin(),
                                         rhs.cend(),
                                         std::greater<T>());
  }

  //*************************************************************************
  /// Greater than operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the first array is lexicographically greater than the second, otherwise <b>false</b>
  template <typename T, std::size_t SIZE>
  //*************************************************************************
  bool operator >(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return std::lexicographical_compare(lhs.cbegin(),
                                        lhs.cend(), 
                                        rhs.cbegin(), 
                                        rhs.cend(),
                                        std::greater<T>());
  }

  //*************************************************************************
  /// Greater than or equal operator.
  ///\param lhs The first array.
  ///\param rhs The second array.
  ///\return <b>true</b> if the first array is lexicographically greater than or equal to the second, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, std::size_t SIZE>
  bool operator >=(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return !std::lexicographical_compare(lhs.cbegin(), 
                                         lhs.cend(), 
                                         rhs.cbegin(),
                                         rhs.cend());
  }

  //*************************************************************************
  /// Gets a reference to an element in the array.
  ///\tparam I The index.
  ///\tparam T The type.
  ///\tparam N The array size.
  ///\param a The array.
  ///\return A reference to the element
  //*************************************************************************
  template <std::size_t I, typename T, std::size_t N>
  inline T& get(array<T, N>& a)
  {
    STATIC_ASSERT(I < N, "Index out of bounds");
    return a[I];
  }

  //*************************************************************************
  /// Gets a const reference to an element in the array.
  ///\tparam I The index.
  ///\tparam T The type.
  ///\tparam N The array size.
  ///\param a The array.
  ///\return A const reference to the element
  //*************************************************************************
  template <std::size_t I, typename T, std::size_t N>
  inline const T& get(const array<T, N>& a)
  {
    STATIC_ASSERT(I < N, "Index out of bounds");
    return a[I];
  }
}

#endif

