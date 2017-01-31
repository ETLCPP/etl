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

#ifndef __ETL_UNORDERED_MULTIMAP__
#define __ETL_UNORDERED_MULTIMAP__

#include <stddef.h>
#include <iterator>
#include <functional>

#include "iunordered_multimap.h"
#include "container.h"
#include "pool.h"
#include "vector.h"
#include "intrusive_forward_list.h"
#include "hash.h"

//*****************************************************************************
///\defgroup unordered_multimap unordered_multimap
/// A unordered_multimap with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated unordered_multimap implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename TKey, typename TValue, const size_t MAX_SIZE_, const size_t MAX_BUCKETS_ = MAX_SIZE_, typename THash = etl::hash<TKey>, typename TKeyEqual = std::equal_to<TKey> >
  class unordered_multimap : public iunordered_multimap<TKey, TValue, THash, TKeyEqual>
  {
  private:

    typedef iunordered_multimap<TKey, TValue, THash, TKeyEqual> base;

  public:

    static const size_t MAX_SIZE    = MAX_SIZE_;
    static const size_t MAX_BUCKETS = MAX_BUCKETS_;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    unordered_multimap()
      : base(node_pool, buckets, MAX_BUCKETS)
    {
      base::initialise();
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    unordered_multimap(const unordered_multimap& other)
      : base(node_pool, buckets, MAX_BUCKETS)
    {
			base::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    unordered_multimap(TIterator first, TIterator last)
      : base(node_pool, buckets, MAX_BUCKETS)
    {
      base::assign(first, last);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~unordered_multimap()
    {
      base::initialise();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    unordered_multimap& operator = (const unordered_multimap& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        base::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    /// The pool of nodes used for the unordered_multimap.
    etl::pool<typename base::node_t, MAX_SIZE> node_pool;

    /// The buckets of node lists.
    etl::intrusive_forward_list<typename base::node_t> buckets[MAX_BUCKETS_];
  };

}

#endif
