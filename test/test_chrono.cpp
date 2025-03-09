/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#include "unit_test_framework.h"

#include "etl/chrono.h"

#include <limits>
#include <type_traits>

namespace chrono = etl::chrono;

static uint64_t currentTimeNs = 0;

extern "C" uint64_t etl_get_system_time_nanosec()
{
  return currentTimeNs;
}

namespace chrono_test
{
  struct Clock
  {
    using duration = chrono::minutes;
  };

}  // namespace chrono_test

namespace etl
{
  namespace chrono
  {
    template struct duration_values<int32_t>;
    template struct duration<int32_t, etl::milli>;
    template struct time_point<chrono_test::Clock, duration<int32_t, etl::milli>>;

  }  // namespace chrono
}  // namespace etl

static_assert(
  std::is_same<chrono_test::Clock::duration, chrono::time_point<chrono_test::Clock>::duration>::
    value,
  "");
static_assert(
  std::is_same<chrono::nanoseconds, chrono::high_resolution_clock::duration>::value, "");
static_assert(
  std::is_same<chrono::nanoseconds::rep, chrono::high_resolution_clock::rep>::value, "");
static_assert(
  std::is_same<chrono::nanoseconds::period, chrono::high_resolution_clock::period>::value, "");

namespace
{
  SUITE(test_chrono)
  {
    TEST(TetlurationCast)
    {
      chrono::duration<int32_t, etl::milli> src(1001);
      {
        chrono::duration<int32_t, etl::micro> cut = chrono::duration_cast<chrono::duration<int32_t, etl::micro>>(src);
        CHECK_EQUAL(1001000, cut.count());
      }
      {
        chrono::duration<int32_t, etl::centi> cut = chrono::duration_cast<const chrono::duration<int32_t, etl::centi>>(src);
        CHECK_EQUAL(100, cut.count());
      }
      {
        chrono::duration<int16_t, etl::centi> cut = chrono::duration_cast<chrono::duration<int16_t, etl::centi>>(src);
        CHECK_EQUAL(100, cut.count());
      }
      {
        chrono::duration<int32_t, etl::milli> cut = chrono::duration_cast<const chrono::duration<int32_t, etl::milli>>(src);
        CHECK_EQUAL(1001, cut.count());
      }
      {
        chrono::duration<int32_t, etl::ratio<2, 15>> cut = chrono::duration_cast<chrono::duration<int32_t, etl::ratio<2, 15>>>(
          chrono::duration<int32_t, etl::ratio<3, 10>>(4));
        CHECK_EQUAL(9, cut.count());
      }
    }

    TEST(TetlurationConstructors)
    {
      {
        chrono::duration<int16_t, etl::milli> cut;
        CHECK_EQUAL(0, cut.count());
      }
      {
        chrono::duration<int32_t, etl::nano> cut(1000);
        CHECK_EQUAL(1000, cut.count());
      }
      {
        chrono::duration<int32_t, etl::micro> src(1000);
        chrono::duration<int32_t, etl::micro> cut(src);
        CHECK_EQUAL(1000, cut.count());
      }
      {
        chrono::duration<int64_t, etl::micro> src(1000);
        chrono::duration<int64_t, etl::nano>  cut(src);
        CHECK_EQUAL(1000000, cut.count());
      }
    }

    TEST(TetlurationAssignment)
    {
      chrono::duration<int32_t, etl::milli> src(1000);
      chrono::duration<int32_t, etl::milli> cut;
      cut = src;
      CHECK_EQUAL(1000, cut.count());
    }

    TEST(TetlurationUnaryPlusMinusOperators)
    {
      chrono::duration<int32_t, etl::milli> cut(1000);
      CHECK_EQUAL(1000, (+cut).count());
      CHECK_EQUAL(-1000, (-cut).count());
    }

    TEST(TetlurationIncrementDecrementOperators)
    {
      chrono::duration<int32_t, etl::milli> cut(1000);
      CHECK_EQUAL(1001, (++cut).count());
      CHECK_EQUAL(1001, cut.count());
      CHECK_EQUAL(1000, (--cut).count());
      CHECK_EQUAL(1000, cut.count());

      CHECK_EQUAL(1000, (cut++).count());
      CHECK_EQUAL(1001, cut.count());
      CHECK_EQUAL(1001, (cut--).count());
      CHECK_EQUAL(1000, cut.count());
    }

    TEST(TetlurationCompoundAssignmentOperators)
    {
      chrono::duration<int32_t, etl::milli> cut(1000);
      chrono::duration<int32_t, etl::milli> other(500);
      chrono::duration<int32_t, etl::milli> prime(737);

      CHECK_EQUAL(1500, (cut += other).count());
      CHECK_EQUAL(1500, cut.count());
      CHECK_EQUAL(1000, (cut -= other).count());
      CHECK_EQUAL(1000, cut.count());
      CHECK_EQUAL(3000, (cut *= 3).count());
      CHECK_EQUAL(3000, cut.count());
      CHECK_EQUAL(1000, (cut /= 3).count());
      CHECK_EQUAL(1000, cut.count());
      CHECK_EQUAL(263, (cut %= 737).count());
      CHECK_EQUAL(263, cut.count());
      CHECK_EQUAL(263, (cut %= prime).count());
      CHECK_EQUAL(263, cut.count());
    }

    TEST(TetlurationArithmeticOperators)
    {
      chrono::duration<int64_t, etl::milli> d1(500);
      chrono::duration<int32_t, etl::micro> d2(1000);
      chrono::duration<int16_t, etl::micro> d3(int16_t(250));
      chrono::duration<int32_t, etl::micro> prime(737);

      {
        chrono::duration<int64_t, etl::micro> cut = d1 + d2;
        CHECK_EQUAL(501000, cut.count());
      }
      {
        chrono::duration<int64_t, etl::micro> cut = d1 - d2;
        CHECK_EQUAL(499000, cut.count());
      }
      {
        chrono::duration<int64_t, etl::milli> cut = d1 * 100;
        CHECK_EQUAL(50000, cut.count());
      }
      {
        chrono::duration<int64_t, etl::milli> cut = 100 * d1;
        CHECK_EQUAL(50000, cut.count());
      }
      {
        CHECK_EQUAL(2000, d1 / d3);
      }
      {
        chrono::duration<int64_t, etl::milli> cut = d1 / 250;
        CHECK_EQUAL(2, cut.count());
      }
      {
        chrono::duration<int64_t, etl::micro> cut = d1 % prime;
        CHECK_EQUAL(int64_t(500000 % 737), cut.count());
      }
      {
        chrono::duration<int64_t, etl::milli> cut = d1 % 73;
        CHECK_EQUAL((500 % 73), cut.count());
      }
    }

    TEST(TetlurationComparisonOperators)
    {
      chrono::duration<int32_t, etl::milli> l1(999);
      chrono::duration<int32_t, etl::milli> g1(1000);
      chrono::duration<int32_t, etl::milli> g2(1000);

      CHECK_FALSE(l1 == g1);
      CHECK_FALSE(g1 == l1);
      CHECK_TRUE(g1 == g2);
      CHECK_TRUE(g2 == g1);

      CHECK_TRUE(l1 != g1);
      CHECK_TRUE(g1 != l1);
      CHECK_FALSE(g1 != g2);
      CHECK_FALSE(g2 != g1);

      CHECK_TRUE(l1 < g1);
      CHECK_FALSE(g1 < l1);
      CHECK_FALSE(g1 < g2);
      CHECK_FALSE(g2 < g1);

      CHECK_TRUE(l1 <= g1);
      CHECK_FALSE(g1 <= l1);
      CHECK_TRUE(g1 <= g2);
      CHECK_TRUE(g2 <= g1);

      CHECK_FALSE(l1 > g1);
      CHECK_TRUE(g1 > l1);
      CHECK_FALSE(g1 > g2);
      CHECK_FALSE(g2 > g1);

      CHECK_FALSE(l1 >= g1);
      CHECK_TRUE(g1 >= l1);
      CHECK_TRUE(g1 >= g2);
      CHECK_TRUE(g2 >= g1);
    }

    TEST(TetlurationConstants)
    {
      CHECK_EQUAL(0, (chrono::duration<int8_t, etl::milli>::zero().count()));
      CHECK_EQUAL(
        std::numeric_limits<int8_t>::min(),
        (chrono::duration<int8_t, etl::milli>::min().count()));
      CHECK_EQUAL(
        std::numeric_limits<int8_t>::max(),
        (chrono::duration<int8_t, etl::milli>::max().count()));

      CHECK_EQUAL(0U, (chrono::duration<uint8_t, etl::milli>::zero().count()));
      CHECK_EQUAL(
        std::numeric_limits<uint8_t>::min(),
        (chrono::duration<uint8_t, etl::milli>::min().count()));
      CHECK_EQUAL(
        std::numeric_limits<uint8_t>::max(),
        (chrono::duration<uint8_t, etl::milli>::max().count()));

      CHECK_EQUAL(0, (chrono::duration<int16_t, etl::milli>::zero().count()));
      CHECK_EQUAL(
        std::numeric_limits<int16_t>::min(),
        (chrono::duration<int16_t, etl::milli>::min().count()));
      CHECK_EQUAL(
        std::numeric_limits<int16_t>::max(),
        (chrono::duration<int16_t, etl::milli>::max().count()));

      CHECK_EQUAL(0U, (chrono::duration<uint16_t, etl::milli>::zero().count()));
      CHECK_EQUAL(
        std::numeric_limits<uint16_t>::min(),
        (chrono::duration<uint16_t, etl::milli>::min().count()));
      CHECK_EQUAL(
        std::numeric_limits<uint16_t>::max(),
        (chrono::duration<uint16_t, etl::milli>::max().count()));

      CHECK_EQUAL(0, (chrono::duration<int32_t, etl::milli>::zero().count()));
      CHECK_EQUAL(
        std::numeric_limits<int32_t>::min(),
        (chrono::duration<int32_t, etl::milli>::min().count()));
      CHECK_EQUAL(
        std::numeric_limits<int32_t>::max(),
        (chrono::duration<int32_t, etl::milli>::max().count()));

      CHECK_EQUAL(0U, (chrono::duration<uint32_t, etl::milli>::zero().count()));
      CHECK_EQUAL(
        std::numeric_limits<uint32_t>::min(),
        (chrono::duration<uint32_t, etl::milli>::min().count()));
      CHECK_EQUAL(
        std::numeric_limits<uint32_t>::max(),
        (chrono::duration<uint32_t, etl::milli>::max().count()));

      CHECK_EQUAL(0, (chrono::duration<int64_t, etl::milli>::zero().count()));
      CHECK_EQUAL(
        std::numeric_limits<int64_t>::min(),
        (chrono::duration<int64_t, etl::milli>::min().count()));
      CHECK_EQUAL(
        std::numeric_limits<int64_t>::max(),
        (chrono::duration<int64_t, etl::milli>::max().count()));

      CHECK_EQUAL(0U, (chrono::duration<uint64_t, etl::milli>::zero().count()));
      CHECK_EQUAL(
        std::numeric_limits<uint64_t>::min(),
        (chrono::duration<uint64_t, etl::milli>::min().count()));
      CHECK_EQUAL(
        std::numeric_limits<uint64_t>::max(),
        (chrono::duration<uint64_t, etl::milli>::max().count()));
    }

    TEST(TestPredefinedTypes)
    {
      {
        chrono::milliseconds cut = chrono::hours(3) + chrono::minutes(16) + chrono::seconds(35) + chrono::milliseconds(26);
        CHECK_EQUAL((((((3 * 60) + 16) * 60) + 35) * 1000 + 26), cut.count());
      }
      {
        chrono::nanoseconds cut = chrono::milliseconds(251) + chrono::microseconds(365) + chrono::nanoseconds(857);
        CHECK_EQUAL((((251 * 1000 + 365) * 1000) + 857), cut.count());
      }
    }

    TEST(TestTimePointCast)
    {
      {
        chrono::time_point<chrono_test::Clock, chrono::duration<int32_t, etl::milli>> src(
          chrono::duration<int32_t, etl::milli>(1001));
        {
          chrono::time_point<chrono_test::Clock, chrono::duration<int32_t, etl::micro>> cut = chrono::time_point_cast<chrono::duration<int32_t, etl::micro>>(src);
          CHECK_EQUAL(1001000, cut.time_since_epoch().count());
        }
      }
    }

    TEST(TestTimePointConstructors)
    {
      {
        chrono::time_point<chrono_test::Clock, chrono::milliseconds> cut;
        CHECK_EQUAL(0, cut.time_since_epoch().count());
      }
      {
        chrono::time_point<chrono_test::Clock, chrono::microseconds> cut(chrono::microseconds(715));
        CHECK_EQUAL(715, cut.time_since_epoch().count());
      }
      {
        chrono::time_point<chrono_test::Clock, chrono::microseconds> cut(
          chrono::time_point<chrono_test::Clock, chrono::duration<int16_t, etl::milli>>(
            chrono::duration<int16_t, etl::milli>(int16_t(715))));
        CHECK_EQUAL(715000, cut.time_since_epoch().count());
      }
      {
        chrono::time_point<chrono_test::Clock> cut;
        CHECK_EQUAL(0, cut.time_since_epoch().count());
      }
      {
        // copy constructor
        chrono::time_point<chrono_test::Clock, chrono::microseconds> cut(chrono::microseconds(715));
        chrono::time_point<chrono_test::Clock, chrono::microseconds> other(cut);
        CHECK_EQUAL(715, other.time_since_epoch().count());
      }
      {
        // assignment operator
        chrono::time_point<chrono_test::Clock, chrono::microseconds> cut(chrono::microseconds(715));
        chrono::time_point<chrono_test::Clock, chrono::microseconds> other(
          chrono::microseconds(716));

        other = cut;
        CHECK_EQUAL(715, other.time_since_epoch().count());
      }
    }

    TEST(TestTimePointCompoundAssignmentOperators)
    {
      chrono::time_point<chrono_test::Clock, chrono::milliseconds> cut(chrono::milliseconds(1000));
      chrono::milliseconds                                         duration = chrono::milliseconds(500);

      CHECK_EQUAL(1500, (cut += duration).time_since_epoch().count());
      CHECK_EQUAL(1500, cut.time_since_epoch().count());
      CHECK_EQUAL(1000, (cut -= duration).time_since_epoch().count());
      CHECK_EQUAL(1000, cut.time_since_epoch().count());
    }

    TEST(TestTimePointArithmeticOperators)
    {
      chrono::time_point<chrono_test::Clock, chrono::milliseconds> tp1(chrono::milliseconds(500));
      chrono::time_point<chrono_test::Clock, chrono::microseconds> tp2(chrono::microseconds(1000));
      chrono::microseconds                                         d(1000);

      {
        chrono::time_point<chrono_test::Clock, chrono::microseconds> cut = tp1 + d;
        CHECK_EQUAL(501000, cut.time_since_epoch().count());
      }
      {
        chrono::time_point<chrono_test::Clock, chrono::microseconds> cut = d + tp1;
        CHECK_EQUAL(501000, cut.time_since_epoch().count());
      }
      {
        chrono::time_point<chrono_test::Clock, chrono::microseconds> cut = tp1 - d;
        CHECK_EQUAL(499000, cut.time_since_epoch().count());
      }
      {
        chrono::microseconds cut = tp1 - tp2;
        CHECK_EQUAL(499000, cut.count());
      }
    }

    TEST(TestTimePointComparisonOperators)
    {
      chrono::time_point<chrono_test::Clock, chrono::milliseconds> l1(chrono::milliseconds(999));
      chrono::time_point<chrono_test::Clock, chrono::milliseconds> g1(chrono::milliseconds(1000));
      chrono::time_point<chrono_test::Clock, chrono::milliseconds> g2(chrono::milliseconds(1000));

      CHECK_FALSE(l1 == g1);
      CHECK_FALSE(g1 == l1);
      CHECK_TRUE(g1 == g2);
      CHECK_TRUE(g2 == g1);

      CHECK_TRUE(l1 != g1);
      CHECK_TRUE(g1 != l1);
      CHECK_FALSE(g1 != g2);
      CHECK_FALSE(g2 != g1);

      CHECK_TRUE(l1 < g1);
      CHECK_FALSE(g1 < l1);
      CHECK_FALSE(g1 < g2);
      CHECK_FALSE(g2 < g1);

      CHECK_TRUE(l1 <= g1);
      CHECK_FALSE(g1 <= l1);
      CHECK_TRUE(g1 <= g2);
      CHECK_TRUE(g2 <= g1);

      CHECK_FALSE(l1 > g1);
      CHECK_TRUE(g1 > l1);
      CHECK_FALSE(g1 > g2);
      CHECK_FALSE(g2 > g1);

      CHECK_FALSE(l1 >= g1);
      CHECK_TRUE(g1 >= l1);
      CHECK_TRUE(g1 >= g2);
      CHECK_TRUE(g2 >= g1);
    }

    TEST(TestTimePointConstants)
    {
      CHECK_EQUAL(
        std::numeric_limits<int8_t>::min(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<int8_t, etl::milli>>::min()
           .time_since_epoch()
           .count()));
      CHECK_EQUAL(
        std::numeric_limits<int8_t>::max(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<int8_t, etl::milli>>::max()
           .time_since_epoch()
           .count()));

      CHECK_EQUAL(
        std::numeric_limits<uint8_t>::min(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<uint8_t, etl::milli>>::min()
           .time_since_epoch()
           .count()));
      CHECK_EQUAL(
        std::numeric_limits<uint8_t>::max(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<uint8_t, etl::milli>>::max()
           .time_since_epoch()
           .count()));

      CHECK_EQUAL(
        std::numeric_limits<int16_t>::min(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<int16_t, etl::milli>>::min()
           .time_since_epoch()
           .count()));
      CHECK_EQUAL(
        std::numeric_limits<int16_t>::max(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<int16_t, etl::milli>>::max()
           .time_since_epoch()
           .count()));

      CHECK_EQUAL(
        std::numeric_limits<uint16_t>::min(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<uint16_t, etl::milli>>::min()
           .time_since_epoch()
           .count()));
      CHECK_EQUAL(
        std::numeric_limits<uint16_t>::max(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<uint16_t, etl::milli>>::max()
           .time_since_epoch()
           .count()));

      CHECK_EQUAL(
        std::numeric_limits<int32_t>::min(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<int32_t, etl::milli>>::min()
           .time_since_epoch()
           .count()));
      CHECK_EQUAL(
        std::numeric_limits<int32_t>::max(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<int32_t, etl::milli>>::max()
           .time_since_epoch()
           .count()));

      CHECK_EQUAL(
        std::numeric_limits<uint32_t>::min(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<uint32_t, etl::milli>>::min()
           .time_since_epoch()
           .count()));
      CHECK_EQUAL(
        std::numeric_limits<uint32_t>::max(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<uint32_t, etl::milli>>::max()
           .time_since_epoch()
           .count()));

      CHECK_EQUAL(
        std::numeric_limits<int64_t>::min(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<int64_t, etl::milli>>::min()
           .time_since_epoch()
           .count()));
      CHECK_EQUAL(
        std::numeric_limits<int64_t>::max(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<int64_t, etl::milli>>::max()
           .time_since_epoch()
           .count()));

      CHECK_EQUAL(
        std::numeric_limits<uint64_t>::min(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<uint64_t, etl::milli>>::min()
           .time_since_epoch()
           .count()));
      CHECK_EQUAL(
        std::numeric_limits<uint64_t>::max(),
        (chrono::time_point<chrono_test::Clock, chrono::duration<uint64_t, etl::milli>>::max()
           .time_since_epoch()
           .count()));
    }

    TEST(TestHighResolutionClock)
    {
      currentTimeNs = 15;
      CHECK_EQUAL(
        static_cast<int64_t>(currentTimeNs), chrono::high_resolution_clock::now().time_since_epoch().count());
      currentTimeNs = 0x7fffffffffffffff;
      CHECK_EQUAL(
        static_cast<int64_t>(currentTimeNs), chrono::high_resolution_clock::now().time_since_epoch().count());
    }
  }
}  // namespace
