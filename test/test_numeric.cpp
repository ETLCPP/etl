/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/deque.h"
#include "etl/list.h"
#include "etl/numeric.h"

#include <algorithm>
#include <array>
#include <deque>
#include <limits>
#include <list>
#include <numeric>
#include <vector>

namespace
{
  SUITE(test_numeric)
  {
    //*************************************************************************
    TEST(test_iota)
    {
      int data1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      int data2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

      etl::iota(std::begin(data2), std::end(data2), 0);

      bool are_same = std::equal(std::begin(data1), std::end(data1), std::begin(data2));

      CHECK(are_same);
    }

    //*************************************************************************
    TEST(test_midpoint_signed_integral)
    {
      CHECK_EQUAL(0, (etl::midpoint(0, 0)));
      CHECK_EQUAL(0, (etl::midpoint(0, 1)));
      CHECK_EQUAL(1, (etl::midpoint(1, 0)));

      CHECK_EQUAL(std::numeric_limits<int32_t>::max() / 2, (etl::midpoint(0, std::numeric_limits<int32_t>::max())));
      CHECK_EQUAL((std::numeric_limits<int32_t>::max() / 2) + 1, (etl::midpoint(std::numeric_limits<int32_t>::max(), 0)));

      CHECK_EQUAL(-1, (etl::midpoint(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max())));
      CHECK_EQUAL(0, (etl::midpoint(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min())));
    }

    //*************************************************************************
    TEST(test_midpoint_unsigned_integral)
    {
      CHECK_EQUAL(0, (etl::midpoint(0U, 0U)));
      CHECK_EQUAL(0, (etl::midpoint(0U, 1U)));
      CHECK_EQUAL(1, (etl::midpoint(1U, 0U)));

      CHECK_EQUAL((std::numeric_limits<uint32_t>::max() / 2U),
                  (etl::midpoint(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max())));
      CHECK_EQUAL((std::numeric_limits<uint32_t>::max() / 2U) + 1,
                  (etl::midpoint(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::min())));
    }

    //*************************************************************************
    TEST(test_midpoint_floating_point)
    {
      CHECK_CLOSE(0.0, (etl::midpoint(0.0, 0.0)), 0.001);

      CHECK_CLOSE(0.5, (etl::midpoint(0.0, 1.0)), 0.001);
      CHECK_CLOSE(0.5, (etl::midpoint(1.0, 0.0)), 0.001);

      CHECK_CLOSE(0.0, (etl::midpoint(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max())), 0.001);
      CHECK_CLOSE(0.0, (etl::midpoint(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max())), 0.001);
    }

    //*************************************************************************
    TEST(test_midpoint_pointer)
    {
      std::vector<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      CHECK_EQUAL(data[5], (*etl::midpoint(data.data(), data.data() + data.size())));
      CHECK_EQUAL(data[5], (*etl::midpoint(data.data() + data.size(), data.data())));
    }

    //*************************************************************************
    TEST(test_midpoint_etl_random_access_iterator)
    {
      std::array<int, 10> initial = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::deque<int, 10> data(initial.begin(), initial.end());

      etl::deque<int, 10>::iterator b = data.begin();
      etl::deque<int, 10>::iterator e = data.end();

      CHECK_EQUAL(data[5], (*etl::midpoint(b, e)));
      CHECK_EQUAL(data[5], (*etl::midpoint(e, b)));
    }

    //*************************************************************************
    TEST(test_midpoint_etl_bidirectional_iterator)
    {
      std::array<int, 10> initial = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      etl::list<int, 10>  data(initial.begin(), initial.end());

      etl::list<int, 10>::iterator b = data.begin();
      etl::list<int, 10>::iterator e = data.end();

      etl::list<int, 10>::iterator c = data.begin();
      etl::advance(c, 5);

      int v = *etl::midpoint(b, e);

      CHECK_EQUAL(*c, v);
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_midpoint_std_random_access_iterator)
    {
      std::deque<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      std::deque<int>::iterator b = data.begin();
      std::deque<int>::iterator e = data.end();

      CHECK_EQUAL(data[5], (*etl::midpoint(b, e)));
      CHECK_EQUAL(data[5], (*etl::midpoint(e, b)));
    }

    //*************************************************************************
    TEST(test_midpoint_std_bidirectional_iterator)
    {
      std::list<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      std::list<int>::iterator b = data.begin();
      std::list<int>::iterator e = data.end();

      std::list<int>::iterator c = data.begin();
      std::advance(c, 5);

      int v = *etl::midpoint(b, e);

      CHECK_EQUAL(*c, v);
    }
#endif

    //*************************************************************************
    TEST(test_lerp_floating_point)
    {
      CHECK_CLOSE(-10.0, etl::lerp(-10.0, 10.0, 0.0), 0.001);
      CHECK_CLOSE(-8.0, etl::lerp(-10.0, 10.0, 0.1), 0.001);
      CHECK_CLOSE(-6.0, etl::lerp(-10.0, 10.0, 0.2), 0.001);
      CHECK_CLOSE(-4.0, etl::lerp(-10.0, 10.0, 0.3), 0.001);
      CHECK_CLOSE(-2.0, etl::lerp(-10.0, 10.0, 0.4), 0.001);
      CHECK_CLOSE(0.0, etl::lerp(-10.0, 10.0, 0.5), 0.001);
      CHECK_CLOSE(2.0, etl::lerp(-10.0, 10.0, 0.6), 0.001);
      CHECK_CLOSE(4.0, etl::lerp(-10.0, 10.0, 0.7), 0.001);
      CHECK_CLOSE(6.0, etl::lerp(-10.0, 10.0, 0.8), 0.001);
      CHECK_CLOSE(8.0, etl::lerp(-10.0, 10.0, 0.9), 0.001);
      CHECK_CLOSE(10.0, etl::lerp(-10.0, 10.0, 1.0), 0.001);

      // Equal a & b
      CHECK_CLOSE(10.0, etl::lerp(10.0, 10.0, 1.0), 0.001);
    }

    //*************************************************************************
    TEST(test_lerp_at_least_one_integral)
    {
      CHECK_CLOSE(-10.0, etl::lerp(-10.0, 10.0, 0), 0.001);
      CHECK_CLOSE(-8.0, etl::lerp(-10, 10.0, 0.1), 0.001);
      CHECK_CLOSE(-6.0, etl::lerp(-10.0, 10, 0.2), 0.001);
      CHECK_CLOSE(-4.0, etl::lerp(-10, 10, 0.3), 0.001);
      CHECK_CLOSE(-2.0, etl::lerp(-10, 10.0, 0.4), 0.001);
      CHECK_CLOSE(0.0, etl::lerp(-10.0, 10, 0.5), 0.001);
      CHECK_CLOSE(2.0, etl::lerp(-10, 10, 0.6), 0.001);
      CHECK_CLOSE(4.0, etl::lerp(-10, 10.0, 0.7), 0.001);
      CHECK_CLOSE(6.0, etl::lerp(-10.0, 10, 0.8), 0.001);
      CHECK_CLOSE(8.0, etl::lerp(-10, 10, 0.9), 0.001);
      CHECK_CLOSE(10.0, etl::lerp(-10.0, 10.0, 1), 0.001);

      // Equal a & b
      CHECK_CLOSE(10.0, etl::lerp(10.0, 10.0, 1.0), 0.001);
      CHECK_CLOSE(10.0, etl::lerp(10, 10.0, 1.0), 0.001);
      CHECK_CLOSE(10.0, etl::lerp(10.0, 10, 1.0), 0.001);
      CHECK_CLOSE(10.0, etl::lerp(10.0, 10.0, 1), 0.001);
      CHECK_CLOSE(10.0, etl::lerp(10, 10, 1), 0.001);
    }

    //*************************************************************************
    TEST(test_add_sat_unsigned)
    {
      // Normal addition (no overflow)
      CHECK_EQUAL(uint8_t(0), etl::add_sat(uint8_t(0), uint8_t(0)));
      CHECK_EQUAL(uint8_t(3), etl::add_sat(uint8_t(1), uint8_t(2)));
      CHECK_EQUAL(uint8_t(254), etl::add_sat(uint8_t(127), uint8_t(127)));
      CHECK_EQUAL(uint8_t(255), etl::add_sat(uint8_t(200), uint8_t(55)));

      // Overflow saturates to max
      CHECK_EQUAL(uint8_t(255), etl::add_sat(uint8_t(255), uint8_t(1)));
      CHECK_EQUAL(uint8_t(255), etl::add_sat(uint8_t(1), uint8_t(255)));
      CHECK_EQUAL(uint8_t(255), etl::add_sat(uint8_t(255), uint8_t(255)));
      CHECK_EQUAL(uint8_t(255), etl::add_sat(uint8_t(200), uint8_t(200)));
      CHECK_EQUAL(uint8_t(255), etl::add_sat(uint8_t(128), uint8_t(128)));

      // Identity: adding zero
      CHECK_EQUAL(uint8_t(42), etl::add_sat(uint8_t(42), uint8_t(0)));
      CHECK_EQUAL(uint8_t(0), etl::add_sat(uint8_t(0), uint8_t(0)));
      CHECK_EQUAL(uint8_t(255), etl::add_sat(uint8_t(255), uint8_t(0)));

      // uint16_t
      CHECK_EQUAL(uint16_t(65535), etl::add_sat(uint16_t(65535), uint16_t(1)));
      CHECK_EQUAL(uint16_t(65535), etl::add_sat(uint16_t(65535), uint16_t(65535)));
      CHECK_EQUAL(uint16_t(1000), etl::add_sat(uint16_t(500), uint16_t(500)));

      // uint32_t
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::add_sat(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::add_sat(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max()));
      CHECK_EQUAL(uint32_t(100), etl::add_sat(uint32_t(60), uint32_t(40)));
    }

    //*************************************************************************
    TEST(test_add_sat_signed)
    {
      // Normal addition (no overflow)
      CHECK_EQUAL(int8_t(0), etl::add_sat(int8_t(0), int8_t(0)));
      CHECK_EQUAL(int8_t(3), etl::add_sat(int8_t(1), int8_t(2)));
      CHECK_EQUAL(int8_t(-3), etl::add_sat(int8_t(-1), int8_t(-2)));
      CHECK_EQUAL(int8_t(0), etl::add_sat(int8_t(1), int8_t(-1)));
      CHECK_EQUAL(int8_t(0), etl::add_sat(int8_t(-1), int8_t(1)));
      CHECK_EQUAL(int8_t(126), etl::add_sat(int8_t(63), int8_t(63)));

      // Positive overflow saturates to max
      CHECK_EQUAL(int8_t(127), etl::add_sat(int8_t(127), int8_t(1)));
      CHECK_EQUAL(int8_t(127), etl::add_sat(int8_t(1), int8_t(127)));
      CHECK_EQUAL(int8_t(127), etl::add_sat(int8_t(127), int8_t(127)));
      CHECK_EQUAL(int8_t(127), etl::add_sat(int8_t(100), int8_t(100)));

      // Negative overflow saturates to min
      CHECK_EQUAL(int8_t(-128), etl::add_sat(int8_t(-128), int8_t(-1)));
      CHECK_EQUAL(int8_t(-128), etl::add_sat(int8_t(-1), int8_t(-128)));
      CHECK_EQUAL(int8_t(-128), etl::add_sat(int8_t(-128), int8_t(-128)));
      CHECK_EQUAL(int8_t(-128), etl::add_sat(int8_t(-100), int8_t(-100)));

      // Mixed signs: no overflow possible
      CHECK_EQUAL(int8_t(-1), etl::add_sat(int8_t(127), int8_t(-128)));
      CHECK_EQUAL(int8_t(-1), etl::add_sat(int8_t(-128), int8_t(127)));
      CHECK_EQUAL(int8_t(27), etl::add_sat(int8_t(127), int8_t(-100)));
      CHECK_EQUAL(int8_t(-28), etl::add_sat(int8_t(-128), int8_t(100)));

      // Identity: adding zero
      CHECK_EQUAL(int8_t(42), etl::add_sat(int8_t(42), int8_t(0)));
      CHECK_EQUAL(int8_t(-42), etl::add_sat(int8_t(-42), int8_t(0)));
      CHECK_EQUAL(int8_t(127), etl::add_sat(int8_t(127), int8_t(0)));
      CHECK_EQUAL(int8_t(-128), etl::add_sat(int8_t(-128), int8_t(0)));

      // int16_t
      CHECK_EQUAL(int16_t(32767), etl::add_sat(int16_t(32767), int16_t(1)));
      CHECK_EQUAL(int16_t(-32768), etl::add_sat(int16_t(-32768), int16_t(-1)));
      CHECK_EQUAL(int16_t(32767), etl::add_sat(int16_t(32767), int16_t(32767)));
      CHECK_EQUAL(int16_t(-32768), etl::add_sat(int16_t(-32768), int16_t(-32768)));
      CHECK_EQUAL(int16_t(1000), etl::add_sat(int16_t(500), int16_t(500)));

      // int32_t
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::add_sat(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::add_sat(std::numeric_limits<int32_t>::min(), int32_t(-1)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::add_sat(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::add_sat(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min()));
      CHECK_EQUAL(int32_t(100), etl::add_sat(int32_t(60), int32_t(40)));
    }

    //*************************************************************************
    TEST(test_sub_sat_unsigned)
    {
      // Normal subtraction (no underflow)
      CHECK_EQUAL(uint8_t(0), etl::sub_sat(uint8_t(0), uint8_t(0)));
      CHECK_EQUAL(uint8_t(1), etl::sub_sat(uint8_t(3), uint8_t(2)));
      CHECK_EQUAL(uint8_t(0), etl::sub_sat(uint8_t(2), uint8_t(2)));
      CHECK_EQUAL(uint8_t(255), etl::sub_sat(uint8_t(255), uint8_t(0)));
      CHECK_EQUAL(uint8_t(128), etl::sub_sat(uint8_t(255), uint8_t(127)));

      // Underflow saturates to 0
      CHECK_EQUAL(uint8_t(0), etl::sub_sat(uint8_t(0), uint8_t(1)));
      CHECK_EQUAL(uint8_t(0), etl::sub_sat(uint8_t(0), uint8_t(255)));
      CHECK_EQUAL(uint8_t(0), etl::sub_sat(uint8_t(1), uint8_t(2)));
      CHECK_EQUAL(uint8_t(0), etl::sub_sat(uint8_t(100), uint8_t(200)));
      CHECK_EQUAL(uint8_t(0), etl::sub_sat(uint8_t(127), uint8_t(255)));

      // Identity: subtracting zero
      CHECK_EQUAL(uint8_t(42), etl::sub_sat(uint8_t(42), uint8_t(0)));
      CHECK_EQUAL(uint8_t(0), etl::sub_sat(uint8_t(0), uint8_t(0)));
      CHECK_EQUAL(uint8_t(255), etl::sub_sat(uint8_t(255), uint8_t(0)));

      // uint16_t
      CHECK_EQUAL(uint16_t(0), etl::sub_sat(uint16_t(0), uint16_t(1)));
      CHECK_EQUAL(uint16_t(0), etl::sub_sat(uint16_t(0), uint16_t(65535)));
      CHECK_EQUAL(uint16_t(100), etl::sub_sat(uint16_t(500), uint16_t(400)));

      // uint32_t
      CHECK_EQUAL(uint32_t(0), etl::sub_sat(uint32_t(0), uint32_t(1)));
      CHECK_EQUAL(uint32_t(0), etl::sub_sat(uint32_t(0), std::numeric_limits<uint32_t>::max()));
      CHECK_EQUAL(uint32_t(1), etl::sub_sat(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max() - 1));
      CHECK_EQUAL(uint32_t(20), etl::sub_sat(uint32_t(60), uint32_t(40)));
    }

    //*************************************************************************
    TEST(test_sub_sat_signed)
    {
      // Normal subtraction (no overflow)
      CHECK_EQUAL(int8_t(0), etl::sub_sat(int8_t(0), int8_t(0)));
      CHECK_EQUAL(int8_t(1), etl::sub_sat(int8_t(3), int8_t(2)));
      CHECK_EQUAL(int8_t(-1), etl::sub_sat(int8_t(2), int8_t(3)));
      CHECK_EQUAL(int8_t(0), etl::sub_sat(int8_t(1), int8_t(1)));
      CHECK_EQUAL(int8_t(-2), etl::sub_sat(int8_t(-1), int8_t(1)));
      CHECK_EQUAL(int8_t(2), etl::sub_sat(int8_t(1), int8_t(-1)));

      // Positive overflow: subtracting large negative from positive saturates to max
      CHECK_EQUAL(int8_t(127), etl::sub_sat(int8_t(127), int8_t(-1)));
      CHECK_EQUAL(int8_t(127), etl::sub_sat(int8_t(1), int8_t(-127)));
      CHECK_EQUAL(int8_t(127), etl::sub_sat(int8_t(127), int8_t(-128)));
      CHECK_EQUAL(int8_t(127), etl::sub_sat(int8_t(100), int8_t(-100)));

      // Negative overflow: subtracting large positive from negative saturates to min
      CHECK_EQUAL(int8_t(-128), etl::sub_sat(int8_t(-128), int8_t(1)));
      CHECK_EQUAL(int8_t(-128), etl::sub_sat(int8_t(-1), int8_t(127)));
      CHECK_EQUAL(int8_t(-128), etl::sub_sat(int8_t(-128), int8_t(127)));
      CHECK_EQUAL(int8_t(-128), etl::sub_sat(int8_t(-100), int8_t(100)));

      // No overflow when same sign
      CHECK_EQUAL(int8_t(0), etl::sub_sat(int8_t(-128), int8_t(-128)));
      CHECK_EQUAL(int8_t(-1), etl::sub_sat(int8_t(-128), int8_t(-127)));

      // Identity: subtracting zero
      CHECK_EQUAL(int8_t(42), etl::sub_sat(int8_t(42), int8_t(0)));
      CHECK_EQUAL(int8_t(-42), etl::sub_sat(int8_t(-42), int8_t(0)));
      CHECK_EQUAL(int8_t(127), etl::sub_sat(int8_t(127), int8_t(0)));
      CHECK_EQUAL(int8_t(-128), etl::sub_sat(int8_t(-128), int8_t(0)));

      // int16_t
      CHECK_EQUAL(int16_t(32767), etl::sub_sat(int16_t(32767), int16_t(-1)));
      CHECK_EQUAL(int16_t(-32768), etl::sub_sat(int16_t(-32768), int16_t(1)));
      CHECK_EQUAL(int16_t(32767), etl::sub_sat(int16_t(32767), int16_t(-32768)));
      CHECK_EQUAL(int16_t(-32768), etl::sub_sat(int16_t(-32768), int16_t(32767)));
      CHECK_EQUAL(int16_t(100), etl::sub_sat(int16_t(500), int16_t(400)));

      // int32_t
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::sub_sat(std::numeric_limits<int32_t>::max(), int32_t(-1)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::sub_sat(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::sub_sat(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::sub_sat(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(int32_t(20), etl::sub_sat(int32_t(60), int32_t(40)));
    }

    //*************************************************************************
    TEST(test_mul_sat_unsigned)
    {
      // Normal multiplication (no overflow)
      CHECK_EQUAL(uint8_t(0), etl::mul_sat(uint8_t(0), uint8_t(0)));
      CHECK_EQUAL(uint8_t(0), etl::mul_sat(uint8_t(0), uint8_t(255)));
      CHECK_EQUAL(uint8_t(0), etl::mul_sat(uint8_t(255), uint8_t(0)));
      CHECK_EQUAL(uint8_t(6), etl::mul_sat(uint8_t(2), uint8_t(3)));
      CHECK_EQUAL(uint8_t(1), etl::mul_sat(uint8_t(1), uint8_t(1)));
      CHECK_EQUAL(uint8_t(255), etl::mul_sat(uint8_t(255), uint8_t(1)));
      CHECK_EQUAL(uint8_t(250), etl::mul_sat(uint8_t(25), uint8_t(10)));

      // Overflow saturates to max
      CHECK_EQUAL(uint8_t(255), etl::mul_sat(uint8_t(255), uint8_t(2)));
      CHECK_EQUAL(uint8_t(255), etl::mul_sat(uint8_t(2), uint8_t(255)));
      CHECK_EQUAL(uint8_t(255), etl::mul_sat(uint8_t(255), uint8_t(255)));
      CHECK_EQUAL(uint8_t(255), etl::mul_sat(uint8_t(128), uint8_t(3)));
      CHECK_EQUAL(uint8_t(255), etl::mul_sat(uint8_t(16), uint8_t(16)));

      // uint16_t
      CHECK_EQUAL(uint16_t(65535), etl::mul_sat(uint16_t(65535), uint16_t(2)));
      CHECK_EQUAL(uint16_t(65535), etl::mul_sat(uint16_t(65535), uint16_t(65535)));
      CHECK_EQUAL(uint16_t(10000), etl::mul_sat(uint16_t(100), uint16_t(100)));

      // uint32_t
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::mul_sat(std::numeric_limits<uint32_t>::max(), uint32_t(2)));
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::mul_sat(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max()));
      CHECK_EQUAL(uint32_t(600), etl::mul_sat(uint32_t(20), uint32_t(30)));
    }

    //*************************************************************************
    TEST(test_mul_sat_signed)
    {
      // Normal multiplication (no overflow)
      CHECK_EQUAL(int8_t(0), etl::mul_sat(int8_t(0), int8_t(0)));
      CHECK_EQUAL(int8_t(0), etl::mul_sat(int8_t(0), int8_t(127)));
      CHECK_EQUAL(int8_t(0), etl::mul_sat(int8_t(0), int8_t(-128)));
      CHECK_EQUAL(int8_t(6), etl::mul_sat(int8_t(2), int8_t(3)));
      CHECK_EQUAL(int8_t(-6), etl::mul_sat(int8_t(2), int8_t(-3)));
      CHECK_EQUAL(int8_t(-6), etl::mul_sat(int8_t(-2), int8_t(3)));
      CHECK_EQUAL(int8_t(6), etl::mul_sat(int8_t(-2), int8_t(-3)));
      CHECK_EQUAL(int8_t(1), etl::mul_sat(int8_t(1), int8_t(1)));
      CHECK_EQUAL(int8_t(-1), etl::mul_sat(int8_t(1), int8_t(-1)));
      CHECK_EQUAL(int8_t(1), etl::mul_sat(int8_t(-1), int8_t(-1)));
      CHECK_EQUAL(int8_t(100), etl::mul_sat(int8_t(10), int8_t(10)));

      // Positive overflow: both positive
      CHECK_EQUAL(int8_t(127), etl::mul_sat(int8_t(127), int8_t(2)));
      CHECK_EQUAL(int8_t(127), etl::mul_sat(int8_t(2), int8_t(127)));
      CHECK_EQUAL(int8_t(127), etl::mul_sat(int8_t(127), int8_t(127)));
      CHECK_EQUAL(int8_t(127), etl::mul_sat(int8_t(64), int8_t(3)));

      // Positive overflow: both negative
      CHECK_EQUAL(int8_t(127), etl::mul_sat(int8_t(-128), int8_t(-2)));
      CHECK_EQUAL(int8_t(127), etl::mul_sat(int8_t(-2), int8_t(-128)));
      CHECK_EQUAL(int8_t(127), etl::mul_sat(int8_t(-128), int8_t(-128)));

      // Negative overflow: positive * negative
      CHECK_EQUAL(int8_t(-128), etl::mul_sat(int8_t(127), int8_t(-2)));
      CHECK_EQUAL(int8_t(-128), etl::mul_sat(int8_t(2), int8_t(-127)));
      CHECK_EQUAL(int8_t(-128), etl::mul_sat(int8_t(127), int8_t(-128)));

      // Negative overflow: negative * positive
      CHECK_EQUAL(int8_t(-128), etl::mul_sat(int8_t(-128), int8_t(2)));
      CHECK_EQUAL(int8_t(-128), etl::mul_sat(int8_t(-2), int8_t(127)));
      CHECK_EQUAL(int8_t(-128), etl::mul_sat(int8_t(-128), int8_t(127)));

      // int16_t
      CHECK_EQUAL(int16_t(32767), etl::mul_sat(int16_t(32767), int16_t(2)));
      CHECK_EQUAL(int16_t(-32768), etl::mul_sat(int16_t(32767), int16_t(-2)));
      CHECK_EQUAL(int16_t(32767), etl::mul_sat(int16_t(-32768), int16_t(-2)));
      CHECK_EQUAL(int16_t(-32768), etl::mul_sat(int16_t(-32768), int16_t(2)));
      CHECK_EQUAL(int16_t(10000), etl::mul_sat(int16_t(100), int16_t(100)));

      // int32_t
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::mul_sat(std::numeric_limits<int32_t>::max(), int32_t(2)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::mul_sat(std::numeric_limits<int32_t>::max(), int32_t(-2)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::mul_sat(std::numeric_limits<int32_t>::min(), int32_t(-2)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::mul_sat(std::numeric_limits<int32_t>::min(), int32_t(2)));
      CHECK_EQUAL(int32_t(600), etl::mul_sat(int32_t(20), int32_t(30)));
    }

    //*************************************************************************
    TEST(test_div_sat_unsigned)
    {
      // Normal division
      CHECK_EQUAL(uint8_t(0), etl::div_sat(uint8_t(0), uint8_t(1)));
      CHECK_EQUAL(uint8_t(0), etl::div_sat(uint8_t(0), uint8_t(255)));
      CHECK_EQUAL(uint8_t(1), etl::div_sat(uint8_t(1), uint8_t(1)));
      CHECK_EQUAL(uint8_t(3), etl::div_sat(uint8_t(6), uint8_t(2)));
      CHECK_EQUAL(uint8_t(127), etl::div_sat(uint8_t(255), uint8_t(2)));
      CHECK_EQUAL(uint8_t(255), etl::div_sat(uint8_t(255), uint8_t(1)));
      CHECK_EQUAL(uint8_t(0), etl::div_sat(uint8_t(1), uint8_t(2)));

      // uint16_t
      CHECK_EQUAL(uint16_t(32767), etl::div_sat(uint16_t(65535), uint16_t(2)));
      CHECK_EQUAL(uint16_t(65535), etl::div_sat(uint16_t(65535), uint16_t(1)));
      CHECK_EQUAL(uint16_t(100), etl::div_sat(uint16_t(1000), uint16_t(10)));

      // uint32_t
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::div_sat(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(1), etl::div_sat(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max()));
      CHECK_EQUAL(uint32_t(30), etl::div_sat(uint32_t(600), uint32_t(20)));
    }

    //*************************************************************************
    TEST(test_div_sat_signed)
    {
      // Normal division
      CHECK_EQUAL(int8_t(0), etl::div_sat(int8_t(0), int8_t(1)));
      CHECK_EQUAL(int8_t(0), etl::div_sat(int8_t(0), int8_t(-1)));
      CHECK_EQUAL(int8_t(1), etl::div_sat(int8_t(1), int8_t(1)));
      CHECK_EQUAL(int8_t(-1), etl::div_sat(int8_t(1), int8_t(-1)));
      CHECK_EQUAL(int8_t(-1), etl::div_sat(int8_t(-1), int8_t(1)));
      CHECK_EQUAL(int8_t(1), etl::div_sat(int8_t(-1), int8_t(-1)));
      CHECK_EQUAL(int8_t(3), etl::div_sat(int8_t(6), int8_t(2)));
      CHECK_EQUAL(int8_t(-3), etl::div_sat(int8_t(6), int8_t(-2)));
      CHECK_EQUAL(int8_t(-3), etl::div_sat(int8_t(-6), int8_t(2)));
      CHECK_EQUAL(int8_t(3), etl::div_sat(int8_t(-6), int8_t(-2)));
      CHECK_EQUAL(int8_t(127), etl::div_sat(int8_t(127), int8_t(1)));
      CHECK_EQUAL(int8_t(-127), etl::div_sat(int8_t(127), int8_t(-1)));
      CHECK_EQUAL(int8_t(-128), etl::div_sat(int8_t(-128), int8_t(1)));
      CHECK_EQUAL(int8_t(0), etl::div_sat(int8_t(1), int8_t(2)));

      // The only overflow case: min / -1 saturates to max
      CHECK_EQUAL(int8_t(127), etl::div_sat(int8_t(-128), int8_t(-1)));

      // int16_t
      CHECK_EQUAL(int16_t(32767), etl::div_sat(int16_t(-32768), int16_t(-1)));
      CHECK_EQUAL(int16_t(-32768), etl::div_sat(int16_t(-32768), int16_t(1)));
      CHECK_EQUAL(int16_t(100), etl::div_sat(int16_t(1000), int16_t(10)));

      // int32_t
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::div_sat(std::numeric_limits<int32_t>::min(), int32_t(-1)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::div_sat(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::div_sat(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(0), etl::div_sat(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min()));
      CHECK_EQUAL(int32_t(30), etl::div_sat(int32_t(600), int32_t(20)));
    }

    //*************************************************************************
    TEST(test_saturate_cast_unsigned_to_unsigned)
    {
      // Fits
      CHECK_EQUAL(uint8_t(42), (etl::saturate_cast<uint8_t>(uint16_t(42))));
      CHECK_EQUAL(uint8_t(0), (etl::saturate_cast<uint8_t>(uint16_t(0))));
      CHECK_EQUAL(uint8_t(255), (etl::saturate_cast<uint8_t>(uint16_t(255))));

      // Overflow: clamp to max
      CHECK_EQUAL(uint8_t(255), (etl::saturate_cast<uint8_t>(uint16_t(256))));
      CHECK_EQUAL(uint8_t(255), (etl::saturate_cast<uint8_t>(uint16_t(1000))));
      CHECK_EQUAL(uint8_t(255), (etl::saturate_cast<uint8_t>(uint16_t(65535))));

      // Widening (always fits)
      CHECK_EQUAL(uint16_t(42), (etl::saturate_cast<uint16_t>(uint8_t(42))));
      CHECK_EQUAL(uint16_t(255), (etl::saturate_cast<uint16_t>(uint8_t(255))));
    }

    //*************************************************************************
    TEST(test_saturate_cast_signed_to_signed)
    {
      // Fits
      CHECK_EQUAL(int8_t(42), (etl::saturate_cast<int8_t>(int16_t(42))));
      CHECK_EQUAL(int8_t(-42), (etl::saturate_cast<int8_t>(int16_t(-42))));
      CHECK_EQUAL(int8_t(0), (etl::saturate_cast<int8_t>(int16_t(0))));
      CHECK_EQUAL(int8_t(127), (etl::saturate_cast<int8_t>(int16_t(127))));
      CHECK_EQUAL(int8_t(-128), (etl::saturate_cast<int8_t>(int16_t(-128))));

      // Overflow: clamp to max
      CHECK_EQUAL(int8_t(127), (etl::saturate_cast<int8_t>(int16_t(128))));
      CHECK_EQUAL(int8_t(127), (etl::saturate_cast<int8_t>(int16_t(1000))));

      // Underflow: clamp to min
      CHECK_EQUAL(int8_t(-128), (etl::saturate_cast<int8_t>(int16_t(-129))));
      CHECK_EQUAL(int8_t(-128), (etl::saturate_cast<int8_t>(int16_t(-1000))));

      // Widening (always fits)
      CHECK_EQUAL(int16_t(42), (etl::saturate_cast<int16_t>(int8_t(42))));
      CHECK_EQUAL(int16_t(-128), (etl::saturate_cast<int16_t>(int8_t(-128))));
    }

    //*************************************************************************
    TEST(test_saturate_cast_signed_to_unsigned)
    {
      // Fits
      CHECK_EQUAL(uint8_t(42), (etl::saturate_cast<uint8_t>(int16_t(42))));
      CHECK_EQUAL(uint8_t(0), (etl::saturate_cast<uint8_t>(int16_t(0))));
      CHECK_EQUAL(uint8_t(255), (etl::saturate_cast<uint8_t>(int16_t(255))));

      // Negative source: clamp to 0
      CHECK_EQUAL(uint8_t(0), (etl::saturate_cast<uint8_t>(int16_t(-1))));
      CHECK_EQUAL(uint8_t(0), (etl::saturate_cast<uint8_t>(int16_t(-1000))));
      CHECK_EQUAL(uint16_t(0), (etl::saturate_cast<uint16_t>(int16_t(-1))));

      // Overflow: clamp to max
      CHECK_EQUAL(uint8_t(255), (etl::saturate_cast<uint8_t>(int16_t(256))));
      CHECK_EQUAL(uint8_t(255), (etl::saturate_cast<uint8_t>(int16_t(1000))));
    }

    //*************************************************************************
    TEST(test_saturate_cast_unsigned_to_signed)
    {
      // Fits
      CHECK_EQUAL(int8_t(42), (etl::saturate_cast<int8_t>(uint16_t(42))));
      CHECK_EQUAL(int8_t(0), (etl::saturate_cast<int8_t>(uint16_t(0))));
      CHECK_EQUAL(int8_t(127), (etl::saturate_cast<int8_t>(uint16_t(127))));

      // Overflow: clamp to max
      CHECK_EQUAL(int8_t(127), (etl::saturate_cast<int8_t>(uint16_t(128))));
      CHECK_EQUAL(int8_t(127), (etl::saturate_cast<int8_t>(uint16_t(1000))));
      CHECK_EQUAL(int8_t(127), (etl::saturate_cast<int8_t>(uint16_t(65535))));

      // Widening (always fits)
      CHECK_EQUAL(int16_t(255), (etl::saturate_cast<int16_t>(uint8_t(255))));
      CHECK_EQUAL(int16_t(0), (etl::saturate_cast<int16_t>(uint8_t(0))));
    }

    //*************************************************************************
    TEST(test_saturate_cast_same_type)
    {
      // Same type, no conversion needed
      CHECK_EQUAL(int32_t(42), (etl::saturate_cast<int32_t>(int32_t(42))));
      CHECK_EQUAL(uint32_t(42), (etl::saturate_cast<uint32_t>(uint32_t(42))));
      CHECK_EQUAL(int32_t(-42), (etl::saturate_cast<int32_t>(int32_t(-42))));
    }

    //*************************************************************************
    TEST(test_saturate_cast_boundary_values)
    {
      // int32_t max -> uint16_t
      CHECK_EQUAL(uint16_t(65535), (etl::saturate_cast<uint16_t>(int32_t(2147483647))));

      // int32_t min -> uint16_t
      CHECK_EQUAL(uint16_t(0), (etl::saturate_cast<uint16_t>(std::numeric_limits<int32_t>::min())));

      // uint32_t max -> int32_t
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), (etl::saturate_cast<int32_t>(std::numeric_limits<uint32_t>::max())));

      // uint32_t max -> int16_t
      CHECK_EQUAL(int16_t(32767), (etl::saturate_cast<int16_t>(std::numeric_limits<uint32_t>::max())));
    }
  }
} // namespace
