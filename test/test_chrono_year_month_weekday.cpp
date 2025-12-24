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

#include <vector>
#include <algorithm>

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
  SUITE(test_chrono_year_month_weekday)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Chrono::year_month_weekday ymwd;

      CHECK_FALSE(ymwd.ok()); // Default-constructed year_month_weekday is not valid
    }

    //*************************************************************************
    TEST(test_constructor_with_month_and_day)
    {
      Chrono::year_month_weekday ymwd{Chrono::year(2000), Chrono::January, Chrono::weekday_indexed(Chrono::Thursday, 1)};

      CHECK_TRUE(ymwd.ok()); // Valid year_month_weekday
      CHECK_EQUAL(2000,                          (int)ymwd.year());
      CHECK_EQUAL((unsigned)Chrono::January,     (unsigned)ymwd.month());
      CHECK_EQUAL(Chrono::Thursday.c_encoding(), ymwd.weekday().c_encoding());
      CHECK_EQUAL(1,                             ymwd.index());
      CHECK_TRUE(ymwd.weekday_indexed().ok());
      CHECK_EQUAL(Chrono::Thursday.c_encoding(), ymwd.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(1,                             ymwd.weekday_indexed().index());
    }

    //*************************************************************************
    TEST(test_constructor_with_sys_days)
    {
      Chrono::year_month_weekday expected{Chrono::year(2000), Chrono::February, Chrono::weekday_indexed(Chrono::Thursday, 1)};
      Chrono::year_month_weekday actual{Chrono::sys_days(Chrono::days(10990))};

      CHECK_EQUAL((int)expected.year(), (int)actual.year());
      CHECK_EQUAL((unsigned)expected.month(), (unsigned)actual.month());
      CHECK_EQUAL((unsigned)expected.weekday().c_encoding(), (unsigned)actual.weekday().c_encoding());
      CHECK_EQUAL((unsigned)expected.index(), (unsigned)actual.index());
    }

    //*************************************************************************
    TEST(test_invalid_year_month)
    {
      Chrono::year_month_weekday ymwd{Chrono::year{32768}, Chrono::January, Chrono::weekday_indexed(Chrono::Thursday, 1)}; // Invalid year

      CHECK_FALSE(ymwd.ok()); // Invalid year_month_weekday
    }

    //*************************************************************************
    TEST(test_invalid_month_in_year_month)
    {
      Chrono::year_month_weekday ymwd{Chrono::year{2000}, Chrono::month{13}, Chrono::weekday_indexed(Chrono::Thursday, 1)}; // Invalid month (13)

      CHECK_FALSE(ymwd.ok()); // Invalid year_month_weekday
    }

    //*************************************************************************
    TEST(test_invalid_day_in_year_month)
    {
      Chrono::year_month_weekday ymwd{Chrono::year{2000}, Chrono::January, Chrono::weekday_indexed(Chrono::Thursday, 6)}; // Invalid day (6)

      CHECK_FALSE(ymwd.ok()); // Invalid year_month_weekday
    }

    //*************************************************************************
    TEST(test_to_sys_days)
    {
      Chrono::year_month_weekday ymwd{Chrono::year(2000), Chrono::February, Chrono::weekday_indexed(Chrono::Thursday, 1)};
      Chrono::sys_days sd = Chrono::sys_days(ymwd);

      CHECK_EQUAL(10990, sd.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_year_month_weekday_equality_operator)
    {
      Chrono::year_month_weekday ym1{Chrono::year(2000), Chrono::January,  Chrono::weekday_indexed(Chrono::Thursday, 1)};
      Chrono::year_month_weekday ym2{Chrono::year(2001), Chrono::January,  Chrono::weekday_indexed(Chrono::Thursday, 1)};
      Chrono::year_month_weekday ym3{Chrono::year(2000), Chrono::February, Chrono::weekday_indexed(Chrono::Thursday, 1)};
      Chrono::year_month_weekday ym4{Chrono::year(2000), Chrono::January,  Chrono::weekday_indexed(Chrono::Friday, 2)};

      CHECK_TRUE(ym1 == ym1);  // Same year/month/day
      CHECK_FALSE(ym1 == ym2); // Different year
      CHECK_FALSE(ym1 == ym3); // Different month
      CHECK_FALSE(ym1 == ym4); // Different day
    }

    //*************************************************************************
    TEST(test_year_month_weekday_not_equality_operator)
    {
      Chrono::year_month_weekday ym1{Chrono::year(2000), Chrono::January,  Chrono::weekday_indexed(Chrono::Thursday, 1)};
      Chrono::year_month_weekday ym2{Chrono::year(2001), Chrono::January,  Chrono::weekday_indexed(Chrono::Thursday, 1)};
      Chrono::year_month_weekday ym3{Chrono::year(2000), Chrono::February, Chrono::weekday_indexed(Chrono::Thursday, 1)};
      Chrono::year_month_weekday ym4{Chrono::year(2000), Chrono::January,  Chrono::weekday_indexed(Chrono::Friday, 2)};

      CHECK_FALSE(ym1 != ym1); // Same year/month/day
      CHECK_TRUE(ym1 != ym2);  // Different year
      CHECK_TRUE(ym1 != ym3);  // Different month
      CHECK_TRUE(ym1 != ym4);  // Different day
    }
  };
}
