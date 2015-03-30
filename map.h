///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2014 jwellbelove, rlindeman

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

#ifndef __ETL_MAP__
#define __ETL_MAP__

#include <stddef.h>
#include <iterator>
#include <functional>

#include "imap.h"
#include "container.h"
#include "pool.h"

//*****************************************************************************
///\defgroup map map
/// A map with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated map implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, typename TCompare = std::less<TKey>>
  class map : public imap<TKey, TValue, TCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    map()
      : imap<TKey, TValue, TCompare>(node_pool, MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    map(const map& other)
      : imap<TKey, TValue, TCompare>(node_pool, MAX_SIZE)
    {
			imap<TKey, TValue, TCompare>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    map(TIterator first, TIterator last)
      : imap<TKey, TValue, TCompare>(node_pool, MAX_SIZE)
    {
      imap<TKey, TValue, TCompare>::insert(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    map& operator = (const map& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        imap<TKey, TValue, TCompare>::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    /// The pool of data nodes used for the map.
    pool<typename imap<TKey, TValue, TCompare>::Data_Node, MAX_SIZE> node_pool;
  };

}

#endif
