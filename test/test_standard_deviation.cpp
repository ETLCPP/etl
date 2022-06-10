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

#include "etl/standard_deviation.h"

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

  SUITE(test_standard_deviation)
  {
    //*************************************************************************
    TEST(test_char_standard_deviation_default_constructor)
    {
      etl::standard_deviation<etl::standard_deviation_type::Population, char, int32_t> standard_deviation;

      double standard_deviation_result = standard_deviation;

      CHECK_EQUAL(0.0, standard_deviation_result);
    }

    //*************************************************************************
    TEST(test_float_standard_deviation_default_constructor)
    {
      etl::standard_deviation<etl::standard_deviation_type::Population, float> standard_deviation;

      double standard_deviation_result = standard_deviation;

      CHECK_EQUAL(0.0, standard_deviation_result);
    }

    //*************************************************************************
    TEST(test_double_standard_deviation_default_constructor)
    {
      etl::standard_deviation<etl::standard_deviation_type::Population, double> standard_deviation;

      double standard_deviation_result = standard_deviation;

      CHECK_EQUAL(0.0, standard_deviation_result);
    }

    //*************************************************************************
    TEST(test_char_standard_deviation_constructor_population)
    {
      double standard_deviation_result;
      double variance_result;

      etl::standard_deviation<etl::standard_deviation_type::Population, char, int32_t> standard_deviation(input_c.begin(), input_c.end());
      standard_deviation_result = standard_deviation.get_standard_deviation();
      CHECK_CLOSE(2.87, standard_deviation_result, 0.1);
      variance_result = standard_deviation.get_variance();
      CHECK_CLOSE(8.25, variance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_char_standard_deviation_constructor_sample)
    {
      double standard_deviation_result;
      double variance_result;

      etl::standard_deviation<etl::standard_deviation_type::Sample, char, int32_t> standard_deviation(input_c.begin(), input_c.end());
      standard_deviation_result = standard_deviation.get_standard_deviation();
      CHECK_CLOSE(3.03, standard_deviation_result, 0.1);
      variance_result = standard_deviation.get_variance();
      CHECK_CLOSE(9.17, variance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_float_standard_deviation_constructor_population)
    {
      double standard_deviation_result;
      double variance_result;

      etl::standard_deviation<etl::standard_deviation_type::Population, float> standard_deviation(input_f.begin(), input_f.end());
      standard_deviation_result = standard_deviation.get_standard_deviation();
      CHECK_CLOSE(2.87, standard_deviation_result, 0.1);
      variance_result = standard_deviation.get_variance();
      CHECK_CLOSE(8.25, variance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_float_standard_deviation_constructor_sample)
    {
      double standard_deviation_result;
      double variance_result;

      etl::standard_deviation<etl::standard_deviation_type::Sample, float> standard_deviation(input_f.begin(), input_f.end());
      standard_deviation_result = standard_deviation.get_standard_deviation();
      CHECK_CLOSE(3.03, standard_deviation_result, 0.1);
      variance_result = standard_deviation.get_variance();
      CHECK_CLOSE(9.17, variance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_double_standard_deviation_constructor_population)
    {
      double standard_deviation_result;
      double variance_result;

      etl::standard_deviation<etl::standard_deviation_type::Population, double> standard_deviation(input_d.begin(), input_d.end());
      standard_deviation_result = standard_deviation.get_standard_deviation();
      CHECK_CLOSE(2.87, standard_deviation_result, 0.1);
      variance_result = standard_deviation.get_variance();
      CHECK_CLOSE(8.25, variance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_double_standard_deviation_constructor_sample)
    {
      double standard_deviation_result;
      double variance_result;

      etl::standard_deviation<etl::standard_deviation_type::Sample, double> standard_deviation(input_d.begin(), input_d.end());
      standard_deviation_result = standard_deviation.get_standard_deviation();
      CHECK_CLOSE(3.03, standard_deviation_result, 0.1);
      variance_result = standard_deviation.get_variance();
      CHECK_CLOSE(9.17, variance_result, 0.1);
    }
  };
}
