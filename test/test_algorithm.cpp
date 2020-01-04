/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include "etl/algorithm.h"
#include "etl/container.h"

#include "data.h"

#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>

namespace
{
  using NDC = TestDataNDC<int>;
  std::random_device rng;
  std::mt19937 urng(rng());

  typedef std::vector<int> Data;
  Data data = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  constexpr size_t SIZE = 10;

  int dataS[SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  std::list<int> dataSL = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  struct StructData
  {
    int a;
    int b;
  };

  bool operator ==(const StructData& lhs, const StructData& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b);
  }

  struct StructDataPredicate
  {
    bool operator ()(const StructData& lhs, const StructData& rhs) const
    {
      return lhs.a < rhs.a;
    }
  };

  struct StructDataEquality
  {
    bool operator ()(const StructData& lhs, const StructData& rhs) const
    {
      return lhs.a == rhs.a;
    }
  };

  std::ostream& operator << (std::ostream& os, const StructData& data_)
  {
    os << data_.a << "," << data_.b;
    return os;
  }

  SUITE(test_algorithm)
  {
    //*************************************************************************
    TEST(copy_4_parameter_random_iterator)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      int data2[] = { 1, 2, 3, 4, 5 };

      int out1[10];
      int out2[5];

      int check1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      int check2[] = { 1, 2, 3, 4, 5 };
      int check3[] = { 1, 2, 3, 4, 5, 0, 0, 0, 0, 0 };

      int* result;

      // Same size.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy(std::begin(data2), std::end(data2), std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::begin(out1) + 5, result);
      is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(copy_4_parameter_non_random_iterator)
    {
      std::list<int> data1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      std::list<int> data2 = { 1, 2, 3, 4, 5 };

      int out1[10];
      int out2[5];

      int check1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      int check2[] = { 1, 2, 3, 4, 5 };
      int check3[] = { 1, 2, 3, 4, 5, 0, 0, 0, 0, 0 };

      int* result;

      // Same size.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy(std::begin(data2), std::end(data2), std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::begin(out1) + 5, result);
      is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(copy_n_4_parameter)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      int out1[10];
      int out2[5];

      int check1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      int check2[] = { 1, 2, 3, 4, 5 };
      int check3[] = { 1, 2, 3, 4, 5, 0, 0, 0, 0, 0 };

      int* result;

      // Same size.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_n(std::begin(data1), 10, std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_n(std::begin(data1), 10, std::begin(out2), std::end(out2));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_n(std::begin(data1), 5, std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::begin(out1) + 5, result);
      is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(copy_2n_4_parameter)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      int out1[10];
      int out2[5];

      int check1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      int check2[] = { 1, 2, 3, 4, 5 };
      int check3[] = { 1, 2, 3, 4, 5, 0, 0, 0, 0, 0 };

      int* result;

      // Same size.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_n(std::begin(data1), 10, std::begin(out1), 10);
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_n(std::begin(data1), 10, std::begin(out2), 5);
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_n(std::begin(data1), 5, std::begin(out1), 10);
      CHECK_EQUAL(std::begin(out1) + 5, result);
      is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(copy_n_if)
    {
      int data1[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      // Copy everything less than 5.
      int *pout = data2;
      for (int* pin = std::begin(data1); pin != std::begin(data1) + 6; ++pin)
      {
        if (*pin < 5)
        {
          *pout++ = *pin;
        }
      }
      etl::copy_n_if(std::begin(data1), 6, std::begin(data3), std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool is_same = std::equal(std::begin(data2), std::end(data2), std::begin(data3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(copy_if_4_parameter)
    {
      int data1[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };

      int out1[4];
      int out2[2];
      int out3[10];

      int check1[] = { 1, 2, 3, 4 };
      int check2[] = { 1, 2 };
      int check3[] = { 1, 2, 3, 4, 0, 0, 0, 0, 0, 0 };

      int* result;

      // Exact size.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_if(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1), std::bind(std::less<int>(), std::placeholders::_1, 5));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_if(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2), std::bind(std::less<int>(), std::placeholders::_1, 5));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Destination larger.
      std::fill(std::begin(out3), std::end(out3), 0);
      result = etl::copy_if(std::begin(data1), std::end(data1), std::begin(out3), std::end(out3), std::bind(std::less<int>(), std::placeholders::_1, 5));
      CHECK_EQUAL(std::begin(out3) + 4, result);
      is_same = std::equal(std::begin(out3), std::end(out3), std::begin(check3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(binary_find)
    {
      int data1[] = { 1, 2, 3, 5, 6, 7, 8 };

      // Find the element of value 5.
      int* p = etl::binary_find(std::begin(data1), std::end(data1), 5);
      CHECK_EQUAL(5, *p);

      // Find the element of value 4.
      p = etl::binary_find(std::begin(data1), std::end(data1), 4);
      CHECK_EQUAL(std::end(data1), p);
    }

    //*************************************************************************
    TEST(binary_find_StructDataPredicate_StructDataEquality)
    {
      StructData data1[] = { { 1, 8 }, { 2, 7 }, { 3, 6 },{ 4, 5 },{ 5, 4 },{ 6, 3 },{ 7, 2 },{ 8, 1 } };
      StructData test1   = { 4, 5 };
      StructData test2   = { 9, 0 };

      // Find the element of value 5.
      StructData* p = etl::binary_find(std::begin(data1), std::end(data1), test1, StructDataPredicate(), StructDataEquality());
      CHECK_EQUAL(test1, *p);

      // Find the element of value 4.
      p = etl::binary_find(std::begin(data1), std::end(data1), test2, StructDataPredicate(), StructDataEquality());
      CHECK_EQUAL(std::end(data1), p);
    }

    //*************************************************************************
    TEST(for_each_if)
    {
      int data1[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };

      struct Sum
      {
        Sum() : sum(0) { }

        Sum& operator()(int i)
        {
          sum += i;

          return *this;
        }

        int sum;
      } accumulator;

      // For each if everything less than 5.
      accumulator = etl::for_each_if(std::begin(data1),
                                     std::end(data1),
                                     accumulator,
                                     std::bind(std::less<int>(), std::placeholders::_1, 5));

      CHECK_EQUAL(10, accumulator.sum);
    }

    //*************************************************************************
    TEST(for_each_n)
    {
      int data1[] = { 1,  8, 2, 7,  3, 6, 4, 5, 10, 9 };
      int data2[] = { 2, 16, 4, 14, 6, 6, 4, 5, 10, 9 };

      struct Multiply
      {
        void operator()(int& i)
        {
          i *= 2;
        }
      } multiplier;

      etl::for_each_n(std::begin(data1), 5, multiplier);

      bool are_equal = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(for_each_n_if)
    {
      int data1[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int data2[] = { 2, 8, 4, 7, 6, 6, 4, 5, 10, 9 };

      struct Multiply
      {
        void operator()(int& i)
        {
          i *= 2;
        }
      } multiplier;

      etl::for_each_n_if(std::begin(data1), 5, multiplier, std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool are_equal = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(transform_4_parameter)
    {
      int input[]   = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare[] = { 2, 16, 4, 14, 6, 0, 0, 0, 0, 0 };

      // Double everything and copy to output.
      etlstd::transform(std::begin(input),
                     std::end(input),
                     std::begin(output),
                     std::begin(output) + (etl::size(output) / 2),
                     std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);

      std::fill(std::begin(output), std::end(output), 0);

      etlstd::transform(std::begin(input),
                     std::begin(input) + (etl::size(input) / 2),
                     std::begin(output),
                     std::end(output),
                     std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_random_iterator)
    {
      int input[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare[] = { 2, 16, 4, 14, 6, 12, 8, 0, 0, 0 };

      etl::transform_n(std::begin(input),
                       7,
                       std::begin(output),
                       std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_non_random_iterator)
    {
      std::list<int> input = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare[] = { 2, 16, 4, 14, 6, 12, 8, 0, 0, 0 };

      etl::transform_n(std::begin(input),
                       7,
                       std::begin(output),
                       std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_if)
    {
      int input[]   = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[]  = { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0 };
      int compare[] = { 2, 4, 6, 8, 0, 0, 0, 0,  0, 0 };

      // Double everything less than 5 and copy to output.
      etl::transform_if(std::begin(input),
                        std::end(input),
                        std::begin(output),
                        std::bind(std::multiplies<int>(), std::placeholders::_1, 2),
                        std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_if_2_input_ranges)
    {
      int input1[] = { 1, 8, 2, 7, 3,  6, 4, 5, 10, 9 };
      int input2[] = { 8, 7, 6, 5, 4, 10, 9, 3,  2, 1 };
      int output[]  = { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0 };
      int compare[] = { 8, 12, 12, 60, 36, 0, 0, 0,  0, 0 };

      // Multiply together everything where input1 is less than input2 and copy to output.
      etl::transform_if(std::begin(input1),
                        std::end(input1),
                        std::begin(input2),
                        std::begin(output),
                        std::multiplies<int>(),
                        std::less<int>());

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_if)
    {
      int input[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0 };
      int compare[] = { 2, 4, 6, 0, 0, 0, 0, 0,  0, 0 };

      // Double everything less than 5 and copy to output.
      etl::transform_n_if(std::begin(input),
                          5,
                          std::begin(output),
                          std::bind(std::multiplies<int>(), std::placeholders::_1, 2),
                          std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_if_2_input_ranges)
    {
      int input1[] = { 1, 8, 2, 7, 3,  6, 4, 5, 10, 9 };
      int input2[] = { 8, 7, 6, 5, 4, 10, 9, 3,  2, 1 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0 };
      int compare[] = { 8, 12, 12, 0, 0, 0, 0, 0,  0, 0 };

      // Multiply together everything where input1 is less than input2 and copy to output.
      etl::transform_n_if(std::begin(input1),
                          std::begin(input2),
                          5,
                          std::begin(output),
                          std::multiplies<int>(),
                          std::less<int>());

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(partition_transform)
    {
      int input[]         = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output_true[]   = { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0 };
      int output_false[]  = { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0 };
      int compare_true[]  = { 2, 4, 6, 8, 0, 0, 0, 0,  0, 0 };
      int compare_false[] = { -16, -14, -12, -10, -20, -18, 0, 0, 0, 0 };

      // Multiply everything less than 5 by 2 and copy to output_true.
      // Multiply everything not less than 5 by -2 and copy to output_false.
      etl::partition_transform(std::begin(input),
                               std::end(input),
                               std::begin(output_true),
                               std::begin(output_false),
                               std::bind(std::multiplies<int>(), std::placeholders::_1, 2),
                               std::bind(std::multiplies<int>(), std::placeholders::_1, -2),
                               std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool is_same = std::equal(std::begin(output_true), std::end(output_true), std::begin(compare_true));
      CHECK(is_same);

      is_same = std::equal(std::begin(output_false), std::end(output_false), std::begin(compare_false));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(partition_transform_2_input_ranges)
    {
      int input1[] = { 1, 8, 2, 7, 3,  6, 4, 5, 10, 9 };
      int input2[] = { 8, 7, 6, 5, 4, 10, 9, 3,  2, 1 };
      int output_true[]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int output_false[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare_true[]  = { 8, 12, 12, 60, 36, 0, 0, 0, 0, 0 };
      int compare_false[] = { 15, 12, 8, 12, 10, 0, 0, 0, 0, 0 };

      // If input1 < input2 multiply else add.
      etl::partition_transform(std::begin(input1),
                               std::end(input1),
                               std::begin(input2),
                               std::begin(output_true),
                               std::begin(output_false),
                               std::multiplies<int>(),
                               std::plus<int>(),
                               std::less<int>());

      bool is_same = std::equal(std::begin(output_true), std::end(output_true), std::begin(compare_true));
      CHECK(is_same);

      is_same = std::equal(std::begin(output_false), std::end(output_false), std::begin(compare_false));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(sort_default)
    {
      std::vector<int> data(100, 0);
      std::iota(data.begin(), data.end(), 1);

      for (int i = 0; i < 100; ++i)
      {
        std::shuffle(data.begin(), data.end(), urng);

        std::vector<int> data1 = data;
        std::vector<int> data2 = data;

        std::sort(data1.begin(), data1.end());
        etl::sort(data2.begin(), data2.end());

        bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(is_same);
      }
    }

    //*************************************************************************
    TEST(sort_greater)
    {
      std::vector<int> data(100, 0);
      std::iota(data.begin(), data.end(), 1);

      for (int i = 0; i < 100; ++i)
      {
        std::shuffle(data.begin(), data.end(), urng);

        std::vector<int> data1 = data;
        std::vector<int> data2 = data;

        std::sort(data1.begin(), data1.end(), std::greater<int>());
        etl::sort(data2.begin(), data2.end(), std::greater<int>());

        bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(is_same);
      }
    }

    //*************************************************************************
    TEST(stable_sort_default)
    {
      std::vector<NDC> initial_data = { NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1) };

      std::vector<NDC> data1(initial_data);
      std::vector<NDC> data2(initial_data);

      std::stable_sort(data1.begin(), data1.end());
      etl::stable_sort(data2.begin(), data2.end());

      bool is_same = std::equal(data1.begin(), data1.end(), data2.begin(), NDC::are_identical);
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(stable_sort_greater)
    {
      std::vector<NDC> initial_data = { NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1) };

      std::vector<NDC> data1(initial_data);
      std::vector<NDC> data2(initial_data);

      std::stable_sort(data1.begin(), data1.end(), std::greater<NDC>());
      etl::stable_sort(data2.begin(), data2.end(), std::greater<NDC>());

      bool is_same = std::equal(data1.begin(), data1.end(), data2.begin(), NDC::are_identical);
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(shell_sort_default)
    {
      std::vector<int> data(100, 0);
      std::iota(data.begin(), data.end(), 1);

      for (int i = 0; i < 100; ++i)
      {
        std::shuffle(data.begin(), data.end(), urng);

        std::vector<int> data1 = data;
        std::vector<int> data2 = data;

        std::sort(data1.begin(), data1.end());
        etl::shell_sort(data2.begin(), data2.end());

        bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(is_same);
      }
    }

    //*************************************************************************
    TEST(shell_sort_greater)
    {
      std::vector<int> data(100, 0);
      std::iota(data.begin(), data.end(), 1);

      for (int i = 0; i < 100; ++i)
      {
        std::shuffle(data.begin(), data.end(), urng);

        std::vector<int> data1 = data;
        std::vector<int> data2 = data;

        std::sort(data1.begin(), data1.end(), std::greater<int>());
        etl::shell_sort(data2.begin(), data2.end(), std::greater<int>());

        bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(is_same);
      }
    }

    //*************************************************************************
    TEST(insertion_sort_default)
    {
      std::vector<NDC> initial_data = { NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1) };

      std::vector<NDC> data1(initial_data);
      std::vector<NDC> data2(initial_data);

      std::stable_sort(data1.begin(), data1.end());
      etl::insertion_sort(data2.begin(), data2.end());

      bool is_same = std::equal(data1.begin(), data1.end(), data2.begin(), NDC::are_identical);
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(insertion_sort_greater)
    {
      std::vector<NDC> initial_data = { NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1) };

      std::vector<NDC> data1(initial_data);
      std::vector<NDC> data2(initial_data);

      std::stable_sort(data1.begin(), data1.end(), std::greater<NDC>());
      etl::insertion_sort(data2.begin(), data2.end(), std::greater<NDC>());

      bool is_same = std::equal(data1.begin(), data1.end(), data2.begin(), NDC::are_identical);
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(multimax)
    {
      CHECK_EQUAL(8, etl::multimax(1, 2, 3, 4, 5, 6, 7, 8));
      CHECK_EQUAL(8, etl::multimax_compare(std::less<int>(), 1, 2, 3, 4, 5, 6, 7, 8));
      CHECK_EQUAL(1, etl::multimax_compare(std::greater<int>(), 1, 2, 3, 4, 5, 6, 7, 8));
    }

    //*************************************************************************
    TEST(multimax_iter)
    {
      int i[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

      CHECK_EQUAL(8, *etl::multimax_iter(&i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
      CHECK_EQUAL(8, *etl::multimax_iter_compare(std::less<int>(), &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
      CHECK_EQUAL(1, *etl::multimax_iter_compare(std::greater<int>(), &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
    }

    //*************************************************************************
    TEST(multimin)
    {
      CHECK_EQUAL(1, etl::multimin(1, 2, 3, 4, 5, 6, 7, 8));
      CHECK_EQUAL(1, etl::multimin_compare(std::less<int>(), 1, 2, 3, 4, 5, 6, 7, 8));
      CHECK_EQUAL(8, etl::multimin_compare(std::greater<int>(), 1, 2, 3, 4, 5, 6, 7, 8));
    }

    //*************************************************************************
    TEST(multimin_iter)
    {
      int i[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

      CHECK_EQUAL(1, *etl::multimin_iter(&i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
      CHECK_EQUAL(1, *etl::multimin_iter_compare(std::less<int>(), &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
      CHECK_EQUAL(8, *etl::multimin_iter_compare(std::greater<int>(), &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
    }
  };
}
