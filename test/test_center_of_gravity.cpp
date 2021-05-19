///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2021 jwellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#include "unit_test_framework.h"

#include "etl/center_of_gravity.h"
#include "etl/utility.h"
#include "etl/scaled_rounding.h"

#include <vector>

namespace
{
  using Cog = etl::center_of_gravity<int, int, uint8_t>;

  //***********************************
  struct Point
  {    
    int x;
    int y;
    uint8_t value;
  };

  //***********************************
  std::vector<Point> points =
  {
    { 31, 21, 1 }, { 40, 21, 1 }, { 31, 30, 1 }, { 40, 30, 1 }, 
    { 21, 11, 2 }, { 21, 40, 2 }, { 50, 11, 2 }, { 50, 40, 2 }, 
  };

  //***********************************
  struct Proxy
  {
    void operator()(Point point)
    {
      cog(point.x, point.y, point.value);
    }

    Cog& cog;
  }; 

  SUITE(test_center_of_gravity)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Cog cog;

      CHECK_EQUAL(0,   cog.get_x<int>());
      CHECK_EQUAL(0,   cog.get_y<int>());
      CHECK_EQUAL(0.0, cog.get_x<double>());
      CHECK_EQUAL(0.0, cog.get_y<double>());
      CHECK((std::pair<int, int>(0, 0))     == (cog.get<std::pair<int, int>, int>()));
      CHECK((etl::coordinate_2d<int>(0, 0)) == (cog.get<etl::coordinate_2d<int>>()));
    }

    //*************************************************************************
    TEST(test_all_same_coordinates)
    {
      Cog cog;

      for (int i = 0; i < 10; ++i)
      {
        cog(1, 2, 3);
      }

      CHECK_EQUAL(1,   cog.get_x<int>());
      CHECK_EQUAL(2,   cog.get_y<int>());
      CHECK_EQUAL(1.0, cog.get_x<double>());
      CHECK_EQUAL(2.0, cog.get_y<double>());
      CHECK((std::pair<int, int>(1, 2))           == (cog.get<std::pair<int, int>, int>()));
      CHECK((std::pair<double, double>(1.0, 2.0)) == (cog.get<std::pair<double, double>, double>()));
      CHECK((etl::coordinate_2d<int>(1, 2))       == (cog.get<etl::coordinate_2d<int>>()));
      CHECK((etl::coordinate_2d<double>(1, 2))    == (cog.get<etl::coordinate_2d<double>>()));
    }

    //*************************************************************************
    TEST(test_vector_of_coordinates)
    {
      Cog cog;
      Proxy proxy{ cog };
      using coordi = etl::coordinate_2d<int>;
      using coordd = etl::coordinate_2d<double>;

      for (const auto& point : points)
      {
        proxy(point);
      }

      CHECK_EQUAL(35,   cog.get_x<int>());
      CHECK_EQUAL(25,   cog.get_y<int>());

      CHECK_EQUAL(35.5, cog.get_x<double>());
      CHECK_EQUAL(25.5, cog.get_y<double>());

      CHECK((coordi(35, 25))     == (cog.get<coordi>()));
      CHECK((coordd(35.5, 25.5)) == (cog.get<coordd>()));
    }

    //*************************************************************************
    TEST(test_vector_of_coordinates_with_round_half_up)
    {
      Cog cog;
      Proxy proxy{ cog };
      using coord = etl::coordinate_2d<int>;

      for (const auto& point : points)
      {
        proxy(point);
      }

      constexpr int Scale = 10;

      int(*rounding)(int) = &etl::round_half_up_scaled<Scale, int>;

      CHECK_EQUAL(36, (cog.get_x<int, Scale>(rounding)));
      CHECK_EQUAL(26, (cog.get_y<int, Scale>(rounding)));
      CHECK((coord(36, 26)) == (cog.get<coord, coord::value_type, Scale>(rounding)));
    }
  };
}
