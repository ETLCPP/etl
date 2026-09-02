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

// Set to 0 to reference against std::chrono
#define ETL_USING_ETL_CHRONO 1

#if ETL_USING_ETL_CHRONO
  #define Chrono etl::chrono
  #define Ratio  etl::ratio
#else
  #if ETL_USING_CPP20
    #include <chrono>
    #define Chrono std::chrono
    #define Ratio  std::ratio
  #else
    #error std::chrono not supported
  #endif
#endif

namespace
{
  SUITE(test_chrono_hh_mm_ss)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      using duration_type = Chrono::seconds;

      // Create a duration of 0 hour, 0 minutes, 0 seconds
      Chrono::hh_mm_ss<duration_type> time;

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(Chrono::hours(0).count(), h.count());
      CHECK_EQUAL(Chrono::minutes(0).count(), m.count());
      CHECK_EQUAL(Chrono::seconds(0).count(), s.count());
      CHECK_EQUAL(duration_type(0).count(), sub.count());
      CHECK_EQUAL(0, dur.count());
      CHECK_FALSE(time.is_negative());
      CHECK_EQUAL(0, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_construction_with_seconds)
    {
      using duration_type = Chrono::seconds;

      // Create a duration of 1 hour, 2 minutes, 3 seconds
      duration_type duration = Chrono::hours(1) + Chrono::minutes(2) + Chrono::seconds(3);

      Chrono::hh_mm_ss<duration_type> time(duration);

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(Chrono::hours(1).count(), h.count());
      CHECK_EQUAL(Chrono::minutes(2).count(), m.count());
      CHECK_EQUAL(Chrono::seconds(3).count(), s.count());
      CHECK_EQUAL(duration_type(0).count(), sub.count());
      CHECK_EQUAL(3723, dur.count());
      CHECK_FALSE(time.is_negative());
      CHECK_EQUAL(0, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_construction_with_negative_seconds)
    {
      using duration_type = Chrono::seconds;

      // Create a duration of minus 1 hour, 2 minutes, 3 seconds
      duration_type duration = -(Chrono::hours(1) + Chrono::minutes(2) + Chrono::seconds(3));

      Chrono::hh_mm_ss<duration_type> time(duration);

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(Chrono::hours(1).count(), h.count());
      CHECK_EQUAL(Chrono::minutes(2).count(), m.count());
      CHECK_EQUAL(Chrono::seconds(3).count(), s.count());
      CHECK_EQUAL(duration_type(0).count(), sub.count());
      CHECK_EQUAL(-3723, dur.count());
      CHECK_TRUE(time.is_negative());
      CHECK_EQUAL(0, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_construction_with_milliseconds)
    {
      using duration_type = Chrono::milliseconds;

      // Create a duration of 1 hour, 2 minutes, 3 seconds, 456 milliseconds
      duration_type duration = Chrono::hours(1) + Chrono::minutes(2) + Chrono::seconds(3) + duration_type(456);

      Chrono::hh_mm_ss<duration_type> time(duration);

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(Chrono::hours(1).count(), h.count());
      CHECK_EQUAL(Chrono::minutes(2).count(), m.count());
      CHECK_EQUAL(Chrono::seconds(3).count(), s.count());
      CHECK_EQUAL(duration_type(456).count(), sub.count());
      CHECK_EQUAL(3723456, dur.count());
      CHECK_FALSE(time.is_negative());
      CHECK_EQUAL(3, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_construction_with_negative_milliseconds)
    {
      using duration_type = Chrono::milliseconds;

      // Create a duration of minus 1 hour, 2 minutes, 3 seconds, 456
      // milliseconds
      duration_type duration = -(Chrono::hours(1) + Chrono::minutes(2) + Chrono::seconds(3) + duration_type(456));

      Chrono::hh_mm_ss<duration_type> time(duration);

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(Chrono::hours(1).count(), h.count());
      CHECK_EQUAL(Chrono::minutes(2).count(), m.count());
      CHECK_EQUAL(Chrono::seconds(3).count(), s.count());
      CHECK_EQUAL(duration_type(456).count(), sub.count());
      CHECK_EQUAL(-3723456, dur.count());
      CHECK_TRUE(time.is_negative());
      CHECK_EQUAL(3, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_construction_with_float_seconds)
    {
      using duration_type = Chrono::duration<float, Ratio<1, 1>>;

      // Create a duration of 1 hour, 2 minutes, 3.456 seconds
      duration_type duration = Chrono::hours(1) + Chrono::minutes(2) + duration_type(3.456);

      Chrono::hh_mm_ss<duration_type> time(duration);

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(1, h.count());
      CHECK_EQUAL(2, m.count());
      CHECK_EQUAL(3, s.count());
      CHECK_CLOSE(0.456f, sub.count(), 0.0001f);
      CHECK_CLOSE(3723.456f, dur.count(), 0.0001f);
      CHECK_FALSE(time.is_negative());
      CHECK_EQUAL(0, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_construction_with_negative_float_seconds)
    {
      using duration_type = Chrono::duration<float, Ratio<1, 1>>;

      // Create a duration of 1 hour, 2 minutes, 3.456 seconds
      duration_type duration = -(Chrono::hours(1) + Chrono::minutes(2) + duration_type(3.456));

      Chrono::hh_mm_ss<duration_type> time(duration);

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(1, h.count());
      CHECK_EQUAL(2, m.count());
      CHECK_EQUAL(3, s.count());
      CHECK_CLOSE(0.456f, sub.count(), 0.0001f);
      CHECK_CLOSE(-3723.456f, dur.count(), 0.0001f);
      CHECK_TRUE(time.is_negative());
      CHECK_EQUAL(0, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_construction_with_double_seconds)
    {
      using duration_type = Chrono::duration<double, Ratio<1, 1>>;

      // Create a duration of 1 hour, 2 minutes, 3.456 seconds
      duration_type duration = Chrono::hours(1) + Chrono::minutes(2) + duration_type(3.456);

      Chrono::hh_mm_ss<duration_type> time(duration);

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(1, h.count());
      CHECK_EQUAL(2, m.count());
      CHECK_EQUAL(3, s.count());
      CHECK_CLOSE(0.456, sub.count(), 0.0001);
      CHECK_CLOSE(3723.456, dur.count(), 0.0001);
      CHECK_FALSE(time.is_negative());
      CHECK_EQUAL(0, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_construction_with_negative_double_seconds)
    {
      using duration_type = Chrono::duration<double, Ratio<1, 1>>;

      // Create a duration of 1 hour, 2 minutes, 3.456 seconds
      duration_type duration = -(Chrono::hours(1) + Chrono::minutes(2) + duration_type(3.456));

      Chrono::hh_mm_ss<duration_type> time(duration);

      auto h   = time.hours();
      auto m   = time.minutes();
      auto s   = time.seconds();
      auto sub = time.subseconds();
      auto dur = time.to_duration();

      CHECK_EQUAL(1, h.count());
      CHECK_EQUAL(2, m.count());
      CHECK_EQUAL(3, s.count());
      CHECK_CLOSE(0.456, sub.count(), 0.0001);
      CHECK_CLOSE(-3723.456, dur.count(), 0.0001);
      CHECK_TRUE(time.is_negative());
      CHECK_EQUAL(0, time.fractional_width);
      CHECK_TRUE((std::is_same<duration_type, Chrono::hh_mm_ss<duration_type>::precision>::value));
    }

    //*************************************************************************
    TEST(test_is_am)
    {
      for (int h = 0; h <= 11; ++h)
      {
        CHECK_TRUE(Chrono::is_am(Chrono::hours(h)));
      }

      for (int h = 12; h <= 23; ++h)
      {
        CHECK_FALSE(Chrono::is_am(Chrono::hours(h)));
      }
    }

    //*************************************************************************
    TEST(test_is_pm)
    {
      for (int h = 0; h <= 11; ++h)
      {
        CHECK_FALSE(Chrono::is_pm(Chrono::hours(h)));
      }

      for (int h = 12; h <= 23; ++h)
      {
        CHECK_TRUE(Chrono::is_pm(Chrono::hours(h)));
      }
    }

    //*************************************************************************
    TEST(test_make12)
    {
      // Midnight (0h) maps to 12h.
      CHECK_EQUAL(12, Chrono::make12(Chrono::hours(0)).count());

      // 1h..12h are unchanged.
      for (int h = 1; h <= 12; ++h)
      {
        CHECK_EQUAL(h, Chrono::make12(Chrono::hours(h)).count());
      }

      // 13h..23h map to 1h..11h.
      for (int h = 13; h <= 23; ++h)
      {
        CHECK_EQUAL(h - 12, Chrono::make12(Chrono::hours(h)).count());
      }
    }

    //*************************************************************************
    TEST(test_make24)
    {
      // am: 12h maps to 0h, 1h..11h are unchanged.
      CHECK_EQUAL(0, Chrono::make24(Chrono::hours(12), false).count());

      for (int h = 1; h <= 11; ++h)
      {
        CHECK_EQUAL(h, Chrono::make24(Chrono::hours(h), false).count());
      }

      // pm: 12h stays 12h, 1h..11h map to 13h..23h.
      CHECK_EQUAL(12, Chrono::make24(Chrono::hours(12), true).count());

      for (int h = 1; h <= 11; ++h)
      {
        CHECK_EQUAL(h + 12, Chrono::make24(Chrono::hours(h), true).count());
      }
    }

    //*************************************************************************
    TEST(test_make12_make24_round_trip)
    {
      // For every 24-hour value, make12 plus its am/pm flag round-trips via make24.
      for (int h = 0; h <= 23; ++h)
      {
        Chrono::hours h24(h);
        bool          pm  = Chrono::is_pm(h24);
        Chrono::hours h12 = Chrono::make12(h24);

        CHECK_EQUAL(h, Chrono::make24(h12, pm).count());
      }
    }
  }
} // namespace
