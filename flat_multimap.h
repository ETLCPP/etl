///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2015 jwellbelove

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

#ifndef __ETL_FLAT_MULTMAP__
#define __ETL_FLAT_MULTMAP__

#include <stddef.h>
#include <iterator>
#include <functional>

#include "iflat_multimap.h"
#include "vector.h"

//*****************************************************************************
///\defgroup flat_multimap flat_multimap
/// A flat_multimapmap with the capacity defined at compile time.
/// Has insertion of O(N) and find of O(logN)
/// Duplicate entries and not allowed.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, typename TCompare = std::less<TKey>>
  //***************************************************************************
  /// A flat_multimap implementation that uses a fixed size buffer.
  ///\tparam TKey     The key type.
  ///\tparam TValue   The value type.
  ///\tparam TCompare The type to compare keys. Default = std::less<TKey>
  ///\tparam MAX_SIZE_ The maximum number of elements that can be stored.
  ///\ingroup flat_multimap
  //***************************************************************************
  class flat_multimap : public iflat_multimap<TKey, TValue, TCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    flat_multimap()
      : iflat_multimap<TKey, TValue, TCompare>(buffer)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    flat_multimap(const flat_multimap& other)
      : iflat_multimap<TKey, TValue, TCompare>(buffer)
    {
      iflat_multimap<TKey, TValue, TCompare>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    flat_multimap(TIterator first, TIterator last)
      : iflat_multimap<TKey, TValue, TCompare>(buffer)
    {
      iflat_multimap<TKey, TValue, TCompare>::insert(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    flat_multimap& operator = (const flat_multimap& rhs)
    {
      if (&rhs != this)
      {
        iflat_multimap<TKey, TValue, TCompare>::clear();
        iflat_multimap<TKey, TValue, TCompare>::insert(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    etl::vector<typename iflat_multimap<TKey, TValue, TCompare>::value_type, MAX_SIZE> buffer; ///<The vector that stores the elements.
  };
}

#endif
