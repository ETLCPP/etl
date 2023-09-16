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
#include <array>
#include <algorithm>

namespace
{
  SUITE(test_chrono_month)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      std::chrono::month std_month;
      etl::chrono::month month;

      CHECK_EQUAL(std_month.ok(), month.ok());
    }

    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      for (unsigned i = 0U; i < 256; ++i)
      {
        std::chrono::month std_month(i);
        etl::chrono::month month(i);

        CHECK_EQUAL(std_month.ok(), month.ok());
        CHECK_EQUAL(unsigned(std_month), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      std::chrono::month std_month(0);
      etl::chrono::month month(0);

      for (int i = 0; i < 255; ++i)
      {
        ++std_month;
        ++month;

        CHECK_EQUAL(std_month.ok(), month.ok());
        CHECK_EQUAL(unsigned(std_month), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      std::chrono::month std_month(0);
      etl::chrono::month month(0);

      for (int i = 0; i < 256; ++i)
      {
        std::chrono::month std_last_month = std_month++;
        etl::chrono::month last_month     = month++;

        CHECK_EQUAL(std_last_month.ok(), last_month.ok());
        CHECK_EQUAL(unsigned(std_last_month), unsigned(last_month));

        CHECK_EQUAL(std_month.ok(), month.ok());
        CHECK_EQUAL(unsigned(std_month), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      std::chrono::month std_month(255);
      etl::chrono::month month(255);

      for (int i = 0; i < 256; ++i)
      {
        --std_month;
        --month;

        CHECK_EQUAL(std_month.ok(), month.ok());
        CHECK_EQUAL(unsigned(std_month), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      std::chrono::month std_month(255);
      etl::chrono::month month(255);

      for (int i = 0; i < 256; ++i)
      {
        std::chrono::month std_last_month = std_month--;
        etl::chrono::month last_month = month--;

        CHECK_EQUAL(std_last_month.ok(), last_month.ok());
        CHECK_EQUAL(unsigned(std_last_month), unsigned(last_month));

        CHECK_EQUAL(std_month.ok(), month.ok());
        CHECK_EQUAL(unsigned(std_month), unsigned(month));
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_months)
    {
      for (int m = 0; m <= 12; ++m)
      {
        for (int ms = 0; ms <= 24; ++ms)
        {
          std::chrono::month std_month(m);
          etl::chrono::month month(m);

          std::chrono::months std_months(ms);
          etl::chrono::months months(ms);

          std_month += std_months;
          month     += months;

          CHECK_EQUAL(std_month.ok(), month.ok());
          CHECK_EQUAL(unsigned(std_month), unsigned(month));
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
          std::chrono::month std_month(m);
          etl::chrono::month month(m);

          std::chrono::months std_months(ms);
          etl::chrono::months months(ms);

          std_month = std_month + std_months;
          month     = month + months;

          CHECK_EQUAL(std_month.ok(), month.ok());
          CHECK_EQUAL(unsigned(std_month), unsigned(month));
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
          std::chrono::month std_month(m);
          etl::chrono::month month(m);

          std::chrono::months std_months(ms);
          etl::chrono::months months(ms);

          std_month = std_months + std_month;
          month     = months + month;

          CHECK_EQUAL(std_month.ok(), month.ok());
          CHECK_EQUAL(unsigned(std_month), unsigned(month));
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
          std::chrono::month std_month(m);
          etl::chrono::month month(m);

          std::chrono::months std_months(ms);
          etl::chrono::months months(ms);

          std_month -= std_months;
          month     -= months;

          CHECK_EQUAL(std_month.ok(), month.ok());
          CHECK_EQUAL(unsigned(std_month), unsigned(month));
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
          std::chrono::month std_month(m);
          etl::chrono::month month(m);

          std::chrono::months std_months(ms);
          etl::chrono::months months(ms);

          std_month = std_month - std_months;
          month     = month - months;

          CHECK_EQUAL(std_month.ok(), month.ok());
          CHECK_EQUAL(unsigned(std_month), unsigned(month));
        }
      }
    }

    //*************************************************************************
    TEST(test_month_minus_month)
    {
      for (int m = 0; m < 256; ++m)
      {
        std::chrono::month std_month1(m);
        std::chrono::month std_month2(255 - m);

        std::chrono::month month1(m);
        std::chrono::month month2(255 - m);

        auto std_months12 = std_month1 - std_month2;
        auto std_months21 = std_month2 - std_month1;

        auto months12 = month1 - month2;
        auto months21 = month2 - month1;

        CHECK_EQUAL(std_months12.count(), months12.count());
        CHECK_EQUAL(std_months21.count(), months21.count());
      }
    }

    //*************************************************************************
    TEST(test_min_max_month)
    {
      CHECK_EQUAL(1U, etl::chrono::month::min());
      CHECK_EQUAL(12U, etl::chrono::month::max());
    }

    //*************************************************************************
    TEST(test_literal_month)
    {
      using namespace etl::literals::chrono_literals;

      etl::chrono::month month1  = 1_month;
      etl::chrono::month month2  = 2_month;
      etl::chrono::month month3  = 3_month;
      etl::chrono::month month4  = 4_month;
      etl::chrono::month month5  = 5_month;
      etl::chrono::month month6  = 6_month;
      etl::chrono::month month7  = 7_month;
      etl::chrono::month month8  = 8_month;
      etl::chrono::month month9  = 9_month;
      etl::chrono::month month10 = 10_month;
      etl::chrono::month month11 = 11_month;
      etl::chrono::month month12 = 12_month;

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

    //*************************************************************************
    TEST(test_month_comparison_operators)
    {
        etl::chrono::month month1(1);
        etl::chrono::month month2(2);

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

    //*************************************************************************
    TEST(test_month_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 256; ++i)
      {
        hashes.push_back(etl::hash<etl::chrono::month>()(etl::chrono::month(i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(256U, hashes.size());
    }

    //*************************************************************************
    TEST(test_month_types)
    {
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::January),   static_cast<unsigned>(etl::chrono::January));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::February),  static_cast<unsigned>(etl::chrono::February));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::March),     static_cast<unsigned>(etl::chrono::March));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::April),     static_cast<unsigned>(etl::chrono::April));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::May),       static_cast<unsigned>(etl::chrono::May));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::June),      static_cast<unsigned>(etl::chrono::June));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::July),      static_cast<unsigned>(etl::chrono::July));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::August),    static_cast<unsigned>(etl::chrono::August));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::September), static_cast<unsigned>(etl::chrono::September));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::October),   static_cast<unsigned>(etl::chrono::October));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::November),  static_cast<unsigned>(etl::chrono::November));
      CHECK_EQUAL(static_cast<unsigned>(std::chrono::December),  static_cast<unsigned>(etl::chrono::December));
    }
  };
}

#endif