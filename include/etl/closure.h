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
#include "type_list.h"
#include "utility.h"

namespace etl
{
  //*************************************************************************
  /// Base template for closure.
  /// \tparam TSignature The callback signature.
  /// \tparam TCallback  The callback type, defaults to etl::delegate<TSignature>.
  //*************************************************************************
  template <typename TSignature, typename TCallback = etl::delegate<TSignature>>
  class closure;

#if ETL_USING_CPP11 && !defined(ETL_CLOSURE_FORCE_CPP03_IMPLEMENTATION)
  //*************************************************************************
  /// Closure for binding arguments to a callback and invoking it later.
  /// Stores a callback and its arguments, allowing deferred execution.
  /// Arguments are stored in a tuple and can be rebound using bind().
  /// Example usage:
  /// \code
  /// etl::closure<void(int, int)> c(my_delegate, 1, 2);
  /// c(); // Invokes my_delegate(1, 2)
  /// \endcode
  /// \tparam TReturn The return type of the callback.
  /// \tparam TArgs   The argument types of the callback.
  /// \tparam TCallback The callback type.
  //*************************************************************************
  template <typename TReturn, typename... TArgs, typename TCallback>
  class closure<TReturn(TArgs...), TCallback>
  {
  public:

    ETL_DEPRECATED using delegate_type = TCallback;                ///< The callback type to be invoked. Deprecated, use callback_type instead.
    using callback_type                = TCallback;                ///< The callback type to be invoked.
    using argument_types               = etl::type_list<TArgs...>; ///< The type list of arguments.

    //*********************************************************************
    /// Construct a closure with a callback and its arguments.
    /// \param f    The callback to be invoked.
    /// \param args The arguments to bind to the callback.
    //*********************************************************************
    ETL_CONSTEXPR14 closure(const callback_type& f, const TArgs... args)
      : m_f(f)
      , m_args(args...)
    {
    }

    //*********************************************************************
    /// Invoke the stored callback with the bound arguments.
    /// \return The result of the callback invocation.
    //*********************************************************************
    ETL_CONSTEXPR14 TReturn operator()() const
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
    template <size_t Index, typename UArg>
    ETL_CONSTEXPR14 void bind(UArg arg)
    {
      static_assert(etl::is_convertible< UArg, etl::type_list_type_at_index_t<argument_types, Index>>::value, "Argument is not convertible");
      static_assert(!etl::is_reference<UArg>::value, "Cannot bind reference arguments");

      etl::get<Index>(m_args) = arg;
    }

    //*********************************************************************
    /// Bind new values to multiple arguments at once.
    /// The number of arguments must match the tuple.
    /// \param args The new values to bind.
    ///*********************************************************************
    template <typename... UArgs>
    ETL_CONSTEXPR14 void bind(UArgs&&... args)
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
    ETL_CONSTEXPR14 void bind_impl(etl::index_sequence<Indexes...>, UArgs&&... args)
    {
      // Expand the pack and call bind<Index>(arg) for each argument
      int dummy[] = {0, (bind<Indexes>(etl::forward<UArgs>(args)), 0)...};
      (void)dummy; // Suppress unused variable warning
    }

    //*********************************************************************
    /// Execute the closure with the stored arguments.
    /// \tparam idx Index sequence for tuple unpacking.
    /// \return The result of the callback invocation.
    //*********************************************************************
    template <size_t... Indexes>
    ETL_CONSTEXPR14 TReturn execute(etl::index_sequence<Indexes...>) const
    {
      return m_f(etl::get<Indexes>(m_args)...);
    }

    callback_type        m_f;    ///< The callback to invoke.
    etl::tuple<TArgs...> m_args; ///< The bound arguments.
  };
#else
  //*************************************************************************
  /// Closure for binding one argument to a callback and invoking it later.
  /// \tparam TReturn The return type of the callback.
  /// \tparam TArg0    The type of the argument.
  /// \tparam TCallback The callback type.
  //*************************************************************************
  template <typename TReturn, typename TArg0, typename TCallback>
  class closure<TReturn(TArg0), TCallback>
  {
  public:

    /// The callback type to be invoked.
    typedef TCallback callback_type;

    //*********************************************************************
    /// Construct a closure with a callback and its argument.
    /// \param f    The callback to be invoked.
    /// \param arg0 The argument to bind to the callback.
    //*********************************************************************
    closure(const callback_type& f, const TArg0 arg0)
      : m_f(f)
      , m_arg0(arg0)
    {
    }

    //*********************************************************************
    /// Invoke the stored callback with the bound argument.
    /// \return The result of the callback invocation.
    //*********************************************************************
    TReturn operator()() const
    {
      return m_f(m_arg0);
    }

  private:

    callback_type m_f; ///< The callback to invoke.
    TArg0         m_arg0;
  };

  //*************************************************************************
  /// Closure for binding two arguments to a callback and invoking it later.
  /// \tparam TReturn The return type of the callback.
  /// \tparam TArg0    The type of the first argument.
  /// \tparam TArg1    The type of the second argument.
  /// \tparam TCallback The callback type.
  //*************************************************************************
  template <typename TReturn, typename TArg0, typename TArg1, typename TCallback>
  class closure<TReturn(TArg0, TArg1), TCallback>
  {
  public:

    typedef TCallback callback_type;

    //*********************************************************************
    /// Construct a closure with a callback and its arguments.
    /// \param f    The callback to be invoked.
    /// \param arg0 The first argument to bind.
    /// \param arg1 The second argument to bind.
    //*********************************************************************
    closure(const callback_type& f, const TArg0 arg0, const TArg1 arg1)
      : m_f(f)
      , m_arg0(arg0)
      , m_arg1(arg1)
    {
    }

