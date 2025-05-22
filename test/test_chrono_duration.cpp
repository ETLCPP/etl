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
  SUITE(test_chrono_duration)
  {
#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_default_constructor)
    {
      using duration_type = Chrono::duration<size_t, etl::milli>;

      duration_type duration;

      CHECK_TRUE((etl::is_same<size_t, duration_type::rep>::value));
      CHECK_TRUE((etl::is_same<etl::milli, duration_type::period>::value));
      CHECK_EQUAL(0, duration.count());
    }
#endif

    //*************************************************************************
    TEST(test_duration_values_zero_min_max)
    {
      using duration_values_type = Chrono::duration_values<size_t>;
      
      CHECK_EQUAL(size_t(0),                          duration_values_type::zero());
      CHECK_EQUAL(std::numeric_limits<size_t>::min(), duration_values_type::min());
      CHECK_EQUAL(std::numeric_limits<size_t>::max(), duration_values_type::max());
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_zero_min_max)
    {
      using duration_type = Chrono::duration<size_t, etl::milli>;

      CHECK_EQUAL(duration_type(0).count(),                                  duration_type::zero().count());
      CHECK_EQUAL(duration_type(std::numeric_limits<size_t>::min()).count(), duration_type::min().count());
      CHECK_EQUAL(duration_type(std::numeric_limits<size_t>::max()).count(), duration_type::max().count());
    }
#endif

    //*************************************************************************
    TEST(test_predefined_duration_periods)
    {
      CHECK_EQUAL((etl::ratio<1U, 1000000000U>::type::num), Chrono::nanoseconds::period::num);
      CHECK_EQUAL((etl::ratio<1U, 1000000000U>::type::den), Chrono::nanoseconds::period::den);

      CHECK_EQUAL((etl::ratio<1U, 1000000U>::type::num), Chrono::microseconds::period::num);
      CHECK_EQUAL((etl::ratio<1U, 1000000U>::type::den), Chrono::microseconds::period::den);

      CHECK_EQUAL((etl::ratio<1U, 1000U>::type::num), Chrono::milliseconds::period::num);
      CHECK_EQUAL((etl::ratio<1U, 1000U>::type::den), Chrono::milliseconds::period::den);

      CHECK_EQUAL(etl::ratio<1U>::type::num, Chrono::seconds::period::num);
      CHECK_EQUAL(etl::ratio<1U>::type::den, Chrono::seconds::period::den);

      CHECK_EQUAL(etl::ratio<60U>::type::num, Chrono::minutes::period::num);
      CHECK_EQUAL(etl::ratio<60U>::type::den, Chrono::minutes::period::den);

      CHECK_EQUAL(etl::ratio<3600U>::type::num, Chrono::hours::period::num);
      CHECK_EQUAL(etl::ratio<3600U>::type::den, Chrono::hours::period::den);

      CHECK_EQUAL(etl::ratio<86400U>::type::num, Chrono::days::period::num);
      CHECK_EQUAL(etl::ratio<86400U>::type::den, Chrono::days::period::den);

      CHECK_EQUAL(etl::ratio<604800U>::type::num, Chrono::weeks::period::num);
      CHECK_EQUAL(etl::ratio<604800U>::type::den, Chrono::weeks::period::den);

      CHECK_EQUAL(etl::ratio<604800U>::type::num, Chrono::weeks::period::num);
      CHECK_EQUAL(etl::ratio<604800U>::type::den, Chrono::weeks::period::den);

      CHECK_EQUAL(etl::ratio<2629746U>::type::num, Chrono::months::period::num);
      CHECK_EQUAL(etl::ratio<2629746U>::type::den, Chrono::months::period::den);

      CHECK_EQUAL(etl::ratio<31556952U>::type::num, Chrono::years::period::num);
      CHECK_EQUAL(etl::ratio<31556952U>::type::den, Chrono::years::period::den);
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_common_type)
    {
      using duration_type1 = Chrono::duration<size_t, etl::milli>; 
      using duration_type2 = Chrono::duration<int,    etl::micro>; 

      using duration_type = etl::common_type_t<duration_type1, duration_type2>;

      CHECK_TRUE((std::is_same<size_t,        duration_type::rep>::value));
      CHECK_TRUE((std::is_same<etl::micro, duration_type::period>::value));
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_cast_with_same_rep_and_period_for_duration_type1_and_duration_type2)
    {
      using duration_type1 = Chrono::duration<size_t, etl::milli>;
      using duration_type2 = Chrono::duration<int,    etl::ratio<etl::milli::num * 10, etl::milli::den * 10>>; 

      duration_type1 dur1(245);
      duration_type2 dur2 = Chrono::duration_cast<duration_type2>(dur1);

      CHECK_EQUAL(duration_type1::period::num, duration_type2::period::num);
      CHECK_EQUAL(duration_type1::period::den, duration_type2::period::den);
      CHECK_EQUAL(dur1.count(), dur2.count());
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_cast_where_ratio_divide_of_periods_num_is_1)
    {
      using duration_type1 = Chrono::duration<int,    etl::micro>; 
      using duration_type2 = Chrono::duration<size_t, etl::milli>;

      duration_type1 dur1(245000);
      duration_type2 dur2 = Chrono::duration_cast<duration_type2>(dur1);

      typedef typename etl::ratio_divide<duration_type1::period, duration_type2::period>::type ratio_divide_t;

      int microseonds_per_millisecond = duration_type1::period::den / duration_type2::period::den;

      CHECK_EQUAL(1, ratio_divide_t::num);
      CHECK_EQUAL(dur1.count(), dur2.count() * microseonds_per_millisecond);
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_cast_where_ratio_divide_of_periods_den_is_1)
    {
      using duration_type1 = Chrono::duration<size_t, etl::milli>;
      using duration_type2 = Chrono::duration<int,    etl::micro>; 

      duration_type1 dur1(245);
      duration_type2 dur2 = Chrono::duration_cast<duration_type2>(dur1);

      typedef typename etl::ratio_divide<duration_type1::period, duration_type2::period>::type ratio_divide_t;

      int microseonds_per_millisecond = duration_type2::period::den / duration_type1::period::den;

      CHECK_EQUAL(1, ratio_divide_t::den);
      CHECK_EQUAL(dur1.count() * microseonds_per_millisecond, dur2.count());
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_cast_when_rep_and_period_are_not_equal_and_ratio_divide_of_periods_num_and_den_are_not_1)
    {
      using duration_type1 = Chrono::duration<size_t, etl::ratio<etl::milli::num * 3, etl::milli::den>>;
      using duration_type2 = Chrono::duration<int,    etl::ratio<etl::milli::num * 7, etl::milli::den>>; 

      duration_type1 dur1(245);
      duration_type2 dur2 = Chrono::duration_cast<duration_type2>(dur1);

      typedef typename etl::ratio_divide<duration_type1::period, duration_type2::period>::type ratio_divide_t;

      CHECK_FALSE((duration_type1::period::num == duration_type2::period::num) && (duration_type1::period::den == duration_type2::period::den));
      CHECK_FALSE(1 == ratio_divide_t::num);
      CHECK_FALSE(1 == ratio_divide_t::den);
      CHECK_EQUAL(dur1.count(), (dur2.count() * duration_type2::period::num) / duration_type1::period::num);
    }
#endif

    //*************************************************************************
    TEST(test_duration_assignment)
    {
      int Two_Hours  = 2;
      int Four_Hours = 4;

      Chrono::hours   hours1(Two_Hours);
      Chrono::hours   hours2(Four_Hours);
      Chrono::seconds seconds(0);

      seconds = hours1;
      int seconds_per_hour = Chrono::hours::period::num / Chrono::seconds::period::num;
      CHECK_EQUAL(seconds.count(), hours1.count() * seconds_per_hour);
      CHECK_EQUAL(Two_Hours, hours1.count());

      hours1 = hours2;
      CHECK_EQUAL(Four_Hours, hours1.count());
      CHECK_EQUAL(Four_Hours, hours2.count());
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_comparison_operators)
    {
      using duration_type1 = Chrono::duration<size_t, etl::milli>;
      using duration_type2 = Chrono::duration<int,    etl::micro>;

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
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_unary_operators)
    {
      using duration_type = Chrono::duration<int, etl::milli>;

      duration_type dur(245);

      duration_type positive = +dur;
      duration_type negative = -dur;

      CHECK_EQUAL(245,  positive.count());
      CHECK_EQUAL(-245, negative.count());
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_hashes_are_unique)
    {
      using duration_type = Chrono::duration<size_t, etl::milli>;

      std::vector<size_t> hashes;

      for (int i = 0; i < 256; ++i)
      {
        hashes.push_back(etl::hash<duration_type>()(duration_type(i)));
      }

      std::sort(hashes.begin(), hashes.end());
      (void)std::unique(hashes.begin(), hashes.end());
      CHECK_EQUAL(256U, hashes.size());
    }
#endif

    //*************************************************************************
    TEST(test_duration_pre_increment)
    {
      Chrono::hours   hours(0);
      Chrono::minutes minutes(0);
      Chrono::seconds seconds(0);

      for (int i = 1; i < 100; ++i)
      {
        ++hours;
        ++minutes;
        ++seconds;

        CHECK_EQUAL(i, hours.count());
        CHECK_EQUAL(i, minutes.count());
        CHECK_EQUAL(i, seconds.count());
      }
    }

    //*************************************************************************
    TEST(test_duration_post_increment)
    {
      Chrono::hours   hours(0);
      Chrono::minutes minutes(0);
      Chrono::seconds seconds(0);

      for (int i = 1; i < 100; ++i)
      {
        Chrono::hours   last_hours   = hours++;
        Chrono::minutes last_minutes = minutes++;
        Chrono::seconds last_seconds = seconds++;

        CHECK_EQUAL(i - 1, last_hours.count());
        CHECK_EQUAL(i - 1, last_minutes.count());
        CHECK_EQUAL(i - 1, last_seconds.count());

        CHECK_EQUAL(i, hours.count());
        CHECK_EQUAL(i, minutes.count());
        CHECK_EQUAL(i, seconds.count());
      }
    }

    //*************************************************************************
    TEST(test_duration_pre_decrement)
    {
      Chrono::hours   hours(101);
      Chrono::minutes minutes(101);
      Chrono::seconds seconds(101);

      for (int i = 100; i > 0; --i)
      {
        --hours;
        --minutes;
        --seconds;

        CHECK_EQUAL(i, hours.count());
        CHECK_EQUAL(i, minutes.count());
        CHECK_EQUAL(i, seconds.count());
      }
    }

    //*************************************************************************
    TEST(test_duration_post_decrement)
    {
      Chrono::hours   hours(101);
      Chrono::minutes minutes(101);
      Chrono::seconds seconds(101);

      for (int i = 100; i > 0; --i)
      {
        Chrono::hours   last_hours   = hours--;
        Chrono::minutes last_minutes = minutes--;
        Chrono::seconds last_seconds = seconds--;

        CHECK_EQUAL(i + 1, last_hours.count());
        CHECK_EQUAL(i + 1, last_minutes.count());
        CHECK_EQUAL(i + 1, last_seconds.count());

        CHECK_EQUAL(i, hours.count());
        CHECK_EQUAL(i, minutes.count());
        CHECK_EQUAL(i, seconds.count());
      }
    }

    //*************************************************************************
    TEST(test_addition_same_type)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(20); // 20 seconds

      auto result = s1 + s2;

      CHECK_EQUAL(30, result.count()); // 10 + 20 = 30 seconds
    }

    //*************************************************************************
    TEST(test_addition_different_types)
    {
      Chrono::seconds s1(10);        // 10 seconds
      Chrono::milliseconds ms1(500); // 500 milliseconds

      auto result = s1 + ms1;

      // Result should be in the common type (milliseconds)
      CHECK_EQUAL(10500, result.count()); // 10 seconds + 500 milliseconds = 10500 milliseconds
    }

    //*************************************************************************
    TEST(test_addition_with_zero)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(0);  // 0 seconds

      auto result = s1 + s2;

      CHECK_EQUAL(10, result.count()); // 10 + 0 = 10 seconds
    }

    //*************************************************************************
    TEST(test_addition_negative_duration)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(-5); // -5 seconds

      auto result = s1 + s2;

      CHECK_EQUAL(5, result.count()); // 10 + (-5) = 5 seconds
    }

    //*************************************************************************
    TEST(test_addition_large_values)
    {
      Chrono::seconds s1(1000000);      // 1,000,000 seconds
      Chrono::milliseconds ms1(500000); // 500,000 milliseconds

      auto result = s1 + ms1;

      // Result should be in the common type (milliseconds)
      CHECK_EQUAL(1000500000, result.count()); // 1,000,000 seconds + 500,000 milliseconds = 1,000,500,000 milliseconds
    }

    //*************************************************************************
    TEST(test_subtraction_same_type)
    {
      Chrono::seconds s1(20); // 20 seconds
      Chrono::seconds s2(10); // 10 seconds

      auto result = s1 - s2;

      CHECK_EQUAL(10, result.count()); // 20 - 10 = 10 seconds
    }

    //*************************************************************************
    TEST(test_subtraction_different_types)
    {
      Chrono::seconds s1(10);        // 10 seconds
      Chrono::milliseconds ms1(500); // 500 milliseconds

      auto result = s1 - ms1;

      // Result should be in the common type (milliseconds)
      CHECK_EQUAL(9500, result.count()); // 10 seconds - 500 milliseconds = 9500 milliseconds
    }

    //*************************************************************************
    TEST(test_subtraction_with_zero)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(0);  // 0 seconds

      auto result = s1 - s2;

      CHECK_EQUAL(10, result.count()); // 10 - 0 = 10 seconds
    }

    //*************************************************************************
    TEST(test_subtraction_negative_duration)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(-5); // -5 seconds

      auto result = s1 - s2;

      CHECK_EQUAL(15, result.count()); // 10 - (-5) = 15 seconds
    }

    //*************************************************************************
    TEST(test_subtraction_large_values)
    {
      Chrono::seconds s1(1000000);      // 1,000,000 seconds
      Chrono::milliseconds ms1(500000); // 500,000 milliseconds

      auto result = s1 - ms1;

      // Result should be in the common type (milliseconds)
      CHECK_EQUAL(999500000, result.count()); // 1,000,000 seconds - 500,000 milliseconds = 999,500,000 milliseconds
    }

    //*************************************************************************
    TEST(test_subtraction_resulting_in_negative)
    {
      Chrono::seconds s1(5);  // 5 seconds
      Chrono::seconds s2(10); // 10 seconds

      auto result = s1 - s2;

      CHECK_EQUAL(-5, result.count()); // 5 - 10 = -5 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_scalar_positive_duration_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 5;              // Scalar value

      auto result = s1 * scalar;

      CHECK_EQUAL(50, result.count()); // 10 * 5 = 50 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_scalar_negative_duration_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = -3;             // Negative scalar value

      auto result = s1 * scalar;

      CHECK_EQUAL(-30, result.count()); // 10 * (-3) = -30 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_scalar_zero_duration_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 0;              // Scalar value

      auto result = s1 * scalar;

      CHECK_EQUAL(0, result.count()); // 10 * 0 = 0 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_large_scalar_duration_scalar)
    {
      Chrono::seconds s1(1000); // 1,000 seconds
      int scalar = 1000000;          // Large scalar value

      auto result = s1 * scalar;

      CHECK_EQUAL(1000000000, result.count()); // 1,000 * 1,000,000 = 1,000,000,000 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_floating_point_scalar_duration_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      double scalar = 2.5;         // Floating-point scalar value

      auto result = s1 * scalar;

      CHECK_EQUAL(25, result.count()); // 10 * 2.5 = 25 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_different_representation_duration_scalar)
    {
      Chrono::milliseconds ms1(500); // 500 milliseconds
      int scalar = 3;                     // Scalar value

      auto result = ms1 * scalar;

      CHECK_EQUAL(1500, result.count()); // 500 * 3 = 1,500 milliseconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_scalar_positive_scalar_duration)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 5;              // Scalar value

      auto result = scalar * s1;

      CHECK_EQUAL(50, result.count()); // 10 * 5 = 50 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_scalar_negative_scalar_duration)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = -3;             // Negative scalar value

      auto result = scalar * s1;

      CHECK_EQUAL(-30, result.count()); // 10 * (-3) = -30 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_scalar_zero_scalar_duration)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 0;              // Scalar value

      auto result = scalar * s1;

      CHECK_EQUAL(0, result.count()); // 10 * 0 = 0 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_large_scalar_scalar_duration)
    {
      Chrono::seconds s1(1000); // 1,000 seconds
      int scalar = 1000000;          // Large scalar value

      auto result = scalar * s1;

      CHECK_EQUAL(1000000000, result.count()); // 1,000 * 1,000,000 = 1,000,000,000 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_floating_point_scalar_scalar_duration)
    {
      Chrono::seconds s1(10); // 10 seconds
      double scalar = 2.5;         // Floating-point scalar value

      auto result = scalar * s1;

      CHECK_EQUAL(25, result.count()); // 10 * 2.5 = 25 seconds
    }

    //*************************************************************************
    TEST(test_multiplication_with_different_representation_scalar_duration)
    {
      Chrono::milliseconds ms1(500); // 500 milliseconds
      int scalar = 3;                     // Scalar value

      auto result = scalar * ms1;

      CHECK_EQUAL(1500, result.count()); // 500 * 3 = 1,500 milliseconds
    }

    //*************************************************************************
    TEST(test_division_with_scalar_positive_duration_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 5;              // Scalar value

      auto result = s1 / scalar;

      CHECK_EQUAL(2, result.count()); // 10 / 5 = 2 seconds
    }

    //*************************************************************************
    TEST(test_division_with_scalar_negative_duration_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = -2;             // Negative scalar value

      auto result = s1 / scalar;

      CHECK_EQUAL(-5, result.count()); // 10 / (-2) = -5 seconds
    }

    //*************************************************************************
    TEST(test_division_with_large_scalar_duration_scalar)
    {
      Chrono::seconds s1(10000000); // 10,000,000 seconds
      int scalar = 1000000;              // Large scalar value

      auto result = s1 / scalar;

      CHECK_EQUAL(10, result.count()); // 10,000,000 / 1,000,000 = 10 seconds
    }

    //*************************************************************************
    TEST(test_division_with_floating_point_scalar_duration_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      double scalar = 2.5;         // Floating-point scalar value

      auto result = s1 / scalar;

      CHECK_EQUAL(4, result.count()); // 10 / 2.5 = 4 seconds
    }

    //*************************************************************************
    TEST(test_division_with_different_representation_duration_scalar)
    {
      Chrono::milliseconds ms1(500); // 500 milliseconds
      int scalar = 2;                     // Scalar value

      auto result = ms1 / scalar;

      CHECK_EQUAL(250, result.count()); // 500 / 2 = 250 milliseconds
    }

    //*************************************************************************
    TEST(test_division_same_type)
    {
      Chrono::seconds s1(20); // 20 seconds
      Chrono::seconds s2(10); // 10 seconds

      auto result = s1 / s2;

      CHECK_EQUAL(2, result); // 20 / 10 = 2
    }

    //*************************************************************************
    TEST(test_division_different_types)
    {
      Chrono::seconds s1(10);        // 10 seconds
      Chrono::milliseconds ms1(500); // 500 milliseconds

      auto result = s1 / ms1;

      // Result should be in the common type (milliseconds)
      CHECK_EQUAL(20, result); // 10 seconds / 500 milliseconds = 20
    }

    //*************************************************************************
    TEST(test_division_with_large_values)
    {
      Chrono::seconds s1(1000000);      // 1,000,000 seconds
      Chrono::milliseconds ms1(500000); // 500,000 milliseconds

      auto result = s1 / ms1;

      CHECK_EQUAL(2000, result); // 1,000,000 seconds / 500,000 milliseconds = 2000
    }

    //*************************************************************************
    TEST(test_division_resulting_in_fraction)
    {
      Chrono::seconds s1(7); // 7 seconds
      Chrono::seconds s2(3); // 3 seconds

      auto result = s1 / s2;

      CHECK_EQUAL(2, result); // 7 / 3 = 2 (integer division)
    }

    //*************************************************************************
    TEST(test_division_by_larger_duration)
    {
      Chrono::seconds s1(5);  // 5 seconds
      Chrono::seconds s2(10); // 10 seconds

      auto result = s1 / s2;

      CHECK_EQUAL(0, result); // 5 / 10 = 0 (integer division)
    }

    //*************************************************************************
    TEST(test_division_by_one)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(1);  // 1 second

      auto result = s1 / s2;

      CHECK_EQUAL(10, result); // 10 / 1 = 10
    }

    //*************************************************************************
    TEST(test_modulus_with_positive_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 3;              // Scalar value

      Chrono::seconds result = s1 % scalar;

      CHECK_EQUAL(1, result.count()); // 10 % 3 = 1 second
    }

    //*************************************************************************
    TEST(test_modulus_with_negative_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = -3;             // Negative scalar value

      Chrono::seconds result = s1 % scalar;

      CHECK_EQUAL(1, result.count()); // 10 % -3 = 1 second (modulus result is positive)
    }

    //*************************************************************************
    TEST(test_modulus_with_large_scalar)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 100;            // Scalar value larger than duration

      Chrono::seconds result = s1 % scalar;

      CHECK_EQUAL(10, result.count()); // 10 % 100 = 10 seconds
    }

    //*************************************************************************
    TEST(test_modulus_with_different_representation)
    {
      Chrono::milliseconds ms1(1050); // 1050 milliseconds
      int scalar = 500;                    // Scalar value

      Chrono::milliseconds result = ms1 % scalar;

      CHECK_EQUAL(50, result.count()); // 1050 % 500 = 50 milliseconds
    }

    //*************************************************************************
    TEST(test_modulus_same_type)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(3);  // 3 seconds

      Chrono::seconds result = s1 % s2;

      CHECK_EQUAL(1, result.count()); // 10 % 3 = 1 second
    }

    //*************************************************************************
    TEST(test_modulus_different_types)
    {
      Chrono::seconds s1(10);         // 10 seconds
      Chrono::milliseconds ms1(3000); // 3000 milliseconds

      Chrono::milliseconds result = s1 % ms1;

      // Result should be in the common type (milliseconds)
      CHECK_EQUAL(1000, result.count()); // 10 seconds % 3000 milliseconds = 1000 milliseconds
    }

    //*************************************************************************
    TEST(test_modulus_with_large_values)
    {
      Chrono::seconds s1(1000000);      // 1,000,000 seconds
      Chrono::milliseconds ms1(500000); // 500,000 milliseconds

      Chrono::milliseconds result = s1 % ms1;

      CHECK_EQUAL(0, result.count()); // 1,000,000 seconds % 500,000 milliseconds = 0
    }

    //*************************************************************************
    TEST(test_modulus_resulting_in_fraction)
    {
      Chrono::seconds s1(7); // 7 seconds
      Chrono::seconds s2(3); // 3 seconds

      Chrono::seconds result = s1 % s2;

      CHECK_EQUAL(1, result.count()); // 7 % 3 = 1 second
    }

    //*************************************************************************
    TEST(test_modulus_by_larger_duration)
    {
      Chrono::seconds s1(5);  // 5 seconds
      Chrono::seconds s2(10); // 10 seconds

      Chrono::seconds result = s1 % s2;

      CHECK_EQUAL(5, result.count()); // 5 % 10 = 5 seconds
    }

    //*************************************************************************
    TEST(test_modulus_with_different_periods)
    {
      Chrono::seconds s1(10);         // 10 seconds
      Chrono::milliseconds ms1(2500); // 2500 milliseconds

      Chrono::milliseconds result = s1 % ms1;

      // Result should be in the common type (milliseconds)
      CHECK_EQUAL(0, result.count()); // 10 seconds % 2500 milliseconds = 0 milliseconds
    }

    //*************************************************************************
    TEST(test_operator_plus_equals_positive_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(5);  // 5 seconds

      s1 += s2;

      CHECK_EQUAL(15, s1.count()); // 10 + 5 = 15 seconds
    }

    //*************************************************************************
    TEST(test_operator_plus_equals_negative_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(-5); // -5 seconds

      s1 += s2;

      CHECK_EQUAL(5, s1.count()); // 10 + (-5) = 5 seconds
    }

    //*************************************************************************
    TEST(test_operator_minus_equals_positive_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(5);  // 5 seconds

      s1 -= s2;

      CHECK_EQUAL(5, s1.count()); // 10 - 5 = 5 seconds
    }

    //*************************************************************************
    TEST(test_operator_minus_equals_negative_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(-5); // -5 seconds

      s1 -= s2;

      CHECK_EQUAL(15, s1.count()); // 10 - (-5) = 15 seconds
    }

    //*************************************************************************
    TEST(test_operator_times_equals_positive_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 5;

      s1 *= scalar;

      CHECK_EQUAL(50, s1.count()); // 10 * 5 = 50 seconds
    }

    //*************************************************************************
    TEST(test_operator_times_equals_negative_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = -5;

      s1 *= scalar;

      CHECK_EQUAL(-50, s1.count()); // 10 * (-5) = -50 seconds
    }

    //*************************************************************************
    TEST(test_operator_divide_equals_positive_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 5;

      s1 /= scalar;

      CHECK_EQUAL(2, s1.count()); // 10 / 5 = 2 seconds
    }

    //*************************************************************************
    TEST(test_operator_divide_equals_negative_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = -5;

      s1 /= scalar;

      CHECK_EQUAL(-2, s1.count()); // 10 / (-5) = -2 seconds
    }

    //*************************************************************************
    TEST(test_operator_mod_equals_positive_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = 4;

      s1 %= scalar;

      CHECK_EQUAL(2, s1.count()); // 10 % 4 = 2 seconds
    }

    //*************************************************************************
    TEST(test_operator_mod_equals_negative_value)
    {
      Chrono::seconds s1(10); // 10 seconds
      int scalar = -4;

      s1 %= scalar;

      CHECK_EQUAL(2, s1.count()); // 10 % (-4) = 2 seconds
    }

    //*************************************************************************
    TEST(test_operator_mod_equals_positive_duration)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(3);  // 3 seconds

      s1 %= s2;

      CHECK_EQUAL(1, s1.count()); // 10 seconds % 3 seconds = 1 second
    }

    //*************************************************************************
    TEST(test_operator_mod_equals_negative_duration)
    {
      Chrono::seconds s1(10); // 10 seconds
      Chrono::seconds s2(-3); // 3 seconds

      s1 %= s2;

      CHECK_EQUAL(1, s1.count()); // 10 seconds % (-3) seconds = 1 second
    }

