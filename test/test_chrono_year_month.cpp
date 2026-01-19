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
  SUITE(test_chrono_year_month)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Chrono::year_month ym;

      CHECK_FALSE(ym.ok()); // Default-constructed year_month is not valid
    }

    //*************************************************************************
    TEST(test_constructor_with_month_and_day)
    {
      Chrono::year_month ym{Chrono::year(2000), Chrono::January};

      CHECK_TRUE(ym.ok()); // Valid year_month
      CHECK_EQUAL(2000,            (int)ym.year());
      CHECK_EQUAL((unsigned)Chrono::January, (unsigned)ym.month());
    }

    //*************************************************************************
    TEST(test_invalid_year_month)
    {
      Chrono::year_month ym{Chrono::year{32768}, Chrono::January}; // Invalid year

      CHECK_FALSE(ym.ok()); // Invalid year_month
    }

    //*************************************************************************
    TEST(test_invalid_month_in_year_month)
    {
      Chrono::year_month ym{Chrono::year{2000}, Chrono::month{13}}; // Invalid month (13)

      CHECK_FALSE(ym.ok()); // Invalid year_month
    }

#if ETL_USING_CPP20
    //*************************************************************************
    TEST(test_year_month_spaceship_operator)
    {
      Chrono::year_month ym1{Chrono::year(2000), Chrono::January};
      Chrono::year_month ym2{Chrono::year(2001), Chrono::January};
      Chrono::year_month ym3{Chrono::year(2000), Chrono::February};

      CHECK_TRUE((ym1 <=> ym1) == std::strong_ordering::equal);
      CHECK_TRUE((ym1 <=> ym2) == std::strong_ordering::less);
      CHECK_TRUE((ym2 <=> ym1) == std::strong_ordering::greater);
      CHECK_TRUE((ym1 <=> ym3) == std::strong_ordering::less);
      CHECK_TRUE((ym3 <=> ym1) == std::strong_ordering::greater);
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_year_month_compare)
    {
      Chrono::year_month ym1{Chrono::year(2000), Chrono::January};
      Chrono::year_month ym2{Chrono::year(2001), Chrono::January};
      Chrono::year_month ym3{Chrono::year(2000), Chrono::February};

      CHECK_TRUE(ym1.compare(ym1) == 0);
      CHECK_TRUE(ym1.compare(ym2) == -1);
      CHECK_TRUE(ym2.compare(ym1) == 1);
      CHECK_TRUE(ym1.compare(ym3) == -1);
      CHECK_TRUE(ym3.compare(ym1) == 1);
    }
#endif

    //*************************************************************************
    TEST(test_year_month_equality_operator)
    {
      Chrono::year_month ym1{Chrono::year(2000), Chrono::January};
      Chrono::year_month ym2{Chrono::year(2001), Chrono::January};
      Chrono::year_month ym3{Chrono::year(2000), Chrono::February};

      CHECK_TRUE(ym1 == ym1);  // Same year/month
      CHECK_FALSE(ym1 == ym2); // Different year
      CHECK_FALSE(ym1 == ym3); // Different month
    }

    //*************************************************************************
    TEST(test_year_month_not_equality_operator)
    {
      Chrono::year_month ym1{Chrono::year(2000), Chrono::January};
      Chrono::year_month ym2{Chrono::year(2001), Chrono::January};
      Chrono::year_month ym3{Chrono::year(2000), Chrono::February};

      CHECK_FALSE(ym1 != ym1); // Same year/month
      CHECK_TRUE(ym1 != ym2);  // Different year
      CHECK_TRUE(ym1 != ym3);  // Different month
    }

    //*************************************************************************
    TEST(test_year_month_relational_operators)
    {
      Chrono::year_month ym1(Chrono::year(2021), Chrono::January);
      CHECK_FALSE(ym1 < ym1); // Same year/month
      CHECK_TRUE(ym1 <= ym1);
      CHECK_FALSE(ym1 > ym1);
      CHECK_TRUE(ym1 >= ym1);

      Chrono::year_month ym2(Chrono::year(2026), Chrono::December);
      CHECK_TRUE(ym1 < ym2); // left year/month strict less
      CHECK_TRUE(ym1 <= ym2);
      CHECK_FALSE(ym1 > ym2);
      CHECK_FALSE(ym1 >= ym2);

      CHECK_FALSE(ym2 < ym1); // left year/month strict greater
      CHECK_FALSE(ym2 <= ym1);
      CHECK_TRUE(ym2 > ym1);
      CHECK_TRUE(ym2 >= ym1);
    }

    //*************************************************************************
    TEST(test_year_month_year_month_diff_operator)
    {
      Chrono::year_month ym1(Chrono::year(2021), Chrono::January);
      Chrono::months dms = ym1 - ym1;
      CHECK_EQUAL(dms.count(), 0);
      Chrono::year_month ym2(Chrono::year(2026), Chrono::December);
      dms = ym2 - ym1; // positive
      CHECK_EQUAL(dms.count(), (2026 - 2021) * 12 + 11);
      dms = ym1 - ym2; // negative
      CHECK_EQUAL(dms.count(), (2021 - 2026) * 12 - 11);
    }

    //*************************************************************************
    TEST(test_year_month_add_sub_months_operators)
    {
      Chrono::year_month ym1(Chrono::year(2021), Chrono::January);
      Chrono::months dms(0); // zero
      CHECK_TRUE(dms + ym1 == ym1);
      CHECK_TRUE(ym1 + dms == ym1);
      CHECK_TRUE(ym1 - dms == ym1);
      Chrono::year_month ym2(Chrono::year(2026), Chrono::December);
      dms = ym1 - ym2; // negative
      CHECK_TRUE(dms + ym2 == ym1);
      CHECK_TRUE(ym2 + dms == ym1);
      CHECK_TRUE(ym1 - dms == ym2);
      dms = ym2 - ym1; // positive
      CHECK_TRUE(dms + ym1 == ym2);
      CHECK_TRUE(ym1 + dms == ym2);
      CHECK_TRUE(ym2 - dms == ym1);
    }

    //*************************************************************************
    TEST(test_year_month_add_sub_years_operator)
    {
      Chrono::year_month ym(Chrono::year(2021), Chrono::January);
      Chrono::years dys(0); // zero
      CHECK_TRUE((ym + dys == Chrono::year_month(ym.year() + dys, ym.month())));
      CHECK_TRUE((dys + ym == Chrono::year_month(ym.year() + dys, ym.month())));
      CHECK_TRUE((ym - dys == Chrono::year_month(ym.year() - dys, ym.month())));
      dys = Chrono::years(-200); // negative
      CHECK_TRUE((ym + dys == Chrono::year_month(ym.year() + dys, ym.month())));
      CHECK_TRUE((dys + ym == Chrono::year_month(ym.year() + dys, ym.month())));
      CHECK_TRUE((ym - dys == Chrono::year_month(ym.year() - dys, ym.month())));
      dys = Chrono::years(300); // positive
      CHECK_TRUE((ym + dys == Chrono::year_month(ym.year() + dys, ym.month())));
      CHECK_TRUE((dys + ym == Chrono::year_month(ym.year() + dys, ym.month())));
      CHECK_TRUE((ym - dys == Chrono::year_month(ym.year() - dys, ym.month())));
    }
  }
}
