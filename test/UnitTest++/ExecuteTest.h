#ifndef UNITTEST_EXECUTE_TEST_H
#define UNITTEST_EXECUTE_TEST_H

#include "Config.h"
#include "ExceptionMacros.h"
#include "TestDetails.h"
#include "TestResults.h"
#include "MemoryOutStream.h"
#include "AssertException.h"
#include "RequiredCheckException.h"
#include "CurrentTest.h"

#ifdef UNITTEST_NO_EXCEPTIONS
#include "ReportAssertImpl.h"
#endif

#ifdef UNITTEST_POSIX
#include "Posix/SignalTranslator.h"
#endif

namespace UnitTest {

   template< typename T >
   void ExecuteTest(T& testObject, TestDetails const& details, bool isMockTest)
   {
      if (isMockTest == false)
         CurrentTest::Details() = &details;

#ifdef UNITTEST_NO_EXCEPTIONS
      if (UNITTEST_SET_ASSERT_JUMP_TARGET() == 0)
      {
#endif
#ifndef UNITTEST_POSIX
      UNITTEST_IMPL_TRY({ testObject.RunImpl(); })
#else
      UNITTEST_IMPL_TRY
         ({
         UNITTEST_THROW_SIGNALS_POSIX_ONLY
         testObject.RunImpl();
      })
#endif
      UNITTEST_IMPL_CATCH(RequiredCheckException, e, { (void)e; })
      UNITTEST_IMPL_CATCH(AssertException, e, { (void)e; })
      UNITTEST_IMPL_CATCH(std::exception, e,
      {
         MemoryOutStream stream;
         stream << "Unhandled exception: " << e.what();
         CurrentTest::Results()->OnTestFailure(details, stream.GetText());
      })
      UNITTEST_IMPL_CATCH_ALL
         ({
         CurrentTest::Results()->OnTestFailure(details, "Unhandled exception: test crashed");
      })
#ifdef UNITTEST_NO_EXCEPTIONS
   }
#endif
   }

}

#endif
