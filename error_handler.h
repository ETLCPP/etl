
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

#ifndef __ETL_ERROR_HANDLER__
#define __ETL_ERROR_HANDLER__

///\defgroup error_handler error_handler
/// Error handler for when throwing exceptions is not required.
///\ingroup utilities

#include "exception.h"
#include "function.h"

namespace etl
{
  //***************************************************************************
  /// Error handler for when throwing exceptions is not required.
  ///\ingroup error_handler
  //***************************************************************************
  class error_handler
  {
  public:

    //*************************************************************************
    /// Callback class for free handler functions.
    //*************************************************************************
    struct free_function : public function<void, const exception&>
    {
      free_function(void (*p_function)(const exception&))
        : function<void, const exception&>(p_function)
      {
      }
    };

    //*************************************************************************
    /// Callback class for member handler functions.
    //*************************************************************************
    template <typename TObject>
    struct member_function : public function<TObject, const exception&>
    {
      member_function(TObject& object, void(TObject::*p_function)(const exception&))
        : function<TObject, const exception&>(object, p_function)
      {
      }
    };

    static void set_callback(ifunction<const exception&>& f);
    static void error(const exception& e);

  private:

    static ifunction<const exception&>* p_ifunction;
  };

  //***************************************************************************
  /// Raise an error.
  /// If ETL_THROW_EXCEPTIONS is defined then the error is thrown, otherwise
  /// the error handler is called.
  ///\ingroup error_handler
  //***************************************************************************
#ifdef ETL_THROW_EXCEPTIONS
  #define ETL_ERROR(e) throw e
#else
  #define ETL_ERROR(e) etl::error_handler::error(e);
#endif
}

#endif

