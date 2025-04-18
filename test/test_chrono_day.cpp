/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation: 

Copyright(c) 2023 John Wellbelove

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

#include <vector>
#include <algorithm>

namespace
{
  //*************************************************************************
  bool is_day_ok(etl::chrono::day day)
  {
    unsigned d = unsigned(day);

    if ((d < 1) || (d > 31))
    {
      return (day.ok() == false);
    }
    else
    { 
      return (day.ok() == true);
    }
  }

  SUITE(test_chrono_day)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::chrono::day day;

      CHECK_FALSE(day.ok());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned expected = 1U; expected < 31; ++expected)
      {
        etl::chrono::day day(expected);

        CHECK_TRUE(is_day_ok(day));
        CHECK_EQUAL(expected, unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      etl::chrono::day day(0);

      for (int expected = 0; expected < 256; ++expected, ++day)
      {
        CHECK_TRUE(is_day_ok(day));       
        CHECK_EQUAL(expected, unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      etl::chrono::day day(0);

      for (int expected = 0; expected < 256; expected++, day++)
      {
        CHECK_TRUE(is_day_ok(day));
        CHECK_EQUAL(expected, unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      etl::chrono::day day(255);

      for (int expected = 255; expected > 0; --expected, --day)
      {
        CHECK_TRUE(is_day_ok(day));
        CHECK_EQUAL(expected, unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      etl::chrono::day day(255);

      for (int expected = 255; expected > 0; expected--, day--)
      {
        CHECK_TRUE(is_day_ok(day));
        CHECK_EQUAL(expected, unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_days)
    {
      etl::chrono::day day(0);
      etl::chrono::days days(2);

      for (int expected = 2; expected < 256; expected += 2)
      {
        day += days;

        CHECK_TRUE(is_day_ok(day));
        CHECK_EQUAL(expected, unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_day_plus_days)
    {
      for (int d = 0; d < 256; ++d)
      {
        for (int ds = 0; ds < 256; ++ds)
        {
          etl::chrono::day day(d);
          etl::chrono::days days(ds);

          day = day + days;

          unsigned expected = (d + ds) % 256;

          CHECK_TRUE(is_day_ok(day));
          CHECK_EQUAL(expected, unsigned(day));
        }
      }
    }

    //*************************************************************************
    TEST(test_days_plus_day)
    {
      for (int d = 0; d < 256; ++d)
      {
        for (int ds = 0; ds < d; ++ds)
        {
          etl::chrono::day day(d);
          etl::chrono::days days(ds);

          day = days + day;

          unsigned expected = (d + ds) % 256;

          CHECK_TRUE(is_day_ok(day));
          CHECK_EQUAL(expected, unsigned(day));
        }
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_days)
    {
      etl::chrono::day day(255);
      etl::chrono::days days(2);

      for (int expected = 253; expected > 0; expected -= 2)
      {
        day -= days;

        CHECK_TRUE(is_day_ok(day));
        CHECK_EQUAL(expected, unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_day_minus_days)
    {
      etl::chrono::day day(255);
      etl::chrono::days days(2);

      for (int expected = 253; expected > 0; expected -= 2)
      {
        day = day - days;

        CHECK_TRUE(is_day_ok(day));
        CHECK_EQUAL(expected, unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_day_minus_day)
    {
      for (int d1 = 0; d1 < 256; ++d1)
      {
        for (int d2 = 0; d2 < 256; ++d2)
        {
          etl::chrono::day day1(d1);
          etl::chrono::day day2(d2);

          etl::chrono::days result_days = day1 - day2;
          int expected_days = d1 - d2;
          CHECK_EQUAL(expected_days, result_days.count());
        }
      }
    }

    //*************************************************************************
    TEST(test_min_max_day)
    {
      CHECK_EQUAL(1U,  etl::chrono::day::min());
      CHECK_EQUAL(31U, etl::chrono::day::max());
    }

    //*************************************************************************
    TEST(test_literal_day)
    {
      using namespace etl::literals::chrono_literals;

      etl::chrono::day day = 25_day;

      CHECK_TRUE(day.ok());
      CHECK_EQUAL(25, unsigned(day));
    }

    //*************************************************************************
    TEST(test_day_comparison_operators)
    {
        etl::chrono::day day10(10);
        etl::chrono::day day20(20);

        CHECK_TRUE(day10  == day10);
        CHECK_FALSE(day10 != day10);
        CHECK_TRUE(day10   < day20);
        CHECK_FALSE(day10  < day10);
        CHECK_FALSE(day20  < day10);
        CHECK_TRUE(day10  <= day20);
        CHECK_TRUE(day10  <= day10);
        CHECK_FALSE(day20 <= day10);
        CHECK_FALSE(day10  > day20);
        CHECK_FALSE(day10  > day10);
        CHECK_TRUE(day20   > day10);
        CHECK_FALSE(day10 >= day20);
        CHECK_TRUE(day10  >= day10);
        CHECK_TRUE(day20  >= day10);

#if ETL_USING_CPP20
        CHECK_TRUE((day10 <=> day10) == 0);
        CHECK_TRUE((day10 <=> day20)  < 0);
        CHECK_TRUE((day20 <=> day10)  > 0);
#endif
    }

    //*************************************************************************
    TEST(test_day_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 256; ++i)
      {
        hashes.push_back(etl::hash<etl::chrono::day>()(etl::chrono::day(i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(256U, hashes.size());
    }
  };
}
