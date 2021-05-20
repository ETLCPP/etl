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

#include "etl/coordinate.h"

#include <stdint.h>

namespace
{
  using Coord_2d   = etl::coordinate_2d<int>;
  using Coord_v_2d = etl::coordinate_2d<int, int>;

  using Coord_3d   = etl::coordinate_3d<int>;
  using Coord_v_3d = etl::coordinate_3d<int, int>;

  SUITE(test_coordinate)
  {      
    //*************************************************************************
    TEST(test_coordinate_2d_default_constructor)
    {
      Coord_2d   coord;
      Coord_v_2d coord_v;

      CHECK_EQUAL(0, coord.x);
      CHECK_EQUAL(0, coord.y);

      CHECK_EQUAL(0, coord_v.x);
      CHECK_EQUAL(0, coord_v.y);
      CHECK_EQUAL(0, coord_v.value);
    }

    //*************************************************************************
    TEST(test_coordinate_2d_constructor)
    {
      Coord_2d   coord(1, 2);
      Coord_v_2d coord_v(1, 2, 3);

      CHECK_EQUAL(1, coord.x);
      CHECK_EQUAL(2, coord.y);

      CHECK_EQUAL(1, coord_v.x);
      CHECK_EQUAL(2, coord_v.y);
      CHECK_EQUAL(3, coord_v.value);
    }

    //*************************************************************************
    TEST(test_coordinate_2d_copy_constructor)
    {
      Coord_2d   input(1, 2);
      Coord_2d   coord(input);
      Coord_v_2d input_v(1, 2, 3);
      Coord_v_2d coord_v(input_v);

      CHECK_EQUAL(1, coord.x);
      CHECK_EQUAL(2, coord.y);

      CHECK_EQUAL(1, coord_v.x);
      CHECK_EQUAL(2, coord_v.y);
      CHECK_EQUAL(3, coord_v.value);
    }

    //*************************************************************************
    TEST(test_coordinate_2d_assignment)
    {
      Coord_2d   input(1, 2);
      Coord_2d   coord;
      Coord_v_2d input_v(1, 2, 3);
      Coord_v_2d coord_v;

      coord   = input;
      coord_v = input_v;

      CHECK_EQUAL(1, coord.x);
      CHECK_EQUAL(2, coord.y);

      CHECK_EQUAL(1, coord_v.x);
      CHECK_EQUAL(2, coord_v.y);
      CHECK_EQUAL(3, coord_v.value);
    }

    //*************************************************************************
    TEST(test_coordinate_2d_addition)
    {
      Coord_2d   input1(1, 2);
      Coord_2d   input2(3, 4);
      Coord_2d   coord;
      Coord_2d   result(4, 6);

      Coord_v_2d input1_v(1, 2, 3);
      Coord_v_2d input2_v(4, 5, 6);
      Coord_v_2d coord_v;
      Coord_v_2d result_v(5, 7);

      coord   = input1   + input2;
      coord_v = input1_v + input2_v;

      CHECK(coord   == result);
      CHECK(coord_v == result_v);

      coord   = input1;
      coord_v = input1_v;
      
      coord   += input2;
      coord_v += input2_v;

      CHECK(coord   == result);
      CHECK(coord_v == result_v);
    }

    //*************************************************************************
    TEST(test_coordinate_2d_subtraction)
    {
      Coord_2d   input1(1, 2);
      Coord_2d   input2(3, 4);
      Coord_2d   coord;
      Coord_2d   result(-2, -2);

      Coord_v_2d input1_v(1, 2, 3);
      Coord_v_2d input2_v(4, 5, 6);
      Coord_v_2d coord_v;
      Coord_v_2d result_v(-3, -3);

      coord   = input1   - input2;
      coord_v = input1_v - input2_v;

      CHECK(coord   == result);
      CHECK(coord_v == result_v);

      coord   = input1;
      coord_v = input1_v;
      
      coord   -= input2;
      coord_v -= input2_v;

      CHECK(coord   == result);
      CHECK(coord_v == result_v);
    }

    //*************************************************************************
    TEST(test_coordinate_2d_)
    {
      Coord_2d   input1(1, 2);
      Coord_2d   input2(3, 4);
      Coord_2d   coord(input1);
      Coord_v_2d input1_v(1, 2, 3);
      Coord_v_2d input2_v(4, 5, 6);
      Coord_v_2d coord_v(input1_v);

      CHECK(coord   == input1);
      CHECK(coord_v == input1_v);

      CHECK(coord   != input2);
      CHECK(coord_v != input2_v);
    }

    //*************************************************************************
    TEST(test_coordinate_3d_default_constructor)
    {
      Coord_3d   coord;
      Coord_v_3d coord_v;

      CHECK_EQUAL(0, coord.x);
      CHECK_EQUAL(0, coord.y);
      CHECK_EQUAL(0, coord.z);

      CHECK_EQUAL(0, coord_v.x);
      CHECK_EQUAL(0, coord_v.y);
      CHECK_EQUAL(0, coord_v.z);
      CHECK_EQUAL(0, coord_v.value);
    }

    //*************************************************************************
    TEST(test_coordinate_3d_constructor)
    {
      Coord_3d   coord(1, 2, 3);
      Coord_v_3d coord_v(1, 2, 3, 4);

      CHECK_EQUAL(1, coord.x);
      CHECK_EQUAL(2, coord.y);
      CHECK_EQUAL(3, coord.z);

      CHECK_EQUAL(1, coord_v.x);
      CHECK_EQUAL(2, coord_v.y);
      CHECK_EQUAL(3, coord_v.z);
      CHECK_EQUAL(4, coord_v.value);
    }

    //*************************************************************************
    TEST(test_coordinate_3d_copy_constructor)
    {
      Coord_3d   input(1, 2, 3);
      Coord_3d   coord(input);
      Coord_v_3d input_v(1, 2, 3, 4);
      Coord_v_3d coord_v(input_v);

      CHECK_EQUAL(1, coord.x);
      CHECK_EQUAL(2, coord.y);
      CHECK_EQUAL(3, coord.z);

      CHECK_EQUAL(1, coord_v.x);
      CHECK_EQUAL(2, coord_v.y);
      CHECK_EQUAL(3, coord_v.z);
      CHECK_EQUAL(4, coord_v.value);
    }

    //*************************************************************************
    TEST(test_coordinate_3d_assignment)
    {
      Coord_3d   input(1, 2, 3);
      Coord_3d   coord;
      Coord_v_3d input_v(1, 2, 3, 4);
      Coord_v_3d coord_v;

      coord   = input;
      coord_v = input_v;

      CHECK_EQUAL(1, coord.x);
      CHECK_EQUAL(2, coord.y);
      CHECK_EQUAL(3, coord.z);

      CHECK_EQUAL(1, coord_v.x);
      CHECK_EQUAL(2, coord_v.y);
      CHECK_EQUAL(3, coord_v.z);
      CHECK_EQUAL(4, coord_v.value);
    }

    //*************************************************************************
    TEST(test_coordinate_3d_equality)
    {
      Coord_3d   input1(1, 2, 3);
      Coord_3d   input2(4, 5, 6);
      Coord_3d   coord(input1);
      Coord_v_3d input1_v(1, 2, 3, 4);
      Coord_v_3d input2_v(5, 6, 7, 8);
      Coord_v_3d coord_v(input1_v);

      CHECK(coord   == input1);
      CHECK(coord_v == input1_v);

      CHECK(coord   != input2);
      CHECK(coord_v != input2_v);
    }

    //*************************************************************************
    TEST(test_coordinate_3d_addition)
    {
      Coord_3d   input1(1, 2, 3);
      Coord_3d   input2(4, 5, 6);
      Coord_3d   coord;
      Coord_3d   result(5, 7, 9);

      Coord_v_3d input1_v(1, 2, 3, 4);
      Coord_v_3d input2_v(5, 6, 7, 8);
      Coord_v_3d coord_v;
      Coord_v_3d result_v(6, 8, 10, 12);

      coord   = input1   + input2;
      coord_v = input1_v + input2_v;

      CHECK(coord   == result);
      CHECK(coord_v == result_v);

      coord   = input1;
      coord_v = input1_v;
      
      coord   += input2;
      coord_v += input2_v;

      CHECK(coord   == result);
      CHECK(coord_v == result_v);
    }

    //*************************************************************************
    TEST(test_coordinate_3d_subtraction)
    {
      Coord_3d   input1(1, 2, 3);
      Coord_3d   input2(4, 5, 6);
      Coord_3d   coord;
      Coord_3d   result(-3, -3, -3);

      Coord_v_3d input1_v(1, 2, 3, 4);
      Coord_v_3d input2_v(5, 6, 7, 8);
      Coord_v_3d coord_v;
      Coord_v_3d result_v(-4, -4, -4, -4);

      coord   = input1   - input2;
      coord_v = input1_v - input2_v;

      CHECK(coord   == result);
      CHECK(coord_v == result_v);

      coord   = input1;
      coord_v = input1_v;
      
      coord   -= input2;
      coord_v -= input2_v;

      CHECK(coord   == result);
      CHECK(coord_v == result_v);
    }
  };
}
