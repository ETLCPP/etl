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
  SUITE(test_chrono_year_month_weekday_last)
  {
    //*************************************************************************
    TEST(test_constructor_with_month_and_day)
    {
      Chrono::year_month_weekday_last ymwdl{Chrono::year(2000), Chrono::January, Chrono::weekday_last(Chrono::Thursday)};

      CHECK_EQUAL(2000,                          (int)ymwdl.year());
      CHECK_EQUAL((unsigned)Chrono::January,     (unsigned)ymwdl.month());
      CHECK_EQUAL(Chrono::Thursday.c_encoding(), ymwdl.weekday().c_encoding());

      using namespace etl::literals::chrono_literals;
    }

    //*************************************************************************
    TEST(test_to_sys_days)
    {
      Chrono::year_month_weekday_last ymwdl{Chrono::year(2000), Chrono::February, Chrono::weekday_last(Chrono::Thursday)};
      Chrono::sys_days sd = Chrono::sys_days(ymwdl);

      CHECK_EQUAL(11012, sd.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_year_month_weekday_last_equality_operator)
    {
      Chrono::year_month_weekday_last ym1{Chrono::year(2000), Chrono::January,  Chrono::weekday_last(Chrono::Thursday)};
      Chrono::year_month_weekday_last ym2{Chrono::year(2001), Chrono::January,  Chrono::weekday_last(Chrono::Thursday)};
      Chrono::year_month_weekday_last ym3{Chrono::year(2000), Chrono::February, Chrono::weekday_last(Chrono::Thursday)};
      Chrono::year_month_weekday_last ym4{Chrono::year(2000), Chrono::January,  Chrono::weekday_last(Chrono::Friday)};

      CHECK_TRUE(ym1 == ym1);  // Same year/month/day
      CHECK_FALSE(ym1 == ym2); // Different year
      CHECK_FALSE(ym1 == ym3); // Different month
      CHECK_FALSE(ym1 == ym4); // Different day
    }

    //*************************************************************************
    TEST(test_year_month_weekday_last_not_equality_operator)
    {
      Chrono::year_month_weekday_last ym1{Chrono::year(2000), Chrono::January,  Chrono::weekday_last(Chrono::Thursday)};
      Chrono::year_month_weekday_last ym2{Chrono::year(2001), Chrono::January,  Chrono::weekday_last(Chrono::Thursday)};
      Chrono::year_month_weekday_last ym3{Chrono::year(2000), Chrono::February, Chrono::weekday_last(Chrono::Thursday)};
      Chrono::year_month_weekday_last ym4{Chrono::year(2000), Chrono::January,  Chrono::weekday_last(Chrono::Friday)};

      CHECK_FALSE(ym1 != ym1); // Same year/month/day
      CHECK_TRUE(ym1 != ym2);  // Different year
      CHECK_TRUE(ym1 != ym3);  // Different month
      CHECK_TRUE(ym1 != ym4);  // Different day
    }
  };
}
