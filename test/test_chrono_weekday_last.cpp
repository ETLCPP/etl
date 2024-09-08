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

#include <chrono>

#include <array>
#include <algorithm>

namespace
{
  SUITE(test_chrono_weekday_last)
  {
    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 1U; i < 5U; ++i)
      {
        etl::chrono::weekday_last weekday_last_monday(etl::chrono::Monday);
        etl::chrono::weekday_last weekday_last_tuesday(etl::chrono::Tuesday);
        etl::chrono::weekday_last weekday_last_wednesday(etl::chrono::Wednesday);
        etl::chrono::weekday_last weekday_last_thursday(etl::chrono::Thursday);
        etl::chrono::weekday_last weekday_last_friday(etl::chrono::Friday);
        etl::chrono::weekday_last weekday_last_saturday(etl::chrono::Saturday);
        etl::chrono::weekday_last weekday_last_sunday(etl::chrono::Sunday);

        CHECK_TRUE(weekday_last_monday.ok());
        CHECK_TRUE(weekday_last_tuesday.ok());
        CHECK_TRUE(weekday_last_wednesday.ok());
        CHECK_TRUE(weekday_last_thursday.ok());
        CHECK_TRUE(weekday_last_friday.ok());
        CHECK_TRUE(weekday_last_saturday.ok());
        CHECK_TRUE(weekday_last_sunday.ok());

        CHECK_EQUAL(etl::chrono::Monday.c_encoding(),    weekday_last_monday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Tuesday.c_encoding(),   weekday_last_tuesday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Wednesday.c_encoding(), weekday_last_wednesday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Thursday.c_encoding(),  weekday_last_thursday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Friday.c_encoding(),    weekday_last_friday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Saturday.c_encoding(),  weekday_last_saturday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Sunday.c_encoding(),    weekday_last_sunday.weekday().c_encoding());
      }
    }

    //*************************************************************************
    TEST(test_constructor_out_of_range)
    {
      for (unsigned i = 8U; i < 256U; ++i)
      {
        auto wd = etl::chrono::weekday(i);
        etl::chrono::weekday_last weekday_last(wd);

        CHECK_FALSE(weekday_last.ok());
      }
    }

    //*************************************************************************
    TEST(test_weekday_last_comparison_operators)
    {
      etl::chrono::weekday_last weekday_last1(etl::chrono::Monday);
      etl::chrono::weekday_last weekday_last2(etl::chrono::Monday);
      etl::chrono::weekday_last weekday_last3(etl::chrono::Tuesday);

      CHECK_TRUE(weekday_last1  == weekday_last2);
      CHECK_FALSE(weekday_last1 == weekday_last3);
    }

    //*************************************************************************
    TEST(test_weekday_last_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 6; ++i)
      {
        hashes.push_back(etl::hash<etl::chrono::weekday_last>()(etl::chrono::weekday_last(etl::chrono::Monday)));
        hashes.push_back(etl::hash<etl::chrono::weekday_last>()(etl::chrono::weekday_last(etl::chrono::Tuesday)));
        hashes.push_back(etl::hash<etl::chrono::weekday_last>()(etl::chrono::weekday_last(etl::chrono::Wednesday)));
        hashes.push_back(etl::hash<etl::chrono::weekday_last>()(etl::chrono::weekday_last(etl::chrono::Thursday)));
        hashes.push_back(etl::hash<etl::chrono::weekday_last>()(etl::chrono::weekday_last(etl::chrono::Friday)));
        hashes.push_back(etl::hash<etl::chrono::weekday_last>()(etl::chrono::weekday_last(etl::chrono::Saturday)));
        hashes.push_back(etl::hash<etl::chrono::weekday_last>()(etl::chrono::weekday_last(etl::chrono::Sunday)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
    }
  };
}
