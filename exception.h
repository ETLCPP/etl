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

#ifndef __ETL_EXCEPTION__
#define __ETL_EXCEPTION__

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

    typedef const char* value_type;

    //*************************************************************************
    /// Constructor.
    //*************************************************************************
    exception(value_type reason)
      : reason(reason)
    {
    }

    //***************************************************************************
    /// Gets the reason for the exception.
    /// \return const char* to the reason.
    //***************************************************************************
    value_type what() const
    {
      return reason;
    }

  private:

    value_type reason; ///< The reason for the exception.
  };
}

#endif
