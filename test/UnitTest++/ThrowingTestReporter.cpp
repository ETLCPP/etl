#include "ThrowingTestReporter.h"
#include "RequiredCheckException.h"

#ifdef UNITTEST_NO_EXCEPTIONS
#include "ReportAssertImpl.h"
#endif 

namespace UnitTest {

   ThrowingTestReporter::ThrowingTestReporter(TestReporter* decoratedReporter)
      : m_decoratedReporter(decoratedReporter)
   {}

   //virtual
   ThrowingTestReporter::~ThrowingTestReporter()
   {}

   //virtual
   void ThrowingTestReporter::ReportTestStart(TestDetails const& test)
   {
      if(m_decoratedReporter)
      {
         m_decoratedReporter->ReportTestStart(test);
      }
   }

   //virtual
   void ThrowingTestReporter::ReportFailure(TestDetails const& test, char const* failure)
   {
      if(m_decoratedReporter)
      {
         m_decoratedReporter->ReportFailure(test, failure);
      }
      
      #ifndef UNITTEST_NO_EXCEPTIONS
         throw RequiredCheckException();
      #else
         static const int stopTest = 1;
         UNITTEST_LONGJMP(*UnitTest::Detail::GetAssertJmpBuf(), stopTest);
      #endif
   }

   //virtual
   void ThrowingTestReporter::ReportTestFinish(TestDetails const& test, float secondsElapsed)
   {
      if(m_decoratedReporter)
      {
         m_decoratedReporter->ReportTestFinish(test, secondsElapsed);
      }
   }

   //virtual
   void ThrowingTestReporter::ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed)
   {
      if(m_decoratedReporter)
      {
         m_decoratedReporter->ReportSummary(totalTestCount, failedTestCount, failureCount, secondsElapsed);
      }
   }

}
