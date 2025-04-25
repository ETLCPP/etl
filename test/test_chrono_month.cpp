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
  //*************************************************************************
  unsigned expected_month(int i)
  {
    i = i % 12;

    if (i < 0)
    { 
      i = 12 + i;
    }

    return i == 0 ? 12 : i;
  }

  //*************************************************************************
  bool expected_month_ok(Chrono::month month)
  {
    unsigned m = unsigned(month);

    if ((m < 1) || (m > 12))
    {
      return (month.ok() == false);
    }
    else
    { 
      return (month.ok() == true);
    }
  }

  SUITE(test_chrono_month)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Chrono::month month;

      CHECK_TRUE(expected_month_ok(month));
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 0U; i < 256; ++i)
      {
        Chrono::month month(i);

        CHECK_TRUE(expected_month_ok(month));
        CHECK_EQUAL(i, unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      Chrono::month month(0);

      for (int i = 1; i < 256; ++i)
      {
        ++month;

        CHECK_TRUE(expected_month_ok(month));
        CHECK_EQUAL(expected_month(i), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      Chrono::month month(0);

      for (int i = 1; i < 256; ++i)
      {
        month++;

        CHECK_TRUE(expected_month_ok(month));
        CHECK_EQUAL(expected_month(i), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      Chrono::month month(255);

      for (int i = 254; i > 1; --i)
      {
        --month;

        CHECK_TRUE(expected_month_ok(month));
        CHECK_EQUAL(expected_month(i), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      Chrono::month month(255);

      for (int i = 254; i > 1; --i)
      {
        month--;

        CHECK_TRUE(expected_month_ok(month));
        CHECK_EQUAL(expected_month(i), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_months)
    {
      for (int m = 0; m <= 12; ++m)
      {
        for (int ms = 0; ms <= 24; ++ms)
        {
          Chrono::month month(m);
          Chrono::months months(ms);
          month += months;

          CHECK_TRUE(expected_month_ok(month));
          CHECK_EQUAL(expected_month(m + ms), unsigned(month));
        }
      }
    }

    //*************************************************************************
    TEST(test_month_plus_months)
    {
      for (int m = 0; m <= 12; ++m)
      {
        for (int ms = 0; ms <= 24; ++ms)
        {
          Chrono::month month(m);
          Chrono::months months(ms);
          month = month + months;

          CHECK_TRUE(expected_month_ok(month));
          CHECK_EQUAL(expected_month(m + ms), unsigned(month));
        }
      }
    }

    //*************************************************************************
    TEST(test_months_plus_month)
    {
      for (int m = 0; m <= 12; ++m)
      {
        for (int ms = 0; ms <= 24; ++ms)
        {
          Chrono::month month(m);
          Chrono::months months(ms);
          month = months + month;

          CHECK_TRUE(expected_month_ok(month));
          CHECK_EQUAL(expected_month(m + ms), unsigned(month));
        }
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_months)
    {
      for (int m = 0; m <= 12; ++m)
      {
        for (int ms = 0; ms <= 24; ++ms)
        {
          Chrono::month month(m);
          Chrono::months months(ms);
          month -= months;

          CHECK_TRUE(expected_month_ok(month));
          CHECK_EQUAL(expected_month(m - ms), unsigned(month));
        }
      }
    }

    //*************************************************************************
    TEST(test_month_minus_months)
    {
      for (int m = 0; m <= 12; ++m)
      {
        for (int ms = 0; ms <= 24; ++ms)
        {
          Chrono::month month(m);
          Chrono::months months(ms);
          month = month - months;

          CHECK_TRUE(expected_month_ok(month));
          CHECK_EQUAL(expected_month(m - ms), unsigned(month));
        }
      }
    }

    //*************************************************************************
    TEST(test_month_minus_month)
    {
      for (int m = 1; m < 13; ++m)
      {
        int m1 = m;
        int m2 = 13 - m;

        Chrono::month month1(m1);
        Chrono::month month2(m2);

        Chrono::months months12 = month1 - month2;
        Chrono::months months21 = month2 - month1;

        int difference12 = expected_month(m1) - expected_month(m2);
        int difference21 = expected_month(m2) - expected_month(m1);

        difference12 = (difference12 < 0) ? 12 + difference12: difference12;
        difference21 = (difference21 < 0) ? 12 + difference21: difference21;

        CHECK_EQUAL(difference12, months12.count());
        CHECK_EQUAL(difference21, months21.count());
      }
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_min_max_month)
    {
      CHECK_EQUAL(1U, Chrono::month::min());
      CHECK_EQUAL(12U, Chrono::month::max());
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_month_compare)
    {
      Chrono::month month1(Chrono::January);
      Chrono::month month2(Chrono::February);

      CHECK_EQUAL(0,  month1.compare(month1));
      CHECK_EQUAL(-1, month1.compare(month2));
      CHECK_EQUAL(1,  month2.compare(month1));
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_literal_month)
    {
      using namespace etl::literals::chrono_literals;

      Chrono::month month1  = 1_month;
      Chrono::month month2  = 2_month;
      Chrono::month month3  = 3_month;
      Chrono::month month4  = 4_month;
      Chrono::month month5  = 5_month;
      Chrono::month month6  = 6_month;
      Chrono::month month7  = 7_month;
      Chrono::month month8  = 8_month;
      Chrono::month month9  = 9_month;
      Chrono::month month10 = 10_month;
      Chrono::month month11 = 11_month;
      Chrono::month month12 = 12_month;

      CHECK_TRUE(month1.ok());
      CHECK_TRUE(month2.ok());
      CHECK_TRUE(month3.ok());
      CHECK_TRUE(month4.ok());
      CHECK_TRUE(month5.ok());
      CHECK_TRUE(month6.ok());
      CHECK_TRUE(month7.ok());
      CHECK_TRUE(month8.ok());
      CHECK_TRUE(month9.ok());
      CHECK_TRUE(month10.ok());
      CHECK_TRUE(month11.ok());
      CHECK_TRUE(month12.ok());

      CHECK_EQUAL(1U,  unsigned(month1));
      CHECK_EQUAL(2U,  unsigned(month2));
      CHECK_EQUAL(3U,  unsigned(month3));
      CHECK_EQUAL(4U,  unsigned(month4));
      CHECK_EQUAL(5U,  unsigned(month5));
      CHECK_EQUAL(6U,  unsigned(month6));
      CHECK_EQUAL(7U,  unsigned(month7));
      CHECK_EQUAL(8U,  unsigned(month8));
      CHECK_EQUAL(9U,  unsigned(month9));
      CHECK_EQUAL(10U, unsigned(month10));
      CHECK_EQUAL(11U, unsigned(month11));
      CHECK_EQUAL(12U, unsigned(month12));
    }
#endif

    //*************************************************************************
    TEST(test_month_comparison_operators)
    {
        Chrono::month month1(1);
        Chrono::month month2(2);

        CHECK_TRUE(month1  == month1);
        CHECK_FALSE(month1 != month1);
        CHECK_TRUE(month1   < month2);
        CHECK_FALSE(month1  < month1);
        CHECK_FALSE(month2  < month1);
        CHECK_TRUE(month1  <= month2);
        CHECK_TRUE(month1  <= month1);
        CHECK_FALSE(month2 <= month1);
        CHECK_FALSE(month1  > month2);
        CHECK_FALSE(month1  > month1);
        CHECK_TRUE(month2   > month1);
        CHECK_FALSE(month1 >= month2);
        CHECK_TRUE(month1  >= month1);
        CHECK_TRUE(month2  >= month1);

#if ETL_USING_CPP20
        CHECK_TRUE((month1 <=> month1) == 0);
        CHECK_TRUE((month1 <=> month2)  < 0);
        CHECK_TRUE((month2 <=> month1)  > 0);
#endif
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_month_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 256; ++i)
      {
        hashes.push_back(etl::hash<Chrono::month>()(Chrono::month(i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(256U, hashes.size());
    }
#endif

    //*************************************************************************
    TEST(test_month_types)
    {
      CHECK_EQUAL(1U,  static_cast<unsigned>(Chrono::January));
      CHECK_EQUAL(2U,  static_cast<unsigned>(Chrono::February));
      CHECK_EQUAL(3U,  static_cast<unsigned>(Chrono::March));
      CHECK_EQUAL(4U,  static_cast<unsigned>(Chrono::April));
      CHECK_EQUAL(5U,  static_cast<unsigned>(Chrono::May));
      CHECK_EQUAL(6U,  static_cast<unsigned>(Chrono::June));
      CHECK_EQUAL(7U,  static_cast<unsigned>(Chrono::July));
      CHECK_EQUAL(8U,  static_cast<unsigned>(Chrono::August));
      CHECK_EQUAL(9U,  static_cast<unsigned>(Chrono::September));
      CHECK_EQUAL(10U, static_cast<unsigned>(Chrono::October));
      CHECK_EQUAL(11U, static_cast<unsigned>(Chrono::November));
      CHECK_EQUAL(12U, static_cast<unsigned>(Chrono::December));
    }
  };
}
