/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include "etl/numeric.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <deque>
#include <list>

namespace
{		
  SUITE(test_numeric)
  {
    //*************************************************************************
    TEST(test_iota)
    {
      int data1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      etl::iota(std::begin(data2), std::end(data2), 0);

      bool are_same = std::equal(std::begin(data1), std::end(data1), std::begin(data2));

      CHECK(are_same);
    }

    //*************************************************************************
    TEST(test_midpoint_signed_integral)
    {
      CHECK_EQUAL(int32_t(0), (etl::midpoint(int32_t(0), int32_t(0))));
      CHECK_EQUAL(int32_t(0), (etl::midpoint(int32_t(0), int32_t(1))));
      CHECK_EQUAL(int32_t(1), (etl::midpoint(int32_t(1), int32_t(0))));

      CHECK_EQUAL(std::numeric_limits<int32_t>::max() / 2, (etl::midpoint(0, std::numeric_limits<int32_t>::max())));
      CHECK_EQUAL((std::numeric_limits<int32_t>::max() / 2) + 1, (etl::midpoint(std::numeric_limits<int32_t>::max(), 0)));
      
      CHECK_EQUAL(int32_t(-1), (etl::midpoint(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max())));
      CHECK_EQUAL(int32_t(0), (etl::midpoint(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min())));
    }

    //*************************************************************************
    TEST(test_midpoint_unsigned_integral)
    {
      CHECK_EQUAL(uint32_t(0), (etl::midpoint(uint32_t(0), uint32_t(0))));
      CHECK_EQUAL(uint32_t(0), (etl::midpoint(uint32_t(0), uint32_t(1))));
      CHECK_EQUAL(uint32_t(1), (etl::midpoint(uint32_t(1), uint32_t(0))));

      CHECK_EQUAL((std::numeric_limits<uint32_t>::max() / 2U), (etl::midpoint(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max())));
      CHECK_EQUAL((std::numeric_limits<uint32_t>::max() / 2U) + 1, (etl::midpoint(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::min())));
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
      std::vector<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data[5], (*etl::midpoint(data.data(), data.data() + data.size())));
      CHECK_EQUAL(data[5], (*etl::midpoint(data.data() + data.size(), data.data())));
    }

    //*************************************************************************
    TEST(test_midpoint_random_access_iterator)
    {
      std::deque<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      std::deque<int>::iterator b = data.begin();
      std::deque<int>::iterator e = data.end();

      CHECK_EQUAL(data[5], (*etl::midpoint(b, e)));
      CHECK_EQUAL(data[5], (*etl::midpoint(e, b)));
    }

    //*************************************************************************
    TEST(test_midpoint_bidirectional_iterator)
    {
      std::list<int> data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      std::list<int>::iterator b = data.begin();
      std::list<int>::iterator e = data.end();

      std::list<int>::iterator c = data.begin();
      std::advance(c, 5);

      CHECK_EQUAL(*c, *etl::midpoint(b, e));
    }

    //*************************************************************************
    TEST(test_lerp_floating_point)
    {
      CHECK(false);
    }

    //*************************************************************************
    TEST(test_lerp_at_least_one_integral)
    {
      CHECK(false);
    }
  };
}
