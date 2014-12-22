///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#ifndef __ETL_IN_ILOOKUP_H__
#error This header is a private element of etl::lookup & etl::ilookup
#endif

#ifndef __ETL_LOOKUP_BASE__
#define __ETL_LOOKUP_BASE__

#include <stddef.h>

#include "exception.h"
#include "ivector.h"

#ifndef ETL_THROW_EXCEPTIONS
#include "error_handler.h"
#endif

namespace etl
{
  //***************************************************************************
  ///\ingroup lookup
  /// Exception base for lookups
  //***************************************************************************
  class lookup_exception : public exception
  {
  public:

    lookup_exception(const char* what)
      : exception(what)
    {
    }
  };

  //***************************************************************************
  ///\ingroup lookup
  /// Vector full exception.
  //***************************************************************************
  class lookup_full : public lookup_exception
  {
  public:

    lookup_full()
      : lookup_exception("lookup: full")
    {
    }
  };

  //***************************************************************************
  ///\ingroup lookup
  /// Vector out of bounds exception.
  //***************************************************************************
  class lookup_out_of_bounds : public lookup_exception
  {
  public:

    lookup_out_of_bounds()
      : lookup_exception("lookup: out of bounds")
    {
    }
  };

  //***************************************************************************
  ///\ingroup lookup
  /// Vector iterator exception.
  //***************************************************************************
  class lookup_iterator : public lookup_exception
  {
  public:

    lookup_iterator()
      : lookup_exception("lookup: iterator error")
    {
    }
  };

  //***************************************************************************
  ///\ingroup lookup
  /// The base class for all templated lookup types.
  //***************************************************************************
  class lookup_base
  {
  public:

    typedef size_t size_type;

    //*************************************************************************
    /// Gets the current size of the lookup.
    ///\return The current size of the lookup.
    //*************************************************************************
    size_type size() const
    {
      return vbase.size();
    }

    //*************************************************************************
    /// Checks the 'empty' state of the lookup.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return vbase.empty();
    }

    //*************************************************************************
    /// Checks the 'full' state of the lookup.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return vbase.full();
    }

    //*************************************************************************
    /// Returns the capacity of the lookup.
    ///\return The capacity of the lookup.
    //*************************************************************************
    size_type capacity() const
    {
      return vbase.capacity();
    }

    //*************************************************************************
    /// Returns the maximum possible size of the lookup.
    ///\return The maximum size of the lookup.
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

    //*************************************************************************
    /// Clears the lookup.
    //*************************************************************************
    void clear()
    {
      vbase.clear();
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    lookup_base(vector_base& vbase)
      : vbase(vbase)
    {
    }

    vector_base& vbase;
  };
}

#endif
