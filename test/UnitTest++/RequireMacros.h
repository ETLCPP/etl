#ifndef UNITTEST_REQUIREMACROS_H
#define UNITTEST_REQUIREMACROS_H

#include "RequiredCheckTestReporter.h"

#define UNITTEST_REQUIRE for(UnitTest::RequiredCheckTestReporter decoratedReporter(*UnitTest::CurrentTest::Results()); decoratedReporter.Next(); )

#ifndef UNITTEST_DISABLE_SHORT_MACROS
   #ifdef REQUIRE
      #error REQUIRE already defined, re-configure with UNITTEST_ENABLE_SHORT_MACROS set to 0 and use UNITTEST_REQUIRE instead
   #else
      #define REQUIRE UNITTEST_REQUIRE
   #endif
#endif

#endif
