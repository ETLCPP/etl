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
  SUITE(test_chrono_weekday_indexed)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Chrono::weekday_indexed weekday_indexed;

      CHECK_FALSE(weekday_indexed.ok());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 1U; i < 5U; ++i)
      {
        Chrono::weekday_indexed weekday_indexed_monday(Chrono::Monday,       i);
        Chrono::weekday_indexed weekday_indexed_tuesday(Chrono::Tuesday,     i);
        Chrono::weekday_indexed weekday_indexed_wednesday(Chrono::Wednesday, i);
        Chrono::weekday_indexed weekday_indexed_thursday(Chrono::Thursday,   i);
        Chrono::weekday_indexed weekday_indexed_friday(Chrono::Friday,       i);
        Chrono::weekday_indexed weekday_indexed_saturday(Chrono::Saturday,   i);
        Chrono::weekday_indexed weekday_indexed_sunday(Chrono::Sunday,       i);

        CHECK_TRUE(weekday_indexed_monday.ok());
        CHECK_TRUE(weekday_indexed_tuesday.ok());
        CHECK_TRUE(weekday_indexed_wednesday.ok());
        CHECK_TRUE(weekday_indexed_thursday.ok());
        CHECK_TRUE(weekday_indexed_friday.ok());
        CHECK_TRUE(weekday_indexed_saturday.ok());
        CHECK_TRUE(weekday_indexed_sunday.ok());
        
        CHECK_EQUAL(Chrono::Monday.c_encoding(),    weekday_indexed_monday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Tuesday.c_encoding(),   weekday_indexed_tuesday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Wednesday.c_encoding(), weekday_indexed_wednesday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Thursday.c_encoding(),  weekday_indexed_thursday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Friday.c_encoding(),    weekday_indexed_friday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Saturday.c_encoding(),  weekday_indexed_saturday.weekday().c_encoding());
        CHECK_EQUAL(Chrono::Sunday.c_encoding(),    weekday_indexed_sunday.weekday().c_encoding());
        
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
        Chrono::weekday_indexed weekday_indexed_monday(Chrono::Monday,       i);
        Chrono::weekday_indexed weekday_indexed_tuesday(Chrono::Tuesday,     i);
        Chrono::weekday_indexed weekday_indexed_wednesday(Chrono::Wednesday, i);
        Chrono::weekday_indexed weekday_indexed_thursday(Chrono::Thursday,   i);
        Chrono::weekday_indexed weekday_indexed_friday(Chrono::Friday,       i);
        Chrono::weekday_indexed weekday_indexed_saturday(Chrono::Saturday,   i);
        Chrono::weekday_indexed weekday_indexed_sunday(Chrono::Sunday,       i);

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
      Chrono::weekday_indexed weekday_indexed1(Chrono::Monday,  1);
      Chrono::weekday_indexed weekday_indexed2(Chrono::Monday,  1);
      Chrono::weekday_indexed weekday_indexed3(Chrono::Monday,  2);
      Chrono::weekday_indexed weekday_indexed4(Chrono::Tuesday, 1);

      CHECK_TRUE(weekday_indexed1  == weekday_indexed2); 
      CHECK_FALSE(weekday_indexed1 == weekday_indexed3);
      CHECK_FALSE(weekday_indexed1 == weekday_indexed4);
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_weekday_indexed_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 6; ++i)
      {
        hashes.push_back(etl::hash<Chrono::weekday_indexed>()(Chrono::weekday_indexed(Chrono::Monday,    i)));
        hashes.push_back(etl::hash<Chrono::weekday_indexed>()(Chrono::weekday_indexed(Chrono::Tuesday,   i)));
        hashes.push_back(etl::hash<Chrono::weekday_indexed>()(Chrono::weekday_indexed(Chrono::Wednesday, i)));
        hashes.push_back(etl::hash<Chrono::weekday_indexed>()(Chrono::weekday_indexed(Chrono::Thursday,  i)));
        hashes.push_back(etl::hash<Chrono::weekday_indexed>()(Chrono::weekday_indexed(Chrono::Friday,    i)));
        hashes.push_back(etl::hash<Chrono::weekday_indexed>()(Chrono::weekday_indexed(Chrono::Saturday,  i)));
        hashes.push_back(etl::hash<Chrono::weekday_indexed>()(Chrono::weekday_indexed(Chrono::Sunday,    i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
    }
#endif
  };
}
