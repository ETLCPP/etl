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

#ifndef __ETL_IN_IFORWARD_LIST_H__
#error This header is a private element of etl::forward_list & etl::iforward_list
#endif

#ifndef __ETL_LIST_BASE__
#define __ETL_LIST_BASE__

#include <stddef.h>
#include "exception.h"

namespace etl
{
  //***************************************************************************
  /// Exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_exception : public exception
  {
  public:

    forward_list_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  /// Full exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_full : public forward_list_exception
  {
  public:

    forward_list_full()
      : forward_list_exception("forward list: full")
    {
    }
  };

  //***************************************************************************
  /// Iterator exception for the forward_list.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_iterator : public forward_list_exception
  {
  public:

    forward_list_iterator()
      : forward_list_exception("forward_list: iterator problem")
    {
    }
  };

  //***************************************************************************
  /// The base class for all forward_lists.
  ///\ingroup forward_list
  //***************************************************************************
  class forward_list_base
  {
  public:

    typedef size_t size_type; ///< The type used for determining the size of forward_list.

    //*************************************************************************
    /// Gets the size of the forward_list.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Gets the maximum possible size of the forward_list.
    //*************************************************************************
    size_type max_size() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Checks to see if the forward_list is empty.
    //*************************************************************************
    bool empty() const
    {
      return current_size == 0;
    }

    //*************************************************************************
    /// Checks to see if the forward_list is full.
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
    forward_list_base(size_type max_size)
      : next_free(0),
        current_size(0),
        MAX_SIZE(max_size)
    {
    }

    size_type next_free;      ///< The index of the next free node.
    size_type current_size;   ///< The number of items in the list.
    const size_type MAX_SIZE; ///< The maximum size of the forward_list.
  };
}

#endif
