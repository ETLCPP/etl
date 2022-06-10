/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include <string>
#include <array>
#include <algorithm>
#include <map>

namespace
{
  constexpr size_t Size = 10UL;
  constexpr int    Start = -4;

  using IntRuntimeOffsetHistogram = etl::histogram<int32_t, int8_t, Size>;
  using IntOffset0Histogram       = etl::histogram<int32_t, int8_t, Size, 0>;
  using IntOffsetminus4Histogram  = etl::histogram<int32_t, int8_t, Size, Start>;
  using StringHistogram           = etl::sparse_histogram<std::string, int8_t, Size>;

  //***********************************
  std::array<int8_t, 55> input1 =
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

  //***********************************
  std::array<int8_t, 55> input2 =
  {
    1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0,
    2, 2, 2, 2, 2, 2, 2,
    -1, -1, -1, -1,
    3, 3, 3, 3, 3, 3, 3, 3,
    -2, -2, -2,
    4, 4, 4, 4, 4, 4, 4, 4, 4,
    -3, -3,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    -4
  };

  //***********************************
  std::array<std::string, 55> input3 =
  {
    "5", "5", "5", "5", "5", "5",
    "4", "4", "4", "4", "4",
    "6", "6", "6", "6", "6", "6", "6",
    "3", "3", "3", "3",
    "7", "7", "7", "7", "7", "7", "7", "7",
    "2", "2", "2",
    "8", "8", "8", "8", "8", "8", "8", "8", "8",
    "1", "1",
    "9", "9", "9", "9", "9", "9", "9", "9", "9", "9",
    "0"
  };

  //***********************************
  std::array<int8_t, Size> output1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  //***********************************
  std::array<ETL_OR_STD::pair<const std::string, int8_t>, Size> output2 =
  {
    ETL_OR_STD::pair<const std::string, int8_t>("0",  1),
    ETL_OR_STD::pair<const std::string, int8_t>("1",  2),
    ETL_OR_STD::pair<const std::string, int8_t>("2",  3),
    ETL_OR_STD::pair<const std::string, int8_t>("3",  4),
    ETL_OR_STD::pair<const std::string, int8_t>("4",  5),
    ETL_OR_STD::pair<const std::string, int8_t>("5",  6),
    ETL_OR_STD::pair<const std::string, int8_t>("6",  7),
    ETL_OR_STD::pair<const std::string, int8_t>("7",  8),
    ETL_OR_STD::pair<const std::string, int8_t>("8",  9),
    ETL_OR_STD::pair<const std::string, int8_t>("9", 10),
  };

