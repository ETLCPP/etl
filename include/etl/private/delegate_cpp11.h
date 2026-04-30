///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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

Original publication:
https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

******************************************************************************/

#ifndef ETL_DELEGATE_CPP11_INCLUDED
#define ETL_DELEGATE_CPP11_INCLUDED

#include "../platform.h"
#include "../error_handler.h"
#include "../exception.h"
#include "../function_traits.h"
#include "../optional.h"
#include "../type_list.h"
#include "../type_traits.h"
#include "../utility.h"

namespace etl
{
  //***************************************************************************
  /// The base class for delegate exceptions.
  //***************************************************************************
  class delegate_exception : public exception
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

  //*****************************************************************
  /// The tag to identify an etl::delegate.
  ///\ingroup delegate
  //*****************************************************************
  struct delegate_tag
  {
  };

  //***************************************************************************
  /// is_delegate
  //***************************************************************************
  template <typename T>
  struct is_delegate : etl::bool_constant<etl::is_base_of<delegate_tag, T>::value>
  {
  };

#if ETL_USING_CPP17
  template <typename T>
  inline constexpr bool is_delegate_v = is_delegate<T>::value;
#endif

  //*************************************************************************
  /// Declaration.
  //*************************************************************************
  template <typename T>
  class delegate;

  //*************************************************************************
  /// Specialisation.
  //*************************************************************************
  template <typename TReturn, typename... TArgs>
  class delegate<TReturn(TArgs...)> final : public delegate_tag
  {
  private:

    using object_ptr   = void*;
    using function_ptr = TReturn (*)(TArgs...);

  public:

    using return_type    = TReturn;
    using argument_types = etl::type_list<TArgs...>;

    //*************************************************************************
    /// Default constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate() ETL_NOEXCEPT
    {
    }

    //*************************************************************************
    // Copy constructor.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate(const delegate& other) = default;

