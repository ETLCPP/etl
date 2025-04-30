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
#include <array>
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
  SUITE(test_chrono_month_day_last)
  {
    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      Chrono::month_day_last month_day_last_january(Chrono::January);
      Chrono::month_day_last month_day_last_february(Chrono::February);
      Chrono::month_day_last month_day_last_march(Chrono::March);
      Chrono::month_day_last month_day_last_april(Chrono::April);
      Chrono::month_day_last month_day_last_may(Chrono::May);
      Chrono::month_day_last month_day_last_june(Chrono::June);
      Chrono::month_day_last month_day_last_july(Chrono::July);
      Chrono::month_day_last month_day_last_august(Chrono::August);
      Chrono::month_day_last month_day_last_september(Chrono::September);
      Chrono::month_day_last month_day_last_october(Chrono::October);
      Chrono::month_day_last month_day_last_november(Chrono::November);
      Chrono::month_day_last month_day_last_december(Chrono::December);

      CHECK_TRUE(month_day_last_january.ok());
      CHECK_TRUE(month_day_last_february.ok());
      CHECK_TRUE(month_day_last_march.ok());
      CHECK_TRUE(month_day_last_april.ok());
      CHECK_TRUE(month_day_last_may.ok());
      CHECK_TRUE(month_day_last_june.ok());
      CHECK_TRUE(month_day_last_july.ok());
      CHECK_TRUE(month_day_last_august.ok());
      CHECK_TRUE(month_day_last_september.ok());
      CHECK_TRUE(month_day_last_october.ok());
      CHECK_TRUE(month_day_last_november.ok());
      CHECK_TRUE(month_day_last_december.ok());
    }

    //*************************************************************************
    TEST(test_month_day_last_comparison_operators)
    {
      Chrono::month_day_last month_day_last1(Chrono::January);
      Chrono::month_day_last month_day_last2(Chrono::January);
      Chrono::month_day_last month_day_last3(Chrono::February);

      CHECK_TRUE(month_day_last1  == month_day_last2);
      CHECK_FALSE(month_day_last1 == month_day_last3);

      CHECK_FALSE(month_day_last1 != month_day_last2);
      CHECK_TRUE(month_day_last1  != month_day_last3);
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_month_day_last_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 6; ++i)
      {
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::January)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::February)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::March)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::April)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::May)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::June)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::July)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::August)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::September)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::October)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::November)));
        hashes.push_back(etl::hash<Chrono::month_day_last>()(Chrono::month_day_last(Chrono::December)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
    }
#endif
  };
}
