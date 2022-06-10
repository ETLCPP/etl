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

#include "etl/rescale.h"

#include <array>
#include <algorithm>
#include <math.h>

namespace
{
  constexpr size_t Size = 10UL;

  using CharIntRescale    = etl::rescale<char, int>;
  using CharDoubleRescale = etl::rescale<char, double>;

  struct Compare
  {
    bool operator ()(double lhs, double rhs) const
    {
      return fabs(lhs - rhs) < 1.0;
    }
  };

  //***********************************
  const std::array<char, Size> input1 =
  {
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19
  };

  //***********************************
  const std::array<int, Size> result1 =
  {
    40000, 40211, 40422, 40633, 40844, 41055, 41266, 41477, 41688, 41900
  };

  std::array<int, Size> output1;

  const std::array<double, Size> result2 =
  {
    40000.0, 40211.0, 40422.0, 40633.0, 40844.0, 41055.0, 41266.0, 41477.0, 41688.0, 41900.0
  };

  std::array<double, Size> output2;

  SUITE(test_rescale)
  {
    //*************************************************************************
    TEST(test_char_int_rescale)
    {
      CharIntRescale rescale(10, 19, 40000, 41900);

      std::transform(input1.begin(), input1.end(), output1.begin(), rescale);

      bool isEqual = std::equal(output1.begin(), output1.end(), result1.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_char_double_rescale)
    {
      CharDoubleRescale rescale(10, 19, 40000.0, 41900.0);

      std::transform(input1.begin(), input1.end(), output2.begin(), rescale);

      bool isEqual = std::equal(output2.begin(), output2.end(), result2.begin(), Compare());
      CHECK(isEqual);
    }
  };
}
