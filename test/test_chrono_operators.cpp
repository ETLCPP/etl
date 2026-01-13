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

      CHECK_EQUAL((unsigned)etl::chrono::January, (unsigned)md1.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(2),  (unsigned)md1.day());

      CHECK_EQUAL((unsigned)etl::chrono::February, (unsigned)md2.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(3),   (unsigned)md2.day());

      CHECK_EQUAL((unsigned)etl::chrono::March,  (unsigned)md3.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(4), (unsigned)md3.day());

      CHECK_EQUAL((unsigned)etl::chrono::April,  (unsigned)md4.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(5), (unsigned)md4.day());

      CHECK_EQUAL((unsigned)etl::chrono::May,    (unsigned)md5.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(6), (unsigned)md5.day());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_month_day_last)
    {
      etl::chrono::month_day_last mdl1 = etl::chrono::January / etl::chrono::last;
      etl::chrono::month_day_last mdl2 = 2 / etl::chrono::last;
      etl::chrono::month_day_last mdl3 = etl::chrono::last / etl::chrono::March;
      etl::chrono::month_day_last mdl4 = etl::chrono::last / 4;

      CHECK_EQUAL((unsigned)etl::chrono::January,  (unsigned)mdl1.month());
      CHECK_EQUAL((unsigned)etl::chrono::February, (unsigned)mdl2.month());
      CHECK_EQUAL((unsigned)etl::chrono::March,    (unsigned)mdl3.month());
      CHECK_EQUAL((unsigned)etl::chrono::April,    (unsigned)mdl4.month());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_month_weekday)
    {
      etl::chrono::month_weekday mwd1 = etl::chrono::January / etl::chrono::weekday_indexed(etl::chrono::Monday, 0);
      etl::chrono::month_weekday mwd2 = 2 / etl::chrono::weekday_indexed(etl::chrono::Wednesday, 1);
      etl::chrono::month_weekday mwd3 = etl::chrono::weekday_indexed(etl::chrono::Friday, 2) / etl::chrono::March;
      etl::chrono::month_weekday mwd4 = etl::chrono::weekday_indexed(etl::chrono::Sunday, 3) / 4;

      CHECK_EQUAL((unsigned)etl::chrono::January, (unsigned)mwd1.month());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Monday, 0).weekday().c_encoding(),
                  mwd1.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Monday, 0).index(),
                  mwd1.weekday_indexed().index());

      CHECK_EQUAL((unsigned)etl::chrono::February, (unsigned)mwd2.month());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Wednesday, 1).weekday().c_encoding(), 
                  mwd2.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Wednesday, 1).index(), 
                  mwd2.weekday_indexed().index());


      CHECK_EQUAL((unsigned)etl::chrono::March,  (unsigned)mwd3.month());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Friday, 2).weekday().c_encoding(), 
                  mwd3.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::weekday_indexed(etl::chrono::Friday, 2).index(), 
                  mwd3.weekday_indexed().index());

      CHECK_EQUAL((unsigned)etl::chrono::April,  (unsigned)mwd4.month());
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

      CHECK_EQUAL((unsigned)etl::chrono::January,      (unsigned)mdl1.month());
      CHECK_EQUAL(etl::chrono::Monday.c_encoding(),    mdl1.weekday_last().weekday().c_encoding());
      CHECK_EQUAL((unsigned)etl::chrono::February,     (unsigned)mdl2.month());
      CHECK_EQUAL(etl::chrono::Tuesday.c_encoding(),   mdl2.weekday_last().weekday().c_encoding());
      CHECK_EQUAL((unsigned)etl::chrono::March,        (unsigned)mdl3.month());
      CHECK_EQUAL(etl::chrono::Wednesday.c_encoding(), mdl3.weekday_last().weekday().c_encoding());
      CHECK_EQUAL((unsigned)etl::chrono::April,        (unsigned)mdl4.month());
      CHECK_EQUAL(etl::chrono::Thursday.c_encoding(),  mdl4.weekday_last().weekday().c_encoding());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_year_month)
    {
      etl::chrono::year_month ym1 = etl::chrono::year(2000) / etl::chrono::April;
      etl::chrono::year_month ym2 = etl::chrono::year(2001) / 5;

      CHECK_EQUAL((int)etl::chrono::year(2000), (int)ym1.year());
      CHECK_EQUAL((unsigned)etl::chrono::April, (unsigned)ym1.month());

      CHECK_EQUAL((int)etl::chrono::year(2001), (int)ym2.year());
      CHECK_EQUAL((unsigned)etl::chrono::May,   (unsigned)ym2.month());
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

      CHECK_EQUAL((int)etl::chrono::year(2000),      (int)ymd1.year());
      CHECK_EQUAL((unsigned)etl::chrono::January,    (unsigned)ymd1.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(1),     (unsigned)ymd1.day());

      CHECK_EQUAL((int)etl::chrono::year(2001),      (int)ymd2.year());
      CHECK_EQUAL((unsigned)etl::chrono::February,   (unsigned)ymd2.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(2),     (unsigned)ymd2.day());

      CHECK_EQUAL((int)etl::chrono::year(2002),      (int)ymd3.year());
      CHECK_EQUAL((unsigned)etl::chrono::March,      (unsigned)ymd3.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(3),     (unsigned)ymd3.day());

      CHECK_EQUAL((int)etl::chrono::year(2003),      (int)ymd4.year());
      CHECK_EQUAL((unsigned)etl::chrono::April,      (unsigned)ymd4.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(4),     (unsigned)ymd4.day());

      CHECK_EQUAL((int)etl::chrono::year(2004),      (int)ymd5.year());
      CHECK_EQUAL((unsigned)etl::chrono::May,        (unsigned)ymd5.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(5),     (unsigned)ymd5.day());

      CHECK_EQUAL((int)etl::chrono::year(2005),      (int)ymd6.year());
      CHECK_EQUAL((unsigned)etl::chrono::June,       (unsigned)ymd6.month());
      CHECK_EQUAL((unsigned)etl::chrono::day(6),     (unsigned)ymd6.day());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_year_month_day_last)
    {
      etl::chrono::year_month_day_last ymdl1 = etl::chrono::year_month(etl::chrono::year(2000), etl::chrono::January) / etl::chrono::last;
      etl::chrono::year_month_day_last ymdl2 = etl::chrono::year(2001) / etl::chrono::month_day_last(etl::chrono::February);
      etl::chrono::year_month_day_last ymdl3 = 2002 / etl::chrono::month_day_last(etl::chrono::March);
      etl::chrono::year_month_day_last ymdl4 = etl::chrono::month_day_last(etl::chrono::April) / etl::chrono::year(2003);
      etl::chrono::year_month_day_last ymdl5 = etl::chrono::month_day_last(etl::chrono::May) / 2004;

      CHECK_EQUAL((int)etl::chrono::year(2000), (int)ymdl1.year());
      CHECK_EQUAL((unsigned)etl::chrono::January, (unsigned)ymdl1.month());
      CHECK_EQUAL((unsigned)etl::chrono::month_day_last(etl::chrono::January).month(), (unsigned)ymdl1.month_day_last().month());

      CHECK_EQUAL((int)etl::chrono::year(2001), (int)ymdl2.year());
      CHECK_EQUAL((unsigned)etl::chrono::February, (unsigned)ymdl2.month());
      CHECK_EQUAL((unsigned)etl::chrono::month_day_last(etl::chrono::February).month(), (unsigned)ymdl2.month_day_last().month());

      CHECK_EQUAL((int)etl::chrono::year(2002), (int)ymdl3.year());
      CHECK_EQUAL((unsigned)etl::chrono::March,      (unsigned)ymdl3.month());
      CHECK_EQUAL((unsigned)etl::chrono::month_day_last(etl::chrono::March).month(), (unsigned)ymdl3.month_day_last().month());

      CHECK_EQUAL((int)etl::chrono::year(2003), (int)ymdl4.year());
      CHECK_EQUAL((unsigned)etl::chrono::April,      (unsigned)ymdl4.month());
      CHECK_EQUAL((unsigned)etl::chrono::month_day_last(etl::chrono::April).month(), (unsigned)ymdl4.month_day_last().month());

      CHECK_EQUAL((int)etl::chrono::year(2004), (int)ymdl5.year());
      CHECK_EQUAL((unsigned)etl::chrono::May,        (unsigned)ymdl5.month());
      CHECK_EQUAL((unsigned)etl::chrono::month_day_last(etl::chrono::May).month(), (unsigned)ymdl5.month_day_last().month());
    }

    //*************************************************************************
    TEST(test_construction_operator_for_year_month_weekday)
    {
      etl::chrono::year_month_weekday ymwd1 = etl::chrono::year_month(etl::chrono::year(2000), etl::chrono::January) / etl::chrono::weekday_indexed(etl::chrono::Monday, 0);
      etl::chrono::year_month_weekday ymwd2 = etl::chrono::year(2001) / etl::chrono::month_weekday(etl::chrono::February, etl::chrono::weekday_indexed(etl::chrono::Tuesday, 1));
      etl::chrono::year_month_weekday ymwd3 = 2002 / etl::chrono::month_weekday(etl::chrono::March, etl::chrono::weekday_indexed(etl::chrono::Wednesday, 2));
      etl::chrono::year_month_weekday ymwd4 = etl::chrono::month_weekday(etl::chrono::April, etl::chrono::weekday_indexed(etl::chrono::Thursday, 3)) / etl::chrono::year(2003);
      etl::chrono::year_month_weekday ymwd5 = etl::chrono::month_weekday(etl::chrono::May, etl::chrono::weekday_indexed(etl::chrono::Friday, 4)) / 2004;

      CHECK_EQUAL((int)etl::chrono::year(2000),    (int)ymwd1.year());
      CHECK_EQUAL((unsigned)etl::chrono::January,  (unsigned)ymwd1.month());
      CHECK_EQUAL(etl::chrono::Monday.c_encoding(), ymwd1.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Monday.c_encoding(), ymwd1.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(0, ymwd1.weekday_indexed().index());

      CHECK_EQUAL((int)etl::chrono::year(2001),      (int)ymwd2.year());
      CHECK_EQUAL((unsigned)etl::chrono::February,   (unsigned)ymwd2.month());
      CHECK_EQUAL(etl::chrono::Tuesday.c_encoding(), ymwd2.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Tuesday.c_encoding(), ymwd2.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(1, ymwd2.weekday_indexed().index());

      CHECK_EQUAL((int)etl::chrono::year(2002),      (int)ymwd3.year());
      CHECK_EQUAL((unsigned)etl::chrono::March,      (unsigned)ymwd3.month());
      CHECK_EQUAL(etl::chrono::Wednesday.c_encoding(), ymwd3.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Wednesday.c_encoding(), ymwd3.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(2, ymwd3.weekday_indexed().index());

      CHECK_EQUAL((int)etl::chrono::year(2003),      (int)ymwd4.year());
      CHECK_EQUAL((unsigned)etl::chrono::April,      (unsigned)ymwd4.month());
      CHECK_EQUAL(etl::chrono::Thursday.c_encoding(), ymwd4.weekday().c_encoding());
      CHECK_EQUAL(etl::chrono::Thursday.c_encoding(), ymwd4.weekday_indexed().weekday().c_encoding());
      CHECK_EQUAL(3, ymwd4.weekday_indexed().index());

      CHECK_EQUAL((int)etl::chrono::year(2004),     (int)ymwd5.year());
      CHECK_EQUAL((unsigned)etl::chrono::May,       (unsigned)ymwd5.month());
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

      CHECK_EQUAL((int)etl::chrono::year(2000),   (int)ymdl1.year());
      CHECK_EQUAL((unsigned)etl::chrono::January, (unsigned)ymdl1.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Monday).weekday().c_encoding(), ymdl1.weekday().c_encoding());

      CHECK_EQUAL((int)etl::chrono::year(2001),    (int)ymdl2.year());
      CHECK_EQUAL((unsigned)etl::chrono::February, (unsigned)ymdl2.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Tuesday).weekday().c_encoding(), ymdl2.weekday().c_encoding());

      CHECK_EQUAL((int)etl::chrono::year(2002),    (int)ymdl3.year());
      CHECK_EQUAL((unsigned)etl::chrono::March,    (unsigned)ymdl3.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Wednesday).weekday().c_encoding(), ymdl3.weekday().c_encoding());

      CHECK_EQUAL((int)etl::chrono::year(2003),    (int)ymdl4.year());
      CHECK_EQUAL((unsigned)etl::chrono::April,    (unsigned)ymdl4.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Thursday).weekday().c_encoding(), ymdl4.weekday().c_encoding());

      CHECK_EQUAL((int)etl::chrono::year(2004),    (int)ymdl5.year());
      CHECK_EQUAL((unsigned)etl::chrono::May,      (unsigned)ymdl5.month());
      CHECK_EQUAL(etl::chrono::weekday_last(etl::chrono::Friday).weekday().c_encoding(), ymdl5.weekday().c_encoding());
    }
  }
}