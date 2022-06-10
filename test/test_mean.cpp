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

#include "etl/mean.h"

#include <array>

namespace
{
  std::array<char, 10> input_c
  {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  };

  //*********************************
  std::array<float, 10> input_f
  {
    0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f
  };

  //*********************************
  std::array<double, 10> input_d
  {
    0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
  };

  SUITE(test_mean)
  {
    //*************************************************************************
    TEST(test_char_mean_default_constructor)
    {
      etl::mean<char, int32_t> mean;

      double mean_result = mean;

      CHECK_EQUAL(0.0, mean_result);
    }

    //*************************************************************************
    TEST(test_float_mean_default_constructor)
    {
      etl::mean<float> mean;

      double mean_result = mean;

      CHECK_EQUAL(0.0, mean_result);
    }

    //*************************************************************************
    TEST(test_double_mean_default_constructor)
    {
      etl::mean<double> mean;

      double mean_result = mean;

      CHECK_EQUAL(0.0, mean_result);
    }

    //*************************************************************************
    TEST(test_char_mean_constructor)
    {
      double mean_result;

      etl::mean<char, int32_t> mean1(input_c.begin(), input_c.end());
      mean_result = mean1.get_mean();
      CHECK_CLOSE(4.5, mean_result, 0.1);
    }

    //*************************************************************************
    TEST(test_float_mean_constructor)
    {
      double mean_result;

      etl::mean<float> mean1(input_f.begin(), input_f.end());
      mean_result = mean1.get_mean();
      CHECK_CLOSE(4.5, mean_result, 0.1);
    }

    //*************************************************************************
    TEST(test_double_mean_constructor)
    {
      double mean_result;

      etl::mean<double> mean1(input_d.begin(), input_d.end());
      mean_result = mean1.get_mean();
      CHECK_CLOSE(4.5, mean_result, 0.1);
    }
  };
}
