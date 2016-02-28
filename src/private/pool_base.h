///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include "../exception.h"
#include "../error_handler.h"
#include "../error_handler.h"

#undef ETL_FILE
#define ETL_FILE "11"

namespace etl
{
  //***************************************************************************
  /// The base class for pool exceptions.
  ///\ingroup pool
  //***************************************************************************
  class pool_exception : public exception
  {
  public:

    pool_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {}
  };

  //***************************************************************************
  /// The exception thrown when the pool has no more free items.
  ///\ingroup pool
  //***************************************************************************
  class pool_no_allocation : public pool_exception
  {
  public:

    explicit pool_no_allocation(string_type file_name, numeric_type line_number)
      : pool_exception(ETL_ERROR_TEXT("pool:allocation", ETL_FILE"A"), file_name, line_number)
    {}
  };

  //***************************************************************************
  /// The exception thrown when an object is released which does not belong to the pool.
  ///\ingroup pool
  //***************************************************************************
  class pool_object_not_in_pool : public pool_exception
  {
  public:

    pool_object_not_in_pool(string_type file_name, numeric_type line_number)
      : pool_exception(ETL_ERROR_TEXT("pool:notinpool", ETL_FILE"B"), file_name, line_number)
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
    /// Returns the maximum number of items in the pool.
    //*************************************************************************
    size_t max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the number of free items in the pool.
    //*************************************************************************
    size_t available() const
    {
      return MAX_SIZE - items_allocated;
    }

    //*************************************************************************
    /// Returns the number of allocated items in the pool.
    //*************************************************************************
    size_t size() const
    {
      return items_allocated;
    }

    //*************************************************************************
    /// Checks to see if there are no allocated items in the pool.
    /// \return <b>true</b> if there are none allocated.
    //*************************************************************************
    bool empty() const
    {
      return items_allocated == 0;
    }

    //*************************************************************************
    /// Checks to see if there are no free items in the pool.
    /// \return <b>true</b> if there are none free.
    //*************************************************************************
    bool full() const
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

#undef ETL_FILE

#endif

