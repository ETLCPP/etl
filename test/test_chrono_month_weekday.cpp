/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation: 

Copyright(c) 2024 John Wellbelove

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
  SUITE(test_chrono_month_weekday)
  {
    //*************************************************************************
    TEST(test_constructor_with_month_and_day)
    {
      Chrono::month_weekday mwd{ Chrono::January, Chrono::weekday_indexed(Chrono::Friday, 2) };

      CHECK_TRUE(mwd.ok()); // Valid month_weekday
      CHECK_EQUAL(Chrono::January, mwd.month());
      CHECK_EQUAL(Chrono::Friday.c_encoding(), mwd.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(2, mwd.weekday_indexed().index());
    }

    //*************************************************************************
    TEST(test_invalid_month_weekday)
    {
      Chrono::month_weekday mwd{Chrono::month{13}, Chrono::weekday_indexed(Chrono::Friday, 2)}; // Invalid month (13)

      CHECK_FALSE(mwd.ok()); // Invalid month_weekday
    }

    //*************************************************************************
    TEST(test_invalid_day_in_month_weekday)
    {
      Chrono::month_weekday mwd{Chrono::January, Chrono::weekday_indexed(Chrono::weekday{8}, 2)}; // Invalid day (8)

      CHECK_FALSE(mwd.ok()); // Invalid month_weekday
    }

    //*************************************************************************
    TEST(test_month_weekday_equality_operator)
    {
      Chrono::month_weekday mwd1{Chrono::January,  Chrono::weekday_indexed(Chrono::Friday, 2)};
      Chrono::month_weekday mwd2{Chrono::February, Chrono::weekday_indexed(Chrono::Friday, 2)};

      CHECK_TRUE(mwd1 == mwd1);  // January == January
      CHECK_FALSE(mwd1 == mwd2); // January != February
      CHECK_FALSE(mwd1 == mwd2); // Friday != Saturday
    }

    //*************************************************************************
    TEST(test_month_weekday_not_equality_operator)
    {
      Chrono::month_weekday mwd1{Chrono::January,  Chrono::weekday_indexed(Chrono::Friday, 2)};
      Chrono::month_weekday mwd2{Chrono::February, Chrono::weekday_indexed(Chrono::Friday, 2)};

      CHECK_FALSE(mwd1 != mwd1); // January == January
      CHECK_TRUE(mwd1 != mwd2);  // January != February
      CHECK_TRUE(mwd1 != mwd2);  // Friday != Saturday
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_month_weekday_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 256; ++i)
      {
        hashes.push_back(etl::hash<Chrono::month_weekday>()(Chrono::month_weekday(Chrono::month((i % 12) + 1), Chrono::weekday_indexed(Chrono::weekday(i % 7), i % 5))));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(256U, hashes.size());
    }
#endif
  };
}
