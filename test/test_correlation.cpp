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

#include "etl/correlation.h"

#include <array>

namespace
{
  std::array<int8_t, 10> input_c
  {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  };

  std::array<int8_t, 10> input_c_flat
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  std::array<int8_t, 10> input_c_inv
  {
    0, -1, -2, -3, -4, -5, -6, -7, -8, -9
  };

  //*********************************
  std::array<float, 10> input_f
  {
    0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f
  };

  std::array<float, 10> input_f_flat
  {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
  };

  std::array<float, 10> input_f_inv
  {
    0.0f, -1.0f, -2.0f, -3.0f, -4.0f, -5.0f, -6.0f, -7.0f, -8.0f, -9.0f
  };

  //*********************************
  std::array<double, 10> input_d
  {
    0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0
  };

  std::array<double, 10> input_d_flat
  {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
  };

  std::array<double, 10> input_d_inv
  {
    0.0, -1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0
  };

  SUITE(test_correlation)
  {
    //*************************************************************************
    TEST(test_char_correlation_default_constructor)
    {
      etl::correlation<etl::correlation_type::Population, signed char, int32_t> correlation;

      double correlation_result = correlation;

      CHECK_EQUAL(0.0, correlation_result);
    }

    //*************************************************************************
    TEST(test_float_correlation_default_constructor)
    {
      etl::correlation<etl::correlation_type::Population, float> correlation;

      double correlation_result = correlation;

      CHECK_EQUAL(0.0, correlation_result);
    }

    //*************************************************************************
    TEST(test_double_correlation_default_constructor)
    {
      etl::correlation<etl::correlation_type::Population, double> correlation;

      double correlation_result = correlation;

      CHECK_EQUAL(0.0, correlation_result);
    }

    //*************************************************************************
    TEST(test_char_correlation_constructor_population)
    {
      double correlation_result;
      double covariance_result;

      // Negative correlation.
      etl::correlation<etl::correlation_type::Population, int8_t, int32_t> correlation1(input_c.begin(), input_c.end(), input_c_inv.begin());
      correlation_result = correlation1;
      CHECK_CLOSE(-1.0, correlation_result, 0.1);
      covariance_result = correlation1.get_covariance();
      CHECK_CLOSE(-8.25, covariance_result, 0.1);

      // Zero correlation
      etl::correlation<etl::correlation_type::Population, int8_t, int32_t> correlation2(input_c.begin(), input_c.end(), input_c_flat.begin());
      correlation_result = correlation2;
      CHECK_CLOSE(0.0, correlation_result, 0.1);
      covariance_result = correlation2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive correlation.
      etl::correlation<etl::correlation_type::Population, int8_t, int32_t> correlation3(input_c.begin(), input_c.end(), input_c.begin());
      correlation_result = correlation3;
      CHECK_CLOSE(1.0, correlation_result, 0.1);
      covariance_result = correlation3.get_covariance();
      CHECK_CLOSE(8.25, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_char_correlation_constructor_sample)
    {
      double correlation_result;
      double covariance_result;

      // Negative correlation.
      etl::correlation<etl::correlation_type::Sample, int8_t, int32_t> correlation1(input_c.begin(), input_c.end(), input_c_inv.begin());
      correlation_result = correlation1;
      CHECK_CLOSE(-1.0, correlation_result, 0.1);
      covariance_result = correlation1.get_covariance();
      CHECK_CLOSE(-9.17, covariance_result, 0.1);

      // Zero correlation
      etl::correlation<etl::correlation_type::Sample, int8_t, int32_t> correlation2(input_c.begin(), input_c.end(), input_c_flat.begin());
      correlation_result = correlation2;
      CHECK_CLOSE(0.0, correlation_result, 0.1);
      covariance_result = correlation2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive correlation.
      etl::correlation<etl::correlation_type::Sample, int8_t, int32_t> correlation3(input_c.begin(), input_c.end(), input_c.begin());
      correlation_result = correlation3;
      CHECK_CLOSE(1.0, correlation_result, 0.1);
      covariance_result = correlation3.get_covariance();
      CHECK_CLOSE(9.17, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_float_correlation_constructor_population)
    {
      double correlation_result;
      double covariance_result;

      // Negative correlation.
      etl::correlation<etl::correlation_type::Population, float> correlation1(input_f.begin(), input_f.end(), input_f_inv.begin());
      correlation_result = correlation1;
      CHECK_CLOSE(-1.0, correlation_result, 0.1);
      covariance_result = correlation1.get_covariance();
      CHECK_CLOSE(-8.25, covariance_result, 0.1);

      // Zero correlation
      etl::correlation<etl::correlation_type::Population, float> correlation2(input_f.begin(), input_f.end(), input_f_flat.begin());
      correlation_result = correlation2;
      CHECK_CLOSE(0.0, correlation_result, 0.1);
      covariance_result = correlation2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive correlation.
      etl::correlation<etl::correlation_type::Population, float> correlation3(input_f.begin(), input_f.end(), input_f.begin());
      correlation_result = correlation3;
      CHECK_CLOSE(1.0, correlation_result, 0.1);
      covariance_result = correlation3.get_covariance();
      CHECK_CLOSE(8.25, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_float_correlation_constructor_sample)
    {
      double correlation_result;
      double covariance_result;

      // Negative correlation.
      etl::correlation<etl::correlation_type::Sample, float> correlation1(input_f.begin(), input_f.end(), input_f_inv.begin());
      correlation_result = correlation1;
      CHECK_CLOSE(-1.0, correlation_result, 0.1);
      covariance_result = correlation1.get_covariance();
      CHECK_CLOSE(-9.17, covariance_result, 0.1);

      // Zero correlation
      etl::correlation<etl::correlation_type::Sample, float> correlation2(input_f.begin(), input_f.end(), input_f_flat.begin());
      correlation_result = correlation2;
      CHECK_CLOSE(0.0, correlation_result, 0.1);
      covariance_result = correlation2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive correlation.
      etl::correlation<etl::correlation_type::Sample, float> correlation3(input_f.begin(), input_f.end(), input_f.begin());
      correlation_result = correlation3;
      CHECK_CLOSE(1.0, correlation_result, 0.1);
      covariance_result = correlation3.get_covariance();
      CHECK_CLOSE(9.17, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_double_correlation_constructor_population)
    {
      double correlation_result;
      double covariance_result;

      // Negative correlation.
      etl::correlation<etl::correlation_type::Population, double> correlation1(input_d.begin(), input_d.end(), input_d_inv.begin());
      correlation_result = correlation1;
      CHECK_CLOSE(-1.0, correlation_result, 0.1);
      covariance_result = correlation1.get_covariance();
      CHECK_CLOSE(-8.25, covariance_result, 0.1);

      // Zero correlation
      etl::correlation<etl::correlation_type::Population, double> correlation2(input_d.begin(), input_d.end(), input_d_flat.begin());
      correlation_result = correlation2;
      CHECK_CLOSE(0.0, correlation_result, 0.1);
      covariance_result = correlation2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive correlation.
      etl::correlation<etl::correlation_type::Population, double> correlation3(input_d.begin(), input_d.end(), input_d.begin());
      correlation_result = correlation3;
      CHECK_CLOSE(1.0, correlation_result, 0.1);
      covariance_result = correlation3.get_covariance();
      CHECK_CLOSE(8.25, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_double_correlation_constructor_sample)
    {
      double correlation_result;
      double covariance_result;

      // Negative correlation.
      etl::correlation<etl::correlation_type::Sample, double> correlation1(input_d.begin(), input_d.end(), input_d_inv.begin());
      correlation_result = correlation1;
      CHECK_CLOSE(-1.0, correlation_result, 0.1);
      covariance_result = correlation1.get_covariance();
      CHECK_CLOSE(-9.17, covariance_result, 0.1);

      // Zero correlation
      etl::correlation<etl::correlation_type::Sample, double> correlation2(input_d.begin(), input_d.end(), input_d_flat.begin());
      correlation_result = correlation2;
      CHECK_CLOSE(0.0, correlation_result, 0.1);
      covariance_result = correlation2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive correlation.
      etl::correlation<etl::correlation_type::Sample, double> correlation3(input_d.begin(), input_d.end(), input_d.begin());
      correlation_result = correlation3;
      CHECK_CLOSE(1.0, correlation_result, 0.1);
      covariance_result = correlation3.get_covariance();
      CHECK_CLOSE(9.17, covariance_result, 0.1);
    }
  };
}
