
#ifndef ETL_STL_FUNCTIONAL_INCLUDED
#define ETL_STL_FUNCTIONAL_INCLUDED

#include "../platform.h"
#include "choose_namespace.h"

#if defined(ETL_NO_STL)
  #include "alternate/functional.h"
#else
  #include <functional>
#endif

#endif