    //*********************************************************************
    /// Invoke the stored callback with the bound arguments.
    /// \return The result of the callback invocation.
    //*********************************************************************
    TReturn operator()() const
    {
      return m_f(m_arg0, m_arg1);
    }

  private:

    callback_type m_f; ///< The callback to invoke.
    TArg0         m_arg0;
    TArg1         m_arg1;
  };

  //*************************************************************************
  /// Closure for binding three arguments to a callback and invoking it later.
  /// \tparam TReturn The return type of the callback.
  /// \tparam TArg0    The type of the first argument.
  /// \tparam TArg1    The type of the second argument.
  /// \tparam TArg2    The type of the third argument.
  /// \tparam TCallback The callback type.
  //*************************************************************************
  template <typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TCallback>
  class closure<TReturn(TArg0, TArg1, TArg2), TCallback>
  {
  public:

    typedef TCallback callback_type;

    //*********************************************************************
    /// Construct a closure with a callback and its arguments.
    /// \param f    The callback to be invoked.
    /// \param arg0 The first argument to bind.
    /// \param arg1 The second argument to bind.
    /// \param arg2 The third argument to bind.
    //*********************************************************************
    closure(const callback_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2)
      : m_f(f)
      , m_arg0(arg0)
      , m_arg1(arg1)
      , m_arg2(arg2)
    {
    }

    //*********************************************************************
    /// Invoke the stored callback with the bound arguments.
    /// \return The result of the callback invocation.
    //*********************************************************************
    TReturn operator()() const
    {
      return m_f(m_arg0, m_arg1, m_arg2);
    }

  private:

    callback_type m_f; ///< The callback to invoke.
    TArg0         m_arg0;
    TArg1         m_arg1;
    TArg2         m_arg2;
  };

  //*************************************************************************
  /// Closure for binding four arguments to a callback and invoking it later.
  /// \tparam TReturn The return type of the callback.
  /// \tparam TArg0    The type of the first argument.
  /// \tparam TArg1    The type of the second argument.
  /// \tparam TArg2    The type of the third argument.
  /// \tparam TArg3    The type of the fourth argument.
  /// \tparam TCallback The callback type.
  //*************************************************************************
  template <typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3, typename TCallback>
  class closure<TReturn(TArg0, TArg1, TArg2, TArg3), TCallback>
  {
  public:

    typedef TCallback callback_type;

    //*********************************************************************
    /// Construct a closure with a callback and its arguments.
    /// \param f    The callback to be invoked.
    /// \param arg0 The first argument to bind.
    /// \param arg1 The second argument to bind.
    /// \param arg2 The third argument to bind.
    /// \param arg3 The fourth argument to bind.
    //*********************************************************************
    closure(const callback_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2, const TArg3 arg3)
      : m_f(f)
      , m_arg0(arg0)
      , m_arg1(arg1)
      , m_arg2(arg2)
      , m_arg3(arg3)
    {
    }

    //*********************************************************************
    /// Invoke the stored callback with the bound arguments.
    /// \return The result of the callback invocation.
    //*********************************************************************
    TReturn operator()() const
    {
      return m_f(m_arg0, m_arg1, m_arg2, m_arg3);
    }

  private:

    callback_type m_f; ///< The callback to invoke.
    TArg0         m_arg0;
    TArg1         m_arg1;
    TArg2         m_arg2;
    TArg3         m_arg3;
  };

  //*************************************************************************
  /// Closure for binding five arguments to a callback and invoking it later.
  /// \tparam TReturn The return type of the callback.
  /// \tparam TArg0    The type of the first argument.
  /// \tparam TArg1    The type of the second argument.
  /// \tparam TArg2    The type of the third argument.
  /// \tparam TArg3    The type of the fourth argument.
  /// \tparam TArg4    The type of the fifth argument.
  /// \tparam TCallback The callback type.
  //*************************************************************************
  template <typename TReturn, typename TArg0, typename TArg1, typename TArg2, typename TArg3, typename TArg4, typename TCallback>
  class closure<TReturn(TArg0, TArg1, TArg2, TArg3, TArg4), TCallback>
  {
  public:

    typedef TCallback callback_type;

    //*********************************************************************
    /// Construct a closure with a callback and its arguments.
    /// \param f    The callback to be invoked.
    /// \param arg0 The first argument to bind.
    /// \param arg1 The second argument to bind.
    /// \param arg2 The third argument to bind.
    /// \param arg3 The fourth argument to bind.
    /// \param arg4 The fifth argument to bind.
    //*********************************************************************
    closure(const callback_type& f, const TArg0 arg0, const TArg1 arg1, const TArg2 arg2, const TArg3 arg3, const TArg4 arg4)
      : m_f(f)
      , m_arg0(arg0)
      , m_arg1(arg1)
      , m_arg2(arg2)
      , m_arg3(arg3)
      , m_arg4(arg4)
    {
    }

    //*********************************************************************
    /// Invoke the stored callback with the bound arguments.
    /// \return The result of the callback invocation.
    //*********************************************************************
    TReturn operator()() const
    {
      return m_f(m_arg0, m_arg1, m_arg2, m_arg3, m_arg4);
    }

  private:

    callback_type m_f; ///< The callback to invoke.
    TArg0         m_arg0;
    TArg1         m_arg1;
    TArg2         m_arg2;
    TArg3         m_arg3;
    TArg4         m_arg4;
  };
#endif
} // namespace etl

#endif
