///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#ifndef ETL_FUNCTION_WRAPPER_INCLUDED
#define ETL_FUNCTION_WRAPPER_INCLUDED

#include "platform.h"
#include "nullptr.h"
#include "utility.h"
#include "optional.h"
#include "exception.h"
#include "error_handler.h"

//*****************************************************************************
///\defgroup function_wrapper function_wrapper
/// A set of wrapper templates to allow a function to be stored and called based on its const/non-const definition.
///\ingroup utilities
//*****************************************************************************

#if !ETL_USING_CPP11
  #error NOT SUPPORTED FOR C++03 OR BELOW
#else
namespace etl
{
  //***************************************************************************
  /// The base for function_wrapper exceptions.
  //***************************************************************************
  class function_wrapper_exception : public exception
  {
  public:

    function_wrapper_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The uninitialised exception.
  //***************************************************************************
  class function_wrapper_uninitialised : public function_wrapper_exception
  {
  public:

    function_wrapper_uninitialised(string_type file_name_, numeric_type line_number_)
      : function_wrapper_exception(ETL_ERROR_TEXT("function_wrapper:uninitialised", ETL_MEMBER_FUNCTION_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// Prototype
  //***************************************************************************
  template <typename T>
  class function_wrapper;

  //***************************************************************************
  /// Template to wrap free functions, lambdas and functors.
  //***************************************************************************
  template <typename TReturn, typename... TParameters>
  class function_wrapper<TReturn(TParameters...)>
  {
  public:

    using this_type     = function_wrapper<TReturn(TParameters...)>;
    using function_type = TReturn(*)(TParameters...);

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper() = default;

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper(const function_wrapper& other) = default;

    //*************************************************************************
    /// Assignment
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper& operator =(const function_wrapper& other) = default;

    //*************************************************************************
    /// Create from function.
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper(function_type function_)
      : invocation(function_, function_stub)
    {
    }

    //*************************************************************************
    /// Create from a lambda or functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !etl::is_same<this_type, TLambda>::value, void>>
    ETL_CONSTEXPR14 
    function_wrapper(TLambda& instance_)
      : invocation((void*)(&instance_), lambda_stub<TLambda>)
    {
    }

    //*************************************************************************
    /// Create from a const lambda or functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !etl::is_same<this_type, TLambda>::value, void>>
    ETL_CONSTEXPR14 
    function_wrapper(const TLambda& instance_)
      : invocation((void*)(&instance_), const_lambda_stub<TLambda>)
    {
    }

    //*************************************************************************
    /// Assign from function.
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper& operator =(function_type function_)
    {
      invocation.alternate.func = function_;
      invocation.stub           = function_stub;

      return *this;
    }

    //*************************************************************************
    /// Assign from a lambda or functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !etl::is_same<this_type, TLambda>::value, void>>
    ETL_CONSTEXPR14
    function_wrapper& operator =(TLambda& instance_)
    {
      invocation.alternate.object = (void*)(&instance_);
      invocation.stub             = lambda_stub<TLambda>;

      return *this;
    }

    //*************************************************************************
    /// Assign from a const lambda or functor.
    //*************************************************************************
    template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value && !etl::is_same<this_type, TLambda>::value, void>>
    ETL_CONSTEXPR14
    function_wrapper& operator =(const TLambda& instance_)
    {
      invocation.alternate.object = (void*)(&instance_);
      invocation.stub             = const_lambda_stub<TLambda>;

      return *this;
    }

    //*************************************************************************
    /// Returns true if initialised
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 
    bool is_valid() const
    {
      return (invocation.stub != ETL_NULLPTR);
    }

    //*************************************************************************
    /// Returns true if initialised
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    operator bool() const
    {
      return is_valid();
    }

    //*************************************************************************
    /// Equality operator
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 bool operator ==(const function_wrapper& rhs) const
    {
      return invocation == rhs.invocation;
    }

    //*************************************************************************
    /// Inequality operator
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 bool operator !=(const function_wrapper& rhs) const
    {
      return invocation != rhs.invocation;
    }

    //*************************************************************************
    /// Call the wrapped function if initialised
    //*************************************************************************
    template <typename TRet = TReturn>
    ETL_CONSTEXPR14
    etl::enable_if_t<is_void<TRet>::value, bool>
      call_if(TParameters... params) const
    {
      if (is_valid())
      {
        operator()(etl::forward<TParameters>(params)...);
        return true;
      }
      else
      {
        return false;
      }
    }

    //*************************************************************************
    /// Call the wrapped function if initialised
    //*************************************************************************
    template <typename TRet = TReturn>
    ETL_CONSTEXPR14 
    etl::enable_if_t<!is_void<TRet>::value, etl::optional<TRet>>
      call_if(TParameters... params) const
    {
      etl::optional<TRet> result;

      if (is_valid())
      {
        result = operator()(etl::forward<TParameters>(params)...);
      }

      return result;
    }

    //*************************************************************************
    /// Call the wrapped function if initialised, or call the alternative 
    //*************************************************************************
    template <typename TAlternative>
    ETL_CONSTEXPR14
    TReturn call_or(TAlternative alternative, TParameters... params) const
    {
      if (is_valid())
      {
        return operator()(etl::forward<TParameters>(params)...);
      }
      else
      {
        return alternative(etl::forward<TParameters>(params)...);
      }
    }

    //*************************************************************************
    /// Execute the function.
    //*************************************************************************
    ETL_CONSTEXPR14 TReturn operator()(TParameters... params) const
    {
      ETL_ASSERT(is_valid(), ETL_ERROR(etl::function_wrapper_uninitialised));

      return (*invocation.stub)(invocation, etl::forward<TParameters>(params)...);
    }

  private:

    struct invocation_element;

    using stub_type = TReturn(*)(const invocation_element&, TParameters...);

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      ETL_CONSTEXPR14 invocation_element() = default;
      ETL_CONSTEXPR14 invocation_element(const invocation_element&) = default;
      ETL_CONSTEXPR14 invocation_element& operator=(const invocation_element&) = default;

      //*******************************
      ETL_CONSTEXPR14 invocation_element(void* object_, stub_type stub_)
        : alternate(object_)
        , stub(stub_)
      {
      }

      //*******************************
      ETL_CONSTEXPR14 invocation_element(function_type func_, stub_type stub_)
        : alternate(func_)
        , stub(stub_)
      {
      }

      //*******************************
      ETL_CONSTEXPR14 bool operator ==(const invocation_element& rhs) const
      {
        return (stub == rhs.stub) &&
                 ((stub == ETL_NULLPTR) ||
                  ((stub == function_stub) && (rhs.alternate.func == alternate.func)) ||
                  (rhs.alternate.object == alternate.object));
      }

      //*******************************
      ETL_CONSTEXPR14 bool operator !=(const invocation_element& rhs) const
      {
        return !(*this == rhs);
      }

      //*******************************
      union alternate_method_t
      {
        ETL_CONSTEXPR14 alternate_method_t()
          : object(ETL_NULLPTR)
        {
        }

        ETL_CONSTEXPR14 alternate_method_t(void* object_)
          : object(object_)
        {
        }

        ETL_CONSTEXPR14 alternate_method_t(function_type func_)
          : func(func_)
        {
        }

        void*         object;
        function_type func;
      } alternate;

      stub_type stub = ETL_NULLPTR;
    };

    //*************************************************************************
    /// Stub call for a free function.
    //*************************************************************************
    static ETL_CONSTEXPR14 
    TReturn function_stub(const invocation_element& invocation, TParameters... params)
    {
      return (*invocation.alternate.func)(etl::forward<TParameters>(params)...);
    }

    //*************************************************************************
    /// Stub call for a lambda or functor.
    //*************************************************************************
    template <typename TLambda>
    static ETL_CONSTEXPR14 
    TReturn lambda_stub(const invocation_element& invocation, TParameters... params)
    {
      TLambda* p = static_cast<TLambda*>(invocation.alternate.object);
      return (p->operator())(etl::forward<TParameters>(params)...);
    }

    //*************************************************************************
    /// Stub call for a lambda or functor.
    //*************************************************************************
    template <typename TLambda>
    static ETL_CONSTEXPR14 
    TReturn const_lambda_stub(const invocation_element& invocation, TParameters... params)
    {
      const TLambda* p = static_cast<const TLambda*>(invocation.alternate.object);
      return (p->operator())(etl::forward<TParameters>(params)...);
    }

    //*************************************************************************
    /// The invocation object.
    //*************************************************************************
    invocation_element invocation;
  };

  //***************************************************************************
  /// Template to wrap member functions.
  //***************************************************************************
  template <typename TObject, typename TReturn, typename... TParameters>
  class function_wrapper<TReturn(TObject&, TParameters...)>
  {
  public:

    using function_type = TReturn(TObject::*)(TParameters...);

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    ETL_CONSTEXPR14
    function_wrapper() = default;

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper(const function_wrapper& other) = default;

    //*************************************************************************
    /// Assignment
    //*************************************************************************
    ETL_CONSTEXPR14 function_wrapper& operator =(const function_wrapper& other) = default;

    //*************************************************************************
    /// Construct from method
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper(function_type method_)
      : invocation(method_, method_stub)
    {
    }

    //*************************************************************************
    /// Assign from method
    //*************************************************************************
    ETL_CONSTEXPR14
    function_wrapper& operator =(function_type method_)
    {
      invocation.method = method_;
      invocation.stub   = method_stub;

      return *this;
    }

    //*************************************************************************
    /// Returns true if the function_wrapper is initialised
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool is_valid() const
    {
      return (invocation.stub != ETL_NULLPTR);
    }

    //*************************************************************************
    /// Returns true if the function_wrapper is initialised
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    operator bool() const
    {
      return is_valid();
    }

    //*************************************************************************
    /// Equality operator
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 
    bool operator ==(const function_wrapper& rhs) const
    {
      return invocation == rhs.invocation;
    }

    //*************************************************************************
    /// Inequality operator
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool operator !=(const function_wrapper& rhs) const
    {
      return invocation != rhs.invocation;
    }

    //*************************************************************************
    /// Call the wrapped function if initialised
    //*************************************************************************
    template <typename TRet = TReturn>
    ETL_CONSTEXPR14
    etl::enable_if_t<is_void<TRet>::value, bool>
      call_if(TObject& object, TParameters... params) const
    {
      if (is_valid())
      {
        operator()(object, etl::forward<TParameters>(params)...);
        return true;
      }
      else
      {
        return false;
      }
    }

    //*************************************************************************
    /// Call the wrapped function if initialised
    //*************************************************************************
    template <typename TRet = TReturn>
    ETL_CONSTEXPR14
    etl::enable_if_t<!is_void<TRet>::value, etl::optional<TRet>>
      call_if(TObject& object, TParameters... params) const
    {
      etl::optional<TRet> result;

      if (is_valid())
      {
        result = operator()(object, etl::forward<TParameters>(params)...);
      }

      return result;
    }

    //*************************************************************************
    /// Call the wrapped function if initialised, or call the alternative
    //*************************************************************************
    template <typename TAlternative>
    ETL_CONSTEXPR14
    TReturn call_or(TAlternative alternative, TObject& object, TParameters... params) const
    {
      if (is_valid())
      {
        return operator()(object, etl::forward<TParameters>(params)...);
      }
      else
      {
        return alternative(etl::forward<TParameters>(params)...);
      }
    }

    //*************************************************************************
    /// Execute the function.
    //*************************************************************************
    ETL_CONSTEXPR14
    TReturn operator()(TObject& object, TParameters... params) const
    {
      ETL_ASSERT(is_valid(), ETL_ERROR(etl::function_wrapper_uninitialised));

      return (*invocation.stub)(object, invocation.method, etl::forward<TParameters>(params)...);
    }

  private:

    using stub_type = TReturn(*)(TObject&, function_type, TParameters...);

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      ETL_CONSTEXPR14 invocation_element() = default;
      ETL_CONSTEXPR14 invocation_element(const invocation_element&) = default;
      ETL_CONSTEXPR14 invocation_element& operator =(const invocation_element&) = default;
      
      //*******************************
      ETL_CONSTEXPR14 invocation_element(function_type method_, stub_type stub_)
        : method(method_)
        , stub(stub_)
      {
      }

      //*********************************
      ETL_CONSTEXPR14 bool operator ==(const invocation_element& rhs) const
      {
        return (rhs.method == method) && (rhs.stub == stub);
      }

      //*********************************
      ETL_CONSTEXPR14 bool operator !=(const invocation_element& rhs) const
      {
        return !(*this == rhs);
      }

      function_type method = ETL_NULLPTR;
      stub_type   stub     = ETL_NULLPTR;
    };

    //*************************************************************************
    /// Stub call for a member function.
    //*************************************************************************
    static ETL_CONSTEXPR14 
    TReturn method_stub(TObject& object, function_type method, TParameters... params)
    {
      return (object.*method)(etl::forward<TParameters>(params)...);
    }

    //*************************************************************************
    /// The invocation object.
    //*************************************************************************
    invocation_element invocation;
  };

