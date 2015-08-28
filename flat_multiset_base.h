///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2015 jwellbelove

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

#ifndef __ETL_IN_IFLAT_MULTISET_H__
#error This header is a private element of etl::flat_multiset & etl::iflat_multiset
#endif

#ifndef __ETL_FLAT_MULTISET_BASE__
#define __ETL_FLAT_MULTISET_BASE__

#include <stddef.h>

#include "exception.h"
#include "ivector.h"
#include "error_handler.h"

namespace etl
{
  //***************************************************************************
  ///\ingroup flat_multiset
  /// Exception base for flat_multisets
  //***************************************************************************
  class flat_multiset_exception : public exception
  {
  public:

    flat_multiset_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_multiset
  /// Flat multiset full exception.
  //***************************************************************************
  class flat_multiset_full : public flat_multiset_exception
  {
  public:

    flat_multiset_full()
      : flat_multiset_exception("flat_multiset: full")
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_multiset
  /// Flat multiset out of bounds exception.
  //***************************************************************************
  class flat_multiset_out_of_bounds : public flat_multiset_exception
  {
  public:

    flat_multiset_out_of_bounds()
      : flat_multiset_exception("flat_multiset: out of bounds")
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_multiset
  /// Vector iterator exception.
  //***************************************************************************
  class flat_multiset_iterator : public flat_multiset_exception
  {
  public:

    flat_multiset_iterator()
      : flat_multiset_exception("flat_multiset: iterator error")
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_multiset
  /// The base class for all templated flat_multiset types.
  //***************************************************************************
  class flat_multiset_base
  {
  public:

    typedef size_t size_type;

    //*************************************************************************
    /// Gets the current size of the flat_multiset.
    ///\return The current size of the flat_multiset.
    //*************************************************************************
    size_type size() const
    {
      return vbase.size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the flat_multiset.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return vbase.empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the flat_multiset.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return vbase.full();
    }

    //*************************************************************************
    /// Returns the capacity of the flat_multiset.
    ///\return The capacity of the flat_multiset.
    //*************************************************************************
    size_type capacity() const
    {
      return vbase.capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the flat_multiset.
    ///\return The maximum size of the flat_multiset.
    //*************************************************************************
    size_type max_size() const
    {
      return vbase.max_size();
    }

    //*************************************************************************
    /// Returns the remaining capacity.
    ///\return The remaining capacity.
    //*************************************************************************
    size_t available() const
    {
      return vbase.available();
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    flat_multiset_base(vector_base& vbase)
      : vbase(vbase)
    {
    }

    vector_base& vbase;
  };
}

#endif
