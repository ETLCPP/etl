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
#include <limits>

namespace
{
  SUITE(test_constant)
  {
    //*************************************************************************
    TEST(test_round_to_ceiling_signed_different_types)
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
    TEST(test_round_to_ceiling_signed_same_types)
    {
      const std::array<int32_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
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
    TEST(test_round_to_ceiling_signed_limits)
    {
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::divide_round_to_ceiling(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(1),                          etl::divide_round_to_ceiling(int32_t(1),                          std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::divide_round_to_ceiling(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_to_ceiling(int32_t(1),                          std::numeric_limits<int32_t>::min()));
    }

    //*************************************************************************
    TEST(test_round_to_ceiling_unsigned_different_types)
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
    TEST(test_round_to_ceiling_unsigned_same_types)
    {
      const std::array<uint32_t, 21> numerator{ 0,  49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
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
    TEST(test_round_to_ceiling_unsigned_limits)
    {
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::divide_round_to_ceiling(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(1),                          etl::divide_round_to_ceiling(uint32_t(1),                         std::numeric_limits<uint32_t>::max()));
    }

    //*************************************************************************
    TEST(test_round_to_floor_signed_different_types)
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
    TEST(test_round_to_floor_signed_same_types)
    {
      const std::array<int32_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
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
    TEST(test_round_to_floor_signed_limits)
    {
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::divide_round_to_floor(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_to_floor(int32_t(1),                          std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::divide_round_to_floor(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(int32_t(-1),                         etl::divide_round_to_floor(int32_t(1),                          std::numeric_limits<int32_t>::min()));
    }

    //*************************************************************************
    TEST(test_round_to_floor_unsigned_different_types)
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
    TEST(test_round_to_floor_unsigned_same_types)
    {
      const std::array<uint32_t, 21> numerator{ 0,  49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
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
    TEST(test_round_to_floor_unsigned_limits)
    {
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::divide_round_to_floor(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(0),                          etl::divide_round_to_floor(uint32_t(1),                          std::numeric_limits<uint32_t>::max()));
    }

    //*************************************************************************
    TEST(test_round_to_zero_signed_different_types)
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
    TEST(test_round_to_zero_signed_same_types)
    {
      const std::array<int32_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
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
    TEST(test_round_to_zero_signed_limits)
    {
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::divide_round_to_zero(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_to_zero(int32_t(1),                          std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::divide_round_to_zero(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_to_zero(int32_t(1),                          std::numeric_limits<int32_t>::min()));
    }

    //*************************************************************************
    TEST(test_round_to_zero_unsigned_different_types)
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
    TEST(test_round_to_zero_unsigned_same_types)
    {
      const std::array<uint32_t, 21> numerator{ 0,  49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
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
    TEST(test_round_to_zero_unsigned_limits)
    {
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::divide_round_to_zero(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(0),                          etl::divide_round_to_zero(uint32_t(1),                          std::numeric_limits<uint32_t>::max()));
    }

    //*************************************************************************
    TEST(test_round_to_infinity_signed_different_types)
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
    TEST(test_round_to_infinity_signed_same_types)
    {
      const std::array<int32_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
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
    TEST(test_round_to_infinity_signed_limits)
    {
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::divide_round_to_infinity(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(1),                          etl::divide_round_to_infinity(int32_t(1),                          std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::divide_round_to_infinity(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(int32_t(-1),                         etl::divide_round_to_infinity(int32_t(1),                          std::numeric_limits<int32_t>::min()));
    }

    //*************************************************************************
    TEST(test_round_to_infinity_unsigned_different_types)
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
    TEST(test_round_to_infinity_unsigned_same_types)
    {
      const std::array<uint32_t, 21> numerator{ 0, 49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
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
    TEST(test_round_to_infinity_unsigned_limits)
    {
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::divide_round_to_infinity(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(1),                          etl::divide_round_to_infinity(uint32_t(1),                          std::numeric_limits<uint32_t>::max()));
    }

    //*************************************************************************
    TEST(test_round_to_half_up_signed_different_types)
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
    TEST(test_round_to_half_up_signed_same_types)
    {
      const std::array<int32_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
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
    TEST(test_round_to_half_up_signed_limits)
    {
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::divide_round_half_up(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_half_up(int32_t(1),                          std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::divide_round_half_up(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_half_up(int32_t(1),                          std::numeric_limits<int32_t>::min()));
    }

    //*************************************************************************
    TEST(test_round_to_half_up_unsigned_different_types)
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
    TEST(test_round_to_half_up_unsigned_same_types)
    {
      const std::array<uint32_t, 21> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400 };
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
    TEST(test_round_to_half_up_unsigned_limits)
    {
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::divide_round_half_up(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(0U),                         etl::divide_round_half_up(uint32_t(1),                          std::numeric_limits<uint32_t>::max()));
    }

    //*************************************************************************
    TEST(test_round_to_half_down_signed_different_types)
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
    TEST(test_round_to_half_down_signed_same_types)
    {
      const std::array<int32_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
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
    TEST(test_round_to_half_down_signed_limits)
    {
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::divide_round_half_down(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_half_down(int32_t(1),                          std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::divide_round_half_down(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(int32_t(-1),                         etl::divide_round_half_down(int32_t(1),                          std::numeric_limits<int32_t>::min()));
    }

    //*************************************************************************
    TEST(test_round_to_half_down_unsigned_different_types)
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
    TEST(test_round_to_half_down_unsigned_same_types)
    {
      const std::array<uint32_t, 21> numerator{ 0, 49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
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
    TEST(test_round_to_half_down_unsigned_limits)
    {
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::divide_round_half_down(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(0U),                         etl::divide_round_half_down(uint32_t(1),                          std::numeric_limits<uint32_t>::max()));
    }

    //*************************************************************************
    TEST(test_round_to_half_even_signed_different_types)
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
    TEST(test_round_to_half_even_signed_same_types)
    {
      const std::array<int32_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
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
    TEST(test_round_to_half_even_signed_limits)
    {
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::divide_round_half_even(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_half_even(int32_t(1),                          std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::divide_round_half_even(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(int32_t(-1),                         etl::divide_round_half_even(int32_t(1),                          std::numeric_limits<int32_t>::min()));
    }

    //*************************************************************************
    TEST(test_round_to_half_even_unsigned_different_types)
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

    //*************************************************************************
    TEST(test_round_to_half_even_unsigned_same_types)
    {
      const std::array<uint32_t, 21> numerator{ 0, 49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
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

    //*************************************************************************
    TEST(test_round_to_half_even_unsigned_limits)
    {
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::divide_round_half_even(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(0U),                         etl::divide_round_half_even(uint32_t(1),                          std::numeric_limits<uint32_t>::max()));
    }

    //*************************************************************************
    TEST(test_round_to_half_odd_signed_different_types)
    {
      const std::array<int16_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -250, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0, 0,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  4,  4,  4,
                                              0, 0, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -4, -4, -4,
                                              0, 0, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -4, -4, -4,
                                              0, 0,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  4,  4,  4 };
      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_odd(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_odd_signed_same_types)
    {
      const std::array<int32_t, 42> numerator{ 0,  49,  50,  51,  99,  100,  149,  150,  151,  199,  200,  249,  250,  251,  299,  300,  349,  350,  351,  399,  400,
                                               0, -49, -50, -51, -99, -100, -149, -150, -151, -199, -200, -249, -250, -251, -299, -300, -349, -350, -351, -399, -400 };
      const std::array<int32_t, 2>  denominator{ 100, -100 };
      const std::array<int32_t, 84> expected{ 0, 0,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  4,  4,  4,
                                              0, 0, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -4, -4, -4,
                                              0, 0, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -4, -4, -4,
                                              0, 0,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  4,  4,  4 };
      std::array<int32_t, 84> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_odd(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_odd_signed_limits)
    {
      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), etl::divide_round_half_odd(std::numeric_limits<int32_t>::max(), int32_t(1)));
      CHECK_EQUAL(int32_t(0),                          etl::divide_round_half_odd(int32_t(1),                          std::numeric_limits<int32_t>::max()));
      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), etl::divide_round_half_odd(std::numeric_limits<int32_t>::min(), int32_t(1)));
      CHECK_EQUAL(int32_t(-1),                         etl::divide_round_half_odd(int32_t(1),                          std::numeric_limits<int32_t>::min()));
    }

    //*************************************************************************
    TEST(test_round_to_half_odd_unsigned_different_types)
    {
      const std::array<uint16_t, 21> numerator{ 0, 49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4 };
      std::array<uint32_t, 42> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_odd(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_odd_unsigned_same_types)
    {
      const std::array<uint32_t, 21> numerator{ 0, 49, 50, 51, 99, 100, 149, 150, 151, 199, 200, 249, 250, 251, 299, 300, 349, 350, 351, 399, 400 };
      const std::array<uint32_t, 1>  denominator{ 100 };
      const std::array<uint32_t, 21> expected{ 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4 };
      std::array<uint32_t, 42> actual{};

      for (size_t i = 0; i < denominator.size(); ++i)
      {
        for (size_t j = 0; j < numerator.size(); ++j)
        {
          size_t index = j + (i * numerator.size());

          actual[index] = etl::divide_round_half_odd(numerator[j], denominator[i]);
        }
      }

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(test_round_to_half_odd_unsigned_limits)
    {
      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), etl::divide_round_half_odd(std::numeric_limits<uint32_t>::max(), uint32_t(1)));
      CHECK_EQUAL(uint32_t(0U),                         etl::divide_round_half_odd(uint32_t(1),                          std::numeric_limits<uint32_t>::max()));
    }
  };
}
