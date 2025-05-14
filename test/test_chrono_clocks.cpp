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

#include <type_traits>

//*****************************************************************************
// Global clock functions.
//*****************************************************************************
namespace
{
  int nanoseconds  = 0;
  int milliseconds = 0;
  int seconds      = 0;
}

extern "C"
{
  etl::chrono::system_clock::rep etl_get_system_clock()
  {
    return etl::chrono::system_clock::rep(milliseconds);
  }

  etl::chrono::high_resolution_clock::rep etl_get_high_resolution_clock()
  {
    return etl::chrono::high_resolution_clock::rep(nanoseconds);
  }

  etl::chrono::steady_clock::rep etl_get_steady_clock()
  {
    return etl::chrono::steady_clock::rep(seconds);
  }
}

namespace
{
  SUITE(test_chrono_clocks)
  {
    //*************************************************************************
    TEST(test_system_clock)
    {
      using Clock     = etl::chrono::system_clock;
      using TimePoint = etl::chrono::time_point<Clock>;
      using Duration  = ETL_CHRONO_SYSTEM_CLOCK_DURATION;
     
      CHECK_TRUE((std::is_same<TimePoint, Clock::time_point>::value));
      CHECK_TRUE((std::is_same<Duration, Clock::duration>::value));
      CHECK_TRUE((std::is_same<Duration::rep, Clock::duration::rep>::value));
      CHECK_TRUE((std::is_same<Duration::period, Clock::duration::period>::value));
      CHECK_FALSE(Clock::is_steady); // Set in etl_profile.h

      Clock::time_point now;

      milliseconds = 1000;
      now = Clock::now();
      CHECK_EQUAL(1000, now.time_since_epoch().count());

      milliseconds = 2000;
      now = Clock::now();
      CHECK_EQUAL(2000, now.time_since_epoch().count());

      // Convert to seconds.
      etl::time_t time = Clock::to_time_t(now);
      CHECK_EQUAL(2, time);

      // Convert back to a TimePoint.
      TimePoint tp2 = Clock::from_time_t(time);
      CHECK_EQUAL(2000, tp2.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_high_resolution_clock)
    {
      using Clock     = etl::chrono::high_resolution_clock;
      using TimePoint = etl::chrono::time_point<Clock>;
      using Duration  = ETL_CHRONO_HIGH_RESOLUTION_CLOCK_DURATION;

      CHECK_TRUE((std::is_same<TimePoint, Clock::time_point>::value));
      CHECK_TRUE((std::is_same<Duration, Clock::duration>::value));
      CHECK_TRUE((std::is_same<Duration::rep, Clock::duration::rep>::value));
      CHECK_TRUE((std::is_same<Duration::period, Clock::duration::period>::value));
      CHECK_TRUE(Clock::is_steady);

      Clock::time_point now;

      nanoseconds = 1;
      now = Clock::now();
      CHECK_EQUAL(1, now.time_since_epoch().count());

      nanoseconds = 2;
      now = Clock::now();
      CHECK_EQUAL(2, now.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_steady_clock)
    {
      using Clock     = etl::chrono::steady_clock;
      using TimePoint = etl::chrono::time_point<Clock>;
      using Duration  = ETL_CHRONO_STEADY_CLOCK_DURATION;

      CHECK_TRUE((std::is_same<TimePoint, Clock::time_point>::value));
      CHECK_TRUE((std::is_same<Duration, Clock::duration>::value));
      CHECK_TRUE((std::is_same<Duration::rep, Clock::duration::rep>::value));
      CHECK_TRUE((std::is_same<Duration::period, Clock::duration::period>::value));
      CHECK_TRUE(Clock::is_steady);

      Clock::time_point now;

      seconds = 1;
      now = Clock::now();
      CHECK_EQUAL(1, now.time_since_epoch().count());

      seconds = 2;
      now = Clock::now();
      CHECK_EQUAL(2, now.time_since_epoch().count());
    }

    //*************************************************************************
    TEST(test_clock_cast)
    {      
      using FromClock     = etl::chrono::system_clock;
      using FromTimePoint = etl::chrono::time_point<FromClock>;
      using FromDuration  = FromClock::duration;

      using ToClock     = etl::chrono::steady_clock;
      using ToTimePoint = etl::chrono::time_point<ToClock>;
      using ToDuration  = ToClock::duration;

      FromTimePoint from_tp(FromDuration(1000));

      ToTimePoint to_tp = etl::chrono::clock_cast<ToClock>(from_tp);

      auto num_sys_clock    = FromDuration::period::den;
      auto num_steady_clock = ToDuration::period::num;

      auto sys_clock_count          = from_tp.time_since_epoch().count();
      auto scaled_steady_lock_count = (to_tp.time_since_epoch().count() * num_sys_clock) / num_steady_clock;

      CHECK_EQUAL(sys_clock_count, scaled_steady_lock_count);
    }
  };
}
