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

#include "etl/gamma.h"

#include <array>
#include <algorithm>
#include <math.h>

namespace
{
  constexpr size_t Size = 10UL;

  using GammaEncode = etl::gamma_encode<double>;
  using GammaDecode = etl::gamma_decode<double>;

  struct Compare
  {
    bool operator ()(double lhs, double rhs) const
    {
      return fabs(lhs - rhs) < 0.1;
    }
  };

  //***********************************
  const std::array<double, Size> input1a =
  {
    0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
  };

  const std::array<double, Size> result2a =
  {
    0.0, 0.11, 0.44, 1.00, 1.78, 2.78, 4.00, 5.44, 7.11, 9.00
  };

  const std::array<double, Size> input1b =
  {
    0.0, 0.11, 0.44, 1.00, 1.78, 2.78, 4.00, 5.44, 7.11, 9.00
  };

  const std::array<double, Size> result2b =
  {
    0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
  };

  std::array<double, Size> output2;

  SUITE(test_gamma)
  {
    //*************************************************************************
    TEST(test_gamma_encode)
    {
      GammaEncode gamma(0.5, 9.0);

      std::transform(input1a.begin(), input1a.end(), output2.begin(), gamma);

      bool isEqual = std::equal(output2.begin(), output2.end(), result2a.begin(), Compare());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_gamma_decode)
    {
      GammaDecode gamma(0.5, 9.0);

      std::transform(input1b.begin(), input1b.end(), output2.begin(), gamma);

      bool isEqual = std::equal(output2.begin(), output2.end(), result2b.begin(), Compare());
      CHECK(isEqual);
    }
  };
}
