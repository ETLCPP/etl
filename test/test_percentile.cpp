/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#include "etl/algorithm.h"
#include "etl/histogram.h"
#include "etl/percentile.h"

#include <array>

namespace
{
  constexpr size_t Size = 10U;
 
  etl::histogram<int32_t, int, Size, 0> histogram;

  std::array<int, 10> input1a = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  std::array<int, 5>  input1b = { 15, 20, 35, 40, 50 };
  std::array<int, 10> input1c = { 3, 6, 7, 8, 8, 10, 13, 15, 16, 20 };
  std::array<int, 11> input1d = { 3, 6, 7, 8, 8, 9, 10, 13, 15, 16, 20 };

  //***********************************
  std::array<int, 55> input2 = 
  { 
    5, 5, 5, 5, 5, 5,
    4, 4, 4, 4, 4, 
    6, 6, 6, 6, 6, 6, 6, 
    3, 3, 3, 3, 
    7, 7, 7, 7, 7, 7, 7, 7, 
    2, 2, 2,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 
    1, 1,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    0 
  };

  SUITE(test_percentile)
  {
    //*************************************************************************
    TEST(test_array_percentile)
    {
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 0U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 1U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 2U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 3U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 4U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 5U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 6U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 7U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 8U));
      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 9U));

      CHECK_EQUAL(0, *etl::percentile(input1a.begin(), input1a.end(), 10U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 11U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 12U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 13U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 14U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 15U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 16U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 17U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 18U));
      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 19U));

      CHECK_EQUAL(1, *etl::percentile(input1a.begin(), input1a.end(), 20U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 21U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 22U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 23U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 24U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 25U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 26U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 27U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 28U));
      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 29U));

      CHECK_EQUAL(2, *etl::percentile(input1a.begin(), input1a.end(), 30U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 31U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 32U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 33U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 34U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 35U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 36U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 37U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 38U));
      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 39U));

      CHECK_EQUAL(3, *etl::percentile(input1a.begin(), input1a.end(), 40U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 41U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 42U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 43U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 44U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 45U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 46U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 47U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 48U));
      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 49U));

      CHECK_EQUAL(4, *etl::percentile(input1a.begin(), input1a.end(), 50U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 51U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 52U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 53U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 54U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 55U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 56U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 57U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 58U));
      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 59U));

      CHECK_EQUAL(5, *etl::percentile(input1a.begin(), input1a.end(), 60U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 61U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 62U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 63U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 64U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 65U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 66U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 67U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 68U));
      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 69U));

      CHECK_EQUAL(6, *etl::percentile(input1a.begin(), input1a.end(), 70U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 71U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 72U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 73U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 74U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 75U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 76U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 77U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 78U));
      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 79U));

      CHECK_EQUAL(7, *etl::percentile(input1a.begin(), input1a.end(), 80U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 81U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 82U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 83U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 84U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 85U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 86U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 87U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 88U));
      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 89U));

      CHECK_EQUAL(8, *etl::percentile(input1a.begin(), input1a.end(), 90U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 91U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 92U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 93U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 94U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 95U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 96U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 97U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 98U));
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 99U));
      
      CHECK_EQUAL(9, *etl::percentile(input1a.begin(), input1a.end(), 100U));
    }

    //*************************************************************************
    TEST(test_array_percentile_from_wikipedia_1)
    {
      CHECK_EQUAL(15, *etl::percentile(input1b.begin(), input1b.end(), 0U));
      CHECK_EQUAL(15, *etl::percentile(input1b.begin(), input1b.end(), 5U));
      CHECK_EQUAL(20, *etl::percentile(input1b.begin(), input1b.end(), 30U));
      CHECK_EQUAL(20, *etl::percentile(input1b.begin(), input1b.end(), 40U));
      CHECK_EQUAL(35, *etl::percentile(input1b.begin(), input1b.end(), 50U));
      CHECK_EQUAL(50, *etl::percentile(input1b.begin(), input1b.end(), 100U));
    }

    //*************************************************************************
    TEST(test_array_percentile_from_wikipedia_2)
    {
      CHECK_EQUAL(7,  *etl::percentile(input1c.begin(), input1c.end(), 25U));
      CHECK_EQUAL(8,  *etl::percentile(input1c.begin(), input1c.end(), 50U));
      CHECK_EQUAL(15, *etl::percentile(input1c.begin(), input1c.end(), 75U));
      CHECK_EQUAL(20, *etl::percentile(input1c.begin(), input1c.end(), 100U));
    }

    //*************************************************************************
    TEST(test_array_percentile_from_wikipedia_3)
    {
      CHECK_EQUAL(7,  *etl::percentile(input1d.begin(), input1d.end(), 25U));
      CHECK_EQUAL(9,  *etl::percentile(input1d.begin(), input1d.end(), 50U));
      CHECK_EQUAL(15, *etl::percentile(input1d.begin(), input1d.end(), 75U));
      CHECK_EQUAL(20, *etl::percentile(input1d.begin(), input1d.end(), 100U));
    }

    //*************************************************************************
    TEST(test_histogram_percentile)
    {
      histogram(input2.begin(), input2.end());

      CHECK((etl::histogram_percentile(histogram.begin(), histogram.end(), 0U)) == histogram.end());
      CHECK_EQUAL(5, *etl::histogram_percentile(histogram.begin(), histogram.end(), 25U));
      CHECK_EQUAL(7, *etl::histogram_percentile(histogram.begin(), histogram.end(), 50U));
      CHECK_EQUAL(9, *etl::histogram_percentile(histogram.begin(), histogram.end(), 75U));
      CHECK_EQUAL(10, *etl::histogram_percentile(histogram.begin(), histogram.end(), 100U));
    }
  };
}
