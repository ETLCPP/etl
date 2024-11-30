/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 John Wellbelove

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

#include "etl/bresenham_line.h"

#include <vector>

namespace etl
{
  template <typename T>
  std::ostream& operator << (std::ostream& os, const etl::coordinate_2d<T>& point)
  {
    os << "(" << int(point.x) << "," << int(point.y) << ")";
    return os;
  }
}

namespace
{
  using Value = int8_t;

  using Point = etl::coordinate_2d<Value>;

#include "etl/private/diagnostic_unused_function_push.h"

  std::ostream& operator << (std::ostream& os, const Point& point)
  {
    os << "(" << int(point.x) << "," << int(point.y) << ")";
    return os;
  }

#include "etl/private/diagnostic_pop.h"

  using BresenhamLine = etl::bresenham_line<Value>;

  SUITE(test_bresenham_line)
  {
    //*************************************************************************
    TEST(horizontal_line_left_right)
    {
      Point first = { -5, 5 };
      Point last  = {  5, 5 };

      BresenhamLine bl(first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{-5, 5 }, Point{-4, 5 }, Point{-3, 5 }, Point{-2, 5 }, Point{-1, 5 }, Point{ 0, 5 },
                                   Point{ 1, 5 }, Point{ 2, 5 }, Point{ 3, 5 }, Point{ 4, 5 }, Point{ 5, 5} };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(horizontal_line_right_left)
    {
      Point first = {  5, 5 };
      Point last  = { -5, 5 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{ 5, 5 }, Point{ 4, 5 }, Point{ 3, 5 }, Point{ 2, 5 }, Point{ 1, 5 }, Point{ 0, 5 },
                                   Point{-1, 5 }, Point{-2, 5 }, Point{-3, 5 }, Point{-4, 5 }, Point{-5, 5} };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(vertical_line_bottom_top)
    {
      Point first = { 5, -5 };
      Point last  = { 5,  5 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{ 5, -5 }, Point{ 5, -4 }, Point{ 5, -3 }, Point{ 5, -2 }, Point{ 5, -1 }, Point{ 5,  0 },
                                   Point{ 5,  1 }, Point{ 5,  2 }, Point{ 5,  3 }, Point{ 5,  4 }, Point{ 5,  5} };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(vertical_line_top_bottom)
    {
      Point first = { 5,  5 };
      Point last  = { 5, -5 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{ 5,  5 }, Point{ 5,  4 }, Point{ 5,  3 }, Point{ 5,  2 }, Point{ 5,  1 }, Point{ 5,  0 },
                                   Point{ 5, -1 }, Point{ 5, -2 }, Point{ 5, -3 }, Point{ 5, -4 }, Point{ 5, -5} };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(low_slope_diagonal_line_bottom_left_top_right)
    {
      Point first = { -5, -3 };
      Point last  = {  5,  3};

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{ -5, -3 }, Point{ -4, -3 }, Point{ -3, -2 }, Point{ -2, -1 }, Point{ -1, -1 }, Point{ 0, 0 },
                                   Point{ 1,   1 }, Point{  2,  1 }, Point{  3,  2 }, Point{  4,  2 }, Point{  5,  3 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(low_slope_diagonal_line_top_right_bottom_left)
    {
      Point first = {  5,  3 };
      Point last  = { -5, -3 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{  5,  3 }, Point{ 4,   3 }, Point{  3,  2 }, Point{  2,  1 }, Point{  1,  1 }, Point{ 0, 0 },
                                   Point{ -1, -1 }, Point{ -2, -1 }, Point{ -3, -2 }, Point{ -4, -2 }, Point{ -5, -3 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(low_slope_diagonal_line_bottom_right_top_left)
    {
      Point first = {  5, -3 };
      Point last  = { -5,  3 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{  5, -3 }, Point{  4, -3 }, Point{  3, -2 }, Point{  2, -1 }, Point{ 1,  -1 }, Point{ 0, 0 },
                                   Point{ -1,  1 }, Point{ -2,  1 }, Point{ -3,  2 }, Point{ -4,  2 }, Point{ -5,  3 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(low_slope_diagonal_line_top_left_bottom_right)
    {
      Point first = { -5,  3 };
      Point last  = {  5, -3 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{ -5,  3 }, Point{ -4,  3 }, Point{ -3,  2 }, Point{ -2,  1 }, Point{ -1,  1 }, Point{ 0, 0 },
                                   Point{  1, -1 }, Point{  2, -1 }, Point{  3, -2 }, Point{  4, -2 }, Point{  5, -3 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(high_slope_diagonal_line_bottom_left_top_right)
    {
      Point first = { -3, -5 };
      Point last   = { 3,  5 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{ -3, -5 }, Point{ -3, -4 }, Point{ -2, -3 }, Point{ -1, -2 }, Point{ -1, -1 }, Point{ 0, 0 },
                                   Point{ 1,   1 }, Point{  1,  2 }, Point{  2,  3 }, Point{  2,  4 }, Point{  3,  5 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(high_slope_diagonal_line_top_right_bottom_left)
    {
      Point first = {  3,  5 };
      Point last  = { -3, -5 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{  3,  5 }, Point{  3,  4 }, Point{  2,  3 }, Point{  1,  2 }, Point{  1,  1 }, Point{ 0, 0 },
                                   Point{ -1, -1 }, Point{ -1, -2 }, Point{ -2, -3 }, Point{ -2, -4 }, Point{ -3, -5 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(high_slope_diagonal_line_bottom_right_top_left)
    {
      Point first = {  3, -5 };
      Point last  = { -3,  5 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{  3, -5 }, Point{  3, -4 }, Point{  2, -3 }, Point{  1, -2 }, Point{  1, -1 }, Point{ 0, 0 },
                                   Point{ -1,  1 }, Point{ -1,  2 }, Point{ -2,  3 }, Point{ -2,  4 }, Point{ -3,  5 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(high_slope_diagonal_line_top_left_bottom_right)
    {
      Point first = { -3,  5 };
      Point last  = {  3, -5 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      std::vector<Point> expected{ Point{ -3,  5 }, Point{ -3,  4 }, Point{ -2,  3 }, Point{ -1,  2 }, Point{ -1,  1 }, Point{ 0, 0 },
                                   Point{  1, -1 }, Point{  1, -2 }, Point{  2, -3 }, Point{  2, -4 }, Point{  3, -5 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(reset_the_line_from_coordinate_2d)
    {
      Point first = { 3,  -5 };
      Point last  = { -3,  5 };

      BresenhamLine bl(first, last);

      CHECK_EQUAL(first, bl.front());
      CHECK_EQUAL(last,  bl.back());

      Point new_first = { -3,  5 };
      Point new_last  = {  3, -5 };

      bl.reset(new_first, new_last);

      std::vector<Point> expected{ Point{ -3,  5 }, Point{ -3,  4 }, Point{ -2,  3 }, Point{ -1,  2 }, Point{ -1,  1 }, Point{ 0, 0 },
                                   Point{  1, -1 }, Point{  1, -2 }, Point{  2, -3 }, Point{  2, -4 }, Point{  3, -5 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(reset_the_line_from_individual_coordinates)
    {
      Point first = { 3,  -5 };
      Point last = { -3,  5 };

      BresenhamLine bl(first, last);

      Point new_first = { -3,  5 };
      Point new_last = { 3, -5 };

      bl.reset(new_first.x, new_first.y, new_last.x, new_last.y);

      std::vector<Point> expected{ Point{ -3,  5 }, Point{ -3,  4 }, Point{ -2,  3 }, Point{ -1,  2 }, Point{ -1,  1 }, Point{ 0, 0 },
                                   Point{  1, -1 }, Point{  1, -2 }, Point{  2, -3 }, Point{  2, -4 }, Point{  3, -5 } };
      std::vector<Point> actual;

      for (BresenhamLine::const_iterator itr = bl.begin(); itr != bl.end(); ++itr)
      {
        actual.push_back(*itr);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(test_equality)
    {
      BresenhamLine bl1(0, 1, 2, 3);
      BresenhamLine bl2(0, 1, 2, 3);
      BresenhamLine bl3(0, 1, 2, 4);
      BresenhamLine bl4(0, 2, 2, 3);

      CHECK(bl1 == bl2);
      CHECK(!(bl1 != bl2));

      CHECK(bl1 != bl3);
      CHECK(!(bl1 == bl3));

      CHECK(bl1 != bl4);
      CHECK(!(bl1 == bl4));
    }
  };
}
