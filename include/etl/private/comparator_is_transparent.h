

#ifndef ETL_COMPARATOR_IS_TRANSPARENT
#define ETL_COMPARATOR_IS_TRANSPARENT

#include "../type_traits.h"

namespace etl
{
#if ETL_CPP11_SUPPORTED
  template <typename... T>
  using void_t = void;

  template <typename T, typename = void>
  struct comparator_is_transparent : etl::false_type
  {
  };

  template <typename T>
  struct comparator_is_transparent<T, void_t<typename T::is_transparent>> : etl::true_type
  {
  };
#endif
}

#endif
