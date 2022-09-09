///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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

#ifndef ETL_IHASH_INCLUDED
#define ETL_IHASH_INCLUDED

#include "platform.h"
#include "utility.h"
#include "exception.h"
#include "error_handler.h"

#include <stdint.h>

///\defgroup ihash Common data for all hash type classes.
///\ingroup hash

namespace etl
{
  //***************************************************************************
  ///\ingroup hash
  /// Exception base for hashes.
  //***************************************************************************
  class hash_exception : public exception
  {
  public:

    hash_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {}
  };

  //***************************************************************************
  ///\ingroup vector
  /// Hash finalised exception.
  //***************************************************************************
  class hash_finalised : public hash_exception
  {
  public:

    hash_finalised(string_type file_name_, numeric_type line_number_)
      : hash_exception(ETL_ERROR_TEXT("ihash:finalised", ETL_IHASH_FILE_ID"A"), file_name_, line_number_)
    {}
  };

  /// For the Americans
  typedef hash_finalised hash_finalized;
}

#endif
