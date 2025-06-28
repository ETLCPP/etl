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
  template<typename>
  class closure;

  template <typename R, typename... Args>
  class closure<R(Args...)>
  {
  public:
    using fct = etl::delegate<R(Args...)>;
    using tuple_type = etl::tuple<Args...>;

    closure(fct const& f, const Args... args)
      : m_f(f), m_args(args...)
    {
    }

    R operator()() const
    {
      return execute(etl::index_sequence_for<Args...>{});
    }

    template <size_t index, typename Arg,
      typename = etl::enable_if_t<etl::is_same<Arg, etl::tuple_element_t<index, tuple_type>>::value && !etl::is_reference<Arg>::value>>
    void bind(Arg arg)
    {
      etl::get<index>(m_args) = arg;
    }

  private:
    template<size_t... idx>
    R execute(etl::index_sequence<idx...>) const
    {
      return m_f(etl::get<idx>(m_args)...);
    }

    fct m_f;
    tuple_type m_args;
  };
#else
  //***************************************************************************
  template<typename>
  class closure;

  template<typename R, typename Arg0>
  class closure<R(Arg0)>
  {
  public:
    using fct = etl::delegate<R(Arg0)>;

    closure(fct const& f, Arg0 const arg0)
      : m_f(f), m_arg0(arg0)
    {
    }

    R operator()() const {
      return m_f(m_arg0);
    }

  private:
    fct m_f;
    Arg0 m_arg0;
  };

  template<typename R, typename Arg0, typename Arg1>
  class closure<R(Arg0, Arg1)>
  {
  public:
    using fct = etl::delegate<R(Arg0, Arg1)>;

    closure(fct const& f, Arg0 const arg0, Arg1 const arg1)
      : m_f(f), m_arg0(arg0), m_arg1(arg1)
    {
    }

    R operator()() const {
      return m_f(m_arg0, m_arg1);
    }

  private:
    fct m_f;
    Arg0 m_arg0;
    Arg1 m_arg1;
  };

  template<typename R, typename Arg0, typename Arg1, typename Arg2>
  class closure<R(Arg0, Arg1, Arg2)>
  {
  public:
    using fct = etl::delegate<R(Arg0, Arg1, Arg2)>;

    closure(fct const& f, Arg0 const arg0, Arg1 const arg1, Arg2 const arg2)
      : m_f(f), m_arg0(arg0), m_arg1(arg1), m_arg2(arg2)
    {
    }

    R operator()() const {
      return m_f(m_arg0, m_arg1, m_arg2);
    }

  private:
    fct m_f;
    Arg0 m_arg0;
    Arg1 m_arg1;
    Arg2 m_arg2;
  };

  template<typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
  class closure<R(Arg0, Arg1, Arg2, Arg3)>
  {
  public:
    using fct = etl::delegate<R(Arg0, Arg1, Arg2, Arg3)>;

    closure(fct const& f, Arg0 const arg0, Arg1 const arg1, Arg2 const arg2, Arg3 const arg3)
      : m_f(f), m_arg0(arg0), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3)
    {
    }

    R operator()() const {
      return m_f(m_arg0, m_arg1, m_arg2, m_arg3);
    }

  private:
     fct m_f;
     Arg0 m_arg0;
     Arg1 m_arg1;
     Arg2 m_arg2;
     Arg3 m_arg3;
  };

  template<typename R, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
  class closure<R(Arg0, Arg1, Arg2, Arg3, Arg4)>
  {
  public:
    using fct = etl::delegate<R(Arg0, Arg1, Arg2, Arg3, Arg4)>;

    closure(fct const& f, Arg0 const arg0, Arg1 const arg1, Arg2 const arg2, Arg3 const arg3, Arg4 const arg4)
      : m_f(f), m_arg0(arg0), m_arg1(arg1), m_arg2(arg2), m_arg3(arg3), m_arg4(arg4)
    {
    }

    R operator()() const {
      return m_f(m_arg0, m_arg1, m_arg2, m_arg3, m_arg4);
    }

  private:
    fct m_f;
    Arg0 m_arg0;
    Arg1 m_arg1;
    Arg2 m_arg2;
    Arg3 m_arg3;
    Arg4 m_arg4;
  };
#endif
}

#endif