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

#ifndef __ETL_IVECTOR__
#define __ETL_IVECTOR__
#define __ETL_IN_IVECTOR_H__

#include <iterator>
#include <algorithm>
#include <functional>
#include <cstddef>

#include "vector_base.h"

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

    typedef T*                                    iterator;
    typedef const T*                              const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef T*                                    pointer;
    typedef const T*                              const_pointer;
    typedef T&                                    reference;
    typedef const T&                              const_reference;
    typedef size_t                                size_type;
    typedef T                                     value_type;
    typedef typename std::iterator_traits<iterator>::difference_type difference_type;

    //*********************************************************************
    /// Returns an iterator to the beginning of the vector.
    ///\return An iterator to the beginning of the vector.
    //*********************************************************************
    iterator begin()
    {
      return &p_buffer[0];
    }
    
    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator begin() const
    {
      return &p_buffer[0];
    }        

    //*********************************************************************
    /// Returns a const_iterator to the beginning of the vector.
    ///\return A const iterator to the beginning of the vector.
    //*********************************************************************
    const_iterator cbegin() const
    {
      return &p_buffer[0];
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
    /// Returns a const reverse iterator to the reverse beginning of the vector.
    ///\return Const reverse iterator to the reverse beginning of the vector.
    //*********************************************************************
    const_reverse_iterator crbegin() const
    {
	    return const_reverse_iterator(cend());
    } 

    //*********************************************************************
    /// Returns an iterator to the end of the vector.
    ///\return An iterator to the end of the vector.
    //*********************************************************************
    iterator end()
    {
      return &p_buffer[current_size];
    }
  
    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator end() const
    {
      return &p_buffer[current_size];
    }

    //*********************************************************************
    /// Returns a const_iterator to the end of the vector.
    ///\return A const iterator to the end of the vector.
    //*********************************************************************
    const_iterator cend() const
    {
      return &p_buffer[current_size];
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
    /// Returns a const reverse iterator to the end + 1 of the vector.
    ///\return Const reverse iterator to the end + 1 of the vector.
    //*********************************************************************
    const_reverse_iterator crend() const
    {
	    return const_reverse_iterator(cbegin());
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
    /// Resizes the vector.
    /// If ETL_USE_EXCEPTIONS is defined and the new size is larger than the
    /// maximum then a vector_full is thrown.
    ///\param newSize The new size.
    ///\param value   The value to fill new elements with. Default = default contructed value.
    //*********************************************************************
    void resize(size_t newSize, T value = T())
    {
#ifdef ETL_USE_EXCEPTIONS
      if (newSize > MAX_SIZE)
      {
        throw vector_full();
      }
#endif

	    if (newSize > current_size)
	    {
		    std::fill(&p_buffer[current_size], &p_buffer[newSize], value);
	    }

	    current_size = newSize;
    }

    //*********************************************************************
    /// Clears the vector.
    /// Does not call the destructor for any elements.
    //*********************************************************************
    void clear()
    {
	    current_size = 0;
    }

    //*********************************************************************
    /// Assigns values to the vector.
    /// If ETL_USE_EXCEPTIONS is defined, throws vector_full if the vector does not have enough free space.
    /// If ETL_USE_EXCEPTIONS is defined, throws vector_iterator if the iterators are reversed.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*********************************************************************
    template <typename TIterator>
    void assign(TIterator first, TIterator last)
    {
      difference_type count = std::distance(first, last);

      if (count < 0)
      {
#ifdef ETL_USE_EXCEPTIONS
        throw vector_iterator();
#endif
      }
      else if (static_cast<size_t>(count) > MAX_SIZE)
      {
#ifdef ETL_USE_EXCEPTIONS
        throw vector_full();
#endif
      }
      else
      {
        // Safe to copy.
        std::copy(first, last, begin());
        current_size = count;
      }
    }

    //*********************************************************************
    /// Assigns values to the vector.
    /// If ETL_USE_EXCEPTIONS is defined, throws vector_full if the vector does not have enough free space.
    ///\param n     The number of elements to add.
    ///\param value The value to insert for each element.
    //*********************************************************************
    void assign(size_t n, const T& value)
    {
      if (n > MAX_SIZE)
      {
#ifdef ETL_USE_EXCEPTIONS
        throw vector_full();
#endif
      }
      else
      {
        std::fill_n(begin(), n, value);
        current_size = n;
      }
    }

    //*********************************************************************
    /// Inserts a value to the vector.
    /// If ETL_USE_EXCEPTIONS is defined, throws vector_full if the vector is already full.
    ///\param position The position to insert at.
    ///\param value    The value to insert.
    //*********************************************************************
    iterator insert(iterator position, const T& value)
    {
      if (position == end())
      {
        push_back(value);
        return iterator(&back());
      }
      else
      {
        if ((current_size + 1) > MAX_SIZE)
        {
#ifdef ETL_USE_EXCEPTIONS
          throw vector_full();
#endif
        }
        else
        {
          ++current_size;
          std::copy_backward(position, end() - 1, end());
          *position = value;
        }

        return position;
      }
    }

    //*********************************************************************
    /// Inserts 'n' values to the vector.
    /// If ETL_USE_EXCEPTIONS is defined, throws vector_full if the vector does not have enough free space.
    ///\param position The position to insert at.
    ///\param n        The number of elements to add.
    ///\param value    The value to insert.
    //*********************************************************************
    void insert(iterator position, size_t n, const T& value)
    {
      if ((current_size + n) > MAX_SIZE)
      {
#ifdef ETL_USE_EXCEPTIONS
        throw vector_full();
#endif
      }
      else
      {
        current_size += n;
        std::copy_backward(position, end() - n, end());
        std::fill_n(position, n, value);
      }
    }

    //*********************************************************************
    /// Inserts a range of values to the vector.
    /// If ETL_USE_EXCEPTIONS is defined, throws vector_full if the vector does not have enough free space.
    ///\param position The position to insert at.
    ///\param first    The first element to add.
    ///\param last     The last + 1 element to add.
    //*********************************************************************
    template <class TIterator>
    void insert(iterator position, TIterator first, TIterator last)
    {
      size_t count = std::distance(first, last);

      if ((current_size + count) > MAX_SIZE)
      {
#ifdef ETL_USE_EXCEPTIONS
        throw vector_full();
#endif
      }
      else
      {
        current_size += count;
        std::copy_backward(position, end() - count, end());
        std::copy(first, first + count, position);
      }
    }

    //*********************************************************************
    /// Inserts a value at the end of the vector.
    /// If ETL_USE_EXCEPTIONS is defined, throws vector_full if the vector is already full.
    ///\param value The value to add.
    //*********************************************************************
    void push_back(const T& value)
    {
      if (current_size == MAX_SIZE)
      {
#ifdef ETL_USE_EXCEPTIONS
        throw vector_full();
#endif
      }
      else
      {
        p_buffer[current_size++] = value;
      }
    }

    //*********************************************************************
    /// Increases the size of the vector by one, but does not initialise the new element.
    /// If ETL_USE_EXCEPTIONS is defined, throws vector_full if the vector is already full.
    //*********************************************************************
    void push_back()
    {
      if (current_size == MAX_SIZE)
      {
#ifdef ETL_USE_EXCEPTIONS
        throw vector_full();
#endif
      }
      else
      {
        ++current_size;
      }
    }

    //*********************************************************************
    /// Removes an element from the end of the vector.
    /// Does nothing if the vector is empty.
    //*********************************************************************
    void pop_back()
    {
	    if (current_size > 0)
	    {
		    --current_size;
	    }
    }

    //*********************************************************************
    /// Erases an element.
    ///\param iElement Iterator to the element.
    ///\return An iterator pointing to the element that followed the erased element.
    //*********************************************************************
    iterator erase(iterator iElement)
    {
	    std::copy(iElement + 1, end(), iElement);
	    --current_size;

	    return iElement;
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
	    std::copy(last, end(), first);
	    current_size -= std::distance(first, last);

	    return first;
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
      return p_buffer[current_size - 1];
    }

    //*********************************************************************
    /// Returns a const reference to the last element.
    ///\return A const reference to the last element.
    //*********************************************************************
    const_reference back() const
    {
      return p_buffer[current_size - 1];
    }

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    /// If ETL_USE_EXCEPTIONS is defined, throws a std::range_error if the index is out of range.
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference at(size_t i)
    {
#ifdef ETL_USE_EXCEPTIONS
      if (i >= current_size)
      {
        throw vector_out_of_bounds();
      }
#endif

      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    /// If ETL_USE_EXCEPTIONS is defined, throws a std::range_error if the index is out of range.
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference at(size_t i) const
    {
#ifdef ETL_USE_EXCEPTIONS
      if (i >= current_size)
      {
        throw vector_out_of_bounds();
      }
#endif

      return p_buffer[i];
    } 

    //*********************************************************************
    /// Returns a reference to the value at index 'i'
    ///\param i The index.
    ///\return A reference to the value at index 'i'
    //*********************************************************************
    reference operator[](size_t i)
    {
      return p_buffer[i];
    }

    //*********************************************************************
    /// Returns a const reference to the value at index 'i'
    ///\param i The index.
    ///\return A const reference to the value at index 'i'
    //*********************************************************************
    const_reference operator[](size_t i) const
    {
      return p_buffer[i];
    } 
   
    //*********************************************************************
    /// Assignment operator.
    /// The source vector can be larger than the destination, but 
    /// only the elements that will fit in the destination will be copied.
    ///\param other The other vector.
    //*********************************************************************
    ivector& operator = (ivector& other)
    {
      const size_t length = std::min(MAX_SIZE, other.size());

      std::copy(other.begin(), other.begin() + length, p_buffer);
      current_size = length;

      return *this;
    }

  protected:

    //*********************************************************************
    /// Constructor.
    //*********************************************************************
    ivector(T* p_buffer, size_t MAX_SIZE)
      : vector_base(MAX_SIZE),
        p_buffer(p_buffer)
    {
    }

  private:

    T* p_buffer;
  };

  //***************************************************************************
  /// Equal operator.
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
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
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
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
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
  ///\return <b>true</b> if the first array is lexigraphically less than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator <(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  //***************************************************************************
  /// Greater than operator.
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
  ///\return <b>true</b> if the first array is lexigraphically greater than the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator >(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::greater<T>());
  }

  //***************************************************************************
  /// Less than or equal operator.
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
  ///\return <b>true</b> if the first array is lexigraphically less than or equal to the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator <=(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return !operator >(lhs, rhs);
  }

  //***************************************************************************
  /// Greater than or equal operator.
  ///\param lhs Reference to the first array.
  ///\param rhs Reference to the second array.
  ///\return <b>true</b> if the first array is lexigraphically greater than or equal to the second, otherwise <b>false</b>
  ///\ingroup vector
  //***************************************************************************
  template <typename T>
  bool operator >=(const etl::ivector<T>& lhs, const etl::ivector<T>& rhs)
  {
    return !operator <(lhs, rhs);
  }
}

#undef __etl_in_ivector_h__
#endif