  //***************************************************************************
  /// Template to wrap const member functions.
  //***************************************************************************
  template <typename TObject, typename TReturn, typename... TParameters>
  class function_wrapper<TReturn(const TObject&, TParameters...)>
  {
  public:

    using function_type = TReturn(TObject::*)(TParameters...) const;

    //*************************************************************************
    /// Default constructor
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper() = default;

    //*************************************************************************
    /// Copy constructor
    //*************************************************************************
    ETL_CONSTEXPR14
    function_wrapper(const function_wrapper& other) = default;

    //*************************************************************************
    /// Assignment
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper& operator =(const function_wrapper& other) = default;

    //*************************************************************************
    /// Construct from method
    //*************************************************************************
    ETL_CONSTEXPR14 
    function_wrapper(function_type method_)
      : invocation(method_, method_stub)
    {
    }

    //*************************************************************************
    /// Assign from method
    //*************************************************************************
    ETL_CONSTEXPR14
    function_wrapper& operator =(function_type method_)
    {
      invocation.method = method_;
      invocation.stub   = method_stub;

      return *this;
    }

    //*************************************************************************
    /// Has this been initialised
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    bool is_valid() const
    {
      return (invocation.stub != ETL_NULLPTR);
    }

    //*************************************************************************
    /// Has this been initialised
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14
    operator bool() const
    {
      return is_valid();
    }

