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
  SUITE(test_chrono_year_month_day_last)
  {
    //*************************************************************************
    TEST(test_constructor_in_range)
    {
      Chrono::year_month_day_last year_month_day_last_2000_january(Chrono::year(2000), Chrono::month_day_last(Chrono::January));
      Chrono::year_month_day_last year_month_day_last_2000_february(Chrono::year(2000), Chrono::month_day_last(Chrono::February));
      Chrono::year_month_day_last year_month_day_last_2001_february(Chrono::year(2001), Chrono::month_day_last(Chrono::February));
      Chrono::year_month_day_last year_month_day_last_2002_march(Chrono::year(2002), Chrono::month_day_last(Chrono::March));
      Chrono::year_month_day_last year_month_day_last_2003_april(Chrono::year(2003), Chrono::month_day_last(Chrono::April));
      Chrono::year_month_day_last year_month_day_last_2004_may(Chrono::year(2004), Chrono::month_day_last(Chrono::May));
      Chrono::year_month_day_last year_month_day_last_2005_june(Chrono::year(2005), Chrono::month_day_last(Chrono::June));
      Chrono::year_month_day_last year_month_day_last_2006_july(Chrono::year(2006), Chrono::month_day_last(Chrono::July));
      Chrono::year_month_day_last year_month_day_last_2007_august(Chrono::year(2007), Chrono::month_day_last(Chrono::August));
      Chrono::year_month_day_last year_month_day_last_2008_september(Chrono::year(2008), Chrono::month_day_last(Chrono::September));
      Chrono::year_month_day_last year_month_day_last_2009_october(Chrono::year(2009), Chrono::month_day_last(Chrono::October));
      Chrono::year_month_day_last year_month_day_last_2010_november(Chrono::year(2010), Chrono::month_day_last(Chrono::November));
      Chrono::year_month_day_last year_month_day_last_2011_december(Chrono::year(2011), Chrono::month_day_last(Chrono::December));

      CHECK_TRUE(year_month_day_last_2000_january.ok());
      CHECK_TRUE(year_month_day_last_2000_february.ok());
      CHECK_TRUE(year_month_day_last_2001_february.ok());
      CHECK_TRUE(year_month_day_last_2002_march.ok());
      CHECK_TRUE(year_month_day_last_2003_april.ok());
      CHECK_TRUE(year_month_day_last_2004_may.ok());
      CHECK_TRUE(year_month_day_last_2005_june.ok());
      CHECK_TRUE(year_month_day_last_2006_july.ok());
      CHECK_TRUE(year_month_day_last_2007_august.ok());
      CHECK_TRUE(year_month_day_last_2008_september.ok());
      CHECK_TRUE(year_month_day_last_2009_october.ok());
      CHECK_TRUE(year_month_day_last_2010_november.ok());
      CHECK_TRUE(year_month_day_last_2011_december.ok());

      CHECK_EQUAL(31, (unsigned)year_month_day_last_2000_january.day());
      CHECK_EQUAL(29, (unsigned)year_month_day_last_2000_february.day());
      CHECK_EQUAL(28, (unsigned)year_month_day_last_2001_february.day());
      CHECK_EQUAL(31, (unsigned)year_month_day_last_2002_march.day());
      CHECK_EQUAL(30, (unsigned)year_month_day_last_2003_april.day());
      CHECK_EQUAL(31, (unsigned)year_month_day_last_2004_may.day());
      CHECK_EQUAL(30, (unsigned)year_month_day_last_2005_june.day());
      CHECK_EQUAL(31, (unsigned)year_month_day_last_2006_july.day());
      CHECK_EQUAL(31, (unsigned)year_month_day_last_2007_august.day());
      CHECK_EQUAL(30, (unsigned)year_month_day_last_2008_september.day());
      CHECK_EQUAL(31, (unsigned)year_month_day_last_2009_october.day());
      CHECK_EQUAL(30, (unsigned)year_month_day_last_2010_november.day());
      CHECK_EQUAL(31, (unsigned)year_month_day_last_2011_december.day());
    }

    //*************************************************************************
    TEST(test_year_month_day_last_member_arithmetic_operators)
    {
      Chrono::year_month_day_last ymdl(Chrono::year(2000), Chrono::month_day_last(Chrono::January));

      ymdl += Chrono::years(10);
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2010), Chrono::month_day_last(Chrono::January)) == ymdl);
      
      ymdl -= Chrono::years(5);
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2005), Chrono::month_day_last(Chrono::January)) == ymdl);
      
      ymdl += Chrono::months(10);
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2005), Chrono::month_day_last(Chrono::November)) == ymdl);
      
      ymdl -= Chrono::months(5);
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2005), Chrono::month_day_last(Chrono::June)) == ymdl);
    }

    //*************************************************************************
    TEST(test_year_month_day_last_non_member_arithmetic_operators)
    {
      Chrono::year_month_day_last ymdl(Chrono::year(2000), Chrono::month_day_last(Chrono::January));

      ymdl = ymdl + Chrono::years(10);
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2010), Chrono::month_day_last(Chrono::January)) == ymdl);

      ymdl = Chrono::years(10) + ymdl;
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2020), Chrono::month_day_last(Chrono::January)) == ymdl);

      ymdl = ymdl - Chrono::years(5);
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2015), Chrono::month_day_last(Chrono::January)) == ymdl);

      ymdl = ymdl + Chrono::months(3);
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2015), Chrono::month_day_last(Chrono::April)) == ymdl);

      ymdl = Chrono::months(3) + ymdl;
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2015), Chrono::month_day_last(Chrono::July)) == ymdl);

      ymdl = ymdl - Chrono::months(5);
      CHECK_TRUE(Chrono::year_month_day_last(Chrono::year(2015), Chrono::month_day_last(Chrono::February)) == ymdl);
    }

    //*************************************************************************
    TEST(test_year_month_day_last_comparison_operators)
    {
      Chrono::year_month_day_last year_month_day_last1(Chrono::year(2000), Chrono::month_day_last(Chrono::January));
      Chrono::year_month_day_last year_month_day_last2(Chrono::year(2001), Chrono::month_day_last(Chrono::January));
      Chrono::year_month_day_last year_month_day_last3(Chrono::year(2000), Chrono::month_day_last(Chrono::February));

      CHECK_TRUE(year_month_day_last1  == year_month_day_last1);
      CHECK_FALSE(year_month_day_last1 == year_month_day_last2);
      CHECK_FALSE(year_month_day_last1 == year_month_day_last3);

      CHECK_FALSE(year_month_day_last1 != year_month_day_last1);
      CHECK_TRUE(year_month_day_last1 != year_month_day_last2);
      CHECK_TRUE(year_month_day_last1  != year_month_day_last3);
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_year_month_day_last_hashes_are_unique)
    {
      std::vector<size_t> hashes;

      for (int i = 0; i < 6; ++i)
      {
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2000), Chrono::month_day_last(Chrono::January))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2001), Chrono::month_day_last(Chrono::February))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2002), Chrono::month_day_last(Chrono::March))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2003), Chrono::month_day_last(Chrono::April))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2004), Chrono::month_day_last(Chrono::May))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2005), Chrono::month_day_last(Chrono::January))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2006), Chrono::month_day_last(Chrono::January))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2007), Chrono::month_day_last(Chrono::January))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2008), Chrono::month_day_last(Chrono::January))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2009), Chrono::month_day_last(Chrono::January))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2010), Chrono::month_day_last(Chrono::January))));
        hashes.push_back(etl::hash<Chrono::year_month_day_last>()(Chrono::year_month_day_last(Chrono::year(2011), Chrono::month_day_last(Chrono::January))));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
    }
#endif

    //*************************************************************************
    TEST(test_to_sys_days)
    {
      Chrono::year_month_day_last ymdl{Chrono::year(2000), Chrono::month_day_last(Chrono::February)};
      Chrono::sys_days sd = Chrono::sys_days(ymdl);

      CHECK_EQUAL(11016, sd.time_since_epoch().count());
    }
  };
}