  //***********************************
  std::array<int8_t, Size> zero1 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  //***********************************
  std::array<ETL_OR_STD::pair<const std::string, int8_t>, Size> zero2 =
  {
    ETL_OR_STD::pair<const std::string, int8_t>("0", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("1", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("2", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("3", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("4", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("5", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("6", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("7", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("8", 0),
    ETL_OR_STD::pair<const std::string, int8_t>("9", 0),
  };

  SUITE(test_histogram)
  {
    //*************************************************************************
    TEST(test_int_offset_0_histogram_constructor)
    {
      IntOffset0Histogram histogram;

      CHECK_EQUAL(0U,   histogram.count());
      CHECK_EQUAL(Size, histogram.size());

      bool isEqual = std::equal(zero1.begin(), zero1.end(), histogram.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_int_offset_0_histogram)
    {
      IntOffset0Histogram histogram(input1.begin(), input1.end());

      CHECK_EQUAL(Size, histogram.size());

      bool isEqual;

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram.begin());
      CHECK(!isEqual);

      isEqual = std::equal(output1.begin(), output1.end(), histogram.begin());
      CHECK(isEqual);

      CHECK_EQUAL(55U, histogram.count());

      histogram.clear();

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram.begin());
      CHECK(isEqual);
      CHECK_EQUAL(0U, histogram.count());
    }

    //*************************************************************************
    TEST(test_int_functor1_offset_0_histogram)
    {
      IntOffset0Histogram histogram;

      histogram = std::for_each(input1.begin(), input1.end(), histogram);

      CHECK_EQUAL(Size, histogram.size());

      bool isEqual;

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram.begin());
      CHECK(!isEqual);

      isEqual = std::equal(output1.begin(), output1.end(), histogram.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_int_functor2_offset_0_histogram)
    {
      IntOffset0Histogram histogram;

      histogram(input1.begin(), input1.end());

      CHECK_EQUAL(Size, histogram.size());

      bool isEqual;

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram.begin());
      CHECK(!isEqual);

      isEqual = std::equal(output1.begin(), output1.end(), histogram.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_int_offset_0_histogram_indexed)
    {
      IntOffset0Histogram histogram(input1.begin(), input1.end());

      CHECK_EQUAL(Size, histogram.size());

      for (size_t i = 0UL; i < output1.size(); ++i)
      {
        CHECK_EQUAL(int(output1[i]), int(histogram[i]));
      }
    }

    //*************************************************************************
    TEST(test_int_offset_minus_4_histogram)
    {
      IntOffsetminus4Histogram histogram(input2.begin(), input2.end());

      CHECK_EQUAL(Size, histogram.size());

      bool isEqual;

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram.begin());
      CHECK(!isEqual);

      isEqual = std::equal(output1.begin(), output1.end(), histogram.begin());
      CHECK(isEqual);

      CHECK_EQUAL(55U, histogram.count());

      histogram.clear();

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram.begin());
      CHECK(isEqual);
      CHECK_EQUAL(0U, histogram.count());
    }

    //*************************************************************************
    TEST(test_int_offset_minus_4_histogram_indexed)
    {
      IntOffsetminus4Histogram histogram(input2.begin(), input2.end());

      CHECK_EQUAL(Size, histogram.size());

      for (size_t i = 0UL; i < output1.size(); ++i)
      {
        CHECK_EQUAL(int(output1[i]), int(histogram[i]));
      }
    }

    //*************************************************************************
    TEST(test_int_runtime_offset_minus_4_histogram)
    {
      IntRuntimeOffsetHistogram histogram1(Start);
      IntRuntimeOffsetHistogram histogram2(Start, input2.begin(), input2.end());

      histogram1.add(input2.begin(), input2.end());

      CHECK_EQUAL(Size, histogram1.size());
      CHECK_EQUAL(Size, histogram2.size());

      bool isEqual;

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram1.begin());
      CHECK(!isEqual);

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram2.begin());
      CHECK(!isEqual);

      isEqual = std::equal(output1.begin(), output1.end(), histogram1.begin());
      CHECK(isEqual);

      isEqual = std::equal(output1.begin(), output1.end(), histogram2.begin());
      CHECK(isEqual);

      CHECK_EQUAL(55U, histogram1.count());
      CHECK_EQUAL(55U, histogram2.count());

      histogram1.clear();

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram1.begin());
      CHECK(isEqual);
      CHECK_EQUAL(0U, histogram1.count());

      histogram2.clear();

      isEqual = std::equal(zero1.begin(), zero1.end(), histogram2.begin());
      CHECK(isEqual);
      CHECK_EQUAL(0U, histogram2.count());
    }

    //*************************************************************************
    TEST(test_string_histogram_constructor)
    {
      StringHistogram histogram;

      CHECK_EQUAL(0U, histogram.size());
      CHECK_EQUAL(0U, histogram.count());
    }

    //*************************************************************************
    TEST(test_string_histogram)
    {
      StringHistogram histogram(input3.begin(), input3.end());

      CHECK_EQUAL(Size, histogram.size());
      CHECK_EQUAL(55U,  histogram.count());

      bool isEqual;

      isEqual = std::equal(zero2.begin(), zero2.end(), histogram.begin());
      CHECK(!isEqual);

      auto o1 = *output2.begin();
      auto h1 = *histogram.begin();

      isEqual = std::equal(output2.begin(), output2.end(), histogram.begin());
      CHECK(isEqual);

      CHECK_EQUAL(55U, histogram.count());

      histogram.clear();

      CHECK(isEqual);
      CHECK_EQUAL(0U, histogram.size());
      CHECK_EQUAL(0U, histogram.count());
    }

    //*************************************************************************
    TEST(test_string_histogram_unused_keys)
    {
      StringHistogram histogram(input3.begin(), input3.end());

      CHECK(histogram["5"] == output2[5]);

      StringHistogram::value_type unused = histogram[std::string("99")];
      CHECK(unused.first  == std::string());
      CHECK(unused.second == 0U);
    }

    //*************************************************************************
    TEST(test_string_functor1_histogram)
    {
      StringHistogram histogram;

      histogram = std::for_each(input3.begin(), input3.end(), histogram);

      CHECK_EQUAL(Size, histogram.size());

      bool isEqual;

      isEqual = std::equal(zero2.begin(), zero2.end(), histogram.begin());
      CHECK(!isEqual);

      isEqual = std::equal(output2.begin(), output2.end(), histogram.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_string_functor2_histogram)
    {
      StringHistogram histogram;

      histogram(input3.begin(), input3.end());

      CHECK_EQUAL(Size, histogram.size());

      bool isEqual;

      isEqual = std::equal(zero2.begin(), zero2.end(), histogram.begin());
      CHECK(!isEqual);

      isEqual = std::equal(output2.begin(), output2.end(), histogram.begin());
      CHECK(isEqual);
    }
  };
}
