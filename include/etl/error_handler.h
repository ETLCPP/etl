
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

#ifndef ETL_ERROR_HANDLER_INCLUDED
#define ETL_ERROR_HANDLER_INCLUDED

///\defgroup error_handler error_handler
/// Error handler for when throwing exceptions is not required.
///\ingroup utilities

#include <assert.h>

#include "platform.h"
#include "exception.h"
#include "function.h"

namespace etl
{
  namespace private_error_handler
  {
    template <class dummy>
    struct wrapper
    {
      static etl::ifunction<const etl::exception&>* p_ifunction;
    };

    template <class dummy>
    etl::ifunction<const etl::exception&>* wrapper<dummy>::p_ifunction = nullptr;
  }

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
    struct free_function : public etl::function<void, const etl::exception&>
    {
      free_function(void (*p_function_)(const etl::exception&))
        : etl::function<void, const etl::exception&>(p_function_)
      {
      }
    };

    //*************************************************************************
    /// Callback class for member handler functions.
    //*************************************************************************
    template <typename TObject>
    struct member_function : public etl::function<TObject, const etl::exception&>
    {
      member_function(TObject& object_, void(TObject::*p_function_)(const etl::exception&))
        : etl::function<TObject, const etl::exception&>(object_, p_function_)
      {
      }
    };

    //*****************************************************************************
    /// Sets the error callback function.
    ///\param f A reference to an etl::function object that will handler errors.
    //*****************************************************************************
    static void set_callback(ifunction<const etl::exception&>& f)
    {
      private_error_handler::wrapper<void>::p_ifunction = &f;
    }

    //*****************************************************************************
    /// Sends the exception error to the user's handler function.
    ///\param e The exception error.
    //*****************************************************************************
    static void error(const etl::exception& e)
    {
      if (private_error_handler::wrapper<void>::p_ifunction != nullptr)
      {
        (*private_error_handler::wrapper<void>::p_ifunction)(e);
      }
    }
  };
}

//***************************************************************************
/// Asserts a condition.
/// Versions of the macro that return a constant value of 'true' will allow the compiler to optimise away
/// any 'if' statements that it is contained within.
/// If ETL_NO_CHECKS is defined then no runtime checks are executed at all.
/// If asserts or exceptions are enabled then the error is thrown if the assert fails. The return value is always 'true'.
/// If ETL_LOG_ERRORS is defined then the error is logged if the assert fails. The return value is the value of the boolean test.
/// Otherwise 'assert' is called. The return value is always 'true'.
///\ingroup error_handler
//***************************************************************************
#if defined(ETL_NO_CHECKS)
  #define ETL_ASSERT(b, e)                                                               // Does nothing.
#elif defined(ETL_THROW_EXCEPTIONS)
  #if defined(ETL_LOG_ERRORS)
    #define ETL_ASSERT(b, e) {if (!(b)) {etl::error_handler::error((e)); throw((e));}}   // If the condition fails, calls the error handler then throws an exception.
  #else
    #define ETL_ASSERT(b, e) {if (!(b)) {throw((e));}}                                   // If the condition fails, throws an exception.
  #endif
#else
  #if defined(ETL_LOG_ERRORS)
    #if defined(NDEBUG)
      #define ETL_ASSERT(b, e) {if(!(b)) {etl::error_handler::error((e));}}              // If the condition fails, calls the error handler
    #else
      #define ETL_ASSERT(b, e) {if(!(b)) {etl::error_handler::error((e));} assert((b));} // If the condition fails, calls the error handler then asserts.
    #endif
  #else
    #if defined(NDEBUG)
      #define ETL_ASSERT(b, e)                                                           // Does nothing.
    #else
      #define ETL_ASSERT(b, e) assert((b))                                               // If the condition fails, asserts.
    #endif
  #endif
#endif

#if defined(ETL_VERBOSE_ERRORS)
  #define ETL_ERROR(e) (e(__FILE__, __LINE__)) // Make an exception with the file name and line number.
#else
  #define ETL_ERROR(e) (e("", __LINE__))       // Make an exception with the line number.
#endif

#if defined(ETL_VERBOSE_ERRORS)
  #define ETL_ERROR_TEXT(verbose_text, terse_text) (verbose_text) // Use the verbose text.
#else
  #define ETL_ERROR_TEXT(verbose_text, terse_text) (terse_text)   // Use the terse text.
#endif

#endif

