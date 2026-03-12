/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

Copyright(c) 2014 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "unit_test_framework.h"

#include "UnitTest++/TestReporter.h"
#include "UnitTest++/TestReporterStdout.h"
#include "UnitTest++/TestRunner.h"
#include "UnitTest++/TestDetails.h"

#include <cstring>
#include <iostream>
#include <iomanip>

class VerboseTestReporter : public UnitTest::TestReporter
{
public:
  VerboseTestReporter() : m_testIndex(0), m_currentTestFailed(false) {}

  void ReportTestStart(UnitTest::TestDetails const& test) override
  {
    ++m_testIndex;
    m_currentTestFailed = false;
    std::cout << "[START #" << m_testIndex << "] "
              << test.suiteName << "::" << test.testName << std::endl;
    std::cout.flush();
    std::cerr.flush();
  }

  void ReportTestFinish(UnitTest::TestDetails const& test, float secondsElapsed) override
  {
    std::cout.flush();
    std::cerr.flush();
    std::cout << "[" << (m_currentTestFailed ? "FAILED" : "PASSED")
              << "  #" << m_testIndex << "] "
              << test.suiteName << "::" << test.testName
              << " (" << std::fixed << std::setprecision(4) << secondsElapsed << " s)"
              << std::endl;
  }

  void ReportFailure(UnitTest::TestDetails const& details, char const* failure) override
  {
    m_currentTestFailed = true;
#if defined(__APPLE__) || defined(__GNUG__)
    std::cerr << details.filename << ":" << details.lineNumber << ":1: error: Failure in "
              << details.testName << ": " << failure << std::endl;
#else
    std::cerr << details.filename << "(" << details.lineNumber << "): error: Failure in "
              << details.testName << ": " << failure << std::endl;
#endif
  }

  void ReportSummary(int totalTestCount, int failedTestCount,
                     int failureCount, float secondsElapsed) override
  {
    if (failureCount > 0)
      std::cout << "FAILURE: " << failedTestCount << " out of " << totalTestCount
                << " tests failed (" << failureCount << " failures)." << std::endl;
    else
      std::cout << "Success: " << totalTestCount << " tests passed." << std::endl;

    std::cout << "Test time: " << std::fixed << std::setprecision(2)
              << secondsElapsed << " seconds." << std::endl;
  }

private:
  int  m_testIndex;
  bool m_currentTestFailed;
};

int main(int argc, char* argv[])
{
  bool verbose = false;
  for (int i = 1; i < argc; ++i)
  {
    if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
      verbose = true;
  }

  if (verbose)
  {
    VerboseTestReporter reporter;
    UnitTest::TestRunner runner(reporter);
    return runner.RunTestsIf(UnitTest::Test::GetTestList(), NULL, UnitTest::True(), 0);
  }
  else
  {
    return UnitTest::RunAllTests();
  }
}
