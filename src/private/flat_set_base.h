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

#ifndef __ETL_IN_IFLAT_SET_H__
#error This header is a private element of etl::flat_set & etl::iflat_set
#endif

#ifndef __ETL_FLAT_SET_BASE__
#define __ETL_FLAT_SET_BASE__

#include <stddef.h>

#include "../exception.h"
#include "../error_handler.h"
#include "../debug_count.h"

#undef ETL_FILE
#define ETL_FILE "5"

namespace etl
{
  //***************************************************************************
  ///\ingroup flat_set
  /// Exception base for flat_sets
  //***************************************************************************
  class flat_set_exception : public exception
  {
  public:

    flat_set_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_set
  /// Vector full exception.
  //***************************************************************************
  class flat_set_full : public flat_set_exception
  {
  public:

    flat_set_full(string_type file_name, numeric_type line_number)
      : flat_set_exception(ETL_ERROR_TEXT("flat_set:full", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_set
  /// Vector iterator exception.
  //***************************************************************************
  class flat_set_iterator : public flat_set_exception
  {
  public:

    flat_set_iterator(string_type file_name, numeric_type line_number)
      : flat_set_exception(ETL_ERROR_TEXT("flat_set:iterator", ETL_FILE"C"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup flat_set
  /// The base class for all templated flat_set types.
  //***************************************************************************
  class flat_set_base
  {
  public:

    typedef size_t size_type;

  protected:

    etl::debug_count construct_count;
  };
}

#undef ETL_FILE

#endif
