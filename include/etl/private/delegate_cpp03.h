/////\file
//
///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2021 John Wellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

///******************************************************************************
//
//Copyright (C) 2017 by Sergey A Kryukov: derived work
//http://www.SAKryukov.org
//http://www.codeproject.com/Members/SAKryukov
//
//Based on original work by Sergey Ryazanov:
//"The Impossibly Fast C++ Delegates", 18 Jul 2005
//https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates
//
//MIT license:
//http://en.wikipedia.org/wiki/MIT_License
//
//Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed
//
//******************************************************************************/

#ifndef ETL_DELEGATE_CPP03_INCLUDED
#define ETL_DELEGATE_CPP03_INCLUDED

#include "../platform.h"
#include "../error_handler.h"
#include "../exception.h"
#include "../type_traits.h"
#include "../utility.h"
#include "../optional.h"

#if defined(ETL_IN_DELEGATE_CPP03_UNIT_TEST)
namespace etl_cpp03
#else
namespace etl
#endif
{
  namespace private_delegate
  {
    //***********************************
    template <typename TDelegate, typename TReturn, typename TParam>
    struct call_if_impl
    {    
      etl::optional<TReturn> call_if(TParam param)
      {
				TDelegate& d = static_cast<TDelegate&>(*this);
				
        etl::optional<TReturn> result;

        if (d.is_valid())
        {
          result = d(param);
        }

        return result;
      }
    };

    //***********************************
    template <typename TDelegate>
    struct call_if_impl<TDelegate, void, void>
    {
      bool call_if()
      {
				TDelegate& d = static_cast<TDelegate&>(*this);
				
        if (d.is_valid())
        {
          d();
          return true;
        }
        else
        {
          return false;
        }
      }
    };

    //***********************************
    template <typename TDelegate, typename TReturn>
    struct call_if_impl<TDelegate, TReturn, void>
    {
      etl::optional<TReturn> call_if()
      {
				TDelegate& d = static_cast<TDelegate&>(*this);
				
        etl::optional<TReturn> result;

        if (d.is_valid())
        {
          result = d();
        }

        return result;
      }
    };

    //***********************************
    template <typename TDelegate, typename TParam>
    struct call_if_impl<TDelegate, void, TParam>
    {
      bool call_if(TParam param)
      {
        TDelegate& d = static_cast<TDelegate&>(*this);

        if (d.is_valid())
        {
          d(param);
          return true;
        }
        else
        {
          return false;
        }
      }
    };
  }

  //***************************************************************************
  /// The base class for delegate exceptions.
  //***************************************************************************
  class delegate_exception : public etl::exception
  {
  public:

    delegate_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //***************************************************************************
  /// The exception thrown when the delegate is uninitialised.
  //***************************************************************************
  class delegate_uninitialised : public delegate_exception
  {
  public:

    delegate_uninitialised(string_type file_name_, numeric_type line_number_)
      : delegate_exception(ETL_ERROR_TEXT("delegate:uninitialised", ETL_DELEGATE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //*************************************************************************
  /// Declaration.
  //*************************************************************************
  template <typename T>
  class delegate;

  template <typename TReturn, typename TParam>
  class delegate<TReturn(TParam)> : public private_delegate::call_if_impl<delegate<TReturn(TParam)>, TReturn, TParam>
  {
  private:

    typedef delegate<TReturn(TParam)> delegate_type;

  public:

    using private_delegate::call_if_impl<delegate<TReturn(TParam)>, TReturn, TParam>::call_if;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    delegate()
    {
    }

    //*************************************************************************
    // Copy constructor.
    //*************************************************************************
    delegate(const delegate& other)
    {
      invocation = other.invocation;
    }

    //*************************************************************************
    // Construct from a functor.
    //*************************************************************************
    template <typename TFunctor>
    delegate(TFunctor& instance, typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, int>::type = 0)
    {
      assign((void*)(&instance), functor_stub<TFunctor>);
    }

    //*************************************************************************
    // Construct from a const functor.
    //*************************************************************************
    template <typename TFunctor>
    delegate(const TFunctor& instance, typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, int>::type = 0)
    {
      assign((void*)(&instance), const_functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Create from function (Compile time).
    //*************************************************************************
    template <TReturn(*Method)(TParam)>
    static delegate create()
    {
      return delegate(ETL_NULLPTR, function_stub<Method>);
    }

    //*************************************************************************
    /// Create from a Functor.
    //*************************************************************************
    template <typename TFunctor>
    static 
      typename etl::enable_if<etl::is_class<TFunctor>::value &&!etl::is_same<delegate_type, TFunctor>::value, delegate>::type
      create(TFunctor& instance)
    {
      return delegate((void*)(&instance), functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Create from a const Functor.
    //*************************************************************************
    template <typename TFunctor>
    static
      typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, delegate>::type
      create(const TFunctor& instance)
    {
      return delegate((void*)(&instance), const_functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Create from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParam)>
    static delegate create(T& instance)
    {
      return delegate((void*)(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParam) const>
    static delegate create(const T& instance)
    {
      return delegate((void*)(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::*Method)(TParam)>
    static delegate create()
    {
      return delegate(method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    /// Create from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn(T::*Method)(TParam) const>
    static delegate create()
    {
      return delegate(const_method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    /// Set from function (Compile time).
    //*************************************************************************
    template <TReturn(*Method)(TParam)>
    void set()
    {
      assign(ETL_NULLPTR, function_stub<Method>);
    }

    //*************************************************************************
    /// Set from Functor.
    //*************************************************************************
    template <typename TFunctor>
    typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, void>::type
      set(TFunctor& instance)
    {
      assign((void*)(&instance), functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Set from const Functor.
    //*************************************************************************
    template <typename TFunctor>
    typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, void>::type
      set(const TFunctor& instance)
    {
      assign((void*)(&instance), const_functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Set from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::* Method)(TParam)>
    void set(T& instance)
    {
      assign((void*)(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Set from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::* Method)(TParam) const>
    void set(T& instance)
    {
      assign((void*)(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Set from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::* Method)(TParam)>
    void set()
    {
      assign(ETL_NULLPTR, method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    /// Set from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn(T::* Method)(TParam) const>
    void set()
    {
      assign(ETL_NULLPTR, const_method_instance_stub<T, Instance, Method>);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 8))
    //*************************************************************************
    /// Create from instance function operator (Compile time).
    /// At the time of writing, GCC appears to have trouble with this.
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
    TReturn operator()(TParam param) const
    {
      ETL_ASSERT(is_valid(), ETL_ERROR(delegate_uninitialised));

      return (*invocation.stub)(invocation.object, param);
    }

    //*************************************************************************
    /// Execute the delegate if valid or call alternative.
    /// Run time alternative.
    //*************************************************************************
    template <typename TAlternative>
    TReturn call_or(TAlternative alternative, TParam param) const
    {
      if (is_valid())
      {
        return (*invocation.stub)(invocation.object, param);
      }
      else
      {
        return alternative(param);
      }
    }

    //*************************************************************************
    /// Execute the delegate if valid or call alternative.
    /// Compile time alternative.
    //*************************************************************************
    template <TReturn(*Method)(TParam)>
    TReturn call_or(TParam param) const
    {
      if (is_valid())
      {
        return (*invocation.stub)(invocation.object, param);
      }
      else
      {
        return (Method)(param);
      }
    }

    //*************************************************************************
    /// Create from function (Compile time).
    //*************************************************************************
    delegate& operator =(const delegate& rhs)
    {
      invocation = rhs.invocation;
      return *this;
    }

    //*************************************************************************
    /// Create from Functor.
    //*************************************************************************
    template <typename TFunctor>
    typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, delegate&>::type
      operator =(TFunctor& instance)
    {
      assign((void*)(&instance), functor_stub<TFunctor>);
      return *this;
    }

    //*************************************************************************
    /// Create from const Functor.
    //*************************************************************************
    template <typename TFunctor>
    typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, delegate&>::type
      operator =(const TFunctor& instance)
    {
      assign((void*)(&instance), const_functor_stub<TFunctor>);
      return *this;
    }

    //*************************************************************************
    /// Checks equality.
    //*************************************************************************
    bool operator == (const delegate& rhs) const
    {
      return invocation == rhs.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    bool operator != (const delegate& rhs) const
    {
      return invocation != rhs.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    bool is_valid() const
    {
      return invocation.stub != ETL_NULLPTR;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    operator bool() const
    {
      return is_valid();
    }

  private:

    typedef TReturn(*stub_type)(void* object, TParam);

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      invocation_element()
        : object(ETL_NULLPTR)
        , stub(ETL_NULLPTR)
			{
			}

      //***********************************************************************
      invocation_element(void* object_, stub_type stub_)
        : object(object_)
        , stub(stub_)
      {
      }

      //***********************************************************************
      bool operator ==(const invocation_element& rhs) const
      {
        return (rhs.stub == stub) && (rhs.object == object);
      }

      //***********************************************************************
      bool operator !=(const invocation_element& rhs) const
      {
        return (rhs.stub != stub) || (rhs.object != object);
      }

      //***********************************************************************
      void*     object;
      stub_type stub;
    };

    //*************************************************************************
    /// Constructs a delegate from an object and stub.
    //*************************************************************************
    delegate(void* object, stub_type stub)
      : invocation(object, stub)
    {
    }

    //*************************************************************************
    /// Constructs a delegate from a stub.
    //*************************************************************************
    delegate(stub_type stub)
      : invocation(ETL_NULLPTR, stub)
    {
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
    template <typename T, TReturn(T::*Method)(TParam)>
    static TReturn method_stub(void* object, TParam param)
    {
      T* p = static_cast<T*>(object);
      return (p->*Method)(param);
    }

    //*************************************************************************
    /// Stub call for a const member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn(T::*Method)(TParam) const>
    static TReturn const_method_stub(void* object, TParam param)
    {
      T* const p = static_cast<T*>(object);
      return (p->*Method)(param);
    }

    //*************************************************************************
    /// Stub call for a member function. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::*Method)(TParam)>
    static TReturn method_instance_stub(void*, TParam param)
    {
      return (Instance.*Method)(param);
    }

    //*************************************************************************
    /// Stub call for a const member function. Compile time instance.
    //*************************************************************************
    template <typename T, const T& Instance, TReturn(T::*Method)(TParam) const>
    static TReturn const_method_instance_stub(void*, TParam param)
    {
      return (Instance.*Method)(param);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 8))
    //*************************************************************************
    /// Stub call for a function operator. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance>
    static TReturn operator_instance_stub(void*, TParam param)
    {
      return Instance.operator()(param);
    }
#endif

    //*************************************************************************
    /// Stub call for a free function.
    //*************************************************************************
    template <TReturn(*Method)(TParam)>
    static TReturn function_stub(void*, TParam param)
    {
      return (Method)(param);
    }

    //*************************************************************************
    /// Stub call for a functor function.
    //*************************************************************************
    template <typename TFunctor>
    static TReturn functor_stub(void* object, TParam param)
    {
      TFunctor* p = static_cast<TFunctor*>(object);
      return (p->operator())(param);
    }

    //*************************************************************************
    /// Stub call for a functor function.
    //*************************************************************************
    template <typename TFunctor>
    static TReturn const_functor_stub(void* object, TParam param)
    {
      const TFunctor* p = static_cast<const TFunctor*>(object);
      return (p->operator())(param);
    }

    //*************************************************************************
    /// The invocation object.
    //*************************************************************************
    invocation_element invocation;
  };

  //*************************************************************************
  /// Specialisation for void parameter.
  //*************************************************************************
  template <typename TReturn>
  class delegate<TReturn(void)> : public private_delegate::call_if_impl<delegate<TReturn(void)>, TReturn, void>
  {
  private:

    typedef delegate<TReturn(void)> delegate_type;

  public:

    using private_delegate::call_if_impl< delegate<TReturn(void)>, TReturn, void>::call_if;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    delegate()
    {
    }

    //*************************************************************************
    // Copy constructor.
    //*************************************************************************
    delegate(const delegate& other)
    {
      invocation = other.invocation;
    }

    //*************************************************************************
    // Construct from functor.
    //*************************************************************************
    template <typename TFunctor>
    delegate(TFunctor& instance, typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, int>::type = 0)
    {
      assign((void*)(&instance), functor_stub<TFunctor>);
    }

    //*************************************************************************
    // Construct from const functor.
    //*************************************************************************
    template <typename TFunctor>
    delegate(const TFunctor& instance, typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, int>::type = 0)
    {
      assign((void*)(&instance), const_functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Create from function (Compile time).
    //*************************************************************************
    template <TReturn(*Method)()>
    static delegate create()
    {
      return delegate(ETL_NULLPTR, function_stub<Method>);
    }

    //*************************************************************************
    /// Create from Functor.
    //*************************************************************************
    template <typename TFunctor>
    static 
      typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, delegate>::type
      create(TFunctor& instance)
    {
      return delegate((void*)(&instance), functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Create from const Functor.
    //*************************************************************************
    template <typename TFunctor>
    static
      typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, delegate>::type
      create(const TFunctor& instance)
    {
      return delegate((void*)(&instance), const_functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Create from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::* Method)()>
    static delegate create(T& instance)
    {
      return delegate((void*)(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::* Method)() const>
    static delegate create(const T& instance)
    {
      return delegate((void*)(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::* Method)()>
    static delegate create()
    {
      return delegate(method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    /// Create from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn(T::* Method)() const>
    static delegate create()
    {
      return delegate(const_method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    /// Set from function (Compile time).
    //*************************************************************************
    template <TReturn(*Method)()>
    void set()
    {
      assign(ETL_NULLPTR, function_stub<Method>);
    }

    //*************************************************************************
    /// Set from Functor.
    //*************************************************************************
    template <typename TFunctor>
    typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, void>::type
      set(TFunctor& instance)
    {
      assign((void*)(&instance), functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Set from const Functor.
    //*************************************************************************
    template <typename TFunctor>
    typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, void>::type
      set(const TFunctor& instance)
    {
      assign((void*)(&instance), const_functor_stub<TFunctor>);
    }

    //*************************************************************************
    /// Set from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::* Method)()>
    void set(T& instance)
    {
      assign((void*)(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Set from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn(T::* Method)() const>
    void set(T& instance)
    {
      assign((void*)(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Set from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::* Method)()>
    void set()
    {
      assign(ETL_NULLPTR, method_instance_stub<T, Instance, Method>);
    }

    //*************************************************************************
    /// Set from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn(T::* Method)() const>
    void set()
    {
      assign(ETL_NULLPTR, const_method_instance_stub<T, Instance, Method>);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 8))
    //*************************************************************************
    /// Create from instance function operator (Compile time).
    /// At the time of writing, GCC appears to have trouble with this.
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
    TReturn operator()() const
    {
      ETL_ASSERT(is_valid(), ETL_ERROR(delegate_uninitialised));

      return (*invocation.stub)(invocation.object);
    }

    //*************************************************************************
    /// Execute the delegate if valid or call alternative.
    /// Run time alternative.
    //*************************************************************************
    template <typename TAlternative>
    TReturn call_or(TAlternative alternative) const
    {
      if (is_valid())
      {
        return (*invocation.stub)(invocation.object);
      }
      else
      {
        return alternative();
      }
    }

    //*************************************************************************
    /// Execute the delegate if valid or call alternative.
    /// Compile time alternative.
    //*************************************************************************
    template <TReturn(*Method)()>
    TReturn call_or() const
    {
      if (is_valid())
      {
        return (*invocation.stub)(invocation.object);
      }
      else
      {
        return (Method)();
      }
    }

    //*************************************************************************
    /// Assignment.
    //*************************************************************************
    delegate& operator =(const delegate& rhs)
    {
      invocation = rhs.invocation;
      return *this;
    }

    //*************************************************************************
    /// Create from Functor.
    //*************************************************************************
    template <typename TFunctor>
    typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, delegate&>::type
      operator =(TFunctor& instance)
    {
      assign((void*)(&instance), functor_stub<TFunctor>);
      return *this;
    }

    //*************************************************************************
    /// Create from const Functor.
    //*************************************************************************
    template <typename TFunctor>
    typename etl::enable_if<etl::is_class<TFunctor>::value && !etl::is_same<delegate_type, TFunctor>::value, delegate&>::type
      operator =(const TFunctor& instance)
    {
      assign((void*)(&instance), const_functor_stub<TFunctor>);
      return *this;
    }

    //*************************************************************************
    /// Checks equality.
    //*************************************************************************
    bool operator == (const delegate& rhs) const
    {
      return invocation == rhs.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    bool operator != (const delegate& rhs) const
    {
      return invocation != rhs.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    bool is_valid() const
    {
      return invocation.stub != ETL_NULLPTR;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    operator bool() const
    {
      return is_valid();
    }

  private:

    typedef TReturn(*stub_type)(void* object);

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      invocation_element()
        : object(ETL_NULLPTR)
        , stub(ETL_NULLPTR)
			{
			}

      //***********************************************************************
      invocation_element(void* object_, stub_type stub_)
        : object(object_)
        , stub(stub_)
      {
      }

      //***********************************************************************
      bool operator ==(const invocation_element& rhs) const
      {
        return (rhs.stub == stub) && (rhs.object == object);
      }

      //***********************************************************************
      bool operator !=(const invocation_element& rhs) const
      {
        return (rhs.stub != stub) || (rhs.object != object);
      }

      //***********************************************************************
      void* object;
      stub_type stub;
    };

    //*************************************************************************
    /// Constructs a delegate from an object and stub.
    //*************************************************************************
    delegate(void* object, stub_type stub)
      : invocation(object, stub)
    {
    }

    //*************************************************************************
    /// Constructs a delegate from a stub.
    //*************************************************************************
    delegate(stub_type stub)
      : invocation(ETL_NULLPTR, stub)
    {
    }

    //*************************************************************************
    /// Assign from an object and stub.
    //*************************************************************************
    void assign(void* object, stub_type stub)
    {
      invocation.object = object;
      invocation.stub = stub;
    }

    //*************************************************************************
    /// Stub call for a member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn(T::* Method)()>
    static TReturn method_stub(void* object)
    {
      T* p = static_cast<T*>(object);
      return (p->*Method)();
    }

    //*************************************************************************
    /// Stub call for a const member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn(T::* Method)() const>
    static TReturn const_method_stub(void* object)
    {
      T* const p = static_cast<T*>(object);
      return (p->*Method)();
    }

    //*************************************************************************
    /// Stub call for a member function. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance, TReturn(T::* Method)()>
    static TReturn method_instance_stub(void*)
    {
      return (Instance.*Method)();
    }

    //*************************************************************************
    /// Stub call for a const member function. Compile time instance.
    //*************************************************************************
    template <typename T, const T& Instance, TReturn(T::* Method)() const>
    static TReturn const_method_instance_stub(void*)
    {
      return (Instance.*Method)();
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 8))
    //*************************************************************************
    /// Stub call for a function operator. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance>
    static TReturn operator_instance_stub(void*)
    {
      return Instance.operator()();
    }
#endif

    //*************************************************************************
    /// Stub call for a free function.
    //*************************************************************************
    template <TReturn(*Method)()>
    static TReturn function_stub(void*)
    {
      return (Method)();
    }

    //*************************************************************************
    /// Stub call for a functor function.
    //*************************************************************************
    template <typename TFunctor>
    static TReturn functor_stub(void* object)
    {
      TFunctor* p = static_cast<TFunctor*>(object);
      return (p->operator())();
    }

    //*************************************************************************
    /// Stub call for a const functor function.
    //*************************************************************************
    template <typename TFunctor>
    static TReturn const_functor_stub(void* object)
    {
      const TFunctor* p = static_cast<const TFunctor*>(object);
      return (p->operator())();
    }

    //*************************************************************************
    /// The invocation object.
    //*************************************************************************
    invocation_element invocation;
  };
}

#endif
