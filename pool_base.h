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

#ifndef __ETL_IN_IPOOL_H__
#error This header is a private element of etl::pool & etl::ipool
#endif

#ifndef __ETL_POOL_BASE__
#define __ETL_POOL_BASE__

#include <stddef.h>

#include "exception.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  /// The base class for pool exceptions.
  ///\ingroup pool
  //***************************************************************************
  class pool_exception : public exception
  {
  public:

    pool_exception(const char* what)
      : exception(what)
    {}
  };

  //***************************************************************************
  /// The exception thrown when the pool has no more free items.
  ///\ingroup pool
  //***************************************************************************
  class pool_no_allocation : public pool_exception
  {
  public:

    pool_no_allocation()
      : pool_exception("pool: no allocation")
    {}
  };

  //***************************************************************************
  /// The exception thrown when an object is released which does not belong to the pool.
  ///\ingroup pool
  //***************************************************************************
  class pool_object_not_in_pool : public pool_exception
  {
  public:

    pool_object_not_in_pool()
      : pool_exception("pool: not in pool")
    {}
  };
  
  //*************************************************************************
  /// The base class for all templated pool types.
  ///\ingroup pool
  //*************************************************************************
  class pool_base
  {
  public:

    //*************************************************************************
    /// Returns the number of free items in the pool.
    //*************************************************************************
    size_t available() const
    {
      return MAX_SIZE - items_allocated;
    }

    //*************************************************************************
    /// Checks to see if there are no free items in the pool.
    /// \return <b>true</b> if there are none free (or 'empty')
    //*************************************************************************
    bool none_free() const
    {
      return items_allocated == MAX_SIZE;
    }

  protected:

    //*************************************************************************
    /// Constructor
    //*************************************************************************
    pool_base(size_t max_size)
      : next_free(0),
        items_allocated(0),
        MAX_SIZE(max_size)       
    {
    }

    size_t       next_free;       ///< The next free slot in the block.
    size_t       items_allocated; ///< The number of items allocated.
    const size_t MAX_SIZE;        ///< The maximum number of objects that can be allocated.
  };
}
#endif