    //*************************************************************************
    /// Equality operator
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 
    bool operator ==(const function_wrapper& rhs) const
    {
      return invocation == rhs.invocation;
    }

    //*************************************************************************
    /// Inequality operator
    //*************************************************************************
    ETL_NODISCARD
    ETL_CONSTEXPR14 
    bool operator !=(const function_wrapper& rhs) const
    {
      return invocation != rhs.invocation;
    }

    //*************************************************************************
    /// Call the wrapped function if initialised
    //*************************************************************************
    template <typename TRet = TReturn>
    ETL_CONSTEXPR14
    etl::enable_if_t<is_void<TRet>::value, bool>
      call_if(const TObject& object, TParameters... params) const
    {
      if (is_valid())
      {
        operator()(object, etl::forward<TParameters>(params)...);
        return true;
      }
      else
      {
        return false;
      }
    }

    //*************************************************************************
    /// Call the wrapped function if initialised
    //*************************************************************************
    template <typename TRet = TReturn>
    ETL_CONSTEXPR14
    etl::enable_if_t<!is_void<TRet>::value, etl::optional<TRet>>
      call_if(const TObject& object, TParameters... params) const
    {
      etl::optional<TRet> result;

      if (is_valid())
      {
        result = operator()(object, etl::forward<TParameters>(params)...);
      }

      return result;
    }

