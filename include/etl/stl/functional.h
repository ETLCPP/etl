
#ifndef ETL_STL_FUNCTIONAL_INCLUDED
#define ETL_STL_FUNCTIONAL_INCLUDED

#include "../platform.h"

#include "../private/choose_namespace.h"
#include "../private/choose_tag_types.h"
#include "../private/choose_pair_types.h"

#if defined(ETL_NO_STL)
  #include "alternate/functional.h"
#else
  #include <functional>
#endif

#endif
