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

#if ETL_USING_CPP20

#include "unit_test_framework.h"

#include "etl/chrono.h"

#include <chrono>
#include <array>
#include <algorithm>

namespace
{
  SUITE(test_chrono_weekday)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      std::chrono::weekday std_weekday;
      etl::chrono::weekday weekday;

      CHECK_EQUAL(std_weekday.ok(), weekday.ok());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 0U; i < 256U; ++i)
      {
        std::chrono::weekday std_weekday(i);
        etl::chrono::weekday weekday(i);

        CHECK_EQUAL(std_weekday.ok(), weekday.ok());
        CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
      }
    }

    //*************************************************************************
    TEST(test_encodings)
    {
      std::chrono::weekday std_weekday;
      etl::chrono::weekday weekday;

      for (unsigned i = 0U; i < 256; ++i)
      {
        std_weekday = std::chrono::weekday(i);
        weekday     = etl::chrono::weekday(i);

        CHECK_EQUAL(std_weekday.c_encoding(),   weekday.c_encoding());
        CHECK_EQUAL(std_weekday.iso_encoding(), weekday.iso_encoding());
      }
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      std::chrono::weekday std_weekday(0);
      etl::chrono::weekday weekday(0);

      for (int i = 0; i < 255; ++i)
      {
        ++std_weekday;
        ++weekday;

        CHECK_EQUAL(std_weekday.ok(), weekday.ok());
        CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      std::chrono::weekday std_weekday(0);
      etl::chrono::weekday weekday(0);

      for (int i = 0; i < 256; ++i)
      {
        std::chrono::weekday std_last_weekday = std_weekday++;
        etl::chrono::weekday last_weekday     = weekday++;

        CHECK_EQUAL(std_last_weekday.ok(), last_weekday.ok());
        CHECK_EQUAL(std_last_weekday.c_encoding(), last_weekday.c_encoding());

        CHECK_EQUAL(std_weekday.ok(), weekday.ok());
        CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      std::chrono::weekday std_weekday(255);
      etl::chrono::weekday weekday(255);

      for (int i = 0; i < 256; ++i)
      {
        --std_weekday;
        --weekday;

        CHECK_EQUAL(std_weekday.ok(), weekday.ok());
        CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      std::chrono::weekday std_weekday(255);
      etl::chrono::weekday weekday(255);

      for (int i = 0; i < 256; ++i)
      {
        std::chrono::weekday std_last_weekday = std_weekday--;
        etl::chrono::weekday last_weekday = weekday--;

        CHECK_EQUAL(std_last_weekday.ok(), last_weekday.ok());
        CHECK_EQUAL(std_last_weekday.c_encoding(), last_weekday.c_encoding());

        CHECK_EQUAL(std_weekday.ok(), weekday.ok());
        CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_days)
    {
      for (int wd = 0; wd <= 6; ++wd)
      {
        for (int ds = 0; ds <= 14; ++ds)
        {
          std::chrono::weekday std_weekday(wd);
          etl::chrono::weekday weekday(wd);

          std::chrono::days std_days(ds);
          etl::chrono::days days(ds);

          std_weekday += std_days;
          weekday     += days;

          CHECK_EQUAL(std_weekday.ok(), weekday.ok());
          CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_weekday_plus_days)
    {
      for (int wd = 0; wd <= 6; ++wd)
      {
        for (int ds = 0; ds <= 14; ++ds)
        {
          std::chrono::weekday std_weekday(wd);
          etl::chrono::weekday weekday(wd);

          std::chrono::days std_days(ds);
          etl::chrono::days days(ds);

          std_weekday = std_weekday + std_days;
          weekday     = weekday + days;

          CHECK_EQUAL(std_weekday.ok(), weekday.ok());
          CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_days_plus_weekday)
    {
      for (int wd = 0; wd <= 6; ++wd)
      {
        for (int ds = 0; ds <= 14; ++ds)
        {
          std::chrono::weekday std_weekday(wd);
          etl::chrono::weekday weekday(wd);

          std::chrono::days std_days(ds);
          etl::chrono::days days(ds);

          std_weekday = std_days + std_weekday;
          weekday     = days + weekday;

          CHECK_EQUAL(std_weekday.ok(), weekday.ok());
          CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_days)
    {
      for (int wd = 0; wd <= 6; ++wd)
      {
        for (int ds = 0; ds <= 14; ++ds)
        {
          std::chrono::weekday std_weekday(wd);
          etl::chrono::weekday weekday(wd);

          std::chrono::days std_days(ds);
          etl::chrono::days days(ds);

          std_weekday -= std_days;
          weekday     -= days;

          CHECK_EQUAL(std_weekday.ok(), weekday.ok());
          CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_weekday_minus_days)
    {
      for (int wd = 0; wd <= 6; ++wd)
      {
        for (int ds = 0; ds <= 14; ++ds)
        {
          std::chrono::weekday std_weekday(wd);
          etl::chrono::weekday weekday(wd);

          std::chrono::days std_days(ds);
          etl::chrono::days days(ds);

          std_weekday = std_weekday - std_days;
          weekday     = weekday - days;

          CHECK_EQUAL(std_weekday.ok(), weekday.ok());
          CHECK_EQUAL(std_weekday.c_encoding(), weekday.c_encoding());
        }
      }
    }

    //*************************************************************************
    TEST(test_weekday_minus_weekday)
    {
      for (int m = 0; m < 256; ++m)
      {
        std::chrono::weekday std_weekday1(m);
        std::chrono::weekday std_weekday2(255 - m);

        std::chrono::weekday weekday1(m);
        std::chrono::weekday weekday2(255 - m);

        auto std_days12 = std_weekday1 - std_weekday2;
        auto std_days21 = std_weekday2 - std_weekday1;

        auto days12 = weekday1 - weekday2;
        auto days21 = weekday2 - weekday1;

        CHECK_EQUAL(std_days12.count(), days12.count());
        CHECK_EQUAL(std_days21.count(), days21.count());
      }
    }

    //*************************************************************************
    TEST(test_min_max_weekday)
    {
      CHECK_EQUAL(etl::chrono::Sunday.c_encoding(),   etl::chrono::weekday::min().c_encoding());
      CHECK_EQUAL(etl::chrono::Saturday.c_encoding(), etl::chrono::weekday::max().c_encoding());
    }

    //*************************************************************************
    TEST(test_literal_weekday)
    {
      using namespace etl::literals::chrono_literals;

      etl::chrono::weekday weekday0 = 0_weekday;
      etl::chrono::weekday weekday1 = 1_weekday;
      etl::chrono::weekday weekday2 = 2_weekday;
      etl::chrono::weekday weekday3 = 3_weekday;
      etl::chrono::weekday weekday4 = 4_weekday;
      etl::chrono::weekday weekday5 = 5_weekday;
      etl::chrono::weekday weekday6 = 6_weekday;

      CHECK_TRUE(weekday0.ok());
      CHECK_TRUE(weekday1.ok());
      CHECK_TRUE(weekday2.ok());
      CHECK_TRUE(weekday3.ok());
      CHECK_TRUE(weekday4.ok());
      CHECK_TRUE(weekday5.ok());
      CHECK_TRUE(weekday6.ok());

      CHECK_EQUAL(0U, weekday0.c_encoding());
      CHECK_EQUAL(1U, weekday1.c_encoding());
      CHECK_EQUAL(2U, weekday2.c_encoding());
      CHECK_EQUAL(3U, weekday3.c_encoding());
      CHECK_EQUAL(4U, weekday4.c_encoding());
      CHECK_EQUAL(5U, weekday5.c_encoding());
      CHECK_EQUAL(6U, weekday6.c_encoding());
    }

    //*************************************************************************
    TEST(test_weekday_comparison_operators)
    {
        etl::chrono::weekday weekday1(1);
        etl::chrono::weekday weekday2(2);

        CHECK_TRUE(weekday1  == weekday1);
        CHECK_FALSE(weekday1 != weekday1);
        CHECK_TRUE(weekday1   < weekday2);
        CHECK_FALSE(weekday1  < weekday1);
        CHECK_FALSE(weekday2  < weekday1);
        CHECK_TRUE(weekday1  <= weekday2);
        CHECK_TRUE(weekday1  <= weekday1);
        CHECK_FALSE(weekday2 <= weekday1);
        CHECK_FALSE(weekday1  > weekday2);
        CHECK_FALSE(weekday1  > weekday1);
        CHECK_TRUE(weekday2   > weekday1);
        CHECK_FALSE(weekday1 >= weekday2);
        CHECK_TRUE(weekday1  >= weekday1);
        CHECK_TRUE(weekday2  >= weekday1);

#if ETL_USING_CPP20
        CHECK_TRUE((weekday1 <=> weekday1) == 0);
        CHECK_TRUE((weekday1 <=> weekday2)  < 0);
        CHECK_TRUE((weekday2 <=> weekday1)  > 0);
#endif
    }

    //*************************************************************************
    TEST(test_weekday_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 256; ++i)
      {
        hashes.push_back(etl::hash<etl::chrono::weekday>()(etl::chrono::weekday(i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(256U, hashes.size());
    }

    //*************************************************************************
    TEST(test_weekday_types)
    {
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::January),   static_cast<unsigned>(etl::chrono::January));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::February),  static_cast<unsigned>(etl::chrono::February));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::March),     static_cast<unsigned>(etl::chrono::March));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::April),     static_cast<unsigned>(etl::chrono::April));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::May),       static_cast<unsigned>(etl::chrono::May));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::June),      static_cast<unsigned>(etl::chrono::June));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::July),      static_cast<unsigned>(etl::chrono::July));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::August),    static_cast<unsigned>(etl::chrono::August));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::September), static_cast<unsigned>(etl::chrono::September));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::October),   static_cast<unsigned>(etl::chrono::October));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::November),  static_cast<unsigned>(etl::chrono::November));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::December),  static_cast<unsigned>(etl::chrono::December));
    }
  };
}

#endif