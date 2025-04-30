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
  SUITE(test_chrono_weekday_last)
  {
    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 1U; i < 5U; ++i)
      {
        Chrono::weekday_last weekday_last_monday(Chrono::Monday);
        Chrono::weekday_last weekday_last_tuesday(Chrono::Tuesday);
        Chrono::weekday_last weekday_last_wednesday(Chrono::Wednesday);
        Chrono::weekday_last weekday_last_thursday(Chrono::Thursday);
        Chrono::weekday_last weekday_last_friday(Chrono::Friday);
        Chrono::weekday_last weekday_last_saturday(Chrono::Saturday);
        Chrono::weekday_last weekday_last_sunday(Chrono::Sunday);

        CHECK_TRUE(weekday_last_monday.ok());
        CHECK_TRUE(weekday_last_tuesday.ok());
        CHECK_TRUE(weekday_last_wednesday.ok());
        CHECK_TRUE(weekday_last_thursday.ok());
        CHECK_TRUE(weekday_last_friday.ok());
        CHECK_TRUE(weekday_last_saturday.ok());
        CHECK_TRUE(weekday_last_sunday.ok());

        CHECK_EQUAL(Chrono::Monday.c_encoding(),    weekday_last_monday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Tuesday.c_encoding(),   weekday_last_tuesday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Wednesday.c_encoding(), weekday_last_wednesday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Thursday.c_encoding(),  weekday_last_thursday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Friday.c_encoding(),    weekday_last_friday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Saturday.c_encoding(),  weekday_last_saturday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Sunday.c_encoding(),    weekday_last_sunday.weekday().c_encoding());
      }
    }

    //*************************************************************************
    TEST(test_constructor_out_of_range)
    {
      for (unsigned i = 8U; i < 256U; ++i)
      {
        auto wd = Chrono::weekday(i);
        Chrono::weekday_last weekday_last(wd);

        CHECK_FALSE(weekday_last.ok());
      }
    }

    //*************************************************************************
    TEST(test_weekday_last_comparison_operators)
    {
      Chrono::weekday_last weekday_last1(Chrono::Monday);
      Chrono::weekday_last weekday_last2(Chrono::Monday);
      Chrono::weekday_last weekday_last3(Chrono::Tuesday);

      CHECK_TRUE(weekday_last1  == weekday_last2);
      CHECK_FALSE(weekday_last1 == weekday_last3);
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_weekday_last_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 6; ++i)
      {
        hashes.push_back(etl::hash<Chrono::weekday_last>()(Chrono::weekday_last(Chrono::Monday)));
        hashes.push_back(etl::hash<Chrono::weekday_last>()(Chrono::weekday_last(Chrono::Tuesday)));
        hashes.push_back(etl::hash<Chrono::weekday_last>()(Chrono::weekday_last(Chrono::Wednesday)));
        hashes.push_back(etl::hash<Chrono::weekday_last>()(Chrono::weekday_last(Chrono::Thursday)));
        hashes.push_back(etl::hash<Chrono::weekday_last>()(Chrono::weekday_last(Chrono::Friday)));
        hashes.push_back(etl::hash<Chrono::weekday_last>()(Chrono::weekday_last(Chrono::Saturday)));
        hashes.push_back(etl::hash<Chrono::weekday_last>()(Chrono::weekday_last(Chrono::Sunday)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
    }
#endif
  };
}
