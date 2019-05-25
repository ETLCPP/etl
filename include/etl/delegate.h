///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

/******************************************************************************

Copyright (C) 2017 by Sergey A Kryukov: derived work
http://www.SAKryukov.org
http://www.codeproject.com/Members/SAKryukov

Based on original work by Sergey Ryazanov:
"The Impossibly Fast C++ Delegates", 18 Jul 2005
https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

MIT license:
http://en.wikipedia.org/wiki/MIT_License

Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

******************************************************************************/

#ifndef ETL_DELEGATE_INCLUDED
#define ETL_DELEGATE_INCLUDED

#include "platform.h"

#if ETL_CPP11_SUPPORTED == 0
#error NOT SUPPORTED FOR C++03 OR BELOW
#endif

namespace etl
{
  template <typename T> class delegate;

  template <typename TReturn, typename... TParams>
  class delegate<TReturn(TParams...)> final
  {
  public:

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    delegate() = default;

    //*************************************************************************
    // Copy constructor.
    //*************************************************************************
    delegate(const delegate& other)
    {
      other.invocation.clone(invocation);
    }

    //*************************************************************************
    // Constructor from lambda or functor.
    //*************************************************************************
    template <typename TLambda>
    delegate(const TLambda& instance)
    {
      assign((void*)(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    /// Create from function (Compile time).
    //*************************************************************************
    template <TReturn(*Method)(TParams...)>
    static delegate create()
    {
      return delegate(nullptr, function_stub<Method>);
    }

    //*************************************************************************
    /// Create from Lambda or Functor.
    //*************************************************************************
    template <typename TLambda>
    static delegate create(const TLambda& instance)
    {
      return delegate((void*)(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    /// Create from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...)>
    static delegate create(T& instance)
    {
      return delegate((void*)(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...) const>
    static delegate create(const T& instance)
    {
      return delegate((void*)(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::*Method)(TParams...)>
    static delegate create()
    {
      return delegate(method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    /// Create from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn(T::*Method)(TParams...) const>
    static delegate create()
    {
      return delegate(const_method_instance_stub<T, Instance, Method>);
    }

#if !defined(ETL_COMPILER_GCC)
    //*************************************************************************
    /// Create from instance function operator (Compile time).
    //*************************************************************************
    template <typename T, T& Instance>
    static delegate create()
    {
      return delegate(operator_instance_stub<T, Instance>);
    }
#endif

    //*************************************************************************
    /// Execute the delegate.
    //*************************************************************************
    TReturn operator()(TParams... args) const
    {
      return (*invocation.stub)(invocation.object, args...);
    }

    //*************************************************************************
    /// Create from function (Compile time).
    //*************************************************************************
    delegate& operator =(const delegate& other)
    {
      other.invocation.clone(invocation);
      return *this;
    }

    //*************************************************************************
    /// Create from Lambda or Functor.
    //*************************************************************************
    template <typename TLambda>
    delegate& operator =(const TLambda& instance)
    {
      assign((void*)(&instance), lambda_stub<TLambda>);
      return *this;
    }

    //*************************************************************************
    /// Checks equality.
    //*************************************************************************
    bool operator == (const delegate& other) const
    {
      return invocation == other.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    bool operator != (const delegate& other) const
    {
      return invocation != other.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    bool is_valid() const
    {
      return invocation.stub != nullptr;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    operator bool() const
    {
      return is_valid();
    }

  private:

    using stub_type = TReturn(*)(void* object, TParams...);

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      invocation_element() = default;

      //***********************************************************************
      invocation_element(void* object_, stub_type stub_)
        : object(object_)
        , stub(stub_)
      {
      }

      //***********************************************************************
      void clone(invocation_element& target) const
      {
        target.stub   = stub;
        target.object = object;
      }

      //***********************************************************************
      bool operator ==(const invocation_element& another) const
      {
        return (another.stub == stub) && (another.object == object);
      }

      //***********************************************************************
      bool operator !=(const invocation_element& another) const
      {
        return (another.stub != stub) || (another.object != object);
      }

      //***********************************************************************
      void*     object = nullptr;
      stub_type stub   = nullptr;
    };

    //*************************************************************************
    /// Constructs a delegate from an object and stub.
    //*************************************************************************
    delegate(void* object, stub_type stub)
    {
      invocation.object = object;
      invocation.stub   = stub;
    }

    //*************************************************************************
    /// Constructs a delegate from a stub.
    //*************************************************************************
    delegate(stub_type stub)
    {
      invocation.object = nullptr;
      invocation.stub   = stub;
    }

    //*************************************************************************
    /// Assign from an object and stub.
    //*************************************************************************
    void assign(void* object, stub_type stub)
    {
      invocation.object = object;
      invocation.stub   = stub;
    }

    //*************************************************************************
    /// Stub call for a member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...)>
    static TReturn method_stub(void* object, TParams... params)
    {
      T* p = static_cast<T*>(object);
      return (p->*Method)(params...);
    }

    //*************************************************************************
    /// Stub call for a const member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParams...) const>
    static TReturn const_method_stub(void* object, TParams... params)
    {
      T* const p = static_cast<T*>(object);
      return (p->*Method)(params...);
    }

    //*************************************************************************
    /// Stub call for a member function. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::*Method)(TParams...)>
    static TReturn method_instance_stub(void*, TParams... params)
    {
      return (Instance.*Method)(params...);
    }

    //*************************************************************************
    /// Stub call for a const member function. Compile time instance.
    //*************************************************************************
    template <typename T, const T& Instance, TReturn(T::*Method)(TParams...) const>
    static TReturn const_method_instance_stub(void*, TParams... params)
    {
      return (Instance.*Method)(params...);
    }

#if !defined(ETL_COMPILER_GCC)
    //*************************************************************************
    /// Stub call for a function operator. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance>
    static TReturn operator_instance_stub(void*, TParams... params)
    {
      return Instance.operator()(params...);
    }
#endif

    //*************************************************************************
    /// Stub call for a free function.
    //*************************************************************************
    template <TReturn(*Method)(TParams...)>
    static TReturn function_stub(void*, TParams... params)
    {
      return (Method)(params...);
    }

    //*************************************************************************
    /// Stub call for a lambda or functor function.
    //*************************************************************************
    template <typename TLambda>
    static TReturn lambda_stub(void* object, TParams... arg)
    {
      TLambda* p = static_cast<TLambda*>(object);
      return (p->operator())(arg...);
    }

    //*************************************************************************
    /// The invocation object.
    //*************************************************************************
    invocation_element invocation;
  };
}

#endif
