/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/algorithm.h"
#include "etl/container.h"
#include "etl/binary.h"

#include "data.h"
#include "iterators_for_unit_tests.h"

#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <algorithm>
#include <functional>
#include <numeric>
#include <random>
#include <memory>

namespace
{
  using NDC = TestDataNDC<int>;
  using ItemM = TestDataM<std::string>;
  std::random_device rng;
  std::mt19937 urng(rng());

  using Vector = std::vector<int>;
  Vector data = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  using VectorM = std::vector<ItemM>;

  constexpr size_t SIZE = 10;

  int dataA[SIZE] = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  int dataS[SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  std::list<int> dataSL = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  using List = std::list<int>;
  List dataL = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  int dataEQ[SIZE] = { 1, 1, 3, 3, 5, 5, 7, 7, 9, 9 };
  std::list<int> dataEQL = { 1, 1, 3, 3, 5, 5, 7, 7, 9, 9 };

  Vector dataV = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  int dataD1[SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int dataD2[SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  class Data
  {
  public:

    Data()
      : a(0)
      , b(0)
    {
    }

    Data(int a_, int b_)
      : a(a_)
      , b(b_)
    {
    }

    int a;
    int b;
  };

  bool operator ==(const Data& lhs, const Data& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b);
  }

  struct DataPredicate
  {
    bool operator ()(const Data& lhs, const Data& rhs) const
    {
      return lhs.a < rhs.a;
    }
  };

  struct DataEquality
  {
    bool operator ()(const Data& lhs, const Data& rhs) const
    {
      return lhs.a == rhs.a;
    }
  };

  Data dataD[10] = { Data(1, 2), Data(2, 1), Data(3, 4), Data(4, 3), Data(5, 6), Data(6, 5), Data(7, 8), Data(8, 7), Data(9, 10), Data(10, 9) };

  struct Greater : public etl::binary_function<int, int, bool>
  {
    bool operator()(int a, int b) const
    {
      return a > b;
    }
  };

  std::ostream& operator << (std::ostream& os, const Data& data_)
  {
    os << data_.a << "," << data_.b;
    return os;
  }

  SUITE(test_algorithm)
  {
    //*************************************************************************
    TEST(min)
    {
      int a = 1;
      int b = 2;

      CHECK_EQUAL((std::min(a, b)), (etl::min(a, b)));
      CHECK_EQUAL((std::min(b, a)), (etl::min(b, a)));
    }

    //*************************************************************************
    TEST(min_compare)
    {
      int a = 1;
      int b = 2;

      CHECK_EQUAL((std::min(a, b, Greater())), (etl::min(a, b, Greater())));
      CHECK_EQUAL((std::min(b, a, Greater())), (etl::min(b, a, Greater())));
    }

    //*************************************************************************
    TEST(max)
    {
      int a = 1;
      int b = 2;

      CHECK_EQUAL((std::max(a, b)), (etl::max(a, b)));
      CHECK_EQUAL((std::max(b, a)), (etl::max(b, a)));
    }

    //*************************************************************************
    TEST(max_compare)
    {
      int a = 1;
      int b = 2;

      CHECK_EQUAL((std::max(a, b, Greater())), (etl::max(a, b, Greater())));
      CHECK_EQUAL((std::max(b, a, Greater())), (etl::max(b, a, Greater())));
    }

    //*************************************************************************
    TEST(min_element)
    {
      Vector::iterator expected = std::min_element(data.begin(), data.end());
      Vector::iterator result   = etl::min_element(data.begin(), data.end());
      CHECK_EQUAL(std::distance(data.begin(), expected), std::distance(data.begin(), result));
    }

    //*************************************************************************
    TEST(min_element_compare)
    {
      Vector::iterator expected = std::min_element(data.begin(), data.end(), std::greater<int>());
      Vector::iterator result = etl::min_element(data.begin(), data.end(), std::greater<int>());
      CHECK_EQUAL(std::distance(data.begin(), expected), std::distance(data.begin(), result));
    }

    //*************************************************************************
    TEST(min_element_empty)
    {
      Vector dataEmpty;
      Vector::iterator expected = std::min_element(dataEmpty.begin(), dataEmpty.end());
      Vector::iterator result = etl::min_element(dataEmpty.begin(), dataEmpty.end());
      CHECK_EQUAL(std::distance(dataEmpty.end(), expected), std::distance(dataEmpty.end(), result));
    }

    //*************************************************************************
    TEST(max_element)
    {
      Vector::iterator expected = std::max_element(data.begin(), data.end());
      Vector::iterator result = etl::max_element(data.begin(), data.end());
      CHECK_EQUAL(std::distance(data.begin(), expected), std::distance(data.begin(), result));
    }

    //*************************************************************************
    TEST(max_element_compare)
    {
      Vector::iterator expected = std::max_element(data.begin(), data.end(), std::greater<int>());
      Vector::iterator result = etl::max_element(data.begin(), data.end(), std::greater<int>());
      CHECK_EQUAL(std::distance(data.begin(), expected), std::distance(data.begin(), result));
    }

    //*************************************************************************
    TEST(max_element_empty)
    {
      Vector dataEmpty;
      Vector::iterator expected = std::max_element(dataEmpty.begin(), dataEmpty.end());
      Vector::iterator result = etl::max_element(dataEmpty.begin(), dataEmpty.end());
      CHECK_EQUAL(std::distance(dataEmpty.end(), expected), std::distance(dataEmpty.end(), result));
    }

    //*************************************************************************
    TEST(minmax_element)
    {
      std::pair<Vector::iterator, Vector::iterator> expected = std::minmax_element(data.begin(), data.end());
      std::pair<Vector::iterator, Vector::iterator> result = etl::minmax_element(data.begin(), data.end());
      CHECK_EQUAL(std::distance(data.begin(), expected.first), std::distance(data.begin(), result.first));
      CHECK_EQUAL(std::distance(data.begin(), expected.second), std::distance(data.begin(), result.second));
    }

    //*************************************************************************
    TEST(minmax_element_compare)
    {
      std::pair<Vector::iterator, Vector::iterator> expected = std::minmax_element(data.begin(), data.end(), std::greater<int>());
      std::pair<Vector::iterator, Vector::iterator> result = etl::minmax_element(data.begin(), data.end(), std::greater<int>());
      CHECK_EQUAL(std::distance(data.begin(), expected.first), std::distance(data.begin(), result.first));
      CHECK_EQUAL(std::distance(data.begin(), expected.second), std::distance(data.begin(), result.second));
    }

    //*************************************************************************
    TEST(minmax_element_empty)
    {
      Vector dataEmpty;
      std::pair<Vector::iterator, Vector::iterator> expected = std::minmax_element(dataEmpty.begin(), dataEmpty.end());
      std::pair<Vector::iterator, Vector::iterator> result = etl::minmax_element(dataEmpty.begin(), dataEmpty.end());
      CHECK_EQUAL(std::distance(dataEmpty.begin(), expected.first), std::distance(dataEmpty.begin(), result.first));
      CHECK_EQUAL(std::distance(dataEmpty.begin(), expected.second), std::distance(dataEmpty.begin(), result.second));
    }

    //*************************************************************************
    TEST(minmax)
    {
      int a = 1;
      int b = 2;

      std::pair<int, int> expected = std::minmax(a, b);
      std::pair<int, int> result = etl::minmax(a, b);
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);

      result = etl::minmax(b, a);
      expected = std::minmax(b, a);
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);
    }

    //*************************************************************************
    TEST(minmax_compare)
    {
      int a = 1;
      int b = 2;

      std::pair<int, int> expected = std::minmax(a, b, std::greater<int>());
      std::pair<int, int> result = etl::minmax(a, b, std::greater<int>());
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);

      result = etl::minmax(b, a, std::greater<int>());
      expected = std::minmax(b, a, std::greater<int>());
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);
    }

    //*************************************************************************
    TEST(is_sorted_until)
    {
      int data[] = { 1, 2, 3, 4, 6, 5, 7, 8, 9, 10 };

      int* p1 = std::is_sorted_until(std::begin(data), std::end(data));
      int* p2 = etl::is_sorted_until(std::begin(data), std::end(data));
      CHECK_EQUAL(std::distance(std::begin(data), p1), std::distance(std::begin(data), p2));
    }

    //*************************************************************************
    TEST(is_sorted_until_compare)
    {
      int data[] = { 10, 9, 8, 7, 5, 6, 4, 3, 4, 2, 1 };

      int* p1 = std::is_sorted_until(std::begin(data), std::end(data), std::greater<int>());
      int* p2 = etl::is_sorted_until(std::begin(data), std::end(data), std::greater<int>());
      CHECK_EQUAL(std::distance(std::begin(data), p1), std::distance(std::begin(data), p2));
    }

    //*************************************************************************
    TEST(is_sorted)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      bool is_sorted = etl::is_sorted(std::begin(data1), std::end(data1));
      CHECK(is_sorted);

      int data2[] = { 1, 2, 3, 4, 6, 5, 7, 8 , 9, 10 };

      is_sorted = etl::is_sorted(std::begin(data2), std::end(data2));
      CHECK(!is_sorted);
    }

    //*************************************************************************
    TEST(is_sorted_compare)
    {
      int data1[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      bool is_sorted = etl::is_sorted(std::begin(data1), std::end(data1), std::greater<int>());
      CHECK(is_sorted);

      int data2[] = { 10, 9, 8, 7, 5, 6, 4, 3, 2, 1 };

      is_sorted = etl::is_sorted(std::begin(data2), std::end(data2), std::greater<int>());
      CHECK(!is_sorted);
    }

    //*************************************************************************
    TEST(copy_pod_pointer)
    {
      int data1[10];
      int data2[10];

      int* pstl = std::copy(std::begin(dataA), std::end(dataA), std::begin(data1));
      int* petl = etl::copy(std::begin(dataA), std::end(dataA), std::begin(data2));

      using difference_type_t = std::iterator_traits<int*>::difference_type;

      difference_type_t dstl = std::distance(data1, pstl);
      difference_type_t detl = std::distance(data2, petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_non_pod_pointer)
    {
      Data data1[10];
      Data data2[10];

      Data* pstl = std::copy(std::begin(dataD), std::end(dataD), std::begin(data1));
      Data* petl = etl::copy(std::begin(dataD), std::end(dataD), std::begin(data2));

      using difference_type_t = std::iterator_traits<Data*>::difference_type;

      difference_type_t dstl = std::distance(data1, pstl);
      difference_type_t detl = std::distance(data2, petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_non_random_iterator)
    {
      List data1(dataL.size());
      List data2(dataL.size());

      List::iterator pstl = std::copy(std::begin(dataA), std::end(dataA), std::begin(data1));
      List::iterator petl = etl::copy(std::begin(dataA), std::end(dataA), std::begin(data2));

      using difference_type_t = List::difference_type;

      difference_type_t dstl = std::distance(data1.begin(), pstl);
      difference_type_t detl = std::distance(data2.begin(), petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
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

    //*************************************************************************
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

    //*************************************************************************
    TEST(copy_if)
    {
      int data1[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

      // Copy everything less than 5.
      std::copy_if(std::begin(data1), std::end(data1), std::begin(data2), std::bind(std::less<int>(), std::placeholders::_1, 5));
      etl::copy_if(std::begin(data1), std::end(data1), std::begin(data3), std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool is_same = std::equal(std::begin(data2), std::end(data2), std::begin(data3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(reverse_copy_pod_pointer)
    {
      int data1[10];
      int data2[10];

      int* pstl = std::reverse_copy(std::begin(dataA), std::end(dataA), std::begin(data1));
      int* petl = etl::reverse_copy(std::begin(dataA), std::end(dataA), std::begin(data2));

      using difference_type_t = std::iterator_traits<int*>::difference_type;

      difference_type_t dstl = std::distance(data1, pstl);
      difference_type_t detl = std::distance(data2, petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_n_pod_pointer)
    {
      int data1[10];
      int data2[10];

      int* pstl = std::copy_n(std::begin(dataA), 10, std::begin(data1));
      int* petl = etl::copy_n(std::begin(dataA), 10, std::begin(data2));

      using difference_type_t = std::iterator_traits<int*>::difference_type;

      difference_type_t dstl = std::distance(data1, pstl);
      difference_type_t detl = std::distance(data2, petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_n_non_pod_pointer)
    {
      Data data1[10];
      Data data2[10];

      Data* pstl = std::copy_n(std::begin(dataD), 10, std::begin(data1));
      Data* petl = etl::copy_n(std::begin(dataD), 10, std::begin(data2));

      using difference_type_t = std::iterator_traits<Data*>::difference_type;

      difference_type_t dstl = std::distance(data1, pstl);
      difference_type_t detl = std::distance(data2, petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_backward_pod_pointer)
    {
      int data1[10];
      int data2[10];

      int* pstl = std::copy_backward(std::begin(dataA), std::end(dataA), std::end(data1));
      int* petl = etl::copy_backward(std::begin(dataA), std::end(dataA), std::end(data2));

      using difference_type_t = std::iterator_traits<int*>::difference_type;

      difference_type_t dstl = std::distance(data1, pstl);
      difference_type_t detl = std::distance(data2, petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_backward_non_pod_pointer)
    {
      Data data1[10];
      Data data2[10];

      Data* pstl = std::copy_backward(std::begin(dataD), std::end(dataD), std::end(data1));
      Data* petl = etl::copy_backward(std::begin(dataD), std::end(dataD), std::end(data2));

      using difference_type_t = std::iterator_traits<Data*>::difference_type;

      difference_type_t dstl = std::distance(data1, pstl);
      difference_type_t detl = std::distance(data2, petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_backward_non_random_iterator)
    {
      List data1(dataL.size());
      List data2(dataL.size());

      List::iterator pstl = copy_backward(std::begin(dataA), std::end(dataA), std::end(data1));
      List::iterator petl = etl::copy_backward(std::begin(dataA), std::end(dataA), std::end(data2));

      using difference_type_t = std::iterator_traits<List::iterator>::difference_type;

      difference_type_t dstl = std::distance(data1.begin(), pstl);
      difference_type_t detl = std::distance(data2.begin(), petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_even_non_pointer)
    {
      std::array<int, 10> data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::array<int, 10> data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      std::reverse(data1.begin(), data1.end());
      etl::reverse(data2.begin(), data2.end());

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_odd_non_pointer)
    {
      std::array<int, 9> data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
      std::array<int, 9> data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

      std::reverse(data1.begin(), data1.end());
      etl::reverse(data2.begin(), data2.end());

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_even_pointer)
    {
      int data1[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      int data2[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      std::reverse(std::begin(data1), std::end(data1));
      etl::reverse(std::begin(data2), std::end(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_odd_pointer)
    {
      int data1[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
      int data2[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

      std::reverse(std::begin(data1), std::end(data1));
      etl::reverse(std::begin(data2), std::end(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(lower_bound_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::lower_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etl::lower_bound(random_iterator<int>(std::begin(dataS)), random_iterator<int>(std::end(dataS)), i);

        CHECK_EQUAL(lb1, lb2);
      }
    }

    //*************************************************************************
    TEST(lower_bound_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::lower_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etl::lower_bound(non_random_iterator<int>(std::begin(dataS)), non_random_iterator<int>(std::end(dataS)), i);

        CHECK_EQUAL(std::distance(std::begin(dataS), lb1), std::distance(std::begin(dataS), lb2));
      }
    }

    //*************************************************************************
    TEST(upper_bound_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::upper_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etl::upper_bound(random_iterator<int>(std::begin(dataS)), random_iterator<int>(std::end(dataS)), i);

        CHECK_EQUAL(std::distance(std::begin(dataS), lb1), std::distance(std::begin(dataS), lb2));
      }
    }

    //*************************************************************************
    TEST(upper_bound_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::upper_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etl::upper_bound(non_random_iterator<int>(std::begin(dataS)), non_random_iterator<int>(std::end(dataS)), i);

        CHECK_EQUAL(std::distance(std::begin(dataS), lb1), std::distance(std::begin(dataS), lb2));
      }
    }

    //*************************************************************************
    TEST(equal_range_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        ETL_OR_STD::pair<int*, int*>    lb1 = std::equal_range(std::begin(dataEQ), std::end(dataEQ), i);
        ETL_OR_STD::pair<random_iterator<int>, random_iterator<int>> lb2 = etl::equal_range(random_iterator<int>(std::begin(dataEQ)), random_iterator<int>(std::end(dataEQ)), i);

        CHECK_EQUAL(std::distance(std::begin(dataEQ), lb1.first), std::distance<int*>(std::begin(dataEQ), lb2.first));
        CHECK_EQUAL(std::distance(lb1.first, lb1.second), std::distance<int*>(lb2.first, lb2.second));
      }
    }

    //*************************************************************************
    TEST(equal_range_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        ETL_OR_STD::pair<int*, int*>    lb1 = std::equal_range(std::begin(dataEQ), std::end(dataEQ), i);
        ETL_OR_STD::pair<non_random_iterator<int>, non_random_iterator<int>> lb2 = etl::equal_range(non_random_iterator<int>(std::begin(dataEQ)), non_random_iterator<int>(std::end(dataEQ)), i);

        CHECK_EQUAL(std::distance(std::begin(dataEQ), lb1.first), std::distance<int*>(std::begin(dataEQ), lb2.first));
        CHECK_EQUAL(std::distance(lb1.first, lb1.second), std::distance<int*>(lb2.first, lb2.second));
      }
    }

    //*************************************************************************
    TEST(fill_non_char)
    {
      int data1[10];
      int data2[10];

      std::fill(std::begin(data1), std::end(data1), 0x12345678UL);
      etl::fill(std::begin(data2), std::end(data2), 0x12345678UL);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(fill_char)
    {
      unsigned char data1[10];
      unsigned char data2[10];

      std::fill(std::begin(data1), std::end(data1), char(0x12U));
      etl::fill(std::begin(data2), std::end(data2), char(0x12U));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(iter_swap_same_types)
    {
      int a = 1;
      int b = 2;

      etl::iter_swap(&a, &b);
      CHECK_EQUAL(2, a);
      CHECK_EQUAL(1, b);
    }

    //*************************************************************************
    TEST(iter_swap_differnt_types)
    {
      int  a = 1;
      long b = 2;

      etl::iter_swap(&a, &b);
      CHECK_EQUAL(2, a);
      CHECK_EQUAL(1, b);
    }

    //*************************************************************************
    TEST(equal)
    {
      CHECK(etl::equal(std::begin(dataV), std::end(dataV), std::begin(dataL)));
      CHECK(!etl::equal(std::begin(dataSL), std::end(dataSL), std::begin(dataL)));

      int small[] = { dataS[0] };
      CHECK(etl::equal(std::begin(dataV), std::end(dataV), std::begin(dataL), std::end(dataL)));
      CHECK(!etl::equal(std::begin(dataS), std::end(dataS), std::begin(small), std::end(small)));
    }

    //*************************************************************************
    TEST(lexicographical_compare)
    {
      std::string text1("Hello World");
      std::string text2("Hello Xorld");

      bool t1 = std::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.begin() + 7);
      bool t2 = etl::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.begin() + 7);

      CHECK(t1 == t2);
    }

    //*************************************************************************
    TEST(lexicographical_compare_greater)
    {
      std::string text1("Hello World");
      std::string text2("Hello Xorld");

      bool t1 = std::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.begin() + 7, Greater());
      bool t2 = etl::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.begin() + 7, Greater());

      CHECK(t1 == t2);
    }

    //*************************************************************************
    TEST(search)
    {
      std::string haystack = "ABCDFEGHIJKLMNOPQRSTUVWXYZ";
      std::string needle = "KLMNO";

      std::string::iterator itr1 = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin());
      std::string::iterator itr2 = etl::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin());

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(search_predicate)
    {
      std::string haystack = "ABCDFEGHIJKLMNOPQRSTUVWXYZ";
      std::string needle = "KLMNO";

      std::string::iterator itr1 = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin(), std::equal_to<char>());
      std::string::iterator itr2 = etl::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin(), std::equal_to<char>());

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(heap)
    {
      using Vector = std::vector<std::string>;

      std::string a("A"), b("B"), c("C"), d("D"), e("E"), f("F"), g("G"), h("H"), i("I"), j("J");

      Vector data1 = { a, b, c, d, e, f, g, h, i, j };
      Vector data2 = { a, b, c, d, e, f, g, h, i, j };

      std::make_heap(data1.begin(), data1.end());
      etl::make_heap(data2.begin(), data2.end());

      bool isEqual;

      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      std::pop_heap(data1.begin(), data1.end());
      etl::pop_heap(data2.begin(), data2.end());

      data1.pop_back();
      data2.pop_back();

      CHECK(std::is_heap(data1.begin(), data1.end()));
      CHECK(std::is_heap(data2.begin(), data2.end()));

      CHECK_EQUAL(data1.size(), data2.size());

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      data1.push_back(std::string("K"));
      data2.push_back(std::string("K"));

      std::push_heap(data1.begin(), data1.end());
      etl::push_heap(data2.begin(), data2.end());

      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(heap_movable)
    {
      ItemM a("A"), b("B"), c("C"), d("D"), e("E"), f("F"), g("G"), h("H"), i("I"), j("J");

      VectorM data1;
      data1.emplace_back(std::move(b));
      data1.emplace_back(std::move(a));
      data1.emplace_back(std::move(d));
      data1.emplace_back(std::move(c));
      data1.emplace_back(std::move(f));
      data1.emplace_back(std::move(e));
      data1.emplace_back(std::move(h));
      data1.emplace_back(std::move(g));
      data1.emplace_back(std::move(j));
      data1.emplace_back(std::move(i));

      VectorM data2;
      data2.emplace_back(ItemM("B"));
      data2.emplace_back(ItemM("A"));
      data2.emplace_back(ItemM("D"));
      data2.emplace_back(ItemM("C"));
      data2.emplace_back(ItemM("F"));
      data2.emplace_back(ItemM("E"));
      data2.emplace_back(ItemM("H"));
      data2.emplace_back(ItemM("G"));
      data2.emplace_back(ItemM("J"));
      data2.emplace_back(ItemM("I"));

      std::make_heap(data1.begin(), data1.end());
      etl::make_heap(data2.begin(), data2.end());

      bool isEqual;

      CHECK(std::is_heap(data1.begin(), data1.end()));
      CHECK(std::is_heap(data2.begin(), data2.end()));

      CHECK_EQUAL(data1.size(), data2.size());

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      std::pop_heap(data1.begin(), data1.end());
      etl::pop_heap(data2.begin(), data2.end());

      data1.pop_back();
      data2.pop_back();

      CHECK(std::is_heap(data1.begin(), data1.end()));
      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      data1.push_back(ItemM("K"));
      data2.push_back(ItemM("K"));

      std::push_heap(data1.begin(), data1.end());
      etl::push_heap(data2.begin(), data2.end());

      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(heap_greater)
    {
      Vector data1 = dataV;
      Vector data2 = dataV;

      std::make_heap(data1.begin(), data1.end(), Greater());
      etl::make_heap(data2.begin(), data2.end(), Greater());

      bool isEqual;

      CHECK(std::is_heap(data2.begin(), data2.end(), Greater()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      std::pop_heap(data1.begin(), data1.end(), Greater());
      etl::pop_heap(data2.begin(), data2.end(), Greater());

      data1.pop_back();
      data2.pop_back();

      CHECK(std::is_heap(data1.begin(), data1.end(), Greater()));
      CHECK(std::is_heap(data2.begin(), data2.end(), Greater()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      CHECK(std::is_heap(data2.begin(), data2.end(), Greater()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      data1.push_back(5);
      data2.push_back(5);

      std::push_heap(data1.begin(), data1.end(), Greater());
      etl::push_heap(data2.begin(), data2.end(), Greater());

      CHECK(std::is_heap(data2.begin(), data2.end(), Greater()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(find)
    {
      int* itr1 = std::find(std::begin(dataA), std::end(dataA), 5);
      int* itr2 = etl::find(std::begin(dataA), std::end(dataA), 5);

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(find_if)
    {
      struct predicate
      {
        bool operator()(int i) const
        {
          return (i == 5);
        }
      };

      int* itr1 = std::find_if(std::begin(dataA), std::end(dataA), predicate());
      int* itr2 = etl::find_if(std::begin(dataA), std::end(dataA), predicate());

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(count)
    {
      size_t c1 = std::count(std::begin(dataEQ), std::end(dataEQ), 5);
      size_t c2 = etl::count(std::begin(dataEQ), std::end(dataEQ), 5);

      CHECK(c1 == c2);
    }

    //*************************************************************************
    TEST(count_if)
    {
      struct predicate
      {
        bool operator()(int i) const
        {
          return (i == 5);
        }
      };

      size_t c1 = std::count_if(std::begin(dataEQ), std::end(dataEQ), predicate());
      size_t c2 = etl::count_if(std::begin(dataEQ), std::end(dataEQ), predicate());

      CHECK(c1 == c2);
    }

    //*************************************************************************
    TEST(fill_n)
    {
      (void) std::fill_n(std::begin(dataD1), SIZE, 5);
      int* p2 = etl::fill_n(std::begin(dataD2), SIZE, 5);

      CHECK(p2 == std::end(dataD2));

      bool isEqual = std::equal(std::begin(dataD1), std::end(dataD1), std::begin(dataD2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(transform1)
    {
      struct Function
      {
        int operator()(int d) const
        {
          return d * 2;
        }
      };

      (void) std::transform(std::begin(dataS), std::end(dataS), std::begin(dataD1), Function());
      int* p2 = etl::transform(std::begin(dataS), std::end(dataS), std::begin(dataD2), Function());

      CHECK(p2 == std::end(dataD2));

      bool isEqual = std::equal(std::begin(dataD1), std::end(dataD1), std::begin(dataD2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(move)
    {
      typedef std::vector<std::unique_ptr<unsigned>> Data;

      Data data1;

      // Create some data.
      std::unique_ptr<uint32_t> p1(new uint32_t(1U));
      std::unique_ptr<uint32_t> p2(new uint32_t(2U));
      std::unique_ptr<uint32_t> p3(new uint32_t(3U));
      std::unique_ptr<uint32_t> p4(new uint32_t(4U));
      std::unique_ptr<uint32_t> p5(new uint32_t(5U));

      // Fill data1.
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));
      data1.push_back(std::move(p5));

      Data data2;

      // Move to data2.
      etl::move(data1.begin(), data1.end(), std::back_inserter(data2));

      // Old data now empty.
      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));
      CHECK(!bool(p5));

      CHECK_EQUAL(1U, *(data2[0]));
      CHECK_EQUAL(2U, *(data2[1]));
      CHECK_EQUAL(3U, *(data2[2]));
      CHECK_EQUAL(4U, *(data2[3]));
      CHECK_EQUAL(5U, *(data2[4]));
    }

    //*************************************************************************
    TEST(move_backward)
    {
      typedef std::vector<std::unique_ptr<unsigned>> Data;

      Data data1;

      // Create some data.
      std::unique_ptr<uint32_t> p1(new uint32_t(1U));
      std::unique_ptr<uint32_t> p2(new uint32_t(2U));
      std::unique_ptr<uint32_t> p3(new uint32_t(3U));
      std::unique_ptr<uint32_t> p4(new uint32_t(4U));
      std::unique_ptr<uint32_t> p5(new uint32_t(5U));

      // Fill data1.
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));
      data1.push_back(std::move(p5));

      Data data2;

      // Create some data.
      std::unique_ptr<uint32_t> p6(new uint32_t(6U));
      std::unique_ptr<uint32_t> p7(new uint32_t(7U));
      std::unique_ptr<uint32_t> p8(new uint32_t(8U));
      std::unique_ptr<uint32_t> p9(new uint32_t(9U));
      std::unique_ptr<uint32_t> p10(new uint32_t(10U));

      // Fill data2.
      data2.push_back(std::move(p6));
      data2.push_back(std::move(p7));
      data2.push_back(std::move(p8));
      data2.push_back(std::move(p9));
      data2.push_back(std::move(p10));

      // Overwrite data2 with data1.
      etl::move_backward(data1.begin(), data1.end(), data2.end());

      // Old data now empty.
      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));
      CHECK(!bool(p5));

      CHECK_EQUAL(1U, *(data2[0]));
      CHECK_EQUAL(2U, *(data2[1]));
      CHECK_EQUAL(3U, *(data2[2]));
      CHECK_EQUAL(4U, *(data2[3]));
      CHECK_EQUAL(5U, *(data2[4]));
    }

    //*************************************************************************
    TEST(rotate_pod)
    {
      std::vector<int> initial_data = { 1, 2, 3, 4, 5, 6, 7 };

      for (size_t i = 0UL; i < initial_data.size(); ++i)
      {
        std::vector<int> data1(initial_data);
        std::vector<int> data2(initial_data);

        std::rotate(data1.data(), data1.data() + i, data1.data() + data1.size());
        etl::rotate(data2.data(), data2.data() + i, data2.data() + data2.size());

        bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
        CHECK(isEqual);
      }
    }


    //*************************************************************************
    TEST(rotate_non_pod)
    {
      std::vector<NDC> initial_data = { NDC(1), NDC(2), NDC(3), NDC(4), NDC(5), NDC(6), NDC(7) };

      for (size_t i = 0UL; i < initial_data.size(); ++i)
      {
        std::vector<NDC> data1(initial_data);
        std::vector<NDC> data2(initial_data);

        std::rotate(data1.data(), data1.data() + i, data1.data() + data1.size());
        etl::rotate(data2.data(), data2.data() + i, data2.data() + data2.size());

        bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
        CHECK(isEqual);
      }
    }

    //*************************************************************************
    TEST(any_of)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      bool expected = std::any_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      bool result = etl::any_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);

      expected = std::any_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      result = etl::any_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(all_of)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      bool expected = std::all_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      bool result = etl::all_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      CHECK_EQUAL(expected, result);

      expected = std::all_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      result = etl::all_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(none_of)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

      bool expected = std::none_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 8));
      bool result = etl::none_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 8));
      CHECK_EQUAL(expected, result);

      expected = std::none_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      result = etl::none_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);
    }

    struct Compare
    {
      bool operator()(int a, int b) const
      {
        return a == b;
      }
    };

    //*************************************************************************
    TEST(is_permutation)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
      int permutation[] = { 1, 3, 2, 4, 7, 6, 5, 8 };
      int not_permutation[] = { 1, 2, 3, 4, 5, 6, 7, 7 };

      bool is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation));
      CHECK(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation));
      CHECK(!is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation), etl::equal_to<int>());
      CHECK(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), etl::equal_to<int>());
      CHECK(!is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation), std::end(permutation));
      CHECK(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), std::end(not_permutation));
      CHECK(!is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation), std::end(permutation), etl::equal_to<int>());
      CHECK(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), std::end(not_permutation), etl::equal_to<int>());
      CHECK(!is_permutation);
    }

    //*************************************************************************
    TEST(is_partitioned)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

      bool expected = std::is_partitioned(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      bool result = etl::is_partitioned(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);

      std::partition(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));

      expected = std::is_partitioned(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      result = etl::is_partitioned(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(partition_point)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };

      std::partition(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));

      int* partition1 = std::partition_point(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      int* partition2 = etl::partition_point(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(std::distance(std::begin(data1), partition1), std::distance(std::begin(data1), partition2));

      std::partition(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 8));

      partition1 = std::partition_point(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      partition2 = etl::partition_point(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      CHECK_EQUAL(std::distance(std::begin(data1), partition1), std::distance(std::begin(data1), partition2));
    }

    //*************************************************************************
    TEST(partition_copy)
    {
      int data1[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
      int data2[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      int data3[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      int data4[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      int data5[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

      std::partition_copy(std::begin(data1), std::end(data1), std::begin(data2), std::begin(data3), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      etl::partition_copy(std::begin(data1), std::end(data1), std::begin(data4), std::begin(data5), std::bind(std::greater<int>(), std::placeholders::_1, 4));

      bool are_equal;

      are_equal = std::equal(std::begin(data2), std::end(data2), std::begin(data4));
      CHECK(are_equal);

      are_equal = std::equal(std::begin(data3), std::end(data3), std::begin(data5));
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(find_if_not)
    {
      int data1[] = { 1, 2, 3, 5, 6, 7, 8 };

      // Find the element not less than 4.
      int* p = etl::find_if_not(std::begin(data1), std::end(data1), std::bind(std::less<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(5, *p);
    }

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
      result = etl::copy_s(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_s(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_s(std::begin(data2), std::end(data2), std::begin(out1), std::end(out1));
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
      result = etl::copy_s(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_s(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_s(std::begin(data2), std::end(data2), std::begin(out1), std::end(out1));
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
      result = etl::copy_n_s(std::begin(data1), 10, std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_n_s(std::begin(data1), 10, std::begin(out2), std::end(out2));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_n_s(std::begin(data1), 5, std::begin(out1), std::end(out1));
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
      result = etl::copy_n_s(std::begin(data1), 10, std::begin(out1), 10);
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_n_s(std::begin(data1), 10, std::begin(out2), 5);
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::copy_n_s(std::begin(data1), 5, std::begin(out1), 10);
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
      result = etl::copy_if_s(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1), std::bind(std::less<int>(), std::placeholders::_1, 5));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::copy_if_s(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2), std::bind(std::less<int>(), std::placeholders::_1, 5));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Destination larger.
      std::fill(std::begin(out3), std::end(out3), 0);
      result = etl::copy_if_s(std::begin(data1), std::end(data1), std::begin(out3), std::end(out3), std::bind(std::less<int>(), std::placeholders::_1, 5));
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
      Data data1[] = { { 1, 8 }, { 2, 7 }, { 3, 6 },{ 4, 5 },{ 5, 4 },{ 6, 3 },{ 7, 2 },{ 8, 1 } };
      Data test1   = { 4, 5 };
      Data test2   = { 9, 0 };

      // Find the element of value 5.
      Data* p = etl::binary_find(std::begin(data1), std::end(data1), test1, DataPredicate(), DataEquality());
      CHECK_EQUAL(test1, *p);

      // Find the element of value 4.
      p = etl::binary_find(std::begin(data1), std::end(data1), test2, DataPredicate(), DataEquality());
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
      etl::transform_s(std::begin(input),
                       std::end(input),
                       std::begin(output),
                       std::begin(output) + (ETL_OR_STD17::size(output) / 2),
                       std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);

      std::fill(std::begin(output), std::end(output), 0);

      etl::transform_s(std::begin(input),
                       std::begin(input) + (ETL_OR_STD17::size(input) / 2),
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
    TEST(transform_n_two_ranges_random_iterator)
    {
      int input1[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int input2[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare[] = { 2, 16, 4, 14, 6, 12, 8, 0, 0, 0 };

      etl::transform_n(std::begin(input1),
                       std::begin(input2),
                       7,
                       std::begin(output),
                       std::plus<int>());

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_two_ranges_non_random_iterator)
    {
      std::list<int> input1 = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      std::list<int> input2 = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int output[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
      int compare[] = { 2, 16, 4, 14, 6, 12, 8, 0, 0, 0 };

      etl::transform_n(std::begin(input1),
                       std::begin(input2),
                       7,
                       std::begin(output),
                       std::plus<int>());

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
    TEST(selection_sort_default_forward_iterators)
    {
      std::vector<int> data(100, 0);
      std::iota(data.begin(), data.end(), 1);

      for (int i = 0; i < 100; ++i)
      {
        std::shuffle(data.begin(), data.end(), urng);

        std::vector<int>       data1(data.begin(), data.end());
        std::forward_list<int> data2(data.begin(), data.end());

        std::sort(data1.begin(), data1.end());
        etl::selection_sort(data2.begin(), data2.end());

        bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(is_same);
      }
    }

    //*************************************************************************
    TEST(selection_sort_default_bidirectional_iterators)
    {
      std::vector<int> data(100, 0);
      std::iota(data.begin(), data.end(), 1);

      for (int i = 0; i < 100; ++i)
      {
        std::shuffle(data.begin(), data.end(), urng);

        std::list<int> data1(data.begin(), data.end());
        std::list<int> data2(data.begin(), data.end());

        data1.sort();
        etl::selection_sort(data2.begin(), data2.end());

        bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(is_same);
      }
    }

    //*************************************************************************
    TEST(selection_sort_default_random_access_iterators)
    {
      std::vector<int> data(100, 0);
      std::iota(data.begin(), data.end(), 1);

      for (int i = 0; i < 100; ++i)
      {
        std::shuffle(data.begin(), data.end(), urng);

        std::vector<int> data1 = data;
        std::vector<int> data2 = data;

        std::sort(data1.begin(), data1.end());
        etl::selection_sort(data2.begin(), data2.end());

        bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(is_same);
      }
    }

    //*************************************************************************
    TEST(selection_sort_greater)
    {
      std::vector<int> data(100, 0);
      std::iota(data.begin(), data.end(), 1);

      for (int i = 0; i < 100; ++i)
      {
        std::shuffle(data.begin(), data.end(), urng);

        std::vector<int> data1 = data;
        std::vector<int> data2 = data;

        std::sort(data1.begin(), data1.end(), std::greater<int>());
        etl::selection_sort(data2.begin(), data2.end(), std::greater<int>());

        bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(is_same);
      }
    }

    //*************************************************************************
    TEST(heap_sort_default)
    {
      std::vector<NDC> initial_data = { NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1) };

      std::vector<NDC> data1(initial_data);
      std::vector<NDC> data2(initial_data);

      std::sort(data1.begin(), data1.end());
      etl::heap_sort(data2.begin(), data2.end());

      bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(heap_sort_greater)
    {
      std::vector<NDC> initial_data = { NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1) };

      std::vector<NDC> data1(initial_data);
      std::vector<NDC> data2(initial_data);

      std::sort(data1.begin(), data1.end(), std::greater<NDC>());
      etl::heap_sort(data2.begin(), data2.end(), std::greater<NDC>());

      bool is_same = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(multimax)
    {
      CHECK_EQUAL(8, etl::multimax(1, 2, 3, 4, 5, 6, 7, 8));
      CHECK_EQUAL(8, etl::multimax_compare(std::less<int>(), 1, 2, 3, 4, 5, 6, 7, 8));
      CHECK_EQUAL(1, etl::multimax_compare(std::greater<int>(), 1, 2, 3, 4, 5, 6, 7, 8));

      int temp[etl::multimax(1, 2, 3, 4, 5, 6, 7, 8)] = { 1, 2, 3, 4, 5, 6, 7, 8 };
      (void)temp;
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

    //*************************************************************************
    TEST(replace)
    {
      int data[]     = { 1, 8, 2, 7, 2, 6, 2, 2, 10, 9 };
      int expected[] = { 1, 8, 0, 7, 0, 6, 0, 0, 10, 9 };

      // Replace 2 with 0
      etl::replace(std::begin(data), std::end(data), 2, 0);

      bool is_same = std::equal(std::begin(data), std::end(data), std::begin(expected));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(replace_if)
    {
      int data[]     = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };
      int expected[] = { 0, 8, 0, 7, 0, 6, 0, 0, 10, 9 };

      // Replace <=5 with 0
      etl::replace_if(std::begin(data), std::end(data), std::bind(std::less_equal<int>(), std::placeholders::_1, 5), 0);

      bool is_same = std::equal(std::begin(data), std::end(data), std::begin(expected));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(for_each)
    {
      int data[] = { 1, 8, 2, 7, 3, 6, 4, 5, 10, 9 };

      struct Sum
      {
        void operator()(int i)
        {
          value += i;
        }

        Sum()
          : value(0)
        {
        }

        int value;
      };

      Sum sum;
      sum = etl::for_each(std::begin(data), std::end(data), sum);
      CHECK_EQUAL(std::accumulate(std::begin(data), std::end(data), 0), sum.value);
    }

    //*************************************************************************
    TEST(remove)
    {
      std::array<int, 10> data     = { 1, 8, 2, 7, 7, 7, 4, 5, 10, 9 };
      std::array<int, 7>  expected = { 1, 8, 2, 4, 5, 10, 9 };

      etl::remove(data.begin(), data.end(), 7);

      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(remove_if)
    {
      std::array<int, 10> data     = { 1, 8, 2, 7, 7, 7, 4, 5, 10, 9 };
      std::array<int, 4>  expected = { 1, 2, 4, 5 };

      etl::remove_if(data.begin(), data.end(), [](int value) { return value >= 7; });

      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    struct generator
    {
      generator(int value_)
        : value(value_)
      {
      }

      int operator()()
      {
        return value++;
      }

      int value;
    };

    TEST(generate)
    {
      std::array<int, 10> expected = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
      std::array<int, 10> actual;

      etl::generate(actual.begin(), actual.end(), generator(2));

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(partition_forward_iterator_container)
    {
      // 40,320 permutations.
      std::array<int, 8> origin = { 0, 1, 2, 3, 4, 5, 6, 7 };

      std::forward_list<int> compare(origin.begin(), origin.end());
      std::forward_list<int> data(origin.begin(), origin.end());

      bool complete = false;

      while (!complete)
      {
        auto pivot1 = std::partition(compare.begin(), compare.end(), [](int i) { return etl::is_even(i); });
        auto pivot2 = etl::partition(data.begin(), data.end(), [](int i) { return etl::is_even(i); });

        auto distance1 = std::distance(compare.begin(), pivot1);
        auto distance2 = std::distance(data.begin(), pivot2);

        CHECK_EQUAL(*pivot1, *pivot2);
        CHECK_EQUAL(distance1, distance2);

        for (auto itr = compare.begin(); itr != pivot1; ++itr)
        {
          CHECK_TRUE((etl::is_even(*itr)));
        }

        for (auto itr = pivot1; itr != compare.end(); ++itr)
        {
          CHECK_FALSE((etl::is_even(*itr)));
        }

        complete = !std::next_permutation(origin.begin(), origin.end());

        compare.assign(origin.begin(), origin.end());
        data.assign(origin.begin(), origin.end());
      }
    }

    //*************************************************************************
    TEST(partition_bidirectional_iterator_container)
    {
      // 40,320 permutations.
      std::array<int, 8> initial = { 0, 1, 2, 3, 4, 5, 6, 7 };
      
      std::array<int, 8> compare = initial;
      std::array<int, 8> data = initial;

      bool complete = false;

      while (!complete)
      {
        auto pivot1 = std::partition(compare.begin(), compare.end(), [](int i) { return etl::is_even(i); });
        auto pivot2 = etl::partition(data.begin(), data.end(), [](int i) { return etl::is_even(i); });

        auto distance1 = std::distance(compare.begin(), pivot1);
        auto distance2 = std::distance(data.begin(), pivot2);

        CHECK_EQUAL(*pivot1, *pivot2);
        CHECK_EQUAL(distance1, distance2);

        for (auto itr = compare.begin(); itr != pivot1; ++itr)
        {
          CHECK_TRUE((etl::is_even(*itr)));
        }

        for (auto itr = pivot1; itr != compare.end(); ++itr)
        {
          CHECK_FALSE((etl::is_even(*itr)));
        }

        complete = !std::next_permutation(initial.begin(), initial.end());

        compare = initial;
        data = initial;
      }
    }

    //*************************************************************************
    TEST(nth_element_with_default_less_than_comparison)
    {
      // 40,320 permutations.
      std::array<int, 8> initial = { 0, 1, 2, 3, 4, 5, 6, 7 };

      std::array<int, 8> compare = initial;
      std::array<int, 8> data    = initial;

      bool complete = false;

      // For each nth position of each permutation.
      while (!complete)
      {
        // Try each nth position.
        for (size_t i = 0; i < initial.size(); ++i)
        {
          std::sort(compare.begin(), compare.end());
          etl::nth_element(data.begin(), data.begin() + i, data.end());

          CHECK_EQUAL(compare[i], data[i]);
        }

        complete = !std::next_permutation(initial.begin(), initial.end());

        compare = initial;
        data    = initial;
      }
    }

#if (ETL_USING_CPP20 && ETL_USING_STL) || (ETL_USING_CPP14 && ETL_NOT_USING_STL && !defined(ETL_IN_UNIT_TEST))
    //*************************************************************************
    constexpr int MakeNth(int nth_index)
    {
      std::array<int, 8> data = { 5, 1, 3, 7, 6, 2, 4, 0 };

      etl::nth_element(data.begin(), data.begin() + nth_index, data.end());

      return data[nth_index];
    }
    
    TEST(constexpr_nth_element_with_default_less_than_comparison)
    {
      std::array<int, 8> compare = { 0, 1, 2, 3, 4, 5, 6, 7 };

      constexpr int nth = MakeNth(3);

      CHECK_EQUAL(compare[3], nth);
    }
#endif

    //*************************************************************************
    TEST(nth_element_with_custom_comparison)
    {
      // 40,320 permutations.
      std::array<int, 8> initial = { 0, 1, 2, 3, 4, 5, 6, 7 };

      std::array<int, 8> compare = initial;
      std::array<int, 8> data = initial;

      bool complete = false;

      // For each nth position of each permutation.
      while (!complete)
      {
        // Try each nth position.
        for (size_t i = 0; i < initial.size(); ++i)
        {
          std::sort(compare.begin(), compare.end(), std::greater<int>());
          etl::nth_element(data.begin(), data.begin() + i, data.end(), std::greater<int>());

          CHECK_EQUAL(compare[i], data[i]);
        }

        complete = !std::next_permutation(initial.begin(), initial.end());

        compare = initial;
        data = initial;
      }
    }

    //*************************************************************************
    TEST(clamp_run_time)
    {
      CHECK_EQUAL(5, etl::clamp(5, 0, 10));
      CHECK_EQUAL(0, etl::clamp(-5, 0, 10));
      CHECK_EQUAL(10, etl::clamp(15, 0, 10));
    }

    //*************************************************************************
    TEST(clamp_compile_time)
    {
      CHECK_EQUAL(5,  (etl::clamp<int, 0, 10>(5)));
      CHECK_EQUAL(0,  (etl::clamp<int, 0, 10>(-5)));
      CHECK_EQUAL(10, (etl::clamp<int, 0, 10>(15)));
    }

    //*************************************************************************
    TEST(clamp_constexpr)
    {
      constexpr int result1 = etl::clamp(5, 0, 10);
      constexpr int result2 = etl::clamp(-5, 0, 10);
      constexpr int result3 = etl::clamp(15, 0, 10);

      constexpr int result4 = etl::clamp<int, 0, 10>(5);
      constexpr int result5 = etl::clamp<int, 0, 10>(-5);
      constexpr int result6 = etl::clamp<int, 0, 10>(15);

      CHECK_EQUAL(5,  result1);
      CHECK_EQUAL(0,  result2);
      CHECK_EQUAL(10, result3);

      CHECK_EQUAL(5, result4);
      CHECK_EQUAL(0, result5);
      CHECK_EQUAL(10, result6);
    }
  };
}
