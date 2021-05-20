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

#include "etl/center_of_gravity_1d.h"
#include "etl/utility.h"
#include "etl/scaled_rounding.h"
#include "etl/utility.h"

#include <vector>

namespace
{
  using Cog = etl::center_of_gravity_1d<int, int>;

  //***********************************
  using Pixel = etl::pair<int, uint8_t>;

  //***********************************
  std::vector<Pixel> pixels =
  {
    { 31, 1 }, { 40, 1 }, { 31, 1 }, { 40, 1 }, 
    { 21, 2 }, { 21, 2 }, { 50, 2 }, { 50, 2 }, 
  };

  SUITE(test_center_of_gravity_1d)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Cog cog;

      CHECK_EQUAL(0,   cog.get<int>());
      CHECK_EQUAL(0.0, cog.get<double>());
    }

    //*************************************************************************
    TEST(test_all_same_coordinates)
    {
      Cog cog;

      for (int i = 0; i < 10; ++i)
      {
        cog(1, 2);
      }

      CHECK_EQUAL(1,   cog.get<int>());
      CHECK_EQUAL(1.0, cog.get<double>());
    }

    //*************************************************************************
    TEST(test_vector_of_coordinates)
    {
      Cog cog;
      using coordi = etl::pair<int,    uint8_t>;
      using coordd = etl::pair<double, uint8_t>;

      for (const auto& pixel : pixels)
      {
        cog(pixel.first, pixel.second);
      }

      CHECK_EQUAL(35,   cog.get<int>());
      CHECK_EQUAL(35.5, cog.get<double>());
    }

    //*************************************************************************
    TEST(test_vector_of_coordinates_with_round_half_up)
    {
      Cog cog;
      using coord = etl::coordinate_2d<int>;

      for (const auto& pixel : pixels)
      {
        cog(pixel.first, pixel.second);
      }

      constexpr int Scale = 10;

      auto rounding = &etl::round_half_up_scaled<Scale, int>;

      CHECK_EQUAL(36, (cog.get<int, Scale>(rounding)));
    }
  };
}
