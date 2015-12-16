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

#ifndef __ETL_IN_IUNORDERED_MAP_H__
#error This header is a private element of etl::unordered_map & etl::iunordered_map
#endif

#ifndef __ETL_UNORDERED_MAP_BASE__
#define __ETL_UNORDERED_MAP_BASE__

#include <stddef.h>
#include "../exception.h"
#include "../error_handler.h"

#define ETL_FILE "16"

namespace etl
{
  //***************************************************************************
  /// Exception for the unordered_map.
  ///\ingroup unordered_map
  //***************************************************************************
  class unordered_map_exception : public exception
  {
  public:

    unordered_map_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the unordered_map.
  ///\ingroup unordered_map
  //***************************************************************************
  class unordered_map_full : public unordered_map_exception
  {
  public:

    unordered_map_full(string_type file_name, numeric_type line_number)
      : unordered_map_exception(ETL_ERROR_TEXT("unordered_map:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// Out of range exception for the unordered_map.
  ///\ingroup unordered_map
  //***************************************************************************
  class unordered_map_out_of_range : public unordered_map_exception
  {
  public:

    unordered_map_out_of_range(string_type file_name, numeric_type line_number)
      : unordered_map_exception(ETL_ERROR_TEXT("unordered_map:range", ETL_FILE"B"), file_name, line_number)
    {}
  };

  //***************************************************************************
  /// Iterator exception for the unordered_map.
  ///\ingroup unordered_map
  //***************************************************************************
  class unordered_map_iterator : public unordered_map_exception
  {
  public:

    unordered_map_iterator(string_type file_name, numeric_type line_number)
      : unordered_map_exception("unordered_map:iterator", file_name, line_number)
    {
    }
  };

  //***************************************************************************
  /// The base class for all unordered_maps.
  ///\ingroup unordered_map
  //***************************************************************************
  class unordered_map_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of unordered_map.

    //*************************************************************************
    /// Gets the size of the unordered_map.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the unordered_map.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Checks to see if the unordered_map is empty.
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the unordered_map is full.
    //*************************************************************************
    bool full() const
    {
      return current_size == MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return max_size() - size();
    }

  protected:

    //*************************************************************************
    /// The constructor that is called from derived classes.
    //*************************************************************************
    unordered_map_base(size_type max_size)
      : current_size(0),
        MAX_SIZE(max_size)
    {
    }

    size_type current_size;   ///< The number of the used nodes.
    const size_type MAX_SIZE; ///< The maximum size of the unordered_map.
  };
}

#undef ETL_FILE

#endif
