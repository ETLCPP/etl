///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#ifndef ETL_CLOSURE_INCLUDED
#define ETL_CLOSURE_INCLUDED

#include "platform.h"
#include "delegate.h"
#include "tuple.h"
#include "utility.h"

namespace etl
{
#if ETL_USING_CPP11 && !defined(ETL_CLOSURE_FORCE_CPP03_IMPLEMENTATION)
  //*************************************************************************
  /// Base template for closure.
  //*************************************************************************
  template <typename>
  class closure;

  //*************************************************************************
  /// Closure for binding arguments to a delegate and invoking it later.
  /// Stores a delegate and its arguments, allowing deferred execution.
  /// Arguments are stored in a tuple and can be rebound using bind().
  /// Example usage:
  /// \code
  /// etl::closure<void(int, int)> c(my_delegate, 1, 2);
  /// c(); // Invokes my_delegate(1, 2)
  /// \endcode
  /// \tparam TReturn The return type of the delegate.
  /// \tparam TArgs   The argument types of the delegate.
  //*************************************************************************
  template <typename TReturn, typename... TArgs>
  class closure<TReturn(TArgs...)>
  {
  public:
   
    using delegate_type = etl::delegate<TReturn(TArgs...)>; ///< The delegate type to be invoked.   
    using tuple_type    = etl::tuple<TArgs...>;             ///< Tuple type for storing arguments.

    //*********************************************************************
    /// Construct a closure with a delegate and its arguments.
    /// \param f    The delegate to be invoked.
    /// \param args The arguments to bind to the delegate.
    //*********************************************************************
    closure(const delegate_type& f, const TArgs... args)
      : m_f(f)
      , m_args(args...)
    {
    }

    //*********************************************************************
    /// Invoke the stored delegate with the bound arguments.
    /// \return The result of the delegate invocation.
    //*********************************************************************
    TReturn operator()() const
    {
      return execute(etl::index_sequence_for<TArgs...>{});
    }

    //*********************************************************************
    /// Bind a new value to the argument at the specified index.
    /// Only non-reference types can be rebound.
    /// \tparam index Index of the argument to bind.
    /// \tparam TArg  Type of the argument.
    /// \param arg    The new value to bind.
    //*********************************************************************
    template <size_t index, typename TArg,
              typename = etl::enable_if_t<etl::is_same<TArg, etl::tuple_element_t<index, tuple_type>>::value && !etl::is_reference<TArg>::value>>
    void bind(TArg arg)
    {
      etl::get<index>(m_args) = arg;
    }

    //*********************************************************************
    /// Bind new values to multiple arguments at once.
    /// The number and types of arguments must match the tuple.
    /// \param args The new values to bind.
    ///*********************************************************************
    template <typename... UArgs>
    void bind(UArgs&&... args)
    {
      static_assert(sizeof...(UArgs) == sizeof...(TArgs), "Argument count mismatch");
      bind_impl(etl::make_index_sequence<sizeof...(TArgs)>{}, etl::forward<UArgs>(args)...);
    }

  private:

    //*********************************************************************
    /// Implementation to bind new values to multiple arguments at once.
    /// \param args The new values to bind.
    ///*********************************************************************
    template <size_t... Indexes, typename... UArgs>
    void bind_impl(etl::index_sequence<Indexes...>, UArgs&&... args)
    {
      // Use an initializer list to expand the pack and assign each argument
      int dummy[] = {0, (etl::get<Indexes>(m_args) = etl::forward<UArgs>(args), 0)...};
      (void)dummy; // Suppress unused variable warning
    }

    //*********************************************************************
    /// Execute the closure with the stored arguments.
    /// \tparam idx Index sequence for tuple unpacking.
    /// \return The result of the delegate invocation.
    //*********************************************************************
    template<size_t... Indexes>
    TReturn execute(etl::index_sequence<Indexes...>) const
    {
      return m_f(etl::get<Indexes>(m_args)...);
    }

