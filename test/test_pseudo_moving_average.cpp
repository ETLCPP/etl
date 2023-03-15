/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#include <array>
#include <algorithm>

#include "etl/pseudo_moving_average.h"
#include "etl/scaled_rounding.h"

namespace
{
  const size_t SAMPLE_SIZE = 10UL;
  const size_t SCALING = 100UL;

  SUITE(test_pseudo_moving_average)
  {
    //*************************************************************************
    TEST(integral_signed_average_positive)
    {
      using PMA = etl::pseudo_moving_average<int, SAMPLE_SIZE, SCALING>;
      PMA cma(0);

      CHECK_EQUAL(0, cma.value());

      cma.add(9);
      cma.add(1);
      cma.add(8);
      cma.add(2);
      cma.add(7);
      cma.add(3);
      cma.add(6);
      cma.add(4);
      cma.add(5);

      CHECK_EQUAL(280, cma.value());
    }

    //*************************************************************************
    TEST(integral_signed_average_positive_via_iterator)
    {
      std::array<int, 9> data{ 9, 1, 8, 2, 7, 3, 6, 4, 5 };

      using PMA = etl::pseudo_moving_average<int, SAMPLE_SIZE, SCALING>;
      PMA cma(0);

      CHECK_EQUAL(0, cma.value());

      std::copy(data.begin(), data.end(), cma.input());

      CHECK_EQUAL(280, cma.value());
    }

    //*************************************************************************
    TEST(integral_signed_average_negative)
    {
      using PMA = etl::pseudo_moving_average<int, SAMPLE_SIZE, SCALING>;
      PMA cma(0);

      CHECK_EQUAL(0, cma.value());

      cma.add(-9);
      cma.add(-1);
      cma.add(-8);
      cma.add(-2);
      cma.add(-7);
      cma.add(-3);
      cma.add(-6);
      cma.add(-4);
      cma.add(-5);

      CHECK_EQUAL(-280, cma.value());
    }

    //*************************************************************************
    TEST(integral_signed_average_negative_via_iterator)
    {
      std::array<int, 9> data{ -9, -1, -8, -2, -7, -3, -6, -4, -5 };

      using PMA = etl::pseudo_moving_average<int, SAMPLE_SIZE, SCALING>;
      PMA cma(0);

      CHECK_EQUAL(0, cma.value());

      std::copy(data.begin(), data.end(), cma.input());

      CHECK_EQUAL(-280, cma.value());
    }

    //*************************************************************************
    TEST(integral_unsigned_average_positive)
    {
      using PMA = etl::pseudo_moving_average<unsigned int, SAMPLE_SIZE, SCALING>;
      PMA cma(0U);

      CHECK_EQUAL(0U, cma.value());

      cma.add(9U);
      cma.add(1U);
      cma.add(8U);
      cma.add(2U);
      cma.add(7U);
      cma.add(3U);
      cma.add(6U);
      cma.add(4U);
      cma.add(5U);

      CHECK_EQUAL(280U, cma.value());
    }

    //*************************************************************************
    TEST(integral_unsigned_average_positive_via_iterator)
    {
      std::array<unsigned int, 9> data{ 9U, 1U, 8U, 2U, 7U, 3U, 6U, 4U, 5U };

      using PMA = etl::pseudo_moving_average<unsigned int, SAMPLE_SIZE, SCALING>;
      PMA cma(0U);

      CHECK_EQUAL(0U, cma.value());

      std::copy(data.begin(), data.end(), cma.input());

      CHECK_EQUAL(280U, cma.value());
    }

    //*************************************************************************
    TEST(integral_signed_average_positive_runtime_sample_size)
    {
      using PMA = etl::pseudo_moving_average<int, 0U, SCALING>;
      PMA cma(0, SAMPLE_SIZE * 2U);

      CHECK_EQUAL(0, cma.value());

      cma.set_sample_size(SAMPLE_SIZE);

      cma.add(9);
      cma.add(1);
      cma.add(8);
      cma.add(2);
      cma.add(7);
      cma.add(3);
      cma.add(6);
      cma.add(4);
      cma.add(5);

      CHECK_EQUAL(280, cma.value());
    }

    //*************************************************************************
    TEST(integral_signed_average_positive_runtime_sample_size_via_iterator)
    {
      std::array<int, 9> data{ 9, 1, 8, 2, 7, 3, 6, 4, 5 };

      using PMA = etl::pseudo_moving_average<int, 0U, SCALING>;
      PMA cma(0, SAMPLE_SIZE * 2U);

      CHECK_EQUAL(0, cma.value());

      cma.set_sample_size(SAMPLE_SIZE);

      std::copy(data.begin(), data.end(), cma.input());

      CHECK_EQUAL(280, cma.value());
    }

    //*************************************************************************
    TEST(integral_signed_average_negative_runtime_sample_size)
    {
      using PMA = etl::pseudo_moving_average<int, 0U, SCALING>;
      PMA cma(0, SAMPLE_SIZE * 2U);

      CHECK_EQUAL(0, cma.value());

      cma.set_sample_size(SAMPLE_SIZE);

      cma.add(-9);
      cma.add(-1);
      cma.add(-8);
      cma.add(-2);
      cma.add(-7);
      cma.add(-3);
      cma.add(-6);
      cma.add(-4);
      cma.add(-5);

      CHECK_EQUAL(-280, cma.value());
    }

    //*************************************************************************
    TEST(integral_signed_average_negative_runtime_sample_size_via_iterator)
    {
      std::array<int, 9> data{ -9, -1, -8, -2, -7, -3, -6, -4, -5 };

      using PMA = etl::pseudo_moving_average<int, 0U, SCALING>;
      PMA cma(0, SAMPLE_SIZE * 2U);

      CHECK_EQUAL(0, cma.value());

      cma.set_sample_size(SAMPLE_SIZE);

      std::copy(data.begin(), data.end(), cma.input());

      CHECK_EQUAL(-280, cma.value());
    }

    //*************************************************************************
    TEST(integral_unsigned_average_positive_runtime_sample_size)
    {
      using PMA = etl::pseudo_moving_average<unsigned int, 0U, SCALING>;
      PMA cma(0U, SAMPLE_SIZE * 2U);

      CHECK_EQUAL(0U, cma.value());

      cma.set_sample_size(SAMPLE_SIZE);

      cma.add(9U);
      cma.add(1U);
      cma.add(8U);
      cma.add(2U);
      cma.add(7U);
      cma.add(3U);
      cma.add(6U);
      cma.add(4U);
      cma.add(5U);

      CHECK_EQUAL(280U, cma.value());
    }

    //*************************************************************************
    TEST(integral_unsigned_average_positive_runtime_sample_size_via_iterator)
    {
      std::array<unsigned int, 9> data{ 9U, 1U, 8U, 2U, 7U, 3U, 6U, 4U, 5U };

      using PMA = etl::pseudo_moving_average<int, 0U, SCALING>;
      PMA cma(0U, SAMPLE_SIZE * 2U);

      CHECK_EQUAL(0U, cma.value());

      cma.set_sample_size(SAMPLE_SIZE);

      std::copy(data.begin(), data.end(), cma.input());

      CHECK_EQUAL(280U, cma.value());
    }

    //*************************************************************************
    TEST(floating_point_average)
    {
      using PMA = etl::pseudo_moving_average<double, SAMPLE_SIZE>;
      PMA cma(0);

      CHECK_EQUAL(0.0, cma.value());

      cma.add(9.0);
      cma.add(1.0);
      cma.add(8.0);
      cma.add(2.0);
      cma.add(7.0);
      cma.add(3.0);
      cma.add(6.0);
      cma.add(4.0);
      cma.add(5.0);

      CHECK_CLOSE(2.82, cma.value(), 0.01);
    }

    //*************************************************************************
    TEST(floating_point_average_via_iterator)
    {
      std::array<double, 9> data{ 9.0, 1.0, 8.0, 2.0, 7.0, 3.0, 6.0, 4.0, 5.0 };

      using PMA = etl::pseudo_moving_average<double, SAMPLE_SIZE>;
      PMA cma(0);

      CHECK_EQUAL(0.0, cma.value());

      std::copy(data.begin(), data.end(), cma.input());

      CHECK_CLOSE(2.82, cma.value(), 0.01);
    }

    //*************************************************************************
    TEST(floating_point_average_runtime_sample_size)
    {
      using PMA = etl::pseudo_moving_average<double, 0U>;
      PMA cma(0, SAMPLE_SIZE * 2);

      CHECK_EQUAL(0.0, cma.value());

      cma.set_sample_size(SAMPLE_SIZE);

      cma.add(9.0);
      cma.add(1.0);
      cma.add(8.0);
      cma.add(2.0);
      cma.add(7.0);
      cma.add(3.0);
      cma.add(6.0);
      cma.add(4.0);
      cma.add(5.0);

      CHECK_CLOSE(2.82, cma.value(), 0.01);
    }

    //*************************************************************************
    TEST(floating_point_average_runtime_sample_size_via_iterator)
    {
      std::array<double, 9> data{ 9.0, 1.0, 8.0, 2.0, 7.0, 3.0, 6.0, 4.0, 5.0 };

      using PMA = etl::pseudo_moving_average<double, 0U>;
      PMA cma(0, SAMPLE_SIZE * 2);

      CHECK_EQUAL(0.0, cma.value());

      cma.set_sample_size(SAMPLE_SIZE);

      std::copy(data.begin(), data.end(), cma.input());

      CHECK_CLOSE(2.82, cma.value(), 0.01);
    }
  };
}
