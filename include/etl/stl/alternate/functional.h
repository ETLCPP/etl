
#ifndef ETL_STL_ALTERNATE_FUNCTIONAL_INCLUDED
#define ETL_STL_ALTERNATE_FUNCTIONAL_INCLUDED

#include "../../platform.h"

namespace etlstd
{
  //***************************************************************************
  template <typename T = void>
  struct less
  {
    ETL_CONSTEXPR bool operator()(const T &lhs, const T &rhs) const
    {
      return lhs < rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct greater
  {
    ETL_CONSTEXPR bool operator()(const T &lhs, const T &rhs) const
    {
      return lhs > rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct equal_to
  {
    ETL_CONSTEXPR bool operator()(const T &lhs, const T &rhs) const
    {
      return lhs == rhs;
    }
  };

  //***************************************************************************
  template <typename T = void>
  struct not_equal_to
  {
    ETL_CONSTEXPR bool operator()(const T &lhs, const T &rhs) const
    {
      return lhs != rhs;
    }
  };

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

  template <typename TFunction>
  class binder1st : public etlstd::unary_function<typename TFunction::second_argument_type, typename TFunction::result_type>
  {
  protected:

    TFunction operation;
    typename TFunction::first_argument_type value;

  public:

    binder1st(const TFunction& f, const typename TFunction::first_argument_type& v)
      : operation (f), value(v)
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
  class binder2nd : public etlstd::unary_function<typename TFunction::first_argument_type, typename TFunction::result_type>
  {
  protected:
    TFunction operation;
    typename TFunction::second_argument_type value;
  public:
    binder2nd(const TFunction& f, const typename TFunction::second_argument_type& v)
      : operation (f), value(v)
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
}

#endif
