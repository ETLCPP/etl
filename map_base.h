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

#if !defined(__ETL_IN_IMAP_H__) && !defined(__ETL_IN_IMULTIMAP_H__)
#error This header is a private element of etl::map, etl::multimap & etl::imap, etl::imultimap
#endif

#ifndef __ETL_MAP_BASE__
#define __ETL_MAP_BASE__

#include <stddef.h>
#include "exception.h"

namespace etl
{
  //***************************************************************************
  /// Exception for the map.
  ///\ingroup map
  //***************************************************************************
  class map_exception : public exception
  {
  public:

    map_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the map.
  ///\ingroup map
  //***************************************************************************
  class map_full : public map_exception
  {
  public:

    map_full()
      : map_exception("map: full")
    {
    }
  };

  //***************************************************************************
  /// Map out of bounds exception.
  ///\ingroup map
  //***************************************************************************
  class map_out_of_bounds : public map_exception
  {
  public:

    map_out_of_bounds()
      : map_exception("map: out of bounds")
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the map.
  ///\ingroup map
  //***************************************************************************
  class map_iterator : public map_exception
  {
  public:

    map_iterator()
      : map_exception("map: iterator problem")
    {
    }
  };

  //***************************************************************************
  /// The base class for all maps.
  ///\ingroup map
  //***************************************************************************
  class map_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of map.

    //*************************************************************************
    /// Gets the size of the map.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the map.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Checks to see if the map is empty.
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the map is full.
    //*************************************************************************
    bool full() const
    {
      return current_size == MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the capacity of the vector.
    ///\return The capacity of the vector.
    //*************************************************************************
    size_type capacity() const
    {
      return MAX_SIZE;
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
    map_base(size_type max_size)
      : current_size(0)
      , MAX_SIZE(max_size)

    {
    }

    size_type current_size;   ///< The number of the used nodes.
    const size_type MAX_SIZE; ///< The maximum size of the map.
  };
}

#endif
