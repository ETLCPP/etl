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
  SUITE(test_chrono_year)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::chrono::year year;

      CHECK_TRUE(year.ok());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (int32_t i = -32767; i <= 32767; ++i)
      {
        etl::chrono::year year(i);

        CHECK_TRUE(year.ok());
        CHECK_EQUAL(i, int(year));
      }
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      etl::chrono::year year(-32767);
      int count = int(year);

      for (int32_t i = 0; i < 65534; ++i)
      {
        ++count;
        etl::chrono::year this_year = ++year;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL(count, year);
        CHECK_EQUAL(this_year, year);
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      etl::chrono::year year(-32767);
      int count = -32767;

      for (int32_t i = 0; i < 65534; ++i)
      {
        etl::chrono::year last_year = year++;

        CHECK_TRUE(last_year.ok());
        CHECK_EQUAL(count, int(last_year));

        ++count;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL(count, int(year));
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      etl::chrono::year year(256);
      int count = 256;

      for (int i = 0; i < 255; ++i)
      {
        --year;
        --count;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL(count, int(year));
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      etl::chrono::year year(256);
      int count = (int)year;

      for (int i = 0; i < 255; ++i)
      {
        etl::chrono::year last_year = year--;

        CHECK_TRUE(last_year.ok());
        CHECK_EQUAL(count, int(last_year));

        --count;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL(count, int(year));
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_years)
    {
      etl::chrono::year year(0);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        year += years;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL((2 * i) + 2, int(year));
      }
    }

    //*************************************************************************
    TEST(test_year_plus_years)
    {
      etl::chrono::year year(0);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        year = year + years;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL((2 * i) + 2, int(year));
      }
    }

    //*************************************************************************
    TEST(test_years_plus_year)
    {
      etl::chrono::year year(0);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        year = years + year;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL((2 * i) + 2, int(year));
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_years)
    {
      etl::chrono::year year(256);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        year -= years;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL((256 - (2 * i)) - 2, int(year));
      }
    }

    //*************************************************************************
    TEST(test_year_minus_years)
    {
      etl::chrono::year year(256);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        year = year - years;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL((256 - (2 * i)) - 2, int(year));
      }
    }

    //*************************************************************************
    TEST(test_year_minus_year)
    {
      etl::chrono::year year(256);
      etl::chrono::years years(2);

      for (int i = 0; i < 128; ++i)
      {
        year = years - year;

        CHECK_TRUE(year.ok());
        CHECK_EQUAL((256 - (2 * i)) - 2, int(year));
      }
    }

    //*************************************************************************
    TEST(test_is_leap)
    {
      for (int i = -32767; i <= 32767; ++i)
      {
        bool is_leap = ((i % 4) == 0) && (((i % 100) != 0) || ((i % 400) == 0));

        etl::chrono::year year(i);

        CHECK_EQUAL(is_leap, year.is_leap());
      }
    }


#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_min_max_year)
    {
      CHECK_EQUAL(-32767, etl::chrono::year::min());
      CHECK_EQUAL(32767,  etl::chrono::year::max());
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_literal_year)
    {
      using namespace etl::literals::chrono_literals;

#if ETL_USING_VERBOSE_CHRONO_LITERALS
      etl::chrono::year year = 25_year;
#else
      etl::chrono::year year = 25_y;
#endif

      CHECK_TRUE(year.ok());
      CHECK_EQUAL(25, int(year));
    }
#endif

    //*************************************************************************
    TEST(test_year_comparison_operators)
    {
        etl::chrono::year year10(10);
        etl::chrono::year year20(20);

        CHECK_TRUE(year10  == year10);
        CHECK_FALSE(year10 != year10);

        CHECK_FALSE(year10  == year20);
        CHECK_TRUE(year10 != year20);

#if ETL_USING_CPP20
        CHECK_TRUE((year10 <=> year10) == 0);
        CHECK_TRUE((year10 <=> year20)  < 0);
        CHECK_TRUE((year20 <=> year10)  > 0);
#endif
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_year_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int32_t i = -32767; i < 32768; ++i)
      {
        hashes.push_back(etl::hash<etl::chrono::year>()(etl::chrono::year(i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(65535U, hashes.size());
    }
#endif
  };
}