    //*************************************************************************
    // Construct from lambda or functor.
    //*************************************************************************
    template < typename TLambda, typename = etl::enable_if_t< etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate(TLambda& instance) ETL_NOEXCEPT
    {
      assign(object_ptr(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    // Construct from const lambda or functor.
    //*************************************************************************
    template < typename TLambda, typename = etl::enable_if_t< etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate(const TLambda& instance) ETL_NOEXCEPT
    {
      assign(object_ptr(&instance), const_lambda_stub<TLambda>);
    }

    //*************************************************************************
    // Delete construction from rvalue reference lambda or functor.
    // Excludes non-capturing lambdas convertible to a function pointer.
    //*************************************************************************
    template <typename TLambda,
              typename = etl::enable_if_t<etl::is_class<TLambda>::value && !etl::is_same<etl::delegate<TReturn(TArgs...)>, TLambda>::value
                                            && !etl::is_convertible<TLambda, function_ptr>::value,
                                          void>>
    ETL_CONSTEXPR14 delegate(TLambda&& instance) = delete;

    //*************************************************************************
    // Construct from a function pointer.
    //*************************************************************************
    explicit delegate(function_ptr fp) ETL_NOEXCEPT
    {
      assign(fp, function_ptr_stub);
    }

    //*************************************************************************
    /// Create from function (Compile time).
    //*************************************************************************
    template <TReturn (*Method)(TArgs...)>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(function_stub<Method>);
    }

    //*************************************************************************
    /// Create from Lambda or Functor.
    //*************************************************************************
    template < typename TLambda, typename = etl::enable_if_t< etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(TLambda& instance) ETL_NOEXCEPT
    {
      return delegate(object_ptr(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    /// Create from const Lambda or Functor.
    //*************************************************************************
    template < typename TLambda, typename = etl::enable_if_t< etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(const TLambda& instance) ETL_NOEXCEPT
    {
      return delegate(object_ptr(&instance), const_lambda_stub<TLambda>);
    }

    //*************************************************************************
    // Create from a function pointer.
    //*************************************************************************
    ETL_NODISCARD
    static delegate create(function_ptr fp) ETL_NOEXCEPT
    {
      return delegate(fp, function_ptr_stub);
    }

    //*************************************************************************
    /// Create from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...)>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(T& instance) ETL_NOEXCEPT
    {
      return delegate(object_ptr(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Create from instance method (Run time).
    /// Deleted for rvalue references.
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...)>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(T&& instance) = delete;

    //*************************************************************************
    /// Create from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...) const>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create(const T& instance) ETL_NOEXCEPT
    {
      return delegate(object_ptr(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Disable create from rvalue instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...) const>
    static ETL_CONSTEXPR14 delegate create(T&& instance) = delete;

    //*************************************************************************
    /// Create from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn (T::*Method)(TArgs...)>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Create from instance method (Compile time).
    /// New API
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...), T& Instance>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Create from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn (T::*Method)(TArgs...) const>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(const_method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Create from const instance method (Compile time).
    /// New API
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...) const, T const& Instance>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(const_method_instance_stub<T, Method, Instance>);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 8))
    //*************************************************************************
    /// Create from instance function operator (Compile time).
    /// At the time of writing, GCC appears to have trouble with this.
    //*************************************************************************
    template <typename T, T& Instance>
    ETL_NODISCARD
    static ETL_CONSTEXPR14 delegate create() ETL_NOEXCEPT
    {
      return delegate(operator_instance_stub<T, Instance>);
    }
#endif

    //*************************************************************************
    /// Set from function (Compile time).
    //*************************************************************************
    template <TReturn (*Method)(TArgs...)>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(function_stub<Method>);
    }

    //*************************************************************************
    /// Set from Lambda or Functor.
    //*************************************************************************
    template < typename TLambda, typename = etl::enable_if_t< etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 void set(TLambda& instance) ETL_NOEXCEPT
    {
      assign(object_ptr(&instance), lambda_stub<TLambda>);
    }

    //*************************************************************************
    /// Set from const Lambda or Functor.
    //*************************************************************************
    template < typename TLambda, typename = etl::enable_if_t< etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 void set(const TLambda& instance) ETL_NOEXCEPT
    {
      assign(object_ptr(&instance), const_lambda_stub<TLambda>);
    }

    //*************************************************************************
    // Set from a function pointer.
    //*************************************************************************
    void set(function_ptr fp) ETL_NOEXCEPT
    {
      assign(fp, function_ptr_stub);
    }

    //*************************************************************************
    /// Set from instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...)>
    ETL_CONSTEXPR14 void set(T& instance) ETL_NOEXCEPT
    {
      assign(object_ptr(&instance), method_stub<T, Method>);
    }

    //*************************************************************************
    /// Set from const instance method (Run time).
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...) const>
    ETL_CONSTEXPR14 void set(T& instance) ETL_NOEXCEPT
    {
      assign(object_ptr(&instance), const_method_stub<T, Method>);
    }

    //*************************************************************************
    /// Set from instance method (Compile time).
    //*************************************************************************
    template <typename T, T& Instance, TReturn (T::*Method)(TArgs...)>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Set from instance method (Compile time).
    /// New API
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...), T& Instance>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Set from const instance method (Compile time).
    //*************************************************************************
    template <typename T, T const& Instance, TReturn (T::*Method)(TArgs...) const>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(const_method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Set from const instance method (Compile time).
    /// New API
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...) const, T const& Instance>
    ETL_CONSTEXPR14 void set() ETL_NOEXCEPT
    {
      assign(const_method_instance_stub<T, Method, Instance>);
    }

    //*************************************************************************
    /// Clear the delegate.
    //*************************************************************************
    ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
    {
      invocation.clear();
    }

    //*************************************************************************
    /// Execute the delegate.
    //*************************************************************************
    template <typename... TCallArgs>
    ETL_CONSTEXPR14 return_type operator()(TCallArgs&&... args) const
    {
      ETL_STATIC_ASSERT((sizeof...(TCallArgs) == sizeof...(TArgs)), "Incorrect number of parameters passed to delegate");
      ETL_STATIC_ASSERT((etl::type_lists_are_convertible<etl::type_list<TCallArgs&&...>, argument_types>::value),
                        "Incompatible parameter types passed to delegate");

      ETL_ASSERT(is_valid(), ETL_ERROR(delegate_uninitialised));

      return (*invocation.stub)(invocation, etl::forward<TCallArgs>(args)...);
    }

    //*************************************************************************
    /// Execute the delegate if valid.
    /// 'void' return delegate.
    //*************************************************************************
    template <typename TRet = TReturn, typename... TCallArgs>
    ETL_CONSTEXPR14 typename etl::enable_if_t<etl::is_same<TRet, void>::value, bool> call_if(TCallArgs&&... args) const
    {
      ETL_STATIC_ASSERT((sizeof...(TCallArgs) == sizeof...(TArgs)), "Incorrect number of parameters passed to delegate");
      ETL_STATIC_ASSERT((etl::type_lists_are_convertible<etl::type_list<TCallArgs&&...>, argument_types>::value),
                        "Incompatible parameter types passed to delegate");

      if (is_valid())
      {
        (*invocation.stub)(invocation, etl::forward<TCallArgs>(args)...);
        return true;
      }
      else
      {
        return false;
      }
    }

    //*************************************************************************
    /// Execute the delegate if valid.
    /// Non 'void' return delegate.
    //*************************************************************************
    template <typename TRet = TReturn, typename... TCallArgs>
    ETL_CONSTEXPR14 typename etl::enable_if_t<!etl::is_same<TRet, void>::value, etl::optional<TReturn>> call_if(TCallArgs&&... args) const
    {
      ETL_STATIC_ASSERT((sizeof...(TCallArgs) == sizeof...(TArgs)), "Incorrect number of parameters passed to delegate");
      ETL_STATIC_ASSERT((etl::type_lists_are_convertible<etl::type_list<TCallArgs&&...>, argument_types>::value),
                        "Incompatible parameter types passed to delegate");

      etl::optional<TReturn> result;

      if (is_valid())
      {
        result = (*invocation.stub)(invocation, etl::forward<TCallArgs>(args)...);
      }

      return result;
    }

    //*************************************************************************
    /// Execute the delegate if valid or call alternative.
    /// Run time alternative.
    //*************************************************************************
    template <typename TAlternative, typename... TCallArgs>
    ETL_CONSTEXPR14 TReturn call_or(TAlternative&& alternative, TCallArgs&&... args) const
    {
      ETL_STATIC_ASSERT((sizeof...(TCallArgs) == sizeof...(TArgs)), "Incorrect number of parameters passed to delegate");
      ETL_STATIC_ASSERT((etl::type_lists_are_convertible<etl::type_list<TCallArgs&&...>, argument_types>::value),
                        "Incompatible parameter types passed to delegate");

      if (is_valid())
      {
        return (*invocation.stub)(invocation, etl::forward<TCallArgs>(args)...);
      }
      else
      {
        return etl::forward<TAlternative>(alternative)(etl::forward<TCallArgs>(args)...);
      }
    }

    //*************************************************************************
    /// Execute the delegate if valid or call alternative.
    /// Compile time alternative.
    //*************************************************************************
    template <TReturn (*Method)(TArgs...), typename... TCallArgs>
    ETL_CONSTEXPR14 TReturn call_or(TCallArgs&&... args) const
    {
      ETL_STATIC_ASSERT((sizeof...(TCallArgs) == sizeof...(TArgs)), "Incorrect number of parameters passed to delegate");
      ETL_STATIC_ASSERT((etl::type_lists_are_convertible<etl::type_list<TCallArgs&&...>, argument_types>::value),
                        "Incompatible parameter types passed to delegate");

      if (is_valid())
      {
        return (*invocation.stub)(invocation, etl::forward<TCallArgs>(args)...);
      }
      else
      {
        return (Method)(etl::forward<TCallArgs>(args)...);
      }
    }

    //*************************************************************************
    /// Assignment
    //*************************************************************************
    delegate& operator=(const delegate& rhs) = default;

    //*************************************************************************
    /// Create from Lambda or Functor.
    //*************************************************************************
    template < typename TLambda, typename = etl::enable_if_t< etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate& operator=(TLambda& instance) ETL_NOEXCEPT
    {
      assign(object_ptr(&instance), lambda_stub<TLambda>);
      return *this;
    }

    //*************************************************************************
    /// Create from const Lambda or Functor.
    //*************************************************************************
    template < typename TLambda, typename = etl::enable_if_t< etl::is_class<TLambda>::value && !is_delegate<TLambda>::value, void>>
    ETL_CONSTEXPR14 delegate& operator=(const TLambda& instance) ETL_NOEXCEPT
    {
      assign(object_ptr(&instance), const_lambda_stub<TLambda>);
      return *this;
    }

    //*************************************************************************
    // Create from a function pointer.
    //*************************************************************************
    delegate& operator=(function_ptr fp) ETL_NOEXCEPT
    {
      if (fp == ETL_NULLPTR)
      {
        invocation.clear();
      }
      else
      {
        assign(fp, function_ptr_stub);
      }
      return *this;
    }

    //*************************************************************************
    /// Checks equality.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 bool operator==(const delegate& rhs) const ETL_NOEXCEPT
    {
      return invocation == rhs.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    ETL_CONSTEXPR14 bool operator!=(const delegate& rhs) const ETL_NOEXCEPT
    {
      return invocation != rhs.invocation;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 bool is_valid() const ETL_NOEXCEPT
    {
      // GCC's UBSan instruments function pointer comparisons, which prevents
      // constexpr evaluation. Use implicit bool conversion at compile time
      // to avoid the instrumented != comparison while still checking validity.
      if (etl::is_constant_evaluated())
      {
        return static_cast<bool>(invocation.stub);
      }

      return invocation.stub != ETL_NULLPTR;
    }

    //*************************************************************************
    /// Returns <b>true</b> if the delegate is valid.
    //*************************************************************************
    ETL_NODISCARD ETL_CONSTEXPR14 operator bool() const ETL_NOEXCEPT
    {
      return is_valid();
    }

  private:

    //*************************************************************************
    // Callable compatibility: detects if C (or const C) is invocable with
    // (TArgs...) and returns a type convertible to TReturn. Works with generic
    // lambdas and functors.
    template <typename TCallableType, typename = void>
    struct is_invocable_with : etl::false_type
    {
    };

    template <typename TCallableType>
    struct is_invocable_with< TCallableType, etl::void_t<decltype(etl::declval<TCallableType&>()(etl::declval<TArgs>()...))>>
      : etl::bool_constant< etl::is_convertible< decltype(etl::declval<TCallableType&>()(etl::declval<TArgs>()...)), TReturn>::value>
    {
    };

    template <typename TCallableType, typename = void>
    struct is_invocable_with_const : etl::false_type
    {
    };

    template <typename TCallableType>
    struct is_invocable_with_const< TCallableType, etl::void_t<decltype(etl::declval<const TCallableType&>()(etl::declval<TArgs>()...))>>
      : etl::bool_constant< etl::is_convertible< decltype(etl::declval<const TCallableType&>()(etl::declval<TArgs>()...)), TReturn>::value>
    {
    };

    template <typename TCallableType>
    struct is_compatible_callable : etl::bool_constant<is_invocable_with<TCallableType>::value || is_invocable_with_const<TCallableType>::value>
    {
    };

    //*************************************************************************
    /// The internal invocation object.
    //*************************************************************************
    struct invocation_element
    {
      using stub_type = TReturn (*)(const invocation_element&, TArgs...);

      //***********************************************************************
      ETL_CONSTEXPR14 invocation_element() ETL_NOEXCEPT
        : ptr(object_ptr(ETL_NULLPTR))
        , stub(ETL_NULLPTR)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR14 invocation_element(stub_type stub_) ETL_NOEXCEPT
        : ptr()
        , stub(stub_)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR14 invocation_element(object_ptr object_, stub_type stub_) ETL_NOEXCEPT
        : ptr(object_)
        , stub(stub_)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR14 invocation_element(function_ptr fp_, stub_type stub_) ETL_NOEXCEPT
        : ptr(fp_)
        , stub(stub_)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR14 bool operator==(const invocation_element& rhs) const ETL_NOEXCEPT
      {
        return (rhs.stub == stub) && ((stub == function_ptr_stub) ? (rhs.ptr.fp == ptr.fp) : (rhs.ptr.object == ptr.object));
      }

      //***********************************************************************
      ETL_CONSTEXPR14 bool operator!=(const invocation_element& rhs) const ETL_NOEXCEPT
      {
        return !operator==(rhs);
      }

      //***********************************************************************
      ETL_CONSTEXPR14 void clear() ETL_NOEXCEPT
      {
        stub = ETL_NULLPTR;
      }

      //***********************************************************************
      union ptr_type
      {
        ETL_CONSTEXPR14 ptr_type() ETL_NOEXCEPT
          : object(ETL_NULLPTR)
        {
        }

        ETL_CONSTEXPR14 ptr_type(object_ptr object_) ETL_NOEXCEPT
          : object(object_)
        {
        }

        ETL_CONSTEXPR14 ptr_type(function_ptr fp_) ETL_NOEXCEPT
          : fp(fp_)
        {
        }

        object_ptr   object;
        function_ptr fp;
      };

      ptr_type  ptr;
      stub_type stub;
    };

    using stub_type = typename invocation_element::stub_type;

    //*************************************************************************
    /// Constructs a delegate from an object and stub.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate(object_ptr object, stub_type stub) ETL_NOEXCEPT
      : invocation(object, stub)
    {
    }

    //*************************************************************************
    /// Constructs a delegate from an function and stub.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate(function_ptr fp, stub_type stub) ETL_NOEXCEPT
      : invocation(fp, stub)
    {
    }

    //*************************************************************************
    /// Constructs a delegate from a stub.
    //*************************************************************************
    ETL_CONSTEXPR14 delegate(stub_type stub) ETL_NOEXCEPT
      : invocation(stub)
    {
    }

    //*************************************************************************
    /// Assign from an object and stub.
    //*************************************************************************
    ETL_CONSTEXPR14 void assign(object_ptr object, stub_type stub) ETL_NOEXCEPT
    {
      invocation.ptr.object = object;
      invocation.stub       = stub;
    }

    //*************************************************************************
    /// Assign from a function and stub.
    //*************************************************************************
    ETL_CONSTEXPR14 void assign(function_ptr fp, stub_type stub) ETL_NOEXCEPT
    {
      invocation.ptr.fp = fp;
      invocation.stub   = stub;
    }

    //*************************************************************************
    /// Assign from a stub.
    //*************************************************************************
    ETL_CONSTEXPR14 void assign(stub_type stub) ETL_NOEXCEPT
    {
      invocation.ptr.object = ETL_NULLPTR;
      invocation.stub       = stub;
    }

    //*************************************************************************
    /// Stub call for a member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...)>
    static ETL_CONSTEXPR14 TReturn method_stub(const invocation_element& invocation, TArgs... args)
    {
      T* p = static_cast<T*>(invocation.ptr.object);
      return (p->*Method)(etl::forward<TArgs>(args)...);
    }

    //*************************************************************************
    /// Stub call for a const member function. Run time instance.
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...) const>
    static ETL_CONSTEXPR14 TReturn const_method_stub(const invocation_element& invocation, TArgs... args)
    {
      T* const p = static_cast<T*>(invocation.ptr.object);
      return (p->*Method)(etl::forward<TArgs>(args)...);
    }

    //*************************************************************************
    /// Stub call for a member function. Compile time instance.
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...), T& Instance>
    static ETL_CONSTEXPR14 TReturn method_instance_stub(const invocation_element&, TArgs... args)
    {
      return (Instance.*Method)(etl::forward<TArgs>(args)...);
    }

    //*************************************************************************
    /// Stub call for a const member function. Compile time instance.
    //*************************************************************************
    template <typename T, TReturn (T::*Method)(TArgs...) const, const T& Instance>
    static ETL_CONSTEXPR14 TReturn const_method_instance_stub(const invocation_element&, TArgs... args)
    {
      return (Instance.*Method)(etl::forward<TArgs>(args)...);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 8))
    //*************************************************************************
    /// Stub call for a function operator. Compile time instance.
    //*************************************************************************
    template <typename T, T& Instance>
    static ETL_CONSTEXPR14 TReturn operator_instance_stub(const invocation_element&, TArgs... args)
    {
      return Instance.operator()(etl::forward<TArgs>(args)...);
    }
#endif

    //*************************************************************************
    /// Stub call for a free function.
    //*************************************************************************
    template <TReturn (*Method)(TArgs...)>
    static ETL_CONSTEXPR14 TReturn function_stub(const invocation_element&, TArgs... args)
    {
      return (Method)(etl::forward<TArgs>(args)...);
    }

    //*************************************************************************
    /// Stub call for a runtime function pointer stored in the invocation_element.
    //*************************************************************************
    static TReturn function_ptr_stub(const invocation_element& invocation, TArgs... args)
    {
      return invocation.ptr.fp(etl::forward<TArgs>(args)...);
    }

    //*************************************************************************
    /// Stub call for a lambda or functor function.
    //*************************************************************************
    template <typename TLambda>
    static ETL_CONSTEXPR14 TReturn lambda_stub(const invocation_element& invocation, TArgs... arg)
    {
      ETL_STATIC_ASSERT(is_compatible_callable<TLambda>::value, "etl::delegate: bound lambda/functor is not compatible with the delegate signature");

      TLambda* p = static_cast<TLambda*>(invocation.ptr.object);
      return (p->operator())(etl::forward<TArgs>(arg)...);
    }

    //*************************************************************************
    /// Stub call for a const lambda or functor function.
    //*************************************************************************
    template <typename TLambda>
    static ETL_CONSTEXPR14 TReturn const_lambda_stub(const invocation_element& invocation, TArgs... arg)
    {
      ETL_STATIC_ASSERT(is_compatible_callable<TLambda>::value, "etl::delegate: bound lambda/functor is not compatible with the delegate signature");

      const TLambda* p = static_cast<const TLambda*>(invocation.ptr.object);
      return (p->operator())(etl::forward<TArgs>(arg)...);
    }

    //*************************************************************************
    /// The invocation object.
    //*************************************************************************
    invocation_element invocation;
  };

#if ETL_USING_CPP17
  //*************************************************************************
  /// Make a delegate from a free function.
  //*************************************************************************
  template <auto Function>
  ETL_NODISCARD
  constexpr auto make_delegate() ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Function)>::function_type;

