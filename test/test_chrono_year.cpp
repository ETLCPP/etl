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
  SUITE(test_chrono_year)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      std::chrono::year std_year;
      etl::chrono::year year;

      CHECK_EQUAL(std_year.ok(), year.ok());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (int32_t i = -32767; i <= 32767; ++i)
      {
        std::chrono::year std_year(i);
        etl::chrono::year year(i);

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      std::chrono::year std_year(-32767);
      etl::chrono::year year(-32767);

      for (int32_t i = 0; i < 65536; ++i)
      {
        ++std_year;
        ++year;

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      std::chrono::year std_year(-32767);
      etl::chrono::year year(-32767);

      for (int32_t i = 0; i < 65536; ++i)
      {
        std::chrono::year std_last_year = std_year++;
        etl::chrono::year last_year     = year++;

        CHECK_EQUAL(std_last_year.ok(), last_year.ok());
        CHECK_EQUAL(int(std_last_year), int(last_year));

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      std::chrono::year std_year(256);
      etl::chrono::year year(256);

      for (int i = 0; i < 255; ++i)
      {
        --std_year;
        --year;

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      std::chrono::year std_year(256);
      etl::chrono::year year(256);

      for (int i = 0; i < 255; ++i)
      {
        std::chrono::year std_last_year = std_year--;
        etl::chrono::year last_year = year--;

        CHECK_EQUAL(std_last_year.ok(), last_year.ok());
        CHECK_EQUAL(int(std_last_year), int(last_year));

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_years)
    {
      std::chrono::year std_year(0);
      etl::chrono::year year(0);

      std::chrono::years std_years(2);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        std_year += std_years;
        year     += years;

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_year_plus_years)
    {
      std::chrono::year std_year(0);
      etl::chrono::year year(0);

      std::chrono::years std_years(2);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        std_year = std_year + std_years;        
        year     = year + years;

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_years_plus_year)
    {
      std::chrono::year std_year(0);
      etl::chrono::year year(0);

      std::chrono::years std_years(2);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        std_year = std_years + std_year;
        year     = years + year;

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_years)
    {
      std::chrono::year std_year(256);
      etl::chrono::year year(256);

      std::chrono::years std_years(2);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        std_year -= std_years;
        year     -= years;

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_year_minus_years)
    {
      std::chrono::year std_year(0);
      etl::chrono::year year(0);

      std::chrono::years std_years(2);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        std_year = std_year - std_years;
        year     = year - years;

        CHECK_EQUAL(std_year.ok(), year.ok());
        CHECK_EQUAL(int(std_year), int(year));
      }
    }

    //*************************************************************************
    TEST(test_year_minus_year)
    {
      for (int i = 1; i < 31; ++i)
      {
        std::chrono::year std_year1(i);
        std::chrono::year std_year2(31 - i);

        etl::chrono::year year1(i);
        etl::chrono::year year2(31 - i);

        std::chrono::years std_years = std_year1 - std_year2;
        etl::chrono::years years     = year1 - year2;

        CHECK_EQUAL(std_years.count(), years.count());
      }
    }

    //*************************************************************************
    TEST(test_min_max_year)
    {
      CHECK_EQUAL(-32767, etl::chrono::year::min());
      CHECK_EQUAL(32767,  etl::chrono::year::max());
    }

    //*************************************************************************
    TEST(test_literal_year)
    {
      using namespace std::literals::chrono_literals;
      using namespace etl::literals::chrono_literals;

      std::chrono::year std_year = 25y;
      etl::chrono::year year     = 25_y;

      CHECK_EQUAL(std_year.ok(), year.ok());
      CHECK_EQUAL(int(std_year), int(year));
    }

    //*************************************************************************
    TEST(test_year_comparison_operators)
    {
        etl::chrono::year year10(-10);
        etl::chrono::year year20(20);

        CHECK_TRUE(year10  == year10);
        CHECK_FALSE(year10 != year10);
        CHECK_TRUE(year10   < year20);
        CHECK_FALSE(year10  < year10);
        CHECK_FALSE(year20  < year10);
        CHECK_TRUE(year10  <= year20);
        CHECK_TRUE(year10  <= year10);
        CHECK_FALSE(year20 <= year10);
        CHECK_FALSE(year10  > year20);
        CHECK_FALSE(year10  > year10);
        CHECK_TRUE(year20   > year10);
        CHECK_FALSE(year10 >= year20);
        CHECK_TRUE(year10  >= year10);
        CHECK_TRUE(year20  >= year10);

#if ETL_USING_CPP20
        CHECK_TRUE((year10 <=> year10) == 0);
        CHECK_TRUE((year10 <=> year20)  < 0);
        CHECK_TRUE((year20 <=> year10)  > 0);
#endif
    }

    //*************************************************************************
    TEST(test_year_hash)
    {
      etl::chrono::year year(-32767);

      size_t h = 0;
      
      h = etl::hash<etl::chrono::year>()(year);

      CHECK_TRUE(h != 0);

      year = etl::chrono::year(32767);
      h = 0;
      h = etl::hash<etl::chrono::year>()(year);

      CHECK_TRUE(h != 0);
    }
  };
}

#endif