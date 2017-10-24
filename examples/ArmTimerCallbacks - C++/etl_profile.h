
#ifndef __ETL_PROFILE_H__
#define __ETL_PROFILE_H__

#define ETL_THROW_EXCEPTIONS
#define ETL_VERBOSE_ERRORS
#define ETL_CHECK_PUSH_POP
#define ETL_ISTRING_REPAIR_ENABLE
#define ETL_IVECTOR_REPAIR_ENABLE
#define ETL_IDEQUE_REPAIR_ENABLE
#define ETL_IN_UNIT_TEST

#if (__CC_ARM == 1)
  // ARM5 compiler
  #include "profiles/armv5.h"
#else
  // ARM6 compiler
  #include "profiles/armv6.h"
#endif

#endif
