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
  SUITE(test_chrono_year_month_day)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Chrono::year_month_day ymd;

      CHECK_FALSE(ymd.ok()); // Default-constructed year_month_day is not valid
    }

    //*************************************************************************
    TEST(test_constructor_with_month_and_day)
    {
      Chrono::year_month_day ymd{Chrono::year(2000), Chrono::January, Chrono::day(1)};

      CHECK_TRUE(ymd.ok()); // Valid year_month_day
      CHECK_EQUAL(2000,                      (int)ymd.year());
      CHECK_EQUAL((unsigned)Chrono::January, (unsigned)ymd.month());
      CHECK_EQUAL(Chrono::day(1),            ymd.day());
    }

    //*************************************************************************
    TEST(test_invalid_year_month)
    {
      Chrono::year_month_day ymd{Chrono::year{32768}, Chrono::January, Chrono::day(1)}; // Invalid year

      CHECK_FALSE(ymd.ok()); // Invalid year_month_day
    }

    //*************************************************************************
    TEST(test_invalid_month_in_year_month)
    {
      Chrono::year_month_day ymd{Chrono::year{2000}, Chrono::month{13}, Chrono::day(1)}; // Invalid month (13)

      CHECK_FALSE(ymd.ok()); // Invalid year_month_day
    }

    //*************************************************************************
    TEST(test_invalid_day_in_year_month)
    {
      Chrono::year_month_day ymd{Chrono::year{2000}, Chrono::January, Chrono::day(32)}; // Invalid day (32)

      CHECK_FALSE(ymd.ok()); // Invalid year_month_day
    }

#if ETL_USING_CPP20
    //*************************************************************************
    TEST(test_year_month_day_spaceship_operator)
    {
      Chrono::year_month_day ym1{Chrono::year(2000), Chrono::January,  Chrono::day(1)};
      Chrono::year_month_day ym2{Chrono::year(2001), Chrono::January,  Chrono::day(1)};
      Chrono::year_month_day ym3{Chrono::year(2000), Chrono::February, Chrono::day(1)};
      Chrono::year_month_day ym4{Chrono::year(2000), Chrono::January,  Chrono::day(2)};

      CHECK_TRUE((ym1 <=> ym1) == std::strong_ordering::equal);
      CHECK_TRUE((ym1 <=> ym2) == std::strong_ordering::less);
      CHECK_TRUE((ym2 <=> ym1) == std::strong_ordering::greater);
      CHECK_TRUE((ym1 <=> ym3) == std::strong_ordering::less);
      CHECK_TRUE((ym3 <=> ym1) == std::strong_ordering::greater);
      CHECK_TRUE((ym1 <=> ym4) == std::strong_ordering::less);
      CHECK_TRUE((ym4 <=> ym1) == std::strong_ordering::greater);
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_year_month_day_compare)
    {
      Chrono::year_month_day ym1{Chrono::year(2000), Chrono::January,  Chrono::day(1)};
      Chrono::year_month_day ym2{Chrono::year(2001), Chrono::January,  Chrono::day(1)};
      Chrono::year_month_day ym3{Chrono::year(2000), Chrono::February, Chrono::day(1)};
      Chrono::year_month_day ym4{Chrono::year(2000), Chrono::January,  Chrono::day(2)};

      CHECK_TRUE(ym1.compare(ym1) == 0);
      CHECK_TRUE(ym1.compare(ym2) == -1);
      CHECK_TRUE(ym2.compare(ym1) == 1);
      CHECK_TRUE(ym1.compare(ym3) == -1);
      CHECK_TRUE(ym3.compare(ym1) == 1);
      CHECK_TRUE(ym1.compare(ym4) == -1);
      CHECK_TRUE(ym4.compare(ym1) == 1);
    }
#endif

    //*************************************************************************
    TEST(test_to_sys_days)
    {
      Chrono::year_month_day ymd{Chrono::year(2000), Chrono::February, Chrono::day(10)};
      Chrono::sys_days sd = Chrono::sys_days(ymd);

      CHECK_EQUAL(10997, sd.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_year_month_day_equality_operator)
    {
      Chrono::year_month_day ym1{Chrono::year(2000), Chrono::January,  Chrono::day(1)};
      Chrono::year_month_day ym2{Chrono::year(2001), Chrono::January,  Chrono::day(1)};
      Chrono::year_month_day ym3{Chrono::year(2000), Chrono::February, Chrono::day(1)};
      Chrono::year_month_day ym4{Chrono::year(2000), Chrono::January,  Chrono::day(2)};

      CHECK_TRUE(ym1 == ym1);  // Same year/month/day
      CHECK_FALSE(ym1 == ym2); // Different year
      CHECK_FALSE(ym1 == ym3); // Different month
      CHECK_FALSE(ym1 == ym4); // Different day
    }

    //*************************************************************************
    TEST(test_year_month_day_not_equality_operator)
    {
      Chrono::year_month_day ym1{Chrono::year(2000), Chrono::January,  Chrono::day(1)};
      Chrono::year_month_day ym2{Chrono::year(2001), Chrono::January,  Chrono::day(1)};
      Chrono::year_month_day ym3{Chrono::year(2000), Chrono::February, Chrono::day(1)};
      Chrono::year_month_day ym4{Chrono::year(2000), Chrono::January,  Chrono::day(2)};

      CHECK_FALSE(ym1 != ym1); // Same year/month/day
      CHECK_TRUE(ym1 != ym2);  // Different year
      CHECK_TRUE(ym1 != ym3);  // Different month
      CHECK_TRUE(ym1 != ym4);  // Different day
    }
  };
}
