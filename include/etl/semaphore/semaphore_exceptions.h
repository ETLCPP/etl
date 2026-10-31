/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 Julien Hecht

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

#ifndef ETL_SEMAPHORE_EXCEPTIONS_INCLUDED
#define ETL_SEMAPHORE_EXCEPTIONS_INCLUDED

#include "../platform.h"
#include "../error_handler.h"
#include "../exception.h"

namespace etl
{

  //***************************************************************************
  ///\ingroup semaphore
  /// Exception base for semaphores
  //***************************************************************************
  class semaphore_exception : public exception
  {
  public:

    semaphore_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  ///\ingroup semaphore
  /// Exception thrown when the semaphore value gets out of bounds
  /// For example:
  /// - Creating a semaphore with a `desired` value over `max()`
  /// - Releasing a semaphore with a negative update value, or value that would bring it over `max()`
  //***************************************************************************
  class semaphore_out_of_bounds : public semaphore_exception
  {
  public:

    semaphore_out_of_bounds(string_type file_name_, numeric_type line_number_)
      : semaphore_exception(ETL_ERROR_TEXT("semaphore: out of bounds", ETL_SEMAPHORE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };
} // namespace etl

#endif
