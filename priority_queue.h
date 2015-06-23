///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2015 jwellbelove, rlindeman

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

#ifndef __ETL_PRIORITY_QUEUE__
#define __ETL_PRIORITY_QUEUE__

#include <stddef.h>
#include <functional>

#include "ipriority_queue.h"
#include "container.h"
#include "vector.h"

//*****************************************************************************
///\defgroup queue queue
/// A priority queue with the capacity defined at compile time,
/// written in the STL style.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup queue
  /// A fixed capacity queue.
  /// This queue does not support concurrent access by different threads.
  /// \tparam T    The type this queue should support.
  /// \tparam SIZE The maximum capacity of the queue.
  //***************************************************************************
  template <typename T, const size_t SIZE, typename TContainer = etl::vector<T, SIZE>, typename TCompare = std::less<typename TContainer::value_type> >
  class priority_queue : public ipriority_queue<T, TContainer, TCompare>
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    priority_queue()
      : ipriority_queue<T, TContainer, TCompare>(SIZE)
    {
    }

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    priority_queue(const priority_queue& rhs)
      : ipriority_queue<T, TContainer, TCompare>(SIZE)
    {
      ipriority_queue<T, TContainer, TCompare>::clone(rhs);
    }

    //*************************************************************************
    /// Constructor, from an iterator range.
    ///\tparam TIterator The iterator type.
    ///\param first The iterator to the first element.
    ///\param last  The iterator to the last element + 1.
    //*************************************************************************
    template <typename TIterator>
    priority_queue(TIterator first, TIterator last)
      : ipriority_queue<T, TContainer, TCompare>(SIZE)
    {
      ipriority_queue<T, TContainer, TCompare>::assign(first, last);
    }

    //*************************************************************************
    /// Destructor.
    //*************************************************************************
    ~priority_queue()
    {
      ipriority_queue<T, TContainer, TCompare>::clear();
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    priority_queue& operator = (const priority_queue& rhs)
    {
      if (&rhs != this)
      {
        ipriority_queue<T, TContainer, TCompare>::clone(rhs);
      }

      return *this;
    }
  };
}

#endif
