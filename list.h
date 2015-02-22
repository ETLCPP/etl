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

#include "ilist.h"
#include "container.h"
#include "pool.h"

//*****************************************************************************
///\defgroup list list
/// A linked list with the capacity defined at compile time.
///\ingroup containers
//*****************************************************************************

namespace etl
{
  //*************************************************************************
  /// A templated list implementation that uses a fixed size buffer.
  ///\note 'merge' and 'splice' and are not supported.
  //*************************************************************************
  template <typename T, const size_t MAX_SIZE_>
  class list : public ilist<T>
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
    list()
      : ilist<T>(node_pool, MAX_SIZE)
    {
    }

    //*************************************************************************
    /// Construct from size.
    //*************************************************************************
    explicit list(size_t initialSize)
      : ilist<T>(node_pool, MAX_SIZE)
    {
      ilist<T>::assign(initialSize, T());
    }

    //*************************************************************************
    /// Construct from size and value.
    //*************************************************************************
    list(size_t initialSize, typename ilist<T>::parameter_t value)
      : ilist<T>(node_pool, MAX_SIZE)
    {
      ilist<T>::assign(initialSize, value);
    }

    //*************************************************************************
    /// Copy constructor.
    //*************************************************************************
    list(const list& other)
      : ilist<T>(node_pool, MAX_SIZE)
    {
      ilist<T>::assign(other.cbegin(), other.cend());
    }

    //*************************************************************************
    /// Construct from range.
    //*************************************************************************
    template <typename TIterator>
    list(TIterator first, TIterator last)
      : ilist<T>(node_pool, MAX_SIZE)
    {
      ilist<T>::assign(first, last);
    }

    //*************************************************************************
    /// Assignment operator.
    //*************************************************************************
    list& operator = (const list& rhs)
    {
      if (&rhs != this)
      {
        ilist<T>::assign(rhs.cbegin(), rhs.cend());
      }

      return *this;
    }

  private:

    /// The pool of nodes used in the list.
    etl::pool<typename ilist<T>::Data_Node, MAX_SIZE> node_pool;
  };
}

#endif
