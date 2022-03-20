#ifndef UNITTEST_THROWINGTESTREPORTER_H
#define UNITTEST_THROWINGTESTREPORTER_H

#include "TestReporter.h"

namespace UnitTest {

   // A TestReporter that throws when ReportFailure is called. Otherwise it
   // forwards the calls to a decorated TestReporter
   class ThrowingTestReporter : public TestReporter
   {
   public:
      explicit ThrowingTestReporter(TestReporter* reporter);

      virtual ~ThrowingTestReporter();
      virtual void ReportTestStart(TestDetails const& test);
      virtual void ReportFailure(TestDetails const& test, char const* failure);
      virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed);
      virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);

   private:
      TestReporter* m_decoratedReporter;
   };
}

#endif
