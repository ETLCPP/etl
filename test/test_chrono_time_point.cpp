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
  struct test_clock
  {
    using duration = Chrono::hours;
  };

  using TimePoint = Chrono::time_point<test_clock>;

  SUITE(test_chrono_time_point)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      TimePoint tp;

      CHECK_EQUAL(0, tp.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_constructor_with_duration)
    {
      auto num_days  = Chrono::days::period::num;
      auto num_hours = Chrono::hours::period::num;

      for (int d = 1; d < 100; ++d)
      {
        TimePoint tp{ Chrono::days(d) };

        auto hours                = tp.time_since_epoch().count();
        auto hours_scaled_to_days = (hours * num_hours) / num_days;

        CHECK_EQUAL(d, hours_scaled_to_days);
      }
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      auto num_days  = Chrono::days::period::num;
      auto num_hours = Chrono::hours::period::num;

      for (int d = 1; d < 100; ++d)
      {
        TimePoint tp{ Chrono::days(d) };
        TimePoint tp2{ tp };

        auto hours                = tp2.time_since_epoch().count();
        auto hours_scaled_to_days = (hours * num_hours) / num_days;

        CHECK_EQUAL(d, hours_scaled_to_days);
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_days)
    {
      auto num_days  = Chrono::days::period::num;
      auto num_hours = Chrono::hours::period::num;

      Chrono::days ds(2);
      TimePoint tp{ Chrono::days(0) };

      for (int d = 1; d < 100; ++d)
      {
        tp += ds;

        auto hours                = tp.time_since_epoch().count();
        auto hours_scaled_to_days = (hours * num_hours) / num_days;

        CHECK_EQUAL(d * 2, hours_scaled_to_days);
      }
    }

    //*************************************************************************
    TEST(test_plus_equal_hours)
    {
      auto num_days  = Chrono::days::period::num;
      auto num_hours = Chrono::hours::period::num;
      
      Chrono::hours hs(48);
      TimePoint tp{ Chrono::days(0) };

      for (int d = 1; d < 100; ++d)
      {
        tp += hs;

        auto hours                = tp.time_since_epoch().count();
        auto hours_scaled_to_days = (hours * num_hours) / num_days;

        CHECK_EQUAL(d * 2, hours_scaled_to_days);
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_days)
    {
      auto num_days  = Chrono::days::period::num;
      auto num_hours = Chrono::hours::period::num;

      Chrono::days ds(2);
      TimePoint tp{ Chrono::days(100) };

      for (int d = 1; d < 100; ++d)
      {
        tp -= ds;

        auto hours                = tp.time_since_epoch().count();
        auto hours_scaled_to_days = (hours * num_hours) / num_days;

        CHECK_EQUAL(100 - (d * 2), hours_scaled_to_days);
      }
    }

    //*************************************************************************
    TEST(test_minus_equal_hours)
    {
      auto num_days  = Chrono::days::period::num;
      auto num_hours = Chrono::hours::period::num;

      Chrono::hours hs(48);
      TimePoint tp{ Chrono::days(100) };

      for (int d = 1; d < 100; ++d)
      {
        tp -= hs;

        auto hours                = tp.time_since_epoch().count();
        auto hours_scaled_to_days = (hours * num_hours) / num_days;

        CHECK_EQUAL(100 - (d * 2), hours_scaled_to_days);
      }
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_min_max_time_point)
    {
      using Rep = TimePoint::duration::rep;

      CHECK_EQUAL(etl::integral_limits<Rep>::min, TimePoint::min().time_since_epoch().count());
      CHECK_EQUAL(etl::integral_limits<Rep>::max, TimePoint::max().time_since_epoch().count());
    }
#endif

    //*************************************************************************
    TEST(test_time_point_comparison_operators)
    {
      // 10 days
      TimePoint tp10h{ Chrono::hours(240) };
      TimePoint tp10d{ Chrono::days(10) };

      // 20 days
      TimePoint tp20h{ Chrono::hours(480) };
      TimePoint tp20d{ Chrono::days(20) };

      // ==
      CHECK_TRUE(tp10h == tp10h);
      CHECK_TRUE(tp10d == tp10h);
      CHECK_TRUE(tp10h == tp10d);

      CHECK_FALSE(tp10h == tp20h);
      CHECK_FALSE(tp20h == tp10h);
      CHECK_FALSE(tp10h == tp20d);

      CHECK_FALSE(tp10d == tp20h);
      CHECK_FALSE(tp20h == tp10d);
      CHECK_FALSE(tp20d == tp10h);

      // <
      CHECK_TRUE(tp10h < tp20h);
      CHECK_TRUE(tp10d < tp20h);
      CHECK_TRUE(tp10h < tp20d);

      CHECK_FALSE(tp10h < tp10h);
      CHECK_FALSE(tp10d < tp10h);
      CHECK_FALSE(tp10h < tp10d);

      CHECK_FALSE(tp20h < tp10h);
      CHECK_FALSE(tp20d < tp10h);
      CHECK_FALSE(tp20h < tp10d);

      // <=
      CHECK_TRUE(tp10h <= tp20h);
      CHECK_TRUE(tp10d <= tp20h);
      CHECK_TRUE(tp10h <= tp20d);

      CHECK_TRUE(tp10h <= tp10h);
      CHECK_TRUE(tp10d <= tp10h);
      CHECK_TRUE(tp10h <= tp10d);

      CHECK_FALSE(tp20h <= tp10h);
      CHECK_FALSE(tp20d <= tp10h);
      CHECK_FALSE(tp20h <= tp10d);

      // >
      CHECK_TRUE(tp20h > tp10h);
      CHECK_TRUE(tp20d > tp10h);
      CHECK_TRUE(tp20h > tp10d);

      CHECK_FALSE(tp10h > tp10h);
      CHECK_FALSE(tp10d > tp10h);
      CHECK_FALSE(tp10h > tp10d);

      CHECK_FALSE(tp10h > tp20h);
      CHECK_FALSE(tp10d > tp20h);
      CHECK_FALSE(tp10h > tp20d);

      // >=
      CHECK_TRUE(tp20h >= tp10h);
      CHECK_TRUE(tp20d >= tp10h);
      CHECK_TRUE(tp20h >= tp10d);

      CHECK_TRUE(tp10h >= tp10h);
      CHECK_TRUE(tp10d >= tp10h);
      CHECK_TRUE(tp10h >= tp10d);

      CHECK_FALSE(tp10h >= tp20h);
      CHECK_FALSE(tp10d >= tp20h);
      CHECK_FALSE(tp10h >= tp20d);

#if ETL_USING_CPP20
        CHECK_TRUE((tp10h <=> tp10h) == 0);
        CHECK_TRUE((tp10h <=> tp20h)  < 0);
        CHECK_TRUE((tp20h <=> tp10h)  > 0);

        CHECK_TRUE((tp10h <=> tp10d) == 0);
        CHECK_TRUE((tp10d <=> tp20h)  < 0);
        CHECK_TRUE((tp20h <=> tp10d)  > 0);
        
        CHECK_TRUE((tp10d <=> tp10h) == 0);
        CHECK_TRUE((tp10h <=> tp20d)  < 0);
        CHECK_TRUE((tp20d <=> tp10h)  > 0);
#endif
    }

    //*************************************************************************
    TEST(test_time_point_common_type)
    {
      using TimePointHours = Chrono::time_point<test_clock, Chrono::hours>;
      using TimePointDays  = Chrono::time_point<test_clock, Chrono::days>;
      using CommonType     = etl::common_type_t<TimePointHours, TimePointDays>;

      CHECK_TRUE((std::is_same<TimePointHours::duration, CommonType::duration>::value));
      CHECK_FALSE((std::is_same<TimePointDays::duration, CommonType::duration>::value));
    }

    //*************************************************************************
    TEST(test_floor)
    {
      using TimePoint = Chrono::time_point<test_clock, Chrono::milliseconds>;

      TimePoint tp(Chrono::milliseconds(1234)); // 1234 milliseconds
      auto floored_tp = floor<Chrono::seconds>(tp);
      CHECK_EQUAL(1, floored_tp.time_since_epoch().count());

      TimePoint negative_tp(Chrono::milliseconds(-1234)); // -1234 milliseconds
      auto floored_negative_tp = floor<Chrono::seconds>(negative_tp);
      CHECK_EQUAL(-2, floored_negative_tp.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_ceil)
    {
      using TimePoint = Chrono::time_point<test_clock, Chrono::milliseconds>;

      TimePoint tp(Chrono::milliseconds(1234)); // 1234 milliseconds
      auto ceil_tp = ceil<Chrono::seconds>(tp);
      CHECK_EQUAL(2, ceil_tp.time_since_epoch().count());

      TimePoint negative_tp(Chrono::milliseconds(-1234)); // -1234 milliseconds
      auto ceil_negative_tp = ceil<Chrono::seconds>(negative_tp);
      CHECK_EQUAL(-1, ceil_negative_tp.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_round)
    {
      using TimePoint = Chrono::time_point<test_clock, Chrono::milliseconds>;

      TimePoint tp1(Chrono::milliseconds(1500)); // 1500 milliseconds
      auto round1 = round<Chrono::seconds>(tp1); // Round to seconds
      CHECK_EQUAL(2, round1.time_since_epoch().count());

      TimePoint tp2(Chrono::milliseconds(2500)); // 2500 milliseconds
      auto round2 = round<Chrono::seconds>(tp2); // Round to seconds
      CHECK_EQUAL(2, round2.time_since_epoch().count());

      TimePoint tp3(Chrono::milliseconds(-1500)); // -1500 milliseconds
      auto round3 = round<Chrono::seconds>(tp3);  // Round to seconds
      CHECK_EQUAL(-2, round3.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_time_point_cast)
    {
      using TimePointFrom = Chrono::time_point<test_clock, Chrono::seconds>;
      using TimePointTo   = Chrono::time_point<test_clock, Chrono::milliseconds>;

      TimePointFrom from(Chrono::seconds(2));
      TimePointTo   to = Chrono::time_point_cast<TimePointTo::duration>(from);

      auto expected = from.time_since_epoch().count() * (TimePointTo::period::den / TimePointFrom::period::den);
      auto actual   = to.time_since_epoch().count();

      CHECK_EQUAL(expected, actual);
    }
  };
}
