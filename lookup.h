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

#ifndef __ETL_LOOKUP__
#define __ETL_LOOKUP__

#include <stddef.h>
#include <iterator>
#include <functional>

#include "ilookup.h"
#include "vector.h"

//*****************************************************************************
///\defgroup lookup lookup
/// A lookup table with the capacity defined at compile time.
/// Has insertion of O(N) and lookup of O(logN)
/// Duplicate entries and not allowed.
///\note Uses a predefined array, so MAX_SIZE_ elements will be always be constructed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, typename TCompare = std::less<TKey>>
  //***************************************************************************
  /// A lookup implementation that uses a fixed size buffer.
  ///\note Uses a predefined array, so MAX_SIZE_ elements will be always be constructed.
  ///\tparam T The element type.
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup lookup
  //***************************************************************************
  class lookup : public ilookup<TKey, TValue, TCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    lookup()
      : ilookup<TKey, TValue, TCompare>(buffer)
    {
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    lookup(TIterator first, TIterator last)
      : ilookup<TKey, TValue, TCompare>(buffer)
    {
      ilookup<TKey, TValue, TCompare>::insert(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    lookup& operator = (const lookup& rhs)
    {
      ilookup<TKey, TValue, TCompare>::clear();
      ilookup<TKey, TValue, TCompare>::insert(rhs.cbegin(), rhs.cend());

      return *this;
    }

    //*************************************************************************
    /// Swap
    //*************************************************************************
    void swap(lookup& other)
    {
      buffer.swap(other.buffer);
    }

  private:

    etl::vector<typename ilookup<TKey, TValue, TCompare>::value_type, MAX_SIZE> buffer; ///<The vector that stores the elements.
  };

  //*************************************************************************
  /// Swap
  //*************************************************************************
  template <typename TKey, typename TValue, const size_t MAX_SIZE, typename TCompare>
  void swap(etl::lookup<TKey, TValue, MAX_SIZE, TCompare>& first, etl::lookup<TKey, TValue, MAX_SIZE, TCompare>& second)
  {
    first.swap(second);
  }
}

#endif
