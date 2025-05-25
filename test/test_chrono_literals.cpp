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

#include "etl/platform.h"

#include "unit_test_framework.h"

#include "etl/chrono.h"

#include <type_traits>

namespace
{
  SUITE(test_chrono_literals)
  {
    //*************************************************************************
    TEST(test_chrono_literals_in_chrono_namespace)
    {
      using namespace etl::chrono;

      CHECK_TRUE(etl::chrono::year(2025)         == 2025_year);
      CHECK_TRUE(etl::chrono::day(15)            == 15_day);
      CHECK_TRUE(etl::chrono::hours(23)          == 23_hours);
      CHECK_TRUE(etl::chrono::minutes(19)        == 19_minutes);
      CHECK_TRUE(etl::chrono::seconds(10)        == 10_seconds);
      CHECK_TRUE(etl::chrono::milliseconds(100)  == 100_milliseconds);
      CHECK_TRUE(etl::chrono::microseconds(1000) == 1000_microseconds);
      CHECK_TRUE(etl::chrono::nanoseconds(10000) == 10000_nanoseconds);
    }

    //*************************************************************************
    TEST(test_chrono_literals_in_literals_namespace)
    {
      using namespace etl::literals;

      CHECK_TRUE(etl::chrono::year(2025)         == 2025_year);
      CHECK_TRUE(etl::chrono::day(15)            == 15_day);
      CHECK_TRUE(etl::chrono::hours(23)          == 23_hours);
      CHECK_TRUE(etl::chrono::minutes(19)        == 19_minutes);
      CHECK_TRUE(etl::chrono::seconds(10)        == 10_seconds);
      CHECK_TRUE(etl::chrono::milliseconds(100)  == 100_milliseconds);
      CHECK_TRUE(etl::chrono::microseconds(1000) == 1000_microseconds);
      CHECK_TRUE(etl::chrono::nanoseconds(10000) == 10000_nanoseconds);
    }

    //*************************************************************************
    TEST(test_chrono_literals_in_chrono_literals_namespace)
    {
      using namespace etl::chrono_literals;

      CHECK_TRUE(etl::chrono::year(2025)         == 2025_year);
      CHECK_TRUE(etl::chrono::day(15)            == 15_day);
      CHECK_TRUE(etl::chrono::hours(23)          == 23_hours);
      CHECK_TRUE(etl::chrono::minutes(19)        == 19_minutes);
      CHECK_TRUE(etl::chrono::seconds(10)        == 10_seconds);
      CHECK_TRUE(etl::chrono::milliseconds(100)  == 100_milliseconds);
      CHECK_TRUE(etl::chrono::microseconds(1000) == 1000_microseconds);
      CHECK_TRUE(etl::chrono::nanoseconds(10000) == 10000_nanoseconds);
    }
  };
}
