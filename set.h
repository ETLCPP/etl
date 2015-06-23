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

#ifndef __ETL_SET__
#define __ETL_SET__

#include <stddef.h>
#include <iterator>
#include <functional>

#include "iset.h"
#include "container.h"
#include "pool.h"

//*****************************************************************************
///\defgroup set set
/// A set with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated set implementation that uses a fixed size buffer.
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE_, typename TCompare = std::less<T>>
  class set : public iset<T, TCompare>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    set()
      : iset<T, TCompare>(node_pool, MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    explicit set(const set& other)
      : iset<T, TCompare>(node_pool, MAX_SIZE)
    {
			iset<T, TCompare>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    set(TIterator first, TIterator last)
      : iset<T, TCompare>(node_pool, MAX_SIZE)
    {
      iset<T, TCompare>::insert(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    set& operator = (const set& rhs)
    {
      // Skip if doing self assignment
      if (this != &rhs)
      {
        iset<T, TCompare>::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    /// The pool of data nodes used for the set.
    pool<typename iset<T, TCompare>::Data_Node, MAX_SIZE> node_pool;
  };

}

#endif
