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

#include <iostream>

// Set to 0 to reference against std::chrono
#define ETL_USING_ETL_CHRONO 1

#if ETL_USING_ETL_CHRONO
  #define Chrono etl::chrono
#else
  #if ETL_USING_CPP20
    #include <chrono>
    #define Chrono std::chrono
  #else
    #error std::chrono not supported
  #endif
#endif

namespace
{
  SUITE(test_chrono_weekday)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Chrono::weekday weekday;

      CHECK_FALSE(weekday.ok());
    }

    //*************************************************************************
    TEST(test_constructor_with_sys_days)
    {
      Chrono::weekday expected{Chrono::Tuesday};
      Chrono::weekday actual{Chrono::sys_days(Chrono::days(10988))};

      CHECK_EQUAL((unsigned)expected.c_encoding(), (unsigned)actual.c_encoding());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 0U; i < 7U; ++i)
      {
        Chrono::weekday weekday(i);

        CHECK_TRUE(weekday.ok());
        CHECK_EQUAL(i, weekday.c_encoding());
        CHECK_EQUAL((i == 0U) ? 7U : i, weekday.iso_encoding());
      }
    }

    //*************************************************************************
    TEST(test_constructor_out_of_range)
    {
      for (unsigned i = 8U; i < 256U; ++i)
      {
        Chrono::weekday weekday(i);

        CHECK_FALSE(weekday.ok());
        CHECK_EQUAL(i, weekday.c_encoding());
        CHECK_EQUAL((i == 0U) ? 7U : i, weekday.iso_encoding());
      }
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      Chrono::weekday weekday(0);
      unsigned count = 0;

      for (int i = 0; i < 255; ++i)
      {
        ++weekday;
        ++count;

        CHECK_TRUE(weekday.ok());
        CHECK_EQUAL(count % 7, weekday.c_encoding());
        CHECK_EQUAL((count % 7 == 0U) ? 7U : count % 7, weekday.iso_encoding());
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      Chrono::weekday weekday(0);
      unsigned count = 0;

      for (int i = 0; i < 255; ++i)
      {
        Chrono::weekday last_weekday = weekday++;
        unsigned last_count = count++;

        CHECK_TRUE(last_weekday.ok());
        CHECK_EQUAL(last_count % 7, last_weekday.c_encoding());
        CHECK_EQUAL((last_count % 7 == 0U) ? 7U : last_count % 7, last_weekday.iso_encoding());

        CHECK_TRUE(weekday.ok());
        CHECK_EQUAL(count % 7, weekday.c_encoding());
        CHECK_EQUAL((count % 7 == 0U) ? 7U : count % 7, weekday.iso_encoding());
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      Chrono::weekday weekday(255U);
      unsigned count = 255U;

      for (int i = 0; i < 255; ++i)
      {
        --weekday;
        --count;

        CHECK_TRUE(weekday.ok());
        CHECK_EQUAL(count % 7, weekday.c_encoding());
        CHECK_EQUAL((count % 7 == 0U) ? 7U : count % 7, weekday.iso_encoding());
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      Chrono::weekday weekday(255U);
      unsigned count = 255U;

      for (int i = 0; i < 255; ++i)
      {
        Chrono::weekday last_weekday = weekday--;
        unsigned last_count = count--;

        if (last_count == 255U)
        {
          CHECK_FALSE(last_weekday.ok());
          CHECK_EQUAL(255U, last_weekday.c_encoding());
          CHECK_EQUAL(255U, last_weekday.iso_encoding());
        }
        else
        {
          CHECK_TRUE(last_weekday.ok());
          CHECK_EQUAL(last_count % 7, last_weekday.c_encoding());
          CHECK_EQUAL((last_count % 7 == 0U) ? 7U : last_count % 7, last_weekday.iso_encoding());
        }

        CHECK_TRUE(weekday.ok());
        CHECK_EQUAL(count % 7, weekday.c_encoding());
        CHECK_EQUAL((count % 7 == 0U) ? 7U : count % 7, weekday.iso_encoding());
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_days)
    {
      for (unsigned wd = 0; wd <= 6; ++wd)
      {
        for (unsigned ds = 0; ds <= 14; ++ds)
        {
          Chrono::weekday weekday(wd);
          Chrono::days days(ds);
          weekday += days;

          unsigned expected = (wd + ds) % 7;

          CHECK_TRUE(weekday.ok());
          CHECK_EQUAL(expected, weekday.c_encoding());
          CHECK_EQUAL((expected == 0U) ? 7U : expected, weekday.iso_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_weekday_plus_days)
    {
      for (unsigned wd = 0; wd <= 6; ++wd)
      {
        for (unsigned ds = 0; ds <= 14; ++ds)
        {
          Chrono::weekday weekday(wd);
          Chrono::days days(ds);
          weekday = weekday + days;

          unsigned expected = (wd + ds) % 7;

          CHECK_TRUE(weekday.ok());
          CHECK_EQUAL(expected, weekday.c_encoding());
          CHECK_EQUAL((expected == 0U) ? 7U : expected, weekday.iso_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_days_plus_weekday)
    {
      for (unsigned wd = 0; wd <= 6; ++wd)
      {
        for (unsigned ds = 0; ds <= 14; ++ds)
        {
          Chrono::weekday weekday(wd);
          Chrono::days days(ds);
          weekday = weekday + days;

          unsigned expected = (ds + wd) % 7;

          CHECK_TRUE(weekday.ok());
          CHECK_EQUAL(expected, weekday.c_encoding());
          CHECK_EQUAL((expected == 0U) ? 7U : expected, weekday.iso_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_days)
    {
      for (unsigned wd = 0; wd <= 6; ++wd)
      {
        for (unsigned ds = 0; ds <= 14; ++ds)
        {
          Chrono::weekday weekday(wd);
          Chrono::days days(ds);
          weekday -= days;

          unsigned expected = ((wd + 7U) - (ds % 7U)) % 7U;

          CHECK_TRUE(weekday.ok());
          CHECK_EQUAL(expected, weekday.c_encoding());
          CHECK_EQUAL((expected == 0U) ? 7U : expected, weekday.iso_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_weekday_minus_days)
    {
      for (unsigned wd = 0; wd <= 6; ++wd)
      {
        for (unsigned ds = 0; ds <= 14; ++ds)
        {
          Chrono::weekday weekday(wd);
          Chrono::days days(ds);
          weekday = weekday - days;

          unsigned expected = ((wd + 7U) - (ds % 7U)) % 7U;

          CHECK_TRUE(weekday.ok());
          CHECK_EQUAL(expected, weekday.c_encoding());
          CHECK_EQUAL((expected == 0U) ? 7U : expected, weekday.iso_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_weekday_minus_weekday)
    {
      for (int m = 0; m < 7; ++m)
      {
        Chrono::weekday weekday1(m);
        Chrono::weekday weekday2(7 - m);

        Chrono::weekday std_weekday1(m);
        Chrono::weekday std_weekday2(7 - m);

        auto days12 = weekday1 - weekday2;
        auto days21 = weekday2 - weekday1;

        auto std_days12 = std_weekday1 - std_weekday2;
        auto std_days21 = std_weekday2 - std_weekday1;

        CHECK_EQUAL(std_days12.count(), days12.count());
        CHECK_EQUAL(std_days21.count(), days21.count());
      }
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_min_max_weekday)
    {
      CHECK_EQUAL(Chrono::Sunday.c_encoding(),   Chrono::weekday::min());
      CHECK_EQUAL(Chrono::Saturday.c_encoding(), Chrono::weekday::max());
    }
#endif

    //*************************************************************************
    TEST(test_weekday_constants)
    {
      CHECK_EQUAL(0U, Chrono::Sunday.c_encoding());
      CHECK_EQUAL(1U, Chrono::Monday.c_encoding());
      CHECK_EQUAL(2U, Chrono::Tuesday.c_encoding());
      CHECK_EQUAL(3U, Chrono::Wednesday.c_encoding());
      CHECK_EQUAL(4U, Chrono::Thursday.c_encoding());
      CHECK_EQUAL(5U, Chrono::Friday.c_encoding());
      CHECK_EQUAL(6U, Chrono::Saturday.c_encoding());

      CHECK_EQUAL(7U, Chrono::Sunday.iso_encoding());
      CHECK_EQUAL(1U, Chrono::Monday.iso_encoding());
      CHECK_EQUAL(2U, Chrono::Tuesday.iso_encoding());
      CHECK_EQUAL(3U, Chrono::Wednesday.iso_encoding());
      CHECK_EQUAL(4U, Chrono::Thursday.iso_encoding());
      CHECK_EQUAL(5U, Chrono::Friday.iso_encoding());
      CHECK_EQUAL(6U, Chrono::Saturday.iso_encoding());
    }

    //*************************************************************************
    TEST(test_weekday_comparison_operators)
    {
      Chrono::weekday weekday1(1);
      Chrono::weekday weekday2(2);

      CHECK_TRUE(weekday1  == weekday1);
      CHECK_FALSE(weekday1 != weekday1);
      CHECK_FALSE(weekday1 == weekday2);
      CHECK_TRUE(weekday1  != weekday2);
    }

    //*************************************************************************
    TEST(test_weekday_index_operator_returning_weekday_indexed)
    {
      Chrono::weekday wd(Chrono::Friday);
      Chrono::weekday_indexed wi = wd[2];

      CHECK_EQUAL(2, wi.index());
      CHECK_EQUAL(Chrono::Friday.c_encoding(), wi.weekday().c_encoding());
    }

    //*************************************************************************
    TEST(test_weekday_index_operator_returning_weekday_last)
    {
      Chrono::weekday wd(Chrono::Friday);
      Chrono::weekday_last wl = wd[Chrono::last];

      CHECK_EQUAL(Chrono::Friday.c_encoding(), wl.weekday().c_encoding());
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_weekday_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 256; ++i)
      {
        hashes.push_back(etl::hash<Chrono::weekday>()(Chrono::weekday(i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(256U, hashes.size());
    }
#endif
  };
}
