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

#include <array>
#include <algorithm>
#include <limits>
#include <vector>
#include <chrono>

namespace
{
  SUITE(test_chrono_duration)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      using duration_type = etl::chrono::duration<size_t, etl::milli>;

      duration_type duration;

      CHECK_TRUE((etl::is_same<size_t, duration_type::rep>::value));
      CHECK_TRUE((etl::is_same<etl::milli, duration_type::period>::value));
      CHECK_EQUAL(0, duration.count());
    }

    //*************************************************************************
    TEST(test_duration_values_zero_min_max)
    {
      using duration_values_type = etl::chrono::duration_values<size_t>;
      
      CHECK_EQUAL(size_t(0),                          duration_values_type::zero());
      CHECK_EQUAL(std::numeric_limits<size_t>::min(), duration_values_type::min());
      CHECK_EQUAL(std::numeric_limits<size_t>::max(), duration_values_type::max());
    }

    //*************************************************************************
    TEST(test_duration_zero_min_max)
    {
      using duration_type = etl::chrono::duration<size_t, etl::milli>;

      CHECK_EQUAL(duration_type(0).count(),                                  duration_type::zero().count());
      CHECK_EQUAL(duration_type(std::numeric_limits<size_t>::min()).count(), duration_type::min().count());
      CHECK_EQUAL(duration_type(std::numeric_limits<size_t>::max()).count(), duration_type::max().count());
    }

    //*************************************************************************
    TEST(test_predefined_duration_periods)
    {
      CHECK_EQUAL((etl::ratio<1U, 1000000000U>::type::num), etl::chrono::nanoseconds::period::num);
      CHECK_EQUAL((etl::ratio<1U, 1000000000U>::type::den), etl::chrono::nanoseconds::period::den);

      CHECK_EQUAL((etl::ratio<1U, 1000000U>::type::num), etl::chrono::microseconds::period::num);
      CHECK_EQUAL((etl::ratio<1U, 1000000U>::type::den), etl::chrono::microseconds::period::den);

      CHECK_EQUAL((etl::ratio<1U, 1000U>::type::num), etl::chrono::milliseconds::period::num);
      CHECK_EQUAL((etl::ratio<1U, 1000U>::type::den), etl::chrono::milliseconds::period::den);

      CHECK_EQUAL(etl::ratio<1U>::type::num, etl::chrono::seconds::period::num);
      CHECK_EQUAL(etl::ratio<1U>::type::den, etl::chrono::seconds::period::den);

      CHECK_EQUAL(etl::ratio<60U>::type::num, etl::chrono::minutes::period::num);
      CHECK_EQUAL(etl::ratio<60U>::type::den, etl::chrono::minutes::period::den);

      CHECK_EQUAL(etl::ratio<3600U>::type::num, etl::chrono::hours::period::num);
      CHECK_EQUAL(etl::ratio<3600U>::type::den, etl::chrono::hours::period::den);

      CHECK_EQUAL(etl::ratio<86400U>::type::num, etl::chrono::days::period::num);
      CHECK_EQUAL(etl::ratio<86400U>::type::den, etl::chrono::days::period::den);

      CHECK_EQUAL(etl::ratio<604800U>::type::num, etl::chrono::weeks::period::num);
      CHECK_EQUAL(etl::ratio<604800U>::type::den, etl::chrono::weeks::period::den);

      CHECK_EQUAL(etl::ratio<604800U>::type::num, etl::chrono::weeks::period::num);
      CHECK_EQUAL(etl::ratio<604800U>::type::den, etl::chrono::weeks::period::den);

      CHECK_EQUAL(etl::ratio<2629746U>::type::num, etl::chrono::months::period::num);
      CHECK_EQUAL(etl::ratio<2629746U>::type::den, etl::chrono::months::period::den);

      CHECK_EQUAL(etl::ratio<31556952U>::type::num, etl::chrono::years::period::num);
      CHECK_EQUAL(etl::ratio<31556952U>::type::den, etl::chrono::years::period::den);
    }

    //*************************************************************************
    TEST(test_duration_common_type)
    {
      using duration_type1 = etl::chrono::duration<size_t, etl::milli>; 
      using duration_type2 = etl::chrono::duration<int,    etl::micro>; 

      using duration_type = etl::common_type<duration_type1, duration_type2>::type;

      CHECK_TRUE((std::is_same<size_t,     duration_type::rep>::value));
      CHECK_TRUE((std::is_same<etl::micro, duration_type::period>::value));
    }

    //*************************************************************************
    TEST(test_duration_cast_with_same_rep_and_period_for_duration_type1_and_duration_type2)
    {
      using duration_type1 = etl::chrono::duration<size_t, etl::milli>;
      using duration_type2 = etl::chrono::duration<int,    etl::ratio<etl::milli::num * 10, etl::milli::den * 10>>; 

      duration_type1 dur1(245);
      duration_type2 dur2 = etl::chrono::duration_cast<duration_type2>(dur1);

      CHECK_EQUAL(duration_type1::period::num, duration_type2::period::num);
      CHECK_EQUAL(duration_type1::period::den, duration_type2::period::den);
      CHECK_EQUAL(dur1.count(), dur2.count());
    }

    //*************************************************************************
    TEST(test_duration_cast_where_ratio_divide_of_periods_num_is_1)
    {
      using duration_type1 = etl::chrono::duration<int,    etl::micro>; 
      using duration_type2 = etl::chrono::duration<size_t, etl::milli>;

      duration_type1 dur1(245000);
      duration_type2 dur2 = etl::chrono::duration_cast<duration_type2>(dur1);

      typedef typename etl::ratio_divide<duration_type1::period, duration_type2::period>::type ratio_divide_t;

      int microseonds_per_millisecond = duration_type1::period::den / duration_type2::period::den;

      CHECK_EQUAL(1, ratio_divide_t::num);
      CHECK_EQUAL(dur1.count(), dur2.count() * microseonds_per_millisecond);
    }

    //*************************************************************************
    TEST(test_duration_cast_where_ratio_divide_of_periods_den_is_1)
    {
      using duration_type1 = etl::chrono::duration<size_t, etl::milli>;
      using duration_type2 = etl::chrono::duration<int,    etl::micro>; 

      duration_type1 dur1(245);
      duration_type2 dur2 = etl::chrono::duration_cast<duration_type2>(dur1);

      typedef typename etl::ratio_divide<duration_type1::period, duration_type2::period>::type ratio_divide_t;

      int microseonds_per_millisecond = duration_type2::period::den / duration_type1::period::den;

      CHECK_EQUAL(1, ratio_divide_t::den);
      CHECK_EQUAL(dur1.count() * microseonds_per_millisecond, dur2.count());
    }

    //*************************************************************************
    TEST(test_duration_cast_when_rep_and_period_are_not_equal_and_ratio_divide_of_periods_num_and_den_are_not_1)
    {
      using duration_type1 = etl::chrono::duration<size_t, etl::ratio<etl::milli::num * 3, etl::milli::den>>;
      using duration_type2 = etl::chrono::duration<int,    etl::ratio<etl::milli::num * 7, etl::milli::den>>; 

      duration_type1 dur1(245);
      duration_type2 dur2 = etl::chrono::duration_cast<duration_type2>(dur1);

      typedef typename etl::ratio_divide<duration_type1::period, duration_type2::period>::type ratio_divide_t;

      CHECK_FALSE((duration_type1::period::num == duration_type2::period::num) && (duration_type1::period::den == duration_type2::period::den));
      CHECK_FALSE(1 == ratio_divide_t::num);
      CHECK_FALSE(1 == ratio_divide_t::den);
      CHECK_EQUAL(dur1.count(), (dur2.count() * duration_type2::period::num) / duration_type1::period::num);
    }

    //*************************************************************************
    TEST(test_duration_assignment)
    {
      int Two_Hours  = 2;
      int Four_Hours = 4;

      etl::chrono::hours   hours1(Two_Hours);
      etl::chrono::hours   hours2(Four_Hours);
      etl::chrono::seconds seconds(0);

      seconds = hours1;
      int seconds_per_hour = etl::chrono::hours::period::num / etl::chrono::seconds::period::num;
      CHECK_EQUAL(seconds.count(), hours1.count() * seconds_per_hour);
      CHECK_EQUAL(Two_Hours, hours1.count());

      hours1 = hours2;
      CHECK_EQUAL(Four_Hours, hours1.count());
      CHECK_EQUAL(Four_Hours, hours2.count());
    }

    //*************************************************************************
    TEST(test_duration_comparison_operators)
    {
      using duration_type1 = etl::chrono::duration<size_t, etl::milli>;
      using duration_type2 = etl::chrono::duration<int,    etl::micro>;

      duration_type1 dur1a(245);
      duration_type1 dur1b(245);
      duration_type1 dur1c(246);
      duration_type2 dur2b(245000);
      duration_type2 dur2c(245001);

      //  Same duration types
      CHECK_TRUE(dur1a  == dur1b);
      CHECK_TRUE(dur1a  != dur1c);
      CHECK_FALSE(dur1a <  dur1b);
      CHECK_TRUE(dur1a  <= dur1b);
      CHECK_FALSE(dur1a >  dur1b);
      CHECK_TRUE(dur1a  >= dur1b);

      CHECK_TRUE(dur1a  <  dur1c);
      CHECK_FALSE(dur1c <  dur1a);
      CHECK_TRUE(dur1a  <= dur1c);
      CHECK_FALSE(dur1c <= dur1a);
      CHECK_FALSE(dur1a >  dur1c);
      CHECK_TRUE(dur1c  >  dur1a);
      CHECK_FALSE(dur1a >= dur1c);
      CHECK_TRUE(dur1c  >= dur1a);

      //  Different duration types
      CHECK_TRUE(dur1a  == dur2b);
      CHECK_TRUE(dur2b  == dur1a);
      CHECK_TRUE(dur2c  != dur1a);
      CHECK_TRUE(dur1a  != dur2c);

      CHECK_FALSE(dur1a <  dur2b);
      CHECK_FALSE(dur2b <  dur1a);
      CHECK_TRUE(dur1a  <= dur2b);
      CHECK_TRUE(dur2b  <= dur1a);
      CHECK_FALSE(dur1a >  dur2b);
      CHECK_FALSE(dur2b >  dur1a);
      CHECK_TRUE(dur1a  >= dur2b);
      CHECK_TRUE(dur2b  >= dur1a);

      CHECK_TRUE(dur1a  <  dur2c);
      CHECK_FALSE(dur2c <  dur1a);
      CHECK_TRUE(dur1a  <= dur2c);
      CHECK_FALSE(dur2c <= dur1a);
      CHECK_FALSE(dur1a >  dur2c);
      CHECK_TRUE(dur2c  >  dur1a);
      CHECK_FALSE(dur1a >= dur2c);
      CHECK_TRUE(dur2c  >= dur1a);
    }

    //*************************************************************************
    TEST(test_duration_unary_operators)
    {
      using duration_type = etl::chrono::duration<int, etl::milli>;

      duration_type dur(245);

      duration_type positive = +dur;
      duration_type negative = -dur;

      CHECK_EQUAL(245,  positive.count());
      CHECK_EQUAL(-245, negative.count());
    }

    //*************************************************************************
    TEST(test_duration_hashes_are_unique)
    {
      using duration_type = etl::chrono::duration<size_t, etl::milli>;

      std::vector<size_t> hashes;

      for (int i = 0; i < 256; ++i)
      {
        hashes.push_back(etl::hash<duration_type>()(duration_type(i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(256U, hashes.size());
    }
  };
}