    return etl::delegate<function_type>::template create<Function>();
  }

  //*************************************************************************
  /// Make a delegate from a functor or lambda function.
  //*************************************************************************
  template <typename TLambda, typename = etl::enable_if_t<etl::is_class<TLambda>::value, void>>
  ETL_NODISCARD
  constexpr auto make_delegate(TLambda& instance) ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits< decltype(&TLambda::operator())>::function_type;

    return etl::delegate<function_type>(instance);
  }

  //*************************************************************************
  /// Make a delegate from a functor, compile time.
  //*************************************************************************
  template <typename T, T& Instance>
  ETL_NODISCARD
  constexpr auto make_delegate() ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(&T::operator())>::function_type;

    return etl::delegate<function_type>::template create<T, Instance>();
  }

  //*************************************************************************
  /// Make a delegate from a member function at compile time.
  //*************************************************************************
  template <typename T, auto Method, T& Instance, typename = etl::enable_if_t< !etl::function_traits<decltype(Method)>::is_const>>
  ETL_NODISCARD
  constexpr auto make_delegate() ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::delegate<function_type>::template create<T, Method, Instance>();
  }

  //*************************************************************************
  /// Make a delegate from a const member function at compile time.
  //*************************************************************************
  template <typename T, auto Method, const T& Instance, typename = etl::enable_if_t< etl::function_traits<decltype(Method)>::is_const>>
  ETL_NODISCARD
  constexpr auto make_delegate() ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::delegate<function_type>::template create<T, Method, Instance>();
  }

  //*************************************************************************
  /// Make a delegate from a member function at run time.
  //*************************************************************************
  template <typename T, auto Method>
  ETL_NODISCARD
  constexpr auto make_delegate(T& instance) ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::delegate<function_type>::template create<T, Method>(instance);
  }

  //*************************************************************************
  /// Make a delegate from a member function at run time.
  //*************************************************************************
  template <typename T, auto Method>
  ETL_NODISCARD
  constexpr auto make_delegate(const T& instance) ETL_NOEXCEPT
  {
    using function_type = typename etl::function_traits<decltype(Method)>::function_type;

    return etl::delegate<function_type>::template create<T, Method>(instance);
  }
#endif
} // namespace etl

#endif
