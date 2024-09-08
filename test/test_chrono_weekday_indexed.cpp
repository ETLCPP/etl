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
  SUITE(test_chrono_weekday_indexed)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::chrono::weekday_indexed weekday_indexed;

      CHECK_FALSE(weekday_indexed.ok());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 1U; i < 5U; ++i)
      {
        etl::chrono::weekday_indexed weekday_indexed_monday(etl::chrono::Monday,       i);
        etl::chrono::weekday_indexed weekday_indexed_tuesday(etl::chrono::Tuesday,     i);
        etl::chrono::weekday_indexed weekday_indexed_wednesday(etl::chrono::Wednesday, i);
        etl::chrono::weekday_indexed weekday_indexed_thursday(etl::chrono::Thursday,   i);
        etl::chrono::weekday_indexed weekday_indexed_friday(etl::chrono::Friday,       i);
        etl::chrono::weekday_indexed weekday_indexed_saturday(etl::chrono::Saturday,   i);
        etl::chrono::weekday_indexed weekday_indexed_sunday(etl::chrono::Sunday,       i);

        CHECK_TRUE(weekday_indexed_monday.ok());
        CHECK_TRUE(weekday_indexed_tuesday.ok());
        CHECK_TRUE(weekday_indexed_wednesday.ok());
        CHECK_TRUE(weekday_indexed_thursday.ok());
        CHECK_TRUE(weekday_indexed_friday.ok());
        CHECK_TRUE(weekday_indexed_saturday.ok());
        CHECK_TRUE(weekday_indexed_sunday.ok());
        
        CHECK_EQUAL(etl::chrono::Monday.c_encoding(),    weekday_indexed_monday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Tuesday.c_encoding(),   weekday_indexed_tuesday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Wednesday.c_encoding(), weekday_indexed_wednesday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Thursday.c_encoding(),  weekday_indexed_thursday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Friday.c_encoding(),    weekday_indexed_friday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Saturday.c_encoding(),  weekday_indexed_saturday.weekday().c_encoding());
        CHECK_EQUAL(etl::chrono::Sunday.c_encoding(),    weekday_indexed_sunday.weekday().c_encoding());
        
        CHECK_EQUAL(i, weekday_indexed_monday.index());
        CHECK_EQUAL(i, weekday_indexed_tuesday.index());
        CHECK_EQUAL(i, weekday_indexed_wednesday.index());
        CHECK_EQUAL(i, weekday_indexed_thursday.index());
        CHECK_EQUAL(i, weekday_indexed_friday.index());
        CHECK_EQUAL(i, weekday_indexed_saturday.index());
        CHECK_EQUAL(i, weekday_indexed_sunday.index());
      }
    }

    //*************************************************************************
    TEST(test_constructor_out_of_range)
    {
      for (unsigned i = 6U; i < 256U; ++i)
      {
        etl::chrono::weekday_indexed weekday_indexed_monday(etl::chrono::Monday,       i);
        etl::chrono::weekday_indexed weekday_indexed_tuesday(etl::chrono::Tuesday,     i);
        etl::chrono::weekday_indexed weekday_indexed_wednesday(etl::chrono::Wednesday, i);
        etl::chrono::weekday_indexed weekday_indexed_thursday(etl::chrono::Thursday,   i);
        etl::chrono::weekday_indexed weekday_indexed_friday(etl::chrono::Friday,       i);
        etl::chrono::weekday_indexed weekday_indexed_saturday(etl::chrono::Saturday,   i);
        etl::chrono::weekday_indexed weekday_indexed_sunday(etl::chrono::Sunday,       i);

        CHECK_FALSE(weekday_indexed_monday.ok());
        CHECK_FALSE(weekday_indexed_tuesday.ok());
        CHECK_FALSE(weekday_indexed_wednesday.ok());
        CHECK_FALSE(weekday_indexed_thursday.ok());
        CHECK_FALSE(weekday_indexed_friday.ok());
        CHECK_FALSE(weekday_indexed_saturday.ok());
        CHECK_FALSE(weekday_indexed_sunday.ok());
      }
    }

    //*************************************************************************
    TEST(test_weekday_indexed_comparison_operators)
    {
      etl::chrono::weekday_indexed weekday_indexed1(etl::chrono::Monday,  1);
      etl::chrono::weekday_indexed weekday_indexed2(etl::chrono::Monday,  1);
      etl::chrono::weekday_indexed weekday_indexed3(etl::chrono::Monday,  2);
      etl::chrono::weekday_indexed weekday_indexed4(etl::chrono::Tuesday, 1);

      CHECK_TRUE(weekday_indexed1  == weekday_indexed2);
      CHECK_FALSE(weekday_indexed1 == weekday_indexed3);
      CHECK_FALSE(weekday_indexed1 == weekday_indexed4);
    }

    //*************************************************************************
    TEST(test_weekday_indexed_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 6; ++i)
      {
        hashes.push_back(etl::hash<etl::chrono::weekday_indexed>()(etl::chrono::weekday_indexed(etl::chrono::Monday,    i)));
        hashes.push_back(etl::hash<etl::chrono::weekday_indexed>()(etl::chrono::weekday_indexed(etl::chrono::Tuesday,   i)));
        hashes.push_back(etl::hash<etl::chrono::weekday_indexed>()(etl::chrono::weekday_indexed(etl::chrono::Wednesday, i)));
        hashes.push_back(etl::hash<etl::chrono::weekday_indexed>()(etl::chrono::weekday_indexed(etl::chrono::Thursday,  i)));
        hashes.push_back(etl::hash<etl::chrono::weekday_indexed>()(etl::chrono::weekday_indexed(etl::chrono::Friday,    i)));
        hashes.push_back(etl::hash<etl::chrono::weekday_indexed>()(etl::chrono::weekday_indexed(etl::chrono::Saturday,  i)));
        hashes.push_back(etl::hash<etl::chrono::weekday_indexed>()(etl::chrono::weekday_indexed(etl::chrono::Sunday,    i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
    }
  };
}
