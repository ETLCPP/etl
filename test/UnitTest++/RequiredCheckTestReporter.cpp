#include "RequiredCheckTestReporter.h"

#include "CurrentTest.h"
#include "TestResults.h"

namespace UnitTest {

   RequiredCheckTestReporter::RequiredCheckTestReporter(TestResults& results)
      : m_results(results)
      , m_originalTestReporter(results.m_testReporter)
      , m_throwingReporter(results.m_testReporter)
      , m_continue(0)
   {
      m_results.m_testReporter = &m_throwingReporter;
   }

   RequiredCheckTestReporter::~RequiredCheckTestReporter()
   {
      m_results.m_testReporter = m_originalTestReporter;
   }

   bool RequiredCheckTestReporter::Next()
   {
      return m_continue++ == 0;
   }
}
