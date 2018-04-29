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

#ifndef ETL_EXCEPTION_INCLUDED
#define ETL_EXCEPTION_INCLUDED

#include "platform.h"

///\defgroup exception exception
/// The base class for all ETL exceptions.
///\ingroup utilities

namespace etl
{
  //***************************************************************************
  ///\ingroup exception
  /// A low overhead exception base class.
  //***************************************************************************
  class exception
  {
  public:

    typedef const char* string_type;
    typedef int         numeric_type;

#if defined(ETL_VERBOSE_ERRORS)
    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    exception(string_type reason_, string_type file_, numeric_type line_)
      : reason_text(reason_),
        file_text(file_),
        line(line_)
    {
    }
#else
    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    exception(string_type reason_, string_type file_, numeric_type line_)
      : reason_text(reason_),
        line(line_)
    {
    (void)file_;
    }
#endif

    //***************************************************************************
    /// Gets the reason for the exception.
    /// \return const char* to the reason.
    //***************************************************************************
    string_type what() const
    {
      return reason_text;
    }


    //***************************************************************************
    /// Gets the file for the exception.
    /// \return const char* to the file.
    //***************************************************************************
    string_type file_name() const
    {
#if defined(ETL_VERBOSE_ERRORS)
      return file_text;
#else
      return "";
#endif
    }

    //***************************************************************************
    /// Gets the line for the exception.
    /// \return const char* to the line.
    //***************************************************************************
    numeric_type line_number() const
    {
      return line;
    }

  private:

    string_type  reason_text; ///< The reason for the exception.
#if defined(ETL_VERBOSE_ERRORS)
    string_type  file_text;   ///< The file for the exception.
#endif
    numeric_type line;   ///< The line for the exception.
  };
}

#endif
