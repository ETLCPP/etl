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

#include "etl/center_of_gravity_3d.h"
#include "etl/utility.h"
#include "etl/scaled_rounding.h"

#include <vector>
#include <utility>

namespace
{
  using Cog = etl::center_of_gravity_3d<int, int>;

  //***********************************
  using Pixel = etl::coordinate_3d<int, uint8_t>;

  //***********************************
  std::vector<Pixel> pixels =
  {
    { 31, 21, 11, 1 }, { 40, 21, 11, 1 }, { 31, 30, 11, 1 }, { 40, 30, 11, 1 }, 
    { 21, 11, 11, 2 }, { 21, 40, 11, 2 }, { 50, 11, 11, 2 }, { 50, 40, 11, 2 },
    { 31, 21, 30, 1 }, { 40, 21, 30, 1 }, { 31, 30, 30, 1 }, { 40, 30, 30, 1 }, 
    { 21, 11, 30, 2 }, { 21, 40, 30, 2 }, { 50, 11, 30, 2 }, { 50, 40, 30, 2 }, 
  };

  SUITE(test_center_of_gravity_3d)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Cog cog;

      CHECK_EQUAL(0,   cog.get_x<int>());
      CHECK_EQUAL(0,   cog.get_y<int>());
      CHECK_EQUAL(0,   cog.get_z<int>());
      CHECK_EQUAL(0.0, cog.get_x<double>());
      CHECK_EQUAL(0.0, cog.get_y<double>());
      CHECK_EQUAL(0.0, cog.get_z<double>());

      CHECK((std::tuple<int, int, int>(0, 0, 0)) == (cog.get<std::tuple<int, int, int>, int>()));
      CHECK((etl::coordinate_3d<int>(0, 0, 0))   == (cog.get<etl::coordinate_3d<int>>()));
    }

    //*************************************************************************
    TEST(test_all_same_coordinates)
    {
      Cog cog;

      for (int i = 0; i < 10; ++i)
      {
        cog(1, 2, 3, 4);
      }

      CHECK_EQUAL(1,   cog.get_x<int>());
      CHECK_EQUAL(2,   cog.get_y<int>());
      CHECK_EQUAL(1.0, cog.get_x<double>());
      CHECK_EQUAL(2.0, cog.get_y<double>());
      CHECK((std::tuple<int, int, int>(1, 2, 3))                == (cog.get<std::tuple<int, int, int>, int>()));
      CHECK((std::tuple<double, double, double>(1.0, 2.0, 3.0)) == (cog.get<std::tuple<double, double, double>, double>()));
      CHECK((etl::coordinate_3d<int>(1, 2, 3))                  == (cog.get<etl::coordinate_3d<int>>()));
      CHECK((etl::coordinate_3d<double>(1, 2, 3))               == (cog.get<etl::coordinate_3d<double>>()));
    }

    //*************************************************************************
    TEST(test_vector_of_coordinates)
    {
      Cog cog;
      using coordi = etl::coordinate_3d<int>;
      using coordd = etl::coordinate_3d<double>;

      for (const auto& pixel : pixels)
      {
        cog(pixel);
      }

      CHECK_EQUAL(35,   cog.get_x<int>());
      CHECK_EQUAL(25,   cog.get_y<int>());
      CHECK_EQUAL(20,   cog.get_z<int>());

      CHECK_EQUAL(35.5, cog.get_x<double>());
      CHECK_EQUAL(25.5, cog.get_y<double>());
      CHECK_EQUAL(20.5, cog.get_z<double>());

      CHECK((coordi(35, 25, 20))       == (cog.get<coordi>()));
      CHECK((coordd(35.5, 25.5, 20.5)) == (cog.get<coordd>()));
    }

    //*************************************************************************
    TEST(test_vector_of_coordinates_with_round_half_up)
    {
      Cog cog;
      using coord = etl::coordinate_3d<int>;

      for (const auto& pixel : pixels)
      {
        cog(pixel);
      }

      constexpr int Scale = 10;

      auto rounding = &etl::round_half_up_scaled<Scale, int>;

      CHECK_EQUAL(36, (cog.get_x<int, Scale>(rounding)));
      CHECK_EQUAL(26, (cog.get_y<int, Scale>(rounding)));
      CHECK((coord(36, 26, 26)) == (cog.get<coord, coord::axis_type, Scale>(rounding)));
    }
  };
}
