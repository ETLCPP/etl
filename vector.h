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

#ifndef __ETL_VECTOR__
#define __ETL_VECTOR__

#include <cstddef>
#include <iterator>

#include "ivector.h"

//*****************************************************************************
///\defgroup vector vector
/// A vector with the capacity defined at compile time.
///\note Uses a predefined array, so MAX_SIZE_ elements will be always be constructed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  template <typename T, const size_t MAX_SIZE_>
  //***************************************************************************
  /// A vector implementation that uses a fixed size buffer.
  ///\note Uses a predefined array, so MAX_SIZE_ elements will be always be constructed.
  ///\tparam T The element type.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup vector
  //***************************************************************************
  class vector : public ivector<T>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    vector()
      : ivector<T>(buffer, MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Constructor, with size.
    ///\param initialSize The initial size of the vector.
    //*************************************************************************
    explicit vector(size_t initialSize)
      : ivector<T>(buffer, MAX_SIZE)
    {
      ivector<T>::resize(initialSize);
    }

    //*************************************************************************
    /// Constructor, from initial size and value.
    ///\param initialSize  The initial size of the vector.
    ///\param value        The value to fill the vector with.
    //*************************************************************************
    vector(size_t initialSize, const T& value)
      :  ivector<T>(buffer, MAX_SIZE)
    {
      ivector<T>::resize(initialSize, value);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    vector(TIterator first, TIterator last)
      :  ivector<T>(buffer, MAX_SIZE)
    {
      ivector<T>::assign(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    vector& operator = (const vector& rhs)
    {
      ivector<T>::assign(rhs.cbegin(), rhs.cend());

      return *this;
    }
		
  private:

    T buffer[MAX_SIZE]; ///<The array that stores the elements.
  };
}

#endif
