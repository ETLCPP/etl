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

#ifndef __ETL_DEQUE__
#define __ETL_DEQUE__

#include <stddef.h>
#include <stdint.h>
#include <iterator>
#include <algorithm>

#include "ideque.h"
#include "container.h"
#include "alignment.h"
#include "array.h"

//*****************************************************************************
///\defgroup deque deque
/// A double ended queue with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// A fixed capacity double ended queue.
  ///\note The deque allocates one more element than the specified maximum size.
  ///\tparam T         The type of items this deque holds.
  ///\tparam MAX_SIZE_ The capacity of the deque
  ///\ingroup deque
  //***************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class deque : public ideque<T>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

  private:

    static const size_t BUFFER_SIZE = MAX_SIZE + 1;

  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;
    typedef typename std::iterator_traits<pointer>::difference_type difference_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    deque()
      : ideque<T>(reinterpret_cast<T*>(&buffer[0]), MAX_SIZE, BUFFER_SIZE)
    {
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    deque(const deque& other)
      : ideque<T>(reinterpret_cast<T*>(&buffer[0]), MAX_SIZE, BUFFER_SIZE)
    {
      ideque<T>::assign(other.begin(), other.end());
    }

    //*************************************************************************
    /// Assigns data to the deque.
    //*************************************************************************
    template <typename TIterator>
    deque(TIterator begin, TIterator end)
      : ideque<T>(reinterpret_cast<T*>(&buffer[0]), MAX_SIZE, BUFFER_SIZE)
    {
      ideque<T>::assign(begin, end);
    }

    //*************************************************************************
    /// Assigns data to the deque.
    //*************************************************************************
    explicit deque(size_t n, typename ideque<T>::parameter_t value = value_type())
      : ideque<T>(reinterpret_cast<T*>(&buffer[0]), MAX_SIZE, BUFFER_SIZE)
    {
      ideque<T>::assign(n, value);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    deque& operator =(const deque& rhs)
    {
      if (&rhs != this)
      {
        ideque<T>::assign(rhs.begin(), rhs.end());
      }

      return *this;
    }

  private:

    /// The unititialised buffer of T used in the deque.
    typename etl::aligned_storage<sizeof(T), etl::alignment_of<T>::value>::type buffer[BUFFER_SIZE];
  };
}

#endif