    delegate_type m_f; ///< The delegate to invoke.
    tuple_type m_args; ///< The bound arguments.
  };
#else
  //*************************************************************************
  /// Base template for closure.
  //*************************************************************************
  template <typename>
  class closure;  

  //*************************************************************************
  /// Closure for binding one argument to a delegate and invoking it later.
  /// \tparam TReturn The return type of the delegate.
  /// \tparam TArg0    The type of the argument.
  //*************************************************************************
  template<typename TReturn, typename TArg0>
  class closure<TReturn(TArg0)>
  {
  public:

    /// The delegate type to be invoked.
    typedef etl::delegate<TReturn(TArg0)> delegate_type;

    //*********************************************************************
    /// Construct a closure with a delegate and its argument.
    /// \param f    The delegate to be invoked.
    /// \param arg0 The argument to bind to the delegate.
    //*********************************************************************
    closure(const delegate_type& f, const TArg0 arg0)
      : m_f(f)
      , m_arg0(arg0)
    {
    }

    //*********************************************************************
    /// Invoke the stored delegate with the bound argument.
    /// \return The result of the delegate invocation.
    //*********************************************************************
    TReturn operator()() const 
    {
      return m_f(m_arg0);
    }

  private:

    delegate_type m_f; ///< The delegate to invoke.
    TArg0 m_arg0;
  };

  //*************************************************************************
  /// Closure for binding two arguments to a delegate and invoking it later.
  /// \tparam TReturn The return type of the delegate.
  /// \tparam TArg0    The type of the first argument.
  /// \tparam TArg1    The type of the second argument.
  //*************************************************************************
  template<typename TReturn, typename TArg0, typename TArg1>
  class closure<TReturn(TArg0, TArg1)>
  {
  public:

    typedef etl::delegate<TReturn(TArg0, TArg1)> delegate_type;

    //*********************************************************************
    /// Construct a closure with a delegate and its arguments.
    /// \param f    The delegate to be invoked.
    /// \param arg0 The first argument to bind.
    /// \param arg1 The second argument to bind.
    //*********************************************************************
    closure(const delegate_type& f, const TArg0 arg0, const TArg1 arg1)
      : m_f(f)
      , m_arg0(arg0)
      , m_arg1(arg1)
    {
    }

    //*********************************************************************
    /// Invoke the stored delegate with the bound arguments.
    /// \return The result of the delegate invocation.
    //*********************************************************************
    TReturn operator()() const 
    {
      return m_f(m_arg0, m_arg1);
    }

  private:

    delegate_type m_f; ///< The delegate to invoke.
    TArg0 m_arg0;
    TArg1 m_arg1;
  };

  //*************************************************************************
  /// Closure for binding three arguments to a delegate and invoking it later.
  /// \tparam TReturn The return type of the delegate.
  /// \tparam TArg0    The type of the first argument.
  /// \tparam TArg1    The type of the second argument.
  /// \tparam TArg2    The type of the third argument.
  //*************************************************************************
  template<typename TReturn, typename TArg0, typename TArg1, typename TArg2>
  class closure<TReturn(TArg0, TArg1, TArg2)>
  {
  public:

    typedef etl::delegate<TReturn(TArg0, TArg1, TArg2)> delegate_type;

    //*********************************************************************
    /// Construct a closure with a delegate and its arguments.
    /// \param f    The delegate to be invoked.
    /// \param arg0 The first argument to bind.
    /// \param arg1 The second argument to bind.
    /// \param arg2 The third argument to bind.
    //*********************************************************************
    closure(const delegate_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2)
      : m_f(f)
      , m_arg0(arg0)
      , m_arg1(arg1)
      , m_arg2(arg2)
    {
    }

    //*********************************************************************
    /// Invoke the stored delegate with the bound arguments.
    /// \return The result of the delegate invocation.
    //*********************************************************************
    TReturn operator()() const 
    {
      return m_f(m_arg0, m_arg1, m_arg2);
    }

