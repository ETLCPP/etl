/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/std
https://www.etlcpp.com

Copyright(c) 2022 jwellbelove

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

#include "etl/error_handler.h"

#include <stdio.h>
#include <iostream>

//*****************************************************************************
struct ErrorLog
{
    ErrorLog()
        : log_count(0)
    {
    }

    void Log(const etl::exception& /*e*/)
    {
        ++log_count;
    }

    int log_count;
};

int exception_count = 0;
int return_count = 0;

//*****************************************************************************
class test_exception : public etl::exception
{
public:

  test_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
    : exception(reason_, file_name_, line_number_)
  {
  }
};

//*****************************************************************************
class test_exception_1 : public test_exception
{
public:

  test_exception_1(string_type file_name_, numeric_type line_number_)
    : test_exception(ETL_ERROR_TEXT("Test Exception 1", "1A"), file_name_, line_number_)
  {
  }
};

//*****************************************************************************
void Assert(bool state)
{
  ETL_ASSERT(state, ETL_ERROR(test_exception_1));
}

//*****************************************************************************
void AssertFail()
{
  ETL_ASSERT_FAIL(ETL_ERROR(test_exception_1));
}

//*****************************************************************************
void AssertAndReturn(bool state)
{
  ETL_ASSERT_OR_RETURN(state, ETL_ERROR(test_exception_1));

  ++return_count;
}

//*****************************************************************************
void AssertFailAndReturn()
{
  ETL_ASSERT_FAIL_AND_RETURN(ETL_ERROR(test_exception_1));

  ++return_count;
}

//*****************************************************************************
bool AssertAndReturnValue(bool state)
{
  ETL_ASSERT_OR_RETURN_VALUE(state, ETL_ERROR(test_exception_1), true);

  ++return_count;
  return false;
}

//*****************************************************************************
bool AssertFailAndReturnValue()
{
  ETL_ASSERT_FAIL_AND_RETURN_VALUE(ETL_ERROR(test_exception_1), true);

  ++return_count;
  return false;
}

//*****************************************************************************
int main()
{
  ErrorLog error_log;

  etl::error_handler::set_callback<ErrorLog, &ErrorLog::Log>(error_log);

  try
  {
    Assert(false);
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  try
  {
    Assert(true);
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  try
  {
    AssertFail();
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  try
  {
    AssertAndReturn(false);
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  try
  {
    AssertAndReturn(true);
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  try
  {
    AssertFailAndReturn();
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  try
  {
    AssertAndReturnValue(false);
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  try
  {
    AssertAndReturnValue(true);
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  try
  {
    AssertFailAndReturnValue();
  }
  catch (test_exception_1 e)
  {
    ++exception_count;
  }

  bool log_count_passed = (error_log.log_count == 6);

  if (log_count_passed)
  {
    std::cout << "Log Count Passed\n";
  }
  else
  {
    std::cout << "Log Count Failed\n";
  }

  bool return_count_passed = (return_count == 2);

  if (return_count_passed)
  {
    std::cout << "Return Count Passed\n";
  }
  else
  {
    std::cout << "Return Count Failed\n";
  }

  bool exception_count_passed = (exception_count == 6);

  if (exception_count_passed)
  {
    std::cout << "Exception Count Passed\n";
  }
  else
  {
    std::cout << "Exception Count Failed\n";
  }

  return (log_count_passed && return_count_passed && exception_count_passed) ? 0 : 1;
}

