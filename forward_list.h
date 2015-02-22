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

#ifndef __ETL_LIST__
#define __ETL_LIST__

#include <stddef.h>

#include "pool.h"
#include "iforward_list.h"
#include "container.h"

//*****************************************************************************
///\defgroup forward_list forward_list
/// A linked forward_list with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated forward_list implementation that uses a fixed size pool.
  ///\note 'merge' and 'splice_after' and are not supported.
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class forward_list : public iforward_list<T>
  {
  public:

    static const size_t MAX_SIZE = MAX_SIZE_;

  public:

    typedef T        value_type;
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef T&       reference;
    typedef const T& const_reference;
    typedef size_t   size_type;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    forward_list()
      : iforward_list<T>(node_pool, MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Construct from size and value.
    //*************************************************************************
    explicit forward_list(size_t initialSize, typename iforward_list<T>::parameter_t value = T())
      : iforward_list<T>(node_pool, MAX_SIZE)
    {
      iforward_list<T>::assign(initialSize, value);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    forward_list(const forward_list& other)
      : iforward_list<T>(node_pool, MAX_SIZE)
    {
			iforward_list<T>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Construct from range.
    //*************************************************************************
    template <typename TIterator>
    forward_list(TIterator first, TIterator last)
      : iforward_list<T>(node_pool, MAX_SIZE)
    {
      iforward_list<T>::assign(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    forward_list& operator = (const forward_list& rhs)
    {
      if (&rhs != this)
      {
        iforward_list<T>::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    /// The pool of nodes used in the list.
    etl::pool<typename iforward_list<T>::Data_Node, MAX_SIZE> node_pool;
  };
}

#endif
