///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#ifndef __ETL_IN_ISTRING_H__
#error This header is a private element of etl::string & etl::istring
#endif

#ifndef __ETL_STRING_BASE__
#define __ETL_STRING_BASE__

#include <stddef.h>

#include "../platform.h"
#include "../integral_limits.h"
#include "../exception.h"
#include "../error_handler.h"

#define ETL_FILE "27"

#ifdef ETL_COMPILER_MICROSOFT
#undef max
#endif

namespace etl
{
  //***************************************************************************
  ///\ingroup string
  /// Exception base for strings
  //***************************************************************************
  class string_exception : public exception
  {
  public:

    string_exception(string_type what, string_type file_name, numeric_type line_number)
      : exception(what, file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// String empty exception.
  //***************************************************************************
  class string_empty : public string_exception
  {
  public:

    string_empty(string_type file_name, numeric_type line_number)
      : string_exception(ETL_ERROR_TEXT("string:empty", ETL_FILE"A"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// String out of bounds exception.
  //***************************************************************************
  class string_out_of_bounds : public string_exception
  {
  public:

    string_out_of_bounds(string_type file_name, numeric_type line_number)
      : string_exception(ETL_ERROR_TEXT("string:bounds", ETL_FILE"B"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// String iterator exception.
  //***************************************************************************
  class string_iterator : public string_exception
  {
  public:

    string_iterator(string_type file_name, numeric_type line_number)
      : string_exception(ETL_ERROR_TEXT("string:iterator", ETL_FILE"D"), file_name, line_number)
    {
    }
  };

  //***************************************************************************
  ///\ingroup string
  /// The base class for all templated string types.
  //***************************************************************************
  class string_base
  {
  public:

    typedef size_t size_type;

    enum
    {
      npos = etl::integral_limits<size_t>::max
    };

    //*************************************************************************
    /// Gets the current size of the string.
    ///\return The current size of the string.
    //*************************************************************************
    size_type size() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Gets the current size of the string.
    ///\return The current size of the string.
    //*************************************************************************
    size_type length() const
    {
      return current_size;
    }

    //*************************************************************************
    /// Checks the 'empty' state of the string.
    ///\return <b>true</b> if empty.
    //*************************************************************************
    bool empty() const
    {
      return (current_size == 0);
    }

    //*************************************************************************
    /// Checks the 'full' state of the string.
    ///\return <b>true</b> if full.
    //*************************************************************************
    bool full() const
    {
      return current_size == MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the capacity of the string.
    ///\return The capacity of the string.
    //*************************************************************************
    size_type capacity() const
    {
      return MAX_SIZE;
    }

    //*************************************************************************
    /// Returns the maximum possible size of the string.
    ///\return The maximum size of the string.
    //*************************************************************************
    size_type max_size() const
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

    //*************************************************************************
    /// Returns whether the string was truncated by the lat operation.
    ///\return Whether the string was truncated by the lat operation.
    //*************************************************************************
    size_t truncated() const
    {
      return is_truncated;
    }

  protected:

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    string_base(size_t max_size)
      : is_truncated(false),
        current_size(0),
        MAX_SIZE(max_size)
    {
    }

    bool            is_truncated; ///< Set to true if the operation truncated the string.
    size_type       current_size; ///< The current number of elements in the string.
    const size_type MAX_SIZE;     ///< The maximum number of elements in the string.
  };
}

#ifdef ETL_COMPILER_MICROSOFT
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#undef ETL_FILE

#endif