#if ETL_USING_CPP20
    //*************************************************************************
    TEST(test_duration_spaceship_operator)
    {
      Chrono::seconds s1(2);          // 2 seconds
      Chrono::milliseconds ms1(1500); // 1500 milliseconds

      CHECK_TRUE(s1 <=> s1  == std::strong_ordering::equal);
      CHECK_TRUE(ms1 <=> s1 == std::strong_ordering::less);
      CHECK_TRUE(s1 <=> ms1 == std::strong_ordering::greater);
    }
#endif

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_compare)
    {
      Chrono::seconds s1(2);          // 2 seconds
      Chrono::milliseconds ms1(1500); // 1500 milliseconds

      CHECK_EQUAL(0,  s1.compare(s1));
      CHECK_EQUAL(-1, ms1.compare(s1));
      CHECK_EQUAL(1,  s1.compare(ms1));
    }
#endif

    //*************************************************************************
    TEST(test_floor)
    {
      Chrono::milliseconds ms(1234); // 1234 milliseconds
      Chrono::seconds s = floor<Chrono::seconds>(ms); // Floor to seconds
      CHECK_EQUAL(1, s.count());

      Chrono::milliseconds negative_ms(-1234); // -1234 milliseconds
      Chrono::seconds negative_s = floor<Chrono::seconds>(negative_ms); // Floor to seconds
      CHECK_EQUAL(-2, negative_s.count());
    }

    //*************************************************************************
    TEST(test_ceil)
    {
      Chrono::milliseconds ms(1234); // 1234 milliseconds
      Chrono::seconds s = ceil<Chrono::seconds>(ms); // Ceil to seconds
      CHECK_EQUAL(2, s.count());

      Chrono::milliseconds negative_ms(-1234); // -1234 milliseconds
      Chrono::seconds negative_s = ceil<Chrono::seconds>(negative_ms); // Ceil to seconds
      CHECK_EQUAL(-1, negative_s.count());
    }

    //*************************************************************************
    TEST(test_round)
    {
      Chrono::milliseconds ms(1500); // 1500 milliseconds
      Chrono::seconds s = round<Chrono::seconds>(ms); // Round to seconds
      CHECK_EQUAL(2, s.count());

      Chrono::milliseconds ms_halfway(2500); // 2500 milliseconds
      Chrono::seconds s_halfway = round<Chrono::seconds>(ms_halfway); // Round to seconds
      CHECK_EQUAL(2, s_halfway.count());

      Chrono::milliseconds negative_ms(-1500); // -1500 milliseconds
      Chrono::seconds negative_s = round<Chrono::seconds>(negative_ms); // Round to seconds
      CHECK_EQUAL(-2, negative_s.count());
    }

    //*************************************************************************
    TEST(test_abs)
    {
      using namespace etl::chrono;

      milliseconds ms(1234); // 1234 milliseconds
      milliseconds abs_ms = abs(ms); // Absolute value
      CHECK_EQUAL(1234, abs_ms.count());

      milliseconds negative_ms(-1234); // -1234 milliseconds
      milliseconds abs_negative_ms = abs(negative_ms); // Absolute value
      CHECK_EQUAL(1234, abs_negative_ms.count());
    }

#if ETL_USING_ETL_CHRONO
    //*************************************************************************
    TEST(test_duration_literals)
    {
      using namespace etl::literals::chrono_literals;

#if ETL_USING_VERBOSE_CHRONO_LITERALS
      CHECK_TRUE(Chrono::hours(1)        == 1_hours);
      CHECK_TRUE(Chrono::minutes(2)      == 2_minutes);
      CHECK_TRUE(Chrono::seconds(3)      == 3_seconds);
      CHECK_TRUE(Chrono::milliseconds(4) == 4_milliseconds);
      CHECK_TRUE(Chrono::microseconds(5) == 5_microseconds);
      CHECK_TRUE(Chrono::nanoseconds(6)  == 6_nanoseconds);
#else
      CHECK_TRUE(Chrono::hours(1)        == 1_h);
      CHECK_TRUE(Chrono::minutes(2)      == 2_min);
      CHECK_TRUE(Chrono::seconds(3)      == 3_s);
      CHECK_TRUE(Chrono::milliseconds(4) == 4_ms);
      CHECK_TRUE(Chrono::microseconds(5) == 5_us);
      CHECK_TRUE(Chrono::nanoseconds(6)  == 6_ns);
#endif
    }
#endif
  };
}
