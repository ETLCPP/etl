/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#include "UnitTest++.h"

#include "etl/cumulative_moving_average.h"
#include "etl/scaled_rounding.h"

namespace
{
  const size_t SAMPLE_SIZE = 10U;
  const size_t SCALING = 100U;

  SUITE(test_cumulative_moving_average)
  {
    //*************************************************************************
    TEST(integral_signed_average_positive)
    {
      typedef etl::cumulative_moving_average<int, SAMPLE_SIZE, SCALING> CMA;
      CMA cma(0);

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
    TEST(integral_signed_average_negative)
    {
      typedef etl::cumulative_moving_average<int, SAMPLE_SIZE, SCALING> CMA;
      CMA cma(0);

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
    TEST(integral_unsigned_average_positive)
    {
      typedef etl::cumulative_moving_average<unsigned int, SAMPLE_SIZE, SCALING> CMA;
      CMA cma(0U);

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
    TEST(floating_point_average)
    {
      typedef etl::cumulative_moving_average<double, SAMPLE_SIZE> CMA;
      CMA cma(0);

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
  };
}
