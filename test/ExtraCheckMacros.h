#ifndef UNITTEST_EXTRA_CHECKMACROS_H 
#define UNITTEST_EXTRA_CHECKMACROS_H

#include "../unittest-cpp/UnitTest++/HelperMacros.h"
#include "../unittest-cpp/UnitTest++/ExceptionMacros.h"
#include "../unittest-cpp/UnitTest++/Checks.h"
#include "../unittest-cpp/UnitTest++/AssertException.h"
#include "../unittest-cpp/UnitTest++/MemoryOutStream.h"
#include "../unittest-cpp/UnitTest++/TestDetails.h"
#include "../unittest-cpp/UnitTest++/CurrentTest.h"
#include "../unittest-cpp/UnitTest++/ReportAssertImpl.h"

#ifndef CHECK_NO_THROW
#define CHECK_NO_THROW(expression) \
  UNITTEST_MULTILINE_MACRO_BEGIN \
  bool caught_ = false; \
  try { expression; } \
  catch (...) { caught_ = true; } \
if (caught_) \
  UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), "Unexpected exception thrown"); \
  UNITTEST_MULTILINE_MACRO_END
#endif
#endif