    //*************************************************************************
    /// Call the wrapped function if initialised, or call the alternative 
    //*************************************************************************
    template <typename TAlternative>
    ETL_CONSTEXPR14
    TReturn call_or(TAlternative alternative, const TObject& object, TParameters... params) const
    {
      if (is_valid())
      {
        return operator()(object, etl::forward<TParameters>(params)...);
      }
      else
      {
        return alternative(etl::forward<TParameters>(params)...);
      }
    }

    //*************************************************************************
    /// Execute the function.
    //*************************************************************************
    ETL_CONSTEXPR14
    TReturn operator()(const TObject& object, TParameters... params) const
    {
      ETL_ASSERT(is_valid(), ETL_ERROR(etl::function_wrapper_uninitialised));

      return (*invocation.stub)(object, invocation.method, etl::forward<TParameters>(params)...);
    }

  private:

    using stub_type = TReturn(*)(const TObject&, function_type, TParameters...);

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      ETL_CONSTEXPR14 invocation_element() = default;
      ETL_CONSTEXPR14 invocation_element(const invocation_element&) = default;
      ETL_CONSTEXPR14 invocation_element& operator =(const invocation_element&) = default;
      
      //*******************************
      ETL_CONSTEXPR14 invocation_element(function_type method_, stub_type stub_)
        : method(method_)
        , stub(stub_)
      {
      }

      //*********************************
      ETL_CONSTEXPR14 bool operator ==(const invocation_element& rhs) const
      {
        return (rhs.method == method) && (rhs.stub == stub);
      }

      //*********************************
      ETL_CONSTEXPR14 bool operator !=(const invocation_element& rhs) const
      {
        return !(*this == rhs);
      }

      function_type method = ETL_NULLPTR;
      stub_type     stub   = ETL_NULLPTR;
    };

    //*************************************************************************
    /// Stub call for a const member function.
    //*************************************************************************
    static ETL_CONSTEXPR14 
    TReturn method_stub(const TObject& object, function_type method, TParameters... params)
    {
      return (object.*method)(etl::forward<TParameters>(params)...);
    }

    //*************************************************************************
    /// The invocation object.
    //*************************************************************************
    invocation_element invocation;
  };
}

#endif
#endif
