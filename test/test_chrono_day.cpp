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

namespace
{
  SUITE(test_chrono_day)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      std::chrono::day std_day;
      etl::chrono::day day;

      CHECK_EQUAL(std_day.ok(), day.ok());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 0U; i < 256; ++i)
      {
        std::chrono::day std_day(i);
        etl::chrono::day day(i);

        CHECK_EQUAL(std_day.ok(), day.ok());
        CHECK_EQUAL(unsigned(std_day), unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      std::chrono::day std_day(0);
      etl::chrono::day day(0);

      for (int i = 0; i < 255; ++i)
      {
        ++std_day;
        ++day;

        CHECK_EQUAL(std_day.ok(), day.ok());
        CHECK_EQUAL(unsigned(std_day), unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      std::chrono::day std_day(0);
      etl::chrono::day day(0);

      for (int i = 0; i < 255; ++i)
      {
        std::chrono::day std_last_day = std_day++;
        etl::chrono::day last_day     = day++;

        CHECK_EQUAL(std_last_day.ok(), last_day.ok());
        CHECK_EQUAL(unsigned(std_last_day), unsigned(last_day));

        CHECK_EQUAL(std_day.ok(), day.ok());
        CHECK_EQUAL(unsigned(std_day), unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      std::chrono::day std_day(256);
      etl::chrono::day day(256);

      for (int i = 0; i < 255; ++i)
      {
        --std_day;
        --day;

        CHECK_EQUAL(std_day.ok(), day.ok());
        CHECK_EQUAL(unsigned(std_day), unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      std::chrono::day std_day(256);
      etl::chrono::day day(256);

      for (int i = 0; i < 255; ++i)
      {
        std::chrono::day std_last_day = std_day--;
        etl::chrono::day last_day = day--;

        CHECK_EQUAL(std_last_day.ok(), last_day.ok());
        CHECK_EQUAL(unsigned(std_last_day), unsigned(last_day));

        CHECK_EQUAL(std_day.ok(), day.ok());
        CHECK_EQUAL(unsigned(std_day), unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_days)
    {
      std::chrono::day std_day(0);
      etl::chrono::day day(0);

      std::chrono::days std_days(2);
      etl::chrono::days days(2);

      for (int i = 0; i < 128; ++i)
      {
        std_day += std_days;
        day     += days;

        CHECK_EQUAL(std_day.ok(), day.ok());
        CHECK_EQUAL(unsigned(std_day), unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_plus_days)
    {
      std::chrono::day std_day1(0);
      std::chrono::day std_day2(0);
      etl::chrono::day day1(0);
      etl::chrono::day day2(0);

      std::chrono::days std_days(2);
      etl::chrono::days days(2);

      for (int i = 0; i < 128; ++i)
      {
        std_day1 = std_days + std_day1;
        std_day2 = std_day2 + std_days;
        
        day1 = days + day1;
        day2 = day2 + days;

        CHECK_EQUAL(std_day1.ok(), day1.ok());
        CHECK_EQUAL(std_day2.ok(), day2.ok());
        CHECK_EQUAL(unsigned(std_day1), unsigned(day1));
        CHECK_EQUAL(unsigned(std_day2), unsigned(day2));
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_days)
    {
      std::chrono::day std_day(256);
      etl::chrono::day day(256);

      std::chrono::days std_days(2);
      etl::chrono::days days(2);

      for (int i = 0; i < 128; ++i)
      {
        std_day -= std_days;
        day     -= days;

        CHECK_EQUAL(std_day.ok(), day.ok());
        CHECK_EQUAL(unsigned(std_day), unsigned(day));
      }
    }

    //*************************************************************************
    TEST(test_literal_days)
    {
      using namespace std::literals::chrono_literals;
      using namespace etl::literals::chrono_literals;

      std::chrono::day std_day = 25d;
      etl::chrono::day day     = 25_d;

      CHECK_EQUAL(std_day.ok(), day.ok());
      CHECK_EQUAL(unsigned(std_day), unsigned(day));
    }
  };
}

#endif