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

#include "etl/covariance.h"

#include <array>

namespace
{
  std::array<char, 10> input_c
  {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  };

  std::array<char, 10> input_c_flat
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

  std::array<char, 10> input_c_inv
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

  SUITE(test_covariance)
  {
    //*************************************************************************
    TEST(test_char_covariance_default_constuctor)
    {
      etl::covariance<etl::covariance_type::Population, char, int32_t> covariance;

      double covariance_result = covariance;

      CHECK_EQUAL(0.0, covariance_result);
    }

    //*************************************************************************
    TEST(test_float_covariance_default_constuctor)
    {
      etl::covariance<etl::covariance_type::Population, float> covariance;

      double covariance_result = covariance;

      CHECK_EQUAL(0.0, covariance_result);
    }

    //*************************************************************************
    TEST(test_double_covariance_default_constuctor)
    {
      etl::covariance<etl::covariance_type::Population, double> covariance;

      double covariance_result = covariance;

      CHECK_EQUAL(0.0, covariance_result);
    }

    //*************************************************************************
    TEST(test_char_covariance_constuctor_population)
    {
      double covariance_result;

      // Negative covariance.
      etl::covariance<etl::covariance_type::Population, char, int32_t> covariance1(input_c.begin(), input_c.end(), input_c_inv.begin());
      covariance_result = covariance1.get_covariance();
      CHECK_CLOSE(-8.25, covariance_result, 0.1);

      // Zero covariance
      etl::covariance<etl::covariance_type::Population, char, int32_t> covariance2(input_c.begin(), input_c.end(), input_c_flat.begin());
      covariance_result = covariance2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive covariance.
      etl::covariance<etl::covariance_type::Population, char, int32_t> covariance3(input_c.begin(), input_c.end(), input_c.begin());
      covariance_result = covariance3.get_covariance();
      CHECK_CLOSE(8.25, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_char_covariance_constuctor_sample)
    {
      double covariance_result;

      // Negative covariance.
      etl::covariance<etl::covariance_type::Sample, char, int32_t> covariance1(input_c.begin(), input_c.end(), input_c_inv.begin());
      covariance_result = covariance1.get_covariance();
      CHECK_CLOSE(-9.17, covariance_result, 0.1);

      // Zero covariance
      etl::covariance<etl::covariance_type::Sample, char, int32_t> covariance2(input_c.begin(), input_c.end(), input_c_flat.begin());
      covariance_result = covariance2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive covariance.
      etl::covariance<etl::covariance_type::Sample, char, int32_t> covariance3(input_c.begin(), input_c.end(), input_c.begin());
      covariance_result = covariance3.get_covariance();
      CHECK_CLOSE(9.17, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_float_covariance_constuctor_population)
    {
      double covariance_result;

      // Negative covariance.
      etl::covariance<etl::covariance_type::Population, float> covariance1(input_f.begin(), input_f.end(), input_f_inv.begin());
      covariance_result = covariance1.get_covariance();
      CHECK_CLOSE(-8.25, covariance_result, 0.1);

      // Zero covariance
      etl::covariance<etl::covariance_type::Population, float> covariance2(input_f.begin(), input_f.end(), input_f_flat.begin());
      covariance_result = covariance2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive covariance.
      etl::covariance<etl::covariance_type::Population, float> covariance3(input_f.begin(), input_f.end(), input_f.begin());
      covariance_result = covariance3.get_covariance();
      CHECK_CLOSE(8.25, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_float_covariance_constuctor_sample)
    {
      double covariance_result;

      // Negative covariance.
      etl::covariance<etl::covariance_type::Sample, float> covariance1(input_f.begin(), input_f.end(), input_f_inv.begin());
      covariance_result = covariance1.get_covariance();
      CHECK_CLOSE(-9.17, covariance_result, 0.1);

      // Zero covariance
      etl::covariance<etl::covariance_type::Sample, float> covariance2(input_f.begin(), input_f.end(), input_f_flat.begin());
      covariance_result = covariance2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive covariance.
      etl::covariance<etl::covariance_type::Sample, float> covariance3(input_f.begin(), input_f.end(), input_f.begin());
      covariance_result = covariance3.get_covariance();
      CHECK_CLOSE(9.17, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_double_covariance_constuctor_population)
    {
      double covariance_result;

      // Negative covariance.
      etl::covariance<etl::covariance_type::Population, double> covariance1(input_d.begin(), input_d.end(), input_d_inv.begin());
      covariance_result = covariance1.get_covariance();
      CHECK_CLOSE(-8.25, covariance_result, 0.1);

      // Zero covariance
      etl::covariance<etl::covariance_type::Population, double> covariance2(input_d.begin(), input_d.end(), input_d_flat.begin());
      covariance_result = covariance2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive covariance.
      etl::covariance<etl::covariance_type::Population, double> covariance3(input_d.begin(), input_d.end(), input_d.begin());
      covariance_result = covariance3.get_covariance();
      CHECK_CLOSE(8.25, covariance_result, 0.1);
    }

    //*************************************************************************
    TEST(test_double_covariance_constuctor_sample)
    {
      double covariance_result;

      // Negative covariance.
      etl::covariance<etl::covariance_type::Sample, double> covariance1(input_d.begin(), input_d.end(), input_d_inv.begin());
      covariance_result = covariance1.get_covariance();
      CHECK_CLOSE(-9.17, covariance_result, 0.1);

      // Zero covariance
      etl::covariance<etl::covariance_type::Sample, double> covariance2(input_d.begin(), input_d.end(), input_d_flat.begin());
      covariance_result = covariance2.get_covariance();
      CHECK_CLOSE(0.0, covariance_result, 0.1);

      // Positive covariance.
      etl::covariance<etl::covariance_type::Sample, double> covariance3(input_d.begin(), input_d.end(), input_d.begin());
      covariance_result = covariance3.get_covariance();
      CHECK_CLOSE(9.17, covariance_result, 0.1);
    }
  };
}
