#ifndef UNITTEST_EXTRA_CHECKMACROS_H 
#define UNITTEST_EXTRA_CHECKMACROS_H

#include "HelperMacros.h"
#include "ExceptionMacros.h"
#include "Checks.h"
#include "AssertException.h"
#include "MemoryOutStream.h"
#include "TestDetails.h"
#include "CurrentTest.h"
#include "ReportAssertImpl.h"

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
