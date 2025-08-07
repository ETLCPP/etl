/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#include "etl/rounded_integral_division.h"

#include <stdint.h>
#include <array>

namespace
{
  SUITE(test_constant)
  {
    //*************************************************************************
    TEST(test_round_to_ceiling_signed)
    {
      const std::array<int16_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -259, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0, 1, 1, 1, 1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,
                                              0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4,
                                              0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, 
                                              0, 1, 1, 1, 1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4 };
      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_to_ceiling(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_ceiling_unsigned)
    {
      const std::array<uint16_t, 21> numerator{ 0,  49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0, 1, 1, 1, 1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4 };
      std::array<uint32_t, 21> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          actual[j + (i * numerator.size())] = etl::divide_round_to_ceiling(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_floor_signed)
    {
      const std::array<int16_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -259, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,
                                              0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4,
                                              0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4, 
                                              0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4 };

      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_to_floor(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_floor_unsigned)
    {
      const std::array<uint16_t, 21> numerator{ 0,  49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4 };
      std::array<uint32_t, 21> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          actual[j + (i * numerator.size())] = etl::divide_round_to_floor(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_zero_signed)
    {
      const std::array<int16_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -259, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0, 0, 0, 0, 0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,
                                              0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4,
                                              0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, 
                                              0, 0, 0, 0, 0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4 };
      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_to_zero(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_zero_unsigned)
    {
      const std::array<uint16_t, 21> numerator{ 0,  49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4 };
      std::array<uint32_t, 21> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          actual[j + (i * numerator.size())] = etl::divide_round_to_zero(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_infinity_signed)
    {
      const std::array<int16_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -259, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,
                                              0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4,
                                              0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4, -4, -4,
                                              0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4 };
      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_to_infinity(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_infinity_unsigned)
    {
      const std::array<uint16_t, 21> numerator{ 0, 49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4 };
      std::array<uint32_t, 21> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_to_infinity(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_up_signed)
    {
      const std::array<int16_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -259, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0, 0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4,
                                              0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4, -4,
                                              0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4, -4,
                                              0, 0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4 };
      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_up(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_up_unsigned)
    {
      const std::array<uint16_t, 21> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4 };
      std::array<uint32_t, 21> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_up(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_down_signed)
    {
      const std::array<int16_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -250, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0, 0, 0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,
                                              0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4,
                                              0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -4, -4, -4,
                                              0, 0, 0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4 };
      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_down(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_down_unsigned)
    {
      const std::array<uint16_t, 21> numerator{ 0, 49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4 };
      std::array<uint32_t, 42> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_down(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_even_signed)
    {
      const std::array<int16_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -250, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0, 0, 0,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,
                                              0, 0, 0, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -3, -3, -3, -3, -4, -4, -4, -4,
                                              0, 0, 0, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -3, -3, -3, -3, -4, -4, -4, -4,
                                              0, 0, 0,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4 };
      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_even(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_even_unsigned)
    {
      const std::array<uint16_t, 21> numerator{ 0, 49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4 };
      std::array<uint32_t, 42> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_even(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }
  };
}
