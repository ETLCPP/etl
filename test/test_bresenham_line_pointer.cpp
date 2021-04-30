/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2020 jwellbelove

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

#include "etl/bresenham_line_pointer.h"

#include <vector>
#include <array>

namespace etl
{
  //template <typename T>
  //std::ostream& operator << (std::ostream& os, const etl::coordinate_2d<T>& point)
  //{
  //  os << "(" << int(point.x) << "," << int(point.y) << ")";
  //  return os;
  //}
}

namespace
{
  using Value = uint16_t;

  using Point = etl::coordinate_2d<Value>;

  std::ostream& operator << (std::ostream& os, const Point& point)
  {
    os << "(" << int(point.x) << "," << int(point.y) << ")";
    return os;
  }

  // The 11 x 11 image
  std::array<std::array<Value, 10>, 10> image = 
  {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
  };

  const std::array<std::array<const Value, 10>, 10> const_image = 
  {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
  };

  // Convert a coordinate to a pointer.
  constexpr Value* GetPointer(Value x, Value y)
  {
    return &image[y][x];
  }
  
  constexpr const Value* GetConstPointer(Value x, Value y)
  {
    return &const_image[y][x];
  }

  // Convert a coordinate to an expected value.
  constexpr Value GetValue(Value x, Value y)
  {
    return (x + (y * 10));
  }

  Value* origin             = &image[0][0];
  const Value* const_origin = &const_image[0][0];
  constexpr size_t stride   = image.size();

  using BresenhamLinePointer      = etl::bresenham_line_pointer<Value>;
  using BresenhamLinePointerConst = etl::bresenham_line_pointer<const Value>;

