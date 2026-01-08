/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation: 

Copyright(c) 2025 John Wellbelove

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

#include "etl/platform.h"
#include "etl/error_handler.h"
#include "etl/exception.h"

namespace
{
  class TestException1 : public etl::exception
  {
  public:

    TestException1(string_type file_name_, numeric_type line_number_)
      : exception(ETL_ERROR_TEXT("Test:error1", "1A"), file_name_, line_number_)
    {
    }
  };

  SUITE(test_etl_asserts)
  {
    TEST(test_etl_assert)
    {
      auto te = ETL_ERROR(TestException1);

      CHECK_EQUAL(0, strcmp(te.what(), "Test:error1"));

      CHECK_THROW(ETL_ASSERT(false, ETL_ERROR(TestException1)), TestException1);
      CHECK_NO_THROW(ETL_ASSERT(true, ETL_ERROR(TestException1)));
    }

    TEST(test_etl_assert_generic)
    {
      auto te = ETL_ERROR_GENERIC("Generic Assert Exception");

      CHECK_EQUAL(0, strcmp(te.what(), "Generic Assert Exception"));

      CHECK_THROW(ETL_ASSERT(false, ETL_ERROR_GENERIC("Generic Assert Exception")), etl::exception);
      CHECK_NO_THROW(ETL_ASSERT(true, ETL_ERROR_GENERIC("Generic Assert Exception")));
    }
  }
}
