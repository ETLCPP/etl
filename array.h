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

#ifndef __etl_array__
#define __etl_array__

#include <iterator>
#include <functional>
#include <algorithm>
#include <cstddef>

#include "exception.h"

///\defgroup array array
/// A replacement for std::array if you haven't got C++0x11.
///\ingroup containers

namespace etl
{
#ifdef ETL_USE_EXCEPTIONS
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
  class array_out_of_range_exception : public array_exception
  {
  public:

    array_out_of_range_exception()
      : array_exception("array out of range")
    {
    }
  };
#endif

	//***************************************************************************
  ///\ingroup array
  /// A replacement for std::array if you haven't got C++0x11.
	//***************************************************************************
  template <typename T, const size_t SIZE>
  class array
	{
	public:

		//*************************************************************************
		/// STL style typedefs.
		//*************************************************************************
		typedef T*                                    iterator;
		typedef const T*                              const_iterator;
		typedef std::reverse_iterator<iterator>       reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef T*                                    pointer;
		typedef const T*                              const_pointer;
		typedef T&                                    reference;
		typedef const T&                              const_reference;
		typedef std::ptrdiff_t                        difference_type;
		typedef std::size_t                           size_type;
		typedef T                                     value_type;

		//*************************************************************************
		/// Returns an iterator to the beginning of the array.
		//*************************************************************************
		inline iterator begin()
		{
			return &_buffer[0];
		}
    
		//*************************************************************************
		/// Returns a const iterator to the beginning of the array.
		//*************************************************************************
		inline const_iterator begin() const
		{
			return &_buffer[0];
		}        

		//*************************************************************************
		/// Returns a const iterator to the beginning of the array.
		//*************************************************************************
		inline const_iterator cbegin() const
		{
			return begin();
		} 
  
		//*************************************************************************
		// Returns an reverse iterator to the reverse beginning of the array.
		//*************************************************************************
		inline reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}
    
		//*************************************************************************
		/// Returns a const reverse iterator to the reverse beginning of the array.
		//*************************************************************************
		inline const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}        

		//*************************************************************************
		/// Returns a const reverse iterator to the reverse beginning of the array.
		//*************************************************************************
		inline const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(end());
		} 

		//*************************************************************************
		/// Returns an iterator to the end of the array.
		//*************************************************************************
		inline iterator end()
		{
			return &_buffer[SIZE];
		}
  
		//*************************************************************************
		/// Returns a const iterator to the end of the array.
		//*************************************************************************
		inline const_iterator end() const
		{
			return &_buffer[SIZE];
		}

		//*************************************************************************
		// Returns a const iterator to the end of the array.
		//*************************************************************************
		inline const_iterator cend() const
		{
			return &_buffer[SIZE];
		} 

		//*************************************************************************
		/// Returns a reverse iterator to the end of the array.
		//*************************************************************************
		inline reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}
  
		//*************************************************************************
		/// Returns a const reverse iterator to the end of the array.
		//*************************************************************************
		inline const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		//*************************************************************************
		/// Returns a const reverse iterator to the end of the array.
		//*************************************************************************
		inline const_reverse_iterator crend() const
		{
			return const_reverse_iterator(begin());
		} 

    //*************************************************************************
    /// Returns a reference to the value at index 'i'.
    /// If ETL_USE_EXCEPTIONS is defined then am array_out_of_range_exception is
    /// thown if the index is out of range.
    ///\param i The index of the element to access.
    //*************************************************************************
    inline reference at(size_t i)
    {
#if ETL_USE_EXCEPTIONS
      if (i >= SIZE)
      {
        throw array_out_of_range_exception();
      }
#endif

      return _buffer[i];
    }

    //*************************************************************************
    /// Returns a const reference to the value at index 'i'.
    /// If ETL_USE_EXCEPTIONS is defined then am array_out_of_range_exception is
    /// thown if the index is out of range.
    ///\param i The index of the element to access.
    //*************************************************************************
    inline const_reference at(size_t i) const
    {
#if ETL_USE_EXCEPTIONS
      if (i >= SIZE)
      {
        throw array_out_of_range_exception();
      }
#endif

      return _buffer[i];
    }

    //*************************************************************************
    /// [] operator.
    /// Returns a reference to the value at index 'i'.
    ///\param i The index of the element to access.
    //*************************************************************************
    inline reference operator[](size_t i)
    {
      return _buffer[i];
    }

    //*************************************************************************
    /// [] operator.
    /// Returns a const reference to the value at index 'i'.
    ///\param i The index of the element to access.
    //*************************************************************************
    inline const_reference operator[](size_t i) const
    {
      return _buffer[i];
    }

    //*************************************************************************
    /// Returns <b>true</b> if the array size is zero.
    /// </summary>
    //*************************************************************************
    inline bool empty() const
    {
      return (SIZE == 0);
    }

		//*************************************************************************
		/// Returns the size of the array.
		//*************************************************************************
		inline size_t size() const
		{
			return SIZE;
		}

		//*************************************************************************
		/// Returns the maximum possible size of the array.
		//*************************************************************************
		inline size_t max_size() const
		{
			return SIZE;
		} 

    //*************************************************************************
    /// Fills the array with the specified value.
    ///\param value The value to fill the array with.
    //*************************************************************************
    void fill(const T& value)
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
    
		//*************************************************************************
		/// Returns a reference to the first element.
		//*************************************************************************
		inline reference front()
		{
			return _buffer[0];
		}

		//*************************************************************************
		/// Returns a const reference to the first element.
		//*************************************************************************
		inline const_reference front() const
		{
			return _buffer[0];
		}

		//*************************************************************************
		/// Returns a reference to the last element.
		//*************************************************************************
		inline reference back()
		{
			return _buffer[SIZE - 1];
		}

		//*************************************************************************
		/// Returns a const reference to the last element.
		//*************************************************************************
		inline const_reference back() const
		{
			return _buffer[SIZE - 1];
		}
               
		//*************************************************************************
		/// Returns a pointer to the first element of the internal buffer.
		//*************************************************************************
		inline pointer data()
		{
			return &_buffer[0];
		}
    
		//*************************************************************************
		/// Returns a const pointer to the first element of the internal buffer.
		//*************************************************************************
		inline const_pointer data() const
		{
			return &_buffer[0];
		}
                   	
		T _buffer[SIZE]; ///< The buffer that stores the array data.
	};

  //*************************************************************************
	/// <summary>
	/// Overloaded std::swap for etl::array<T, SIZE>
	/// </summary>
	///\param lhs Reference to the first array.
	///\param rhs Reference to the second array.
	//*************************************************************************
  template <typename T, const size_t SIZE>
  void swap(etl::array<T, SIZE> &lhs, etl::array<T, SIZE> &rhs)
	{
		lhs.swap(rhs);
	}

  //*************************************************************************
  /// <summary>
  /// Equal operator.
  /// </summary>
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
  ///\return <b>true</b> if the arrays are equal, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, std::size_t SIZE>
  bool operator ==(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
  }

  //*************************************************************************
  /// Not equal operator.
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
  ///\return <b>true</b> if the arrays are not equal, otherwise <b>false</b>
  //*************************************************************************
  template <typename T, std::size_t SIZE>
  bool operator !=(const etl::array<T, SIZE>& lhs, const etl::array<T, SIZE>& rhs)
  {
    return !(lhs == rhs);
  }
  
  //*************************************************************************
  /// Less than operator.
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
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
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
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
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
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
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
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
}

#endif

