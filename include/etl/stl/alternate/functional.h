
#ifndef ETL_STL_ALTERNATE_FUNCTIONAL_INCLUDED
#define ETL_STL_ALTERNATE_FUNCTIONAL_INCLUDED

#include "../../platform.h"

#if defined(ETL_IN_UNIT_TEST)
namespace etlstd
#else
namespace std
#endif
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
}

#endif