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

#include "algorithm.h"
#include "container.h"

#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <numeric>

namespace
{
  typedef std::vector<int> Data;
  Data data = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

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
    //=========================================================================
    TEST(minmax_element)
    {
      std::pair<Data::iterator, Data::iterator> expected = std::minmax_element(data.begin(), data.end());
      std::pair<Data::iterator, Data::iterator> result   = etl::minmax_element(data.begin(), data.end());
      CHECK_EQUAL(std::distance(data.begin(), expected.first), std::distance(data.begin(), result.first));
      CHECK_EQUAL(std::distance(data.begin(), expected.second), std::distance(data.begin(), result.second));
    }

    //=========================================================================
    TEST(minmax_element_compare)
    {
      std::pair<Data::iterator, Data::iterator> expected = std::minmax_element(data.begin(), data.end(), std::greater<int>());
      std::pair<Data::iterator, Data::iterator> result   = etl::minmax_element(data.begin(), data.end(), std::greater<int>());
      CHECK_EQUAL(std::distance(data.begin(), expected.first),  std::distance(data.begin(), result.first));
      CHECK_EQUAL(std::distance(data.begin(), expected.second), std::distance(data.begin(), result.second));
    }

    //=========================================================================
    TEST(minmax)
    {
      int a = 1;
      int b = 2;

      std::pair<int, int> expected = std::minmax(a, b);
      std::pair<int, int> result   = etl::minmax(a, b);
      CHECK_EQUAL(expected.first,  result.first);
      CHECK_EQUAL(expected.second, result.second);

      result   = etl::minmax(b, a);
      expected = std::minmax(b, a);
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);
    }

    //=========================================================================
    TEST(minmax_compare)
    {
      int a = 1;
      int b = 2;

      std::pair<int, int> expected = std::minmax(a, b, std::greater<int>());
      std::pair<int, int> result   = etl::minmax(a, b, std::greater<int>());
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);

      result   = etl::minmax(b, a, std::greater<int>());
      expected = std::minmax(b, a, std::greater<int>());
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);
    }

    //=========================================================================
    TEST(is_sorted_until)
    {
      int data[] = { 1, 2, 3, 4, 6, 5, 7, 8, 9, 10 };

      int* p1 = std::is_sorted_until(std::begin(data), std::end(data));
      int* p2 = etl::is_sorted_until(std::begin(data), std::end(data));
      CHECK_EQUAL(std::distance(std::begin(data), p1), std::distance(std::begin(data), p2));
    }

    //=========================================================================
    TEST(is_sorted_until_compare)
    {
      int data[] = { 10, 9, 8, 7, 5, 6, 4, 3, 4, 2, 1 };

      int* p1 = std::is_sorted_until(std::begin(data), std::end(data), std::greater<int>());
      int* p2 = etl::is_sorted_until(std::begin(data), std::end(data), std::greater<int>());
      CHECK_EQUAL(std::distance(etl::begin(data), p1), std::distance(std::begin(data), p2));
    }

    //=========================================================================
    TEST(is_sorted)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      bool is_sorted = etl::is_sorted(std::begin(data1), std::end(data1));
      CHECK(is_sorted);

      int data2[] = { 1, 2, 3, 4, 6, 5, 7, 8 , 9, 10};

      is_sorted = etl::is_sorted(std::begin(data2), std::end(data2));
      CHECK(!is_sorted);
    }

    //=========================================================================
    TEST(is_sorted_compare)
    {
      int data1[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      bool is_sorted = etl::is_sorted(std::begin(data1), std::end(data1), std::greater<int>());
      CHECK(is_sorted);

      int data2[] = { 10, 9, 8, 7, 5, 6, 4, 3, 2, 1 };

      is_sorted = etl::is_sorted(std::begin(data2), std::end(data2), std::greater<int>());
      CHECK(!is_sorted);
    }

    //=========================================================================
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

    //=========================================================================
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

    //=========================================================================
    TEST(copy_n_random_iterator)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      int* result;

      std::copy_n(std::begin(data1), 4, std::begin(data2));
      result = etl::copy_n(std::begin(data1), 4, std::begin(data3));

      CHECK_EQUAL(std::begin(data3) + 4, result);

      bool is_same = std::equal(std::begin(data2), std::end(data2), std::begin(data3));
      CHECK(is_same);
    }

    //=========================================================================
    TEST(copy_n_non_random_iterator)
    {
      std::list<int> data1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      int* result;

      std::copy_n(std::begin(data1), 4, std::begin(data2));
      result = etl::copy_n(std::begin(data1), 4, std::begin(data3));

      CHECK_EQUAL(std::begin(data3) + 4, result);

      bool is_same = std::equal(std::begin(data2), std::end(data2), std::begin(data3));
      CHECK(is_same);
    }

    //=========================================================================
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

    //=========================================================================
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

    //=========================================================================
    TEST(copy_if)
    {
      int data1[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      // Copy everything less than 5.
      std::copy_if(std::begin(data1), std::end(data1), std::begin(data2), std::bind2nd(std::less<int>(), 5));
      etl::copy_if(std::begin(data1), std::end(data1), std::begin(data3), std::bind2nd(std::less<int>(), 5));

      bool is_same = std::equal(std::begin(data2), std::end(data2), std::begin(data3));
      CHECK(is_same);
    }

    //=========================================================================
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
      etl::copy_n_if(std::begin(data1), 6, std::begin(data3), std::bind2nd(std::less<int>(), 5));

      bool is_same = std::equal(std::begin(data2), std::end(data2), std::begin(data3));
      CHECK(is_same);
    }

    //=========================================================================
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
      result = etl::copy_if(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1), std::bind2nd(std::less<int>(), 5));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_if(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2), std::bind2nd(std::less<int>(), 5));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Destination larger.
      std::fill(std::begin(out3), std::end(out3), 0);
      result = etl::copy_if(std::begin(data1), std::end(data1), std::begin(out3), std::end(out3), std::bind2nd(std::less<int>(), 5));
      CHECK_EQUAL(std::begin(out3) + 4, result);
      is_same = std::equal(std::begin(out3), std::end(out3), std::begin(check3));
      CHECK(is_same);
    }

    //=========================================================================
    TEST(any_of)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      bool expected = std::any_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      bool result = etl::any_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      CHECK_EQUAL(expected, result);

      expected = std::any_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 0));
      result = etl::any_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 0));
      CHECK_EQUAL(expected, result);
    }

    //=========================================================================
    TEST(all_of)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      bool expected = std::all_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 0));
      bool result = etl::all_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 0));
      CHECK_EQUAL(expected, result);

      expected = std::all_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      result = etl::all_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      CHECK_EQUAL(expected, result);
    }

    //=========================================================================
    TEST(none_of)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      bool expected = std::none_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 8));
      bool result = etl::none_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 8));
      CHECK_EQUAL(expected, result);

      expected = std::none_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      result = etl::none_of(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      CHECK_EQUAL(expected, result);
    }

    struct Compare : public std::binary_function < int, int, bool >
    {
      bool operator()(int a, int b) const
      {
        return a == b;
      }
    };

    //=========================================================================
    TEST(is_permutation)
    {
      int data1[]           = { 1, 2, 3, 4, 5, 6, 7, 8 };
      int permutation[]     = { 1, 3, 2, 4, 7, 6, 5, 8 };
      int not_permutation[] = { 1, 2, 3, 4, 5, 6, 7, 7 };

      bool is_permutation   = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation));
      CHECK(is_permutation);

      is_permutation   = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation));
      CHECK(!is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation), std::equal_to<int>());
      CHECK(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), std::equal_to<int>());
      CHECK(!is_permutation);

      is_permutation   = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation), std::end(permutation));
      CHECK(is_permutation);

      is_permutation   = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), std::end(not_permutation));
      CHECK(!is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation), std::end(permutation), std::equal_to<int>());
      CHECK(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), std::end(not_permutation), std::equal_to<int>());
      CHECK(!is_permutation);
    }

    //=========================================================================
    TEST(is_partitioned)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

      bool expected = std::is_partitioned(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      bool result = etl::is_partitioned(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      CHECK_EQUAL(expected, result);

      std::partition(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));

      expected = std::is_partitioned(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      result = etl::is_partitioned(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      CHECK_EQUAL(expected, result);
    }

    //=========================================================================
    TEST(partition_point)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

      std::partition(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));

      int* partition1 = std::partition_point(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      int* partition2 = etl::partition_point(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 4));
      CHECK_EQUAL(std::distance(std::begin(data1), partition1), std::distance(std::begin(data1), partition2));

      std::partition(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 8));

      partition1 = std::partition_point(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 0));
      partition2 = etl::partition_point(std::begin(data1), std::end(data1), std::bind2nd(std::greater<int>(), 0));
      CHECK_EQUAL(std::distance(std::begin(data1), partition1), std::distance(std::begin(data1), partition2));
    }

    //=========================================================================
    TEST(partition_copy)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      int data4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      int data5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

      std::partition_copy(std::begin(data1), std::end(data1), std::begin(data2),  std::begin(data3), std::bind2nd(std::greater<int>(), 4));
      etl::partition_copy(std::begin(data1), std::end(data1), std::begin(data4),  std::begin(data5), std::bind2nd(std::greater<int>(), 4));

      bool are_equal;

      are_equal = std::equal(std::begin(data2), std::end(data2), std::begin(data4));
      CHECK(are_equal);

      are_equal = std::equal(std::begin(data3), std::end(data3), std::begin(data5));
      CHECK(are_equal);
    }

    //=========================================================================
    TEST(find_if_not)
    {
      int data1[] = { 1, 2, 3, 5, 6, 7, 8 };

      // Find the element not less than 4.
      int* p = etl::find_if_not(std::begin(data1), std::end(data1), std::bind2nd(std::less<int>(), 4));
      CHECK_EQUAL(5, *p);
    }

    //=========================================================================
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

    //=========================================================================
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

    //=========================================================================
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
                                     std::bind2nd(std::less<int>(), 5));

      CHECK_EQUAL(10, accumulator.sum);
    }

    //=========================================================================
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

    //=========================================================================
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

      etl::for_each_n_if(std::begin(data1), 5, multiplier, std::bind2nd(std::less<int>(), 5));

      bool are_equal = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(are_equal);
    }

    //=========================================================================
    TEST(transform_4_parameter)
    {
      int input[]   = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare[] = { 2, 16, 4, 14, 6, 0, 0, 0, 0, 0 };

      // Double everything and copy to output.
      etl::transform(std::begin(input),
                     std::end(input),
                     std::begin(output),
                     std::begin(output) + (etl::size(output) / 2),
                     std::bind2nd(std::multiplies<int>(), 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);

      std::fill(std::begin(output), std::end(output), 0);

      etl::transform(std::begin(input),
                     std::begin(input) + (etl::size(input) / 2),
                     std::begin(output),
                     std::end(output),
                     std::bind2nd(std::multiplies<int>(), 2));

      is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //=========================================================================
    TEST(transform_n_random_iterator)
    {
      int input[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare[] = { 2, 16, 4, 14, 6, 12, 8, 0, 0, 0 };

      etl::transform_n(std::begin(input),
                       7,
                       std::begin(output),
                       std::bind2nd(std::multiplies<int>(), 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //=========================================================================
    TEST(transform_n_non_random_iterator)
    {
      std::list<int> input = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare[] = { 2, 16, 4, 14, 6, 12, 8, 0, 0, 0 };

      etl::transform_n(std::begin(input),
                       7,
                       std::begin(output),
                       std::bind2nd(std::multiplies<int>(), 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //=========================================================================
    TEST(transform_if)
    {
      int input[]   = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[]  = { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0 };
      int compare[] = { 2, 4, 6, 8, 0, 0, 0, 0,  0, 0 };

      // Double everything less than 5 and copy to output.
      etl::transform_if(std::begin(input),
                        std::end(input),
                        std::begin(output),
                        std::bind2nd(std::multiplies<int>(), 2),
                        std::bind2nd(std::less<int>(), 5));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //=========================================================================
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

    //=========================================================================
    TEST(transform_n_if)
    {
      int input[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0,  0, 0 };
      int compare[] = { 2, 4, 6, 0, 0, 0, 0, 0,  0, 0 };

      // Double everything less than 5 and copy to output.
      etl::transform_n_if(std::begin(input),
                          5,
                          std::begin(output),
                          std::bind2nd(std::multiplies<int>(), 2),
                          std::bind2nd(std::less<int>(), 5));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //=========================================================================
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

    //=========================================================================
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
                               std::bind2nd(std::multiplies<int>(), 2),
                               std::bind2nd(std::multiplies<int>(), -2),
                               std::bind2nd(std::less<int>(), 5));

      bool is_same = std::equal(std::begin(output_true), std::end(output_true), std::begin(compare_true));
      CHECK(is_same);

      is_same = std::equal(std::begin(output_false), std::end(output_false), std::begin(compare_false));
      CHECK(is_same);
    }

    //=========================================================================
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
  };
}