  SUITE(test_bresenham_line_pointer)
  {
    //*************************************************************************
    TEST(horizontal_line_left_right)
    {
      Point first = { 0, 5 };
      Point last  = { 9, 5 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(0, 5), GetPointer(1, 5), GetPointer(2, 5), GetPointer(3, 5), GetPointer(4, 5),
                                     GetPointer(5, 5), GetPointer(6, 5), GetPointer(7, 5), GetPointer(8, 5), GetPointer(9, 5) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(horizontal_line_left_right_const)
    {
      Point first = { 0, 5 };
      Point last  = { 9, 5 };

      const Value* first_ptr = GetConstPointer(first.x, first.y);
      const Value* last_ptr  = GetConstPointer(last.x, last.y);

      BresenhamLinePointerConst bl(const_origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<const Value*> expected { GetConstPointer(0, 5), GetConstPointer(1, 5), GetConstPointer(2, 5), GetConstPointer(3, 5), GetConstPointer(4, 5),
                                           GetConstPointer(5, 5), GetConstPointer(6, 5), GetConstPointer(7, 5), GetConstPointer(8, 5), GetConstPointer(9, 5) };
        
      std::vector<const Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(horizontal_line_right_left)
    {
      Point first = { 9, 5 };
      Point last  = { 0, 5 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(9, 5), GetPointer(8, 5), GetPointer(7, 5), GetPointer(6, 5), GetPointer(5, 5),
                                     GetPointer(4, 5), GetPointer(3, 5), GetPointer(2, 5), GetPointer(1, 5), GetPointer(0, 5) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(vertical_line_bottom_top)
    {
      Point first = { 5, 0 };
      Point last  = { 5, 9 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(5, 0), GetPointer(5, 1), GetPointer(5, 2), GetPointer(5, 3), GetPointer(5, 4),
                                     GetPointer(5, 5), GetPointer(5, 6), GetPointer(5, 7), GetPointer(5, 8), GetPointer(5, 9) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(vertical_line_top_bottom)
    {
      Point first = { 5, 9 };
      Point last  = { 5, 0 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(5, 9), GetPointer(5, 8), GetPointer(5, 7), GetPointer(5, 6), GetPointer(5, 5),
                                     GetPointer(5, 4), GetPointer(5, 3), GetPointer(5, 2), GetPointer(5, 1), GetPointer(5, 0) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(low_slope_diagonal_line_bottom_left_top_right)
    {
      Point first = { 0, 6 };
      Point last  = { 9, 2 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(0, 6), GetPointer(1, 6), GetPointer(2, 5), GetPointer(3, 5), GetPointer(4, 4),
                                     GetPointer(5, 4), GetPointer(6, 3), GetPointer(7, 3), GetPointer(8, 2), GetPointer(9, 2) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(low_slope_diagonal_line_top_right_bottom_left)
    {
      Point first = { 9, 2 };
      Point last  = { 0, 6 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(9, 2), GetPointer(8, 2), GetPointer(7, 3), GetPointer(6, 3), GetPointer(5, 4),
                                     GetPointer(4, 4), GetPointer(3, 5), GetPointer(2, 5), GetPointer(1, 6), GetPointer(0, 6) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(low_slope_diagonal_line_bottom_right_top_left)
    {
      Point first = { 9, 6 };
      Point last  = { 0, 2 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(9, 6), GetPointer(8, 6), GetPointer(7, 5), GetPointer(6, 5), GetPointer(5, 4),
                                     GetPointer(4, 4), GetPointer(3, 3), GetPointer(2, 3), GetPointer(1, 2), GetPointer(0, 2) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(low_slope_diagonal_line_top_left_bottom_right)
    {
      Point first = { 0, 2 };
      Point last  = { 9, 6 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(0, 2), GetPointer(1, 2), GetPointer(2, 3), GetPointer(3, 3), GetPointer(4, 4),
                                     GetPointer(5, 4), GetPointer(6, 5), GetPointer(7, 5), GetPointer(8, 6), GetPointer(9, 6) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(high_slope_diagonal_line_bottom_left_top_right)
    {
      Point first = { 2, 9 };
      Point last  = { 6, 0 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(2, 9), GetPointer(2, 8), GetPointer(3, 7), GetPointer(3, 6), GetPointer(4, 5),
                                     GetPointer(4, 4), GetPointer(5, 3), GetPointer(5, 2), GetPointer(6, 1), GetPointer(6, 0) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(high_slope_diagonal_line_top_right_bottom_left)
    {
      Point first = { 6, 0 };
      Point last  = { 2, 9 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(6, 0), GetPointer(6, 1), GetPointer(5, 2), GetPointer(5, 3), GetPointer(4, 4),
                                     GetPointer(4, 5), GetPointer(3, 6), GetPointer(3, 7), GetPointer(2, 8), GetPointer(2, 9) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(high_slope_diagonal_line_bottom_right_top_left)
    {
      Point first = { 6, 9};
      Point last  = { 2, 0};

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(6, 9), GetPointer(6, 8), GetPointer(5, 7), GetPointer(5, 6), GetPointer(4, 5),
                                     GetPointer(4, 4), GetPointer(3, 3), GetPointer(3, 2), GetPointer(2, 1), GetPointer(2, 0) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(high_slope_diagonal_line_top_left_bottom_right)
    {
      Point first = { 2, 0 };
      Point last  = { 6, 9 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      std::vector<Value*> expected { GetPointer(2, 0), GetPointer(2, 1), GetPointer(3, 2), GetPointer(3, 3), GetPointer(4, 4),
                                     GetPointer(4, 5), GetPointer(5, 6), GetPointer(5, 7), GetPointer(6, 8), GetPointer(6, 9) };
        
      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(reset_the_line_from_individual_coordinates)
    {
      Point first = { 2, 0 };
      Point last  = { 6, 9 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      Point new_first = { 6, 9 };
      Point new_last  = { 2, 0 };

      bl.reset(new_first.x, new_first.y, new_last.x, new_last.y);

      std::vector<Value*> expected { GetPointer(6, 9), GetPointer(6, 8), GetPointer(5, 7), GetPointer(5, 6), GetPointer(4, 5),
                                     GetPointer(4, 4), GetPointer(3, 3), GetPointer(3, 2), GetPointer(2, 1), GetPointer(2, 0) };

      std::vector<Value*> actual;

      for (auto p : bl)
      {
        actual.push_back(p);
      }

      CHECK_EQUAL(expected.size(), bl.size());
      CHECK_EQUAL(expected.size(), actual.size());
      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), (std::max(expected.size(), actual.size())));
    }

    //*************************************************************************
    TEST(diagonal_line_write_read)
    {
      Point first = { 2, 0 };
      Point last  = { 6, 9 };

      Value* first_ptr = GetPointer(first.x, first.y);
      Value* last_ptr  = GetPointer(last.x, last.y);

      BresenhamLinePointer bl(origin, stride, first.x, first.y, last.x, last.y);

      CHECK_EQUAL(first_ptr, bl.front());
      CHECK_EQUAL(last_ptr,  bl.back());

      // Write 100 to the line elements.
      for (auto itr : bl)
      {
        *itr = 100;
      }

      // Read them back.
      for (auto itr : bl)
      {
        CHECK_EQUAL(*itr, 100);
      }
    }

    //*************************************************************************
    TEST(test_equality)
    {
      BresenhamLinePointer bl1(origin, stride, 0, 1, 2, 3);
      BresenhamLinePointer bl2(origin, stride, 0, 1, 2, 3);
      BresenhamLinePointer bl3(origin, stride, 0, 1, 2, 4);
      BresenhamLinePointer bl4(origin, stride, 0, 2, 2, 3);

      CHECK(bl1 == bl2);
      CHECK(!(bl1 != bl2));

      CHECK(bl1 != bl3);
      CHECK(!(bl1 == bl3));

      CHECK(bl1 != bl4);
      CHECK(!(bl1 == bl4));
    }
  };
}
