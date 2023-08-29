
///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "platform.h"
#include "exception.h"
#include "function.h"
#include "nullptr.h"

#include <assert.h>

#if defined(ETL_LOG_ERRORS) || defined(ETL_IN_UNIT_TEST)
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
    /// Callback class for free handler functions. Deprecated.
    //*************************************************************************
    struct free_function : public etl::function<void, const etl::exception&>
    {
      explicit free_function(void (*p_function_)(const etl::exception&))
        : etl::function<void, const etl::exception&>(p_function_)
      {
      }
    };

    //*************************************************************************
    /// Callback class for member handler functions. Deprecated.
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
    /// Sets the error callback function. Deprecated.
    ///\param f A reference to an etl::function object that will handler errors.
    //*****************************************************************************
    static void set_callback(ifunction<const etl::exception&>& f)
    {
      create((void*)(&f), ifunction_stub);
    }

    //*************************************************************************
    /// Create from function (Compile time).
    //*************************************************************************
    template <void(*Method)(const etl::exception&)>
    static void set_callback()
    {
      create(ETL_NULLPTR, function_stub<Method>);
    }

    //*************************************************************************
    /// Create from instance method (Run time).
    //*************************************************************************
    template <typename T, void(T::* Method)(const etl::exception&)>
    static void set_callback(T& instance)
    {
      create((void*)(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from const instance method (Run time).
    //*************************************************************************
    template <typename T, void(T::* Method)(const etl::exception&) const>
    static void set_callback(const T& instance)
    {
      create((void*)(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, void(T::* Method)(const etl::exception&)>
    static void set_callback()
    {
      create(method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    /// Create from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, void(T::* Method)(const etl::exception&) const>
    static void set_callback()
    {
      create(const_method_instance_stub<T, Instance, Method>);
    }

    //*****************************************************************************
    /// Sends the exception error to the user's handler function.
    ///\param e The exception error.
    //*****************************************************************************
    static void error(const etl::exception& e)
    {
      invocation_element& invocation = get_invocation_element();

      if (invocation.stub != ETL_NULLPTR)
      {
        (*invocation.stub)(invocation.object, e);
      }
    }

  private:

    typedef void(*stub_type)(void* object, const etl::exception&);

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      //***********************************************************************
      invocation_element()
        : object(ETL_NULLPTR)
        , stub(ETL_NULLPTR)
      {
      }

      //***********************************************************************
      void* object;
      stub_type stub;
    };

    //*************************************************************************
    /// Returns the static invocation element.
    //*************************************************************************
    static invocation_element& get_invocation_element()
    {
      static invocation_element invocation;

      return invocation;
    }

    //*************************************************************************
    /// Constructs a callback from an object and stub.
    //*************************************************************************
    static void create(void* object, stub_type stub)
    {
      invocation_element& invocation = get_invocation_element();

      invocation.object = object;
      invocation.stub   = stub;
    }

    //*************************************************************************
    /// Constructs a callback from a stub.
    //*************************************************************************
    static void create(stub_type stub)
    {
      invocation_element& invocation = get_invocation_element();

      invocation.object = ETL_NULLPTR;
      invocation.stub   = stub;
    }

    //*************************************************************************
    /// Stub call for a member function. Run time instance.
    //*************************************************************************
    template <typename T, void(T::* Method)(const etl::exception&)>
    static void method_stub(void* object, const etl::exception& e)
    {
      T* p = static_cast<T*>(object);
      return (p->*Method)(e);
    }

    //*************************************************************************
    /// Stub call for a const member function. Run time instance.
    //*************************************************************************
    template <typename T, void(T::* Method)(const etl::exception&) const>
    static void const_method_stub(void* object, const etl::exception& e)
    {
      T* const p = static_cast<T*>(object);
      return (p->*Method)(e);
    }

    //*************************************************************************
    /// Stub call for a member function. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance, void(T::* Method)(const etl::exception&)>
    static void method_instance_stub(void*, const etl::exception& e)
    {
      return (Instance.*Method)(e);
    }

    //*************************************************************************
    /// Stub call for a const member function. Compile time instance.
    //*************************************************************************
    template <typename T, const T& Instance, void(T::* Method)(const etl::exception&) const>
    static void const_method_instance_stub(void*, const etl::exception& e)
    {
      (Instance.*Method)(e);
    }

    //*************************************************************************
    /// Stub call for a free function.
    //*************************************************************************
    template <void(*Method)(const etl::exception&)>
    static void function_stub(void*, const etl::exception& e)
    {
      (Method)(e);
    }

    //*************************************************************************
    /// Stub call for a ifunction. Run time instance.
    //*************************************************************************
    static void ifunction_stub(void* object, const etl::exception& e)
    {
      etl::ifunction<const etl::exception&>* p = static_cast<etl::ifunction<const etl::exception&>*>(object);
      p->operator()(e);
    }
  };
}
#endif

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
  #define ETL_ASSERT(b, e)                       ETL_DO_NOTHING // Does nothing.
  #define ETL_ASSERT_OR_RETURN(b, e)             ETL_DO_NOTHING // Does nothing.
  #define ETL_ASSERT_OR_RETURN_VALUE(b, e, v)    ETL_DO_NOTHING // Does nothing.
  
  #define ETL_ASSERT_FAIL(e)                     ETL_DO_NOTHING // Does nothing.
  #define ETL_ASSERT_FAIL_AND_RETURN(e)          ETL_DO_NOTHING // Does nothing.
  #define ETL_ASSERT_FAIL_AND_RETURN_VALUE(e, v) ETL_DO_NOTHING // Does nothing.
#elif ETL_USING_EXCEPTIONS
  #if defined(ETL_LOG_ERRORS)
    #define ETL_ASSERT(b, e) {if (!(b)) {etl::error_handler::error((e)); throw((e));}}                                // If the condition fails, calls the error handler then throws an exception.
    #define ETL_ASSERT_OR_RETURN(b, e) {if (!(b)) {etl::error_handler::error((e)); throw((e)); return;}}              // If the condition fails, calls the error handler then throws an exception.
    #define ETL_ASSERT_OR_RETURN_VALUE(b, e, v) {if (!(b)) {etl::error_handler::error((e)); throw((e)); return(v);}}  // If the condition fails, calls the error handler then throws an exception.
    
    #define ETL_ASSERT_FAIL(e) {etl::error_handler::error((e)); throw((e));}                                          // Calls the error handler then throws an exception.
    #define ETL_ASSERT_FAIL_AND_RETURN(e) {etl::error_handler::error((e)); throw((e)); return;}                       // Calls the error handler then throws an exception.
    #define ETL_ASSERT_FAIL_AND_RETURN_VALUE(e, v) {etl::error_handler::error((e)); throw((e)); return(v);}           // Calls the error handler then throws an exception.
  #else
    #define ETL_ASSERT(b, e) {if (!(b)) {throw((e));}}                    // If the condition fails, throws an exception.
    #define ETL_ASSERT_OR_RETURN(b, e) {if (!(b)) {throw((e));}}          // If the condition fails, throws an exception.
    #define ETL_ASSERT_OR_RETURN_VALUE(b, e, v) {if (!(b)) {throw((e));}} // If the condition fails, throws an exception.
    
    #define ETL_ASSERT_FAIL(e) {throw((e));}                              // Throws an exception.
    #define ETL_ASSERT_FAIL_AND_RETURN(e) {throw((e));}                   // Throws an exception.
    #define ETL_ASSERT_FAIL_AND_RETURN_VALUE(e, v) {throw((e));}          // Throws an exception.

  #endif
#else
  #if defined(ETL_LOG_ERRORS)
    #define ETL_ASSERT(b, e) {if(!(b)) {etl::error_handler::error((e));}}                                 // If the condition fails, calls the error handler
    #define ETL_ASSERT_OR_RETURN(b, e) {if(!(b)) {etl::error_handler::error((e)); return;}}               // If the condition fails, calls the error handler and return
    #define ETL_ASSERT_OR_RETURN_VALUE(b, e, v) {if(!(b)) {etl::error_handler::error((e)); return (v);}}  // If the condition fails, calls the error handler and return a value
    
    #define ETL_ASSERT_FAIL(e) {etl::error_handler::error((e));}                                          // Calls the error handler
    #define ETL_ASSERT_FAIL_AND_RETURN(e) {etl::error_handler::error((e)); return;}                       // Calls the error handler and return
    #define ETL_ASSERT_FAIL_AND_RETURN_VALUE(e, v) {etl::error_handler::error((e)); return (v);}          // Calls the error handler and return a value
  #else
    #if ETL_IS_DEBUG_BUILD
      #define ETL_ASSERT(b, e) assert((b))                                                                // If the condition fails, asserts.
      #define ETL_ASSERT_OR_RETURN(b, e) {if (!(b)) {assert(false); return;}}                             // If the condition fails, asserts and return.
      #define ETL_ASSERT_OR_RETURN_VALUE(b, e, v) {if (!(b)) {assert(false); return(v);}}                 // If the condition fails, asserts and return a value.
    
      #define ETL_ASSERT_FAIL(e) assert(false)                                    // Asserts.
      #define ETL_ASSERT_FAIL_AND_RETURN(e) {assert(false);  return;}             // Asserts.
      #define ETL_ASSERT_FAIL_AND_RETURN_VALUE(e, v) {assert(false);  return(v);} // Asserts.
    #else
      #define ETL_ASSERT(b, e)                                                    // Does nothing.
      #define ETL_ASSERT_OR_RETURN(b, e) {if (!(b)) return;}                     // Returns.
      #define ETL_ASSERT_OR_RETURN_VALUE(b, e, v) {if (!(b)) return(v);}         // Returns a value.
      
      #define ETL_ASSERT_FAIL(e)                                                  // Does nothing.
      #define ETL_ASSERT_FAIL_AND_RETURN(e) {return;}                             // Returns.
      #define ETL_ASSERT_FAIL_AND_RETURN_VALUE(e, v) {return(v);}                 // Returns a value.
    #endif
  #endif
#endif

#if defined(ETL_VERBOSE_ERRORS)
  #define ETL_ERROR(e) (e(__FILE__, __LINE__))                    // Make an exception with the file name and line number.
  #define ETL_ERROR_WITH_VALUE(e, v) (e(__FILE__, __LINE__, (v))) // Make an exception with the file name, line number and value.
#else
  #define ETL_ERROR(e) (e("", __LINE__))                    // Make an exception with the line number.
  #define ETL_ERROR_WITH_VALUE(e, v) (e("", __LINE__, (v))) // Make an exception with the file name, line number and value.
#endif

#if defined(ETL_VERBOSE_ERRORS)
  #define ETL_ERROR_TEXT(verbose_text, terse_text) (verbose_text) // Use the verbose text.
#else
  #define ETL_ERROR_TEXT(verbose_text, terse_text) (terse_text)   // Use the terse text.
#endif

#endif

