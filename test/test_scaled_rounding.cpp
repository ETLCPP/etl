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

#include "etl/scaled_rounding.h"

#include <array>

namespace
{
  std::array<int, 12> source = { 54, 55, 56, 64, 65, 66, -54, -55, -56, -64, -65, -66 };

  SUITE(test_scaled_rounding)
  {
    //*************************************************************************
    TEST(round_ceiling_scaled)
    {
      std::array<int, 12> expected = { 60, 60, 60, 70, 70, 70, -50, -50, -50, -60, -60, -60 };

      CHECK_EQUAL(expected[0],  etl::round_ceiling_scaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_ceiling_scaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_ceiling_scaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_ceiling_scaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_ceiling_scaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_ceiling_scaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_ceiling_scaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_ceiling_scaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_ceiling_scaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_ceiling_scaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_ceiling_scaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_ceiling_scaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_ceiling_unscaled)
    {
      std::array<int, 12> expected = { 6, 6, 6, 7, 7, 7, -5, -5, -5, -6, -6, -6 };

      CHECK_EQUAL(expected[0],  etl::round_ceiling_unscaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_ceiling_unscaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_ceiling_unscaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_ceiling_unscaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_ceiling_unscaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_ceiling_unscaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_ceiling_unscaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_ceiling_unscaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_ceiling_unscaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_ceiling_unscaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_ceiling_unscaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_ceiling_unscaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_floor_scaled)
    {
      std::array<int, 12> expected = { 50, 50, 50, 60, 60, 60, -60, -60, -60, -70, -70, -70 };

      CHECK_EQUAL(expected[0],  etl::round_floor_scaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_floor_scaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_floor_scaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_floor_scaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_floor_scaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_floor_scaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_floor_scaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_floor_scaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_floor_scaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_floor_scaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_floor_scaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_floor_scaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_floor_unscaled)
    {
      std::array<int, 12> expected = { 5, 5, 5, 6, 6, 6, -6, -6, -6, -7, -7, -7 };

      CHECK_EQUAL(expected[0],  etl::round_floor_unscaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_floor_unscaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_floor_unscaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_floor_unscaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_floor_unscaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_floor_unscaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_floor_unscaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_floor_unscaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_floor_unscaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_floor_unscaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_floor_unscaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_floor_unscaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_half_up_scaled)
    {
      std::array<int, 12> expected = { 50, 60, 60, 60, 70, 70, -50, -60, -60, -60, -70, -70 };

      CHECK_EQUAL(expected[0],  etl::round_half_up_scaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_up_scaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_up_scaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_up_scaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_up_scaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_up_scaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_up_scaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_up_scaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_up_scaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_up_scaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_up_scaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_up_scaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_half_up_unscaled)
    {
      std::array<int, 12> expected = { 5, 6, 6, 6, 7, 7, -5, -6, -6, -6, -7, -7 };

      CHECK_EQUAL(expected[0],  etl::round_half_up_unscaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_up_unscaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_up_unscaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_up_unscaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_up_unscaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_up_unscaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_up_unscaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_up_unscaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_up_unscaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_up_unscaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_up_unscaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_up_unscaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_half_down_scaled)
    {
      std::array<int, 12> expected = { 50, 50, 60, 60, 60, 70, -50, -50, -60, -60, -60, -70 };

      CHECK_EQUAL(expected[0],  etl::round_half_down_scaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_down_scaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_down_scaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_down_scaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_down_scaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_down_scaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_down_scaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_down_scaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_down_scaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_down_scaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_down_scaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_down_scaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_half_down_unscaled)
    {
      std::array<int, 12> expected = { 5, 5, 6, 6, 6, 7, -5, -5, -6, -6, -6, -7 };

      CHECK_EQUAL(expected[0],  etl::round_half_down_unscaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_down_unscaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_down_unscaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_down_unscaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_down_unscaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_down_unscaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_down_unscaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_down_unscaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_down_unscaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_down_unscaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_down_unscaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_down_unscaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_zero_scaled)
    {
      std::array<int, 12> expected = { 50, 50, 50, 60, 60, 60, -50, -50, -50, -60, -60, -60 };

      CHECK_EQUAL(expected[0],  etl::round_zero_scaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_zero_scaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_zero_scaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_zero_scaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_zero_scaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_zero_scaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_zero_scaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_zero_scaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_zero_scaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_zero_scaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_zero_scaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_zero_scaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_zero_unscaled)
    {
      std::array<int, 12> expected = { 5, 5, 5, 6, 6, 6, -5, -5, -5, -6, -6, -6 };

      CHECK_EQUAL(expected[0],  etl::round_zero_unscaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_zero_unscaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_zero_unscaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_zero_unscaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_zero_unscaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_zero_unscaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_zero_unscaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_zero_unscaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_zero_unscaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_zero_unscaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_zero_unscaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_zero_unscaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_infinity_scaled)
    {
      std::array<int, 12> expected = { 60, 60, 60, 70, 70, 70, -60, -60, -60, -70, -70, -70 };

      CHECK_EQUAL(expected[0],  etl::round_infinity_scaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_infinity_scaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_infinity_scaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_infinity_scaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_infinity_scaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_infinity_scaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_infinity_scaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_infinity_scaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_infinity_scaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_infinity_scaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_infinity_scaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_infinity_scaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_infinity_unscaled)
    {
      std::array<int, 12> expected = { 6, 6, 6, 7, 7, 7, -6, -6, -6, -7, -7, -7 };

      CHECK_EQUAL(expected[0],  etl::round_infinity_unscaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_infinity_unscaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_infinity_unscaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_infinity_unscaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_infinity_unscaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_infinity_unscaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_infinity_unscaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_infinity_unscaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_infinity_unscaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_infinity_unscaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_infinity_unscaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_infinity_unscaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_half_even_scaled)
    {
      std::array<int, 12> expected = { 50, 60, 60, 60, 60, 70, -50, -60, -60, -60, -60, -70 };

      CHECK_EQUAL(expected[0],  etl::round_half_even_scaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_even_scaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_even_scaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_even_scaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_even_scaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_even_scaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_even_scaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_even_scaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_even_scaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_even_scaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_even_scaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_even_scaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_half_even_unscaled)
    {
      std::array<int, 12> expected = { 5, 6, 6, 6, 6, 7, -5, -6, -6, -6, -6, -7 };

      CHECK_EQUAL(expected[0],  etl::round_half_even_unscaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_even_unscaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_even_unscaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_even_unscaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_even_unscaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_even_unscaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_even_unscaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_even_unscaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_even_unscaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_even_unscaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_even_unscaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_even_unscaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_half_odd_scaled)
    {
      std::array<int, 12> expected = { 50, 50, 60, 60, 70, 70, -50, -50, -60, -60, -70, -70 };

      CHECK_EQUAL(expected[0],  etl::round_half_odd_scaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_odd_scaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_odd_scaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_odd_scaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_odd_scaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_odd_scaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_odd_scaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_odd_scaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_odd_scaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_odd_scaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_odd_scaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_odd_scaled<10>(source[11]));
    }

    //*************************************************************************
    TEST(round_half_odd_unscaled)
    {
      std::array<int, 12> expected = { 5, 5, 6, 6, 7, 7, -5, -5, -6, -6, -7, -7 };

      CHECK_EQUAL(expected[0],  etl::round_half_odd_unscaled<10>(source[0]));
      CHECK_EQUAL(expected[1],  etl::round_half_odd_unscaled<10>(source[1]));
      CHECK_EQUAL(expected[2],  etl::round_half_odd_unscaled<10>(source[2]));
      CHECK_EQUAL(expected[3],  etl::round_half_odd_unscaled<10>(source[3]));
      CHECK_EQUAL(expected[4],  etl::round_half_odd_unscaled<10>(source[4]));
      CHECK_EQUAL(expected[5],  etl::round_half_odd_unscaled<10>(source[5]));
      CHECK_EQUAL(expected[6],  etl::round_half_odd_unscaled<10>(source[6]));
      CHECK_EQUAL(expected[7],  etl::round_half_odd_unscaled<10>(source[7]));
      CHECK_EQUAL(expected[8],  etl::round_half_odd_unscaled<10>(source[8]));
      CHECK_EQUAL(expected[9],  etl::round_half_odd_unscaled<10>(source[9]));
      CHECK_EQUAL(expected[10], etl::round_half_odd_unscaled<10>(source[10]));
      CHECK_EQUAL(expected[11], etl::round_half_odd_unscaled<10>(source[11]));
    }
  };
}
