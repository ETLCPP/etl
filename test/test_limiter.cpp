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

#include "etl/limiter.h"

#include <array>
#include <algorithm>
#include <math.h>

namespace
{
  constexpr size_t Size = 10UL;

  using IntLimiter    = etl::limiter<int>;
  using DoubleLimiter = etl::limiter<double>;

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
    13, 13, 13, 13, 14, 15, 16, 16, 16, 16
  };

  std::array<int, Size> output1;

  //***********************************
  const std::array<double, Size> input2 =
  {
    10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0
  };

  const std::array<double, Size> result2a =
  {
    13.0, 13.0, 13.0, 13.0, 14.0, 15.0, 16.0, 16.0, 16.0, 16.0
  };

  std::array<double, Size> output2;

  SUITE(test_limiter)
  {
    //*************************************************************************
    TEST(test_int_limiter)
    {
      IntLimiter limiter(13, 16);

      std::transform(input1.begin(), input1.end(), output1.begin(), limiter);

      bool isEqual = std::equal(output1.begin(), output1.end(), result1a.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_double_limiter)
    {
      DoubleLimiter limiter(13, 16);

      std::transform(input2.begin(), input2.end(), output2.begin(), limiter);

      bool isEqual = std::equal(output2.begin(), output2.end(), result2a.begin(), Compare());
      CHECK(isEqual);
    }
  };
}
