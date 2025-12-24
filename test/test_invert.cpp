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

#include "etl/invert.h"

#include <array>
#include <algorithm>
#include <math.h>

namespace
{
  constexpr size_t Size = 10UL;

  using IntInvert    = etl::invert<int>;
  using DoubleInvert = etl::invert<double>;

  struct Compare
  {
    bool operator ()(double lhs, double rhs) const
    {
      return fabs(lhs - rhs) < 0.1;
    }
  };

  //***********************************
  const std::array<int, Size> input1 =
  {
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19
  };

  const std::array<int, Size> result1a =
  {
    90, 89, 88, 87, 86, 85, 84, 83, 82, 81
  };

  const std::array<int, Size> result1b =
  {
    100, 99, 98, 97, 96, 95, 94, 93, 92, 91
  };

  std::array<int, Size> output1;

  //***********************************
  const std::array<double, Size> input2 =
  {
    10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0
  };

  const std::array<double, Size> result2a =
  {
    90.0, 89.0, 88.0, 87.0, 86.0, 85.0, 84.0, 83.0, 82.0, 81.0
  };

  const std::array<double, Size> result2b =
  {
    100.0, 99.0, 98.0, 97.0, 96.0, 95.0, 94.0, 93.0, 92.0, 91.0
  };

  std::array<double, Size> output2;

  SUITE(test_invert)
  {
    //*************************************************************************
    TEST(test_int_invert)
    {
      IntInvert invert(0, 100);

      std::transform(input1.begin(), input1.end(), output1.begin(), invert);

      bool isEqual = std::equal(output1.begin(), output1.end(), result1a.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_int_invert_offset)
    {
      IntInvert invert(10, 100);

      std::transform(input1.begin(), input1.end(), output1.begin(), invert);

      bool isEqual = std::equal(output1.begin(), output1.end(), result1b.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_double_invert)
    {
      DoubleInvert invert(0, 100);

      std::transform(input2.begin(), input2.end(), output2.begin(), invert);

      bool isEqual = std::equal(output2.begin(), output2.end(), result2a.begin(), Compare());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_double_invert_offset)
    {
      DoubleInvert invert(10, 100);

      std::transform(input2.begin(), input2.end(), output2.begin(), invert);

      bool isEqual = std::equal(output2.begin(), output2.end(), result2b.begin(), Compare());
      CHECK(isEqual);
    }
  };
}
