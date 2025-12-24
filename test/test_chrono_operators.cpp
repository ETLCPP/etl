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
#include <algorithm>

using namespace etl::literals::chrono_literals;

namespace
{
  SUITE(test_chrono_operators)
  {
    //*************************************************************************
    TEST(test_construction_operator_for_month_day)
    {
      etl::chrono::month_day md1 = etl::chrono::January / etl::chrono::day(2);
      etl::chrono::month_day md2 = etl::chrono::February / 3;
      etl::chrono::month_day md3 = 3 / etl::chrono::day(4);
      etl::chrono::month_day md4 = etl::chrono::day(5) / etl::chrono::April;
      etl::chrono::month_day md5 = etl::chrono::day(6) / 5;

      CHECK_EQUAL(etl::chrono::January, md1.month());
      CHECK_EQUAL(etl::chrono::day(2),  md1.day());

      CHECK_EQUAL(etl::chrono::February, md2.month());
      CHECK_EQUAL(etl::chrono::day(3),   md2.day());

      CHECK_EQUAL(etl::chrono::March,  md3.month());
      CHECK_EQUAL(etl::chrono::day(4), md3.day());

      CHECK_EQUAL(etl::chrono::April,  md4.month());
      CHECK_EQUAL(etl::chrono::day(5), md4.day());

      CHECK_EQUAL(etl::chrono::May,    md5.month());
      CHECK_EQUAL(etl::chrono::day(6), md5.day());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_month_day_last)
    {
      etl::chrono::month_day_last mdl1 = etl::chrono::January / etl::chrono::last;
      etl::chrono::month_day_last mdl2 = 2 / etl::chrono::last;
      etl::chrono::month_day_last mdl3 = etl::chrono::last / etl::chrono::March;
      etl::chrono::month_day_last mdl4 = etl::chrono::last / 4;

      CHECK_EQUAL(etl::chrono::January,  mdl1.month());
      CHECK_EQUAL(etl::chrono::February, mdl2.month());
      CHECK_EQUAL(etl::chrono::March,    mdl3.month());
      CHECK_EQUAL(etl::chrono::April,    mdl4.month());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_month_weekday)
    {
      etl::chrono::month_weekday mwd1 = etl::chrono::January / etl::chrono::weekday_indexed(etl::chrono::Monday, 0);
      etl::chrono::month_weekday mwd2 = 2 / etl::chrono::weekday_indexed(etl::chrono::Wednesday, 1);
      etl::chrono::month_weekday mwd3 = etl::chrono::weekday_indexed(etl::chrono::Friday, 2) / etl::chrono::March;
      etl::chrono::month_weekday mwd4 = etl::chrono::weekday_indexed(etl::chrono::Sunday, 3) / 4;

      CHECK_EQUAL(etl::chrono::January, mwd1.month());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Monday, 0).weekday().c_encoding(),
                  mwd1.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Monday, 0).index(),
                  mwd1.weekday_indexed().index());

      CHECK_EQUAL(etl::chrono::February, mwd2.month());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Wednesday, 1).weekday().c_encoding(), 
                  mwd2.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Wednesday, 1).index(), 
                  mwd2.weekday_indexed().index());


      CHECK_EQUAL(etl::chrono::March,  mwd3.month());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Friday, 2).weekday().c_encoding(), 
                  mwd3.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Friday, 2).index(), 
                  mwd3.weekday_indexed().index());

      CHECK_EQUAL(etl::chrono::April,  mwd4.month());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Sunday, 3).weekday().c_encoding(), 
                  mwd4.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Sunday, 3).index(), 
                  mwd4.weekday_indexed().index());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_month_weekday_last)
    {
      etl::chrono::month_weekday_last mdl1 = etl::chrono::January / etl::chrono::weekday_last(etl::chrono::Monday);
      etl::chrono::month_weekday_last mdl2 = 2 / etl::chrono::weekday_last(etl::chrono::Tuesday);
      etl::chrono::month_weekday_last mdl3 = etl::chrono::weekday_last(etl::chrono::Wednesday) / etl::chrono::March;
      etl::chrono::month_weekday_last mdl4 = etl::chrono::weekday_last(etl::chrono::Thursday) / 4;

      CHECK_EQUAL(etl::chrono::January,                mdl1.month());
      CHECK_EQUAL(etl::chrono::Monday.c_encoding(),    mdl1.weekday_last().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::February,               mdl2.month());
      CHECK_EQUAL(etl::chrono::Tuesday.c_encoding(),   mdl2.weekday_last().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::March,                  mdl3.month());
      CHECK_EQUAL(etl::chrono::Wednesday.c_encoding(), mdl3.weekday_last().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::April,                  mdl4.month());
      CHECK_EQUAL(etl::chrono::Thursday.c_encoding(),  mdl4.weekday_last().weekday().c_encoding());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_year_month)
    {
      etl::chrono::year_month ym1 = etl::chrono::year(2000) / etl::chrono::April;
      etl::chrono::year_month ym2 = etl::chrono::year(2001) / 5;

      CHECK_EQUAL(etl::chrono::year(2000), ym1.year());
      CHECK_EQUAL(etl::chrono::April, ym1.month());

      CHECK_EQUAL(etl::chrono::year(2001), ym2.year());
      CHECK_EQUAL(etl::chrono::May, ym2.month());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_year_month_day)
    {
      etl::chrono::year_month_day ymd1 = etl::chrono::year_month(etl::chrono::year(2000), etl::chrono::January) / etl::chrono::day(1);
      etl::chrono::year_month_day ymd2 = etl::chrono::year_month(etl::chrono::year(2001), etl::chrono::February) / 2;
      etl::chrono::year_month_day ymd3 = etl::chrono::year(2002) / etl::chrono::month_day(etl::chrono::March, etl::chrono::day(3));
      etl::chrono::year_month_day ymd4 = 2003 / etl::chrono::month_day(etl::chrono::April, etl::chrono::day(4));
      etl::chrono::year_month_day ymd5 = etl::chrono::month_day(etl::chrono::May, etl::chrono::day(5)) / etl::chrono::year(2004);
      etl::chrono::year_month_day ymd6 = etl::chrono::month_day(etl::chrono::June, etl::chrono::day(6)) / 2005;

      CHECK_EQUAL(etl::chrono::year(2000), ymd1.year());
      CHECK_EQUAL(etl::chrono::January,    ymd1.month());
      CHECK_EQUAL(etl::chrono::day(1),     ymd1.day());

      CHECK_EQUAL(etl::chrono::year(2001), ymd2.year());
      CHECK_EQUAL(etl::chrono::February,   ymd2.month());
      CHECK_EQUAL(etl::chrono::day(2),     ymd2.day());

      CHECK_EQUAL(etl::chrono::year(2002), ymd3.year());
      CHECK_EQUAL(etl::chrono::March,      ymd3.month());
      CHECK_EQUAL(etl::chrono::day(3),     ymd3.day());

      CHECK_EQUAL(etl::chrono::year(2003), ymd4.year());
      CHECK_EQUAL(etl::chrono::April,      ymd4.month());
      CHECK_EQUAL(etl::chrono::day(4),     ymd4.day());

      CHECK_EQUAL(etl::chrono::year(2004), ymd5.year());
      CHECK_EQUAL(etl::chrono::May,        ymd5.month());
      CHECK_EQUAL(etl::chrono::day(5),     ymd5.day());

      CHECK_EQUAL(etl::chrono::year(2005), ymd6.year());
      CHECK_EQUAL(etl::chrono::June,       ymd6.month());
      CHECK_EQUAL(etl::chrono::day(6),     ymd6.day());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_year_month_day_last)
    {
      etl::chrono::year_month_day_last ymdl1 = etl::chrono::year_month(etl::chrono::year(2000), etl::chrono::January) / etl::chrono::last;
      etl::chrono::year_month_day_last ymdl2 = etl::chrono::year(2001) / etl::chrono::month_day_last(etl::chrono::February);
      etl::chrono::year_month_day_last ymdl3 = 2002 / etl::chrono::month_day_last(etl::chrono::March);
      etl::chrono::year_month_day_last ymdl4 = etl::chrono::month_day_last(etl::chrono::April) / etl::chrono::year(2003);
      etl::chrono::year_month_day_last ymdl5 = etl::chrono::month_day_last(etl::chrono::May) / 2004;

      CHECK_EQUAL(etl::chrono::year(2000), ymdl1.year());
      CHECK_EQUAL(etl::chrono::January,    ymdl1.month());
      CHECK_EQUAL(etl::chrono::month_day_last(etl::chrono::January).month(), ymdl1.month_day_last().month());

      CHECK_EQUAL(etl::chrono::year(2001), ymdl2.year());
      CHECK_EQUAL(etl::chrono::February,   ymdl2.month());
      CHECK_EQUAL(etl::chrono::month_day_last(etl::chrono::February).month(), ymdl2.month_day_last().month());

      CHECK_EQUAL(etl::chrono::year(2002), ymdl3.year());
      CHECK_EQUAL(etl::chrono::March,      ymdl3.month());
      CHECK_EQUAL(etl::chrono::month_day_last(etl::chrono::March).month(), ymdl3.month_day_last().month());

      CHECK_EQUAL(etl::chrono::year(2003), ymdl4.year());
      CHECK_EQUAL(etl::chrono::April,      ymdl4.month());
      CHECK_EQUAL(etl::chrono::month_day_last(etl::chrono::April).month(), ymdl4.month_day_last().month());

      CHECK_EQUAL(etl::chrono::year(2004), ymdl5.year());
      CHECK_EQUAL(etl::chrono::May,        ymdl5.month());
      CHECK_EQUAL(etl::chrono::month_day_last(etl::chrono::May).month(), ymdl5.month_day_last().month());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_year_month_weekday)
    {
      etl::chrono::year_month_weekday ymwd1 = etl::chrono::year_month(etl::chrono::year(2000), etl::chrono::January) / etl::chrono::weekday_indexed(etl::chrono::Monday, 0);
      etl::chrono::year_month_weekday ymwd2 = etl::chrono::year(2001) / etl::chrono::month_weekday(etl::chrono::February, etl::chrono::weekday_indexed(etl::chrono::Tuesday, 1));
      etl::chrono::year_month_weekday ymwd3 = 2002 / etl::chrono::month_weekday(etl::chrono::March, etl::chrono::weekday_indexed(etl::chrono::Wednesday, 2));
      etl::chrono::year_month_weekday ymwd4 = etl::chrono::month_weekday(etl::chrono::April, etl::chrono::weekday_indexed(etl::chrono::Thursday, 3)) / etl::chrono::year(2003);
      etl::chrono::year_month_weekday ymwd5 = etl::chrono::month_weekday(etl::chrono::May, etl::chrono::weekday_indexed(etl::chrono::Friday, 4)) / 2004;

      CHECK_EQUAL(etl::chrono::year(2000), ymwd1.year());
      CHECK_EQUAL(etl::chrono::January,    ymwd1.month());
      CHECK_EQUAL(etl::chrono::Monday.c_encoding(), ymwd1.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Monday.c_encoding(), ymwd1.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(0, ymwd1.weekday_indexed().index());

      CHECK_EQUAL(etl::chrono::year(2001), ymwd2.year());
      CHECK_EQUAL(etl::chrono::February,   ymwd2.month());
      CHECK_EQUAL(etl::chrono::Tuesday.c_encoding(), ymwd2.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Tuesday.c_encoding(), ymwd2.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(1, ymwd2.weekday_indexed().index());

      CHECK_EQUAL(etl::chrono::year(2002), ymwd3.year());
      CHECK_EQUAL(etl::chrono::March,      ymwd3.month());
      CHECK_EQUAL(etl::chrono::Wednesday.c_encoding(), ymwd3.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Wednesday.c_encoding(), ymwd3.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(2, ymwd3.weekday_indexed().index());

      CHECK_EQUAL(etl::chrono::year(2003), ymwd4.year());
      CHECK_EQUAL(etl::chrono::April,      ymwd4.month());
      CHECK_EQUAL(etl::chrono::Thursday.c_encoding(), ymwd4.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Thursday.c_encoding(), ymwd4.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(3, ymwd4.weekday_indexed().index());

      CHECK_EQUAL(etl::chrono::year(2004), ymwd5.year());
      CHECK_EQUAL(etl::chrono::May,        ymwd5.month());
      CHECK_EQUAL(etl::chrono::Friday.c_encoding(), ymwd5.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Friday.c_encoding(), ymwd5.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(4, ymwd5.weekday_indexed().index());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_year_month_weekday_last)
    {
      etl::chrono::year_month_weekday_last ymdl1 = etl::chrono::year_month(etl::chrono::year(2000), etl::chrono::January) / etl::chrono::weekday_last(etl::chrono::Monday);
      etl::chrono::year_month_weekday_last ymdl2 = etl::chrono::year(2001) / etl::chrono::month_weekday_last(etl::chrono::February, etl::chrono::weekday_last(etl::chrono::Tuesday));
      etl::chrono::year_month_weekday_last ymdl3 = 2002 / etl::chrono::month_weekday_last(etl::chrono::March, etl::chrono::weekday_last(etl::chrono::Wednesday));
      etl::chrono::year_month_weekday_last ymdl4 = etl::chrono::month_weekday_last(etl::chrono::April, etl::chrono::weekday_last(etl::chrono::Thursday)) / etl::chrono::year(2003);
      etl::chrono::year_month_weekday_last ymdl5 = etl::chrono::month_weekday_last(etl::chrono::May, etl::chrono::weekday_last(etl::chrono::Friday))/ 2004;

      CHECK_EQUAL(etl::chrono::year(2000), ymdl1.year());
      CHECK_EQUAL(etl::chrono::January,    ymdl1.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Monday).weekday().c_encoding(), ymdl1.weekday().c_encoding());

      CHECK_EQUAL(etl::chrono::year(2001), ymdl2.year());
      CHECK_EQUAL(etl::chrono::February,    ymdl2.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Tuesday).weekday().c_encoding(), ymdl2.weekday().c_encoding());

      CHECK_EQUAL(etl::chrono::year(2002), ymdl3.year());
      CHECK_EQUAL(etl::chrono::March,    ymdl3.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Wednesday).weekday().c_encoding(), ymdl3.weekday().c_encoding());

      CHECK_EQUAL(etl::chrono::year(2003), ymdl4.year());
      CHECK_EQUAL(etl::chrono::April,    ymdl4.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Thursday).weekday().c_encoding(), ymdl4.weekday().c_encoding());

      CHECK_EQUAL(etl::chrono::year(2004), ymdl5.year());
      CHECK_EQUAL(etl::chrono::May,    ymdl5.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Friday).weekday().c_encoding(), ymdl5.weekday().c_encoding());
    }
  };
}