  private:

    delegate_type m_f; ///< The delegate to invoke.
    TArg0 m_arg0;
    TArg1 m_arg1;
    TArg2 m_arg2;
  };

  //*************************************************************************
  /// Closure for binding four arguments to a delegate and invoking it later.
  /// \tparam TReturn The return type of the delegate.
  /// \tparam TArg0    The type of the first argument.
  /// \tparam TArg1    The type of the second argument.
  /// \tparam TArg2    The type of the third argument.
  /// \tparam TArg3    The type of the fourth argument.
  //*************************************************************************
  template<typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3>
  class closure<TReturn(TArg0, TArg1, TArg2, TArg3)>
  {
  public:

    typedef etl::delegate<TReturn(TArg0, TArg1, TArg2, TArg3)> delegate_type;

    //*********************************************************************
    /// Construct a closure with a delegate and its arguments.
    /// \param f    The delegate to be invoked.
    /// \param arg0 The first argument to bind.
    /// \param arg1 The second argument to bind.
    /// \param arg2 The third argument to bind.
    /// \param arg3 The fourth argument to bind.
    //*********************************************************************
    closure(const delegate_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2, const TArg3 arg3)
      : m_f(f)
      , m_arg0(arg0)
      , m_arg1(arg1)
      , m_arg2(arg2)
      , m_arg3(arg3)
    {
    }

    //*********************************************************************
    /// Invoke the stored delegate with the bound arguments.
    /// \return The result of the delegate invocation.
    //*********************************************************************
    TReturn operator()() const 
    {
      return m_f(m_arg0, m_arg1, m_arg2, m_arg3);
    }

  private:

    delegate_type m_f; ///< The delegate to invoke.
    TArg0 m_arg0;
    TArg1 m_arg1;
    TArg2 m_arg2;
    TArg3 m_arg3;
  };

  //*************************************************************************
  /// Closure for binding five arguments to a delegate and invoking it later.
  /// \tparam TReturn The return type of the delegate.
  /// \tparam TArg0    The type of the first argument.
  /// \tparam TArg1    The type of the second argument.
  /// \tparam TArg2    The type of the third argument.
  /// \tparam TArg3    The type of the fourth argument.
  /// \tparam TArg4    The type of the fifth argument.
  //*************************************************************************
  template<typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3, typename TArg4>
  class closure<TReturn(TArg0, TArg1, TArg2, TArg3, TArg4)>
  {
  public:

    typedef etl::delegate<TReturn(TArg0, TArg1, TArg2, TArg3, TArg4)> delegate_type;

    //*********************************************************************
    /// Construct a closure with a delegate and its arguments.
    /// \param f    The delegate to be invoked.
    /// \param arg0 The first argument to bind.
    /// \param arg1 The second argument to bind.
    /// \param arg2 The third argument to bind.
    /// \param arg3 The fourth argument to bind.
    /// \param arg4 The fifth argument to bind.
    //*********************************************************************
    closure(const delegate_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2, const TArg3 arg3, const TArg4 arg4)
      : m_f(f)
      , m_arg0(arg0)
      , m_arg1(arg1)
      , m_arg2(arg2)
      , m_arg3(arg3)
      , m_arg4(arg4)
    {
    }

    //*********************************************************************
    /// Invoke the stored delegate with the bound arguments.
    /// \return The result of the delegate invocation.
    //*********************************************************************
    TReturn operator()() const 
    {
      return m_f(m_arg0, m_arg1, m_arg2, m_arg3, m_arg4);
    }

  private:

    delegate_type m_f; ///< The delegate to invoke.
    TArg0 m_arg0;
    TArg1 m_arg1;
    TArg2 m_arg2;
    TArg3 m_arg3;
    TArg4 m_arg4;
  };
#endif
}

#endif