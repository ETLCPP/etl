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

#ifndef ETL_FUNCTIONAL_INCLUDED
#define ETL_FUNCTIONAL_INCLUDED

#include "platform.h"

///\defgroup functional functional
///\ingroup utilities

///\defgroup reference_wrapper reference_wrapper
///\ingroup functional

namespace etl
{
  //***************************************************************************
  /// A definition of reference_wrapper for those that don't have C++ 0x11 support.
  ///\ingroup reference
  //***************************************************************************
  template <typename T>
  class reference_wrapper
  {
  public:

    typedef T type;

    ETL_CONSTEXPR20 explicit reference_wrapper(T& t_) ETL_NOEXCEPT
      : t(&t_)
    {
    }

    ETL_CONSTEXPR20 reference_wrapper<T>& operator = (const reference_wrapper& rhs) ETL_NOEXCEPT
    {
      t = rhs.t;
      return *this;
    }

    ETL_CONSTEXPR20 T& get() const ETL_NOEXCEPT
    {
      return *t;
    }

    ETL_CONSTEXPR20 operator T&() const ETL_NOEXCEPT
    {
      return *t;
    }

  private:

    T* t;
  };

  //***************************************************************************
  template <typename T>
  reference_wrapper<T> ref(T& t)
  {
    return reference_wrapper<T>(t);
  }

  //***************************************************************************
  template <typename T>
  reference_wrapper<T> ref(reference_wrapper<T> t)
  {
    return reference_wrapper<T>(t.get());
  }

  //***************************************************************************
  template <typename T>
  reference_wrapper<const T> cref(const T& t)
  {
    return reference_wrapper<const T>(t);
  }

  //***************************************************************************
  template <typename T>
  reference_wrapper<const T> cref(reference_wrapper<T> t)
  {
    return reference_wrapper<const T>(t.get());
  }

  //***************************************************************************
  template <typename TArgumentType, typename TResultType>
  struct unary_function
  {
    typedef TArgumentType argument_type;
    typedef TResultType   result_type;
  };

  //***************************************************************************
  template <typename TFirstArgumentType, typename TSecondArgumentType, typename TResultType>
  struct binary_function
  {
    typedef TFirstArgumentType  first_argument_type;
    typedef TSecondArgumentType second_argument_type;
    typedef TResultType         result_type;
  };

  //***************************************************************************
  template <typename T = void>
  struct less : public etl::binary_function<T, T, bool>
  {
    typedef T value_type;

    ETL_CONSTEXPR bool operator()(const T &lhs, const T &rhs) const
    {
      return (lhs < rhs);
    }
  };

#if __cpp_rvalue_references
  template <>
  struct less<void> : public etl::binary_function<void, void, bool>
  {
    typedef int is_transparent;

    template <typename T1, typename T2>
    constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs))
    {
      return static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs);
    }
  };
#endif

  //***************************************************************************
  template <typename T = void>
  struct less_equal : public etl::binary_function<T, T, bool>
  {
    typedef T value_type;

    ETL_CONSTEXPR bool operator()(const T& lhs, const T& rhs) const
    {
      return !(rhs < lhs);
    }
  };

#if __cpp_rvalue_references
  template <>
  struct less_equal<void> : public etl::binary_function<void, void, bool>
  {
    typedef int is_transparent;

    template <typename T1, typename T2>
    constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs))
    {
      return !(static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs));
    }
  };
#endif

  //***************************************************************************
  template <typename T = void>
  struct greater : public etl::binary_function<T, T, bool>
  {
    typedef T value_type;

    ETL_CONSTEXPR bool operator()(const T &lhs, const T &rhs) const
    {
      return (rhs < lhs);
    }
  };

#if __cpp_rvalue_references
  template <>
  struct greater<void> : public etl::binary_function<void, void, bool>
  {
    typedef int is_transparent;

    template <typename T1, typename T2>
    constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs))
    {
      return static_cast<T1&&>(rhs) < static_cast<T2&&>(lhs);
    }
  };
