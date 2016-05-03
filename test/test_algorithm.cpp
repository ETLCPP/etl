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

#include <UnitTest++/UnitTest++.h>

#include "../src/algorithm.h"
#include "../src/container.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>

namespace
{
  typedef std::vector<int> Data;
  Data data = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

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
    TEST(copy_n)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      std::copy_n(std::begin(data1), 4, std::begin(data2));
      etl::copy_n(std::begin(data1), 4, std::begin(data3));

      bool is_same = std::equal(std::begin(data2), std::end(data2), std::begin(data3));
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
      int* p = std::find_if_not(std::begin(data1), std::end(data1), std::bind2nd(std::less<int>(), 4));
      CHECK_EQUAL(5, *p);
    }
  };
}
