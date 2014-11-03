#ifndef UNITTEST_EXTRA_CHECKMACROS_H 
#define UNITTEST_EXTRA_CHECKMACROS_H

#include <UnitTest++/HelperMacros.h>
#include <UnitTest++/ExceptionMacros.h>
#include <UnitTest++/Checks.h>
#include <UnitTest++/AssertException.h>
#include <UnitTest++/MemoryOutStream.h>
#include <UnitTest++/TestDetails.h>
#include <UnitTest++/CurrentTest.h>
#include <UnitTest++/ReportAssertImpl.h>

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
