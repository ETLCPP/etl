///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include "../exception.h"
#include "../ivector.h"
#include "../error_handler.h"
#include "../debug_count.h"

#undef ETL_FILE
#define ETL_FILE "4"

namespace etl
{
  //***************************************************************************
  ///\ingroup flat_multiset
  /// Exception base for flat_multisets
  //***************************************************************************
  class flat_multiset_exception : public exception
  {
  public:

    flat_multiset_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
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

    flat_multiset_full(string_type file_name, numeric_type line_number)
      : flat_multiset_exception(ETL_ERROR_TEXT("flat_multiset:full", ETL_FILE"A"), file_name, line_number)
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

  protected:

  protected:

    etl::debug_count construct_count;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    flat_multiset_base()
    {
    }
  };
}

#undef ETL_FILE

#endif