#endif

  //***************************************************************************
  template <typename T = void>
  struct greater_equal : public etl::binary_function<T, T, bool>
  {
    typedef T value_type;

    ETL_CONSTEXPR bool operator()(const T& lhs, const T& rhs) const
    {
      return !(lhs < rhs);
    }
  };

#if __cpp_rvalue_references
  template <>
  struct greater_equal<void> : public etl::binary_function<void, void, bool>
  {
    typedef int is_transparent;

    template <typename T1, typename T2>
    constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs))
    {
      return static_cast<T1&&>(rhs) < static_cast<T2&&>(lhs);
    }
  };
#endif

  //***************************************************************************
  template <typename T = void>
  struct equal_to : public etl::binary_function<T, T, bool>
  {
    typedef T value_type;

    ETL_CONSTEXPR bool operator()(const T &lhs, const T &rhs) const
    {
      return lhs == rhs;
    }
  };

#if __cpp_rvalue_references
  template <>
  struct equal_to<void> : public etl::binary_function<void, void, bool>
  {
    typedef void value_type;
    typedef int is_transparent;

    template <typename T1, typename T2>
    constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs))
    {
      return static_cast<T1&&>(lhs) == static_cast<T2&&>(rhs);
    }
  };
#endif

  //***************************************************************************
  template <typename T = void>
  struct not_equal_to : public etl::binary_function<T, T, bool>
  {
    typedef T value_type;

    ETL_CONSTEXPR bool operator()(const T &lhs, const T &rhs) const
    {
      return !(lhs == rhs);
    }
  };

#if __cpp_rvalue_references
  template <>
  struct not_equal_to<void> : public etl::binary_function<void, void, bool>
  {
    typedef int is_transparent;

    template <typename T1, typename T2>
    constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(static_cast<T1&&>(lhs) < static_cast<T2&&>(rhs))
    {
      return !(static_cast<T1&&>(lhs) == static_cast<T2&&>(rhs));
    }
  };
#endif

  //***************************************************************************
  template <typename TFunction>
  class binder1st : public etl::unary_function<typename TFunction::second_argument_type, typename TFunction::result_type>
  {
  protected:

    TFunction operation;
    typename TFunction::first_argument_type value;

  public:

    binder1st(const TFunction& f, const typename TFunction::first_argument_type& v)
      : operation(f), value(v)
    {
    }

    typename TFunction::result_type operator()(typename TFunction::second_argument_type& x) const
    {
      return operation(value, x);
    }

    typename TFunction::result_type operator()(const typename TFunction::second_argument_type& x) const
    {
      return operation(value, x);
    }
  };

  template <typename F, typename T>
  binder1st<F> bind1st(const F& f, const T& x)
  {
    return binder1st<F>(f, x);
  }

  //***************************************************************************
  template <typename TFunction >
  class binder2nd : public etl::unary_function<typename TFunction::first_argument_type, typename TFunction::result_type>
  {
  protected:
    TFunction operation;
    typename TFunction::second_argument_type value;
  public:
    binder2nd(const TFunction& f, const typename TFunction::second_argument_type& v)
      : operation(f), value(v)
    {
    }

    typename TFunction::result_type operator()(typename TFunction::first_argument_type& x) const
    {
      return operation(x, value);
    }

    typename TFunction::result_type operator()(const typename TFunction::first_argument_type& x) const
    {
      return operation(x, value);
    }
  };

  template <typename F, typename T>
  binder2nd<F> bind2nd(const F& f, const T& x)
  {
    return binder2nd<F>(f, x);
  }

  //***************************************************************************
  template <typename T = void>
  struct plus
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs + rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct minus
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs - rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct negate
  {
    typedef T argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs) const
    {
      return -lhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct multiplies
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs * rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct divides
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs / rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct modulus
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs % rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct logical_and
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs && rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct logical_or
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs || rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct logical_not
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs) const
    {
      return !lhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct bit_and
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs & rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct bit_or
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs | rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct bit_xor
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs, const T& rhs) const
    {
      return lhs ^ rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct bit_not
  {
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;

    ETL_CONSTEXPR T operator()(const T& lhs) const
    {
      return ~lhs;
    }
  };
}

#endif

