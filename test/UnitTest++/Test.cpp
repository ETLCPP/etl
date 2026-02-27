#include "Test.h"
#include "AssertException.h"
#include "Config.h"
#include "ExecuteTest.h"
#include "MemoryOutStream.h"
#include "TestList.h"
#include "TestResults.h"

#ifdef UNITTEST_POSIX
  #include "Posix/SignalTranslator.h"
#endif

namespace UnitTest
{

  TestList& Test::GetTestList()
  {
    static TestList s_list;
    return s_list;
  }

  Test::Test(char const* testName, char const* suiteName, char const* filename, int lineNumber)
    : m_details(testName, suiteName, filename, lineNumber)
    , m_nextTest(0)
    , m_isMockTest(false)
  {
  }

  Test::~Test()
  {
  }

  void Test::Run()
  {
    ExecuteTest(*this, m_details, m_isMockTest);
  }

  void Test::RunImpl() const
  {
  }

} // namespace UnitTest
