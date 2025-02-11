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

#include "etl/rms.h"

#include <array>

namespace
{
  std::array<int8_t, 18> input_c
  {
    // Sawtooth wave
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -8, -7, -6, -5, -4, -3, -2, -1
  };

  //*********************************
  std::array<double, 18> input_f
  {
    // Sawtooth wave
    0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, -8.0, -7.0, -6.0, -5.0, -4.0, -3.0, -2.0, -1.0
  };

  SUITE(test_rms)
  {
    //*************************************************************************
    TEST(test_char_rms)
    {
      etl::rms<int8_t, int> rms(input_c.begin(), input_c.end());
      double result = rms.get_rms();

      CHECK_CLOSE(5.21, result, 0.05);
    }

    //*************************************************************************
    TEST(test_double_rms)
    {
      etl::rms<double> rms(input_f.begin(), input_f.end());
      double result = rms.get_rms();

      CHECK_CLOSE(5.21, result, 0.05);
    }
  };
}
