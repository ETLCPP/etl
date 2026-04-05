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
#include "etl/binary.h"
#include "etl/container.h"

#include "data.h"
#include "iterators_for_unit_tests.h"

#include <algorithm>
#include <array>
#include <forward_list>
#include <functional>
#include <list>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

namespace
{
  using NDC   = TestDataNDC<int>;
  using ItemM = TestDataM<std::string>;
  std::random_device rng;
  std::mt19937       urng(rng());

  using Vector = std::vector<int>;
  Vector data  = {2, 1, 1, 4, 3, 6, 5, 8, 7, 10, 10, 9};

  using VectorM = std::vector<ItemM>;

  constexpr size_t SIZE = 10;

  int dataA[SIZE] = {2, 1, 4, 3, 6, 5, 8, 7, 10, 9};

  int            dataS[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::list<int> dataSL      = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  using List = std::list<int>;
  List dataL = {2, 1, 4, 3, 6, 5, 8, 7, 10, 9};

  int            dataEQ[SIZE] = {1, 1, 3, 3, 5, 5, 7, 7, 9, 9};
  std::list<int> dataEQL      = {1, 1, 3, 3, 5, 5, 7, 7, 9, 9};

  Vector dataV = {2, 1, 4, 3, 6, 5, 8, 7, 10, 9};

  int dataD1[SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int dataD2[SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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

  bool operator==(const Data& lhs, const Data& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b);
  }

  struct DataPredicate
  {
    bool operator()(const Data& lhs, const Data& rhs) const
    {
      return lhs.a < rhs.a;
    }
  };

  struct DataEquality
  {
    bool operator()(const Data& lhs, const Data& rhs) const
    {
      return lhs.a == rhs.a;
    }
  };

  struct DataEquivalenceByA : public etl::binary_function<Data, Data, bool>
  {
    bool operator()(const Data& lhs, const Data& rhs) const
    {
      return lhs.a == rhs.a;
    }
  };

  Data dataD[10] = {Data(1, 2), Data(2, 1), Data(3, 4), Data(4, 3), Data(5, 6), Data(6, 5), Data(7, 8), Data(8, 7), Data(9, 10), Data(10, 9)};

  struct Greater : public etl::binary_function<int, int, bool>
  {
    bool operator()(int a, int b) const
    {
      return a > b;
    }
  };

  std::ostream& operator<<(std::ostream& os, const Data& data_)
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
      Vector::iterator result   = etl::min_element(data.begin(), data.end(), std::greater<int>());
      CHECK_EQUAL(std::distance(data.begin(), expected), std::distance(data.begin(), result));
    }

    //*************************************************************************
    TEST(min_element_empty)
    {
      Vector           dataEmpty;
      Vector::iterator expected = std::min_element(dataEmpty.begin(), dataEmpty.end());
      Vector::iterator result   = etl::min_element(dataEmpty.begin(), dataEmpty.end());
      CHECK_EQUAL(std::distance(dataEmpty.end(), expected), std::distance(dataEmpty.end(), result));
    }

    //*************************************************************************
    TEST(max_element)
    {
      Vector::iterator expected = std::max_element(data.begin(), data.end());
      Vector::iterator result   = etl::max_element(data.begin(), data.end());
      CHECK_EQUAL(std::distance(data.begin(), expected), std::distance(data.begin(), result));
    }

    //*************************************************************************
    TEST(max_element_compare)
    {
      Vector::iterator expected = std::max_element(data.begin(), data.end(), std::greater<int>());
      Vector::iterator result   = etl::max_element(data.begin(), data.end(), std::greater<int>());
      CHECK_EQUAL(std::distance(data.begin(), expected), std::distance(data.begin(), result));
    }

    //*************************************************************************
    TEST(max_element_empty)
    {
      Vector           dataEmpty;
      Vector::iterator expected = std::max_element(dataEmpty.begin(), dataEmpty.end());
      Vector::iterator result   = etl::max_element(dataEmpty.begin(), dataEmpty.end());
      CHECK_EQUAL(std::distance(dataEmpty.end(), expected), std::distance(dataEmpty.end(), result));
    }

    //*************************************************************************
    TEST(minmax_element)
    {
      std::pair<Vector::iterator, Vector::iterator> expected = std::minmax_element(data.begin(), data.end());
      std::pair<Vector::iterator, Vector::iterator> result   = etl::minmax_element(data.begin(), data.end());
      CHECK_EQUAL(std::distance(data.begin(), expected.first), std::distance(data.begin(), result.first));
      CHECK_EQUAL(std::distance(data.begin(), expected.second), std::distance(data.begin(), result.second));
    }

    //*************************************************************************
    TEST(minmax_element_compare)
    {
      std::pair<Vector::iterator, Vector::iterator> expected = std::minmax_element(data.begin(), data.end(), std::greater<int>());
      std::pair<Vector::iterator, Vector::iterator> result   = etl::minmax_element(data.begin(), data.end(), std::greater<int>());
      CHECK_EQUAL(std::distance(data.begin(), expected.first), std::distance(data.begin(), result.first));
      CHECK_EQUAL(std::distance(data.begin(), expected.second), std::distance(data.begin(), result.second));
    }

    //*************************************************************************
    TEST(minmax_element_empty)
    {
      Vector                                        dataEmpty;
      std::pair<Vector::iterator, Vector::iterator> expected = std::minmax_element(dataEmpty.begin(), dataEmpty.end());
      std::pair<Vector::iterator, Vector::iterator> result   = etl::minmax_element(dataEmpty.begin(), dataEmpty.end());
      CHECK_EQUAL(std::distance(dataEmpty.begin(), expected.first), std::distance(dataEmpty.begin(), result.first));
      CHECK_EQUAL(std::distance(dataEmpty.begin(), expected.second), std::distance(dataEmpty.begin(), result.second));
    }

    //*************************************************************************
    TEST(minmax)
    {
      int a = 1;
      int b = 2;

      std::pair<int, int> expected = std::minmax(a, b);
      std::pair<int, int> result   = etl::minmax(a, b);
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);

      result   = etl::minmax(b, a);
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
      std::pair<int, int> result   = etl::minmax(a, b, std::greater<int>());
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);

      result   = etl::minmax(b, a, std::greater<int>());
      expected = std::minmax(b, a, std::greater<int>());
      CHECK_EQUAL(expected.first, result.first);
      CHECK_EQUAL(expected.second, result.second);
    }

    //*************************************************************************
    TEST(is_sorted_until)
    {
      int data[] = {1, 2, 3, 4, 6, 5, 7, 8, 9, 10};

      int* p1 = std::is_sorted_until(std::begin(data), std::end(data));
      int* p2 = etl::is_sorted_until(std::begin(data), std::end(data));
      CHECK_EQUAL(std::distance(std::begin(data), p1), std::distance(std::begin(data), p2));
    }

    //*************************************************************************
    TEST(is_sorted_until_compare)
    {
      int data[] = {10, 9, 8, 7, 5, 6, 4, 3, 4, 2, 1};

      int* p1 = std::is_sorted_until(std::begin(data), std::end(data), std::greater<int>());
      int* p2 = etl::is_sorted_until(std::begin(data), std::end(data), std::greater<int>());
      CHECK_EQUAL(std::distance(std::begin(data), p1), std::distance(std::begin(data), p2));
    }

    //*************************************************************************
    TEST(is_sorted)
    {
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      bool is_sorted = etl::is_sorted(std::begin(data1), std::end(data1));
      CHECK(is_sorted);

      int data2[] = {1, 2, 3, 4, 6, 5, 7, 8, 9, 10};

      is_sorted = etl::is_sorted(std::begin(data2), std::end(data2));
      CHECK(!is_sorted);
    }

    //*************************************************************************
    TEST(is_sorted_compare)
    {
      int data1[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      bool is_sorted = etl::is_sorted(std::begin(data1), std::end(data1), std::greater<int>());
      CHECK(is_sorted);

      int data2[] = {10, 9, 8, 7, 5, 6, 4, 3, 2, 1};

      is_sorted = etl::is_sorted(std::begin(data2), std::end(data2), std::greater<int>());
      CHECK(!is_sorted);
    }

    //*************************************************************************
    TEST(is_unique_sorted_until)
    {
      int sorted_data[]     = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int not_sorted_data[] = {1, 2, 3, 4, 6, 5, 7, 8, 9, 10};
      int not_unique_data[] = {1, 2, 3, 4, 4, 5, 6, 8, 9, 10};

      int* p_sorted     = etl::is_unique_sorted_until(std::begin(sorted_data), std::end(sorted_data));
      int* p_not_sorted = etl::is_unique_sorted_until(std::begin(not_sorted_data), std::end(not_sorted_data));
      int* p_not_unique = etl::is_unique_sorted_until(std::begin(not_unique_data), std::end(not_unique_data));

      CHECK_EQUAL(10, std::distance(sorted_data, p_sorted));
      CHECK_EQUAL(5, std::distance(not_sorted_data, p_not_sorted));
      CHECK_EQUAL(4, std::distance(not_unique_data, p_not_unique));
    }

    //*************************************************************************
    TEST(is_unique_sorted_until_compare)
    {
      int sorted_data[]     = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
      int not_sorted_data[] = {10, 9, 8, 7, 5, 6, 4, 3, 2, 1};
      int not_unique_data[] = {10, 9, 8, 6, 5, 4, 4, 3, 2, 1};

      int* p_sorted     = etl::is_unique_sorted_until(std::begin(sorted_data), std::end(sorted_data), std::greater<int>());
      int* p_not_sorted = etl::is_unique_sorted_until(std::begin(not_sorted_data), std::end(not_sorted_data), std::greater<int>());
      int* p_not_unique = etl::is_unique_sorted_until(std::begin(not_unique_data), std::end(not_unique_data), std::greater<int>());

      CHECK_EQUAL(10, std::distance(sorted_data, p_sorted));
      CHECK_EQUAL(5, std::distance(not_sorted_data, p_not_sorted));
      CHECK_EQUAL(6, std::distance(not_unique_data, p_not_unique));
    }

    //*************************************************************************
    TEST(is_unique_sorted)
    {
      int sorted_data[]     = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int not_sorted_data[] = {1, 2, 3, 4, 6, 5, 7, 8, 9, 10};
      int not_unique_data[] = {1, 2, 3, 4, 4, 5, 6, 8, 9, 10};

      CHECK_TRUE((etl::is_unique_sorted(std::begin(sorted_data), std::end(sorted_data))));
      CHECK_FALSE((etl::is_unique_sorted(std::begin(not_sorted_data), std::end(not_sorted_data))));
      CHECK_FALSE((etl::is_unique_sorted(std::begin(not_unique_data), std::end(not_unique_data))));
    }

    //*************************************************************************
    TEST(is_unique_sorted_compare)
    {
      int sorted_data[]     = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
      int not_sorted_data[] = {10, 9, 8, 7, 5, 6, 4, 3, 2, 1};
      int not_unique_data[] = {10, 9, 8, 6, 5, 4, 4, 3, 2, 1};

      CHECK_TRUE((etl::is_unique_sorted(std::begin(sorted_data), std::end(sorted_data), std::greater<int>())));
      CHECK_FALSE((etl::is_unique_sorted(std::begin(not_sorted_data), std::end(not_sorted_data), std::greater<int>())));
      CHECK_FALSE((etl::is_unique_sorted(std::begin(not_unique_data), std::end(not_unique_data), std::greater<int>())));
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
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int data2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int data3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
      std::list<int> data1   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int            data2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int            data3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
      int data1[] = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int data2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int data3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
      std::array<int, 10> data1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      std::array<int, 10> data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      std::reverse(data1.begin(), data1.end());
      etl::reverse(data2.begin(), data2.end());

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_odd_non_pointer)
    {
      std::array<int, 9> data1 = {0, 1, 2, 3, 4, 5, 6, 7, 8};
      std::array<int, 9> data2 = {0, 1, 2, 3, 4, 5, 6, 7, 8};

      std::reverse(data1.begin(), data1.end());
      etl::reverse(data2.begin(), data2.end());

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_even_pointer)
    {
      int data1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      int data2[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

      std::reverse(std::begin(data1), std::end(data1));
      etl::reverse(std::begin(data2), std::end(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_odd_pointer)
    {
      int data1[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
      int data2[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

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
        ETL_OR_STD::pair<int*, int*>                                 lb1 = std::equal_range(std::begin(dataEQ), std::end(dataEQ), i);
        ETL_OR_STD::pair<random_iterator<int>, random_iterator<int>> lb2 =
          etl::equal_range(random_iterator<int>(std::begin(dataEQ)), random_iterator<int>(std::end(dataEQ)), i);

        CHECK_EQUAL(std::distance(std::begin(dataEQ), lb1.first), std::distance<int*>(std::begin(dataEQ), lb2.first));
        CHECK_EQUAL(std::distance(lb1.first, lb1.second), std::distance<int*>(lb2.first, lb2.second));
      }
    }

    //*************************************************************************
    TEST(equal_range_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        ETL_OR_STD::pair<int*, int*>                                         lb1 = std::equal_range(std::begin(dataEQ), std::end(dataEQ), i);
        ETL_OR_STD::pair<non_random_iterator<int>, non_random_iterator<int>> lb2 =
          etl::equal_range(non_random_iterator<int>(std::begin(dataEQ)), non_random_iterator<int>(std::end(dataEQ)), i);

        CHECK_EQUAL(std::distance(std::begin(dataEQ), lb1.first), std::distance<int*>(std::begin(dataEQ), lb2.first));
        CHECK_EQUAL(std::distance(lb1.first, lb1.second), std::distance<int*>(lb2.first, lb2.second));
      }
    }

    //*************************************************************************
    TEST(binary_search_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        bool expected = std::binary_search(std::begin(dataS), std::end(dataS), i);
        bool result   = etl::binary_search(std::begin(dataS), std::end(dataS), i);

        CHECK_EQUAL(expected, result);
      }
    }

    //*************************************************************************
    TEST(binary_search_with_compare)
    {
      for (int i = 0; i < 11; ++i)
      {
        bool expected = std::binary_search(std::begin(dataS), std::end(dataS), i, etl::less<int>());
        bool result   = etl::binary_search(std::begin(dataS), std::end(dataS), i, etl::less<int>());

        CHECK_EQUAL(expected, result);
      }
    }

    //*************************************************************************
    TEST(binary_search_duplicates)
    {
      for (int i = 0; i < 11; ++i)
      {
        bool expected = std::binary_search(std::begin(dataEQ), std::end(dataEQ), i);
        bool result   = etl::binary_search(std::begin(dataEQ), std::end(dataEQ), i);

        CHECK_EQUAL(expected, result);
      }
    }

    //*************************************************************************
    TEST(binary_search_empty_range)
    {
      int empty[] = {0};

      bool result = etl::binary_search(std::begin(empty), std::begin(empty), 1);

      CHECK_EQUAL(false, result);
    }

    //*************************************************************************
    TEST(binary_search_single_element)
    {
      int single[] = {5};

      CHECK_EQUAL(true, etl::binary_search(std::begin(single), std::end(single), 5));
      CHECK_EQUAL(false, etl::binary_search(std::begin(single), std::end(single), 3));
      CHECK_EQUAL(false, etl::binary_search(std::begin(single), std::end(single), 7));
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

      std::fill(std::begin(data1), std::end(data1), static_cast<unsigned char>(0x12U));
      etl::fill(std::begin(data2), std::end(data2), static_cast<unsigned char>(0x12U));

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
    TEST(swap_ranges_pod_pointer)
    {
      int data1[] = {1, 2, 3, 4, 5};
      int data2[] = {6, 7, 8, 9, 10};

      int expected1[] = {6, 7, 8, 9, 10};
      int expected2[] = {1, 2, 3, 4, 5};

      int* result = etl::swap_ranges(std::begin(data1), std::end(data1), std::begin(data2));

      CHECK_EQUAL(std::end(data2), result);

      bool isEqual1 = std::equal(std::begin(data1), std::end(data1), std::begin(expected1));
      CHECK(isEqual1);

      bool isEqual2 = std::equal(std::begin(data2), std::end(data2), std::begin(expected2));
      CHECK(isEqual2);
    }

    //*************************************************************************
    TEST(swap_ranges_non_pod_pointer)
    {
      Data data1[] = {Data(1, 2), Data(3, 4), Data(5, 6)};
      Data data2[] = {Data(7, 8), Data(9, 10), Data(11, 12)};

      Data expected1[] = {Data(7, 8), Data(9, 10), Data(11, 12)};
      Data expected2[] = {Data(1, 2), Data(3, 4), Data(5, 6)};

      Data* result = etl::swap_ranges(std::begin(data1), std::end(data1), std::begin(data2));

      CHECK_EQUAL(std::end(data2), result);

      bool isEqual1 = std::equal(std::begin(data1), std::end(data1), std::begin(expected1));
      CHECK(isEqual1);

      bool isEqual2 = std::equal(std::begin(data2), std::end(data2), std::begin(expected2));
      CHECK(isEqual2);
    }

    //*************************************************************************
    TEST(swap_ranges_non_random_iterator)
    {
      List data1 = {1, 2, 3, 4, 5};
      List data2 = {6, 7, 8, 9, 10};

      List expected1 = {6, 7, 8, 9, 10};
      List expected2 = {1, 2, 3, 4, 5};

      List::iterator result = etl::swap_ranges(data1.begin(), data1.end(), data2.begin());

      CHECK(data2.end() == result);

      bool isEqual1 = std::equal(data1.begin(), data1.end(), expected1.begin());
      CHECK(isEqual1);

      bool isEqual2 = std::equal(data2.begin(), data2.end(), expected2.begin());
      CHECK(isEqual2);
    }

    //*************************************************************************
    TEST(swap_ranges_empty_range)
    {
      int data1[] = {1, 2, 3};
      int data2[] = {4, 5, 6};

      int expected1[] = {1, 2, 3};
      int expected2[] = {4, 5, 6};

      int* result = etl::swap_ranges(std::begin(data1), std::begin(data1), std::begin(data2));

      CHECK_EQUAL(std::begin(data2), result);

      bool isEqual1 = std::equal(std::begin(data1), std::end(data1), std::begin(expected1));
      CHECK(isEqual1);

      bool isEqual2 = std::equal(std::begin(data2), std::end(data2), std::begin(expected2));
      CHECK(isEqual2);
    }

    //*************************************************************************
    TEST(swap_ranges_partial_range)
    {
      int data1[] = {1, 2, 3, 4, 5};
      int data2[] = {6, 7, 8, 9, 10};

      int expected1[] = {6, 7, 8, 4, 5};
      int expected2[] = {1, 2, 3, 9, 10};

      int* result = etl::swap_ranges(std::begin(data1), std::begin(data1) + 3, std::begin(data2));

      CHECK_EQUAL(std::begin(data2) + 3, result);

      bool isEqual1 = std::equal(std::begin(data1), std::end(data1), std::begin(expected1));
      CHECK(isEqual1);

      bool isEqual2 = std::equal(std::begin(data2), std::end(data2), std::begin(expected2));
      CHECK(isEqual2);
    }

    //*************************************************************************
    TEST(swap_ranges_same_data)
    {
      int data1[]    = {1, 2, 3, 4, 5};
      int expected[] = {1, 2, 3, 4, 5};

      etl::swap_ranges(std::begin(data1), std::end(data1), std::begin(data1));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(expected));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(swap_ranges_matches_std)
    {
      int data1_std[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int data2_std[] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

      int data1_etl[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int data2_etl[] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

      int* pstl = std::swap_ranges(std::begin(data1_std), std::end(data1_std), std::begin(data2_std));
      int* petl = etl::swap_ranges(std::begin(data1_etl), std::end(data1_etl), std::begin(data2_etl));

      using difference_type_t = std::iterator_traits<int*>::difference_type;

      difference_type_t dstl = std::distance(data2_std, pstl);
      difference_type_t detl = std::distance(data2_etl, petl);

      CHECK_EQUAL(dstl, detl);

      bool isEqual1 = std::equal(std::begin(data1_std), std::end(data1_std), std::begin(data1_etl));
      CHECK(isEqual1);

      bool isEqual2 = std::equal(std::begin(data2_std), std::end(data2_std), std::begin(data2_etl));
      CHECK(isEqual2);
    }

    //*************************************************************************
    TEST(equal)
    {
      CHECK(etl::equal(std::begin(dataV), std::end(dataV), std::begin(dataL)));
      CHECK(!etl::equal(std::begin(dataSL), std::end(dataSL), std::begin(dataL)));

      int small[] = {dataS[0]};
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
      std::string needle   = "KLMNO";

      std::string::iterator itr1 = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin());
      std::string::iterator itr2 = etl::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin());

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(search_predicate)
    {
      std::string haystack = "ABCDFEGHIJKLMNOPQRSTUVWXYZ";
      std::string needle   = "KLMNO";

      std::string::iterator itr1 = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin(), std::equal_to<char>());
      std::string::iterator itr2 = etl::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin(), std::equal_to<char>());

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(find_end_default)
    {
      int data[]    = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
      int pattern[] = {1, 2, 3};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_predicate)
    {
      int data[]    = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
      int pattern[] = {1, 2, 3};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern), std::equal_to<int>());
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern), std::equal_to<int>());

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_single_occurrence)
    {
      int data[]    = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int pattern[] = {5, 6, 7};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_no_match)
    {
      int data[]    = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int pattern[] = {11, 12};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_empty_sequence)
    {
      int data[]    = {1, 2, 3, 4, 5};
      int pattern[] = {0};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::begin(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::begin(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_pattern_at_end)
    {
      int data[]    = {1, 2, 3, 4, 5, 6, 7};
      int pattern[] = {5, 6, 7};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_pattern_at_start)
    {
      int data[]    = {1, 2, 3, 4, 5, 6, 7};
      int pattern[] = {1, 2, 3};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_entire_range_matches)
    {
      int data[]    = {1, 2, 3, 4, 5};
      int pattern[] = {1, 2, 3, 4, 5};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_overlapping_occurrences)
    {
      int data[]    = {1, 1, 1, 1, 1};
      int pattern[] = {1, 1};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_single_element_pattern)
    {
      int data[]    = {1, 2, 3, 2, 5, 2, 7};
      int pattern[] = {2};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(find_end_non_random_iterator)
    {
      int data_array[]    = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
      int pattern_array[] = {1, 2, 3};

      List data(std::begin(data_array), std::end(data_array));
      List pattern(std::begin(pattern_array), std::end(pattern_array));

      List::iterator expected = std::find_end(data.begin(), data.end(), pattern.begin(), pattern.end(), std::equal_to<int>());
      List::iterator result   = etl::find_end(data.begin(), data.end(), pattern.begin(), pattern.end(), std::equal_to<int>());

      CHECK(expected == result);
    }

    //*************************************************************************
    TEST(find_end_non_random_iterator_predicate)
    {
      int data_array[]    = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
      int pattern_array[] = {1, 2, 3};

      List data(std::begin(data_array), std::end(data_array));
      List pattern(std::begin(pattern_array), std::end(pattern_array));

      List::iterator expected = std::find_end(data.begin(), data.end(), pattern.begin(), pattern.end(), std::equal_to<int>());
      List::iterator result   = etl::find_end(data.begin(), data.end(), pattern.begin(), pattern.end(), std::equal_to<int>());

      CHECK(expected == result);
    }

    //*************************************************************************
    TEST(find_end_pattern_longer_than_data)
    {
      int data[]    = {1, 2, 3};
      int pattern[] = {1, 2, 3, 4, 5};

      int* expected = std::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));
      int* result   = etl::find_end(std::begin(data), std::end(data), std::begin(pattern), std::end(pattern));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_default)
    {
      int data[] = {1, 2, 3, 3, 4, 5};

      int* expected = std::adjacent_find(std::begin(data), std::end(data));
      int* result   = etl::adjacent_find(std::begin(data), std::end(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_predicate)
    {
      int data[] = {1, 2, 3, 3, 4, 5};

      int* expected = std::adjacent_find(std::begin(data), std::end(data), std::equal_to<int>());
      int* result   = etl::adjacent_find(std::begin(data), std::end(data), std::equal_to<int>());

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_no_match)
    {
      int data[] = {1, 2, 3, 4, 5, 6};

      int* expected = std::adjacent_find(std::begin(data), std::end(data));
      int* result   = etl::adjacent_find(std::begin(data), std::end(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_at_beginning)
    {
      int data[] = {1, 1, 2, 3, 4, 5};

      int* expected = std::adjacent_find(std::begin(data), std::end(data));
      int* result   = etl::adjacent_find(std::begin(data), std::end(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_at_end)
    {
      int data[] = {1, 2, 3, 4, 5, 5};

      int* expected = std::adjacent_find(std::begin(data), std::end(data));
      int* result   = etl::adjacent_find(std::begin(data), std::end(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_multiple_pairs)
    {
      int data[] = {1, 1, 2, 2, 3, 3};

      int* expected = std::adjacent_find(std::begin(data), std::end(data));
      int* result   = etl::adjacent_find(std::begin(data), std::end(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_single_element)
    {
      int data[] = {1};

      int* expected = std::adjacent_find(std::begin(data), std::end(data));
      int* result   = etl::adjacent_find(std::begin(data), std::end(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_empty_range)
    {
      int data[] = {1};

      int* expected = std::adjacent_find(std::begin(data), std::begin(data));
      int* result   = etl::adjacent_find(std::begin(data), std::begin(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_all_same)
    {
      int data[] = {5, 5, 5, 5, 5};

      int* expected = std::adjacent_find(std::begin(data), std::end(data));
      int* result   = etl::adjacent_find(std::begin(data), std::end(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_predicate_less)
    {
      int data[] = {5, 4, 3, 2, 1};

      int* expected = std::adjacent_find(std::begin(data), std::end(data), std::greater<int>());
      int* result   = etl::adjacent_find(std::begin(data), std::end(data), std::greater<int>());

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(adjacent_find_non_random_iterator)
    {
      int  data_array[] = {1, 2, 3, 3, 4, 5};
      List data(std::begin(data_array), std::end(data_array));

      List::iterator expected = std::adjacent_find(data.begin(), data.end());
      List::iterator result   = etl::adjacent_find(data.begin(), data.end());

      CHECK(expected == result);
    }

    //*************************************************************************
    TEST(adjacent_find_non_random_iterator_predicate)
    {
      int  data_array[] = {1, 2, 3, 3, 4, 5};
      List data(std::begin(data_array), std::end(data_array));

      List::iterator expected = std::adjacent_find(data.begin(), data.end(), std::equal_to<int>());
      List::iterator result   = etl::adjacent_find(data.begin(), data.end(), std::equal_to<int>());

      CHECK(expected == result);
    }

    //*************************************************************************
    TEST(adjacent_find_non_random_iterator_no_match)
    {
      int  data_array[] = {1, 2, 3, 4, 5, 6};
      List data(std::begin(data_array), std::end(data_array));

      List::iterator expected = std::adjacent_find(data.begin(), data.end());
      List::iterator result   = etl::adjacent_find(data.begin(), data.end());

      CHECK(expected == result);
    }

    //*************************************************************************
    TEST(heap)
    {
      using Vector = std::vector<std::string>;

      std::string a("A"), b("B"), c("C"), d("D"), e("E"), f("F"), g("G"), h("H"), i("I"), j("J");

      Vector data1 = {a, b, c, d, e, f, g, h, i, j};
      Vector data2 = {a, b, c, d, e, f, g, h, i, j};

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
    TEST(is_heap_default_true)
    {
      std::vector<int> data = {9, 8, 7, 6, 5, 4, 3, 2, 1};
      std::make_heap(data.begin(), data.end());

      bool expected = std::is_heap(data.begin(), data.end());
      bool result   = etl::is_heap(data.begin(), data.end());

      CHECK_EQUAL(expected, result);
      CHECK(result);
    }

    //*************************************************************************
    TEST(is_heap_default_false)
    {
      std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      bool expected = std::is_heap(data.begin(), data.end());
      bool result   = etl::is_heap(data.begin(), data.end());

      CHECK_EQUAL(expected, result);
      CHECK(!result);
    }

    //*************************************************************************
    TEST(is_heap_compare_true)
    {
      std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::make_heap(data.begin(), data.end(), Greater());

      bool expected = std::is_heap(data.begin(), data.end(), Greater());
      bool result   = etl::is_heap(data.begin(), data.end(), Greater());

      CHECK_EQUAL(expected, result);
      CHECK(result);
    }

    //*************************************************************************
    TEST(is_heap_compare_false)
    {
      std::vector<int> data = {9, 8, 7, 6, 5, 4, 3, 2, 1};

      bool expected = std::is_heap(data.begin(), data.end(), Greater());
      bool result   = etl::is_heap(data.begin(), data.end(), Greater());

      CHECK_EQUAL(expected, result);
      CHECK(!result);
    }

    //*************************************************************************
    TEST(is_heap_empty)
    {
      std::vector<int> data;

      bool expected = std::is_heap(data.begin(), data.end());
      bool result   = etl::is_heap(data.begin(), data.end());

      CHECK_EQUAL(expected, result);
      CHECK(result);
    }

    //*************************************************************************
    TEST(is_heap_single_element)
    {
      std::vector<int> data = {42};

      bool expected = std::is_heap(data.begin(), data.end());
      bool result   = etl::is_heap(data.begin(), data.end());

      CHECK_EQUAL(expected, result);
      CHECK(result);
    }

    //*************************************************************************
    TEST(is_heap_two_elements)
    {
      std::vector<int> data1 = {5, 3};
      std::vector<int> data2 = {3, 5};

      CHECK_EQUAL(std::is_heap(data1.begin(), data1.end()), etl::is_heap(data1.begin(), data1.end()));
      CHECK_EQUAL(std::is_heap(data2.begin(), data2.end()), etl::is_heap(data2.begin(), data2.end()));
    }

    //*************************************************************************
    TEST(is_heap_pointer)
    {
      int data[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      bool expected = std::is_heap(std::begin(data), std::end(data));
      bool result   = etl::is_heap(std::begin(data), std::end(data));

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(is_heap_after_make_heap)
    {
      // Test all permutations of a small dataset
      std::vector<int> data = {1, 2, 3, 4, 5};

      do {
        std::vector<int> test_data(data);
        etl::make_heap(test_data.begin(), test_data.end());
        CHECK(etl::is_heap(test_data.begin(), test_data.end()));
      } while (std::next_permutation(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST(sort_heap_default)
    {
      std::vector<int> data1 = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> data2(data1);

      std::make_heap(data1.begin(), data1.end());
      etl::make_heap(data2.begin(), data2.end());

      std::sort_heap(data1.begin(), data1.end());
      etl::sort_heap(data2.begin(), data2.end());

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);

      // Verify sorted ascending
      CHECK(std::is_sorted(data2.begin(), data2.end()));
    }

    //*************************************************************************
    TEST(sort_heap_compare)
    {
      std::vector<int> data1 = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> data2(data1);

      std::make_heap(data1.begin(), data1.end(), Greater());
      etl::make_heap(data2.begin(), data2.end(), Greater());

      std::sort_heap(data1.begin(), data1.end(), Greater());
      etl::sort_heap(data2.begin(), data2.end(), Greater());

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);

      // Verify sorted descending
      CHECK(std::is_sorted(data2.begin(), data2.end(), Greater()));
    }

    //*************************************************************************
    TEST(sort_heap_empty)
    {
      std::vector<int> data;

      etl::sort_heap(data.begin(), data.end());

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST(sort_heap_single_element)
    {
      std::vector<int> data = {42};

      etl::sort_heap(data.begin(), data.end());

      CHECK_EQUAL(1U, data.size());
      CHECK_EQUAL(42, data[0]);
    }

    //*************************************************************************
    TEST(sort_heap_pointer)
    {
      int data1[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      int data2[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};

      std::make_heap(std::begin(data1), std::end(data1));
      etl::make_heap(std::begin(data2), std::end(data2));

      std::sort_heap(std::begin(data1), std::end(data1));
      etl::sort_heap(std::begin(data2), std::end(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(sort_heap_all_permutations)
    {
      std::vector<int> initial = {1, 2, 3, 4, 5};

      do {
        std::vector<int> data1(initial);
        std::vector<int> data2(initial);

        std::make_heap(data1.begin(), data1.end());
        etl::make_heap(data2.begin(), data2.end());

        std::sort_heap(data1.begin(), data1.end());
        etl::sort_heap(data2.begin(), data2.end());

        bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
        CHECK(isEqual);
      } while (std::next_permutation(initial.begin(), initial.end()));
    }

    //*************************************************************************
    TEST(sort_heap_duplicates)
    {
      std::vector<int> data1 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
      std::vector<int> data2(data1);

      std::make_heap(data1.begin(), data1.end());
      etl::make_heap(data2.begin(), data2.end());

      std::sort_heap(data1.begin(), data1.end());
      etl::sort_heap(data2.begin(), data2.end());

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(partial_sort_default)
    {
      std::vector<int> data1 = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> data2(data1);

      std::partial_sort(data1.begin(), data1.begin() + 5, data1.end());
      etl::partial_sort(data2.begin(), data2.begin() + 5, data2.end());

      // The first 5 elements should be sorted and match std
      bool isEqual = std::equal(data1.begin(), data1.begin() + 5, data2.begin());
      CHECK(isEqual);

      // Verify sorted range
      CHECK(std::is_sorted(data2.begin(), data2.begin() + 5));
    }

    //*************************************************************************
    TEST(partial_sort_compare)
    {
      std::vector<int> data1 = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> data2(data1);

      std::partial_sort(data1.begin(), data1.begin() + 5, data1.end(), Greater());
      etl::partial_sort(data2.begin(), data2.begin() + 5, data2.end(), Greater());

      // The first 5 elements should be sorted descending and match std
      bool isEqual = std::equal(data1.begin(), data1.begin() + 5, data2.begin());
      CHECK(isEqual);

      // Verify sorted range (descending)
      CHECK(std::is_sorted(data2.begin(), data2.begin() + 5, Greater()));
    }

    //*************************************************************************
    TEST(partial_sort_empty)
    {
      std::vector<int> data;

      etl::partial_sort(data.begin(), data.begin(), data.end());

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST(partial_sort_middle_equals_first)
    {
      std::vector<int> data = {5, 3, 8, 1, 9};
      std::vector<int> original(data);

      etl::partial_sort(data.begin(), data.begin(), data.end());

      // Nothing should change when middle == first
      bool isEqual = std::equal(data.begin(), data.end(), original.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(partial_sort_middle_equals_last)
    {
      std::vector<int> data1 = {5, 3, 8, 1, 9};
      std::vector<int> data2(data1);

      std::partial_sort(data1.begin(), data1.end(), data1.end());
      etl::partial_sort(data2.begin(), data2.end(), data2.end());

      // Full sort
      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(data2.begin(), data2.end()));
    }

    //*************************************************************************
    TEST(partial_sort_single_element)
    {
      std::vector<int> data = {42};

      etl::partial_sort(data.begin(), data.end(), data.end());

      CHECK_EQUAL(1U, data.size());
      CHECK_EQUAL(42, data[0]);
    }

    //*************************************************************************
    TEST(partial_sort_first_one)
    {
      std::vector<int> data1 = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> data2(data1);

      std::partial_sort(data1.begin(), data1.begin() + 1, data1.end());
      etl::partial_sort(data2.begin(), data2.begin() + 1, data2.end());

      // The first element should be the minimum
      CHECK_EQUAL(data1[0], data2[0]);
      CHECK_EQUAL(1, data2[0]);
    }

    //*************************************************************************
    TEST(partial_sort_duplicates)
    {
      std::vector<int> data1 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
      std::vector<int> data2(data1);

      std::partial_sort(data1.begin(), data1.begin() + 6, data1.end());
      etl::partial_sort(data2.begin(), data2.begin() + 6, data2.end());

      bool isEqual = std::equal(data1.begin(), data1.begin() + 6, data2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(data2.begin(), data2.begin() + 6));
    }

    //*************************************************************************
    TEST(partial_sort_already_sorted)
    {
      std::vector<int> data1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> data2(data1);

      std::partial_sort(data1.begin(), data1.begin() + 5, data1.end());
      etl::partial_sort(data2.begin(), data2.begin() + 5, data2.end());

      bool isEqual = std::equal(data1.begin(), data1.begin() + 5, data2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(data2.begin(), data2.begin() + 5));
    }

    //*************************************************************************
    TEST(partial_sort_reverse_sorted)
    {
      std::vector<int> data1 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
      std::vector<int> data2(data1);

      std::partial_sort(data1.begin(), data1.begin() + 5, data1.end());
      etl::partial_sort(data2.begin(), data2.begin() + 5, data2.end());

      bool isEqual = std::equal(data1.begin(), data1.begin() + 5, data2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(data2.begin(), data2.begin() + 5));
    }

    //*************************************************************************
    TEST(partial_sort_pointer)
    {
      int data1[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      int data2[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};

      std::partial_sort(std::begin(data1), std::begin(data1) + 5, std::end(data1));
      etl::partial_sort(std::begin(data2), std::begin(data2) + 5, std::end(data2));

      bool isEqual = std::equal(std::begin(data1), std::begin(data1) + 5, std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(partial_sort_all_permutations)
    {
      std::vector<int> initial = {1, 2, 3, 4, 5};

      do {
        std::vector<int> data1(initial);
        std::vector<int> data2(initial);

        std::partial_sort(data1.begin(), data1.begin() + 3, data1.end());
        etl::partial_sort(data2.begin(), data2.begin() + 3, data2.end());

        bool isEqual = std::equal(data1.begin(), data1.begin() + 3, data2.begin());
        CHECK(isEqual);
      } while (std::next_permutation(initial.begin(), initial.end()));
    }

    //*************************************************************************
    TEST(partial_sort_all_equal)
    {
      std::vector<int> data = {5, 5, 5, 5, 5, 5};

      etl::partial_sort(data.begin(), data.begin() + 3, data.end());

      CHECK(std::is_sorted(data.begin(), data.begin() + 3));

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(5, data[i]);
      }
    }

    //*************************************************************************
    TEST(partial_sort_copy_default)
    {
      std::vector<int> input = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> output1(5);
      std::vector<int> output2(5);

      std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      bool isEqual = std::equal(output1.begin(), output1.end(), output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), output2.end()));
    }

    //*************************************************************************
    TEST(partial_sort_copy_compare)
    {
      std::vector<int> input = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> output1(5);
      std::vector<int> output2(5);

      std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end(), Greater());
      etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end(), Greater());

      bool isEqual = std::equal(output1.begin(), output1.end(), output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), output2.end(), Greater()));
    }

    //*************************************************************************
    TEST(partial_sort_copy_empty_input)
    {
      std::vector<int> input;
      std::vector<int> output(5, 99);

      auto result = etl::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      CHECK(result == output.begin());

      // Output should be unchanged
      for (size_t i = 0; i < output.size(); ++i)
      {
        CHECK_EQUAL(99, output[i]);
      }
    }

    //*************************************************************************
    TEST(partial_sort_copy_empty_output)
    {
      std::vector<int> input = {5, 3, 8, 1, 9};
      std::vector<int> output;

      auto result = etl::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      CHECK(result == output.begin());
    }

    //*************************************************************************
    TEST(partial_sort_copy_output_larger_than_input)
    {
      std::vector<int> input = {5, 3, 1};
      std::vector<int> output1(6, 99);
      std::vector<int> output2(6, 99);

      auto result1 = std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      auto result2 = etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      CHECK_EQUAL(std::distance(output1.begin(), result1), std::distance(output2.begin(), result2));

      bool isEqual = std::equal(output1.begin(), result1, output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), result2));
    }

    //*************************************************************************
    TEST(partial_sort_copy_output_smaller_than_input)
    {
      std::vector<int> input = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> output1(3);
      std::vector<int> output2(3);

      std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      bool isEqual = std::equal(output1.begin(), output1.end(), output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), output2.end()));
    }

    //*************************************************************************
    TEST(partial_sort_copy_output_same_size_as_input)
    {
      std::vector<int> input = {5, 3, 8, 1, 9};
      std::vector<int> output1(5);
      std::vector<int> output2(5);

      auto result1 = std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      auto result2 = etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      CHECK(result1 == output1.end());
      CHECK(result2 == output2.end());

      bool isEqual = std::equal(output1.begin(), output1.end(), output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), output2.end()));
    }

    //*************************************************************************
    TEST(partial_sort_copy_single_element_input)
    {
      std::vector<int> input = {42};
      std::vector<int> output(3, 0);

      auto result = etl::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      CHECK(result == output.begin() + 1);
      CHECK_EQUAL(42, output[0]);
    }

    //*************************************************************************
    TEST(partial_sort_copy_single_element_output)
    {
      std::vector<int> input = {5, 3, 8, 1, 9};
      std::vector<int> output1(1);
      std::vector<int> output2(1);

      std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      CHECK_EQUAL(output1[0], output2[0]);
      CHECK_EQUAL(1, output2[0]);
    }

    //*************************************************************************
    TEST(partial_sort_copy_duplicates)
    {
      std::vector<int> input = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
      std::vector<int> output1(6);
      std::vector<int> output2(6);

      std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      bool isEqual = std::equal(output1.begin(), output1.end(), output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), output2.end()));
    }

    //*************************************************************************
    TEST(partial_sort_copy_already_sorted)
    {
      std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> output1(5);
      std::vector<int> output2(5);

      std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      bool isEqual = std::equal(output1.begin(), output1.end(), output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), output2.end()));
    }

    //*************************************************************************
    TEST(partial_sort_copy_reverse_sorted)
    {
      std::vector<int> input = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
      std::vector<int> output1(5);
      std::vector<int> output2(5);

      std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      bool isEqual = std::equal(output1.begin(), output1.end(), output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), output2.end()));
    }

    //*************************************************************************
    TEST(partial_sort_copy_pointer)
    {
      int input[]    = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      int output1[5] = {};
      int output2[5] = {};

      std::partial_sort_copy(std::begin(input), std::end(input), std::begin(output1), std::end(output1));
      etl::partial_sort_copy(std::begin(input), std::end(input), std::begin(output2), std::end(output2));

      bool isEqual = std::equal(std::begin(output1), std::end(output1), std::begin(output2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(partial_sort_copy_all_equal)
    {
      std::vector<int> input = {5, 5, 5, 5, 5, 5};
      std::vector<int> output(3);

      etl::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      CHECK(std::is_sorted(output.begin(), output.end()));

      for (size_t i = 0; i < output.size(); ++i)
      {
        CHECK_EQUAL(5, output[i]);
      }
    }

    //*************************************************************************
    TEST(partial_sort_copy_input_not_modified)
    {
      std::vector<int> input = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> original(input);
      std::vector<int> output(5);

      etl::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      // Input should not be modified
      bool isEqual = std::equal(input.begin(), input.end(), original.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(partial_sort_copy_from_list)
    {
      std::list<int>   input = {5, 3, 8, 1, 9, 2, 7, 4, 6, 10};
      std::vector<int> output1(5);
      std::vector<int> output2(5);

      std::partial_sort_copy(input.begin(), input.end(), output1.begin(), output1.end());
      etl::partial_sort_copy(input.begin(), input.end(), output2.begin(), output2.end());

      bool isEqual = std::equal(output1.begin(), output1.end(), output2.begin());
      CHECK(isEqual);

      CHECK(std::is_sorted(output2.begin(), output2.end()));
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
      size_t c1 = static_cast<size_t>(std::count(std::begin(dataEQ), std::end(dataEQ), 5));
      size_t c2 = static_cast<size_t>(etl::count(std::begin(dataEQ), std::end(dataEQ), 5));

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

      size_t c1 = static_cast<size_t>(std::count_if(std::begin(dataEQ), std::end(dataEQ), predicate()));
      size_t c2 = static_cast<size_t>(etl::count_if(std::begin(dataEQ), std::end(dataEQ), predicate()));

      CHECK(c1 == c2);
    }

    //*************************************************************************
    TEST(fill_n)
    {
      (void)std::fill_n(std::begin(dataD1), SIZE, 5);
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

      (void)std::transform(std::begin(dataS), std::end(dataS), std::begin(dataD1), Function());
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
    TEST(move_s_random_iterator_same_size)
    {
      typedef std::vector<std::unique_ptr<unsigned>> Data;

      Data data1;
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(1U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(2U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(3U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(4U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(5U)));

      Data data2(5);

      Data::iterator result = etl::move_s(data1.begin(), data1.end(), data2.begin(), data2.end());

      CHECK(data2.end() == result);

      CHECK_EQUAL(1U, *(data2[0]));
      CHECK_EQUAL(2U, *(data2[1]));
      CHECK_EQUAL(3U, *(data2[2]));
      CHECK_EQUAL(4U, *(data2[3]));
      CHECK_EQUAL(5U, *(data2[4]));
    }

    //*************************************************************************
    TEST(move_s_random_iterator_destination_smaller)
    {
      typedef std::vector<std::unique_ptr<unsigned>> Data;

      Data data1;
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(1U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(2U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(3U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(4U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(5U)));

      Data data2(3);

      Data::iterator result = etl::move_s(data1.begin(), data1.end(), data2.begin(), data2.end());

      CHECK(data2.end() == result);

      CHECK_EQUAL(1U, *(data2[0]));
      CHECK_EQUAL(2U, *(data2[1]));
      CHECK_EQUAL(3U, *(data2[2]));
    }

    //*************************************************************************
    TEST(move_s_random_iterator_source_smaller)
    {
      typedef std::vector<std::unique_ptr<unsigned>> Data;

      Data data1;
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(1U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(2U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(3U)));

      Data data2(5);

      Data::iterator result = etl::move_s(data1.begin(), data1.end(), data2.begin(), data2.end());

      CHECK(data2.begin() + 3 == result);

      CHECK_EQUAL(1U, *(data2[0]));
      CHECK_EQUAL(2U, *(data2[1]));
      CHECK_EQUAL(3U, *(data2[2]));
    }

    //*************************************************************************
    TEST(move_s_non_random_iterator_same_size)
    {
      typedef std::list<std::unique_ptr<unsigned>> Data;

      Data data1;
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(1U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(2U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(3U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(4U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(5U)));

      Data data2(5);

      Data::iterator result = etl::move_s(data1.begin(), data1.end(), data2.begin(), data2.end());

      CHECK(data2.end() == result);

      Data::iterator itr = data2.begin();
      CHECK_EQUAL(1U, **(itr++));
      CHECK_EQUAL(2U, **(itr++));
      CHECK_EQUAL(3U, **(itr++));
      CHECK_EQUAL(4U, **(itr++));
      CHECK_EQUAL(5U, **(itr++));
    }

    //*************************************************************************
    TEST(move_s_non_random_iterator_destination_smaller)
    {
      typedef std::list<std::unique_ptr<unsigned>> Data;

      Data data1;
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(1U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(2U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(3U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(4U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(5U)));

      Data data2(3);

      Data::iterator result = etl::move_s(data1.begin(), data1.end(), data2.begin(), data2.end());

      CHECK(data2.end() == result);

      Data::iterator itr = data2.begin();
      CHECK_EQUAL(1U, **(itr++));
      CHECK_EQUAL(2U, **(itr++));
      CHECK_EQUAL(3U, **(itr++));
    }

    //*************************************************************************
    TEST(move_s_non_random_iterator_source_smaller)
    {
      typedef std::list<std::unique_ptr<unsigned>> Data;

      Data data1;
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(1U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(2U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(3U)));

      Data data2(5);

      Data::iterator result = etl::move_s(data1.begin(), data1.end(), data2.begin(), data2.end());

      Data::iterator expected_pos = data2.begin();
      std::advance(expected_pos, 3);
      CHECK(expected_pos == result);

      Data::iterator itr = data2.begin();
      CHECK_EQUAL(1U, **(itr++));
      CHECK_EQUAL(2U, **(itr++));
      CHECK_EQUAL(3U, **(itr++));
    }

    //*************************************************************************
    TEST(move_s_empty_source)
    {
      typedef std::vector<std::unique_ptr<unsigned>> Data;

      Data data1;
      Data data2(3);

      Data::iterator result = etl::move_s(data1.begin(), data1.end(), data2.begin(), data2.end());

      CHECK(data2.begin() == result);
    }

    //*************************************************************************
    TEST(move_s_empty_destination)
    {
      typedef std::vector<std::unique_ptr<unsigned>> Data;

      Data data1;
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(1U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(2U)));
      data1.push_back(std::unique_ptr<uint32_t>(new uint32_t(3U)));

      Data data2;

      Data::iterator result = etl::move_s(data1.begin(), data1.end(), data2.begin(), data2.end());

      CHECK(data2.begin() == result);
    }

    //*************************************************************************
    TEST(move_s_pod_random_iterator)
    {
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int data2[] = {1, 2, 3, 4, 5};

      int out1[10];
      int out2[5];

      int check1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int check2[] = {1, 2, 3, 4, 5};
      int check3[] = {1, 2, 3, 4, 5, 0, 0, 0, 0, 0};

      int* result;

      // Same size.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::move_s(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result = etl::move_s(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Source smaller.
      std::fill(std::begin(out1), std::end(out1), 0);
      result = etl::move_s(std::begin(data2), std::end(data2), std::begin(out1), std::end(out1));
      CHECK_EQUAL(std::begin(out1) + 5, result);
      is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(rotate_pod)
    {
      std::vector<int> initial_data = {1, 2, 3, 4, 5, 6, 7};

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
      std::vector<NDC> initial_data = {NDC(1), NDC(2), NDC(3), NDC(4), NDC(5), NDC(6), NDC(7)};

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
    TEST(rotate_return_value)
    {
      // Verify that etl::rotate returns the same iterator as std::rotate
      // in all cases, including the degenerate first==middle and middle==last
      // cases.
      std::vector<int> initial_data = {1, 2, 3, 4, 5};

      for (size_t i = 0UL; i <= initial_data.size(); ++i)
      {
        std::vector<int> data1(initial_data);
        std::vector<int> data2(initial_data);

        auto std_result = std::rotate(data1.data(), data1.data() + i, data1.data() + data1.size());
        auto etl_result = etl::rotate(data2.data(), data2.data() + i, data2.data() + data2.size());

        // Check that the return value offset matches
        ptrdiff_t std_offset = std_result - data1.data();
        ptrdiff_t etl_offset = etl_result - data2.data();
        CHECK_EQUAL(std_offset, etl_offset);
      }

      // Explicitly test first == middle (empty left half): should return last
      {
        std::vector<int> data   = {1, 2, 3};
        auto             result = etl::rotate(data.data(), data.data(), data.data() + data.size());
        CHECK(result == data.data() + data.size());
      }

      // Explicitly test middle == last (empty right half): should return first
      {
        std::vector<int> data   = {1, 2, 3};
        auto             result = etl::rotate(data.data(), data.data() + data.size(), data.data() + data.size());
        CHECK(result == data.data());
      }
    }

    //*************************************************************************
    TEST(rotate_return_value_non_random_iterator)
    {
      // Verify that etl::rotate returns the correct iterator when called with
      // non-random (bidirectional) iterators, exercising rotate_general for
      // bidirectional iterators rather than the random-access overload.
      std::vector<int> initial_data = {1, 2, 3, 4, 5};

      for (size_t i = 0UL; i <= initial_data.size(); ++i)
      {
        std::vector<int> data1(initial_data);
        std::vector<int> data2(initial_data);

        auto std_result = std::rotate(data1.data(), data1.data() + i, data1.data() + data1.size());

        non_random_iterator<int> nr_first(data2.data());
        non_random_iterator<int> nr_middle(data2.data() + i);
        non_random_iterator<int> nr_last(data2.data() + data2.size());
        auto                     etl_result = etl::rotate(nr_first, nr_middle, nr_last);

        // Check that the data was rotated correctly
        bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
        CHECK(isEqual);

        // Check that the return value offset matches
        ptrdiff_t std_offset = std_result - data1.data();
        ptrdiff_t etl_offset = etl_result.ptr - data2.data();
        CHECK_EQUAL(std_offset, etl_offset);
      }

      // Explicitly test first == middle (empty left half): should return last
      {
        std::vector<int>         data = {1, 2, 3};
        non_random_iterator<int> nr_first(data.data());
        non_random_iterator<int> nr_middle(data.data());
        non_random_iterator<int> nr_last(data.data() + data.size());
        auto                     result = etl::rotate(nr_first, nr_middle, nr_last);
        CHECK(result.ptr == data.data() + data.size());
      }

      // Explicitly test middle == last (empty right half): should return first
      {
        std::vector<int>         data = {1, 2, 3};
        non_random_iterator<int> nr_first(data.data());
        non_random_iterator<int> nr_middle(data.data() + data.size());
        non_random_iterator<int> nr_last(data.data() + data.size());
        auto                     result = etl::rotate(nr_first, nr_middle, nr_last);
        CHECK(result.ptr == data.data());
      }
    }

    //*************************************************************************
    TEST(any_of)
    {
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      bool expected = std::any_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      bool result   = etl::any_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);

      expected = std::any_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      result   = etl::any_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(all_of)
    {
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      bool expected = std::all_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      bool result   = etl::all_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 0));
      CHECK_EQUAL(expected, result);

      expected = std::all_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      result   = etl::all_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(none_of)
    {
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      bool expected = std::none_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 8));
      bool result   = etl::none_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 8));
      CHECK_EQUAL(expected, result);

      expected = std::none_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      result   = etl::none_of(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
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
      int data1[]           = {1, 2, 3, 4, 5, 6, 7, 8};
      int permutation[]     = {1, 3, 2, 4, 7, 6, 5, 8};
      int not_permutation[] = {1, 2, 3, 4, 5, 6, 7, 7};

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

      is_permutation =
        etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), std::end(not_permutation), etl::equal_to<int>());
      CHECK(!is_permutation);
    }

    //*************************************************************************
    TEST(is_permutation_different_lengths)
    {
      int shorter[] = {1, 2};
      int longer[]  = {1, 2, 3};

      // Four-iterator: range2 longer than range1 (extra elements only in
      // range2)
      bool result = etl::is_permutation(std::begin(shorter), std::end(shorter), std::begin(longer), std::end(longer));
      CHECK(!result);

      // Four-iterator: range1 longer than range2
      result = etl::is_permutation(std::begin(longer), std::end(longer), std::begin(shorter), std::end(shorter));
      CHECK(!result);

      // Four-iterator with predicate: range2 longer than range1
      result = etl::is_permutation(std::begin(shorter), std::end(shorter), std::begin(longer), std::end(longer), etl::equal_to<int>());
      CHECK(!result);

      // Four-iterator with predicate: range1 longer than range2
      result = etl::is_permutation(std::begin(longer), std::end(longer), std::begin(shorter), std::end(shorter), etl::equal_to<int>());
      CHECK(!result);
    }

    //*************************************************************************
    TEST(is_partitioned)
    {
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8};

      bool expected = std::is_partitioned(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      bool result   = etl::is_partitioned(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);

      std::partition(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));

      expected = std::is_partitioned(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      result   = etl::is_partitioned(std::begin(data1), std::end(data1), std::bind(std::greater<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(partition_point)
    {
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8};

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
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8};
      int data2[] = {0, 0, 0, 0, 0, 0, 0, 0};
      int data3[] = {0, 0, 0, 0, 0, 0, 0, 0};
      int data4[] = {0, 0, 0, 0, 0, 0, 0, 0};
      int data5[] = {0, 0, 0, 0, 0, 0, 0, 0};

      std::partition_copy(std::begin(data1), std::end(data1), std::begin(data2), std::begin(data3),
                          std::bind(std::greater<int>(), std::placeholders::_1, 4));
      etl::partition_copy(std::begin(data1), std::end(data1), std::begin(data4), std::begin(data5),
                          std::bind(std::greater<int>(), std::placeholders::_1, 4));

      bool are_equal;

      are_equal = std::equal(std::begin(data2), std::end(data2), std::begin(data4));
      CHECK(are_equal);

      are_equal = std::equal(std::begin(data3), std::end(data3), std::begin(data5));
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(find_if_not)
    {
      int data1[] = {1, 2, 3, 5, 6, 7, 8};

      // Find the element not less than 4.
      int* p = etl::find_if_not(std::begin(data1), std::end(data1), std::bind(std::less<int>(), std::placeholders::_1, 4));
      CHECK_EQUAL(5, *p);
    }

    //*************************************************************************
    TEST(copy_4_parameter_random_iterator)
    {
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int data2[] = {1, 2, 3, 4, 5};

      int out1[10];
      int out2[5];

      int check1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int check2[] = {1, 2, 3, 4, 5};
      int check3[] = {1, 2, 3, 4, 5, 0, 0, 0, 0, 0};

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
      std::list<int> data1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::list<int> data2 = {1, 2, 3, 4, 5};

      int out1[10];
      int out2[5];

      int check1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int check2[] = {1, 2, 3, 4, 5};
      int check3[] = {1, 2, 3, 4, 5, 0, 0, 0, 0, 0};

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
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      int out1[10];
      int out2[5];

      int check1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int check2[] = {1, 2, 3, 4, 5};
      int check3[] = {1, 2, 3, 4, 5, 0, 0, 0, 0, 0};

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
      int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      int out1[10];
      int out2[5];

      int check1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int check2[] = {1, 2, 3, 4, 5};
      int check3[] = {1, 2, 3, 4, 5, 0, 0, 0, 0, 0};

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
      int data1[] = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int data2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int data3[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

      // Copy everything less than 5.
      int* pout = data2;
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
      int data1[] = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};

      int out1[4];
      int out2[2];
      int out3[10];

      int check1[] = {1, 2, 3, 4};
      int check2[] = {1, 2};
      int check3[] = {1, 2, 3, 4, 0, 0, 0, 0, 0, 0};

      int* result;

      // Exact size.
      std::fill(std::begin(out1), std::end(out1), 0);
      result =
        etl::copy_if_s(std::begin(data1), std::end(data1), std::begin(out1), std::end(out1), std::bind(std::less<int>(), std::placeholders::_1, 5));
      CHECK_EQUAL(std::end(out1), result);
      bool is_same = std::equal(std::begin(out1), std::end(out1), std::begin(check1));
      CHECK(is_same);

      // Destination smaller.
      std::fill(std::begin(out2), std::end(out2), 0);
      result =
        etl::copy_if_s(std::begin(data1), std::end(data1), std::begin(out2), std::end(out2), std::bind(std::less<int>(), std::placeholders::_1, 5));
      CHECK_EQUAL(std::end(out2), result);
      is_same = std::equal(std::begin(out2), std::end(out2), std::begin(check2));
      CHECK(is_same);

      // Destination larger.
      std::fill(std::begin(out3), std::end(out3), 0);
      result =
        etl::copy_if_s(std::begin(data1), std::end(data1), std::begin(out3), std::end(out3), std::bind(std::less<int>(), std::placeholders::_1, 5));
      CHECK_EQUAL(std::begin(out3) + 4, result);
      is_same = std::equal(std::begin(out3), std::end(out3), std::begin(check3));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(binary_find)
    {
      int data1[] = {1, 2, 3, 5, 6, 7, 8};

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
      Data data1[] = {{1, 8}, {2, 7}, {3, 6}, {4, 5}, {5, 4}, {6, 3}, {7, 2}, {8, 1}};
      Data test1   = {4, 5};
      Data test2   = {9, 0};

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
      int data1[] = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};

      struct Sum
      {
        Sum()
          : sum(0)
        {
        }

        Sum& operator()(int i)
        {
          sum += i;

          return *this;
        }

        int sum;
      } accumulator;

      // For each if everything less than 5.
      accumulator = etl::for_each_if(std::begin(data1), std::end(data1), accumulator, std::bind(std::less<int>(), std::placeholders::_1, 5));

      CHECK_EQUAL(10, accumulator.sum);
    }

    //*************************************************************************
    TEST(for_each_n)
    {
      int data1[] = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int data2[] = {2, 16, 4, 14, 6, 6, 4, 5, 10, 9};

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
      int data1[] = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int data2[] = {2, 8, 4, 7, 6, 6, 4, 5, 10, 9};

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
      int input[]   = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare[] = {2, 16, 4, 14, 6, 0, 0, 0, 0, 0};

      // Double everything and copy to output.
      etl::transform_s(std::begin(input), std::end(input), std::begin(output), std::begin(output) + (ETL_OR_STD17::size(output) / 2),
                       std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);

      std::fill(std::begin(output), std::end(output), 0);

      etl::transform_s(std::begin(input), std::begin(input) + (ETL_OR_STD17::size(input) / 2), std::begin(output), std::end(output),
                       std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_random_iterator)
    {
      int input[]   = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare[] = {2, 16, 4, 14, 6, 12, 8, 0, 0, 0};

      etl::transform_n(std::begin(input), 7, std::begin(output), std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_non_random_iterator)
    {
      std::list<int> input     = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int            output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int            compare[] = {2, 16, 4, 14, 6, 12, 8, 0, 0, 0};

      etl::transform_n(std::begin(input), 7, std::begin(output), std::bind(std::multiplies<int>(), std::placeholders::_1, 2));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_two_ranges_random_iterator)
    {
      int input1[]  = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int input2[]  = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare[] = {2, 16, 4, 14, 6, 12, 8, 0, 0, 0};

      etl::transform_n(std::begin(input1), std::begin(input2), 7, std::begin(output), std::plus<int>());

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_two_ranges_non_random_iterator)
    {
      std::list<int> input1    = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      std::list<int> input2    = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int            output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int            compare[] = {2, 16, 4, 14, 6, 12, 8, 0, 0, 0};

      etl::transform_n(std::begin(input1), std::begin(input2), 7, std::begin(output), std::plus<int>());

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_if)
    {
      int input[]   = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare[] = {2, 4, 6, 8, 0, 0, 0, 0, 0, 0};

      // Double everything less than 5 and copy to output.
      etl::transform_if(std::begin(input), std::end(input), std::begin(output), std::bind(std::multiplies<int>(), std::placeholders::_1, 2),
                        std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_if_2_input_ranges)
    {
      int input1[]  = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int input2[]  = {8, 7, 6, 5, 4, 10, 9, 3, 2, 1};
      int output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare[] = {8, 12, 12, 60, 36, 0, 0, 0, 0, 0};

      // Multiply together everything where input1 is less than input2 and copy
      // to output.
      etl::transform_if(std::begin(input1), std::end(input1), std::begin(input2), std::begin(output), std::multiplies<int>(), std::less<int>());

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_if)
    {
      int input[]   = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare[] = {2, 4, 6, 0, 0, 0, 0, 0, 0, 0};

      // Double everything less than 5 and copy to output.
      etl::transform_n_if(std::begin(input), 5, std::begin(output), std::bind(std::multiplies<int>(), std::placeholders::_1, 2),
                          std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(transform_n_if_2_input_ranges)
    {
      int input1[]  = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int input2[]  = {8, 7, 6, 5, 4, 10, 9, 3, 2, 1};
      int output[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare[] = {8, 12, 12, 0, 0, 0, 0, 0, 0, 0};

      // Multiply together everything where input1 is less than input2 and copy
      // to output.
      etl::transform_n_if(std::begin(input1), std::begin(input2), 5, std::begin(output), std::multiplies<int>(), std::less<int>());

      bool is_same = std::equal(std::begin(output), std::end(output), std::begin(compare));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(partition_transform)
    {
      int input[]         = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int output_true[]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int output_false[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare_true[]  = {2, 4, 6, 8, 0, 0, 0, 0, 0, 0};
      int compare_false[] = {-16, -14, -12, -10, -20, -18, 0, 0, 0, 0};

      // Multiply everything less than 5 by 2 and copy to output_true.
      // Multiply everything not less than 5 by -2 and copy to output_false.
      etl::partition_transform(std::begin(input), std::end(input), std::begin(output_true), std::begin(output_false),
                               std::bind(std::multiplies<int>(), std::placeholders::_1, 2),
                               std::bind(std::multiplies<int>(), std::placeholders::_1, -2), std::bind(std::less<int>(), std::placeholders::_1, 5));

      bool is_same = std::equal(std::begin(output_true), std::end(output_true), std::begin(compare_true));
      CHECK(is_same);

      is_same = std::equal(std::begin(output_false), std::end(output_false), std::begin(compare_false));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(partition_transform_2_input_ranges)
    {
      int input1[]        = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int input2[]        = {8, 7, 6, 5, 4, 10, 9, 3, 2, 1};
      int output_true[]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int output_false[]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      int compare_true[]  = {8, 12, 12, 60, 36, 0, 0, 0, 0, 0};
      int compare_false[] = {15, 12, 8, 12, 10, 0, 0, 0, 0, 0};

      // If input1 < input2 multiply else add.
      etl::partition_transform(std::begin(input1), std::end(input1), std::begin(input2), std::begin(output_true), std::begin(output_false),
                               std::multiplies<int>(), std::plus<int>(), std::less<int>());

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
      std::vector<NDC> initial_data = {NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1)};

      std::vector<NDC> data1(initial_data);
      std::vector<NDC> data2(initial_data);

      std::stable_sort(data1.begin(), data1.end());
      etl::stable_sort(data2.begin(), data2.end());

      bool is_same = std::equal(data1.begin(), data1.end(), data2.begin(), NDC::are_identical);
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(next_permutation)
    {
      std::array<int, 4U> expected = {1, 1, 2, 2};
      std::array<int, 4U> result   = expected;

      for (size_t i = 0U; i < 8U; ++i)
      {
        bool expected_has_next = std::next_permutation(expected.begin(), expected.end());
        bool result_has_next   = etl::next_permutation(result.begin(), result.end());

        CHECK_EQUAL(expected_has_next, result_has_next);
        CHECK_ARRAY_EQUAL(expected.data(), result.data(), result.size());
      }

      // Check one past the end.
      bool expected_has_next = std::next_permutation(expected.begin(), expected.end());
      bool result_has_next   = etl::next_permutation(result.begin(), result.end());
      CHECK_EQUAL(expected_has_next, result_has_next);
      CHECK_ARRAY_EQUAL(expected.data(), result.data(), result.size());

      int single_expected[] = {1};
      int single_result[]   = {1};

      expected_has_next = std::next_permutation(std::begin(single_expected), std::end(single_expected));
      result_has_next   = etl::next_permutation(std::begin(single_result), std::end(single_result));

      CHECK_EQUAL(expected_has_next, result_has_next);
      CHECK_ARRAY_EQUAL(single_expected, single_result, 1U);

      // Check for what happens if the beginning and end are the same.
      expected_has_next = std::next_permutation(std::begin(single_expected), std::begin(single_expected));
      result_has_next   = etl::next_permutation(std::begin(single_result), std::begin(single_result));

      CHECK_EQUAL(expected_has_next, result_has_next);
    }

    //*************************************************************************
    TEST(next_permutation_compare)
    {
      std::array<int, 4U> expected = {3, 2, 2, 1};
      std::array<int, 4U> result   = expected;

      for (size_t i = 0U; i < 8U; ++i)
      {
        bool expected_has_next = std::next_permutation(expected.begin(), expected.end(), std::greater<int>());
        bool result_has_next   = etl::next_permutation(result.begin(), result.end(), std::greater<int>());

        CHECK_EQUAL(expected_has_next, result_has_next);
        CHECK_ARRAY_EQUAL(expected.data(), result.data(), result.size());
      }

      // Check one past the end.
      bool expected_has_next = std::next_permutation(expected.begin(), expected.end(), std::greater<int>());
      bool result_has_next   = etl::next_permutation(result.begin(), result.end(), std::greater<int>());
      CHECK_EQUAL(expected_has_next, result_has_next);
      CHECK_ARRAY_EQUAL(expected.data(), result.data(), result.size());

      int single_expected[] = {1};
      int single_result[]   = {1};

      // Check for what happens if the beginning and end are the same.
      expected_has_next = std::next_permutation(std::begin(single_expected), std::begin(single_expected), std::greater<int>());
      result_has_next   = etl::next_permutation(std::begin(single_result), std::begin(single_result), std::greater<int>());

      CHECK_EQUAL(expected_has_next, result_has_next);
      CHECK_ARRAY_EQUAL(single_expected, single_result, 1U);
    }

    //*************************************************************************
    TEST(prev_permutation)
    {
      std::array<int, 4U> expected = {2, 2, 1, 1};
      std::array<int, 4U> result   = expected;

      for (size_t i = 0U; i < 8U; ++i)
      {
        bool expected_has_prev = std::prev_permutation(expected.begin(), expected.end());
        bool result_has_prev   = etl::prev_permutation(result.begin(), result.end());

        CHECK_EQUAL(expected_has_prev, result_has_prev);
        CHECK_ARRAY_EQUAL(expected.data(), result.data(), result.size());
      }

      // Check one past the end.
      bool expected_has_prev = std::prev_permutation(expected.begin(), expected.end());
      bool result_has_prev   = etl::prev_permutation(result.begin(), result.end());
      CHECK_EQUAL(expected_has_prev, result_has_prev);
      CHECK_ARRAY_EQUAL(expected.data(), result.data(), result.size());

      int single_expected[] = {1};
      int single_result[]   = {1};

      expected_has_prev = std::prev_permutation(std::begin(single_expected), std::end(single_expected));
      result_has_prev   = etl::prev_permutation(std::begin(single_result), std::end(single_result));

      CHECK_EQUAL(expected_has_prev, result_has_prev);
      CHECK_ARRAY_EQUAL(single_expected, single_result, 1U);

      // Check for what happens if the beginning and end are the same.
      expected_has_prev = std::prev_permutation(std::begin(single_expected), std::begin(single_expected));
      result_has_prev   = etl::prev_permutation(std::begin(single_result), std::begin(single_result));

      CHECK_EQUAL(expected_has_prev, result_has_prev);
    }

    //*************************************************************************
    TEST(prev_permutation_compare)
    {
      std::array<int, 4U> expected = {1, 1, 2, 3};
      std::array<int, 4U> result   = expected;

      for (size_t i = 0U; i < 8U; ++i)
      {
        bool expected_has_prev = std::prev_permutation(expected.begin(), expected.end(), std::greater<int>());
        bool result_has_prev   = etl::prev_permutation(result.begin(), result.end(), std::greater<int>());

        CHECK_EQUAL(expected_has_prev, result_has_prev);
        CHECK_ARRAY_EQUAL(expected.data(), result.data(), result.size());
      }

      // Check one past the end.
      bool expected_has_prev = std::prev_permutation(expected.begin(), expected.end(), std::greater<int>());
      bool result_has_prev   = etl::prev_permutation(result.begin(), result.end(), std::greater<int>());
      CHECK_EQUAL(expected_has_prev, result_has_prev);
      CHECK_ARRAY_EQUAL(expected.data(), result.data(), result.size());

      int single_expected[] = {1};
      int single_result[]   = {1};

      // Check for what happens if the beginning and end are the same.
      expected_has_prev = std::prev_permutation(std::begin(single_expected), std::begin(single_expected), std::greater<int>());
      result_has_prev   = etl::prev_permutation(std::begin(single_result), std::begin(single_result), std::greater<int>());

      CHECK_EQUAL(expected_has_prev, result_has_prev);
      CHECK_ARRAY_EQUAL(single_expected, single_result, 1U);
    }

    //*************************************************************************
    TEST(is_permutation_length_mismatch)
    {
      int data1[] = {1, 2, 3};
      int data2[] = {1, 2, 3, 4};

      bool is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(data2), std::end(data2));
      CHECK_FALSE(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(data2), std::end(data2), etl::equal_to<int>());
      CHECK_FALSE(is_permutation);
    }

    //*************************************************************************
    TEST(is_permutation_predicate)
    {
      Data data1[]           = {Data(1, 10), Data(2, 20), Data(2, 30), Data(3, 40)};
      Data permutation[]     = {Data(2, 200), Data(1, 100), Data(3, 300), Data(2, 400)};
      Data not_permutation[] = {Data(2, 200), Data(1, 100), Data(4, 300), Data(2, 400)};

      bool is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation), DataEquivalenceByA());
      CHECK_TRUE(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), DataEquivalenceByA());
      CHECK_FALSE(is_permutation);

      is_permutation = etl::is_permutation(std::begin(data1), std::end(data1), std::begin(permutation), std::end(permutation), DataEquivalenceByA());
      CHECK_TRUE(is_permutation);

      is_permutation =
        etl::is_permutation(std::begin(data1), std::end(data1), std::begin(not_permutation), std::end(not_permutation), DataEquivalenceByA());
      CHECK_FALSE(is_permutation);
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
      std::vector<NDC> initial_data = {NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1)};

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
      std::vector<NDC> initial_data = {NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1)};

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
    TEST(selection_sort_empty_range)
    {
      // Forward iterators
      std::forward_list<int> fwd_data;
      etl::selection_sort(fwd_data.begin(), fwd_data.end());
      CHECK(fwd_data.empty());

      // Bidirectional iterators
      std::list<int> bidir_data;
      etl::selection_sort(bidir_data.begin(), bidir_data.end());
      CHECK(bidir_data.empty());

      // Random access iterators
      std::vector<int> ra_data;
      etl::selection_sort(ra_data.begin(), ra_data.end());
      CHECK(ra_data.empty());

      // With comparator
      std::forward_list<int> fwd_data2;
      etl::selection_sort(fwd_data2.begin(), fwd_data2.end(), std::greater<int>());
      CHECK(fwd_data2.empty());
    }

    //*************************************************************************
    TEST(heap_sort_default)
    {
      std::vector<NDC> initial_data = {NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1)};

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
      std::vector<NDC> initial_data = {NDC(1, 1), NDC(2, 1), NDC(3, 1), NDC(2, 2), NDC(3, 2), NDC(4, 1), NDC(2, 3), NDC(3, 3), NDC(5, 1)};

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

      int temp[etl::multimax(1, 2, 3, 4, 5, 6, 7, 8)] = {1, 2, 3, 4, 5, 6, 7, 8};
      (void)temp;
    }

    //*************************************************************************
    TEST(multimax_iter)
    {
      int i[8] = {1, 2, 3, 4, 5, 6, 7, 8};

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
      int i[8] = {1, 2, 3, 4, 5, 6, 7, 8};

      CHECK_EQUAL(1, *etl::multimin_iter(&i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
      CHECK_EQUAL(1, *etl::multimin_iter_compare(std::less<int>(), &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
      CHECK_EQUAL(8, *etl::multimin_iter_compare(std::greater<int>(), &i[0], &i[1], &i[2], &i[3], &i[4], &i[5], &i[6], &i[7]));
    }

    //*************************************************************************
    TEST(replace)
    {
      int data[]     = {1, 8, 2, 7, 2, 6, 2, 2, 10, 9};
      int expected[] = {1, 8, 0, 7, 0, 6, 0, 0, 10, 9};

      // Replace 2 with 0
      etl::replace(std::begin(data), std::end(data), 2, 0);

      bool is_same = std::equal(std::begin(data), std::end(data), std::begin(expected));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(replace_if)
    {
      int data[]     = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};
      int expected[] = {0, 8, 0, 7, 0, 6, 0, 0, 10, 9};

      // Replace <=5 with 0
      etl::replace_if(std::begin(data), std::end(data), std::bind(std::less_equal<int>(), std::placeholders::_1, 5), 0);

      bool is_same = std::equal(std::begin(data), std::end(data), std::begin(expected));
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(for_each)
    {
      int data[] = {1, 8, 2, 7, 3, 6, 4, 5, 10, 9};

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
      std::array<int, 10> data     = {1, 8, 2, 7, 7, 7, 4, 5, 10, 9};
      std::array<int, 7>  expected = {1, 8, 2, 4, 5, 10, 9};

      etl::remove(data.begin(), data.end(), 7);

      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(remove_if)
    {
      std::array<int, 10> data     = {1, 8, 2, 7, 7, 7, 4, 5, 10, 9};
      std::array<int, 4>  expected = {1, 2, 4, 5};

      etl::remove_if(data.begin(), data.end(), [](int value) { return value >= 7; });

      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique)
    {
      std::array<int, 10> data     = {1, 1, 2, 3, 3, 3, 4, 4, 5, 5};
      std::array<int, 5>  expected = {1, 2, 3, 4, 5};

      auto end = etl::unique(data.begin(), data.end());

      CHECK_EQUAL(5, std::distance(data.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_empty_range)
    {
      std::array<int, 0> data = {};

      auto end = etl::unique(data.begin(), data.end());

      CHECK(end == data.end());
    }

    //*************************************************************************
    TEST(unique_single_element)
    {
      std::array<int, 1> data     = {42};
      std::array<int, 1> expected = {42};

      auto end = etl::unique(data.begin(), data.end());

      CHECK_EQUAL(1, std::distance(data.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_no_duplicates)
    {
      std::array<int, 5> data     = {1, 2, 3, 4, 5};
      std::array<int, 5> expected = {1, 2, 3, 4, 5};

      auto end = etl::unique(data.begin(), data.end());

      CHECK_EQUAL(5, std::distance(data.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_all_same)
    {
      std::array<int, 5> data     = {7, 7, 7, 7, 7};
      std::array<int, 1> expected = {7};

      auto end = etl::unique(data.begin(), data.end());

      CHECK_EQUAL(1, std::distance(data.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_with_predicate)
    {
      std::array<int, 10> data     = {1, 1, 2, 3, 3, 3, 4, 4, 5, 5};
      std::array<int, 5>  expected = {1, 2, 3, 4, 5};

      auto end = etl::unique(data.begin(), data.end(), std::equal_to<int>());

      CHECK_EQUAL(5, std::distance(data.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_with_predicate_custom)
    {
      // Group elements that are close to each other (differ by less than 3)
      std::array<int, 8> data     = {1, 2, 3, 7, 8, 9, 20, 21};
      std::array<int, 3> expected = {1, 7, 20};

      auto end = etl::unique(data.begin(), data.end(), [](int a, int b) { return (b - a) < 3; });

      CHECK_EQUAL(3, std::distance(data.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_matches_std)
    {
      std::array<int, 12> data1 = {1, 1, 2, 2, 2, 3, 4, 4, 5, 5, 5, 5};
      std::array<int, 12> data2 = data1;

      auto std_end = std::unique(data1.begin(), data1.end());
      auto etl_end = etl::unique(data2.begin(), data2.end());

      size_t std_size = static_cast<size_t>(std::distance(data1.begin(), std_end));
      size_t etl_size = static_cast<size_t>(std::distance(data2.begin(), etl_end));

      CHECK_EQUAL(std_size, etl_size);
      bool is_same = std::equal(data1.begin(), std_end, data2.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_with_predicate_matches_std)
    {
      std::array<int, 12> data1 = {1, 1, 2, 2, 2, 3, 4, 4, 5, 5, 5, 5};
      std::array<int, 12> data2 = data1;

      auto std_end = std::unique(data1.begin(), data1.end(), std::equal_to<int>());
      auto etl_end = etl::unique(data2.begin(), data2.end(), std::equal_to<int>());

      size_t std_size = static_cast<size_t>(std::distance(data1.begin(), std_end));
      size_t etl_size = static_cast<size_t>(std::distance(data2.begin(), etl_end));

      CHECK_EQUAL(std_size, etl_size);
      bool is_same = std::equal(data1.begin(), std_end, data2.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy)
    {
      std::array<int, 10> data     = {1, 1, 2, 3, 3, 3, 4, 4, 5, 5};
      std::array<int, 5>  expected = {1, 2, 3, 4, 5};
      std::array<int, 10> result   = {};

      auto end = etl::unique_copy(data.begin(), data.end(), result.begin());

      CHECK_EQUAL(5, std::distance(result.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), result.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy_empty_range)
    {
      std::array<int, 0> data   = {};
      std::array<int, 1> result = {99};

      auto end = etl::unique_copy(data.begin(), data.end(), result.begin());

      CHECK(end == result.begin());
      CHECK_EQUAL(99, result[0]); // output unchanged
    }

    //*************************************************************************
    TEST(unique_copy_single_element)
    {
      std::array<int, 1> data     = {42};
      std::array<int, 1> expected = {42};
      std::array<int, 1> result   = {};

      auto end = etl::unique_copy(data.begin(), data.end(), result.begin());

      CHECK_EQUAL(1, std::distance(result.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), result.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy_no_duplicates)
    {
      std::array<int, 5> data     = {1, 2, 3, 4, 5};
      std::array<int, 5> expected = {1, 2, 3, 4, 5};
      std::array<int, 5> result   = {};

      auto end = etl::unique_copy(data.begin(), data.end(), result.begin());

      CHECK_EQUAL(5, std::distance(result.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), result.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy_all_same)
    {
      std::array<int, 5> data     = {7, 7, 7, 7, 7};
      std::array<int, 1> expected = {7};
      std::array<int, 5> result   = {};

      auto end = etl::unique_copy(data.begin(), data.end(), result.begin());

      CHECK_EQUAL(1, std::distance(result.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), result.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy_source_unchanged)
    {
      std::array<int, 10> data     = {1, 1, 2, 3, 3, 3, 4, 4, 5, 5};
      std::array<int, 10> original = data;
      std::array<int, 10> result   = {};

      etl::unique_copy(data.begin(), data.end(), result.begin());

      bool is_same = std::equal(original.begin(), original.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy_with_predicate)
    {
      std::array<int, 10> data     = {1, 1, 2, 3, 3, 3, 4, 4, 5, 5};
      std::array<int, 5>  expected = {1, 2, 3, 4, 5};
      std::array<int, 10> result   = {};

      auto end = etl::unique_copy(data.begin(), data.end(), result.begin(), std::equal_to<int>());

      CHECK_EQUAL(5, std::distance(result.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), result.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy_with_predicate_custom)
    {
      // Group elements that are close to each other (differ by less than 3)
      std::array<int, 8> data     = {1, 2, 3, 7, 8, 9, 20, 21};
      std::array<int, 3> expected = {1, 7, 20};
      std::array<int, 8> result   = {};

      auto end = etl::unique_copy(data.begin(), data.end(), result.begin(), [](int a, int b) { return (b - a) < 3; });

      CHECK_EQUAL(3, std::distance(result.begin(), end));
      bool is_same = std::equal(expected.begin(), expected.end(), result.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy_matches_std)
    {
      std::array<int, 12> data       = {1, 1, 2, 2, 2, 3, 4, 4, 5, 5, 5, 5};
      std::array<int, 12> std_result = {};
      std::array<int, 12> etl_result = {};

      auto std_end = std::unique_copy(data.begin(), data.end(), std_result.begin());
      auto etl_end = etl::unique_copy(data.begin(), data.end(), etl_result.begin());

      size_t std_size = static_cast<size_t>(std::distance(std_result.begin(), std_end));
      size_t etl_size = static_cast<size_t>(std::distance(etl_result.begin(), etl_end));

      CHECK_EQUAL(std_size, etl_size);
      bool is_same = std::equal(std_result.begin(), std_end, etl_result.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(unique_copy_with_predicate_matches_std)
    {
      std::array<int, 12> data       = {1, 1, 2, 2, 2, 3, 4, 4, 5, 5, 5, 5};
      std::array<int, 12> std_result = {};
      std::array<int, 12> etl_result = {};

      auto std_end = std::unique_copy(data.begin(), data.end(), std_result.begin(), std::equal_to<int>());
      auto etl_end = etl::unique_copy(data.begin(), data.end(), etl_result.begin(), std::equal_to<int>());

      size_t std_size = static_cast<size_t>(std::distance(std_result.begin(), std_end));
      size_t etl_size = static_cast<size_t>(std::distance(etl_result.begin(), etl_end));

      CHECK_EQUAL(std_size, etl_size);
      bool is_same = std::equal(std_result.begin(), std_end, etl_result.begin());
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
      std::array<int, 10> expected = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
      std::array<int, 10> actual;

      etl::generate(actual.begin(), actual.end(), generator(2));

      CHECK_ARRAY_EQUAL(expected.data(), actual.data(), expected.size());
    }

    //*************************************************************************
    TEST(partition_forward_iterator_container)
    {
      // 40,320 permutations.
      std::array<int, 8> origin = {0, 1, 2, 3, 4, 5, 6, 7};

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
      std::array<int, 8> initial = {0, 1, 2, 3, 4, 5, 6, 7};

      std::array<int, 8> compare = initial;
      std::array<int, 8> data    = initial;

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
        data    = initial;
      }
    }

    //*************************************************************************
    TEST(nth_element_with_default_less_than_comparison)
    {
      // 40,320 permutations.
      std::array<int, 8> initial = {0, 1, 2, 3, 4, 5, 6, 7};

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
      std::array<int, 8> data = {5, 1, 3, 7, 6, 2, 4, 0};

      etl::nth_element(data.begin(), data.begin() + nth_index, data.end());

      return data[static_cast<size_t>(nth_index)];
    }

    TEST(constexpr_nth_element_with_default_less_than_comparison)
    {
      std::array<int, 8> compare = {0, 1, 2, 3, 4, 5, 6, 7};

      constexpr int nth = MakeNth(3);

      CHECK_EQUAL(compare[3], nth);
    }
#endif

    //*************************************************************************
    TEST(nth_element_with_custom_comparison)
    {
      // 40,320 permutations.
      std::array<int, 8> initial = {0, 1, 2, 3, 4, 5, 6, 7};

      std::array<int, 8> compare = initial;
      std::array<int, 8> data    = initial;

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
        data    = initial;
      }
    }

    //*************************************************************************
    TEST(accumulate_default)
    {
      int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      int expected = std::accumulate(std::begin(data), std::end(data), 0);
      int result   = etl::accumulate(std::begin(data), std::end(data), 0);

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_with_initial_value)
    {
      int data[] = {1, 2, 3, 4, 5};

      int expected = std::accumulate(std::begin(data), std::end(data), 100);
      int result   = etl::accumulate(std::begin(data), std::end(data), 100);

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_custom_operation)
    {
      int data[] = {1, 2, 3, 4, 5};

      int expected = std::accumulate(std::begin(data), std::end(data), 1, std::multiplies<int>());
      int result   = etl::accumulate(std::begin(data), std::end(data), 1, std::multiplies<int>());

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_empty_range)
    {
      int data[] = {1};

      int expected = std::accumulate(std::begin(data), std::begin(data), 42);
      int result   = etl::accumulate(std::begin(data), std::begin(data), 42);

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_single_element)
    {
      int data[] = {7};

      int expected = std::accumulate(std::begin(data), std::end(data), 0);
      int result   = etl::accumulate(std::begin(data), std::end(data), 0);

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_negative_values)
    {
      int data[] = {-3, -2, -1, 0, 1, 2, 3};

      int expected = std::accumulate(std::begin(data), std::end(data), 0);
      int result   = etl::accumulate(std::begin(data), std::end(data), 0);

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_custom_operation_subtraction)
    {
      int data[] = {1, 2, 3, 4, 5};

      int expected = std::accumulate(std::begin(data), std::end(data), 100, std::minus<int>());
      int result   = etl::accumulate(std::begin(data), std::end(data), 100, std::minus<int>());

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_non_random_iterator)
    {
      List data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      int expected = std::accumulate(data.begin(), data.end(), 0);
      int result   = etl::accumulate(data.begin(), data.end(), 0);

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_non_random_iterator_custom_operation)
    {
      List data = {1, 2, 3, 4, 5};

      int expected = std::accumulate(data.begin(), data.end(), 1, std::multiplies<int>());
      int result   = etl::accumulate(data.begin(), data.end(), 1, std::multiplies<int>());

      CHECK_EQUAL(expected, result);
    }

    //*************************************************************************
    TEST(accumulate_double)
    {
      double data[] = {1.5, 2.5, 3.5, 4.5, 5.5};

      double expected = std::accumulate(std::begin(data), std::end(data), 0.0);
      double result   = etl::accumulate(std::begin(data), std::end(data), 0.0);

      CHECK_CLOSE(expected, result, 1e-10);
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
      CHECK_EQUAL(5, (etl::clamp<int, 0, 10>(5)));
      CHECK_EQUAL(0, (etl::clamp<int, 0, 10>(-5)));
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

      CHECK_EQUAL(5, result1);
      CHECK_EQUAL(0, result2);
      CHECK_EQUAL(10, result3);

      CHECK_EQUAL(5, result4);
      CHECK_EQUAL(0, result5);
      CHECK_EQUAL(10, result6);
    }

    //*************************************************************************
    TEST(merge_default_comparator)
    {
      int input1[] = {1, 3, 5, 7, 9};
      int input2[] = {2, 4, 6, 8, 10};
      int output[10];
      int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      int* result = etl::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(output));

      CHECK_EQUAL(std::end(output), result);
      CHECK_ARRAY_EQUAL(expected, output, 10);
    }

    //*************************************************************************
    TEST(merge_custom_comparator)
    {
      int input1[] = {9, 7, 5, 3, 1};
      int input2[] = {10, 8, 6, 4, 2};
      int output[10];
      int expected[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      int* result = etl::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(output), Greater());

      CHECK_EQUAL(std::end(output), result);
      CHECK_ARRAY_EQUAL(expected, output, 10);
    }

    //*************************************************************************
    TEST(merge_first_range_empty)
    {
      int input1[] = {0}; // dummy, won't be used
      int input2[] = {1, 2, 3};
      int output[3];
      int expected[] = {1, 2, 3};

      int* result = etl::merge(input1, input1, // empty range
                               std::begin(input2), std::end(input2), std::begin(output));

      CHECK_EQUAL(std::end(output), result);
      CHECK_ARRAY_EQUAL(expected, output, 3);
    }

    //*************************************************************************
    TEST(merge_second_range_empty)
    {
      int input1[] = {1, 2, 3};
      int input2[] = {0}; // dummy, won't be used
      int output[3];
      int expected[] = {1, 2, 3};

      int* result = etl::merge(std::begin(input1), std::end(input1), input2,
                               input2, // empty range
                               std::begin(output));

      CHECK_EQUAL(std::end(output), result);
      CHECK_ARRAY_EQUAL(expected, output, 3);
    }

    //*************************************************************************
    TEST(merge_both_ranges_empty)
    {
      int input1[] = {0};
      int output[] = {99};

      int* result = etl::merge(input1, input1, input1, input1, std::begin(output));

      CHECK_EQUAL(std::begin(output), result);
      CHECK_EQUAL(99, output[0]); // output should be unchanged
    }

    //*************************************************************************
    TEST(merge_with_duplicates)
    {
      int input1[] = {1, 3, 3, 5};
      int input2[] = {2, 3, 4, 5};
      int output[8];
      int expected[] = {1, 2, 3, 3, 3, 4, 5, 5};

      int* result = etl::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(output));

      CHECK_EQUAL(std::end(output), result);
      CHECK_ARRAY_EQUAL(expected, output, 8);
    }

    //*************************************************************************
    TEST(merge_different_sizes)
    {
      int input1[] = {1, 5};
      int input2[] = {2, 3, 4, 6, 7, 8};
      int output[8];
      int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};

      int* result = etl::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(output));

      CHECK_EQUAL(std::end(output), result);
      CHECK_ARRAY_EQUAL(expected, output, 8);
    }

    //*************************************************************************
    TEST(merge_single_elements)
    {
      int input1[] = {1};
      int input2[] = {2};
      int output[2];
      int expected[] = {1, 2};

      int* result = etl::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(output));

      CHECK_EQUAL(std::end(output), result);
      CHECK_ARRAY_EQUAL(expected, output, 2);
    }

    //*************************************************************************
    TEST(merge_with_list_iterators)
    {
      std::list<int>   input1 = {1, 3, 5, 7};
      std::list<int>   input2 = {2, 4, 6, 8};
      std::vector<int> output(8);
      std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8};

      std::vector<int>::iterator result = etl::merge(input1.begin(), input1.end(), input2.begin(), input2.end(), output.begin());

      CHECK(output.end() == result);
      CHECK_ARRAY_EQUAL(expected.data(), output.data(), 8);
    }

    //*************************************************************************
    TEST(merge_matches_std)
    {
      int input1[] = {1, 4, 7, 8, 10};
      int input2[] = {2, 3, 5, 6, 9};
      int etl_output[10];
      int std_output[10];

      etl::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(etl_output));

      std::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(std_output));

      CHECK_ARRAY_EQUAL(std_output, etl_output, 10);
    }

    //*************************************************************************
    TEST(merge_matches_std_with_comparator)
    {
      int input1[] = {10, 8, 7, 4, 1};
      int input2[] = {9, 6, 5, 3, 2};
      int etl_output[10];
      int std_output[10];

      etl::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(etl_output), Greater());

      std::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(std_output), Greater());

      CHECK_ARRAY_EQUAL(std_output, etl_output, 10);
    }

    //*************************************************************************
    TEST(merge_stability)
    {
      // Test that merge is stable: equivalent elements from the first range
      // come before those from the second range.
      Data input1[] = {Data(1, 1), Data(3, 1), Data(5, 1)};
      Data input2[] = {Data(1, 2), Data(3, 2), Data(5, 2)};
      Data output[6];

      etl::merge(std::begin(input1), std::end(input1), std::begin(input2), std::end(input2), std::begin(output), DataPredicate());

      // Elements from input1 (b==1) should come before elements from input2
      // (b==2) for equivalent keys.
      CHECK_EQUAL(1, output[0].a);
      CHECK_EQUAL(1, output[0].b); // from input1
      CHECK_EQUAL(1, output[1].a);
      CHECK_EQUAL(2, output[1].b); // from input2
      CHECK_EQUAL(3, output[2].a);
      CHECK_EQUAL(1, output[2].b); // from input1
      CHECK_EQUAL(3, output[3].a);
      CHECK_EQUAL(2, output[3].b); // from input2
      CHECK_EQUAL(5, output[4].a);
      CHECK_EQUAL(1, output[4].b); // from input1
      CHECK_EQUAL(5, output[5].a);
      CHECK_EQUAL(2, output[5].b); // from input2
    }

    //*************************************************************************
    TEST(inplace_merge_default_comparator)
    {
      int data[]     = {1, 3, 5, 7, 9, 2, 4, 6, 8, 10};
      int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      etl::inplace_merge(std::begin(data), std::begin(data) + 5, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 10);
    }

    //*************************************************************************
    TEST(inplace_merge_custom_comparator)
    {
      int data[]     = {9, 7, 5, 3, 1, 10, 8, 6, 4, 2};
      int expected[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      etl::inplace_merge(std::begin(data), std::begin(data) + 5, std::end(data), Greater());

      CHECK_ARRAY_EQUAL(expected, data, 10);
    }

    //*************************************************************************
    TEST(inplace_merge_first_range_empty)
    {
      int data[]     = {1, 2, 3, 4, 5};
      int expected[] = {1, 2, 3, 4, 5};

      etl::inplace_merge(std::begin(data), std::begin(data), std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 5);
    }

    //*************************************************************************
    TEST(inplace_merge_second_range_empty)
    {
      int data[]     = {1, 2, 3, 4, 5};
      int expected[] = {1, 2, 3, 4, 5};

      etl::inplace_merge(std::begin(data), std::end(data), std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 5);
    }

    //*************************************************************************
    TEST(inplace_merge_both_ranges_empty)
    {
      int data[] = {99};

      etl::inplace_merge(data, data, data); // empty range

      CHECK_EQUAL(99, data[0]); // unchanged
    }

    //*************************************************************************
    TEST(inplace_merge_with_duplicates)
    {
      int data[]     = {1, 3, 3, 5, 2, 3, 4, 5};
      int expected[] = {1, 2, 3, 3, 3, 4, 5, 5};

      etl::inplace_merge(std::begin(data), std::begin(data) + 4, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 8);
    }

    //*************************************************************************
    TEST(inplace_merge_different_sizes)
    {
      int data[]     = {1, 5, 2, 3, 4, 6, 7, 8};
      int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};

      etl::inplace_merge(std::begin(data), std::begin(data) + 2, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 8);
    }

    //*************************************************************************
    TEST(inplace_merge_single_elements)
    {
      int data[]     = {2, 1};
      int expected[] = {1, 2};

      etl::inplace_merge(std::begin(data), std::begin(data) + 1, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 2);
    }

    //*************************************************************************
    TEST(inplace_merge_single_element_halves_already_sorted)
    {
      int data[]     = {1, 2};
      int expected[] = {1, 2};

      etl::inplace_merge(std::begin(data), std::begin(data) + 1, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 2);
    }

    //*************************************************************************
    TEST(inplace_merge_already_sorted)
    {
      int data[]     = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      etl::inplace_merge(std::begin(data), std::begin(data) + 5, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 10);
    }

    //*************************************************************************
    TEST(inplace_merge_reverse_halves)
    {
      // Second half all less than first half
      int data[]     = {6, 7, 8, 9, 10, 1, 2, 3, 4, 5};
      int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      etl::inplace_merge(std::begin(data), std::begin(data) + 5, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 10);
    }

    //*************************************************************************
    TEST(inplace_merge_with_list_iterators)
    {
      std::vector<int> data     = {1, 3, 5, 7, 2, 4, 6, 8};
      std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8};

      etl::inplace_merge(data.begin(), data.begin() + 4, data.end());

      CHECK(expected == data);
    }

    //*************************************************************************
    TEST(inplace_merge_matches_std)
    {
      int etl_data[] = {1, 4, 7, 8, 10, 2, 3, 5, 6, 9};
      int std_data[] = {1, 4, 7, 8, 10, 2, 3, 5, 6, 9};

      etl::inplace_merge(std::begin(etl_data), std::begin(etl_data) + 5, std::end(etl_data));
      std::inplace_merge(std::begin(std_data), std::begin(std_data) + 5, std::end(std_data));

      CHECK_ARRAY_EQUAL(std_data, etl_data, 10);
    }

    //*************************************************************************
    TEST(inplace_merge_matches_std_with_comparator)
    {
      int etl_data[] = {10, 8, 7, 4, 1, 9, 6, 5, 3, 2};
      int std_data[] = {10, 8, 7, 4, 1, 9, 6, 5, 3, 2};

      etl::inplace_merge(std::begin(etl_data), std::begin(etl_data) + 5, std::end(etl_data), Greater());
      std::inplace_merge(std::begin(std_data), std::begin(std_data) + 5, std::end(std_data), Greater());

      CHECK_ARRAY_EQUAL(std_data, etl_data, 10);
    }

    //*************************************************************************
    TEST(inplace_merge_stability)
    {
      // Test that inplace_merge is stable: equivalent elements from the first
      // range come before those from the second range.
      Data data[] = {Data(1, 1), Data(3, 1), Data(5, 1), Data(1, 2), Data(3, 2), Data(5, 2)};

      etl::inplace_merge(std::begin(data), std::begin(data) + 3, std::end(data), DataPredicate());

      // Elements from first half (b==1) should come before elements from second
      // half (b==2) for equivalent keys.
      CHECK_EQUAL(1, data[0].a);
      CHECK_EQUAL(1, data[0].b); // from first half
      CHECK_EQUAL(1, data[1].a);
      CHECK_EQUAL(2, data[1].b); // from second half
      CHECK_EQUAL(3, data[2].a);
      CHECK_EQUAL(1, data[2].b); // from first half
      CHECK_EQUAL(3, data[3].a);
      CHECK_EQUAL(2, data[3].b); // from second half
      CHECK_EQUAL(5, data[4].a);
      CHECK_EQUAL(1, data[4].b); // from first half
      CHECK_EQUAL(5, data[5].a);
      CHECK_EQUAL(2, data[5].b); // from second half
    }

    //*************************************************************************
    TEST(inplace_merge_single_element_first_half)
    {
      int data[]     = {5, 1, 2, 3, 4};
      int expected[] = {1, 2, 3, 4, 5};

      etl::inplace_merge(std::begin(data), std::begin(data) + 1, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 5);
    }

    //*************************************************************************
    TEST(inplace_merge_single_element_second_half)
    {
      int data[]     = {1, 2, 3, 4, 5, 3};
      int expected[] = {1, 2, 3, 3, 4, 5};

      etl::inplace_merge(std::begin(data), std::begin(data) + 5, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 6);
    }

    //*************************************************************************
    TEST(inplace_merge_all_equal)
    {
      int data[]     = {5, 5, 5, 5, 5, 5};
      int expected[] = {5, 5, 5, 5, 5, 5};

      etl::inplace_merge(std::begin(data), std::begin(data) + 3, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 6);
    }

    //*************************************************************************
    TEST(inplace_merge_interleaved)
    {
      int data[]     = {1, 3, 5, 7, 9, 11, 2, 4, 6, 8, 10, 12};
      int expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

      etl::inplace_merge(std::begin(data), std::begin(data) + 6, std::end(data));

      CHECK_ARRAY_EQUAL(expected, data, 12);
    }

#if ETL_USING_CPP17

    //*************************************************************************
    TEST(ranges_for_each_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              sum = 0;
      auto             fun = [&sum](const int& v)
      {
        sum += v;
      };

      auto result = etl::ranges::for_each(vec.begin(), vec.end(), fun);

      CHECK_EQUAL(15, sum);
      CHECK(result.in == vec.end());
    }

    //*************************************************************************
    TEST(ranges_for_each_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              sum = 0;
      auto             fun = [&sum](const int& v)
      {
        sum += v;
      };

      auto result = etl::ranges::for_each(vec, fun);

      CHECK_EQUAL(15, sum);
      CHECK(result.in == vec.end()); // range overload returns end
    }

    //*************************************************************************
    TEST(ranges_for_each_empty)
    {
      std::vector<int> vec_empty{};
      int              sum = 0;
      auto             fun = [&sum](const int& v)
      {
        sum += v;
      };

      auto result_it = etl::ranges::for_each(vec_empty.begin(), vec_empty.end(), fun);
      CHECK_EQUAL(0, sum);
      (void)result_it;

      sum           = 0;
      auto result_r = etl::ranges::for_each(vec_empty, fun);
      CHECK_EQUAL(0, sum);
      (void)result_r;
    }

    //*************************************************************************
    TEST(ranges_for_each_mutate)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected{2, 4, 6, 8, 10};
      auto             doubler = [](int& v)
      {
        v *= 2;
      };

      etl::ranges::for_each(vec.begin(), vec.end(), doubler);
      bool is_same = std::equal(vec.begin(), vec.end(), expected.begin());
      CHECK(is_same);

      vec = {1, 2, 3, 4, 5};
      etl::ranges::for_each(vec, doubler);
      is_same = std::equal(vec.begin(), vec.end(), expected.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(ranges_for_each_with_projection)
    {
      struct Item
      {
        int value;
      };

      std::vector<Item> vec{{1}, {2}, {3}, {4}, {5}};
      int               sum = 0;
      auto              fun = [&sum](int v)
      {
        sum += v;
      };
      auto proj = [](const Item& item) -> int
      {
        return item.value;
      };

      auto result = etl::ranges::for_each(vec.begin(), vec.end(), fun, proj);
      CHECK_EQUAL(15, sum);
      (void)result;

      sum           = 0;
      auto result_r = etl::ranges::for_each(vec, fun, proj);
      CHECK_EQUAL(15, sum);
      (void)result_r;
    }

    //*************************************************************************
    TEST(ranges_for_each_with_lambda_projection)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              sum = 0;
      auto             fun = [&sum](int v)
      {
        sum += v;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      etl::ranges::for_each(vec.begin(), vec.end(), fun, proj);
      CHECK_EQUAL(150, sum);

      sum = 0;
      etl::ranges::for_each(vec, fun, proj);
      CHECK_EQUAL(150, sum);
    }

    //*************************************************************************
    TEST(ranges_for_each_returns_fun)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      struct Counter
      {
        int  count = 0;
        void operator()(const int&)
        {
          ++count;
        }
      };

      auto result_it = etl::ranges::for_each(vec.begin(), vec.end(), Counter{});
      CHECK_EQUAL(5, result_it.fun.count);

      auto result_r = etl::ranges::for_each(vec, Counter{});
      CHECK_EQUAL(5, result_r.fun.count);
    }

    //*************************************************************************
    TEST(ranges_for_each_single_element)
    {
      std::vector<int> vec{42};
      int              sum = 0;
      auto             fun = [&sum](const int& v)
      {
        sum += v;
      };

      etl::ranges::for_each(vec.begin(), vec.end(), fun);
      CHECK_EQUAL(42, sum);

      sum = 0;
      etl::ranges::for_each(vec, fun);
      CHECK_EQUAL(42, sum);
    }

    //*************************************************************************
    TEST(ranges_for_each_array)
    {
      std::array<int, 5> arr{1, 2, 3, 4, 5};
      int                sum = 0;
      auto               fun = [&sum](const int& v)
      {
        sum += v;
      };

      etl::ranges::for_each(arr.begin(), arr.end(), fun);
      CHECK_EQUAL(15, sum);

      sum = 0;
      etl::ranges::for_each(arr, fun);
      CHECK_EQUAL(15, sum);
    }

    //*************************************************************************
    TEST(ranges_for_each_n_basic)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              sum = 0;
      auto             fun = [&sum](const int& v)
      {
        sum += v;
      };

      auto result = etl::ranges::for_each_n(vec.begin(), 3, fun);

      CHECK_EQUAL(6, sum); // 1+2+3
      CHECK(result.in == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_for_each_n_all_elements)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              sum = 0;
      auto             fun = [&sum](const int& v)
      {
        sum += v;
      };

      auto result = etl::ranges::for_each_n(vec.begin(), 5, fun);

      CHECK_EQUAL(15, sum);
      CHECK(result.in == vec.end());
    }

    //*************************************************************************
    TEST(ranges_for_each_n_zero_count)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              sum = 0;
      auto             fun = [&sum](const int& v)
      {
        sum += v;
      };

      auto result = etl::ranges::for_each_n(vec.begin(), 0, fun);

      CHECK_EQUAL(0, sum);
      CHECK(result.in == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_for_each_n_single_element)
    {
      std::vector<int> vec{42};
      int              sum = 0;
      auto             fun = [&sum](const int& v)
      {
        sum += v;
      };

      auto result = etl::ranges::for_each_n(vec.begin(), 1, fun);

      CHECK_EQUAL(42, sum);
      CHECK(result.in == vec.end());
    }

    //*************************************************************************
    TEST(ranges_for_each_n_mutate)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected{2, 4, 6, 4, 5};
      auto             doubler = [](int& v)
      {
        v *= 2;
      };

      etl::ranges::for_each_n(vec.begin(), 3, doubler);

      bool is_same = std::equal(vec.begin(), vec.end(), expected.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(ranges_for_each_n_with_projection)
    {
      struct Item
      {
        int value;
      };

      std::vector<Item> vec{{1}, {2}, {3}, {4}, {5}};
      int               sum = 0;
      auto              fun = [&sum](int v)
      {
        sum += v;
      };
      auto proj = [](const Item& item) -> int
      {
        return item.value;
      };

      auto result = etl::ranges::for_each_n(vec.begin(), 3, fun, proj);

      CHECK_EQUAL(6, sum); // 1+2+3
      CHECK(result.in == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_for_each_n_with_lambda_projection)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              sum = 0;
      auto             fun = [&sum](int v)
      {
        sum += v;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      etl::ranges::for_each_n(vec.begin(), 4, fun, proj);

      CHECK_EQUAL(100, sum); // (1+2+3+4)*10
    }

    //*************************************************************************
    TEST(ranges_for_each_n_returns_fun)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      struct Counter
      {
        int  count = 0;
        void operator()(const int&)
        {
          ++count;
        }
      };

      auto result = etl::ranges::for_each_n(vec.begin(), 3, Counter{});
      CHECK_EQUAL(3, result.fun.count);
      CHECK(result.in == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_for_each_n_array)
    {
      std::array<int, 5> arr{1, 2, 3, 4, 5};
      int                sum = 0;
      auto               fun = [&sum](const int& v)
      {
        sum += v;
      };

      auto result = etl::ranges::for_each_n(arr.begin(), 4, fun);

      CHECK_EQUAL(10, sum); // 1+2+3+4
      CHECK(result.in == arr.begin() + 4);
    }

    //*************************************************************************
    TEST(ranges_all_of)
    {
      std::vector<int> vec{1, 2, 3};
      std::vector<int> vec_big1{11, 22, 33};
      std::vector<int> vec_big2{1, 22, 3};
      std::vector<int> vec_empty{};
      auto             is_small = [](const int& v) -> bool
      {
        return v < 10;
      };

      CHECK(etl::ranges::all_of(vec.begin(), vec.end(), is_small));
      CHECK(etl::ranges::all_of(vec, is_small));

      CHECK(etl::ranges::all_of(vec_empty.begin(), vec_empty.end(), is_small));
      CHECK(etl::ranges::all_of(vec_empty, is_small));

      CHECK_FALSE(etl::ranges::all_of(vec_big1.begin(), vec_big1.end(), is_small));
      CHECK_FALSE(etl::ranges::all_of(vec_big1, is_small));

      CHECK_FALSE(etl::ranges::all_of(vec_big2.begin(), vec_big2.end(), is_small));
      CHECK_FALSE(etl::ranges::all_of(vec_big2, is_small));

      auto proj = [](const int& v)
      {
        return v * 10;
      };

      CHECK_FALSE(etl::ranges::all_of(vec.begin(), vec.end(), is_small, proj));
      CHECK_FALSE(etl::ranges::all_of(vec, is_small, proj));

      CHECK(etl::ranges::all_of(vec_empty.begin(), vec_empty.end(), is_small, proj));
      CHECK(etl::ranges::all_of(vec_empty, is_small, proj));

      CHECK_FALSE(etl::ranges::all_of(vec_big1.begin(), vec_big1.end(), is_small, proj));
      CHECK_FALSE(etl::ranges::all_of(vec_big1, is_small, proj));

      CHECK_FALSE(etl::ranges::all_of(vec_big2.begin(), vec_big2.end(), is_small, proj));
      CHECK_FALSE(etl::ranges::all_of(vec_big2, is_small, proj));
    }

    //*************************************************************************
    TEST(ranges_any_of)
    {
      std::vector<int> vec{1, 2, 3};
      std::vector<int> vec_big1{11, 22, 33};
      std::vector<int> vec_big2{0, 22, 3};
      std::vector<int> vec_empty{};
      auto             is_small = [](const int& v) -> bool
      {
        return v < 10;
      };

      CHECK(etl::ranges::any_of(vec.begin(), vec.end(), is_small));
      CHECK(etl::ranges::any_of(vec, is_small));

      CHECK_FALSE(etl::ranges::any_of(vec_empty.begin(), vec_empty.end(), is_small));
      CHECK_FALSE(etl::ranges::any_of(vec_empty, is_small));

      CHECK_FALSE(etl::ranges::any_of(vec_big1.begin(), vec_big1.end(), is_small));
      CHECK_FALSE(etl::ranges::any_of(vec_big1, is_small));

      CHECK(etl::ranges::any_of(vec_big2.begin(), vec_big2.end(), is_small));
      CHECK(etl::ranges::any_of(vec_big2, is_small));

      auto proj = [](const int& v)
      {
        return v * 10;
      };

      CHECK_FALSE(etl::ranges::any_of(vec.begin(), vec.end(), is_small, proj));
      CHECK_FALSE(etl::ranges::any_of(vec, is_small, proj));

      CHECK_FALSE(etl::ranges::any_of(vec_empty.begin(), vec_empty.end(), is_small, proj));
      CHECK_FALSE(etl::ranges::any_of(vec_empty, is_small, proj));

      CHECK_FALSE(etl::ranges::any_of(vec_big1.begin(), vec_big1.end(), is_small, proj));
      CHECK_FALSE(etl::ranges::any_of(vec_big1, is_small, proj));

      CHECK(etl::ranges::any_of(vec_big2.begin(), vec_big2.end(), is_small, proj));
      CHECK(etl::ranges::any_of(vec_big2, is_small, proj));
    }

    //*************************************************************************
    TEST(ranges_none_of)
    {
      std::vector<int> vec{1, 2, 3};
      std::vector<int> vec_big1{11, 22, 33};
      std::vector<int> vec_big2{0, 22, 3};
      std::vector<int> vec_empty{};
      auto             is_small = [](const int& v) -> bool
      {
        return v < 10;
      };

      CHECK_FALSE(etl::ranges::none_of(vec.begin(), vec.end(), is_small));
      CHECK_FALSE(etl::ranges::none_of(vec, is_small));

      CHECK(etl::ranges::none_of(vec_empty.begin(), vec_empty.end(), is_small));
      CHECK(etl::ranges::none_of(vec_empty, is_small));

      CHECK(etl::ranges::none_of(vec_big1.begin(), vec_big1.end(), is_small));
      CHECK(etl::ranges::none_of(vec_big1, is_small));

      CHECK_FALSE(etl::ranges::none_of(vec_big2.begin(), vec_big2.end(), is_small));
      CHECK_FALSE(etl::ranges::none_of(vec_big2, is_small));

      auto proj = [](const int& v)
      {
        return v * 10;
      };

      CHECK(etl::ranges::none_of(vec.begin(), vec.end(), is_small, proj));
      CHECK(etl::ranges::none_of(vec, is_small, proj));

      CHECK(etl::ranges::none_of(vec_empty.begin(), vec_empty.end(), is_small, proj));
      CHECK(etl::ranges::none_of(vec_empty, is_small, proj));

      CHECK(etl::ranges::none_of(vec_big1.begin(), vec_big1.end(), is_small, proj));
      CHECK(etl::ranges::none_of(vec_big1, is_small, proj));

      CHECK_FALSE(etl::ranges::none_of(vec_big2.begin(), vec_big2.end(), is_small, proj));
      CHECK_FALSE(etl::ranges::none_of(vec_big2, is_small, proj));
    }

    //*************************************************************************
    TEST(ranges_find)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };

      {
        std::vector<int> vec{7, 2, 1, 8, 1, 6};
        auto             it = etl::ranges::find(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), 1);
        CHECK_EQUAL(vec[2], *it);
        CHECK_EQUAL(&vec[2], &(*it));
        *it = 3;

        it = etl::ranges::find(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), 1);
        CHECK_EQUAL(vec[4], *it);
        CHECK_EQUAL(&vec[4], &(*it));

        it = etl::ranges::find(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), 9);

        CHECK(vec.end() == it);

        it = etl::ranges::find(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), 12, proj);
        CHECK_EQUAL(vec[5], *it);
        CHECK_EQUAL(&vec[5], &(*it));
      }

      {
        std::vector<int> vec{7, 2, 1, 8, 1, 6};
        auto             it = etl::ranges::find(vec, 1);
        CHECK_EQUAL(vec[2], *it);
        CHECK_EQUAL(&vec[2], &(*it));

        it = etl::ranges::find(vec, 16, proj);
        CHECK_EQUAL(vec[3], *it);
        CHECK_EQUAL(&vec[3], &(*it));
      }
    }

    //*************************************************************************
    TEST(ranges_find_if)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      auto pred = [](const int& v)
      {
        return v == 1;
      };

      {
        std::vector<int> vec{7, 2, 1, 8, 1, 6};
        auto             it = etl::ranges::find_if(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), pred);
        CHECK_EQUAL(vec[2], *it);
        CHECK_EQUAL(&vec[2], &(*it));
        *it = 3;

        it = etl::ranges::find_if(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), pred);
        CHECK_EQUAL(vec[4], *it);
        CHECK_EQUAL(&vec[4], &(*it));

        it = etl::ranges::find_if(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), [](const int& v) { return v == 9; });

        CHECK(vec.end() == it);

        it = etl::ranges::find_if(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), [](const int& v) { return v == 12; }, proj);
        CHECK_EQUAL(vec[5], *it);
        CHECK_EQUAL(&vec[5], &(*it));
      }

      {
        std::vector<int> vec{7, 2, 1, 8, 1, 6};
        auto             it = etl::ranges::find_if(vec, pred);
        CHECK_EQUAL(vec[2], *it);
        CHECK_EQUAL(&vec[2], &(*it));

        it = etl::ranges::find_if(vec, [](const int& v) { return v == 16; }, proj);
        CHECK_EQUAL(vec[3], *it);
        CHECK_EQUAL(&vec[3], &(*it));
      }
    }

    //*************************************************************************
    TEST(ranges_find_if_not)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      auto pred = [](const int& v)
      {
        return v != 1;
      };

      {
        std::vector<int> vec{7, 2, 1, 8, 1, 6};
        auto             it = etl::ranges::find_if_not(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), pred);
        CHECK_EQUAL(vec[2], *it);
        CHECK_EQUAL(&vec[2], &(*it));
        *it = 3;

        it = etl::ranges::find_if_not(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), pred);
        CHECK_EQUAL(vec[4], *it);
        CHECK_EQUAL(&vec[4], &(*it));

        it = etl::ranges::find_if_not(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), [](const int& v) { return v != 9; });

        CHECK(vec.end() == it);

        it = etl::ranges::find_if_not(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), [](const int& v) { return v != 12; }, proj);
        CHECK_EQUAL(vec[5], *it);
        CHECK_EQUAL(&vec[5], &(*it));
      }

      {
        std::vector<int> vec{7, 2, 1, 8, 1, 6};
        auto             it = etl::ranges::find_if_not(vec, pred);
        CHECK_EQUAL(vec[2], *it);
        CHECK_EQUAL(&vec[2], &(*it));

        it = etl::ranges::find_if_not(vec, [](const int& v) { return v != 16; }, proj);
        CHECK_EQUAL(vec[3], *it);
        CHECK_EQUAL(&vec[3], &(*it));
      }
    }

    //*************************************************************************
    TEST(ranges_find_end)
    {
      auto proj1 = [](const int& v)
      {
        return v * 2;
      };
      auto proj2 = [](const int& v)
      {
        return v * 3;
      };
      auto pred = [](const int& v0, const int& v1)
      {
        return v0 == v1;
      };

      {
        std::vector<int> vec{7, 2, 1, 8, 2, 6, 12, 3};
        std::vector<int> vec2{8, 2};
        auto s = etl::ranges::find_end(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(vec2), ETL_OR_STD::end(vec2), pred);
        CHECK_EQUAL(s.size(), 2);
        CHECK_EQUAL(s[0], 8);
        CHECK_EQUAL(s[1], 2);
        vec[3] = 3;

        s = etl::ranges::find_end(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(vec2), ETL_OR_STD::end(vec2), pred);
        CHECK(s.empty());

        vec[3] = 8;

        s = etl::ranges::find_end(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(vec2), ETL_OR_STD::end(vec2),
                                  [](const int& v0, const int& v1) { return v0 == v1; });
        CHECK_EQUAL(s.size(), 2);
        CHECK_EQUAL(s[0], 8);
        CHECK_EQUAL(s[1], 2);

        s = etl::ranges::find_end(
          ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(vec2), ETL_OR_STD::end(vec2),
          [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK_EQUAL(s.size(), 2);
        CHECK_EQUAL(s[0], 12);
        CHECK_EQUAL(s[1], 3);
      }

      {
        std::vector<int> vec{7, 2, 1, 8, 2, 6, 12, 3};
        std::vector<int> vec2{8, 2};
        std::vector<int> vec3{99, 2};
        auto             s = etl::ranges::find_end(vec, vec2, pred);
        CHECK_EQUAL(s.size(), 2);
        CHECK_EQUAL(s[0], 8);
        CHECK_EQUAL(s[1], 2);

        s = etl::ranges::find_end(vec, vec2, [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK_EQUAL(s.size(), 2);
        CHECK_EQUAL(s[0], 12);
        CHECK_EQUAL(s[1], 3);

        s = etl::ranges::find_end(vec, vec3, [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK(s.empty());
      }
    }

    //*************************************************************************
    TEST(ranges_find_first_of_iterator)
    {
      auto proj1 = [](const int& v)
      {
        return v * 2;
      };
      auto proj2 = [](const int& v)
      {
        return v * 3;
      };
      auto pred = [](const int& v0, const int& v1)
      {
        return v0 == v1;
      };

      // Found with predicate
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{5, 8};
        auto             it =
          etl::ranges::find_first_of(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets), pred);
        CHECK_EQUAL(8, *it);
      }

      // Not found with predicate
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{99, 100};
        auto             it =
          etl::ranges::find_first_of(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets), pred);
        CHECK(it == ETL_OR_STD::end(vec));
      }

      // Found with default predicate (no predicate argument)
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{6, 12};
        auto it = etl::ranges::find_first_of(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets));
        CHECK_EQUAL(6, *it);
      }

      // Found with lambda predicate
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{12, 3};
        auto it = etl::ranges::find_first_of(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets),
                                             [](const int& v0, const int& v1) { return v0 == v1; });
        CHECK_EQUAL(12, *it);
      }

      // Found with projections: proj1 doubles, proj2 triples, so 6*2==4*3
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{4};
        auto             it = etl::ranges::find_first_of(
          ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets),
          [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK_EQUAL(6, *it);
      }

      // Empty haystack
      {
        std::vector<int> vec{};
        std::vector<int> targets{1, 2};
        auto             it =
          etl::ranges::find_first_of(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets), pred);
        CHECK(it == ETL_OR_STD::end(vec));
      }

      // Empty targets
      {
        std::vector<int> vec{7, 2, 1, 8};
        std::vector<int> targets{};
        auto             it =
          etl::ranges::find_first_of(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets), pred);
        CHECK(it == ETL_OR_STD::end(vec));
      }

      // First element matches
      {
        std::vector<int> vec{7, 2, 1, 8};
        std::vector<int> targets{7};
        auto             it =
          etl::ranges::find_first_of(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets), pred);
        CHECK_EQUAL(7, *it);
        CHECK(it == ETL_OR_STD::begin(vec));
      }

      // Last element matches
      {
        std::vector<int> vec{7, 2, 1, 8};
        std::vector<int> targets{8};
        auto             it =
          etl::ranges::find_first_of(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(targets), ETL_OR_STD::end(targets), pred);
        CHECK_EQUAL(8, *it);
      }
    }

    //*************************************************************************
    TEST(ranges_find_first_of_range)
    {
      auto proj1 = [](const int& v)
      {
        return v * 2;
      };
      auto proj2 = [](const int& v)
      {
        return v * 3;
      };
      auto pred = [](const int& v0, const int& v1)
      {
        return v0 == v1;
      };

      // Found with predicate
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{5, 8};
        auto             it = etl::ranges::find_first_of(vec, targets, pred);
        CHECK_EQUAL(8, *it);
      }

      // Not found with predicate
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{99, 100};
        auto             it = etl::ranges::find_first_of(vec, targets, pred);
        CHECK(it == ETL_OR_STD::end(vec));
      }

      // Found with default predicate
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{6, 12};
        auto             it = etl::ranges::find_first_of(vec, targets);
        CHECK_EQUAL(6, *it);
      }

      // Found with lambda predicate
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{12, 3};
        auto             it = etl::ranges::find_first_of(vec, targets, [](const int& v0, const int& v1) { return v0 == v1; });
        CHECK_EQUAL(12, *it);
      }

      // Found with projections: proj1 doubles, proj2 triples, so 6*2==4*3
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{4};
        auto             it = etl::ranges::find_first_of(vec, targets, [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK_EQUAL(6, *it);
      }

      // Not found with projections
      {
        std::vector<int> vec{7, 2, 1, 8, 5, 6, 12, 3};
        std::vector<int> targets{99};
        auto             it = etl::ranges::find_first_of(vec, targets, [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK(it == ETL_OR_STD::end(vec));
      }

      // Empty haystack
      {
        std::vector<int> vec{};
        std::vector<int> targets{1, 2};
        auto             it = etl::ranges::find_first_of(vec, targets, pred);
        CHECK(it == ETL_OR_STD::end(vec));
      }

      // Empty targets
      {
        std::vector<int> vec{7, 2, 1, 8};
        std::vector<int> targets{};
        auto             it = etl::ranges::find_first_of(vec, targets, pred);
        CHECK(it == ETL_OR_STD::end(vec));
      }

      // First element matches
      {
        std::vector<int> vec{7, 2, 1, 8};
        std::vector<int> targets{7};
        auto             it = etl::ranges::find_first_of(vec, targets, pred);
        CHECK_EQUAL(7, *it);
        CHECK(it == ETL_OR_STD::begin(vec));
      }

      // Last element matches
      {
        std::vector<int> vec{7, 2, 1, 8};
        std::vector<int> targets{8};
        auto             it = etl::ranges::find_first_of(vec, targets, pred);
        CHECK_EQUAL(8, *it);
      }
    }

    //*************************************************************************
    TEST(ranges_search_iterator)
    {
      auto pred = [](const int& v0, const int& v1)
      {
        return v0 == v1;
      };

      // Search with predicate - found
      {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<int> needle{3, 4, 5};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(3, s[0]);
        CHECK_EQUAL(4, s[1]);
        CHECK_EQUAL(5, s[2]);
      }

      // Search with predicate - not found
      {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<int> needle{3, 5, 4};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK(s.empty());
        CHECK(s.begin() == ETL_OR_STD::end(vec));
      }

      // Search with default predicate (no predicate argument)
      {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<int> needle{6, 7, 8};
        auto             s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle));
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(6, s[0]);
        CHECK_EQUAL(7, s[1]);
        CHECK_EQUAL(8, s[2]);
      }

      // Search with lambda predicate
      {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<int> needle{4, 5};
        auto             s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle),
                                                 [](const int& v0, const int& v1) { return v0 == v1; });
        CHECK_EQUAL(2, s.size());
        CHECK_EQUAL(4, s[0]);
        CHECK_EQUAL(5, s[1]);
      }

      // Search with projections
      {
        std::vector<int> vec{2, 4, 6, 8, 10, 12, 14, 16};
        std::vector<int> needle{3, 4};
        auto             proj1 = [](const int& v)
        {
          return v / 2;
        };
        auto proj2 = [](const int& v)
        {
          return v;
        };
        auto s = etl::ranges::search(
          ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle),
          [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK_EQUAL(2, s.size());
        CHECK_EQUAL(6, s[0]);
        CHECK_EQUAL(8, s[1]);
      }

      // Search at beginning
      {
        std::vector<int> vec{1, 2, 3, 4, 5};
        std::vector<int> needle{1, 2};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK_EQUAL(2, s.size());
        CHECK_EQUAL(1, s[0]);
        CHECK_EQUAL(2, s[1]);
      }

      // Search at end
      {
        std::vector<int> vec{1, 2, 3, 4, 5};
        std::vector<int> needle{4, 5};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK_EQUAL(2, s.size());
        CHECK_EQUAL(4, s[0]);
        CHECK_EQUAL(5, s[1]);
      }

      // Search with empty needle
      {
        std::vector<int> vec{1, 2, 3, 4, 5};
        std::vector<int> needle{};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK(s.empty());
        CHECK(s.begin() == ETL_OR_STD::begin(vec));
      }

      // Search with single element needle - found
      {
        std::vector<int> vec{1, 2, 3, 4, 5};
        std::vector<int> needle{3};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK_EQUAL(1, s.size());
        CHECK_EQUAL(3, s[0]);
      }

      // Search with needle same size as haystack - found
      {
        std::vector<int> vec{1, 2, 3};
        std::vector<int> needle{1, 2, 3};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(1, s[0]);
        CHECK_EQUAL(2, s[1]);
        CHECK_EQUAL(3, s[2]);
      }

      // Search with needle larger than haystack - not found
      {
        std::vector<int> vec{1, 2};
        std::vector<int> needle{1, 2, 3};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK(s.empty());
        CHECK(s.begin() == ETL_OR_STD::end(vec));
      }

      // Search finds first occurrence
      {
        std::vector<int> vec{1, 2, 3, 1, 2, 3};
        std::vector<int> needle{1, 2, 3};
        auto s = etl::ranges::search(ETL_OR_STD::begin(vec), ETL_OR_STD::end(vec), ETL_OR_STD::begin(needle), ETL_OR_STD::end(needle), pred);
        CHECK_EQUAL(3, s.size());
        CHECK(s.begin() == vec.begin());
      }
    }

    //*************************************************************************
    TEST(ranges_search_range)
    {
      auto pred = [](const int& v0, const int& v1)
      {
        return v0 == v1;
      };

      // Search with predicate - found
      {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<int> needle{3, 4, 5};
        auto             s = etl::ranges::search(vec, needle, pred);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(3, s[0]);
        CHECK_EQUAL(4, s[1]);
        CHECK_EQUAL(5, s[2]);
      }

      // Search with predicate - not found
      {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<int> needle{3, 5, 4};
        auto             s = etl::ranges::search(vec, needle, pred);
        CHECK(s.empty());
        CHECK(s.begin() == ETL_OR_STD::end(vec));
      }

      // Search with default predicate
      {
        std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
        std::vector<int> needle{5, 6, 7};
        auto             s = etl::ranges::search(vec, needle);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(5, s[0]);
        CHECK_EQUAL(6, s[1]);
        CHECK_EQUAL(7, s[2]);
      }

      // Search with projections
      {
        std::vector<int> vec{2, 4, 6, 8, 10, 12, 14, 16};
        std::vector<int> needle{3, 4};
        auto             proj1 = [](const int& v)
        {
          return v / 2;
        };
        auto proj2 = [](const int& v)
        {
          return v;
        };
        auto s = etl::ranges::search(vec, needle, [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK_EQUAL(2, s.size());
        CHECK_EQUAL(6, s[0]);
        CHECK_EQUAL(8, s[1]);
      }

      // Search with empty needle
      {
        std::vector<int> vec{1, 2, 3, 4, 5};
        std::vector<int> needle{};
        auto             s = etl::ranges::search(vec, needle, pred);
        CHECK(s.empty());
        CHECK(s.begin() == ETL_OR_STD::begin(vec));
      }

      // Search finds first occurrence
      {
        std::vector<int> vec{1, 2, 3, 1, 2, 3};
        std::vector<int> needle{1, 2, 3};
        auto             s = etl::ranges::search(vec, needle, pred);
        CHECK_EQUAL(3, s.size());
        CHECK(s.begin() == vec.begin());
      }

      // Search not found with projections
      {
        std::vector<int> vec{2, 4, 6, 8, 10};
        std::vector<int> needle{99, 100};
        auto             proj1 = [](const int& v)
        {
          return v / 2;
        };
        auto proj2 = [](const int& v)
        {
          return v;
        };
        auto s = etl::ranges::search(vec, needle, [](const int& v0, const int& v1) { return v0 == v1; }, proj1, proj2);
        CHECK(s.empty());
        CHECK(s.begin() == ETL_OR_STD::end(vec));
      }
    }

    //*************************************************************************
    TEST(ranges_search_n_iterator)
    {
      auto pred = [](const int& v0, const int& v1)
      {
        return v0 == v1;
      };

      // Search_n with predicate - found
      {
        std::vector<int> vec{1, 2, 3, 3, 3, 4, 5};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 3, 3, pred);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(3, s[0]);
        CHECK_EQUAL(3, s[1]);
        CHECK_EQUAL(3, s[2]);
        CHECK(s.begin() == vec.begin() + 2);
      }

      // Search_n with predicate - not found
      {
        std::vector<int> vec{1, 2, 3, 3, 4, 5};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 3, 3, pred);
        CHECK(s.empty());
        CHECK(s.begin() == vec.end());
      }

      // Search_n with default predicate (no predicate argument)
      {
        std::vector<int> vec{1, 2, 2, 2, 3, 4};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 3, 2);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(2, s[0]);
        CHECK_EQUAL(2, s[1]);
        CHECK_EQUAL(2, s[2]);
      }

      // Search_n with lambda predicate
      {
        std::vector<int> vec{1, 2, 3, 3, 3, 4};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 2, 3, [](const int& v0, const int& v1) { return v0 == v1; });
        CHECK_EQUAL(2, s.size());
        CHECK_EQUAL(3, s[0]);
        CHECK_EQUAL(3, s[1]);
      }

      // Search_n with projection
      {
        std::vector<int> vec{2, 4, 6, 6, 6, 8};
        auto             proj = [](const int& v)
        {
          return v / 2;
        };
        auto s = etl::ranges::search_n(vec.begin(), vec.end(), 3, 3, [](const int& v0, const int& v1) { return v0 == v1; }, proj);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(6, s[0]);
        CHECK_EQUAL(6, s[1]);
        CHECK_EQUAL(6, s[2]);
      }

      // Search_n at beginning
      {
        std::vector<int> vec{5, 5, 5, 1, 2, 3};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 3, 5, pred);
        CHECK_EQUAL(3, s.size());
        CHECK(s.begin() == vec.begin());
        CHECK_EQUAL(5, s[0]);
        CHECK_EQUAL(5, s[1]);
        CHECK_EQUAL(5, s[2]);
      }

      // Search_n at end
      {
        std::vector<int> vec{1, 2, 3, 5, 5, 5};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 3, 5, pred);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(5, s[0]);
        CHECK_EQUAL(5, s[1]);
        CHECK_EQUAL(5, s[2]);
      }

      // Search_n with count 0
      {
        std::vector<int> vec{1, 2, 3};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 0, 1, pred);
        CHECK(s.empty());
        CHECK(s.begin() == vec.begin());
      }

      // Search_n with count 1 - found
      {
        std::vector<int> vec{1, 2, 3, 4, 5};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 1, 3, pred);
        CHECK_EQUAL(1, s.size());
        CHECK_EQUAL(3, s[0]);
      }

      // Search_n with empty range
      {
        std::vector<int> vec{};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 1, 3, pred);
        CHECK(s.empty());
      }

      // Search_n entire range matches
      {
        std::vector<int> vec{7, 7, 7, 7};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 4, 7, pred);
        CHECK_EQUAL(4, s.size());
        CHECK(s.begin() == vec.begin());
      }

      // Search_n count larger than range
      {
        std::vector<int> vec{3, 3};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 5, 3, pred);
        CHECK(s.empty());
      }

      // Search_n finds first occurrence
      {
        std::vector<int> vec{1, 2, 2, 1, 2, 2, 3};
        auto             s = etl::ranges::search_n(vec.begin(), vec.end(), 2, 2, pred);
        CHECK_EQUAL(2, s.size());
        CHECK(s.begin() == vec.begin() + 1);
      }
    }

    //*************************************************************************
    TEST(ranges_search_n_range)
    {
      auto pred = [](const int& v0, const int& v1)
      {
        return v0 == v1;
      };

      // Search_n with predicate - found
      {
        std::vector<int> vec{1, 2, 3, 3, 3, 4, 5};
        auto             s = etl::ranges::search_n(vec, 3, 3, pred);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(3, s[0]);
        CHECK_EQUAL(3, s[1]);
        CHECK_EQUAL(3, s[2]);
        CHECK(s.begin() == vec.begin() + 2);
      }

      // Search_n with predicate - not found
      {
        std::vector<int> vec{1, 2, 3, 3, 4, 5};
        auto             s = etl::ranges::search_n(vec, 3, 3, pred);
        CHECK(s.empty());
        CHECK(s.begin() == vec.end());
      }

      // Search_n with default predicate
      {
        std::vector<int> vec{1, 4, 4, 4, 5, 6};
        auto             s = etl::ranges::search_n(vec, 3, 4);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(4, s[0]);
        CHECK_EQUAL(4, s[1]);
        CHECK_EQUAL(4, s[2]);
      }

      // Search_n with projection
      {
        std::vector<int> vec{2, 4, 6, 6, 6, 8};
        auto             proj = [](const int& v)
        {
          return v / 2;
        };
        auto s = etl::ranges::search_n(vec, 3, 3, [](const int& v0, const int& v1) { return v0 == v1; }, proj);
        CHECK_EQUAL(3, s.size());
        CHECK_EQUAL(6, s[0]);
        CHECK_EQUAL(6, s[1]);
        CHECK_EQUAL(6, s[2]);
      }

      // Search_n with count 0
      {
        std::vector<int> vec{1, 2, 3};
        auto             s = etl::ranges::search_n(vec, 0, 1, pred);
        CHECK(s.empty());
        CHECK(s.begin() == vec.begin());
      }

      // Search_n with empty range
      {
        std::vector<int> vec{};
        auto             s = etl::ranges::search_n(vec, 1, 3, pred);
        CHECK(s.empty());
      }

      // Search_n finds first occurrence
      {
        std::vector<int> vec{1, 2, 2, 1, 2, 2, 3};
        auto             s = etl::ranges::search_n(vec, 2, 2, pred);
        CHECK_EQUAL(2, s.size());
        CHECK(s.begin() == vec.begin() + 1);
      }

      // Search_n entire range matches
      {
        std::vector<int> vec{7, 7, 7, 7};
        auto             s = etl::ranges::search_n(vec, 4, 7, pred);
        CHECK_EQUAL(4, s.size());
        CHECK(s.begin() == vec.begin());
      }
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_found)
    {
      // Adjacent duplicates exist
      std::vector<int> vec{1, 2, 3, 3, 4, 5};
      auto             it = etl::ranges::adjacent_find(vec.begin(), vec.end());
      CHECK_EQUAL(3, *it);
      CHECK(it == vec.begin() + 2);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_not_found)
    {
      // No adjacent duplicates
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::adjacent_find(vec.begin(), vec.end());
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_empty)
    {
      // Empty range
      std::vector<int> vec{};
      auto             it = etl::ranges::adjacent_find(vec.begin(), vec.end());
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_single_element)
    {
      // Single element - no adjacent pair possible
      std::vector<int> vec{42};
      auto             it = etl::ranges::adjacent_find(vec.begin(), vec.end());
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_first_pair)
    {
      // Adjacent duplicates at the very beginning
      std::vector<int> vec{5, 5, 1, 2, 3};
      auto             it = etl::ranges::adjacent_find(vec.begin(), vec.end());
      CHECK_EQUAL(5, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_last_pair)
    {
      // Adjacent duplicates at the very end
      std::vector<int> vec{1, 2, 3, 7, 7};
      auto             it = etl::ranges::adjacent_find(vec.begin(), vec.end());
      CHECK_EQUAL(7, *it);
      CHECK(it == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_multiple_pairs)
    {
      // Multiple adjacent duplicate pairs - should find the first one
      std::vector<int> vec{1, 2, 2, 3, 3, 4};
      auto             it = etl::ranges::adjacent_find(vec.begin(), vec.end());
      CHECK_EQUAL(2, *it);
      CHECK(it == vec.begin() + 1);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_all_same)
    {
      // All elements are the same
      std::vector<int> vec{9, 9, 9, 9};
      auto             it = etl::ranges::adjacent_find(vec.begin(), vec.end());
      CHECK_EQUAL(9, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_custom_predicate)
    {
      // Custom predicate: find adjacent pair where second is greater
      std::vector<int> vec{5, 3, 1, 4, 2};
      auto             pred = [](const int& a, const int& b)
      {
        return a < b;
      };
      auto it = etl::ranges::adjacent_find(vec.begin(), vec.end(), pred);
      CHECK_EQUAL(1, *it);
      CHECK(it == vec.begin() + 2);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_custom_predicate_not_found)
    {
      // Custom predicate not satisfied by any adjacent pair
      std::vector<int> vec{5, 4, 3, 2, 1};
      auto             pred = [](const int& a, const int& b)
      {
        return a < b;
      };
      auto it = etl::ranges::adjacent_find(vec.begin(), vec.end(), pred);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_with_projection)
    {
      // Projection: compare absolute values for adjacency
      std::vector<int> vec{1, -2, 3, -3, 5};
      auto             proj = [](const int& v)
      {
        return v < 0 ? -v : v;
      };
      auto it = etl::ranges::adjacent_find(vec.begin(), vec.end(), etl::ranges::equal_to{}, proj);
      CHECK_EQUAL(3, *it);
      CHECK(it == vec.begin() + 2);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_with_projection_not_found)
    {
      // Projection: no adjacent pair matches after projection
      std::vector<int> vec{1, -2, 3, -4, 5};
      auto             proj = [](const int& v)
      {
        return v < 0 ? -v : v;
      };
      auto it = etl::ranges::adjacent_find(vec.begin(), vec.end(), etl::ranges::equal_to{}, proj);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_iterator_predicate_and_projection)
    {
      // Predicate: equality after projection (mod 10)
      // {11, 25, 32, 43, 53} -> mod 10 -> {1, 5, 2, 3, 3}, so 43 and 53 match
      std::vector<int> vec{11, 25, 32, 43, 53};
      auto             proj = [](const int& v)
      {
        return v % 10;
      };
      auto pred = [](const int& a, const int& b)
      {
        return a == b;
      };
      auto it = etl::ranges::adjacent_find(vec.begin(), vec.end(), pred, proj);
      CHECK_EQUAL(43, *it);
      CHECK(it == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_found)
    {
      // Adjacent duplicates exist - range overload
      std::vector<int> vec{1, 2, 3, 3, 4, 5};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK_EQUAL(3, *it);
      CHECK(it == vec.begin() + 2);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_not_found)
    {
      // No adjacent duplicates - range overload
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_empty)
    {
      // Empty range - range overload
      std::vector<int> vec{};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_single_element)
    {
      // Single element - range overload
      std::vector<int> vec{42};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_first_pair)
    {
      // Adjacent duplicates at beginning - range overload
      std::vector<int> vec{5, 5, 1, 2, 3};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK_EQUAL(5, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_last_pair)
    {
      // Adjacent duplicates at end - range overload
      std::vector<int> vec{1, 2, 3, 7, 7};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK_EQUAL(7, *it);
      CHECK(it == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_multiple_pairs)
    {
      // Multiple adjacent pairs - range overload, finds first
      std::vector<int> vec{1, 2, 2, 3, 3, 4};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK_EQUAL(2, *it);
      CHECK(it == vec.begin() + 1);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_all_same)
    {
      // All same - range overload
      std::vector<int> vec{9, 9, 9, 9};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK_EQUAL(9, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_custom_predicate)
    {
      // Custom predicate with range overload
      std::vector<int> vec{5, 3, 1, 4, 2};
      auto             pred = [](const int& a, const int& b)
      {
        return a < b;
      };
      auto it = etl::ranges::adjacent_find(vec, pred);
      CHECK_EQUAL(1, *it);
      CHECK(it == vec.begin() + 2);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_custom_predicate_not_found)
    {
      // Custom predicate not satisfied - range overload
      std::vector<int> vec{5, 4, 3, 2, 1};
      auto             pred = [](const int& a, const int& b)
      {
        return a < b;
      };
      auto it = etl::ranges::adjacent_find(vec, pred);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_with_projection)
    {
      // Projection with range overload
      std::vector<int> vec{1, -2, 3, -3, 5};
      auto             proj = [](const int& v)
      {
        return v < 0 ? -v : v;
      };
      auto it = etl::ranges::adjacent_find(vec, etl::ranges::equal_to{}, proj);
      CHECK_EQUAL(3, *it);
      CHECK(it == vec.begin() + 2);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_with_projection_not_found)
    {
      // Projection with range overload - not found
      std::vector<int> vec{1, -2, 3, -4, 5};
      auto             proj = [](const int& v)
      {
        return v < 0 ? -v : v;
      };
      auto it = etl::ranges::adjacent_find(vec, etl::ranges::equal_to{}, proj);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_range_predicate_and_projection)
    {
      // Predicate and projection with range overload
      // {11, 25, 32, 43, 53} -> mod 10 -> {1, 5, 2, 3, 3}, so 43 and 53 match
      std::vector<int> vec{11, 25, 32, 43, 53};
      auto             proj = [](const int& v)
      {
        return v % 10;
      };
      auto pred = [](const int& a, const int& b)
      {
        return a == b;
      };
      auto it = etl::ranges::adjacent_find(vec, pred, proj);
      CHECK_EQUAL(43, *it);
      CHECK(it == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_two_elements_equal)
    {
      // Exactly two elements that are equal
      std::vector<int> vec{3, 3};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK_EQUAL(3, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_adjacent_find_two_elements_not_equal)
    {
      // Exactly two elements that are not equal
      std::vector<int> vec{3, 4};
      auto             it = etl::ranges::adjacent_find(vec);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_count_iterator_basic)
    {
      std::vector<int> vec{1, 2, 3, 1, 4, 1, 5};

      auto result = etl::ranges::count(vec.begin(), vec.end(), 1);
      CHECK_EQUAL(3, result);

      result = etl::ranges::count(vec.begin(), vec.end(), 4);
      CHECK_EQUAL(1, result);
    }

    //*************************************************************************
    TEST(ranges_count_range_basic)
    {
      std::vector<int> vec{1, 2, 3, 1, 4, 1, 5};

      auto result = etl::ranges::count(vec, 1);
      CHECK_EQUAL(3, result);

      result = etl::ranges::count(vec, 4);
      CHECK_EQUAL(1, result);
    }

    //*************************************************************************
    TEST(ranges_count_not_found)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 99);
      CHECK_EQUAL(0, result_it);

      auto result_r = etl::ranges::count(vec, 99);
      CHECK_EQUAL(0, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_empty)
    {
      std::vector<int> vec{};

      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 1);
      CHECK_EQUAL(0, result_it);

      auto result_r = etl::ranges::count(vec, 1);
      CHECK_EQUAL(0, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_all_match)
    {
      std::vector<int> vec{7, 7, 7, 7};

      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 7);
      CHECK_EQUAL(4, result_it);

      auto result_r = etl::ranges::count(vec, 7);
      CHECK_EQUAL(4, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_single_element_match)
    {
      std::vector<int> vec{42};

      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 42);
      CHECK_EQUAL(1, result_it);

      auto result_r = etl::ranges::count(vec, 42);
      CHECK_EQUAL(1, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_single_element_no_match)
    {
      std::vector<int> vec{42};

      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 99);
      CHECK_EQUAL(0, result_it);

      auto result_r = etl::ranges::count(vec, 99);
      CHECK_EQUAL(0, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_with_projection)
    {
      std::vector<int> vec{1, -2, 3, -2, 5};
      auto             proj = [](const int& v)
      {
        return v < 0 ? -v : v;
      };

      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 2, proj);
      CHECK_EQUAL(2, result_it);

      auto result_r = etl::ranges::count(vec, 2, proj);
      CHECK_EQUAL(2, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_with_projection_no_match)
    {
      std::vector<int> vec{1, -2, 3, -4, 5};
      auto             proj = [](const int& v)
      {
        return v < 0 ? -v : v;
      };

      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 99, proj);
      CHECK_EQUAL(0, result_it);

      auto result_r = etl::ranges::count(vec, 99, proj);
      CHECK_EQUAL(0, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_with_member_projection)
    {
      struct Item
      {
        int id;
        int category;
      };

      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 10}, {4, 10}, {5, 30}};

      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 10, &Item::category);
      CHECK_EQUAL(3, result_it);

      auto result_r = etl::ranges::count(vec, 20, &Item::category);
      CHECK_EQUAL(1, result_r);

      auto result_none = etl::ranges::count(vec, 99, &Item::category);
      CHECK_EQUAL(0, result_none);
    }

    //*************************************************************************
    TEST(ranges_count_with_lambda_projection)
    {
      std::vector<int> vec{10, 21, 30, 41, 50};
      auto             proj = [](const int& v)
      {
        return v % 10;
      };

      // Count elements whose last digit is 0
      auto result_it = etl::ranges::count(vec.begin(), vec.end(), 0, proj);
      CHECK_EQUAL(3, result_it);

      auto result_r = etl::ranges::count(vec, 1, proj);
      CHECK_EQUAL(2, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_array)
    {
      int arr[] = {1, 2, 1, 3, 1, 4};

      auto result = etl::ranges::count(arr, 1);
      CHECK_EQUAL(3, result);

      result = etl::ranges::count(arr, 3);
      CHECK_EQUAL(1, result);

      result = etl::ranges::count(arr, 99);
      CHECK_EQUAL(0, result);
    }

    //*************************************************************************
    TEST(ranges_count_if_iterator_basic)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
      auto             is_even = [](int v)
      {
        return v % 2 == 0;
      };

      auto result = etl::ranges::count_if(vec.begin(), vec.end(), is_even);
      CHECK_EQUAL(4, result);

      auto is_greater_than_5 = [](int v)
      {
        return v > 5;
      };
      result = etl::ranges::count_if(vec.begin(), vec.end(), is_greater_than_5);
      CHECK_EQUAL(3, result);
    }

    //*************************************************************************
    TEST(ranges_count_if_range_basic)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
      auto             is_even = [](int v)
      {
        return v % 2 == 0;
      };

      auto result = etl::ranges::count_if(vec, is_even);
      CHECK_EQUAL(4, result);

      auto is_greater_than_5 = [](int v)
      {
        return v > 5;
      };
      result = etl::ranges::count_if(vec, is_greater_than_5);
      CHECK_EQUAL(3, result);
    }

    //*************************************************************************
    TEST(ranges_count_if_not_found)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             is_negative = [](int v)
      {
        return v < 0;
      };

      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), is_negative);
      CHECK_EQUAL(0, result_it);

      auto result_r = etl::ranges::count_if(vec, is_negative);
      CHECK_EQUAL(0, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_if_empty)
    {
      std::vector<int> vec{};
      auto             is_even = [](int v)
      {
        return v % 2 == 0;
      };

      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), is_even);
      CHECK_EQUAL(0, result_it);

      auto result_r = etl::ranges::count_if(vec, is_even);
      CHECK_EQUAL(0, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_if_all_match)
    {
      std::vector<int> vec{2, 4, 6, 8};
      auto             is_even = [](int v)
      {
        return v % 2 == 0;
      };

      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), is_even);
      CHECK_EQUAL(4, result_it);

      auto result_r = etl::ranges::count_if(vec, is_even);
      CHECK_EQUAL(4, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_if_single_element_match)
    {
      std::vector<int> vec{42};
      auto             is_even = [](int v)
      {
        return v % 2 == 0;
      };

      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), is_even);
      CHECK_EQUAL(1, result_it);

      auto result_r = etl::ranges::count_if(vec, is_even);
      CHECK_EQUAL(1, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_if_single_element_no_match)
    {
      std::vector<int> vec{41};
      auto             is_even = [](int v)
      {
        return v % 2 == 0;
      };

      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), is_even);
      CHECK_EQUAL(0, result_it);

      auto result_r = etl::ranges::count_if(vec, is_even);
      CHECK_EQUAL(0, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_if_with_projection)
    {
      std::vector<int> vec{1, -2, 3, -4, 5};
      auto             proj = [](const int& v)
      {
        return v < 0 ? -v : v;
      };
      auto is_even = [](int v)
      {
        return v % 2 == 0;
      };

      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), is_even, proj);
      CHECK_EQUAL(2, result_it);

      auto result_r = etl::ranges::count_if(vec, is_even, proj);
      CHECK_EQUAL(2, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_if_with_projection_no_match)
    {
      std::vector<int> vec{1, -2, 3, -4, 5};
      auto             proj = [](const int& v)
      {
        return v < 0 ? -v : v;
      };
      auto greater_than_10 = [](int v)
      {
        return v > 10;
      };

      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), greater_than_10, proj);
      CHECK_EQUAL(0, result_it);

      auto result_r = etl::ranges::count_if(vec, greater_than_10, proj);
      CHECK_EQUAL(0, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_if_with_member_projection)
    {
      struct Item
      {
        int id;
        int category;
      };

      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 10}, {4, 10}, {5, 30}};
      auto              is_ten = [](int v)
      {
        return v == 10;
      };

      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), is_ten, &Item::category);
      CHECK_EQUAL(3, result_it);

      auto result_r = etl::ranges::count_if(vec, is_ten, &Item::category);
      CHECK_EQUAL(3, result_r);

      auto is_greater_than_15 = [](int v)
      {
        return v > 15;
      };
      auto result_gt = etl::ranges::count_if(vec, is_greater_than_15, &Item::category);
      CHECK_EQUAL(2, result_gt);
    }

    //*************************************************************************
    TEST(ranges_count_if_with_lambda_projection)
    {
      std::vector<int> vec{10, 21, 30, 41, 50};
      auto             proj = [](const int& v)
      {
        return v % 10;
      };
      auto is_zero = [](int v)
      {
        return v == 0;
      };

      // Count elements whose last digit is 0
      auto result_it = etl::ranges::count_if(vec.begin(), vec.end(), is_zero, proj);
      CHECK_EQUAL(3, result_it);

      auto result_r = etl::ranges::count_if(vec, is_zero, proj);
      CHECK_EQUAL(3, result_r);
    }

    //*************************************************************************
    TEST(ranges_count_if_array)
    {
      int  arr[]  = {1, 2, 3, 4, 5, 6};
      auto is_odd = [](int v)
      {
        return v % 2 != 0;
      };

      auto result = etl::ranges::count_if(arr, is_odd);
      CHECK_EQUAL(3, result);

      auto is_greater_than_4 = [](int v)
      {
        return v > 4;
      };
      result = etl::ranges::count_if(arr, is_greater_than_4);
      CHECK_EQUAL(2, result);

      auto is_negative = [](int v)
      {
        return v < 0;
      };
      result = etl::ranges::count_if(arr, is_negative);
      CHECK_EQUAL(0, result);
    }

    //*************************************************************************
    TEST(ranges_mismatch_iterator_basic)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 9, 4, 5};

      auto result = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end());

      CHECK(result.in1 == v1.begin() + 2);
      CHECK(result.in2 == v2.begin() + 2);
      CHECK_EQUAL(3, *result.in1);
      CHECK_EQUAL(9, *result.in2);
    }

    //*************************************************************************
    TEST(ranges_mismatch_range_basic)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 9, 4, 5};

      auto result = etl::ranges::mismatch(v1, v2);

      CHECK(result.in1 == v1.begin() + 2);
      CHECK(result.in2 == v2.begin() + 2);
      CHECK_EQUAL(3, *result.in1);
      CHECK_EQUAL(9, *result.in2);
    }

    //*************************************************************************
    TEST(ranges_mismatch_all_equal)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3, 4, 5};

      auto result_it = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end());
      CHECK(result_it.in1 == v1.end());
      CHECK(result_it.in2 == v2.end());

      auto result_r = etl::ranges::mismatch(v1, v2);
      CHECK(result_r.in1 == v1.end());
      CHECK(result_r.in2 == v2.end());
    }

    //*************************************************************************
    TEST(ranges_mismatch_first_element_differs)
    {
      std::vector<int> v1{9, 2, 3};
      std::vector<int> v2{1, 2, 3};

      auto result_it = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end());
      CHECK(result_it.in1 == v1.begin());
      CHECK(result_it.in2 == v2.begin());

      auto result_r = etl::ranges::mismatch(v1, v2);
      CHECK(result_r.in1 == v1.begin());
      CHECK(result_r.in2 == v2.begin());
    }

    //*************************************************************************
    TEST(ranges_mismatch_different_lengths)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3};

      auto result_it = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end());
      CHECK(result_it.in1 == v1.begin() + 3);
      CHECK(result_it.in2 == v2.end());

      auto result_r = etl::ranges::mismatch(v1, v2);
      CHECK(result_r.in1 == v1.begin() + 3);
      CHECK(result_r.in2 == v2.end());
    }

    //*************************************************************************
    TEST(ranges_mismatch_empty_ranges)
    {
      std::vector<int> v1{};
      std::vector<int> v2{};

      auto result_it = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end());
      CHECK(result_it.in1 == v1.end());
      CHECK(result_it.in2 == v2.end());

      auto result_r = etl::ranges::mismatch(v1, v2);
      CHECK(result_r.in1 == v1.end());
      CHECK(result_r.in2 == v2.end());
    }

    //*************************************************************************
    TEST(ranges_mismatch_first_empty)
    {
      std::vector<int> v1{};
      std::vector<int> v2{1, 2, 3};

      auto result_it = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end());
      CHECK(result_it.in1 == v1.end());
      CHECK(result_it.in2 == v2.begin());

      auto result_r = etl::ranges::mismatch(v1, v2);
      CHECK(result_r.in1 == v1.end());
      CHECK(result_r.in2 == v2.begin());
    }

    //*************************************************************************
    TEST(ranges_mismatch_with_predicate)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{2, 4, 6, 9, 10};
      auto             pred = [](int a, int b)
      {
        return b == a * 2;
      };

      auto result_it = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end(), pred);
      CHECK(result_it.in1 == v1.begin() + 3);
      CHECK(result_it.in2 == v2.begin() + 3);
      CHECK_EQUAL(4, *result_it.in1);
      CHECK_EQUAL(9, *result_it.in2);

      auto result_r = etl::ranges::mismatch(v1, v2, pred);
      CHECK(result_r.in1 == v1.begin() + 3);
      CHECK(result_r.in2 == v2.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_mismatch_with_projection)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}};
      std::vector<Item> v2{{9, 10}, {8, 20}, {7, 99}};

      auto result_it = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value);
      CHECK(result_it.in1 == v1.begin() + 2);
      CHECK(result_it.in2 == v2.begin() + 2);

      auto result_r = etl::ranges::mismatch(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value);
      CHECK(result_r.in1 == v1.begin() + 2);
      CHECK(result_r.in2 == v2.begin() + 2);
    }

    //*************************************************************************
    TEST(ranges_mismatch_with_lambda_projection)
    {
      std::vector<int> v1{-1, -2, -3};
      std::vector<int> v2{1, 2, 9};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };

      auto result_it = etl::ranges::mismatch(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, abs_proj, etl::identity{});
      CHECK(result_it.in1 == v1.begin() + 2);
      CHECK(result_it.in2 == v2.begin() + 2);

      auto result_r = etl::ranges::mismatch(v1, v2, etl::ranges::equal_to{}, abs_proj, etl::identity{});
      CHECK(result_r.in1 == v1.begin() + 2);
      CHECK(result_r.in2 == v2.begin() + 2);
    }

    //*************************************************************************
    TEST(ranges_mismatch_array)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {1, 2, 3, 9, 5};

      auto result = etl::ranges::mismatch(a1, a2);
      CHECK_EQUAL(4, *result.in1);
      CHECK_EQUAL(9, *result.in2);
    }

    //*************************************************************************
    TEST(ranges_mismatch_single_element_match)
    {
      std::vector<int> v1{42};
      std::vector<int> v2{42};

      auto result = etl::ranges::mismatch(v1, v2);
      CHECK(result.in1 == v1.end());
      CHECK(result.in2 == v2.end());
    }

    //*************************************************************************
    TEST(ranges_mismatch_single_element_no_match)
    {
      std::vector<int> v1{42};
      std::vector<int> v2{99};

      auto result = etl::ranges::mismatch(v1, v2);
      CHECK(result.in1 == v1.begin());
      CHECK(result.in2 == v2.begin());
    }

    //*************************************************************************
    TEST(ranges_equal_iterator_basic)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3, 4, 5};

      bool result = etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end());
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_equal_range_basic)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3, 4, 5};

      bool result = etl::ranges::equal(v1, v2);
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_equal_not_equal)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 9, 4, 5};

      CHECK(!etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::equal(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_equal_different_lengths)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3};

      CHECK(!etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::equal(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_equal_empty_ranges)
    {
      std::vector<int> v1{};
      std::vector<int> v2{};

      CHECK(etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::equal(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_equal_first_empty)
    {
      std::vector<int> v1{};
      std::vector<int> v2{1, 2, 3};

      CHECK(!etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::equal(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_equal_with_predicate)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{2, 4, 6, 8, 10};
      auto             pred = [](int a, int b)
      {
        return b == a * 2;
      };

      CHECK(etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end(), pred));
      CHECK(etl::ranges::equal(v1, v2, pred));
    }

    //*************************************************************************
    TEST(ranges_equal_with_predicate_not_equal)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{2, 4, 6, 9, 10};
      auto             pred = [](int a, int b)
      {
        return b == a * 2;
      };

      CHECK(!etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end(), pred));
      CHECK(!etl::ranges::equal(v1, v2, pred));
    }

    //*************************************************************************
    TEST(ranges_equal_with_projection)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}};
      std::vector<Item> v2{{9, 10}, {8, 20}, {7, 30}};

      CHECK(etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value));
      CHECK(etl::ranges::equal(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_equal_with_projection_not_equal)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}};
      std::vector<Item> v2{{9, 10}, {8, 20}, {7, 99}};

      CHECK(!etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value));
      CHECK(!etl::ranges::equal(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_equal_with_lambda_projection)
    {
      std::vector<int> v1{-1, -2, -3};
      std::vector<int> v2{1, 2, 3};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };

      CHECK(etl::ranges::equal(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, abs_proj, etl::identity{}));
      CHECK(etl::ranges::equal(v1, v2, etl::ranges::equal_to{}, abs_proj, etl::identity{}));
    }

    //*************************************************************************
    TEST(ranges_equal_array)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {1, 2, 3, 4, 5};

      CHECK(etl::ranges::equal(a1, a2));
    }

    //*************************************************************************
    TEST(ranges_equal_array_not_equal)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {1, 2, 3, 9, 5};

      CHECK(!etl::ranges::equal(a1, a2));
    }

    //*************************************************************************
    TEST(ranges_equal_single_element_match)
    {
      std::vector<int> v1{42};
      std::vector<int> v2{42};

      CHECK(etl::ranges::equal(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_equal_single_element_no_match)
    {
      std::vector<int> v1{42};
      std::vector<int> v2{99};

      CHECK(!etl::ranges::equal(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_iterator_basic)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{3, 1, 5, 2, 4};

      CHECK(etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end()));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_range_basic)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{3, 1, 5, 2, 4};

      CHECK(etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_not_permutation)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3, 4, 4};

      CHECK(!etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_different_lengths)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3};

      CHECK(!etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_empty_ranges)
    {
      std::vector<int> v1{};
      std::vector<int> v2{};

      CHECK(etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_first_empty)
    {
      std::vector<int> v1{};
      std::vector<int> v2{1, 2, 3};

      CHECK(!etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_with_duplicates)
    {
      std::vector<int> v1{1, 2, 2, 3, 3, 3};
      std::vector<int> v2{3, 2, 3, 1, 3, 2};

      CHECK(etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_with_duplicates_mismatch)
    {
      std::vector<int> v1{1, 2, 2, 3, 3, 3};
      std::vector<int> v2{3, 2, 3, 1, 2, 2};

      CHECK(!etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_single_element_match)
    {
      std::vector<int> v1{42};
      std::vector<int> v2{42};

      CHECK(etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_single_element_no_match)
    {
      std::vector<int> v1{42};
      std::vector<int> v2{99};

      CHECK(!etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_with_predicate)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{5, 3, 1, 4, 2};
      auto             pred = [](int a, int b)
      {
        return a == b;
      };

      CHECK(etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end(), pred));
      CHECK(etl::ranges::is_permutation(v1, v2, pred));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_with_predicate_not_permutation)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3, 4, 4};
      auto             pred = [](int a, int b)
      {
        return a == b;
      };

      CHECK(!etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end(), pred));
      CHECK(!etl::ranges::is_permutation(v1, v2, pred));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_with_projection)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}};
      std::vector<Item> v2{{9, 30}, {8, 10}, {7, 20}};

      CHECK(etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value));
      CHECK(etl::ranges::is_permutation(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_with_projection_not_permutation)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}};
      std::vector<Item> v2{{9, 10}, {8, 20}, {7, 99}};

      CHECK(!etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value));
      CHECK(!etl::ranges::is_permutation(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_with_lambda_projection)
    {
      std::vector<int> v1{-1, -2, -3};
      std::vector<int> v2{3, 1, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };

      CHECK(etl::ranges::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, abs_proj, etl::identity{}));
      CHECK(etl::ranges::is_permutation(v1, v2, etl::ranges::equal_to{}, abs_proj, etl::identity{}));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_array)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {5, 4, 3, 2, 1};

      CHECK(etl::ranges::is_permutation(a1, a2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_array_not_permutation)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {1, 2, 3, 9, 5};

      CHECK(!etl::ranges::is_permutation(a1, a2));
    }

    //*************************************************************************
    TEST(ranges_is_permutation_identical)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3, 4, 5};

      CHECK(etl::ranges::is_permutation(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_iterator_basic)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3};

      CHECK(etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
    }

    //*************************************************************************
    TEST(ranges_starts_with_range_basic)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3};

      CHECK(etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_not_prefix)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 9, 3};

      CHECK(!etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_prefix_longer_than_range)
    {
      std::vector<int> v1{1, 2, 3};
      std::vector<int> v2{1, 2, 3, 4, 5};

      CHECK(!etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_equal_ranges)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3, 4, 5};

      CHECK(etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_empty_prefix)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{};

      CHECK(etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_both_empty)
    {
      std::vector<int> v1{};
      std::vector<int> v2{};

      CHECK(etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_empty_range_nonempty_prefix)
    {
      std::vector<int> v1{};
      std::vector<int> v2{1, 2, 3};

      CHECK(!etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_single_element_match)
    {
      std::vector<int> v1{42, 1, 2};
      std::vector<int> v2{42};

      CHECK(etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_single_element_no_match)
    {
      std::vector<int> v1{42, 1, 2};
      std::vector<int> v2{99};

      CHECK(!etl::ranges::starts_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_with_predicate)
    {
      std::vector<int> v1{2, 4, 6, 8, 10};
      std::vector<int> v2{1, 2, 3};
      auto             pred = [](int a, int b)
      {
        return a == b * 2;
      };

      CHECK(etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end(), pred));
      CHECK(etl::ranges::starts_with(v1, v2, pred));
    }

    //*************************************************************************
    TEST(ranges_starts_with_with_predicate_no_match)
    {
      std::vector<int> v1{2, 4, 7, 8, 10};
      std::vector<int> v2{1, 2, 3};
      auto             pred = [](int a, int b)
      {
        return a == b * 2;
      };

      CHECK(!etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end(), pred));
      CHECK(!etl::ranges::starts_with(v1, v2, pred));
    }

    //*************************************************************************
    TEST(ranges_starts_with_with_projection)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}, {4, 40}};
      std::vector<Item> v2{{9, 10}, {8, 20}};

      CHECK(etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value));
      CHECK(etl::ranges::starts_with(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_starts_with_with_projection_no_match)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}, {4, 40}};
      std::vector<Item> v2{{9, 10}, {8, 99}};

      CHECK(!etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value));
      CHECK(!etl::ranges::starts_with(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_starts_with_with_lambda_projection)
    {
      std::vector<int> v1{-1, -2, -3, -4, -5};
      std::vector<int> v2{1, 2, 3};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };

      CHECK(etl::ranges::starts_with(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, abs_proj, etl::identity{}));
      CHECK(etl::ranges::starts_with(v1, v2, etl::ranges::equal_to{}, abs_proj, etl::identity{}));
    }

    //*************************************************************************
    TEST(ranges_starts_with_array)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {1, 2, 3};

      CHECK(etl::ranges::starts_with(a1, a2));
    }

    //*************************************************************************
    TEST(ranges_starts_with_array_not_prefix)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {1, 9, 3};

      CHECK(!etl::ranges::starts_with(a1, a2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_match)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{3, 4, 5};

      CHECK(etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_no_match)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{3, 4, 6};

      CHECK(!etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_suffix_longer_than_range)
    {
      std::vector<int> v1{1, 2, 3};
      std::vector<int> v2{1, 2, 3, 4, 5};

      CHECK(!etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_equal_ranges)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{1, 2, 3, 4, 5};

      CHECK(etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_empty_suffix)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{};

      CHECK(etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_both_empty)
    {
      std::vector<int> v1{};
      std::vector<int> v2{};

      CHECK(etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_empty_range_nonempty_suffix)
    {
      std::vector<int> v1{};
      std::vector<int> v2{1, 2, 3};

      CHECK(!etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_single_element_match)
    {
      std::vector<int> v1{1, 2, 42};
      std::vector<int> v2{42};

      CHECK(etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_single_element_no_match)
    {
      std::vector<int> v1{1, 2, 42};
      std::vector<int> v2{99};

      CHECK(!etl::ranges::ends_with(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_with_predicate)
    {
      std::vector<int> v1{2, 4, 6, 8, 10};
      std::vector<int> v2{4, 5};
      auto             pred = [](int a, int b)
      {
        return a == b * 2;
      };

      CHECK(etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end(), pred));
      CHECK(etl::ranges::ends_with(v1, v2, pred));
    }

    //*************************************************************************
    TEST(ranges_ends_with_with_predicate_no_match)
    {
      std::vector<int> v1{2, 4, 7, 8, 10};
      std::vector<int> v2{4, 6};
      auto             pred = [](int a, int b)
      {
        return a == b * 2;
      };

      CHECK(!etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end(), pred));
      CHECK(!etl::ranges::ends_with(v1, v2, pred));
    }

    //*************************************************************************
    TEST(ranges_ends_with_with_projection)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}, {4, 40}};
      std::vector<Item> v2{{9, 30}, {8, 40}};

      CHECK(etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value));
      CHECK(etl::ranges::ends_with(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_ends_with_with_projection_no_match)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}, {4, 40}};
      std::vector<Item> v2{{9, 30}, {8, 99}};

      CHECK(!etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, &Item::value, &Item::value));
      CHECK(!etl::ranges::ends_with(v1, v2, etl::ranges::equal_to{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_ends_with_with_lambda_projection)
    {
      std::vector<int> v1{-1, -2, -3, -4, -5};
      std::vector<int> v2{4, 5};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };

      CHECK(etl::ranges::ends_with(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::equal_to{}, abs_proj, etl::identity{}));
      CHECK(etl::ranges::ends_with(v1, v2, etl::ranges::equal_to{}, abs_proj, etl::identity{}));
    }

    //*************************************************************************
    TEST(ranges_ends_with_array)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {3, 4, 5};

      CHECK(etl::ranges::ends_with(a1, a2));
    }

    //*************************************************************************
    TEST(ranges_ends_with_array_not_suffix)
    {
      int a1[] = {1, 2, 3, 4, 5};
      int a2[] = {3, 9, 5};

      CHECK(!etl::ranges::ends_with(a1, a2));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_iterator_less)
    {
      std::vector<int> v1{1, 2, 3};
      std::vector<int> v2{1, 2, 4};

      CHECK(etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::lexicographical_compare(v2.begin(), v2.end(), v1.begin(), v1.end()));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_range_less)
    {
      std::vector<int> v1{1, 2, 3};
      std::vector<int> v2{1, 2, 4};

      CHECK(etl::ranges::lexicographical_compare(v1, v2));
      CHECK(!etl::ranges::lexicographical_compare(v2, v1));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_equal_ranges)
    {
      std::vector<int> v1{1, 2, 3};
      std::vector<int> v2{1, 2, 3};

      CHECK(!etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::lexicographical_compare(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_first_shorter)
    {
      std::vector<int> v1{1, 2};
      std::vector<int> v2{1, 2, 3};

      CHECK(etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::lexicographical_compare(v1, v2));
      CHECK(!etl::ranges::lexicographical_compare(v2, v1));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_second_shorter)
    {
      std::vector<int> v1{1, 2, 3};
      std::vector<int> v2{1, 2};

      CHECK(!etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::lexicographical_compare(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_both_empty)
    {
      std::vector<int> v1{};
      std::vector<int> v2{};

      CHECK(!etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::lexicographical_compare(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_first_empty)
    {
      std::vector<int> v1{};
      std::vector<int> v2{1, 2, 3};

      CHECK(etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(etl::ranges::lexicographical_compare(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_second_empty)
    {
      std::vector<int> v1{1, 2, 3};
      std::vector<int> v2{};

      CHECK(!etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()));
      CHECK(!etl::ranges::lexicographical_compare(v1, v2));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_with_comparator)
    {
      std::vector<int> v1{3, 2, 1};
      std::vector<int> v2{3, 2, 0};
      auto             comp = [](int a, int b)
      {
        return a > b;
      };

      CHECK(etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), comp));
      CHECK(etl::ranges::lexicographical_compare(v1, v2, comp));
      CHECK(!etl::ranges::lexicographical_compare(v2, v1, comp));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_with_projection)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 30}};
      std::vector<Item> v2{{9, 10}, {8, 20}, {7, 40}};

      CHECK(etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::less{}, &Item::value, &Item::value));
      CHECK(etl::ranges::lexicographical_compare(v1, v2, etl::ranges::less{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_with_projection_not_less)
    {
      struct Item
      {
        int id;
        int value;
      };

      std::vector<Item> v1{{1, 10}, {2, 20}, {3, 40}};
      std::vector<Item> v2{{9, 10}, {8, 20}, {7, 30}};

      CHECK(!etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::less{}, &Item::value, &Item::value));
      CHECK(!etl::ranges::lexicographical_compare(v1, v2, etl::ranges::less{}, &Item::value, &Item::value));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_with_lambda_projection)
    {
      std::vector<int> v1{-1, -2, -3};
      std::vector<int> v2{1, 2, 4};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };

      CHECK(etl::ranges::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), etl::ranges::less{}, abs_proj, etl::identity{}));
      CHECK(etl::ranges::lexicographical_compare(v1, v2, etl::ranges::less{}, abs_proj, etl::identity{}));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_array)
    {
      int a1[] = {1, 2, 3};
      int a2[] = {1, 2, 4};

      CHECK(etl::ranges::lexicographical_compare(a1, a2));
      CHECK(!etl::ranges::lexicographical_compare(a2, a1));
    }

    //*************************************************************************
    TEST(ranges_lexicographical_compare_single_element)
    {
      std::vector<int> v1{1};
      std::vector<int> v2{2};

      CHECK(etl::ranges::lexicographical_compare(v1, v2));
      CHECK(!etl::ranges::lexicographical_compare(v2, v1));
      CHECK(!etl::ranges::lexicographical_compare(v1, v1));
    }

    //*************************************************************************
    TEST(ranges_fold_left_sum_iterators)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left(v.begin(), v.end(), 0, std::plus<int>{});
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_sum_range)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left(v, 0, std::plus<int>{});
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_empty_range)
    {
      std::vector<int> v{};
      auto             result = etl::ranges::fold_left(v.begin(), v.end(), 42, std::plus<int>{});
      CHECK_EQUAL(42, result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_product)
    {
      std::vector<int> v{1, 2, 3, 4};
      auto             result = etl::ranges::fold_left(v, 1, std::multiplies<int>{});
      CHECK_EQUAL(24, result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_string_concat)
    {
      std::vector<std::string> v{"a", "b", "c"};
      auto                     result = etl::ranges::fold_left(v, std::string{}, std::plus<std::string>{});
      CHECK_EQUAL(std::string("abc"), result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_different_init_type)
    {
      std::vector<int> v{1, 2, 3};
      auto             result = etl::ranges::fold_left(v, 0.5, [](double acc, int x) { return acc + x; });
      CHECK_CLOSE(6.5, result, 0.001);
    }

    //*************************************************************************
    TEST(ranges_fold_left_array)
    {
      int  a[]    = {10, 20, 30};
      auto result = etl::ranges::fold_left(a, 0, std::plus<int>{});
      CHECK_EQUAL(60, result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_with_iter_sum)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left_with_iter(v.begin(), v.end(), 0, std::plus<int>{});
      CHECK_EQUAL(15, result.value);
      CHECK(result.in == v.end());
    }

    //*************************************************************************
    TEST(ranges_fold_left_with_iter_range)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left_with_iter(v, 0, std::plus<int>{});
      CHECK_EQUAL(15, result.value);
    }

    //*************************************************************************
    TEST(ranges_fold_left_with_iter_empty)
    {
      std::vector<int> v{};
      auto             result = etl::ranges::fold_left_with_iter(v.begin(), v.end(), 99, std::plus<int>{});
      CHECK_EQUAL(99, result.value);
      CHECK(result.in == v.end());
    }

    //*************************************************************************
    TEST(ranges_fold_left_first_sum)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left_first(v.begin(), v.end(), std::plus<int>{});
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_first_range)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left_first(v, std::plus<int>{});
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_first_single_element)
    {
      std::vector<int> v{42};
      auto             result = etl::ranges::fold_left_first(v, std::plus<int>{});
      CHECK_EQUAL(42, result);
    }

    //*************************************************************************
    TEST(ranges_fold_left_first_with_iter_sum)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left_first_with_iter(v.begin(), v.end(), std::plus<int>{});
      CHECK_EQUAL(15, result.value);
      CHECK(result.in == v.end());
    }

    //*************************************************************************
    TEST(ranges_fold_left_first_with_iter_range)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left_first_with_iter(v, std::plus<int>{});
      CHECK_EQUAL(15, result.value);
    }

    //*************************************************************************
    TEST(ranges_fold_left_lambda)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_left(v, 0, [](int acc, int x) { return acc + x * x; });
      CHECK_EQUAL(55, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_sum_iterators)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_right(v.begin(), v.end(), 0, std::plus<int>{});
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_sum_range)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_right(v, 0, std::plus<int>{});
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_empty_range)
    {
      std::vector<int> v{};
      auto             result = etl::ranges::fold_right(v.begin(), v.end(), 42, std::plus<int>{});
      CHECK_EQUAL(42, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_product)
    {
      std::vector<int> v{1, 2, 3, 4};
      auto             result = etl::ranges::fold_right(v, 1, std::multiplies<int>{});
      CHECK_EQUAL(24, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_string_concat)
    {
      std::vector<std::string> v{"a", "b", "c"};
      auto                     result = etl::ranges::fold_right(v, std::string{}, std::plus<std::string>{});
      CHECK_EQUAL(std::string("abc"), result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_different_init_type)
    {
      std::vector<int> v{1, 2, 3};
      auto             result = etl::ranges::fold_right(v, 0.5, [](int x, double acc) { return acc + x; });
      CHECK_CLOSE(6.5, result, 0.001);
    }

    //*************************************************************************
    TEST(ranges_fold_right_array)
    {
      int  a[]    = {10, 20, 30};
      auto result = etl::ranges::fold_right(a, 0, std::plus<int>{});
      CHECK_EQUAL(60, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_subtraction_order)
    {
      // fold_right({1,2,3}, 0, minus) = 1 - (2 - (3 - 0)) = 1 - (2 - 3) = 1 -
      // (-1) = 2
      std::vector<int> v{1, 2, 3};
      auto             result = etl::ranges::fold_right(v, 0, std::minus<int>{});
      CHECK_EQUAL(2, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_lambda)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_right(v, 0, [](int x, int acc) { return acc + x * x; });
      CHECK_EQUAL(55, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_last_sum)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_right_last(v.begin(), v.end(), std::plus<int>{});
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_last_range)
    {
      std::vector<int> v{1, 2, 3, 4, 5};
      auto             result = etl::ranges::fold_right_last(v, std::plus<int>{});
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_last_single_element)
    {
      std::vector<int> v{42};
      auto             result = etl::ranges::fold_right_last(v, std::plus<int>{});
      CHECK_EQUAL(42, result);
    }

    //*************************************************************************
    TEST(ranges_fold_right_last_subtraction_order)
    {
      // fold_right_last({1,2,3,4}, minus) = 1 - (2 - (3 - 4)) = 1 - (2 - (-1))
      // = 1 - 3 = -2
      std::vector<int> v{1, 2, 3, 4};
      auto             result = etl::ranges::fold_right_last(v, std::minus<int>{});
      CHECK_EQUAL(-2, result);
    }

    //*************************************************************************
    TEST(ranges_copy_iterator)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::copy(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
      CHECK_EQUAL(4, dst[3]);
      CHECK_EQUAL(5, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_copy_range)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::copy(src, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
      CHECK_EQUAL(4, dst[3]);
      CHECK_EQUAL(5, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_copy_empty)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::copy(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_copy_empty_range)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::copy(src, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_copy_single_element)
    {
      std::vector<int> src{42};
      std::vector<int> dst(1);

      auto result = etl::ranges::copy(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(42, dst[0]);
    }

    //*************************************************************************
    TEST(ranges_copy_array)
    {
      int src[]  = {10, 20, 30};
      int dst[3] = {};

      auto result = etl::ranges::copy(src, dst);
      (void)result;

      CHECK_EQUAL(10, dst[0]);
      CHECK_EQUAL(20, dst[1]);
      CHECK_EQUAL(30, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_to_different_container)
    {
      std::vector<int>   src{1, 2, 3};
      std::array<int, 3> dst{};

      auto result = etl::ranges::copy(src, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_if_iterator)
    {
      std::vector<int> src{1, 2, 3, 4, 5, 6};
      std::vector<int> dst(3);

      auto result = etl::ranges::copy_if(src.begin(), src.end(), dst.begin(), [](int x) { return x % 2 == 0; });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin() + 3);
      CHECK_EQUAL(2, dst[0]);
      CHECK_EQUAL(4, dst[1]);
      CHECK_EQUAL(6, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_if_range)
    {
      std::vector<int> src{1, 2, 3, 4, 5, 6};
      std::vector<int> dst(3);

      auto result = etl::ranges::copy_if(src, dst.begin(), [](int x) { return x % 2 == 0; });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin() + 3);
      CHECK_EQUAL(2, dst[0]);
      CHECK_EQUAL(4, dst[1]);
      CHECK_EQUAL(6, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_if_none_match)
    {
      std::vector<int> src{1, 3, 5};
      std::vector<int> dst(3, 0);

      auto result = etl::ranges::copy_if(src.begin(), src.end(), dst.begin(), [](int x) { return x % 2 == 0; });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(0, dst[0]);
      CHECK_EQUAL(0, dst[1]);
      CHECK_EQUAL(0, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_if_all_match)
    {
      std::vector<int> src{2, 4, 6};
      std::vector<int> dst(3);

      auto result = etl::ranges::copy_if(src, dst.begin(), [](int x) { return x % 2 == 0; });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(2, dst[0]);
      CHECK_EQUAL(4, dst[1]);
      CHECK_EQUAL(6, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_if_empty)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::copy_if(src.begin(), src.end(), dst.begin(), [](int) { return true; });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_copy_if_with_projection)
    {
      struct Item
      {
        int value;
      };
      std::vector<Item> src{{1}, {2}, {3}, {4}, {5}, {6}};
      std::vector<Item> dst(3);

      auto result = etl::ranges::copy_if(src.begin(), src.end(), dst.begin(), [](int v) { return v % 2 == 0; }, &Item::value);

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin() + 3);
      CHECK_EQUAL(2, dst[0].value);
      CHECK_EQUAL(4, dst[1].value);
      CHECK_EQUAL(6, dst[2].value);
    }

    //*************************************************************************
    TEST(ranges_copy_if_range_with_projection)
    {
      struct Item
      {
        int value;
      };
      std::vector<Item> src{{1}, {2}, {3}, {4}, {5}, {6}};
      std::vector<Item> dst(3);

      auto result = etl::ranges::copy_if(src, dst.begin(), [](int v) { return v % 2 == 0; }, &Item::value);

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin() + 3);
      CHECK_EQUAL(2, dst[0].value);
      CHECK_EQUAL(4, dst[1].value);
      CHECK_EQUAL(6, dst[2].value);
    }

    //*************************************************************************
    TEST(ranges_copy_n_basic)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(3);

      auto result = etl::ranges::copy_n(src.begin(), 3, dst.begin());

      CHECK(result.in == src.begin() + 3);
      CHECK(result.out == dst.end());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_n_zero)
    {
      std::vector<int> src{1, 2, 3};
      std::vector<int> dst(3, 0);

      auto result = etl::ranges::copy_n(src.begin(), 0, dst.begin());

      CHECK(result.in == src.begin());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(0, dst[0]);
      CHECK_EQUAL(0, dst[1]);
      CHECK_EQUAL(0, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_n_all_elements)
    {
      std::vector<int> src{10, 20, 30, 40, 50};
      std::vector<int> dst(5);

      auto result = etl::ranges::copy_n(src.begin(), 5, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(10, dst[0]);
      CHECK_EQUAL(20, dst[1]);
      CHECK_EQUAL(30, dst[2]);
      CHECK_EQUAL(40, dst[3]);
      CHECK_EQUAL(50, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_copy_n_single_element)
    {
      std::vector<int> src{42, 99};
      std::vector<int> dst(1);

      auto result = etl::ranges::copy_n(src.begin(), 1, dst.begin());

      CHECK(result.in == src.begin() + 1);
      CHECK(result.out == dst.end());
      CHECK_EQUAL(42, dst[0]);
    }

    //*************************************************************************
    TEST(ranges_copy_n_to_different_container)
    {
      std::vector<int>   src{1, 2, 3, 4, 5};
      std::array<int, 3> dst{};

      auto result = etl::ranges::copy_n(src.begin(), 3, dst.begin());

      CHECK(result.in == src.begin() + 3);
      CHECK(result.out == dst.end());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_backward_iterator)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::copy_backward(src.begin(), src.end(), dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
      CHECK_EQUAL(4, dst[3]);
      CHECK_EQUAL(5, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_copy_backward_range)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::copy_backward(src, dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
      CHECK_EQUAL(4, dst[3]);
      CHECK_EQUAL(5, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_copy_backward_empty)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::copy_backward(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_copy_backward_empty_range)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::copy_backward(src, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_copy_backward_single_element)
    {
      std::vector<int> src{42};
      std::vector<int> dst(1);

      auto result = etl::ranges::copy_backward(src.begin(), src.end(), dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(42, dst[0]);
    }

    //*************************************************************************
    TEST(ranges_copy_backward_to_different_container)
    {
      std::vector<int>   src{1, 2, 3};
      std::array<int, 3> dst{};

      auto result = etl::ranges::copy_backward(src, dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_copy_backward_overlapping)
    {
      std::vector<int> v{1, 2, 3, 4, 5, 0, 0};

      auto result = etl::ranges::copy_backward(v.begin(), v.begin() + 5, v.end());

      CHECK(result.in == v.begin() + 5);
      CHECK(result.out == v.begin() + 2);
      CHECK_EQUAL(1, v[0]);
      CHECK_EQUAL(2, v[1]);
      CHECK_EQUAL(1, v[2]);
      CHECK_EQUAL(2, v[3]);
      CHECK_EQUAL(3, v[4]);
      CHECK_EQUAL(4, v[5]);
      CHECK_EQUAL(5, v[6]);
    }

    //*************************************************************************
    TEST(ranges_move_iterator)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::move(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
      CHECK_EQUAL(4, dst[3]);
      CHECK_EQUAL(5, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_move_range)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::move(src, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
      CHECK_EQUAL(4, dst[3]);
      CHECK_EQUAL(5, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_move_empty)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::move(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_move_empty_range)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::move(src, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_move_single_element)
    {
      std::vector<int> src{42};
      std::vector<int> dst(1);

      auto result = etl::ranges::move(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(42, dst[0]);
    }

    //*************************************************************************
    TEST(ranges_move_unique_ptr)
    {
      std::vector<std::unique_ptr<int>> src;
      src.push_back(std::make_unique<int>(10));
      src.push_back(std::make_unique<int>(20));
      src.push_back(std::make_unique<int>(30));

      std::vector<std::unique_ptr<int>> dst(3);

      auto result = etl::ranges::move(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(10, *dst[0]);
      CHECK_EQUAL(20, *dst[1]);
      CHECK_EQUAL(30, *dst[2]);
      CHECK(src[0] == nullptr);
      CHECK(src[1] == nullptr);
      CHECK(src[2] == nullptr);
    }

    //*************************************************************************
    TEST(ranges_move_to_different_container)
    {
      std::vector<int>   src{1, 2, 3};
      std::array<int, 3> dst{};

      auto result = etl::ranges::move(src, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_move_backward_iterator)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::move_backward(src.begin(), src.end(), dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
      CHECK_EQUAL(4, dst[3]);
      CHECK_EQUAL(5, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_move_backward_range)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::move_backward(src, dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
      CHECK_EQUAL(4, dst[3]);
      CHECK_EQUAL(5, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_move_backward_empty)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::move_backward(src.begin(), src.end(), dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_move_backward_empty_range)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::move_backward(src, dst.begin());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_move_backward_single_element)
    {
      std::vector<int> src{42};
      std::vector<int> dst(1);

      auto result = etl::ranges::move_backward(src.begin(), src.end(), dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(42, dst[0]);
    }

    //*************************************************************************
    TEST(ranges_move_backward_unique_ptr)
    {
      std::vector<std::unique_ptr<int>> src;
      src.push_back(std::make_unique<int>(10));
      src.push_back(std::make_unique<int>(20));
      src.push_back(std::make_unique<int>(30));

      std::vector<std::unique_ptr<int>> dst(3);

      auto result = etl::ranges::move_backward(src.begin(), src.end(), dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(10, *dst[0]);
      CHECK_EQUAL(20, *dst[1]);
      CHECK_EQUAL(30, *dst[2]);
      CHECK(src[0] == nullptr);
      CHECK(src[1] == nullptr);
      CHECK(src[2] == nullptr);
    }

    //*************************************************************************
    TEST(ranges_move_backward_to_different_container)
    {
      std::vector<int>   src{1, 2, 3};
      std::array<int, 3> dst{};

      auto result = etl::ranges::move_backward(src, dst.end());

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
      CHECK_EQUAL(1, dst[0]);
      CHECK_EQUAL(2, dst[1]);
      CHECK_EQUAL(3, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_move_backward_overlapping)
    {
      std::vector<int> v{1, 2, 3, 4, 5, 0, 0};

      auto result = etl::ranges::move_backward(v.begin(), v.begin() + 5, v.end());

      CHECK(result.in == v.begin() + 5);
      CHECK(result.out == v.begin() + 2);
      CHECK_EQUAL(1, v[0]);
      CHECK_EQUAL(2, v[1]);
      CHECK_EQUAL(1, v[2]);
      CHECK_EQUAL(2, v[3]);
      CHECK_EQUAL(3, v[4]);
      CHECK_EQUAL(4, v[5]);
      CHECK_EQUAL(5, v[6]);
    }

    //*************************************************************************
    TEST(ranges_swap_ranges_iterator)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{6, 7, 8, 9, 10};

      auto result = etl::ranges::swap_ranges(v1.begin(), v1.end(), v2.begin(), v2.end());

      CHECK(result.in1 == v1.end());
      CHECK(result.in2 == v2.end());
      CHECK_EQUAL(6, v1[0]);
      CHECK_EQUAL(7, v1[1]);
      CHECK_EQUAL(8, v1[2]);
      CHECK_EQUAL(9, v1[3]);
      CHECK_EQUAL(10, v1[4]);
      CHECK_EQUAL(1, v2[0]);
      CHECK_EQUAL(2, v2[1]);
      CHECK_EQUAL(3, v2[2]);
      CHECK_EQUAL(4, v2[3]);
      CHECK_EQUAL(5, v2[4]);
    }

    //*************************************************************************
    TEST(ranges_swap_ranges_range)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{6, 7, 8, 9, 10};

      auto result = etl::ranges::swap_ranges(v1, v2);

      CHECK(result.in1 == v1.end());
      CHECK(result.in2 == v2.end());
      CHECK_EQUAL(6, v1[0]);
      CHECK_EQUAL(7, v1[1]);
      CHECK_EQUAL(8, v1[2]);
      CHECK_EQUAL(9, v1[3]);
      CHECK_EQUAL(10, v1[4]);
      CHECK_EQUAL(1, v2[0]);
      CHECK_EQUAL(2, v2[1]);
      CHECK_EQUAL(3, v2[2]);
      CHECK_EQUAL(4, v2[3]);
      CHECK_EQUAL(5, v2[4]);
    }

    //*************************************************************************
    TEST(ranges_swap_ranges_empty)
    {
      std::vector<int> v1{};
      std::vector<int> v2{};

      auto result = etl::ranges::swap_ranges(v1.begin(), v1.end(), v2.begin(), v2.end());

      CHECK(result.in1 == v1.end());
      CHECK(result.in2 == v2.end());
    }

    //*************************************************************************
    TEST(ranges_swap_ranges_empty_range)
    {
      std::vector<int> v1{};
      std::vector<int> v2{};

      auto result = etl::ranges::swap_ranges(v1, v2);

      CHECK(result.in1 == v1.end());
      CHECK(result.in2 == v2.end());
    }

    //*************************************************************************
    TEST(ranges_swap_ranges_different_lengths_first_shorter)
    {
      std::vector<int> v1{1, 2, 3};
      std::vector<int> v2{6, 7, 8, 9, 10};

      auto result = etl::ranges::swap_ranges(v1.begin(), v1.end(), v2.begin(), v2.end());

      CHECK(result.in1 == v1.end());
      CHECK(result.in2 == v2.begin() + 3);
      CHECK_EQUAL(6, v1[0]);
      CHECK_EQUAL(7, v1[1]);
      CHECK_EQUAL(8, v1[2]);
      CHECK_EQUAL(1, v2[0]);
      CHECK_EQUAL(2, v2[1]);
      CHECK_EQUAL(3, v2[2]);
      CHECK_EQUAL(9, v2[3]);
      CHECK_EQUAL(10, v2[4]);
    }

    //*************************************************************************
    TEST(ranges_swap_ranges_different_lengths_second_shorter)
    {
      std::vector<int> v1{1, 2, 3, 4, 5};
      std::vector<int> v2{6, 7, 8};

      auto result = etl::ranges::swap_ranges(v1, v2);

      CHECK(result.in1 == v1.begin() + 3);
      CHECK(result.in2 == v2.end());
      CHECK_EQUAL(6, v1[0]);
      CHECK_EQUAL(7, v1[1]);
      CHECK_EQUAL(8, v1[2]);
      CHECK_EQUAL(4, v1[3]);
      CHECK_EQUAL(5, v1[4]);
      CHECK_EQUAL(1, v2[0]);
      CHECK_EQUAL(2, v2[1]);
      CHECK_EQUAL(3, v2[2]);
    }

    //*************************************************************************
    TEST(ranges_swap_ranges_single_element)
    {
      std::vector<int> v1{42};
      std::vector<int> v2{99};

      auto result = etl::ranges::swap_ranges(v1.begin(), v1.end(), v2.begin(), v2.end());

      CHECK(result.in1 == v1.end());
      CHECK(result.in2 == v2.end());
      CHECK_EQUAL(99, v1[0]);
      CHECK_EQUAL(42, v2[0]);
    }

    //*************************************************************************
    TEST(ranges_swap_ranges_array)
    {
      int a1[] = {10, 20, 30};
      int a2[] = {40, 50, 60};

      auto result = etl::ranges::swap_ranges(a1, a2);
      (void)result;

      CHECK_EQUAL(40, a1[0]);
      CHECK_EQUAL(50, a1[1]);
      CHECK_EQUAL(60, a1[2]);
      CHECK_EQUAL(10, a2[0]);
      CHECK_EQUAL(20, a2[1]);
      CHECK_EQUAL(30, a2[2]);
    }

    //*************************************************************************
    TEST(ranges_transform_unary_iterator)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::transform(src.begin(), src.end(), dst.begin(), [](int x) { return x * 2; });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(2, dst[0]);
      CHECK_EQUAL(4, dst[1]);
      CHECK_EQUAL(6, dst[2]);
      CHECK_EQUAL(8, dst[3]);
      CHECK_EQUAL(10, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_transform_unary_range)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dst(5);

      auto result = etl::ranges::transform(src, dst.begin(), [](int x) { return x * 2; });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(2, dst[0]);
      CHECK_EQUAL(4, dst[1]);
      CHECK_EQUAL(6, dst[2]);
      CHECK_EQUAL(8, dst[3]);
      CHECK_EQUAL(10, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_transform_unary_empty)
    {
      std::vector<int> src{};
      std::vector<int> dst{};

      auto result = etl::ranges::transform(src.begin(), src.end(), dst.begin(), [](int x) { return x; });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_transform_unary_with_projection)
    {
      struct Item
      {
        int value;
      };
      std::vector<Item> src{{1}, {2}, {3}};
      std::vector<int>  dst(3);

      auto result = etl::ranges::transform(src.begin(), src.end(), dst.begin(), [](int v) { return v * 10; }, &Item::value);

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(10, dst[0]);
      CHECK_EQUAL(20, dst[1]);
      CHECK_EQUAL(30, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_transform_unary_range_with_projection)
    {
      struct Item
      {
        int value;
      };
      std::vector<Item> src{{1}, {2}, {3}};
      std::vector<int>  dst(3);

      auto result = etl::ranges::transform(src, dst.begin(), [](int v) { return v * 10; }, &Item::value);

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(10, dst[0]);
      CHECK_EQUAL(20, dst[1]);
      CHECK_EQUAL(30, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_transform_binary_iterator)
    {
      std::vector<int> src1{1, 2, 3, 4, 5};
      std::vector<int> src2{10, 20, 30, 40, 50};
      std::vector<int> dst(5);

      auto result = etl::ranges::transform(src1.begin(), src1.end(), src2.begin(), src2.end(), dst.begin(), [](int a, int b) { return a + b; });

      CHECK(result.in1 == src1.end());
      CHECK(result.in2 == src2.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(11, dst[0]);
      CHECK_EQUAL(22, dst[1]);
      CHECK_EQUAL(33, dst[2]);
      CHECK_EQUAL(44, dst[3]);
      CHECK_EQUAL(55, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_transform_binary_range)
    {
      std::vector<int> src1{1, 2, 3, 4, 5};
      std::vector<int> src2{10, 20, 30, 40, 50};
      std::vector<int> dst(5);

      auto result = etl::ranges::transform(src1, src2, dst.begin(), [](int a, int b) { return a + b; });

      CHECK(result.in1 == src1.end());
      CHECK(result.in2 == src2.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(11, dst[0]);
      CHECK_EQUAL(22, dst[1]);
      CHECK_EQUAL(33, dst[2]);
      CHECK_EQUAL(44, dst[3]);
      CHECK_EQUAL(55, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_transform_binary_different_lengths)
    {
      std::vector<int> src1{1, 2, 3, 4, 5};
      std::vector<int> src2{10, 20, 30};
      std::vector<int> dst(5, 0);

      auto result = etl::ranges::transform(src1.begin(), src1.end(), src2.begin(), src2.end(), dst.begin(), [](int a, int b) { return a + b; });

      CHECK(result.in1 == src1.begin() + 3);
      CHECK(result.in2 == src2.end());
      CHECK(result.out == dst.begin() + 3);
      CHECK_EQUAL(11, dst[0]);
      CHECK_EQUAL(22, dst[1]);
      CHECK_EQUAL(33, dst[2]);
      CHECK_EQUAL(0, dst[3]);
      CHECK_EQUAL(0, dst[4]);
    }

    //*************************************************************************
    TEST(ranges_transform_binary_empty)
    {
      std::vector<int> src1{};
      std::vector<int> src2{};
      std::vector<int> dst{};

      auto result = etl::ranges::transform(src1.begin(), src1.end(), src2.begin(), src2.end(), dst.begin(), [](int a, int b) { return a + b; });

      CHECK(result.in1 == src1.end());
      CHECK(result.in2 == src2.end());
      CHECK(result.out == dst.begin());
    }

    //*************************************************************************
    TEST(ranges_transform_binary_with_projections)
    {
      struct Item
      {
        int value;
      };
      std::vector<Item> src1{{1}, {2}, {3}};
      std::vector<Item> src2{{10}, {20}, {30}};
      std::vector<int>  dst(3);

      auto result = etl::ranges::transform(
        src1.begin(), src1.end(), src2.begin(), src2.end(), dst.begin(), [](int a, int b) { return a + b; }, &Item::value, &Item::value);

      CHECK(result.in1 == src1.end());
      CHECK(result.in2 == src2.end());
      CHECK(result.out == dst.end());
      CHECK_EQUAL(11, dst[0]);
      CHECK_EQUAL(22, dst[1]);
      CHECK_EQUAL(33, dst[2]);
    }

    //*************************************************************************
    TEST(ranges_transform_unary_to_different_type)
    {
      std::vector<int>         src{1, 2, 3};
      std::vector<std::string> dst(3);

      auto result = etl::ranges::transform(src, dst.begin(), [](int x) { return std::to_string(x); });

      CHECK(result.in == src.end());
      CHECK(result.out == dst.end());
      CHECK(dst[0] == "1");
      CHECK(dst[1] == "2");
      CHECK(dst[2] == "3");
    }

    //*************************************************************************
    TEST(ranges_transform_unary_in_place)
    {
      std::vector<int> v{1, 2, 3, 4, 5};

      auto result = etl::ranges::transform(v, v.begin(), [](int x) { return x * x; });

      CHECK(result.in == v.end());
      CHECK(result.out == v.end());
      CHECK_EQUAL(1, v[0]);
      CHECK_EQUAL(4, v[1]);
      CHECK_EQUAL(9, v[2]);
      CHECK_EQUAL(16, v[3]);
      CHECK_EQUAL(25, v[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_iterator)
    {
      std::vector<int> vec{1, 2, 3, 2, 5, 2};
      auto             it = etl::ranges::replace(vec.begin(), vec.end(), 2, 9);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(9, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(9, vec[3]);
      CHECK_EQUAL(5, vec[4]);
      CHECK_EQUAL(9, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_replace_range)
    {
      std::vector<int> vec{1, 2, 3, 2, 5, 2};
      auto             it = etl::ranges::replace(vec, 2, 9);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(9, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(9, vec[3]);
      CHECK_EQUAL(5, vec[4]);
      CHECK_EQUAL(9, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_replace_no_match)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::replace(vec.begin(), vec.end(), 9, 0);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_empty)
    {
      std::vector<int> vec;
      auto             it = etl::ranges::replace(vec.begin(), vec.end(), 1, 2);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_replace_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::replace(vec.begin(), vec.end(), 6, 9, proj);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(9, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::replace(vec, 6, 9, proj);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(9, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_if_iterator)
    {
      auto is_even = [](int v)
      {
        return v % 2 == 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5, 6};
      auto             it = etl::ranges::replace_if(vec.begin(), vec.end(), is_even, 0);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(0, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(0, vec[3]);
      CHECK_EQUAL(5, vec[4]);
      CHECK_EQUAL(0, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_replace_if_range)
    {
      auto is_even = [](int v)
      {
        return v % 2 == 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5, 6};
      auto             it = etl::ranges::replace_if(vec, is_even, 0);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(0, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(0, vec[3]);
      CHECK_EQUAL(5, vec[4]);
      CHECK_EQUAL(0, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_replace_if_no_match)
    {
      auto is_negative = [](int v)
      {
        return v < 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::replace_if(vec.begin(), vec.end(), is_negative, 0);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_if_empty)
    {
      auto always_true = [](int)
      {
        return true;
      };
      std::vector<int> vec;
      auto             it = etl::ranges::replace_if(vec.begin(), vec.end(), always_true, 0);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_replace_if_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      auto is_greater_than_6 = [](int v)
      {
        return v > 6;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::replace_if(vec.begin(), vec.end(), is_greater_than_6, 0, proj);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(0, vec[3]);
      CHECK_EQUAL(0, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_if_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      auto is_greater_than_6 = [](int v)
      {
        return v > 6;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::replace_if(vec, is_greater_than_6, 0, proj);
      CHECK(it == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(0, vec[3]);
      CHECK_EQUAL(0, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_iterator)
    {
      std::vector<int> vec{1, 2, 3, 2, 5, 2};
      std::vector<int> out(6);
      auto [in_it, out_it] = etl::ranges::replace_copy(vec.begin(), vec.end(), out.begin(), 2, 9);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(9, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(9, out[3]);
      CHECK_EQUAL(5, out[4]);
      CHECK_EQUAL(9, out[5]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_range)
    {
      std::vector<int> vec{1, 2, 3, 2, 5, 2};
      std::vector<int> out(6);
      auto [in_it, out_it] = etl::ranges::replace_copy(vec, out.begin(), 2, 9);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(9, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(9, out[3]);
      CHECK_EQUAL(5, out[4]);
      CHECK_EQUAL(9, out[5]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_no_match)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5);
      auto [in_it, out_it] = etl::ranges::replace_copy(vec.begin(), vec.end(), out.begin(), 9, 0);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_empty)
    {
      std::vector<int> vec;
      std::vector<int> out;
      auto [in_it, out_it] = etl::ranges::replace_copy(vec.begin(), vec.end(), out.begin(), 1, 2);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
    }

    //*************************************************************************
    TEST(ranges_replace_copy_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5);
      auto [in_it, out_it] = etl::ranges::replace_copy(vec.begin(), vec.end(), out.begin(), 6, 9, proj);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(9, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5);
      auto [in_it, out_it] = etl::ranges::replace_copy(vec, out.begin(), 6, 9, proj);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(9, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_if_iterator)
    {
      auto is_even = [](int v)
      {
        return v % 2 == 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5, 6};
      std::vector<int> out(6);
      auto [in_it, out_it] = etl::ranges::replace_copy_if(vec.begin(), vec.end(), out.begin(), is_even, 0);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(0, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(0, out[3]);
      CHECK_EQUAL(5, out[4]);
      CHECK_EQUAL(0, out[5]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_if_range)
    {
      auto is_even = [](int v)
      {
        return v % 2 == 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5, 6};
      std::vector<int> out(6);
      auto [in_it, out_it] = etl::ranges::replace_copy_if(vec, out.begin(), is_even, 0);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(0, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(0, out[3]);
      CHECK_EQUAL(5, out[4]);
      CHECK_EQUAL(0, out[5]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_if_no_match)
    {
      auto is_negative = [](int v)
      {
        return v < 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5);
      auto [in_it, out_it] = etl::ranges::replace_copy_if(vec.begin(), vec.end(), out.begin(), is_negative, 0);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_if_empty)
    {
      auto always_true = [](int)
      {
        return true;
      };
      std::vector<int> vec;
      std::vector<int> out;
      auto [in_it, out_it] = etl::ranges::replace_copy_if(vec.begin(), vec.end(), out.begin(), always_true, 0);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
    }

    //*************************************************************************
    TEST(ranges_replace_copy_if_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      auto is_greater_than_6 = [](int v)
      {
        return v > 6;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5);
      auto [in_it, out_it] = etl::ranges::replace_copy_if(vec.begin(), vec.end(), out.begin(), is_greater_than_6, 0, proj);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(0, out[3]);
      CHECK_EQUAL(0, out[4]);
    }

    //*************************************************************************
    TEST(ranges_replace_copy_if_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      auto is_greater_than_6 = [](int v)
      {
        return v > 6;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5);
      auto [in_it, out_it] = etl::ranges::replace_copy_if(vec, out.begin(), is_greater_than_6, 0, proj);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(0, out[3]);
      CHECK_EQUAL(0, out[4]);
    }

    //*************************************************************************
    TEST(ranges_remove_iterator)
    {
      std::vector<int> vec{1, 2, 3, 2, 5, 2};
      auto             result = etl::ranges::remove(vec.begin(), vec.end(), 2);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(5, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_remove_range)
    {
      std::vector<int> vec{1, 2, 3, 2, 5, 2};
      auto             result = etl::ranges::remove(vec, 2);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(5, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_remove_no_match)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::remove(vec.begin(), vec.end(), 9);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_remove_empty)
    {
      std::vector<int> vec;
      auto             result = etl::ranges::remove(vec.begin(), vec.end(), 1);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_remove_all_same)
    {
      std::vector<int> vec{2, 2, 2, 2};
      auto             result = etl::ranges::remove(vec, 2);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_remove_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::remove(vec.begin(), vec.end(), 6, proj);
      CHECK(result.begin() == vec.begin() + 4);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(4, vec[2]);
      CHECK_EQUAL(5, vec[3]);
    }

    //*************************************************************************
    TEST(ranges_remove_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::remove(vec, 6, proj);
      CHECK(result.begin() == vec.begin() + 4);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(4, vec[2]);
      CHECK_EQUAL(5, vec[3]);
    }

    //*************************************************************************
    TEST(ranges_remove_if_iterator)
    {
      auto is_even = [](int v)
      {
        return v % 2 == 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5, 6};
      auto             result = etl::ranges::remove_if(vec.begin(), vec.end(), is_even);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(5, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_remove_if_range)
    {
      auto is_even = [](int v)
      {
        return v % 2 == 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5, 6};
      auto             result = etl::ranges::remove_if(vec, is_even);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(5, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_remove_if_no_match)
    {
      auto is_negative = [](int v)
      {
        return v < 0;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::remove_if(vec.begin(), vec.end(), is_negative);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_remove_if_empty)
    {
      auto is_even = [](int v)
      {
        return v % 2 == 0;
      };
      std::vector<int> vec;
      auto             result = etl::ranges::remove_if(vec.begin(), vec.end(), is_even);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_remove_if_all_match)
    {
      auto always_true = [](int)
      {
        return true;
      };
      std::vector<int> vec{1, 2, 3, 4};
      auto             result = etl::ranges::remove_if(vec, always_true);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_remove_if_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      auto is_greater_than_6 = [](int v)
      {
        return v > 6;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::remove_if(vec.begin(), vec.end(), is_greater_than_6, proj);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_remove_if_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      auto is_greater_than_6 = [](int v)
      {
        return v > 6;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::remove_if(vec, is_greater_than_6, proj);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_remove_copy_iterator)
    {
      std::vector<int> vec{1, 2, 3, 2, 5, 2};
      std::vector<int> out(6, 0);
      auto [in_it, out_it] = etl::ranges::remove_copy(vec.begin(), vec.end(), out.begin(), 2);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 3);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(3, out[1]);
      CHECK_EQUAL(5, out[2]);
    }

    //*************************************************************************
    TEST(ranges_remove_copy_range)
    {
      std::vector<int> vec{1, 2, 3, 2, 5, 2};
      std::vector<int> out(6, 0);
      auto [in_it, out_it] = etl::ranges::remove_copy(vec, out.begin(), 2);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 3);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(3, out[1]);
      CHECK_EQUAL(5, out[2]);
    }

    //*************************************************************************
    TEST(ranges_remove_copy_no_match)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::remove_copy(vec.begin(), vec.end(), out.begin(), 9);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 5);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_remove_copy_empty)
    {
      std::vector<int> vec;
      std::vector<int> out;
      auto [in_it, out_it] = etl::ranges::remove_copy(vec.begin(), vec.end(), out.begin(), 1);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin());
    }

    //*************************************************************************
    TEST(ranges_remove_copy_all_same)
    {
      std::vector<int> vec{2, 2, 2, 2};
      std::vector<int> out(4, 0);
      auto [in_it, out_it] = etl::ranges::remove_copy(vec, out.begin(), 2);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin());
    }

    //*************************************************************************
    TEST(ranges_remove_copy_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::remove_copy(vec.begin(), vec.end(), out.begin(), 6, proj);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 4);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(4, out[2]);
      CHECK_EQUAL(5, out[3]);
    }

    //*************************************************************************
    TEST(ranges_remove_copy_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v * 2;
      };
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::remove_copy(vec, out.begin(), 6, proj);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 4);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(4, out[2]);
      CHECK_EQUAL(5, out[3]);
    }

    //*************************************************************************
    TEST(ranges_fill_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::fill(vec.begin(), vec.end(), 7);
      CHECK(it == vec.end());
      CHECK_EQUAL(7, vec[0]);
      CHECK_EQUAL(7, vec[1]);
      CHECK_EQUAL(7, vec[2]);
      CHECK_EQUAL(7, vec[3]);
      CHECK_EQUAL(7, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_fill_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::fill(vec, 7);
      CHECK(it == vec.end());
      CHECK_EQUAL(7, vec[0]);
      CHECK_EQUAL(7, vec[1]);
      CHECK_EQUAL(7, vec[2]);
      CHECK_EQUAL(7, vec[3]);
      CHECK_EQUAL(7, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_fill_empty)
    {
      std::vector<int> vec;
      auto             it = etl::ranges::fill(vec.begin(), vec.end(), 7);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_fill_empty_range)
    {
      std::vector<int> vec;
      auto             it = etl::ranges::fill(vec, 7);
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_fill_partial)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::fill(vec.begin(), vec.begin() + 3, 9);
      CHECK(it == vec.begin() + 3);
      CHECK_EQUAL(9, vec[0]);
      CHECK_EQUAL(9, vec[1]);
      CHECK_EQUAL(9, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_fill_n_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::fill_n(vec.begin(), 3, 7);
      CHECK(it == vec.begin() + 3);
      CHECK_EQUAL(7, vec[0]);
      CHECK_EQUAL(7, vec[1]);
      CHECK_EQUAL(7, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_fill_n_zero)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::fill_n(vec.begin(), 0, 7);
      CHECK(it == vec.begin());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_fill_n_all)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             it = etl::ranges::fill_n(vec.begin(), 5, 0);
      CHECK(it == vec.end());
      CHECK_EQUAL(0, vec[0]);
      CHECK_EQUAL(0, vec[1]);
      CHECK_EQUAL(0, vec[2]);
      CHECK_EQUAL(0, vec[3]);
      CHECK_EQUAL(0, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_generate_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              counter = 10;
      auto             it      = etl::ranges::generate(vec.begin(), vec.end(), [&counter]() { return counter++; });
      CHECK(it == vec.end());
      CHECK_EQUAL(10, vec[0]);
      CHECK_EQUAL(11, vec[1]);
      CHECK_EQUAL(12, vec[2]);
      CHECK_EQUAL(13, vec[3]);
      CHECK_EQUAL(14, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_generate_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              counter = 10;
      auto             it      = etl::ranges::generate(vec, [&counter]() { return counter++; });
      CHECK(it == vec.end());
      CHECK_EQUAL(10, vec[0]);
      CHECK_EQUAL(11, vec[1]);
      CHECK_EQUAL(12, vec[2]);
      CHECK_EQUAL(13, vec[3]);
      CHECK_EQUAL(14, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_generate_empty)
    {
      std::vector<int> vec;
      int              counter = 10;
      auto             it      = etl::ranges::generate(vec.begin(), vec.end(), [&counter]() { return counter++; });
      CHECK(it == vec.end());
      CHECK_EQUAL(10, counter);
    }

    //*************************************************************************
    TEST(ranges_generate_empty_range)
    {
      std::vector<int> vec;
      int              counter = 10;
      auto             it      = etl::ranges::generate(vec, [&counter]() { return counter++; });
      CHECK(it == vec.end());
      CHECK_EQUAL(10, counter);
    }

    //*************************************************************************
    TEST(ranges_generate_partial)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              counter = 10;
      auto             it      = etl::ranges::generate(vec.begin(), vec.begin() + 3, [&counter]() { return counter++; });
      CHECK(it == vec.begin() + 3);
      CHECK_EQUAL(10, vec[0]);
      CHECK_EQUAL(11, vec[1]);
      CHECK_EQUAL(12, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_generate_n_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              counter = 10;
      auto             it      = etl::ranges::generate_n(vec.begin(), 3, [&counter]() { return counter++; });
      CHECK(it == vec.begin() + 3);
      CHECK_EQUAL(10, vec[0]);
      CHECK_EQUAL(11, vec[1]);
      CHECK_EQUAL(12, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_generate_n_zero)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              counter = 10;
      auto             it      = etl::ranges::generate_n(vec.begin(), 0, [&counter]() { return counter++; });
      CHECK(it == vec.begin());
      CHECK_EQUAL(10, counter);
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_generate_n_all)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      int              counter = 0;
      auto             it      = etl::ranges::generate_n(vec.begin(), 5, [&counter]() { return counter++; });
      CHECK(it == vec.end());
      CHECK_EQUAL(0, vec[0]);
      CHECK_EQUAL(1, vec[1]);
      CHECK_EQUAL(2, vec[2]);
      CHECK_EQUAL(3, vec[3]);
      CHECK_EQUAL(4, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_iota_iterator)
    {
      std::vector<int> vec(5, 0);
      auto             result = etl::ranges::iota(vec.begin(), vec.end(), 1);
      CHECK(result.out == vec.end());
      CHECK_EQUAL(6, result.value);
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_iota_range)
    {
      std::vector<int> vec(5, 0);
      auto             result = etl::ranges::iota(vec, 1);
      CHECK(result.out == vec.end());
      CHECK_EQUAL(6, result.value);
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_iota_empty)
    {
      std::vector<int> vec;
      auto             result = etl::ranges::iota(vec.begin(), vec.end(), 10);
      CHECK(result.out == vec.end());
      CHECK_EQUAL(10, result.value);
    }

    //*************************************************************************
    TEST(ranges_iota_empty_range)
    {
      std::vector<int> vec;
      auto             result = etl::ranges::iota(vec, 10);
      CHECK(result.out == vec.end());
      CHECK_EQUAL(10, result.value);
    }

    //*************************************************************************
    TEST(ranges_iota_partial)
    {
      std::vector<int> vec{0, 0, 0, 0, 0};
      auto             result = etl::ranges::iota(vec.begin(), vec.begin() + 3, 5);
      CHECK(result.out == vec.begin() + 3);
      CHECK_EQUAL(8, result.value);
      CHECK_EQUAL(5, vec[0]);
      CHECK_EQUAL(6, vec[1]);
      CHECK_EQUAL(7, vec[2]);
      CHECK_EQUAL(0, vec[3]);
      CHECK_EQUAL(0, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_iota_negative_start)
    {
      std::vector<int> vec(5, 0);
      auto             result = etl::ranges::iota(vec, -2);
      CHECK(result.out == vec.end());
      CHECK_EQUAL(3, result.value);
      CHECK_EQUAL(-2, vec[0]);
      CHECK_EQUAL(-1, vec[1]);
      CHECK_EQUAL(0, vec[2]);
      CHECK_EQUAL(1, vec[3]);
      CHECK_EQUAL(2, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_iota_single_element)
    {
      std::vector<int> vec(1, 0);
      auto             result = etl::ranges::iota(vec, 42);
      CHECK(result.out == vec.end());
      CHECK_EQUAL(43, result.value);
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_iota_array)
    {
      int  arr[5] = {};
      auto result = etl::ranges::iota(arr, 10);
      CHECK_EQUAL(15, result.value);
      CHECK_EQUAL(10, arr[0]);
      CHECK_EQUAL(11, arr[1]);
      CHECK_EQUAL(12, arr[2]);
      CHECK_EQUAL(13, arr[3]);
      CHECK_EQUAL(14, arr[4]);
    }

    //*************************************************************************
    TEST(ranges_unique_iterator)
    {
      std::vector<int> vec{1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
      auto             result = etl::ranges::unique(vec.begin(), vec.end());
      CHECK(result.begin() == vec.begin() + 5);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_unique_range)
    {
      std::vector<int> vec{1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
      auto             result = etl::ranges::unique(vec);
      CHECK(result.begin() == vec.begin() + 5);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_unique_no_duplicates)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::unique(vec.begin(), vec.end());
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_unique_all_same)
    {
      std::vector<int> vec{2, 2, 2, 2};
      auto             result = etl::ranges::unique(vec);
      CHECK(result.begin() == vec.begin() + 1);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(2, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_unique_empty)
    {
      std::vector<int> vec;
      auto             result = etl::ranges::unique(vec.begin(), vec.end());
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_unique_single_element)
    {
      std::vector<int> vec{42};
      auto             result = etl::ranges::unique(vec);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_unique_with_predicate)
    {
      auto pred = [](int a, int b)
      {
        return (a / 10) == (b / 10);
      };
      std::vector<int> vec{11, 15, 21, 25, 29, 31};
      auto             result = etl::ranges::unique(vec.begin(), vec.end(), pred);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(11, vec[0]);
      CHECK_EQUAL(21, vec[1]);
      CHECK_EQUAL(31, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_with_predicate_range)
    {
      auto pred = [](int a, int b)
      {
        return (a / 10) == (b / 10);
      };
      std::vector<int> vec{11, 15, 21, 25, 29, 31};
      auto             result = etl::ranges::unique(vec, pred);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(11, vec[0]);
      CHECK_EQUAL(21, vec[1]);
      CHECK_EQUAL(31, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v / 10;
      };
      std::vector<int> vec{11, 15, 21, 25, 29, 31};
      auto             result = etl::ranges::unique(vec.begin(), vec.end(), etl::ranges::equal_to{}, proj);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(11, vec[0]);
      CHECK_EQUAL(21, vec[1]);
      CHECK_EQUAL(31, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v / 10;
      };
      std::vector<int> vec{11, 15, 21, 25, 29, 31};
      auto             result = etl::ranges::unique(vec, etl::ranges::equal_to{}, proj);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(11, vec[0]);
      CHECK_EQUAL(21, vec[1]);
      CHECK_EQUAL(31, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_consecutive_pairs)
    {
      std::vector<int> vec{1, 1, 2, 2, 1, 1};
      auto             result = etl::ranges::unique(vec);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(1, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_iterator)
    {
      std::vector<int> vec{1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
      std::vector<int> out(10, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec.begin(), vec.end(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 5);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_range)
    {
      std::vector<int> vec{1, 1, 2, 2, 2, 3, 3, 4, 5, 5};
      std::vector<int> out(10, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 5);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_no_duplicates)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec.begin(), vec.end(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 5);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_empty)
    {
      std::vector<int> vec;
      std::vector<int> out;
      auto [in_it, out_it] = etl::ranges::unique_copy(vec.begin(), vec.end(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin());
    }

    //*************************************************************************
    TEST(ranges_unique_copy_all_same)
    {
      std::vector<int> vec{2, 2, 2, 2};
      std::vector<int> out(4, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 1);
      CHECK_EQUAL(2, out[0]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_single_element)
    {
      std::vector<int> vec{42};
      std::vector<int> out(1, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 1);
      CHECK_EQUAL(42, out[0]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_with_predicate)
    {
      auto pred = [](int a, int b)
      {
        return (a / 10) == (b / 10);
      };
      std::vector<int> vec{11, 15, 21, 25, 29, 31};
      std::vector<int> out(6, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec.begin(), vec.end(), out.begin(), pred);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 3);
      CHECK_EQUAL(11, out[0]);
      CHECK_EQUAL(21, out[1]);
      CHECK_EQUAL(31, out[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_with_predicate_range)
    {
      auto pred = [](int a, int b)
      {
        return (a / 10) == (b / 10);
      };
      std::vector<int> vec{11, 15, 21, 25, 29, 31};
      std::vector<int> out(6, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec, out.begin(), pred);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 3);
      CHECK_EQUAL(11, out[0]);
      CHECK_EQUAL(21, out[1]);
      CHECK_EQUAL(31, out[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_with_projection)
    {
      auto proj = [](const int& v)
      {
        return v / 10;
      };
      std::vector<int> vec{11, 15, 21, 25, 29, 31};
      std::vector<int> out(6, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec.begin(), vec.end(), out.begin(), etl::ranges::equal_to{}, proj);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 3);
      CHECK_EQUAL(11, out[0]);
      CHECK_EQUAL(21, out[1]);
      CHECK_EQUAL(31, out[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_with_projection_range)
    {
      auto proj = [](const int& v)
      {
        return v / 10;
      };
      std::vector<int> vec{11, 15, 21, 25, 29, 31};
      std::vector<int> out(6, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec, out.begin(), etl::ranges::equal_to{}, proj);
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 3);
      CHECK_EQUAL(11, out[0]);
      CHECK_EQUAL(21, out[1]);
      CHECK_EQUAL(31, out[2]);
    }

    //*************************************************************************
    TEST(ranges_unique_copy_consecutive_pairs)
    {
      std::vector<int> vec{1, 1, 2, 2, 1, 1};
      std::vector<int> out(6, 0);
      auto [in_it, out_it] = etl::ranges::unique_copy(vec, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin() + 3);
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(1, out[2]);
    }

    //*************************************************************************
    TEST(ranges_reverse_iterator_sentinel)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::reverse(vec.begin(), vec.end());
      CHECK(result == vec.end());
      CHECK_EQUAL(5, vec[0]);
      CHECK_EQUAL(4, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(2, vec[3]);
      CHECK_EQUAL(1, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_reverse_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::reverse(vec);
      CHECK(result == vec.end());
      CHECK_EQUAL(5, vec[0]);
      CHECK_EQUAL(4, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(2, vec[3]);
      CHECK_EQUAL(1, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_reverse_even_count)
    {
      std::vector<int> vec{1, 2, 3, 4};
      etl::ranges::reverse(vec);
      CHECK_EQUAL(4, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(2, vec[2]);
      CHECK_EQUAL(1, vec[3]);
    }

    //*************************************************************************
    TEST(ranges_reverse_single_element)
    {
      std::vector<int> vec{42};
      etl::ranges::reverse(vec);
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_reverse_empty)
    {
      std::vector<int> vec{};
      auto             result = etl::ranges::reverse(vec);
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_reverse_copy_iterator_sentinel)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::reverse_copy(vec.begin(), vec.end(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(5, out[0]);
      CHECK_EQUAL(4, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(2, out[3]);
      CHECK_EQUAL(1, out[4]);
    }

    //*************************************************************************
    TEST(ranges_reverse_copy_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::reverse_copy(vec, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(5, out[0]);
      CHECK_EQUAL(4, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(2, out[3]);
      CHECK_EQUAL(1, out[4]);
    }

    //*************************************************************************
    TEST(ranges_reverse_copy_empty)
    {
      std::vector<int> vec{};
      std::vector<int> out{};
      auto [in_it, out_it] = etl::ranges::reverse_copy(vec, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin());
    }

    //*************************************************************************
    TEST(ranges_reverse_copy_single_element)
    {
      std::vector<int> vec{42};
      std::vector<int> out(1, 0);
      auto [in_it, out_it] = etl::ranges::reverse_copy(vec, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(42, out[0]);
    }

    //*************************************************************************
    TEST(ranges_rotate_iterator_sentinel)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::rotate(vec.begin(), vec.begin() + 2, vec.end());
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(3, vec[0]);
      CHECK_EQUAL(4, vec[1]);
      CHECK_EQUAL(5, vec[2]);
      CHECK_EQUAL(1, vec[3]);
      CHECK_EQUAL(2, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::rotate(vec, vec.begin() + 2);
      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(3, vec[0]);
      CHECK_EQUAL(4, vec[1]);
      CHECK_EQUAL(5, vec[2]);
      CHECK_EQUAL(1, vec[3]);
      CHECK_EQUAL(2, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_middle_at_begin)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::rotate(vec, vec.begin());
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_middle_at_end)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::rotate(vec, vec.end());
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_single_element)
    {
      std::vector<int> vec{42};
      auto             result = etl::ranges::rotate(vec, vec.begin());
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_rotate_by_one)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::rotate(vec, vec.begin() + 1);
      CHECK(result.begin() == vec.begin() + 4);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(2, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(4, vec[2]);
      CHECK_EQUAL(5, vec[3]);
      CHECK_EQUAL(1, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_by_last_minus_one)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::rotate(vec, vec.begin() + 4);
      CHECK(result.begin() == vec.begin() + 1);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(5, vec[0]);
      CHECK_EQUAL(1, vec[1]);
      CHECK_EQUAL(2, vec[2]);
      CHECK_EQUAL(3, vec[3]);
      CHECK_EQUAL(4, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_copy_iterator_sentinel)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::rotate_copy(vec.begin(), vec.begin() + 2, vec.end(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(3, out[0]);
      CHECK_EQUAL(4, out[1]);
      CHECK_EQUAL(5, out[2]);
      CHECK_EQUAL(1, out[3]);
      CHECK_EQUAL(2, out[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_copy_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::rotate_copy(vec, vec.begin() + 2, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(3, out[0]);
      CHECK_EQUAL(4, out[1]);
      CHECK_EQUAL(5, out[2]);
      CHECK_EQUAL(1, out[3]);
      CHECK_EQUAL(2, out[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_copy_middle_at_begin)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::rotate_copy(vec, vec.begin(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_copy_middle_at_end)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::rotate_copy(vec, vec.end(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(1, out[0]);
      CHECK_EQUAL(2, out[1]);
      CHECK_EQUAL(3, out[2]);
      CHECK_EQUAL(4, out[3]);
      CHECK_EQUAL(5, out[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_copy_single_element)
    {
      std::vector<int> vec{42};
      std::vector<int> out(1, 0);
      auto [in_it, out_it] = etl::ranges::rotate_copy(vec, vec.begin(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(42, out[0]);
    }

    //*************************************************************************
    TEST(ranges_rotate_copy_empty)
    {
      std::vector<int> vec{};
      std::vector<int> out{};
      auto [in_it, out_it] = etl::ranges::rotate_copy(vec, vec.begin(), out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.begin());
    }

    //*************************************************************************
    TEST(ranges_rotate_copy_by_one)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::rotate_copy(vec, vec.begin() + 1, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(2, out[0]);
      CHECK_EQUAL(3, out[1]);
      CHECK_EQUAL(4, out[2]);
      CHECK_EQUAL(5, out[3]);
      CHECK_EQUAL(1, out[4]);
    }

    //*************************************************************************
    TEST(ranges_rotate_copy_by_last_minus_one)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> out(5, 0);
      auto [in_it, out_it] = etl::ranges::rotate_copy(vec, vec.begin() + 4, out.begin());
      CHECK(in_it == vec.end());
      CHECK(out_it == out.end());
      CHECK_EQUAL(5, out[0]);
      CHECK_EQUAL(1, out[1]);
      CHECK_EQUAL(2, out[2]);
      CHECK_EQUAL(3, out[3]);
      CHECK_EQUAL(4, out[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_left_iterator_sentinel)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_left(vec.begin(), vec.end(), 2);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin() + 3);
      CHECK_EQUAL(3, vec[0]);
      CHECK_EQUAL(4, vec[1]);
      CHECK_EQUAL(5, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_shift_left_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_left(vec, 2);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin() + 3);
      CHECK_EQUAL(3, vec[0]);
      CHECK_EQUAL(4, vec[1]);
      CHECK_EQUAL(5, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_shift_left_by_zero)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_left(vec, 0);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_left_by_negative)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_left(vec, -1);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_left_by_size)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_left(vec, 5);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_shift_left_by_more_than_size)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_left(vec, 10);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_shift_left_by_one)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_left(vec, 1);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin() + 4);
      CHECK_EQUAL(2, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(4, vec[2]);
      CHECK_EQUAL(5, vec[3]);
    }

    //*************************************************************************
    TEST(ranges_shift_left_by_last_minus_one)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_left(vec, 4);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin() + 1);
      CHECK_EQUAL(5, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_shift_left_empty)
    {
      std::vector<int> vec{};
      auto             result = etl::ranges::shift_left(vec, 1);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_shift_left_single_element)
    {
      std::vector<int> vec{42};
      auto             result = etl::ranges::shift_left(vec, 1);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_shift_left_single_element_by_zero)
    {
      std::vector<int> vec{42};
      auto             result = etl::ranges::shift_left(vec, 0);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_shift_right_iterator_sentinel)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_right(vec.begin(), vec.end(), 2);
      CHECK(result.begin() == vec.begin() + 2);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[2]);
      CHECK_EQUAL(2, vec[3]);
      CHECK_EQUAL(3, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_right_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_right(vec, 2);
      CHECK(result.begin() == vec.begin() + 2);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[2]);
      CHECK_EQUAL(2, vec[3]);
      CHECK_EQUAL(3, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_right_by_zero)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_right(vec, 0);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_right_by_negative)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_right(vec, -1);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(4, vec[3]);
      CHECK_EQUAL(5, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_right_by_size)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_right(vec, 5);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_shift_right_by_more_than_size)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_right(vec, 10);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_shift_right_by_one)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_right(vec, 1);
      CHECK(result.begin() == vec.begin() + 1);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[1]);
      CHECK_EQUAL(2, vec[2]);
      CHECK_EQUAL(3, vec[3]);
      CHECK_EQUAL(4, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_right_by_last_minus_one)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             result = etl::ranges::shift_right(vec, 4);
      CHECK(result.begin() == vec.begin() + 4);
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(1, vec[4]);
    }

    //*************************************************************************
    TEST(ranges_shift_right_empty)
    {
      std::vector<int> vec{};
      auto             result = etl::ranges::shift_right(vec, 1);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_shift_right_single_element)
    {
      std::vector<int> vec{42};
      auto             result = etl::ranges::shift_right(vec, 1);
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_shift_right_single_element_by_zero)
    {
      std::vector<int> vec{42};
      auto             result = etl::ranges::shift_right(vec, 0);
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_shuffle_iterator_sentinel)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> original = vec;
      std::mt19937     gen(42);
      auto             result = etl::ranges::shuffle(vec.begin(), vec.end(), gen);

      CHECK(result == vec.end());
      // All original elements must still be present (permutation check)
      std::vector<int> sorted_vec = vec;
      std::sort(sorted_vec.begin(), sorted_vec.end());
      CHECK(sorted_vec == original);
    }

    //*************************************************************************
    TEST(ranges_shuffle_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> original = vec;
      std::mt19937     gen(42);
      auto             result = etl::ranges::shuffle(vec, gen);

      CHECK(result == vec.end());
      // All original elements must still be present (permutation check)
      std::vector<int> sorted_vec = vec;
      std::sort(sorted_vec.begin(), sorted_vec.end());
      CHECK(sorted_vec == original);
    }

    //*************************************************************************
    TEST(ranges_shuffle_empty)
    {
      std::vector<int> vec{};
      std::mt19937     gen(42);
      auto             result = etl::ranges::shuffle(vec.begin(), vec.end(), gen);
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_shuffle_single_element)
    {
      std::vector<int> vec{42};
      std::mt19937     gen(42);
      auto             result = etl::ranges::shuffle(vec, gen);
      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_shuffle_two_elements)
    {
      std::vector<int> vec{1, 2};
      std::vector<int> original = vec;
      std::mt19937     gen(42);
      auto             result = etl::ranges::shuffle(vec, gen);

      CHECK(result == vec.end());
      std::vector<int> sorted_vec = vec;
      std::sort(sorted_vec.begin(), sorted_vec.end());
      CHECK(sorted_vec == original);
    }

    //*************************************************************************
    TEST(ranges_shuffle_deterministic)
    {
      // Same seed should produce the same permutation
      std::vector<int> vec1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> vec2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::mt19937     gen1(123);
      std::mt19937     gen2(123);

      etl::ranges::shuffle(vec1, gen1);
      etl::ranges::shuffle(vec2, gen2);

      CHECK(vec1 == vec2);
    }

    //*************************************************************************
    TEST(ranges_shuffle_large)
    {
      std::vector<int> vec(100);
      std::iota(vec.begin(), vec.end(), 0);
      std::vector<int> original = vec;
      std::mt19937     gen(99);

      etl::ranges::shuffle(vec, gen);

      // All original elements must still be present
      std::vector<int> sorted_vec = vec;
      std::sort(sorted_vec.begin(), sorted_vec.end());
      CHECK(sorted_vec == original);

      // It's extremely unlikely a shuffle of 100 elements leaves them in order
      CHECK(vec != original);
    }

    //*************************************************************************
    TEST(ranges_sample_iterator_sentinel)
    {
      std::vector<int> src{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> dest(5);
      std::mt19937     gen(42);

      auto result = etl::ranges::sample(src.begin(), src.end(), dest.begin(), 5, gen);

      // result should point past the last written element
      CHECK(result == dest.begin() + 5);

      // All sampled elements must come from the source
      for (auto& v : dest)
      {
        CHECK(std::find(src.begin(), src.end(), v) != src.end());
      }

      // No duplicates in the sample
      std::sort(dest.begin(), dest.end());
      CHECK(std::unique(dest.begin(), dest.end()) == dest.end());
    }

    //*************************************************************************
    TEST(ranges_sample_range)
    {
      std::vector<int> src{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> dest(5);
      std::mt19937     gen(42);

      auto result = etl::ranges::sample(src, dest.begin(), 5, gen);

      CHECK(result == dest.begin() + 5);

      for (auto& v : dest)
      {
        CHECK(std::find(src.begin(), src.end(), v) != src.end());
      }

      std::sort(dest.begin(), dest.end());
      CHECK(std::unique(dest.begin(), dest.end()) == dest.end());
    }

    //*************************************************************************
    TEST(ranges_sample_n_greater_than_population)
    {
      std::vector<int> src{1, 2, 3};
      std::vector<int> dest(5, 0);
      std::mt19937     gen(42);

      auto result = etl::ranges::sample(src.begin(), src.end(), dest.begin(), 5, gen);

      // Only 3 elements available, so only 3 should be copied
      CHECK(result == dest.begin() + 3);
      CHECK_EQUAL(1, dest[0]);
      CHECK_EQUAL(2, dest[1]);
      CHECK_EQUAL(3, dest[2]);
    }

    //*************************************************************************
    TEST(ranges_sample_n_equal_to_population)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dest(5, 0);
      std::mt19937     gen(42);

      auto result = etl::ranges::sample(src, dest.begin(), 5, gen);

      CHECK(result == dest.begin() + 5);

      // All elements should be copied
      std::vector<int> sorted_dest = dest;
      std::sort(sorted_dest.begin(), sorted_dest.end());
      CHECK(sorted_dest == src);
    }

    //*************************************************************************
    TEST(ranges_sample_empty_source)
    {
      std::vector<int> src{};
      std::vector<int> dest(5, 0);
      std::mt19937     gen(42);

      auto result = etl::ranges::sample(src.begin(), src.end(), dest.begin(), 5, gen);

      CHECK(result == dest.begin());
    }

    //*************************************************************************
    TEST(ranges_sample_zero_count)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> dest(5, 0);
      std::mt19937     gen(42);

      auto result = etl::ranges::sample(src, dest.begin(), 0, gen);

      CHECK(result == dest.begin());
    }

    //*************************************************************************
    TEST(ranges_sample_single_element_source)
    {
      std::vector<int> src{42};
      std::vector<int> dest(1, 0);
      std::mt19937     gen(42);

      auto result = etl::ranges::sample(src, dest.begin(), 1, gen);

      CHECK(result == dest.begin() + 1);
      CHECK_EQUAL(42, dest[0]);
    }

    //*************************************************************************
    TEST(ranges_sample_deterministic)
    {
      // Same seed should produce the same sample
      std::vector<int> src{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> dest1(5);
      std::vector<int> dest2(5);
      std::mt19937     gen1(123);
      std::mt19937     gen2(123);

      etl::ranges::sample(src, dest1.begin(), 5, gen1);
      etl::ranges::sample(src, dest2.begin(), 5, gen2);

      CHECK(dest1 == dest2);
    }

    //*************************************************************************
    TEST(ranges_sample_preserves_relative_order)
    {
      // Selection sampling preserves relative order of elements
      std::vector<int> src{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> dest(5);
      std::mt19937     gen(42);

      etl::ranges::sample(src, dest.begin(), 5, gen);

      // The sampled elements should be in ascending order (since source is)
      for (size_t i = 1; i < dest.size(); ++i)
      {
        CHECK(dest[i - 1] < dest[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_sample_large)
    {
      std::vector<int> src(100);
      std::iota(src.begin(), src.end(), 0);
      std::vector<int> dest(20);
      std::mt19937     gen(99);

      auto result = etl::ranges::sample(src, dest.begin(), 20, gen);

      CHECK(result == dest.begin() + 20);

      // All sampled elements must be from the source
      for (auto& v : dest)
      {
        CHECK(v >= 0);
        CHECK(v < 100);
      }

      // No duplicates
      std::sort(dest.begin(), dest.end());
      CHECK(std::unique(dest.begin(), dest.end()) == dest.end());
    }

    //*************************************************************************
    TEST(ranges_partition_iterator_sentinel)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition(vec.begin(), vec.end(), pred);

      // All elements before the partition point should satisfy the predicate
      for (auto it = vec.begin(); it != result.begin(); ++it)
      {
        CHECK(*it < 10);
      }

      // All elements from the partition point onward should not satisfy the
      // predicate
      for (auto it = result.begin(); it != result.end(); ++it)
      {
        CHECK(*it >= 10);
      }
    }

    //*************************************************************************
    TEST(ranges_partition_range)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition(vec, pred);

      for (auto it = vec.begin(); it != result.begin(); ++it)
      {
        CHECK(*it < 10);
      }

      for (auto it = result.begin(); it != result.end(); ++it)
      {
        CHECK(*it >= 10);
      }
    }

    //*************************************************************************
    TEST(ranges_partition_already_partitioned)
    {
      std::vector<int> vec{1, 2, 3, 10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());

      // Verify the range is correctly partitioned
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
    }

    //*************************************************************************
    TEST(ranges_partition_all_true)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition(vec.begin(), vec.end(), pred);

      // Partition point should be at the end (all elements satisfy predicate)
      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_partition_all_false)
    {
      std::vector<int> vec{10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition(vec.begin(), vec.end(), pred);

      // Partition point should be at the beginning (no elements satisfy
      // predicate)
      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_partition_empty)
    {
      std::vector<int> vec{};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_partition_single_true)
    {
      std::vector<int> vec{1};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_partition_single_false)
    {
      std::vector<int> vec{20};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_partition_with_projection)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      // With projection: values become 10,200,30,100,20,300
      // pred(proj(v)) < 100: true for 1,3,2 (proj gives 10,30,20), false for
      // 20,10,30 (proj gives 200,100,300)
      auto result = etl::ranges::partition(vec.begin(), vec.end(), pred, proj);

      for (auto it = vec.begin(); it != result.begin(); ++it)
      {
        CHECK((*it * 10) < 100);
      }

      for (auto it = result.begin(); it != result.end(); ++it)
      {
        CHECK((*it * 10) >= 100);
      }
    }

    //*************************************************************************
    TEST(ranges_partition_with_projection_range)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      auto result = etl::ranges::partition(vec, pred, proj);

      for (auto it = vec.begin(); it != result.begin(); ++it)
      {
        CHECK((*it * 10) < 100);
      }

      for (auto it = result.begin(); it != result.end(); ++it)
      {
        CHECK((*it * 10) >= 100);
      }
    }

    //*************************************************************************
    TEST(ranges_partition_preserves_elements)
    {
      std::vector<int> vec{5, 1, 4, 2, 3};
      std::vector<int> sorted_original{1, 2, 3, 4, 5};
      auto             pred = [](const int& v)
      {
        return v <= 3;
      };

      etl::ranges::partition(vec, pred);

      // All original elements should still be present
      std::vector<int> sorted_result(vec.begin(), vec.end());
      std::sort(sorted_result.begin(), sorted_result.end());
      CHECK(sorted_result == sorted_original);
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_iterator_sentinel)
    {
      std::vector<int> vec{1, 2, 3, 10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      CHECK(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_range)
    {
      std::vector<int> vec{1, 2, 3, 10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      CHECK(etl::ranges::is_partitioned(vec, pred));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_not_partitioned)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      CHECK_FALSE(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred));
      CHECK_FALSE(etl::ranges::is_partitioned(vec, pred));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_all_true)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      CHECK(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred));
      CHECK(etl::ranges::is_partitioned(vec, pred));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_all_false)
    {
      std::vector<int> vec{10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      CHECK(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred));
      CHECK(etl::ranges::is_partitioned(vec, pred));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_empty)
    {
      std::vector<int> vec{};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      CHECK(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred));
      CHECK(etl::ranges::is_partitioned(vec, pred));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_single_true)
    {
      std::vector<int> vec{1};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      CHECK(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred));
      CHECK(etl::ranges::is_partitioned(vec, pred));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_single_false)
    {
      std::vector<int> vec{20};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      CHECK(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred));
      CHECK(etl::ranges::is_partitioned(vec, pred));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_with_projection)
    {
      std::vector<int> vec{1, 2, 3, 10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      // With projection: values become 10,20,30,100,200,300
      // pred(proj(v)) < 100: true for 10,20,30, false for 100,200,300
      CHECK(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred, proj));
      CHECK(etl::ranges::is_partitioned(vec, pred, proj));
    }

    //*************************************************************************
    TEST(ranges_is_partitioned_with_projection_not_partitioned)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      // With projection: values become 10,200,30,100,20,300
      // pred(proj(v)) < 100: true,false,true,false,true,false => not
      // partitioned
      CHECK_FALSE(etl::ranges::is_partitioned(vec.begin(), vec.end(), pred, proj));
      CHECK_FALSE(etl::ranges::is_partitioned(vec, pred, proj));
    }

    //*************************************************************************
    TEST(ranges_partition_copy_iterator_sentinel)
    {
      std::vector<int> src{1, 20, 3, 10, 2, 30};
      std::vector<int> out_true(6, 0);
      std::vector<int> out_false(6, 0);
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_copy(src.begin(), src.end(), out_true.begin(), out_false.begin(), pred);

      CHECK(result.in == src.end());

      std::vector<int> expected_true{1, 3, 2};
      std::vector<int> expected_false{20, 10, 30};

      CHECK_EQUAL(3, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(3, std::distance(out_false.begin(), result.out2));

      CHECK(std::equal(expected_true.begin(), expected_true.end(), out_true.begin()));
      CHECK(std::equal(expected_false.begin(), expected_false.end(), out_false.begin()));
    }

    //*************************************************************************
    TEST(ranges_partition_copy_range)
    {
      std::vector<int> src{1, 20, 3, 10, 2, 30};
      std::vector<int> out_true(6, 0);
      std::vector<int> out_false(6, 0);
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_copy(src, out_true.begin(), out_false.begin(), pred);

      CHECK(result.in == src.end());

      std::vector<int> expected_true{1, 3, 2};
      std::vector<int> expected_false{20, 10, 30};

      CHECK_EQUAL(3, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(3, std::distance(out_false.begin(), result.out2));

      CHECK(std::equal(expected_true.begin(), expected_true.end(), out_true.begin()));
      CHECK(std::equal(expected_false.begin(), expected_false.end(), out_false.begin()));
    }

    //*************************************************************************
    TEST(ranges_partition_copy_all_true)
    {
      std::vector<int> src{1, 2, 3, 4, 5};
      std::vector<int> out_true(5, 0);
      std::vector<int> out_false(5, 0);
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_copy(src.begin(), src.end(), out_true.begin(), out_false.begin(), pred);

      CHECK(result.in == src.end());
      CHECK_EQUAL(5, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(0, std::distance(out_false.begin(), result.out2));

      CHECK(std::equal(src.begin(), src.end(), out_true.begin()));
    }

    //*************************************************************************
    TEST(ranges_partition_copy_all_false)
    {
      std::vector<int> src{10, 20, 30};
      std::vector<int> out_true(3, 0);
      std::vector<int> out_false(3, 0);
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_copy(src.begin(), src.end(), out_true.begin(), out_false.begin(), pred);

      CHECK(result.in == src.end());
      CHECK_EQUAL(0, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(3, std::distance(out_false.begin(), result.out2));

      CHECK(std::equal(src.begin(), src.end(), out_false.begin()));
    }

    //*************************************************************************
    TEST(ranges_partition_copy_empty)
    {
      std::vector<int> src{};
      std::vector<int> out_true{};
      std::vector<int> out_false{};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_copy(src.begin(), src.end(), out_true.begin(), out_false.begin(), pred);

      CHECK(result.in == src.end());
      CHECK(result.out1 == out_true.begin());
      CHECK(result.out2 == out_false.begin());
    }

    //*************************************************************************
    TEST(ranges_partition_copy_single_true)
    {
      std::vector<int> src{5};
      std::vector<int> out_true(1, 0);
      std::vector<int> out_false(1, 0);
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_copy(src.begin(), src.end(), out_true.begin(), out_false.begin(), pred);

      CHECK(result.in == src.end());
      CHECK_EQUAL(1, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(0, std::distance(out_false.begin(), result.out2));
      CHECK_EQUAL(5, out_true[0]);
    }

    //*************************************************************************
    TEST(ranges_partition_copy_single_false)
    {
      std::vector<int> src{20};
      std::vector<int> out_true(1, 0);
      std::vector<int> out_false(1, 0);
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_copy(src.begin(), src.end(), out_true.begin(), out_false.begin(), pred);

      CHECK(result.in == src.end());
      CHECK_EQUAL(0, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(1, std::distance(out_false.begin(), result.out2));
      CHECK_EQUAL(20, out_false[0]);
    }

    //*************************************************************************
    TEST(ranges_partition_copy_with_projection)
    {
      std::vector<int> src{1, 20, 3, 10, 2, 30};
      std::vector<int> out_true(6, 0);
      std::vector<int> out_false(6, 0);
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      // With projection: values become 10,200,30,100,20,300
      // pred(proj(v)) < 100: true for 1,3,2 (proj gives 10,30,20), false for
      // 20,10,30 (proj gives 200,100,300)
      auto result = etl::ranges::partition_copy(src.begin(), src.end(), out_true.begin(), out_false.begin(), pred, proj);

      CHECK(result.in == src.end());

      std::vector<int> expected_true{1, 3, 2};
      std::vector<int> expected_false{20, 10, 30};

      CHECK_EQUAL(3, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(3, std::distance(out_false.begin(), result.out2));

      CHECK(std::equal(expected_true.begin(), expected_true.end(), out_true.begin()));
      CHECK(std::equal(expected_false.begin(), expected_false.end(), out_false.begin()));
    }

    //*************************************************************************
    TEST(ranges_partition_copy_with_projection_range)
    {
      std::vector<int> src{1, 20, 3, 10, 2, 30};
      std::vector<int> out_true(6, 0);
      std::vector<int> out_false(6, 0);
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      auto result = etl::ranges::partition_copy(src, out_true.begin(), out_false.begin(), pred, proj);

      CHECK(result.in == src.end());

      std::vector<int> expected_true{1, 3, 2};
      std::vector<int> expected_false{20, 10, 30};

      CHECK_EQUAL(3, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(3, std::distance(out_false.begin(), result.out2));

      CHECK(std::equal(expected_true.begin(), expected_true.end(), out_true.begin()));
      CHECK(std::equal(expected_false.begin(), expected_false.end(), out_false.begin()));
    }

    //*************************************************************************
    TEST(ranges_partition_copy_preserves_order)
    {
      std::vector<int> src{2, 8, 1, 7, 3, 6, 4, 5};
      std::vector<int> out_true(8, 0);
      std::vector<int> out_false(8, 0);
      auto             pred = [](const int& v)
      {
        return v <= 4;
      };

      auto result = etl::ranges::partition_copy(src, out_true.begin(), out_false.begin(), pred);

      // partition_copy is a stable operation - relative order is preserved
      std::vector<int> expected_true{2, 1, 3, 4};
      std::vector<int> expected_false{8, 7, 6, 5};

      CHECK_EQUAL(4, std::distance(out_true.begin(), result.out1));
      CHECK_EQUAL(4, std::distance(out_false.begin(), result.out2));

      CHECK(std::equal(expected_true.begin(), expected_true.end(), out_true.begin()));
      CHECK(std::equal(expected_false.begin(), expected_false.end(), out_false.begin()));
    }

    //*************************************************************************
    TEST(ranges_partition_copy_matches_std)
    {
      int  data1[]      = {1, 2, 3, 4, 5, 6, 7, 8};
      int  std_true[8]  = {};
      int  std_false[8] = {};
      int  etl_true[8]  = {};
      int  etl_false[8] = {};
      auto pred         = [](const int& v)
      {
        return v > 4;
      };

      std::partition_copy(std::begin(data1), std::end(data1), std::begin(std_true), std::begin(std_false), pred);
      auto result = etl::ranges::partition_copy(std::begin(data1), std::end(data1), std::begin(etl_true), std::begin(etl_false), pred);

      CHECK(result.in == std::end(data1));

      bool are_equal;
      are_equal = std::equal(std::begin(std_true), std::end(std_true), std::begin(etl_true));
      CHECK(are_equal);

      are_equal = std::equal(std::begin(std_false), std::end(std_false), std::begin(etl_false));
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(ranges_partition_point_iterator_sentinel)
    {
      std::vector<int> vec{1, 3, 5, 7, 2, 4, 6, 8};
      auto             pred = [](const int& v)
      {
        return v % 2 != 0;
      };

      auto result   = etl::ranges::partition_point(vec.begin(), vec.end(), pred);
      auto expected = std::partition_point(vec.begin(), vec.end(), pred);

      CHECK(result == expected);
    }

    //*************************************************************************
    TEST(ranges_partition_point_range)
    {
      std::vector<int> vec{1, 3, 5, 7, 2, 4, 6, 8};
      auto             pred = [](const int& v)
      {
        return v % 2 != 0;
      };

      auto result   = etl::ranges::partition_point(vec, pred);
      auto expected = std::partition_point(vec.begin(), vec.end(), pred);

      CHECK(result == expected);
    }

    //*************************************************************************
    TEST(ranges_partition_point_all_true)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};
      auto             pred = [](const int& v)
      {
        return v % 2 != 0;
      };

      auto result = etl::ranges::partition_point(vec.begin(), vec.end(), pred);

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_partition_point_all_false)
    {
      std::vector<int> vec{2, 4, 6, 8};
      auto             pred = [](const int& v)
      {
        return v % 2 != 0;
      };

      auto result = etl::ranges::partition_point(vec.begin(), vec.end(), pred);

      CHECK(result == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_partition_point_empty)
    {
      std::vector<int> vec{};
      auto             pred = [](const int& v)
      {
        return v % 2 != 0;
      };

      auto result = etl::ranges::partition_point(vec.begin(), vec.end(), pred);

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_partition_point_single_true)
    {
      std::vector<int> vec{1};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_point(vec.begin(), vec.end(), pred);

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_partition_point_single_false)
    {
      std::vector<int> vec{20};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::partition_point(vec.begin(), vec.end(), pred);

      CHECK(result == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_partition_point_with_projection)
    {
      std::vector<int> vec{1, 2, 3, 10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      // With projection: values become 10, 20, 30, 100, 200, 300
      // pred(proj(v)) < 100 is true for first 3 elements
      auto result = etl::ranges::partition_point(vec.begin(), vec.end(), pred, proj);

      CHECK(result == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_partition_point_with_projection_range)
    {
      std::vector<int> vec{1, 2, 3, 10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      auto result = etl::ranges::partition_point(vec, pred, proj);

      CHECK(result == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_partition_point_matches_std)
    {
      int data1[] = {10, 8, 6, 4, 3, 2, 1};

      auto pred = [](const int& v)
      {
        return v > 4;
      };

      int* std_result = std::partition_point(std::begin(data1), std::end(data1), pred);
      auto etl_result = etl::ranges::partition_point(std::begin(data1), std::end(data1), pred);

      CHECK(std_result == etl_result);
    }

    //*************************************************************************
    TEST(ranges_stable_partition_iterator_sentinel)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::stable_partition(vec.begin(), vec.end(), pred);

      // All elements before the partition point should satisfy the predicate
      for (auto it = vec.begin(); it != result.begin(); ++it)
      {
        CHECK(*it < 10);
      }

      // All elements from the partition point onward should not satisfy the
      // predicate
      for (auto it = result.begin(); it != result.end(); ++it)
      {
        CHECK(*it >= 10);
      }

      // Verify stability: relative order of elements should be preserved
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(2, vec[2]);
      CHECK_EQUAL(20, vec[3]);
      CHECK_EQUAL(10, vec[4]);
      CHECK_EQUAL(30, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_stable_partition_range)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::stable_partition(vec, pred);

      for (auto it = vec.begin(); it != result.begin(); ++it)
      {
        CHECK(*it < 10);
      }

      for (auto it = result.begin(); it != result.end(); ++it)
      {
        CHECK(*it >= 10);
      }

      // Verify stability
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(2, vec[2]);
      CHECK_EQUAL(20, vec[3]);
      CHECK_EQUAL(10, vec[4]);
      CHECK_EQUAL(30, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_stable_partition_already_partitioned)
    {
      std::vector<int> vec{1, 2, 3, 10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::stable_partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.begin() + 3);
      CHECK(result.end() == vec.end());

      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(2, vec[1]);
      CHECK_EQUAL(3, vec[2]);
      CHECK_EQUAL(10, vec[3]);
      CHECK_EQUAL(20, vec[4]);
      CHECK_EQUAL(30, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_stable_partition_all_true)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::stable_partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_stable_partition_all_false)
    {
      std::vector<int> vec{10, 20, 30};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::stable_partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_stable_partition_empty)
    {
      std::vector<int> vec{};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::stable_partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_stable_partition_single_true)
    {
      std::vector<int> vec{1};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::stable_partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.end());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_stable_partition_single_false)
    {
      std::vector<int> vec{20};
      auto             pred = [](const int& v)
      {
        return v < 10;
      };

      auto result = etl::ranges::stable_partition(vec.begin(), vec.end(), pred);

      CHECK(result.begin() == vec.begin());
      CHECK(result.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_stable_partition_with_projection)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      // With projection: values become 10,200,30,100,20,300
      // pred(proj(v)) < 100: true for 1,3,2 (proj gives 10,30,20), false for
      // 20,10,30 (proj gives 200,100,300)
      auto result = etl::ranges::stable_partition(vec.begin(), vec.end(), pred, proj);

      for (auto it = vec.begin(); it != result.begin(); ++it)
      {
        CHECK((*it * 10) < 100);
      }

      for (auto it = result.begin(); it != result.end(); ++it)
      {
        CHECK((*it * 10) >= 100);
      }

      // Verify stability
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(2, vec[2]);
      CHECK_EQUAL(20, vec[3]);
      CHECK_EQUAL(10, vec[4]);
      CHECK_EQUAL(30, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_stable_partition_with_projection_range)
    {
      std::vector<int> vec{1, 20, 3, 10, 2, 30};
      auto             pred = [](const int& v)
      {
        return v < 100;
      };
      auto proj = [](const int& v)
      {
        return v * 10;
      };

      auto result = etl::ranges::stable_partition(vec, pred, proj);

      for (auto it = vec.begin(); it != result.begin(); ++it)
      {
        CHECK((*it * 10) < 100);
      }

      for (auto it = result.begin(); it != result.end(); ++it)
      {
        CHECK((*it * 10) >= 100);
      }

      // Verify stability
      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
      CHECK_EQUAL(2, vec[2]);
      CHECK_EQUAL(20, vec[3]);
      CHECK_EQUAL(10, vec[4]);
      CHECK_EQUAL(30, vec[5]);
    }

    //*************************************************************************
    TEST(ranges_stable_partition_preserves_elements)
    {
      std::vector<int> vec{5, 1, 4, 2, 3};
      std::vector<int> sorted_original{1, 2, 3, 4, 5};
      auto             pred = [](const int& v)
      {
        return v <= 3;
      };

      etl::ranges::stable_partition(vec, pred);

      // All original elements should still be present
      std::vector<int> sorted_result(vec.begin(), vec.end());
      std::sort(sorted_result.begin(), sorted_result.end());
      CHECK(sorted_result == sorted_original);
    }

    //*************************************************************************
    TEST(ranges_stable_partition_matches_std)
    {
      std::vector<int> data_std{1, 20, 3, 10, 2, 30, 5, 15, 7};
      std::vector<int> data_etl = data_std;
      auto             pred     = [](const int& v)
      {
        return v < 10;
      };

      std::stable_partition(data_std.begin(), data_std.end(), pred);
      etl::ranges::stable_partition(data_etl.begin(), data_etl.end(), pred);

      bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(ranges_sort_iterator)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      std::vector<int> expected{1, 2, 3, 4, 5};

      auto result = etl::ranges::sort(vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_sort_range)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      std::vector<int> expected{1, 2, 3, 4, 5};

      auto result = etl::ranges::sort(vec);

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_sort_with_comparator_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected{5, 4, 3, 2, 1};

      auto result = etl::ranges::sort(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_sort_with_comparator_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected{5, 4, 3, 2, 1};

      auto result = etl::ranges::sort(vec, etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_sort_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {2, 20}};

      etl::ranges::sort(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(2, vec[1].key);
      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(10, vec[0].value);
      CHECK_EQUAL(20, vec[1].value);
      CHECK_EQUAL(30, vec[2].value);
    }

    //*************************************************************************
    TEST(ranges_sort_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {2, 20}};

      etl::ranges::sort(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(2, vec[1].key);
      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(10, vec[0].value);
      CHECK_EQUAL(20, vec[1].value);
      CHECK_EQUAL(30, vec[2].value);
    }

    //*************************************************************************
    TEST(ranges_sort_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::sort(vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(vec.empty());
    }

    //*************************************************************************
    TEST(ranges_sort_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::sort(vec);

      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_sort_already_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected{1, 2, 3, 4, 5};

      etl::ranges::sort(vec);

      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_sort_duplicates)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
      std::vector<int> expected{1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9};

      etl::ranges::sort(vec);

      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_sort_matches_std)
    {
      std::vector<int> data_std{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      std::vector<int> data_etl = data_std;

      std::sort(data_std.begin(), data_std.end());
      etl::ranges::sort(data_etl);

      bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_iterator)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      std::vector<int> expected{1, 2, 3, 4, 5};

      auto result = etl::ranges::stable_sort(vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_range)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      std::vector<int> expected{1, 2, 3, 4, 5};

      auto result = etl::ranges::stable_sort(vec);

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_with_comparator_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected{5, 4, 3, 2, 1};

      auto result = etl::ranges::stable_sort(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_with_comparator_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected{5, 4, 3, 2, 1};

      auto result = etl::ranges::stable_sort(vec, etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {2, 20}};

      etl::ranges::stable_sort(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(2, vec[1].key);
      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(10, vec[0].value);
      CHECK_EQUAL(20, vec[1].value);
      CHECK_EQUAL(30, vec[2].value);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {2, 20}};

      etl::ranges::stable_sort(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(2, vec[1].key);
      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(10, vec[0].value);
      CHECK_EQUAL(20, vec[1].value);
      CHECK_EQUAL(30, vec[2].value);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::stable_sort(vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(vec.empty());
    }

    //*************************************************************************
    TEST(ranges_stable_sort_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::stable_sort(vec);

      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_already_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected{1, 2, 3, 4, 5};

      etl::ranges::stable_sort(vec);

      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_duplicates)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
      std::vector<int> expected{1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9};

      etl::ranges::stable_sort(vec);

      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_stability)
    {
      // Verify stability: elements with equal keys preserve their relative
      // order
      struct Item
      {
        int key;
        int order;
      };
      std::vector<Item> vec{{2, 0}, {1, 1}, {2, 2}, {1, 3}, {3, 4}, {2, 5}};

      etl::ranges::stable_sort(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      // key==1 items should keep original relative order
      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(1, vec[0].order);
      CHECK_EQUAL(1, vec[1].key);
      CHECK_EQUAL(3, vec[1].order);
      // key==2 items should keep original relative order
      CHECK_EQUAL(2, vec[2].key);
      CHECK_EQUAL(0, vec[2].order);
      CHECK_EQUAL(2, vec[3].key);
      CHECK_EQUAL(2, vec[3].order);
      CHECK_EQUAL(2, vec[4].key);
      CHECK_EQUAL(5, vec[4].order);
      // key==3 items
      CHECK_EQUAL(3, vec[5].key);
      CHECK_EQUAL(4, vec[5].order);
    }

    //*************************************************************************
    TEST(ranges_stable_sort_matches_std)
    {
      std::vector<int> data_std{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      std::vector<int> data_etl = data_std;

      std::stable_sort(data_std.begin(), data_std.end());
      etl::ranges::stable_sort(data_etl);

      bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_iterator)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      std::vector<int> expected_prefix{1, 2, 3};

      auto result = etl::ranges::partial_sort(vec.begin(), vec.begin() + 3, vec.end());

      CHECK(result == vec.end());
      CHECK(std::equal(expected_prefix.begin(), expected_prefix.end(), vec.begin()));
    }

    //*************************************************************************
    TEST(ranges_partial_sort_range)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      std::vector<int> expected_prefix{1, 2, 3};

      auto result = etl::ranges::partial_sort(vec, vec.begin() + 3);

      CHECK(result == vec.end());
      CHECK(std::equal(expected_prefix.begin(), expected_prefix.end(), vec.begin()));
    }

    //*************************************************************************
    TEST(ranges_partial_sort_with_comparator_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected_prefix{5, 4, 3};

      auto result = etl::ranges::partial_sort(vec.begin(), vec.begin() + 3, vec.end(), etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(std::equal(expected_prefix.begin(), expected_prefix.end(), vec.begin()));
    }

    //*************************************************************************
    TEST(ranges_partial_sort_with_comparator_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      std::vector<int> expected_prefix{5, 4, 3};

      auto result = etl::ranges::partial_sort(vec, vec.begin() + 3, etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(std::equal(expected_prefix.begin(), expected_prefix.end(), vec.begin()));
    }

    //*************************************************************************
    TEST(ranges_partial_sort_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {5, 50}, {2, 20}, {4, 40}};

      etl::ranges::partial_sort(vec.begin(), vec.begin() + 3, vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(2, vec[1].key);
      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(10, vec[0].value);
      CHECK_EQUAL(20, vec[1].value);
      CHECK_EQUAL(30, vec[2].value);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {5, 50}, {2, 20}, {4, 40}};

      etl::ranges::partial_sort(vec, vec.begin() + 3, etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(2, vec[1].key);
      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(10, vec[0].value);
      CHECK_EQUAL(20, vec[1].value);
      CHECK_EQUAL(30, vec[2].value);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::partial_sort(vec.begin(), vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(vec.empty());
    }

    //*************************************************************************
    TEST(ranges_partial_sort_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::partial_sort(vec, vec.begin() + 1);

      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_middle_equals_last)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      std::vector<int> expected{1, 2, 3, 4, 5};

      auto result = etl::ranges::partial_sort(vec.begin(), vec.end(), vec.end());

      CHECK(result == vec.end());
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_middle_equals_first)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      std::vector<int> original = vec;

      auto result = etl::ranges::partial_sort(vec.begin(), vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(vec == original);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_matches_std)
    {
      std::vector<int> data_std{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      std::vector<int> data_etl = data_std;

      std::partial_sort(data_std.begin(), data_std.begin() + 4, data_std.end());
      etl::ranges::partial_sort(data_etl.begin(), data_etl.begin() + 4, data_etl.end());

      // The first 4 elements should be the same smallest values in sorted order
      bool prefix_equal = std::equal(data_std.begin(), data_std.begin() + 4, data_etl.begin());
      CHECK(prefix_equal);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_iterator)
    {
      std::vector<int> input{5, 3, 1, 4, 2};
      std::vector<int> output(3, 0);
      std::vector<int> expected{1, 2, 3};

      auto result = etl::ranges::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      CHECK(result.in == input.end());
      CHECK(result.out == output.end());
      CHECK(output == expected);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_range)
    {
      std::vector<int> input{5, 3, 1, 4, 2};
      std::vector<int> output(3, 0);
      std::vector<int> expected{1, 2, 3};

      auto result = etl::ranges::partial_sort_copy(input, output);

      CHECK(result.in == input.end());
      CHECK(result.out == output.end());
      CHECK(output == expected);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_with_comparator_iterator)
    {
      std::vector<int> input{1, 2, 3, 4, 5};
      std::vector<int> output(3, 0);
      std::vector<int> expected{5, 4, 3};

      auto result = etl::ranges::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end(), etl::greater<int>{});

      CHECK(result.in == input.end());
      CHECK(result.out == output.end());
      CHECK(output == expected);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_with_comparator_range)
    {
      std::vector<int> input{1, 2, 3, 4, 5};
      std::vector<int> output(3, 0);
      std::vector<int> expected{5, 4, 3};

      auto result = etl::ranges::partial_sort_copy(input, output, etl::greater<int>{});

      CHECK(result.in == input.end());
      CHECK(result.out == output.end());
      CHECK(output == expected);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> input{{3, 30}, {1, 10}, {5, 50}, {2, 20}, {4, 40}};
      std::vector<Item> output(3, Item{0, 0});

      etl::ranges::partial_sort_copy(
        input.begin(), input.end(), output.begin(), output.end(), etl::ranges::less{}, [](const Item& item) { return item.key; },
        [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, output[0].key);
      CHECK_EQUAL(2, output[1].key);
      CHECK_EQUAL(3, output[2].key);
      CHECK_EQUAL(10, output[0].value);
      CHECK_EQUAL(20, output[1].value);
      CHECK_EQUAL(30, output[2].value);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> input{{3, 30}, {1, 10}, {5, 50}, {2, 20}, {4, 40}};
      std::vector<Item> output(3, Item{0, 0});

      etl::ranges::partial_sort_copy(
        input, output, etl::ranges::less{}, [](const Item& item) { return item.key; }, [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, output[0].key);
      CHECK_EQUAL(2, output[1].key);
      CHECK_EQUAL(3, output[2].key);
      CHECK_EQUAL(10, output[0].value);
      CHECK_EQUAL(20, output[1].value);
      CHECK_EQUAL(30, output[2].value);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_empty_input)
    {
      std::vector<int> input{};
      std::vector<int> output(3, 0);
      std::vector<int> expected{0, 0, 0};

      auto result = etl::ranges::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      CHECK(result.in == input.end());
      CHECK(result.out == output.begin());
      CHECK(output == expected);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_empty_output)
    {
      std::vector<int> input{5, 3, 1, 4, 2};
      std::vector<int> output{};

      auto result = etl::ranges::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      CHECK(result.in == input.end());
      CHECK(result.out == output.end());
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_output_larger_than_input)
    {
      std::vector<int> input{3, 1, 2};
      std::vector<int> output(5, 0);
      std::vector<int> expected{1, 2, 3, 0, 0};

      auto result = etl::ranges::partial_sort_copy(input.begin(), input.end(), output.begin(), output.end());

      CHECK(result.in == input.end());
      CHECK(result.out == output.begin() + 3);
      CHECK(output == expected);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_output_same_size_as_input)
    {
      std::vector<int> input{5, 3, 1, 4, 2};
      std::vector<int> output(5, 0);
      std::vector<int> expected{1, 2, 3, 4, 5};

      auto result = etl::ranges::partial_sort_copy(input, output);

      CHECK(result.in == input.end());
      CHECK(result.out == output.end());
      CHECK(output == expected);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_single_element)
    {
      std::vector<int> input{42};
      std::vector<int> output(1, 0);

      auto result = etl::ranges::partial_sort_copy(input, output);

      CHECK(result.in == input.end());
      CHECK(result.out == output.end());
      CHECK_EQUAL(42, output[0]);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_does_not_modify_input)
    {
      std::vector<int> input{5, 3, 1, 4, 2};
      std::vector<int> original = input;
      std::vector<int> output(3, 0);

      etl::ranges::partial_sort_copy(input, output);

      CHECK(input == original);
    }

    //*************************************************************************
    TEST(ranges_partial_sort_copy_matches_std)
    {
      std::vector<int> data{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      std::vector<int> output_std(4, 0);
      std::vector<int> output_etl(4, 0);

      std::partial_sort_copy(data.begin(), data.end(), output_std.begin(), output_std.end());
      etl::ranges::partial_sort_copy(data.begin(), data.end(), output_etl.begin(), output_etl.end());

      CHECK(output_std == output_etl);
    }

    //*************************************************************************
    TEST(ranges_nth_element_iterator)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};

      auto result = etl::ranges::nth_element(vec.begin(), vec.begin() + 2, vec.end());

      CHECK(result == vec.end());
      CHECK_EQUAL(3, vec[2]);

      // All elements before nth should be <= vec[2]
      for (size_t i = 0; i < 2; ++i)
      {
        CHECK(vec[i] <= vec[2]);
      }

      // All elements after nth should be >= vec[2]
      for (size_t i = 3; i < 5; ++i)
      {
        CHECK(vec[i] >= vec[2]);
      }
    }

    //*************************************************************************
    TEST(ranges_nth_element_range)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};

      auto result = etl::ranges::nth_element(vec, vec.begin() + 2);

      CHECK(result == vec.end());
      CHECK_EQUAL(3, vec[2]);

      for (size_t i = 0; i < 2; ++i)
      {
        CHECK(vec[i] <= vec[2]);
      }

      for (size_t i = 3; i < 5; ++i)
      {
        CHECK(vec[i] >= vec[2]);
      }
    }

    //*************************************************************************
    TEST(ranges_nth_element_with_comparator_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result = etl::ranges::nth_element(vec.begin(), vec.begin() + 2, vec.end(), etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK_EQUAL(3, vec[2]);

      for (size_t i = 0; i < 2; ++i)
      {
        CHECK(vec[i] >= vec[2]);
      }

      for (size_t i = 3; i < 5; ++i)
      {
        CHECK(vec[i] <= vec[2]);
      }
    }

    //*************************************************************************
    TEST(ranges_nth_element_with_comparator_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result = etl::ranges::nth_element(vec, vec.begin() + 2, etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK_EQUAL(3, vec[2]);

      for (size_t i = 0; i < 2; ++i)
      {
        CHECK(vec[i] >= vec[2]);
      }

      for (size_t i = 3; i < 5; ++i)
      {
        CHECK(vec[i] <= vec[2]);
      }
    }

    //*************************************************************************
    TEST(ranges_nth_element_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {5, 50}, {2, 20}, {4, 40}};

      etl::ranges::nth_element(vec.begin(), vec.begin() + 2, vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(30, vec[2].value);

      for (size_t i = 0; i < 2; ++i)
      {
        CHECK(vec[i].key <= vec[2].key);
      }

      for (size_t i = 3; i < 5; ++i)
      {
        CHECK(vec[i].key >= vec[2].key);
      }
    }

    //*************************************************************************
    TEST(ranges_nth_element_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {5, 50}, {2, 20}, {4, 40}};

      etl::ranges::nth_element(vec, vec.begin() + 2, etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(30, vec[2].value);

      for (size_t i = 0; i < 2; ++i)
      {
        CHECK(vec[i].key <= vec[2].key);
      }

      for (size_t i = 3; i < 5; ++i)
      {
        CHECK(vec[i].key >= vec[2].key);
      }
    }

    //*************************************************************************
    TEST(ranges_nth_element_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::nth_element(vec.begin(), vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(vec.empty());
    }

    //*************************************************************************
    TEST(ranges_nth_element_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::nth_element(vec, vec.begin());

      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_nth_element_first_position)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};

      etl::ranges::nth_element(vec.begin(), vec.begin(), vec.end());

      CHECK_EQUAL(1, vec[0]);

      for (size_t i = 1; i < vec.size(); ++i)
      {
        CHECK(vec[i] >= vec[0]);
      }
    }

    //*************************************************************************
    TEST(ranges_nth_element_last_position)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};

      etl::ranges::nth_element(vec.begin(), vec.begin() + 4, vec.end());

      CHECK_EQUAL(5, vec[4]);

      for (size_t i = 0; i < 4; ++i)
      {
        CHECK(vec[i] <= vec[4]);
      }
    }

    //*************************************************************************
    TEST(ranges_nth_element_matches_std)
    {
      std::vector<int> data_std{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      std::vector<int> data_etl = data_std;

      std::nth_element(data_std.begin(), data_std.begin() + 4, data_std.end());
      etl::ranges::nth_element(data_etl.begin(), data_etl.begin() + 4, data_etl.end());

      // The element at position 4 should be the same
      CHECK_EQUAL(data_std[4], data_etl[4]);
    }

    //*************************************************************************
    TEST(ranges_is_sorted_until_iterator_sentinel_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result = etl::ranges::is_sorted_until(vec.begin(), vec.end());

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_sorted_until_range_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result = etl::ranges::is_sorted_until(vec);

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_sorted_until_iterator_sentinel_not_sorted)
    {
      std::vector<int> vec{1, 2, 5, 4, 3};

      auto result = etl::ranges::is_sorted_until(vec.begin(), vec.end());

      CHECK(result == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_is_sorted_until_range_not_sorted)
    {
      std::vector<int> vec{1, 2, 5, 4, 3};

      auto result = etl::ranges::is_sorted_until(vec);

      CHECK(result == vec.begin() + 3);
    }

    //*************************************************************************
    TEST(ranges_is_sorted_until_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::is_sorted_until(vec.begin(), vec.end());
      CHECK(result == vec.end());

      result = etl::ranges::is_sorted_until(vec);
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_sorted_until_single)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::is_sorted_until(vec.begin(), vec.end());
      CHECK(result == vec.end());

      result = etl::ranges::is_sorted_until(vec);
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_sorted_until_with_comparator)
    {
      std::vector<int> vec{5, 4, 3, 2, 1};

      auto result = etl::ranges::is_sorted_until(vec.begin(), vec.end(), etl::greater<int>{});
      CHECK(result == vec.end());

      result = etl::ranges::is_sorted_until(vec, etl::greater<int>{});
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_sorted_until_with_projection)
    {
      std::vector<int> vec{-1, -2, -3, -4, -5};
      auto             proj = [](const int& v)
      {
        return -v;
      };

      // After projection, values become 1,2,3,4,5 which is sorted
      auto result = etl::ranges::is_sorted_until(vec.begin(), vec.end(), etl::ranges::less{}, proj);
      CHECK(result == vec.end());

      result = etl::ranges::is_sorted_until(vec, etl::ranges::less{}, proj);
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_sorted_iterator_sentinel_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      CHECK(etl::ranges::is_sorted(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_range_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      CHECK(etl::ranges::is_sorted(vec));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_iterator_sentinel_not_sorted)
    {
      std::vector<int> vec{1, 3, 2, 4, 5};

      CHECK_FALSE(etl::ranges::is_sorted(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_range_not_sorted)
    {
      std::vector<int> vec{1, 3, 2, 4, 5};

      CHECK_FALSE(etl::ranges::is_sorted(vec));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_empty)
    {
      std::vector<int> vec{};

      CHECK(etl::ranges::is_sorted(vec.begin(), vec.end()));
      CHECK(etl::ranges::is_sorted(vec));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_single)
    {
      std::vector<int> vec{42};

      CHECK(etl::ranges::is_sorted(vec.begin(), vec.end()));
      CHECK(etl::ranges::is_sorted(vec));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_with_comparator)
    {
      std::vector<int> vec{5, 4, 3, 2, 1};

      CHECK(etl::ranges::is_sorted(vec.begin(), vec.end(), etl::greater<int>{}));
      CHECK(etl::ranges::is_sorted(vec, etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_with_comparator_not_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      CHECK_FALSE(etl::ranges::is_sorted(vec.begin(), vec.end(), etl::greater<int>{}));
      CHECK_FALSE(etl::ranges::is_sorted(vec, etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_with_projection)
    {
      std::vector<int> vec{-1, -2, -3, -4, -5};
      auto             proj = [](const int& v)
      {
        return -v;
      };

      // After projection, values become 1,2,3,4,5 which is sorted
      CHECK(etl::ranges::is_sorted(vec.begin(), vec.end(), etl::ranges::less{}, proj));
      CHECK(etl::ranges::is_sorted(vec, etl::ranges::less{}, proj));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_with_projection_not_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};
      auto             proj = [](const int& v)
      {
        return -v;
      };

      // After projection, values become -1,-2,-3,-4,-5 which is not sorted
      // ascending
      CHECK_FALSE(etl::ranges::is_sorted(vec.begin(), vec.end(), etl::ranges::less{}, proj));
      CHECK_FALSE(etl::ranges::is_sorted(vec, etl::ranges::less{}, proj));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_equal_elements)
    {
      std::vector<int> vec{3, 3, 3, 3, 3};

      CHECK(etl::ranges::is_sorted(vec.begin(), vec.end()));
      CHECK(etl::ranges::is_sorted(vec));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_two_elements_sorted)
    {
      std::vector<int> vec{1, 2};

      CHECK(etl::ranges::is_sorted(vec.begin(), vec.end()));
      CHECK(etl::ranges::is_sorted(vec));
    }

    //*************************************************************************
    TEST(ranges_is_sorted_two_elements_not_sorted)
    {
      std::vector<int> vec{2, 1};

      CHECK_FALSE(etl::ranges::is_sorted(vec.begin(), vec.end()));
      CHECK_FALSE(etl::ranges::is_sorted(vec));
    }

    //*************************************************************************
    TEST(ranges_lower_bound_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      auto result = etl::ranges::lower_bound(vec.begin(), vec.end(), 5);
      CHECK_EQUAL(5, *result);
      CHECK_EQUAL(4, etl::distance(vec.begin(), result));
    }

    //*************************************************************************
    TEST(ranges_lower_bound_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      auto result = etl::ranges::lower_bound(vec, 5);
      CHECK_EQUAL(5, *result);
      CHECK_EQUAL(4, etl::distance(vec.begin(), result));
    }

    //*************************************************************************
    TEST(ranges_lower_bound_value_not_present)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};

      auto result_it = etl::ranges::lower_bound(vec.begin(), vec.end(), 4);
      CHECK_EQUAL(5, *result_it);
      CHECK_EQUAL(2, etl::distance(vec.begin(), result_it));

      auto result_r = etl::ranges::lower_bound(vec, 4);
      CHECK_EQUAL(5, *result_r);
      CHECK_EQUAL(2, etl::distance(vec.begin(), result_r));
    }

    //*************************************************************************
    TEST(ranges_lower_bound_value_at_beginning)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result_it = etl::ranges::lower_bound(vec.begin(), vec.end(), 1);
      CHECK(result_it == vec.begin());

      auto result_r = etl::ranges::lower_bound(vec, 1);
      CHECK(result_r == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_lower_bound_value_past_end)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result_it = etl::ranges::lower_bound(vec.begin(), vec.end(), 10);
      CHECK(result_it == vec.end());

      auto result_r = etl::ranges::lower_bound(vec, 10);
      CHECK(result_r == vec.end());
    }

    //*************************************************************************
    TEST(ranges_lower_bound_empty)
    {
      std::vector<int> vec{};

      auto result_it = etl::ranges::lower_bound(vec.begin(), vec.end(), 5);
      CHECK(result_it == vec.end());

      auto result_r = etl::ranges::lower_bound(vec, 5);
      CHECK(result_r == vec.end());
    }

    //*************************************************************************
    TEST(ranges_lower_bound_with_comparator_iterator)
    {
      std::vector<int> vec{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      auto result = etl::ranges::lower_bound(vec.begin(), vec.end(), 5, etl::greater<int>{});
      CHECK_EQUAL(5, *result);
      CHECK_EQUAL(5, etl::distance(vec.begin(), result));
    }

    //*************************************************************************
    TEST(ranges_lower_bound_with_comparator_range)
    {
      std::vector<int> vec{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      auto result = etl::ranges::lower_bound(vec, 5, etl::greater<int>{});
      CHECK_EQUAL(5, *result);
      CHECK_EQUAL(5, etl::distance(vec.begin(), result));
    }

    //*************************************************************************
    TEST(ranges_lower_bound_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}};

      auto result = etl::ranges::lower_bound(vec.begin(), vec.end(), 3, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK_EQUAL(3, result->key);
      CHECK_EQUAL(30, result->value);
    }

    //*************************************************************************
    TEST(ranges_lower_bound_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}};

      auto result = etl::ranges::lower_bound(vec, 3, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK_EQUAL(3, result->key);
      CHECK_EQUAL(30, result->value);
    }

    //*************************************************************************
    TEST(ranges_lower_bound_duplicates)
    {
      std::vector<int> vec{1, 2, 2, 2, 3, 4, 5};

      auto result_it = etl::ranges::lower_bound(vec.begin(), vec.end(), 2);
      CHECK_EQUAL(2, *result_it);
      CHECK_EQUAL(1, etl::distance(vec.begin(), result_it));

      auto result_r = etl::ranges::lower_bound(vec, 2);
      CHECK_EQUAL(2, *result_r);
      CHECK_EQUAL(1, etl::distance(vec.begin(), result_r));
    }

    //*************************************************************************
    TEST(ranges_lower_bound_single_element)
    {
      std::vector<int> vec{5};

      auto result_found = etl::ranges::lower_bound(vec, 5);
      CHECK(result_found == vec.begin());

      auto result_less = etl::ranges::lower_bound(vec, 3);
      CHECK(result_less == vec.begin());

      auto result_greater = etl::ranges::lower_bound(vec, 10);
      CHECK(result_greater == vec.end());
    }

    //*************************************************************************
    TEST(ranges_lower_bound_matches_std)
    {
      std::vector<int> vec{1, 3, 5, 7, 9, 11, 13, 15};

      for (int val = 0; val <= 16; ++val)
      {
        auto std_result = std::lower_bound(vec.begin(), vec.end(), val);
        auto etl_result = etl::ranges::lower_bound(vec.begin(), vec.end(), val);
        CHECK(std_result == etl_result);
      }
    }

    //*************************************************************************
    TEST(ranges_upper_bound_iterator)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      auto result = etl::ranges::upper_bound(vec.begin(), vec.end(), 5);
      CHECK_EQUAL(6, *result);
      CHECK_EQUAL(5, etl::distance(vec.begin(), result));
    }

    //*************************************************************************
    TEST(ranges_upper_bound_range)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      auto result = etl::ranges::upper_bound(vec, 5);
      CHECK_EQUAL(6, *result);
      CHECK_EQUAL(5, etl::distance(vec.begin(), result));
    }

    //*************************************************************************
    TEST(ranges_upper_bound_value_not_present)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};

      auto result_it = etl::ranges::upper_bound(vec.begin(), vec.end(), 4);
      CHECK_EQUAL(5, *result_it);
      CHECK_EQUAL(2, etl::distance(vec.begin(), result_it));

      auto result_r = etl::ranges::upper_bound(vec, 4);
      CHECK_EQUAL(5, *result_r);
      CHECK_EQUAL(2, etl::distance(vec.begin(), result_r));
    }

    //*************************************************************************
    TEST(ranges_upper_bound_value_at_beginning)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result_it = etl::ranges::upper_bound(vec.begin(), vec.end(), 1);
      CHECK_EQUAL(2, *result_it);
      CHECK_EQUAL(1, etl::distance(vec.begin(), result_it));

      auto result_r = etl::ranges::upper_bound(vec, 1);
      CHECK_EQUAL(2, *result_r);
      CHECK_EQUAL(1, etl::distance(vec.begin(), result_r));
    }

    //*************************************************************************
    TEST(ranges_upper_bound_value_past_end)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result_it = etl::ranges::upper_bound(vec.begin(), vec.end(), 10);
      CHECK(result_it == vec.end());

      auto result_r = etl::ranges::upper_bound(vec, 10);
      CHECK(result_r == vec.end());
    }

    //*************************************************************************
    TEST(ranges_upper_bound_empty)
    {
      std::vector<int> vec{};

      auto result_it = etl::ranges::upper_bound(vec.begin(), vec.end(), 5);
      CHECK(result_it == vec.end());

      auto result_r = etl::ranges::upper_bound(vec, 5);
      CHECK(result_r == vec.end());
    }

    //*************************************************************************
    TEST(ranges_upper_bound_with_comparator_iterator)
    {
      std::vector<int> vec{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      auto result = etl::ranges::upper_bound(vec.begin(), vec.end(), 5, etl::greater<int>{});
      CHECK_EQUAL(4, *result);
      CHECK_EQUAL(6, etl::distance(vec.begin(), result));
    }

    //*************************************************************************
    TEST(ranges_upper_bound_with_comparator_range)
    {
      std::vector<int> vec{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      auto result = etl::ranges::upper_bound(vec, 5, etl::greater<int>{});
      CHECK_EQUAL(4, *result);
      CHECK_EQUAL(6, etl::distance(vec.begin(), result));
    }

    //*************************************************************************
    TEST(ranges_upper_bound_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}};

      auto result = etl::ranges::upper_bound(vec.begin(), vec.end(), 3, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK_EQUAL(4, result->key);
      CHECK_EQUAL(40, result->value);
    }

    //*************************************************************************
    TEST(ranges_upper_bound_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}};

      auto result = etl::ranges::upper_bound(vec, 3, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK_EQUAL(4, result->key);
      CHECK_EQUAL(40, result->value);
    }

    //*************************************************************************
    TEST(ranges_upper_bound_duplicates)
    {
      std::vector<int> vec{1, 2, 2, 2, 3, 4, 5};

      auto result_it = etl::ranges::upper_bound(vec.begin(), vec.end(), 2);
      CHECK_EQUAL(3, *result_it);
      CHECK_EQUAL(4, etl::distance(vec.begin(), result_it));

      auto result_r = etl::ranges::upper_bound(vec, 2);
      CHECK_EQUAL(3, *result_r);
      CHECK_EQUAL(4, etl::distance(vec.begin(), result_r));
    }

    //*************************************************************************
    TEST(ranges_upper_bound_single_element)
    {
      std::vector<int> vec{5};

      auto result_found = etl::ranges::upper_bound(vec, 5);
      CHECK(result_found == vec.end());

      auto result_less = etl::ranges::upper_bound(vec, 3);
      CHECK(result_less == vec.begin());

      auto result_greater = etl::ranges::upper_bound(vec, 10);
      CHECK(result_greater == vec.end());
    }

    //*************************************************************************
    TEST(ranges_upper_bound_matches_std)
    {
      std::vector<int> vec{1, 3, 5, 7, 9, 11, 13, 15};

      for (int val = 0; val <= 16; ++val)
      {
        auto std_result = std::upper_bound(vec.begin(), vec.end(), val);
        auto etl_result = etl::ranges::upper_bound(vec.begin(), vec.end(), val);
        CHECK(std_result == etl_result);
      }
    }

    //*************************************************************************
    TEST(ranges_equal_range_iterator)
    {
      std::vector<int> vec{1, 2, 3, 3, 3, 4, 5, 6, 7, 8};

      auto result = etl::ranges::equal_range(vec.begin(), vec.end(), 3);
      CHECK_EQUAL(3, *result.begin());
      CHECK_EQUAL(2, etl::distance(vec.begin(), result.begin()));
      CHECK_EQUAL(5, etl::distance(vec.begin(), result.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_range)
    {
      std::vector<int> vec{1, 2, 3, 3, 3, 4, 5, 6, 7, 8};

      auto result = etl::ranges::equal_range(vec, 3);
      CHECK_EQUAL(3, *result.begin());
      CHECK_EQUAL(2, etl::distance(vec.begin(), result.begin()));
      CHECK_EQUAL(5, etl::distance(vec.begin(), result.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_value_not_present)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};

      auto result_it = etl::ranges::equal_range(vec.begin(), vec.end(), 4);
      CHECK(result_it.begin() == result_it.end());
      CHECK_EQUAL(2, etl::distance(vec.begin(), result_it.begin()));

      auto result_r = etl::ranges::equal_range(vec, 4);
      CHECK(result_r.begin() == result_r.end());
      CHECK_EQUAL(2, etl::distance(vec.begin(), result_r.begin()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_value_at_beginning)
    {
      std::vector<int> vec{1, 1, 1, 2, 3, 4, 5};

      auto result_it = etl::ranges::equal_range(vec.begin(), vec.end(), 1);
      CHECK(result_it.begin() == vec.begin());
      CHECK_EQUAL(3, etl::distance(result_it.begin(), result_it.end()));

      auto result_r = etl::ranges::equal_range(vec, 1);
      CHECK(result_r.begin() == vec.begin());
      CHECK_EQUAL(3, etl::distance(result_r.begin(), result_r.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_value_at_end)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 5, 5};

      auto result_it = etl::ranges::equal_range(vec.begin(), vec.end(), 5);
      CHECK_EQUAL(4, etl::distance(vec.begin(), result_it.begin()));
      CHECK(result_it.end() == vec.end());
      CHECK_EQUAL(3, etl::distance(result_it.begin(), result_it.end()));

      auto result_r = etl::ranges::equal_range(vec, 5);
      CHECK_EQUAL(4, etl::distance(vec.begin(), result_r.begin()));
      CHECK(result_r.end() == vec.end());
      CHECK_EQUAL(3, etl::distance(result_r.begin(), result_r.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_value_past_end)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      auto result_it = etl::ranges::equal_range(vec.begin(), vec.end(), 10);
      CHECK(result_it.begin() == vec.end());
      CHECK(result_it.end() == vec.end());

      auto result_r = etl::ranges::equal_range(vec, 10);
      CHECK(result_r.begin() == vec.end());
      CHECK(result_r.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_equal_range_empty)
    {
      std::vector<int> vec{};

      auto result_it = etl::ranges::equal_range(vec.begin(), vec.end(), 5);
      CHECK(result_it.begin() == vec.end());
      CHECK(result_it.end() == vec.end());

      auto result_r = etl::ranges::equal_range(vec, 5);
      CHECK(result_r.begin() == vec.end());
      CHECK(result_r.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_equal_range_with_comparator_iterator)
    {
      std::vector<int> vec{10, 9, 8, 7, 6, 5, 5, 5, 4, 3, 2, 1};

      auto result = etl::ranges::equal_range(vec.begin(), vec.end(), 5, etl::greater<int>{});
      CHECK_EQUAL(5, *result.begin());
      CHECK_EQUAL(5, etl::distance(vec.begin(), result.begin()));
      CHECK_EQUAL(8, etl::distance(vec.begin(), result.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_with_comparator_range)
    {
      std::vector<int> vec{10, 9, 8, 7, 6, 5, 5, 5, 4, 3, 2, 1};

      auto result = etl::ranges::equal_range(vec, 5, etl::greater<int>{});
      CHECK_EQUAL(5, *result.begin());
      CHECK_EQUAL(5, etl::distance(vec.begin(), result.begin()));
      CHECK_EQUAL(8, etl::distance(vec.begin(), result.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}, {3, 31}, {3, 32}, {4, 40}, {5, 50}};

      auto result = etl::ranges::equal_range(vec.begin(), vec.end(), 3, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK_EQUAL(3, result.begin()->key);
      CHECK_EQUAL(30, result.begin()->value);
      CHECK_EQUAL(3, etl::distance(result.begin(), result.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}, {3, 31}, {3, 32}, {4, 40}, {5, 50}};

      auto result = etl::ranges::equal_range(vec, 3, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK_EQUAL(3, result.begin()->key);
      CHECK_EQUAL(30, result.begin()->value);
      CHECK_EQUAL(3, etl::distance(result.begin(), result.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_duplicates)
    {
      std::vector<int> vec{1, 2, 2, 2, 3, 4, 5};

      auto result_it = etl::ranges::equal_range(vec.begin(), vec.end(), 2);
      CHECK_EQUAL(1, etl::distance(vec.begin(), result_it.begin()));
      CHECK_EQUAL(4, etl::distance(vec.begin(), result_it.end()));
      CHECK_EQUAL(3, etl::distance(result_it.begin(), result_it.end()));

      auto result_r = etl::ranges::equal_range(vec, 2);
      CHECK_EQUAL(1, etl::distance(vec.begin(), result_r.begin()));
      CHECK_EQUAL(4, etl::distance(vec.begin(), result_r.end()));
      CHECK_EQUAL(3, etl::distance(result_r.begin(), result_r.end()));
    }

    //*************************************************************************
    TEST(ranges_equal_range_single_element)
    {
      std::vector<int> vec{5};

      auto result_found = etl::ranges::equal_range(vec, 5);
      CHECK(result_found.begin() == vec.begin());
      CHECK(result_found.end() == vec.end());

      auto result_less = etl::ranges::equal_range(vec, 3);
      CHECK(result_less.begin() == vec.begin());
      CHECK(result_less.end() == vec.begin());

      auto result_greater = etl::ranges::equal_range(vec, 10);
      CHECK(result_greater.begin() == vec.end());
      CHECK(result_greater.end() == vec.end());
    }

    //*************************************************************************
    TEST(ranges_equal_range_matches_std)
    {
      std::vector<int> vec{1, 2, 2, 3, 3, 3, 5, 5, 7, 9, 11, 13, 15};

      for (int val = 0; val <= 16; ++val)
      {
        auto std_result = std::equal_range(vec.begin(), vec.end(), val);
        auto etl_result = etl::ranges::equal_range(vec.begin(), vec.end(), val);
        CHECK(std_result.first == etl_result.begin());
        CHECK(std_result.second == etl_result.end());
      }
    }

    //*************************************************************************
    TEST(ranges_binary_search_iterator_found)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      bool result = etl::ranges::binary_search(vec.begin(), vec.end(), 5);
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_binary_search_range_found)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

      bool result = etl::ranges::binary_search(vec, 5);
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_binary_search_not_found)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};

      bool result_it = etl::ranges::binary_search(vec.begin(), vec.end(), 4);
      CHECK(!result_it);

      bool result_r = etl::ranges::binary_search(vec, 4);
      CHECK(!result_r);
    }

    //*************************************************************************
    TEST(ranges_binary_search_value_at_beginning)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      bool result_it = etl::ranges::binary_search(vec.begin(), vec.end(), 1);
      CHECK(result_it);

      bool result_r = etl::ranges::binary_search(vec, 1);
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_binary_search_value_at_end)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      bool result_it = etl::ranges::binary_search(vec.begin(), vec.end(), 5);
      CHECK(result_it);

      bool result_r = etl::ranges::binary_search(vec, 5);
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_binary_search_value_past_end)
    {
      std::vector<int> vec{1, 2, 3, 4, 5};

      bool result_it = etl::ranges::binary_search(vec.begin(), vec.end(), 10);
      CHECK(!result_it);

      bool result_r = etl::ranges::binary_search(vec, 10);
      CHECK(!result_r);
    }

    //*************************************************************************
    TEST(ranges_binary_search_empty)
    {
      std::vector<int> vec{};

      bool result_it = etl::ranges::binary_search(vec.begin(), vec.end(), 5);
      CHECK(!result_it);

      bool result_r = etl::ranges::binary_search(vec, 5);
      CHECK(!result_r);
    }

    //*************************************************************************
    TEST(ranges_binary_search_with_comparator_iterator)
    {
      std::vector<int> vec{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      bool result = etl::ranges::binary_search(vec.begin(), vec.end(), 5, etl::greater<int>{});
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_binary_search_with_comparator_range)
    {
      std::vector<int> vec{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

      bool result = etl::ranges::binary_search(vec, 5, etl::greater<int>{});
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_binary_search_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}};

      bool result = etl::ranges::binary_search(vec.begin(), vec.end(), 3, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK(result);

      bool result_not_found = etl::ranges::binary_search(vec.begin(), vec.end(), 6, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK(!result_not_found);
    }

    //*************************************************************************
    TEST(ranges_binary_search_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}};

      bool result = etl::ranges::binary_search(vec, 3, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK(result);

      bool result_not_found = etl::ranges::binary_search(vec, 6, etl::ranges::less{}, [](const Item& item) { return item.key; });
      CHECK(!result_not_found);
    }

    //*************************************************************************
    TEST(ranges_binary_search_duplicates)
    {
      std::vector<int> vec{1, 2, 2, 2, 3, 4, 5};

      bool result_it = etl::ranges::binary_search(vec.begin(), vec.end(), 2);
      CHECK(result_it);

      bool result_r = etl::ranges::binary_search(vec, 2);
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_binary_search_single_element)
    {
      std::vector<int> vec{5};

      bool result_found = etl::ranges::binary_search(vec, 5);
      CHECK(result_found);

      bool result_less = etl::ranges::binary_search(vec, 3);
      CHECK(!result_less);

      bool result_greater = etl::ranges::binary_search(vec, 10);
      CHECK(!result_greater);
    }

    //*************************************************************************
    TEST(ranges_binary_search_matches_std)
    {
      std::vector<int> vec{1, 2, 2, 3, 3, 3, 5, 5, 7, 9, 11, 13, 15};

      for (int val = 0; val <= 16; ++val)
      {
        bool std_result = std::binary_search(vec.begin(), vec.end(), val);
        bool etl_result = etl::ranges::binary_search(vec.begin(), vec.end(), val);
        CHECK_EQUAL(std_result, etl_result);
      }
    }

    //*************************************************************************
    TEST(ranges_includes_iterator_basic)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> vec2{2, 4, 6};

      bool result = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_includes_range_basic)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      std::vector<int> vec2{2, 4, 6};

      bool result = etl::ranges::includes(vec1, vec2);
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_includes_not_included)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 6};

      bool result_it = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
      CHECK(!result_it);

      bool result_r = etl::ranges::includes(vec1, vec2);
      CHECK(!result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_empty_second_range)
    {
      std::vector<int> vec1{1, 2, 3};
      std::vector<int> vec2{};

      bool result_it = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
      CHECK(result_it);

      bool result_r = etl::ranges::includes(vec1, vec2);
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_empty_first_range)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{1, 2, 3};

      bool result_it = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
      CHECK(!result_it);

      bool result_r = etl::ranges::includes(vec1, vec2);
      CHECK(!result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_both_empty)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{};

      bool result_it = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
      CHECK(result_it);

      bool result_r = etl::ranges::includes(vec1, vec2);
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_identical_ranges)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5};
      std::vector<int> vec2{1, 2, 3, 4, 5};

      bool result_it = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
      CHECK(result_it);

      bool result_r = etl::ranges::includes(vec1, vec2);
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_with_duplicates)
    {
      std::vector<int> vec1{1, 2, 2, 3, 3, 3, 4, 5};
      std::vector<int> vec2{2, 3, 3};

      bool result_it = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
      CHECK(result_it);

      bool result_r = etl::ranges::includes(vec1, vec2);
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_with_duplicates_not_enough)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5};
      std::vector<int> vec2{2, 3, 3};

      bool result_it = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
      CHECK(!result_it);

      bool result_r = etl::ranges::includes(vec1, vec2);
      CHECK(!result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_with_custom_comparator)
    {
      std::vector<int> vec1{10, 8, 6, 4, 2};
      std::vector<int> vec2{8, 4, 2};

      bool result_it = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), etl::greater<int>{});
      CHECK(result_it);

      bool result_r = etl::ranges::includes(vec1, vec2, etl::greater<int>{});
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_with_projection)
    {
      struct Item
      {
        int key;
        int value;
      };

      std::vector<Item> vec1{{1, 10}, {2, 20}, {3, 30}, {4, 40}, {5, 50}};
      std::vector<Item> vec2{{2, 99}, {4, 99}};

      bool result_it = etl::ranges::includes(
        vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), etl::ranges::less{}, [](const Item& item) { return item.key; },
        [](const Item& item) { return item.key; });
      CHECK(result_it);

      bool result_r =
        etl::ranges::includes(vec1, vec2, etl::ranges::less{}, [](const Item& item) { return item.key; }, [](const Item& item) { return item.key; });
      CHECK(result_r);
    }

    //*************************************************************************
    TEST(ranges_includes_single_element_included)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5};
      std::vector<int> vec2{3};

      bool result = etl::ranges::includes(vec1, vec2);
      CHECK(result);
    }

    //*************************************************************************
    TEST(ranges_includes_single_element_not_included)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5};
      std::vector<int> vec2{6};

      bool result = etl::ranges::includes(vec1, vec2);
      CHECK(!result);
    }

    //*************************************************************************
    TEST(ranges_includes_matches_std)
    {
      std::vector<int>              vec1{1, 2, 2, 3, 3, 3, 5, 5, 7, 9, 11, 13, 15};
      std::vector<std::vector<int>> test_cases = {
        {1, 3, 5}, {2, 2, 3}, {1, 2, 3, 4}, {1, 15}, {}, {1, 2, 2, 3, 3, 3, 5, 5, 7, 9, 11, 13, 15}, {3, 3, 3, 3}, {0}, {16},
      };

      for (const auto& vec2 : test_cases)
      {
        bool std_result = std::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
        bool etl_result = etl::ranges::includes(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
        CHECK_EQUAL(std_result, etl_result);
      }
    }

    //*************************************************************************
    TEST(ranges_set_union_iterator_basic)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 6, 7, 8};
      std::vector<int> result(10);

      auto [in1, in2, out] = etl::ranges::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
      CHECK(in1 == vec1.end());
      CHECK(in2 == vec2.end());
    }

    //*************************************************************************
    TEST(ranges_set_union_range_basic)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 6, 7, 8};
      std::vector<int> result(10);

      auto [in1, in2, out] = etl::ranges::set_union(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_union_empty_first)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{1, 2, 3};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_union_empty_second)
    {
      std::vector<int> vec1{1, 2, 3};
      std::vector<int> vec2{};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_union_both_empty)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{};
      std::vector<int> result{};

      auto [in1, in2, out] = etl::ranges::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

      CHECK(out == result.begin());
    }

    //*************************************************************************
    TEST(ranges_set_union_identical_ranges)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5};
      std::vector<int> vec2{1, 2, 3, 4, 5};
      std::vector<int> result(5);

      auto [in1, in2, out] = etl::ranges::set_union(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3, 4, 5};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_union_disjoint_ranges)
    {
      std::vector<int> vec1{1, 3, 5};
      std::vector<int> vec2{2, 4, 6};
      std::vector<int> result(6);

      auto [in1, in2, out] = etl::ranges::set_union(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3, 4, 5, 6};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_union_with_duplicates)
    {
      std::vector<int> vec1{1, 2, 2, 3};
      std::vector<int> vec2{2, 2, 2, 4};
      std::vector<int> result(7);

      auto [in1, in2, out] = etl::ranges::set_union(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> std_result(7);
      auto             std_out = std::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
      std_result.erase(std_out, std_result.end());

      CHECK_EQUAL(std_result.size(), result.size());
      for (size_t i = 0; i < std_result.size(); ++i)
      {
        CHECK_EQUAL(std_result[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_union_with_custom_comparator)
    {
      std::vector<int> vec1{7, 5, 3, 1};
      std::vector<int> vec2{6, 4, 3, 2};
      std::vector<int> result(8);

      auto [in1, in2, out] = etl::ranges::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::greater<int>{});
      result.erase(out, result.end());

      std::vector<int> expected{7, 6, 5, 4, 3, 2, 1};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_union_with_projection)
    {
      struct Item
      {
        int key;
        int value;
      };

      std::vector<Item> vec1{{1, 10}, {3, 30}, {5, 50}};
      std::vector<Item> vec2{{2, 20}, {3, 99}, {4, 40}};
      std::vector<Item> result(5);

      auto [in1, in2, out] = etl::ranges::set_union(
        vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::ranges::less{}, [](const Item& item) { return item.key; },
        [](const Item& item) { return item.key; });
      result.erase(out, result.end());

      // Should contain keys: 1, 2, 3, 4, 5
      CHECK_EQUAL(5u, result.size());
      CHECK_EQUAL(1, result[0].key);
      CHECK_EQUAL(2, result[1].key);
      CHECK_EQUAL(3, result[2].key);
      CHECK_EQUAL(30, result[2].value); // From first range when equal
      CHECK_EQUAL(4, result[3].key);
      CHECK_EQUAL(5, result[4].key);
    }

    //*************************************************************************
    TEST(ranges_set_union_matches_std)
    {
      std::vector<int>              vec1{1, 2, 2, 3, 3, 3, 5, 5, 7, 9};
      std::vector<std::vector<int>> test_cases = {
        {1, 3, 5}, {2, 2, 3}, {1, 2, 3, 4}, {1, 9}, {}, {1, 2, 2, 3, 3, 3, 5, 5, 7, 9}, {3, 3, 3, 3}, {0}, {10}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
      };

      for (const auto& vec2 : test_cases)
      {
        std::vector<int> std_result(vec1.size() + vec2.size());
        auto             std_out = std::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
        std_result.erase(std_out, std_result.end());

        std::vector<int> etl_result(vec1.size() + vec2.size());
        auto [in1, in2, out] = etl::ranges::set_union(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), etl_result.begin());
        etl_result.erase(out, etl_result.end());

        CHECK_EQUAL(std_result.size(), etl_result.size());
        for (size_t i = 0; i < std_result.size(); ++i)
        {
          CHECK_EQUAL(std_result[i], etl_result[i]);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_set_intersection_iterator_basic)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 5, 7, 8};
      std::vector<int> result(5);

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{2, 5, 7};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
      CHECK(in1 == vec1.end());
    }

    //*************************************************************************
    TEST(ranges_set_intersection_range_basic)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 5, 7, 8};
      std::vector<int> result(5);

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{2, 5, 7};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_intersection_empty_first)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{1, 2, 3};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      CHECK_EQUAL(0u, result.size());
    }

    //*************************************************************************
    TEST(ranges_set_intersection_empty_second)
    {
      std::vector<int> vec1{1, 2, 3};
      std::vector<int> vec2{};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      CHECK_EQUAL(0u, result.size());
    }

    //*************************************************************************
    TEST(ranges_set_intersection_both_empty)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{};
      std::vector<int> result{};

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

      CHECK(out == result.begin());
    }

    //*************************************************************************
    TEST(ranges_set_intersection_identical_ranges)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5};
      std::vector<int> vec2{1, 2, 3, 4, 5};
      std::vector<int> result(5);

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3, 4, 5};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_intersection_disjoint_ranges)
    {
      std::vector<int> vec1{1, 3, 5};
      std::vector<int> vec2{2, 4, 6};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1, vec2, result.begin());
      result.erase(out, result.end());

      CHECK_EQUAL(0u, result.size());
    }

    //*************************************************************************
    TEST(ranges_set_intersection_with_duplicates)
    {
      std::vector<int> vec1{1, 2, 2, 3};
      std::vector<int> vec2{2, 2, 2, 4};
      std::vector<int> result(4);

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> std_result(4);
      auto             std_out = std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
      std_result.erase(std_out, std_result.end());

      CHECK_EQUAL(std_result.size(), result.size());
      for (size_t i = 0; i < std_result.size(); ++i)
      {
        CHECK_EQUAL(std_result[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_intersection_with_custom_comparator)
    {
      std::vector<int> vec1{7, 5, 3, 1};
      std::vector<int> vec2{6, 5, 3, 2};
      std::vector<int> result(4);

      auto [in1, in2, out] = etl::ranges::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::greater<int>{});
      result.erase(out, result.end());

      std::vector<int> expected{5, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_intersection_with_projection)
    {
      struct Item
      {
        int key;
        int value;
      };

      std::vector<Item> vec1{{1, 10}, {3, 30}, {5, 50}};
      std::vector<Item> vec2{{2, 20}, {3, 99}, {4, 40}};
      std::vector<Item> result(3);

      auto [in1, in2, out] = etl::ranges::set_intersection(
        vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::ranges::less{}, [](const Item& item) { return item.key; },
        [](const Item& item) { return item.key; });
      result.erase(out, result.end());

      // Should contain only key 3 (from first range)
      CHECK_EQUAL(1u, result.size());
      CHECK_EQUAL(3, result[0].key);
      CHECK_EQUAL(30, result[0].value); // From first range when equal
    }

    //*************************************************************************
    TEST(ranges_set_intersection_matches_std)
    {
      std::vector<int>              vec1{1, 2, 2, 3, 3, 3, 5, 5, 7, 9};
      std::vector<std::vector<int>> test_cases = {
        {1, 3, 5}, {2, 2, 3}, {1, 2, 3, 4}, {1, 9}, {}, {1, 2, 2, 3, 3, 3, 5, 5, 7, 9}, {3, 3, 3, 3}, {0}, {10}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
      };

      for (const auto& vec2 : test_cases)
      {
        std::vector<int> std_result(vec1.size() + vec2.size());
        auto             std_out = std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
        std_result.erase(std_out, std_result.end());

        std::vector<int> etl_result(vec1.size() + vec2.size());
        auto [in1, in2, out] = etl::ranges::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), etl_result.begin());
        etl_result.erase(out, etl_result.end());

        CHECK_EQUAL(std_result.size(), etl_result.size());
        for (size_t i = 0; i < std_result.size(); ++i)
        {
          CHECK_EQUAL(std_result[i], etl_result[i]);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_set_difference_iterator_basic)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 5, 7, 8};
      std::vector<int> result(5);

      auto [in, out] = etl::ranges::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
      CHECK(in == vec1.end());
    }

    //*************************************************************************
    TEST(ranges_set_difference_range_basic)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 5, 7, 8};
      std::vector<int> result(5);

      auto [in, out] = etl::ranges::set_difference(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_difference_empty_first)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{1, 2, 3};
      std::vector<int> result(3);

      auto [in, out] = etl::ranges::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      CHECK_EQUAL(0u, result.size());
    }

    //*************************************************************************
    TEST(ranges_set_difference_empty_second)
    {
      std::vector<int> vec1{1, 2, 3};
      std::vector<int> vec2{};
      std::vector<int> result(3);

      auto [in, out] = etl::ranges::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_difference_both_empty)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{};
      std::vector<int> result{};

      auto [in, out] = etl::ranges::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

      CHECK(out == result.begin());
    }

    //*************************************************************************
    TEST(ranges_set_difference_identical_ranges)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5};
      std::vector<int> vec2{1, 2, 3, 4, 5};
      std::vector<int> result(5);

      auto [in, out] = etl::ranges::set_difference(vec1, vec2, result.begin());
      result.erase(out, result.end());

      CHECK_EQUAL(0u, result.size());
    }

    //*************************************************************************
    TEST(ranges_set_difference_disjoint_ranges)
    {
      std::vector<int> vec1{1, 3, 5};
      std::vector<int> vec2{2, 4, 6};
      std::vector<int> result(3);

      auto [in, out] = etl::ranges::set_difference(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 3, 5};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_difference_with_duplicates)
    {
      std::vector<int> vec1{1, 2, 2, 3};
      std::vector<int> vec2{2, 2, 2, 4};
      std::vector<int> result(4);

      auto [in, out] = etl::ranges::set_difference(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> std_result(4);
      auto             std_out = std::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
      std_result.erase(std_out, std_result.end());

      CHECK_EQUAL(std_result.size(), result.size());
      for (size_t i = 0; i < std_result.size(); ++i)
      {
        CHECK_EQUAL(std_result[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_difference_with_custom_comparator)
    {
      std::vector<int> vec1{7, 5, 3, 1};
      std::vector<int> vec2{6, 5, 3, 2};
      std::vector<int> result(4);

      auto [in, out] = etl::ranges::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::greater<int>{});
      result.erase(out, result.end());

      std::vector<int> expected{7, 1};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_difference_with_projection)
    {
      struct Item
      {
        int key;
        int value;
      };

      std::vector<Item> vec1{{1, 10}, {3, 30}, {5, 50}};
      std::vector<Item> vec2{{2, 20}, {3, 99}, {4, 40}};
      std::vector<Item> result(3);

      auto [in, out] = etl::ranges::set_difference(
        vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::ranges::less{}, [](const Item& item) { return item.key; },
        [](const Item& item) { return item.key; });
      result.erase(out, result.end());

      // Should contain keys: 1, 5 (elements in vec1 not in vec2)
      CHECK_EQUAL(2u, result.size());
      CHECK_EQUAL(1, result[0].key);
      CHECK_EQUAL(10, result[0].value);
      CHECK_EQUAL(5, result[1].key);
      CHECK_EQUAL(50, result[1].value);
    }

    //*************************************************************************
    TEST(ranges_set_difference_matches_std)
    {
      std::vector<int>              vec1{1, 2, 2, 3, 3, 3, 5, 5, 7, 9};
      std::vector<std::vector<int>> test_cases = {
        {1, 3, 5}, {2, 2, 3}, {1, 2, 3, 4}, {1, 9}, {}, {1, 2, 2, 3, 3, 3, 5, 5, 7, 9}, {3, 3, 3, 3}, {0}, {10}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
      };

      for (const auto& vec2 : test_cases)
      {
        std::vector<int> std_result(vec1.size() + vec2.size());
        auto             std_out = std::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
        std_result.erase(std_out, std_result.end());

        std::vector<int> etl_result(vec1.size() + vec2.size());
        auto [in, out] = etl::ranges::set_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), etl_result.begin());
        etl_result.erase(out, etl_result.end());

        CHECK_EQUAL(std_result.size(), etl_result.size());
        for (size_t i = 0; i < std_result.size(); ++i)
        {
          CHECK_EQUAL(std_result[i], etl_result[i]);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_iterator_basic)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 5, 7, 8};
      std::vector<int> result(10);

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 3, 4, 8};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
      CHECK(in1 == vec1.end());
      CHECK(in2 == vec2.end());
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_range_basic)
    {
      std::vector<int> vec1{1, 2, 3, 5, 7};
      std::vector<int> vec2{2, 4, 5, 7, 8};
      std::vector<int> result(10);

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 3, 4, 8};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_empty_first)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{1, 2, 3};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_empty_second)
    {
      std::vector<int> vec1{1, 2, 3};
      std::vector<int> vec2{};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_both_empty)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{};
      std::vector<int> result{};

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

      CHECK(out == result.begin());
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_identical_ranges)
    {
      std::vector<int> vec1{1, 2, 3, 4, 5};
      std::vector<int> vec2{1, 2, 3, 4, 5};
      std::vector<int> result(5);

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1, vec2, result.begin());
      result.erase(out, result.end());

      CHECK_EQUAL(0u, result.size());
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_disjoint_ranges)
    {
      std::vector<int> vec1{1, 3, 5};
      std::vector<int> vec2{2, 4, 6};
      std::vector<int> result(6);

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> expected{1, 2, 3, 4, 5, 6};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_with_duplicates)
    {
      std::vector<int> vec1{1, 2, 2, 3};
      std::vector<int> vec2{2, 2, 2, 4};
      std::vector<int> result(8);

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1, vec2, result.begin());
      result.erase(out, result.end());

      std::vector<int> std_result(8);
      auto             std_out = std::set_symmetric_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
      std_result.erase(std_out, std_result.end());

      CHECK_EQUAL(std_result.size(), result.size());
      for (size_t i = 0; i < std_result.size(); ++i)
      {
        CHECK_EQUAL(std_result[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_with_custom_comparator)
    {
      std::vector<int> vec1{7, 5, 3, 1};
      std::vector<int> vec2{6, 5, 3, 2};
      std::vector<int> result(8);

      auto [in1, in2, out] =
        etl::ranges::set_symmetric_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::greater<int>{});
      result.erase(out, result.end());

      std::vector<int> expected{7, 6, 2, 1};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_with_projection)
    {
      struct Item
      {
        int key;
        int value;
      };

      std::vector<Item> vec1{{1, 10}, {3, 30}, {5, 50}};
      std::vector<Item> vec2{{2, 20}, {3, 99}, {4, 40}};
      std::vector<Item> result(6);

      auto [in1, in2, out] = etl::ranges::set_symmetric_difference(
        vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::ranges::less{}, [](const Item& item) { return item.key; },
        [](const Item& item) { return item.key; });
      result.erase(out, result.end());

      // Should contain keys: 1, 2, 4, 5 (elements in either but not both)
      CHECK_EQUAL(4u, result.size());
      CHECK_EQUAL(1, result[0].key);
      CHECK_EQUAL(10, result[0].value);
      CHECK_EQUAL(2, result[1].key);
      CHECK_EQUAL(20, result[1].value);
      CHECK_EQUAL(4, result[2].key);
      CHECK_EQUAL(40, result[2].value);
      CHECK_EQUAL(5, result[3].key);
      CHECK_EQUAL(50, result[3].value);
    }

    //*************************************************************************
    TEST(ranges_set_symmetric_difference_matches_std)
    {
      std::vector<int>              vec1{1, 2, 2, 3, 3, 3, 5, 5, 7, 9};
      std::vector<std::vector<int>> test_cases = {
        {1, 3, 5}, {2, 2, 3}, {1, 2, 3, 4}, {1, 9}, {}, {1, 2, 2, 3, 3, 3, 5, 5, 7, 9}, {3, 3, 3, 3}, {0}, {10}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
      };

      for (const auto& vec2 : test_cases)
      {
        std::vector<int> std_result(vec1.size() + vec2.size());
        auto             std_out = std::set_symmetric_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
        std_result.erase(std_out, std_result.end());

        std::vector<int> etl_result(vec1.size() + vec2.size());
        auto [in1, in2, out] = etl::ranges::set_symmetric_difference(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), etl_result.begin());
        etl_result.erase(out, etl_result.end());

        CHECK_EQUAL(std_result.size(), etl_result.size());
        for (size_t i = 0; i < std_result.size(); ++i)
        {
          CHECK_EQUAL(std_result[i], etl_result[i]);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_merge_iterator_basic)
    {
      std::vector<int> vec1{1, 3, 5, 7};
      std::vector<int> vec2{2, 4, 6, 8};
      std::vector<int> result(8);

      auto [in1, in2, out] = etl::ranges::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

      std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
      CHECK(in1 == vec1.end());
      CHECK(in2 == vec2.end());
      CHECK(out == result.end());
    }

    //*************************************************************************
    TEST(ranges_merge_range_basic)
    {
      std::vector<int> vec1{1, 3, 5, 7};
      std::vector<int> vec2{2, 4, 6, 8};
      std::vector<int> result(8);

      auto [in1, in2, out] = etl::ranges::merge(vec1, vec2, result.begin());
      (void)in1;
      (void)in2;

      std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8};
      CHECK_EQUAL(expected.size(), size_t(out - result.begin()));
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_merge_empty_first)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{1, 2, 3};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

      std::vector<int> expected{1, 2, 3};
      CHECK_EQUAL(expected.size(), size_t(out - result.begin()));
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_merge_empty_second)
    {
      std::vector<int> vec1{1, 2, 3};
      std::vector<int> vec2{};
      std::vector<int> result(3);

      auto [in1, in2, out] = etl::ranges::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

      std::vector<int> expected{1, 2, 3};
      CHECK_EQUAL(expected.size(), size_t(out - result.begin()));
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_merge_both_empty)
    {
      std::vector<int> vec1{};
      std::vector<int> vec2{};
      std::vector<int> result{};

      auto [in1, in2, out] = etl::ranges::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin());

      CHECK(out == result.begin());
    }

    //*************************************************************************
    TEST(ranges_merge_identical_ranges)
    {
      std::vector<int> vec1{1, 2, 3};
      std::vector<int> vec2{1, 2, 3};
      std::vector<int> result(6);

      auto [in1, in2, out] = etl::ranges::merge(vec1, vec2, result.begin());
      (void)in1;
      (void)in2;
      result.erase(out, result.end());

      std::vector<int> expected{1, 1, 2, 2, 3, 3};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_merge_with_duplicates)
    {
      std::vector<int> vec1{1, 2, 2, 3};
      std::vector<int> vec2{2, 2, 2, 4};
      std::vector<int> result(8);

      auto [in1, in2, out] = etl::ranges::merge(vec1, vec2, result.begin());
      (void)in1;
      (void)in2;
      result.erase(out, result.end());

      std::vector<int> std_result(8);
      auto             std_out = std::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
      std_result.erase(std_out, std_result.end());

      CHECK_EQUAL(std_result.size(), result.size());
      for (size_t i = 0; i < std_result.size(); ++i)
      {
        CHECK_EQUAL(std_result[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_merge_with_custom_comparator)
    {
      std::vector<int> vec1{7, 5, 3, 1};
      std::vector<int> vec2{6, 4, 2};
      std::vector<int> result(7);

      auto [in1, in2, out] = etl::ranges::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::greater<int>{});
      result.erase(out, result.end());

      std::vector<int> expected{7, 6, 5, 4, 3, 2, 1};
      CHECK_EQUAL(expected.size(), result.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], result[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_merge_with_projection)
    {
      struct Item
      {
        int key;
        int value;
      };

      std::vector<Item> vec1{{1, 10}, {3, 30}, {5, 50}};
      std::vector<Item> vec2{{2, 20}, {3, 99}, {4, 40}};
      std::vector<Item> result(6);

      auto [in1, in2, out] = etl::ranges::merge(
        vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), etl::ranges::less{}, [](const Item& item) { return item.key; },
        [](const Item& item) { return item.key; });
      result.erase(out, result.end());

      // Merge keeps all elements: 1, 2, 3(from vec1), 3(from vec2), 4, 5
      CHECK_EQUAL(6u, result.size());
      CHECK_EQUAL(1, result[0].key);
      CHECK_EQUAL(10, result[0].value);
      CHECK_EQUAL(2, result[1].key);
      CHECK_EQUAL(20, result[1].value);
      CHECK_EQUAL(3, result[2].key);
      CHECK_EQUAL(30, result[2].value); // From first range (stable)
      CHECK_EQUAL(3, result[3].key);
      CHECK_EQUAL(99, result[3].value); // From second range
      CHECK_EQUAL(4, result[4].key);
      CHECK_EQUAL(40, result[4].value);
      CHECK_EQUAL(5, result[5].key);
      CHECK_EQUAL(50, result[5].value);
    }

    //*************************************************************************
    TEST(ranges_merge_matches_std)
    {
      std::vector<int>              vec1{1, 2, 2, 3, 3, 3, 5, 5, 7, 9};
      std::vector<std::vector<int>> test_cases = {
        {1, 3, 5}, {2, 2, 3}, {1, 2, 3, 4}, {1, 9}, {}, {1, 2, 2, 3, 3, 3, 5, 5, 7, 9}, {3, 3, 3, 3}, {0}, {10}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
      };

      for (const auto& vec2 : test_cases)
      {
        std::vector<int> std_result(vec1.size() + vec2.size());
        auto             std_out = std::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), std_result.begin());
        std_result.erase(std_out, std_result.end());

        std::vector<int> etl_result(vec1.size() + vec2.size());
        auto [in1, in2, out] = etl::ranges::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), etl_result.begin());
        etl_result.erase(out, etl_result.end());

        CHECK_EQUAL(std_result.size(), etl_result.size());
        for (size_t i = 0; i < std_result.size(); ++i)
        {
          CHECK_EQUAL(std_result[i], etl_result[i]);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_iterator_basic)
    {
      std::vector<int> vec{1, 3, 5, 7, 2, 4, 6, 8};
      auto             middle = vec.begin() + 4;

      auto result = etl::ranges::inplace_merge(vec.begin(), middle, vec.end());

      std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8};
      CHECK_EQUAL(expected.size(), vec.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], vec[i]);
      }
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_range_basic)
    {
      std::vector<int> vec{1, 3, 5, 7, 2, 4, 6, 8};
      auto             middle = vec.begin() + 4;

      auto result = etl::ranges::inplace_merge(vec, middle);

      std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8};
      CHECK_EQUAL(expected.size(), vec.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], vec[i]);
      }
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_empty_first_half)
    {
      std::vector<int> vec{1, 2, 3};
      auto             middle = vec.begin(); // empty first half

      auto result = etl::ranges::inplace_merge(vec.begin(), middle, vec.end());

      std::vector<int> expected{1, 2, 3};
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], vec[i]);
      }
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_empty_second_half)
    {
      std::vector<int> vec{1, 2, 3};
      auto             middle = vec.end(); // empty second half

      auto result = etl::ranges::inplace_merge(vec.begin(), middle, vec.end());

      std::vector<int> expected{1, 2, 3};
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], vec[i]);
      }
      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_single_elements)
    {
      std::vector<int> vec{5, 2};
      auto             middle = vec.begin() + 1;

      etl::ranges::inplace_merge(vec.begin(), middle, vec.end());

      CHECK_EQUAL(2, vec[0]);
      CHECK_EQUAL(5, vec[1]);
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_already_sorted)
    {
      std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8};
      auto             middle = vec.begin() + 4;

      etl::ranges::inplace_merge(vec.begin(), middle, vec.end());

      std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8};
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], vec[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_with_duplicates)
    {
      std::vector<int> vec{1, 2, 2, 3, 2, 2, 2, 4};
      auto             middle = vec.begin() + 4;

      etl::ranges::inplace_merge(vec.begin(), middle, vec.end());

      std::vector<int> expected{1, 2, 2, 2, 2, 2, 3, 4};
      CHECK_EQUAL(expected.size(), vec.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], vec[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_with_custom_comparator)
    {
      std::vector<int> vec{7, 5, 3, 1, 8, 6, 4, 2};
      auto             middle = vec.begin() + 4;

      etl::ranges::inplace_merge(vec.begin(), middle, vec.end(), etl::greater<int>{});

      std::vector<int> expected{8, 7, 6, 5, 4, 3, 2, 1};
      CHECK_EQUAL(expected.size(), vec.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], vec[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_with_projection)
    {
      struct Item
      {
        int key;
        int value;
      };

      std::vector<Item> vec{{1, 10}, {3, 30}, {5, 50}, {2, 20}, {4, 40}};
      auto              middle = vec.begin() + 3;

      etl::ranges::inplace_merge(vec.begin(), middle, vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(10, vec[0].value);
      CHECK_EQUAL(2, vec[1].key);
      CHECK_EQUAL(20, vec[1].value);
      CHECK_EQUAL(3, vec[2].key);
      CHECK_EQUAL(30, vec[2].value);
      CHECK_EQUAL(4, vec[3].key);
      CHECK_EQUAL(40, vec[3].value);
      CHECK_EQUAL(5, vec[4].key);
      CHECK_EQUAL(50, vec[4].value);
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_matches_std)
    {
      std::vector<int>              base1{1, 2, 2, 3, 3, 3, 5, 5, 7, 9};
      std::vector<std::vector<int>> test_cases = {
        {1, 3, 5}, {2, 2, 3}, {1, 2, 3, 4}, {1, 9}, {}, {1, 2, 2, 3, 3, 3, 5, 5, 7, 9}, {3, 3, 3, 3}, {0}, {10}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
      };

      for (const auto& second : test_cases)
      {
        // Build a combined vector with two sorted halves
        std::vector<int> std_vec;
        std_vec.insert(std_vec.end(), base1.begin(), base1.end());
        std_vec.insert(std_vec.end(), second.begin(), second.end());

        std::vector<int> etl_vec = std_vec;

        auto std_middle = std_vec.begin() + static_cast<std::ptrdiff_t>(base1.size());
        auto etl_middle = etl_vec.begin() + static_cast<std::ptrdiff_t>(base1.size());

        std::inplace_merge(std_vec.begin(), std_middle, std_vec.end());
        etl::ranges::inplace_merge(etl_vec.begin(), etl_middle, etl_vec.end());

        CHECK_EQUAL(std_vec.size(), etl_vec.size());
        for (size_t i = 0; i < std_vec.size(); ++i)
        {
          CHECK_EQUAL(std_vec[i], etl_vec[i]);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_single_element_halves)
    {
      std::vector<int> vec{3, 1};
      auto             middle = vec.begin() + 1;

      etl::ranges::inplace_merge(vec.begin(), middle, vec.end());

      CHECK_EQUAL(1, vec[0]);
      CHECK_EQUAL(3, vec[1]);
    }

    //*************************************************************************
    TEST(ranges_inplace_merge_unequal_halves)
    {
      std::vector<int> vec{1, 5, 9, 2, 3, 4, 6, 7, 8, 10};
      auto             middle = vec.begin() + 3;

      etl::ranges::inplace_merge(vec.begin(), middle, vec.end());

      std::vector<int> expected{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      CHECK_EQUAL(expected.size(), vec.size());
      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], vec[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_make_heap_iterator)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};

      auto result = etl::ranges::make_heap(vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(std::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_make_heap_range)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};

      auto result = etl::ranges::make_heap(vec);

      CHECK(result == vec.end());
      CHECK(std::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_make_heap_with_comparator_iterator)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};

      auto result = etl::ranges::make_heap(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(std::is_heap(vec.begin(), vec.end(), etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_make_heap_with_comparator_range)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};

      auto result = etl::ranges::make_heap(vec, etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(std::is_heap(vec.begin(), vec.end(), etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_make_heap_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {5, 50}, {2, 20}, {4, 40}};

      etl::ranges::make_heap(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      // Verify max-heap property on keys
      for (size_t i = 0; i < vec.size(); ++i)
      {
        size_t left  = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < vec.size())
        {
          CHECK(vec[i].key >= vec[left].key);
        }
        if (right < vec.size())
        {
          CHECK(vec[i].key >= vec[right].key);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_make_heap_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {1, 10}, {5, 50}, {2, 20}, {4, 40}};

      etl::ranges::make_heap(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      // Verify max-heap property on keys
      for (size_t i = 0; i < vec.size(); ++i)
      {
        size_t left  = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < vec.size())
        {
          CHECK(vec[i].key >= vec[left].key);
        }
        if (right < vec.size())
        {
          CHECK(vec[i].key >= vec[right].key);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_make_heap_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::make_heap(vec.begin(), vec.end());

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_make_heap_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::make_heap(vec);

      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_make_heap_already_heap)
    {
      std::vector<int> vec{5, 4, 3, 2, 1};

      etl::ranges::make_heap(vec);

      CHECK(std::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_make_heap_duplicates)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

      etl::ranges::make_heap(vec);

      CHECK(std::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_make_heap_matches_std)
    {
      std::vector<int> data_std{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      std::vector<int> data_etl = data_std;

      std::make_heap(data_std.begin(), data_std.end());
      etl::ranges::make_heap(data_etl);

      CHECK(std::is_heap(data_etl.begin(), data_etl.end()));
    }

    //*************************************************************************
    TEST(ranges_push_heap_iterator)
    {
      std::vector<int> vec{5, 3, 1};
      std::make_heap(vec.begin(), vec.end());
      vec.push_back(10);

      auto result = etl::ranges::push_heap(vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(std::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_push_heap_range)
    {
      std::vector<int> vec{5, 3, 1};
      std::make_heap(vec.begin(), vec.end());
      vec.push_back(4);

      auto result = etl::ranges::push_heap(vec);

      CHECK(result == vec.end());
      CHECK(std::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_push_heap_with_comparator_iterator)
    {
      std::vector<int> vec{1, 3, 5};
      std::make_heap(vec.begin(), vec.end(), etl::greater<int>{});
      vec.push_back(0);

      auto result = etl::ranges::push_heap(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(std::is_heap(vec.begin(), vec.end(), etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_push_heap_with_comparator_range)
    {
      std::vector<int> vec{1, 3, 5};
      std::make_heap(vec.begin(), vec.end(), etl::greater<int>{});
      vec.push_back(2);

      auto result = etl::ranges::push_heap(vec, etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(std::is_heap(vec.begin(), vec.end(), etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_push_heap_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{5, 50}, {3, 30}, {1, 10}};
      // Make it a heap by key first
      etl::ranges::make_heap(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });
      vec.push_back({10, 100});

      etl::ranges::push_heap(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      // Verify max-heap property on keys
      for (size_t i = 0; i < vec.size(); ++i)
      {
        size_t left  = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < vec.size())
        {
          CHECK(vec[i].key >= vec[left].key);
        }
        if (right < vec.size())
        {
          CHECK(vec[i].key >= vec[right].key);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_push_heap_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{5, 50}, {3, 30}, {1, 10}};
      etl::ranges::make_heap(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });
      vec.push_back({4, 40});

      etl::ranges::push_heap(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      // Verify max-heap property on keys
      for (size_t i = 0; i < vec.size(); ++i)
      {
        size_t left  = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < vec.size())
        {
          CHECK(vec[i].key >= vec[left].key);
        }
        if (right < vec.size())
        {
          CHECK(vec[i].key >= vec[right].key);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_push_heap_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::push_heap(vec.begin(), vec.end());

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_push_heap_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::push_heap(vec);

      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_push_heap_multiple_pushes)
    {
      std::vector<int> vec;
      int              values[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

      for (int v : values)
      {
        vec.push_back(v);
        etl::ranges::push_heap(vec);
      }

      CHECK(std::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_push_heap_matches_std)
    {
      std::vector<int> data_std;
      std::vector<int> data_etl;
      int              values[] = {9, 3, 7, 1, 5, 8, 2, 6, 4, 10};

      for (int v : values)
      {
        data_std.push_back(v);
        std::push_heap(data_std.begin(), data_std.end());

        data_etl.push_back(v);
        etl::ranges::push_heap(data_etl);
      }

      CHECK(std::is_heap(data_etl.begin(), data_etl.end()));

      for (size_t i = 0; i < data_std.size(); ++i)
      {
        CHECK_EQUAL(data_std[i], data_etl[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_pop_heap_iterator)
    {
      std::vector<int> vec{9, 5, 7, 1, 3};
      std::make_heap(vec.begin(), vec.end());

      auto result = etl::ranges::pop_heap(vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK_EQUAL(9, vec.back());
      CHECK(std::is_heap(vec.begin(), vec.end() - 1));
    }

    //*************************************************************************
    TEST(ranges_pop_heap_range)
    {
      std::vector<int> vec{9, 5, 7, 1, 3};
      std::make_heap(vec.begin(), vec.end());

      auto result = etl::ranges::pop_heap(vec);

      CHECK(result == vec.end());
      CHECK_EQUAL(9, vec.back());
      CHECK(std::is_heap(vec.begin(), vec.end() - 1));
    }

    //*************************************************************************
    TEST(ranges_pop_heap_with_comparator_iterator)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};
      std::make_heap(vec.begin(), vec.end(), etl::greater<int>{});

      auto result = etl::ranges::pop_heap(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK_EQUAL(1, vec.back());
      CHECK(std::is_heap(vec.begin(), vec.end() - 1, etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_pop_heap_with_comparator_range)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};
      std::make_heap(vec.begin(), vec.end(), etl::greater<int>{});

      auto result = etl::ranges::pop_heap(vec, etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK_EQUAL(1, vec.back());
      CHECK(std::is_heap(vec.begin(), vec.end() - 1, etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_pop_heap_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{9, 90}, {5, 50}, {7, 70}, {1, 10}, {3, 30}};
      etl::ranges::make_heap(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      etl::ranges::pop_heap(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(9, vec.back().key);

      // Verify max-heap property on keys for remaining elements
      for (size_t i = 0; i < vec.size() - 1; ++i)
      {
        size_t left  = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < vec.size() - 1)
        {
          CHECK(vec[i].key >= vec[left].key);
        }
        if (right < vec.size() - 1)
        {
          CHECK(vec[i].key >= vec[right].key);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_pop_heap_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{9, 90}, {5, 50}, {7, 70}, {1, 10}, {3, 30}};
      etl::ranges::make_heap(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      etl::ranges::pop_heap(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK_EQUAL(9, vec.back().key);

      // Verify max-heap property on keys for remaining elements
      for (size_t i = 0; i < vec.size() - 1; ++i)
      {
        size_t left  = 2 * i + 1;
        size_t right = 2 * i + 2;

        if (left < vec.size() - 1)
        {
          CHECK(vec[i].key >= vec[left].key);
        }
        if (right < vec.size() - 1)
        {
          CHECK(vec[i].key >= vec[right].key);
        }
      }
    }

    //*************************************************************************
    TEST(ranges_pop_heap_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::pop_heap(vec.begin(), vec.end());

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_pop_heap_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::pop_heap(vec);

      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_pop_heap_multiple_pops)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
      std::make_heap(vec.begin(), vec.end());

      std::vector<int> sorted;
      auto             end = vec.end();

      while (end != vec.begin())
      {
        etl::ranges::pop_heap(vec.begin(), end);
        --end;
        sorted.push_back(*end);
      }

      // Should produce descending order
      for (size_t i = 1; i < sorted.size(); ++i)
      {
        CHECK(sorted[i - 1] >= sorted[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_pop_heap_matches_std)
    {
      std::vector<int> data_std{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      std::vector<int> data_etl = data_std;

      std::make_heap(data_std.begin(), data_std.end());
      std::make_heap(data_etl.begin(), data_etl.end());

      std::pop_heap(data_std.begin(), data_std.end());
      etl::ranges::pop_heap(data_etl);

      for (size_t i = 0; i < data_std.size(); ++i)
      {
        CHECK_EQUAL(data_std[i], data_etl[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_sort_heap_iterator)
    {
      std::vector<int> vec{9, 5, 7, 1, 3};
      std::make_heap(vec.begin(), vec.end());

      auto result = etl::ranges::sort_heap(vec.begin(), vec.end());

      CHECK(result == vec.end());
      CHECK(std::is_sorted(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_sort_heap_range)
    {
      std::vector<int> vec{9, 5, 7, 1, 3};
      std::make_heap(vec.begin(), vec.end());

      auto result = etl::ranges::sort_heap(vec);

      CHECK(result == vec.end());
      CHECK(std::is_sorted(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_sort_heap_with_comparator_iterator)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};
      std::make_heap(vec.begin(), vec.end(), etl::greater<int>{});

      auto result = etl::ranges::sort_heap(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(std::is_sorted(vec.begin(), vec.end(), etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_sort_heap_with_comparator_range)
    {
      std::vector<int> vec{1, 3, 5, 7, 9};
      std::make_heap(vec.begin(), vec.end(), etl::greater<int>{});

      auto result = etl::ranges::sort_heap(vec, etl::greater<int>{});

      CHECK(result == vec.end());
      CHECK(std::is_sorted(vec.begin(), vec.end(), etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_sort_heap_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{9, 90}, {5, 50}, {7, 70}, {1, 10}, {3, 30}};
      etl::ranges::make_heap(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      etl::ranges::sort_heap(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      for (size_t i = 1; i < vec.size(); ++i)
      {
        CHECK(vec[i - 1].key <= vec[i].key);
      }
    }

    //*************************************************************************
    TEST(ranges_sort_heap_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{9, 90}, {5, 50}, {7, 70}, {1, 10}, {3, 30}};
      etl::ranges::make_heap(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      etl::ranges::sort_heap(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      for (size_t i = 1; i < vec.size(); ++i)
      {
        CHECK(vec[i - 1].key <= vec[i].key);
      }
    }

    //*************************************************************************
    TEST(ranges_sort_heap_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::sort_heap(vec.begin(), vec.end());

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_sort_heap_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::sort_heap(vec);

      CHECK(result == vec.end());
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_sort_heap_matches_std)
    {
      std::vector<int> data_std{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      std::vector<int> data_etl = data_std;

      std::make_heap(data_std.begin(), data_std.end());
      std::make_heap(data_etl.begin(), data_etl.end());

      std::sort_heap(data_std.begin(), data_std.end());
      etl::ranges::sort_heap(data_etl);

      for (size_t i = 0; i < data_std.size(); ++i)
      {
        CHECK_EQUAL(data_std[i], data_etl[i]);
      }
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_iterator)
    {
      std::vector<int> vec{9, 5, 7, 1, 3, 2, 6};

      auto result   = etl::ranges::is_heap_until(vec.begin(), vec.end());
      auto expected = std::is_heap_until(vec.begin(), vec.end());

      CHECK(result == expected);
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_range)
    {
      std::vector<int> vec{9, 5, 7, 1, 3, 2, 6};

      auto result   = etl::ranges::is_heap_until(vec);
      auto expected = std::is_heap_until(vec.begin(), vec.end());

      CHECK(result == expected);
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_not_heap)
    {
      std::vector<int> vec{1, 5, 3, 7, 2};

      auto result   = etl::ranges::is_heap_until(vec.begin(), vec.end());
      auto expected = std::is_heap_until(vec.begin(), vec.end());

      CHECK(result == expected);
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_with_comparator_iterator)
    {
      std::vector<int> vec{1, 3, 2, 5, 7, 4, 6};

      auto result   = etl::ranges::is_heap_until(vec.begin(), vec.end(), etl::greater<int>{});
      auto expected = std::is_heap_until(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result == expected);
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_with_comparator_range)
    {
      std::vector<int> vec{1, 3, 2, 5, 7, 4, 6};

      auto result   = etl::ranges::is_heap_until(vec, etl::greater<int>{});
      auto expected = std::is_heap_until(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result == expected);
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{9, 90}, {5, 50}, {7, 70}, {1, 10}, {3, 30}};

      auto result = etl::ranges::is_heap_until(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{9, 90}, {5, 50}, {7, 70}, {1, 10}, {3, 30}};

      auto result = etl::ranges::is_heap_until(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::is_heap_until(vec.begin(), vec.end());

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::is_heap_until(vec);

      CHECK(result == vec.end());
    }

    //*************************************************************************
    TEST(ranges_is_heap_until_matches_std)
    {
      std::vector<int> vec{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};

      auto result_etl = etl::ranges::is_heap_until(vec);
      auto result_std = std::is_heap_until(vec.begin(), vec.end());

      CHECK(result_etl == result_std);
    }

    //*************************************************************************
    TEST(ranges_is_heap_iterator)
    {
      std::vector<int> vec{9, 5, 7, 1, 3, 2, 6};

      CHECK(etl::ranges::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_is_heap_range)
    {
      std::vector<int> vec{9, 5, 7, 1, 3, 2, 6};

      CHECK(etl::ranges::is_heap(vec));
    }

    //*************************************************************************
    TEST(ranges_is_heap_not_heap_iterator)
    {
      std::vector<int> vec{1, 5, 3, 7, 2};

      CHECK(!etl::ranges::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_is_heap_not_heap_range)
    {
      std::vector<int> vec{1, 5, 3, 7, 2};

      CHECK(!etl::ranges::is_heap(vec));
    }

    //*************************************************************************
    TEST(ranges_is_heap_with_comparator_iterator)
    {
      std::vector<int> vec{1, 3, 2, 5, 7, 4, 6};

      CHECK(etl::ranges::is_heap(vec.begin(), vec.end(), etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_is_heap_with_comparator_range)
    {
      std::vector<int> vec{1, 3, 2, 5, 7, 4, 6};

      CHECK(etl::ranges::is_heap(vec, etl::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_is_heap_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{9, 90}, {5, 50}, {7, 70}, {1, 10}, {3, 30}};

      CHECK(etl::ranges::is_heap(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; }));
    }

    //*************************************************************************
    TEST(ranges_is_heap_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{9, 90}, {5, 50}, {7, 70}, {1, 10}, {3, 30}};

      CHECK(etl::ranges::is_heap(vec, etl::ranges::less{}, [](const Item& item) { return item.key; }));
    }

    //*************************************************************************
    TEST(ranges_is_heap_empty)
    {
      std::vector<int> vec{};

      CHECK(etl::ranges::is_heap(vec.begin(), vec.end()));
    }

    //*************************************************************************
    TEST(ranges_is_heap_single_element)
    {
      std::vector<int> vec{42};

      CHECK(etl::ranges::is_heap(vec));
    }

    //*************************************************************************
    TEST(ranges_is_heap_after_make_heap)
    {
      std::vector<int> vec{5, 3, 1, 4, 2};
      etl::ranges::make_heap(vec);

      CHECK(etl::ranges::is_heap(vec));
    }

    //*************************************************************************
    TEST(ranges_is_heap_matches_std)
    {
      std::vector<int> data1{9, 3, 7, 1, 5, 8, 2, 6, 4, 10};
      CHECK_EQUAL(std::is_heap(data1.begin(), data1.end()), etl::ranges::is_heap(data1));

      std::vector<int> data2{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
      CHECK_EQUAL(std::is_heap(data2.begin(), data2.end()), etl::ranges::is_heap(data2));

      std::vector<int> data3{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      CHECK_EQUAL(std::is_heap(data3.begin(), data3.end()), etl::ranges::is_heap(data3));
    }

    //*************************************************************************
    TEST(ranges_min_two_values)
    {
      CHECK_EQUAL(3, etl::ranges::min(3, 5));
      CHECK_EQUAL(3, etl::ranges::min(5, 3));
      CHECK_EQUAL(7, etl::ranges::min(7, 7));
    }

    //*************************************************************************
    TEST(ranges_min_two_values_with_comparator)
    {
      CHECK_EQUAL(5, etl::ranges::min(3, 5, std::greater<int>{}));
      CHECK_EQUAL(5, etl::ranges::min(5, 3, std::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_min_two_values_with_projection)
    {
      auto abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      CHECK_EQUAL(3, etl::ranges::min(3, -5, etl::ranges::less{}, abs_proj));
      CHECK_EQUAL(3, etl::ranges::min(-5, 3, etl::ranges::less{}, abs_proj));
    }

  #if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(ranges_min_initializer_list)
    {
      CHECK_EQUAL(1, etl::ranges::min({3, 1, 4, 1, 5, 9, 2, 6}));
      CHECK_EQUAL(42, etl::ranges::min({42}));
    }

    //*************************************************************************
    TEST(ranges_min_initializer_list_with_comparator)
    {
      CHECK_EQUAL(9, etl::ranges::min({3, 1, 4, 1, 5, 9, 2, 6}, std::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_min_initializer_list_with_projection)
    {
      auto negate = [](int v)
      {
        return -v;
      };
      CHECK_EQUAL(9, etl::ranges::min({3, 1, 4, 5, 9, 2, 6}, etl::ranges::less{}, negate));
    }
  #endif

    //*************************************************************************
    TEST(ranges_min_range)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      CHECK_EQUAL(1, etl::ranges::min(vec));
    }

    //*************************************************************************
    TEST(ranges_min_range_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      CHECK_EQUAL(9, etl::ranges::min(vec, std::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_min_range_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      CHECK_EQUAL(-1, etl::ranges::min(vec, etl::ranges::less{}, abs_proj));
    }

    //*************************************************************************
    TEST(ranges_min_single_element_range)
    {
      std::vector<int> vec{42};
      CHECK_EQUAL(42, etl::ranges::min(vec));
    }

    //*************************************************************************
    TEST(ranges_min_all_equal)
    {
      std::vector<int> vec{7, 7, 7, 7};
      CHECK_EQUAL(7, etl::ranges::min(vec));
    }

    //*************************************************************************
    TEST(ranges_min_negative_values)
    {
      std::vector<int> vec{-3, -1, -4, -1, -5};
      CHECK_EQUAL(-5, etl::ranges::min(vec));
    }

    //*************************************************************************
    TEST(ranges_min_matches_std)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
      CHECK_EQUAL(*std::min_element(vec.begin(), vec.end()), etl::ranges::min(vec));

      std::vector<int> vec2{-10, -20, -5, -15};
      CHECK_EQUAL(*std::min_element(vec2.begin(), vec2.end()), etl::ranges::min(vec2));
    }

    //*************************************************************************
    TEST(ranges_min_element_iterator_sentinel)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             it = etl::ranges::min_element(vec.begin(), vec.end());
      CHECK_EQUAL(1, *it);
    }

    //*************************************************************************
    TEST(ranges_min_element_iterator_sentinel_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             it = etl::ranges::min_element(vec.begin(), vec.end(), std::greater<int>{});
      CHECK_EQUAL(9, *it);
    }

    //*************************************************************************
    TEST(ranges_min_element_iterator_sentinel_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      auto it = etl::ranges::min_element(vec.begin(), vec.end(), etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(-1, *it);
    }

    //*************************************************************************
    TEST(ranges_min_element_range)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             it = etl::ranges::min_element(vec);
      CHECK_EQUAL(1, *it);
    }

    //*************************************************************************
    TEST(ranges_min_element_range_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             it = etl::ranges::min_element(vec, std::greater<int>{});
      CHECK_EQUAL(9, *it);
    }

    //*************************************************************************
    TEST(ranges_min_element_range_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      auto it = etl::ranges::min_element(vec, etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(-1, *it);
    }

    //*************************************************************************
    TEST(ranges_min_element_single_element)
    {
      std::vector<int> vec{42};
      auto             it = etl::ranges::min_element(vec);
      CHECK_EQUAL(42, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_min_element_empty_range)
    {
      std::vector<int> vec{};
      auto             it = etl::ranges::min_element(vec.begin(), vec.end());
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_min_element_all_equal)
    {
      std::vector<int> vec{7, 7, 7, 7};
      auto             it = etl::ranges::min_element(vec);
      CHECK_EQUAL(7, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_min_element_matches_std)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
      CHECK(std::min_element(vec.begin(), vec.end()) == etl::ranges::min_element(vec.begin(), vec.end()));

      std::vector<int> vec2{-10, -20, -5, -15};
      CHECK(std::min_element(vec2.begin(), vec2.end()) == etl::ranges::min_element(vec2));
    }

    //*************************************************************************
    TEST(ranges_max_two_values)
    {
      CHECK_EQUAL(5, etl::ranges::max(3, 5));
      CHECK_EQUAL(5, etl::ranges::max(5, 3));
      CHECK_EQUAL(7, etl::ranges::max(7, 7));
    }

    //*************************************************************************
    TEST(ranges_max_two_values_with_comparator)
    {
      CHECK_EQUAL(3, etl::ranges::max(3, 5, std::greater<int>{}));
      CHECK_EQUAL(3, etl::ranges::max(5, 3, std::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_max_two_values_with_projection)
    {
      auto abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      CHECK_EQUAL(-5, etl::ranges::max(3, -5, etl::ranges::less{}, abs_proj));
      CHECK_EQUAL(-5, etl::ranges::max(-5, 3, etl::ranges::less{}, abs_proj));
    }

  #if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(ranges_max_initializer_list)
    {
      CHECK_EQUAL(9, etl::ranges::max({3, 1, 4, 1, 5, 9, 2, 6}));
      CHECK_EQUAL(42, etl::ranges::max({42}));
    }

    //*************************************************************************
    TEST(ranges_max_initializer_list_with_comparator)
    {
      CHECK_EQUAL(1, etl::ranges::max({3, 1, 4, 1, 5, 9, 2, 6}, std::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_max_initializer_list_with_projection)
    {
      auto negate = [](int v)
      {
        return -v;
      };
      CHECK_EQUAL(1, etl::ranges::max({3, 1, 4, 5, 9, 2, 6}, etl::ranges::less{}, negate));
    }
  #endif

    //*************************************************************************
    TEST(ranges_max_range)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      CHECK_EQUAL(9, etl::ranges::max(vec));
    }

    //*************************************************************************
    TEST(ranges_max_range_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      CHECK_EQUAL(1, etl::ranges::max(vec, std::greater<int>{}));
    }

    //*************************************************************************
    TEST(ranges_max_range_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      CHECK_EQUAL(-5, etl::ranges::max(vec, etl::ranges::less{}, abs_proj));
    }

    //*************************************************************************
    TEST(ranges_max_single_element_range)
    {
      std::vector<int> vec{42};
      CHECK_EQUAL(42, etl::ranges::max(vec));
    }

    //*************************************************************************
    TEST(ranges_max_all_equal)
    {
      std::vector<int> vec{7, 7, 7, 7};
      CHECK_EQUAL(7, etl::ranges::max(vec));
    }

    //*************************************************************************
    TEST(ranges_max_negative_values)
    {
      std::vector<int> vec{-3, -1, -4, -1, -5};
      CHECK_EQUAL(-1, etl::ranges::max(vec));
    }

    //*************************************************************************
    TEST(ranges_max_matches_std)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
      CHECK_EQUAL(*std::max_element(vec.begin(), vec.end()), etl::ranges::max(vec));

      std::vector<int> vec2{-10, -20, -5, -15};
      CHECK_EQUAL(*std::max_element(vec2.begin(), vec2.end()), etl::ranges::max(vec2));
    }

    //*************************************************************************
    TEST(ranges_max_element_iterator_sentinel)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             it = etl::ranges::max_element(vec.begin(), vec.end());
      CHECK_EQUAL(9, *it);
    }

    //*************************************************************************
    TEST(ranges_max_element_iterator_sentinel_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             it = etl::ranges::max_element(vec.begin(), vec.end(), std::greater<int>{});
      CHECK_EQUAL(1, *it);
    }

    //*************************************************************************
    TEST(ranges_max_element_iterator_sentinel_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      auto it = etl::ranges::max_element(vec.begin(), vec.end(), etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(-5, *it);
    }

    //*************************************************************************
    TEST(ranges_max_element_range)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             it = etl::ranges::max_element(vec);
      CHECK_EQUAL(9, *it);
    }

    //*************************************************************************
    TEST(ranges_max_element_range_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             it = etl::ranges::max_element(vec, std::greater<int>{});
      CHECK_EQUAL(1, *it);
    }

    //*************************************************************************
    TEST(ranges_max_element_range_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      auto it = etl::ranges::max_element(vec, etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(-5, *it);
    }

    //*************************************************************************
    TEST(ranges_max_element_single_element)
    {
      std::vector<int> vec{42};
      auto             it = etl::ranges::max_element(vec);
      CHECK_EQUAL(42, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_max_element_empty_range)
    {
      std::vector<int> vec{};
      auto             it = etl::ranges::max_element(vec.begin(), vec.end());
      CHECK(it == vec.end());
    }

    //*************************************************************************
    TEST(ranges_max_element_all_equal)
    {
      std::vector<int> vec{7, 7, 7, 7};
      auto             it = etl::ranges::max_element(vec);
      CHECK_EQUAL(7, *it);
      CHECK(it == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_max_element_matches_std)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
      CHECK(std::max_element(vec.begin(), vec.end()) == etl::ranges::max_element(vec.begin(), vec.end()));

      std::vector<int> vec2{-10, -20, -5, -15};
      CHECK(std::max_element(vec2.begin(), vec2.end()) == etl::ranges::max_element(vec2));
    }

    //*************************************************************************
    TEST(ranges_minmax_two_values)
    {
      int  a = 3, b = 5;
      auto result = etl::ranges::minmax(a, b);
      CHECK_EQUAL(3, result.min);
      CHECK_EQUAL(5, result.max);

      int  c = 5, d = 3;
      auto result2 = etl::ranges::minmax(c, d);
      CHECK_EQUAL(3, result2.min);
      CHECK_EQUAL(5, result2.max);

      int  e = 7, f = 7;
      auto result3 = etl::ranges::minmax(e, f);
      CHECK_EQUAL(7, result3.min);
      CHECK_EQUAL(7, result3.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_two_values_with_comparator)
    {
      int  a = 3, b = 5;
      auto result = etl::ranges::minmax(a, b, std::greater<int>{});
      CHECK_EQUAL(5, result.min);
      CHECK_EQUAL(3, result.max);

      int  c = 5, d = 3;
      auto result2 = etl::ranges::minmax(c, d, std::greater<int>{});
      CHECK_EQUAL(5, result2.min);
      CHECK_EQUAL(3, result2.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_two_values_with_projection)
    {
      auto abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      int  a = 3, b = -5;
      auto result = etl::ranges::minmax(a, b, etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(3, result.min);
      CHECK_EQUAL(-5, result.max);

      int  c = -5, d = 3;
      auto result2 = etl::ranges::minmax(c, d, etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(3, result2.min);
      CHECK_EQUAL(-5, result2.max);
    }

  #if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(ranges_minmax_initializer_list)
    {
      auto result = etl::ranges::minmax({3, 1, 4, 1, 5, 9, 2, 6});
      CHECK_EQUAL(1, result.min);
      CHECK_EQUAL(9, result.max);

      auto result2 = etl::ranges::minmax({42});
      CHECK_EQUAL(42, result2.min);
      CHECK_EQUAL(42, result2.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_initializer_list_with_comparator)
    {
      auto result = etl::ranges::minmax({3, 1, 4, 1, 5, 9, 2, 6}, std::greater<int>{});
      CHECK_EQUAL(9, result.min);
      CHECK_EQUAL(1, result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_initializer_list_with_projection)
    {
      auto negate = [](int v)
      {
        return -v;
      };
      auto result = etl::ranges::minmax({3, 1, 4, 5, 9, 2, 6}, etl::ranges::less{}, negate);
      CHECK_EQUAL(9, result.min);
      CHECK_EQUAL(1, result.max);
    }
  #endif

    //*************************************************************************
    TEST(ranges_minmax_range)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             result = etl::ranges::minmax(vec);
      CHECK_EQUAL(1, result.min);
      CHECK_EQUAL(9, result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_range_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             result = etl::ranges::minmax(vec, std::greater<int>{});
      CHECK_EQUAL(9, result.min);
      CHECK_EQUAL(1, result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_range_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      auto result = etl::ranges::minmax(vec, etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(-1, result.min);
      CHECK_EQUAL(-5, result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_single_element_range)
    {
      std::vector<int> vec{42};
      auto             result = etl::ranges::minmax(vec);
      CHECK_EQUAL(42, result.min);
      CHECK_EQUAL(42, result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_all_equal)
    {
      std::vector<int> vec{7, 7, 7, 7};
      auto             result = etl::ranges::minmax(vec);
      CHECK_EQUAL(7, result.min);
      CHECK_EQUAL(7, result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_negative_values)
    {
      std::vector<int> vec{-3, -1, -4, -1, -5};
      auto             result = etl::ranges::minmax(vec);
      CHECK_EQUAL(-5, result.min);
      CHECK_EQUAL(-1, result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_element_iterator_sentinel)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             result = etl::ranges::minmax_element(vec.begin(), vec.end());
      CHECK_EQUAL(1, *result.min);
      CHECK_EQUAL(9, *result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_element_iterator_sentinel_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             result = etl::ranges::minmax_element(vec.begin(), vec.end(), std::greater<int>{});
      CHECK_EQUAL(9, *result.min);
      CHECK_EQUAL(1, *result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_element_iterator_sentinel_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      auto result = etl::ranges::minmax_element(vec.begin(), vec.end(), etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(-1, *result.min);
      CHECK_EQUAL(-5, *result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_element_range)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             result = etl::ranges::minmax_element(vec);
      CHECK_EQUAL(1, *result.min);
      CHECK_EQUAL(9, *result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_element_range_with_comparator)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6};
      auto             result = etl::ranges::minmax_element(vec, std::greater<int>{});
      CHECK_EQUAL(9, *result.min);
      CHECK_EQUAL(1, *result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_element_range_with_projection)
    {
      std::vector<int> vec{3, -1, 4, -5, 2};
      auto             abs_proj = [](int v)
      {
        return v < 0 ? -v : v;
      };
      auto result = etl::ranges::minmax_element(vec, etl::ranges::less{}, abs_proj);
      CHECK_EQUAL(-1, *result.min);
      CHECK_EQUAL(-5, *result.max);
    }

    //*************************************************************************
    TEST(ranges_minmax_element_single_element)
    {
      std::vector<int> vec{42};
      auto             result = etl::ranges::minmax_element(vec);
      CHECK_EQUAL(42, *result.min);
      CHECK_EQUAL(42, *result.max);
      CHECK(result.min == vec.begin());
      CHECK(result.max == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_minmax_element_empty_range)
    {
      std::vector<int> vec{};
      auto             result = etl::ranges::minmax_element(vec.begin(), vec.end());
      CHECK(result.min == vec.end());
      CHECK(result.max == vec.end());
    }

    //*************************************************************************
    TEST(ranges_minmax_element_all_equal)
    {
      std::vector<int> vec{7, 7, 7, 7};
      auto             result = etl::ranges::minmax_element(vec);
      CHECK_EQUAL(7, *result.min);
      CHECK_EQUAL(7, *result.max);
      CHECK(result.min == vec.begin());
    }

    //*************************************************************************
    TEST(ranges_minmax_element_matches_std)
    {
      std::vector<int> vec{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
      auto             std_result = std::minmax_element(vec.begin(), vec.end());
      auto             etl_result = etl::ranges::minmax_element(vec.begin(), vec.end());
      CHECK(std_result.first == etl_result.min);
      CHECK(std_result.second == etl_result.max);

      std::vector<int> vec2{-10, -20, -5, -15};
      auto             std_result2 = std::minmax_element(vec2.begin(), vec2.end());
      auto             etl_result2 = etl::ranges::minmax_element(vec2);
      CHECK(std_result2.first == etl_result2.min);
      CHECK(std_result2.second == etl_result2.max);
    }

    //*************************************************************************
    // ranges::clamp
    //*************************************************************************
    TEST(ranges_clamp_value_in_range)
    {
      CHECK_EQUAL(5, etl::ranges::clamp(5, 0, 10));
    }

    TEST(ranges_clamp_value_below_low)
    {
      CHECK_EQUAL(0, etl::ranges::clamp(-5, 0, 10));
    }

    TEST(ranges_clamp_value_above_high)
    {
      CHECK_EQUAL(10, etl::ranges::clamp(15, 0, 10));
    }

    TEST(ranges_clamp_value_equal_to_low)
    {
      CHECK_EQUAL(0, etl::ranges::clamp(0, 0, 10));
    }

    TEST(ranges_clamp_value_equal_to_high)
    {
      CHECK_EQUAL(10, etl::ranges::clamp(10, 0, 10));
    }

    TEST(ranges_clamp_with_comparator)
    {
      // Using greater: clamp(5, 10, 0, greater) means low=10, high=0 in reverse
      // order
      CHECK_EQUAL(5, etl::ranges::clamp(5, 10, 0, std::greater<int>{}));
      CHECK_EQUAL(10, etl::ranges::clamp(15, 10, 0, std::greater<int>{}));
      CHECK_EQUAL(0, etl::ranges::clamp(-5, 10, 0, std::greater<int>{}));
    }

    TEST(ranges_clamp_with_projection)
    {
      auto abs_proj = [](int x)
      {
        return x < 0 ? -x : x;
      };

      // Clamp by absolute value: value=-3, low=2, high=8
      // proj(-3)=3, proj(2)=2, proj(8)=8 => 3 is in [2,8] => returns -3
      CHECK_EQUAL(-3, etl::ranges::clamp(-3, 2, 8, etl::ranges::less{}, abs_proj));

      // proj(1)=1, proj(2)=2 => 1 < 2 => returns low=2
      CHECK_EQUAL(2, etl::ranges::clamp(1, 2, 8, etl::ranges::less{}, abs_proj));

      // proj(-10)=10, proj(8)=8 => 10 > 8 => returns high=8
      CHECK_EQUAL(8, etl::ranges::clamp(-10, 2, 8, etl::ranges::less{}, abs_proj));
    }

    TEST(ranges_clamp_returns_reference)
    {
      const int  value  = 5;
      const int  low    = 0;
      const int  high   = 10;
      const int& result = etl::ranges::clamp(value, low, high);
      CHECK_EQUAL(&value, &result);

      const int  below      = -5;
      const int& result_low = etl::ranges::clamp(below, low, high);
      CHECK_EQUAL(&low, &result_low);

      const int  above       = 15;
      const int& result_high = etl::ranges::clamp(above, low, high);
      CHECK_EQUAL(&high, &result_high);
    }

    TEST(ranges_clamp_constexpr)
    {
      constexpr int result1 = etl::ranges::clamp(5, 0, 10);
      constexpr int result2 = etl::ranges::clamp(-5, 0, 10);
      constexpr int result3 = etl::ranges::clamp(15, 0, 10);

      CHECK_EQUAL(5, result1);
      CHECK_EQUAL(0, result2);
      CHECK_EQUAL(10, result3);
    }

    TEST(ranges_clamp_matches_std)
    {
      for (int v = -20; v <= 20; ++v)
      {
        CHECK_EQUAL(std::clamp(v, 0, 10), etl::ranges::clamp(v, 0, 10));
      }
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_iterator)
    {
      std::vector<int> vec{3, 2, 1};
      std::vector<int> expected{3, 1, 2};

      auto result = etl::ranges::prev_permutation(vec.begin(), vec.end());

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_range)
    {
      std::vector<int> vec{3, 2, 1};
      std::vector<int> expected{3, 1, 2};

      auto result = etl::ranges::prev_permutation(vec);

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_first_permutation)
    {
      // {1, 2, 3} is the first (smallest) permutation; prev should wrap to last
      // and return found=false
      std::vector<int> vec{1, 2, 3};
      std::vector<int> expected{3, 2, 1};

      auto result = etl::ranges::prev_permutation(vec.begin(), vec.end());

      CHECK(result.in == vec.end());
      CHECK(result.found == false);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::prev_permutation(vec.begin(), vec.end());

      CHECK(result.in == vec.end());
      CHECK(result.found == false);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::prev_permutation(vec);

      CHECK(result.in == vec.end());
      CHECK(result.found == false);
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_with_comparator_iterator)
    {
      // With greater<>, prev_permutation acts like next_permutation with less<>
      std::vector<int> vec{1, 2, 3};
      std::vector<int> expected{1, 3, 2};

      auto result = etl::ranges::prev_permutation(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_with_comparator_range)
    {
      std::vector<int> vec{1, 2, 3};
      std::vector<int> expected{1, 3, 2};

      auto result = etl::ranges::prev_permutation(vec, etl::greater<int>{});

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {2, 20}, {1, 10}};

      auto result = etl::ranges::prev_permutation(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK_EQUAL(3, vec[0].key);
      CHECK_EQUAL(1, vec[1].key);
      CHECK_EQUAL(2, vec[2].key);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{3, 30}, {2, 20}, {1, 10}};

      auto result = etl::ranges::prev_permutation(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK_EQUAL(3, vec[0].key);
      CHECK_EQUAL(1, vec[1].key);
      CHECK_EQUAL(2, vec[2].key);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_full_cycle)
    {
      // Starting from last permutation {3,2,1}, calling prev_permutation
      // repeatedly should visit all 6 permutations and wrap around.
      std::vector<int> vec{3, 2, 1};
      int              count = 0;
      bool             found = true;

      do {
        ++count;
        auto result = etl::ranges::prev_permutation(vec);
        found       = result.found;
      } while (found);

      // 3! = 6 permutations
      CHECK_EQUAL(6, count);
      // Should wrap back to {3, 2, 1}
      std::vector<int> expected{3, 2, 1};
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_matches_std)
    {
      std::vector<int> data_std{3, 2, 1};
      std::vector<int> data_etl = data_std;

      bool std_result = std::prev_permutation(data_std.begin(), data_std.end());
      auto etl_result = etl::ranges::prev_permutation(data_etl);

      CHECK_EQUAL(std_result, etl_result.found);
      bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_matches_std_all_permutations)
    {
      std::vector<int> data_std{4, 3, 2, 1};
      std::vector<int> data_etl = data_std;

      bool complete = false;
      while (!complete)
      {
        bool std_result = std::prev_permutation(data_std.begin(), data_std.end());
        auto etl_result = etl::ranges::prev_permutation(data_etl);

        CHECK_EQUAL(std_result, etl_result.found);
        bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
        CHECK(are_equal);

        complete = !std_result;
      }
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_duplicates)
    {
      std::vector<int> data_std{2, 2, 1};
      std::vector<int> data_etl = data_std;

      bool std_result = std::prev_permutation(data_std.begin(), data_std.end());
      auto etl_result = etl::ranges::prev_permutation(data_etl);

      CHECK_EQUAL(std_result, etl_result.found);
      bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(ranges_prev_permutation_two_elements)
    {
      std::vector<int> vec{2, 1};
      std::vector<int> expected{1, 2};

      auto result = etl::ranges::prev_permutation(vec);

      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_iterator)
    {
      std::vector<int> vec{1, 2, 3};
      std::vector<int> expected{1, 3, 2};

      auto result = etl::ranges::next_permutation(vec.begin(), vec.end());

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_range)
    {
      std::vector<int> vec{1, 2, 3};
      std::vector<int> expected{1, 3, 2};

      auto result = etl::ranges::next_permutation(vec);

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_last_permutation)
    {
      // {3, 2, 1} is the last permutation; next should wrap to first and return
      // found=false
      std::vector<int> vec{3, 2, 1};
      std::vector<int> expected{1, 2, 3};

      auto result = etl::ranges::next_permutation(vec.begin(), vec.end());

      CHECK(result.in == vec.end());
      CHECK(result.found == false);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_empty)
    {
      std::vector<int> vec{};

      auto result = etl::ranges::next_permutation(vec.begin(), vec.end());

      CHECK(result.in == vec.end());
      CHECK(result.found == false);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_single_element)
    {
      std::vector<int> vec{42};

      auto result = etl::ranges::next_permutation(vec);

      CHECK(result.in == vec.end());
      CHECK(result.found == false);
      CHECK_EQUAL(42, vec[0]);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_with_comparator_iterator)
    {
      // With greater<>, next_permutation acts like prev_permutation with less<>
      std::vector<int> vec{3, 2, 1};
      std::vector<int> expected{3, 1, 2};

      auto result = etl::ranges::next_permutation(vec.begin(), vec.end(), etl::greater<int>{});

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_with_comparator_range)
    {
      std::vector<int> vec{3, 2, 1};
      std::vector<int> expected{3, 1, 2};

      auto result = etl::ranges::next_permutation(vec, etl::greater<int>{});

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_with_projection_iterator)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}};

      auto result = etl::ranges::next_permutation(vec.begin(), vec.end(), etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(3, vec[1].key);
      CHECK_EQUAL(2, vec[2].key);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_with_projection_range)
    {
      struct Item
      {
        int key;
        int value;
      };
      std::vector<Item> vec{{1, 10}, {2, 20}, {3, 30}};

      auto result = etl::ranges::next_permutation(vec, etl::ranges::less{}, [](const Item& item) { return item.key; });

      CHECK(result.in == vec.end());
      CHECK(result.found == true);
      CHECK_EQUAL(1, vec[0].key);
      CHECK_EQUAL(3, vec[1].key);
      CHECK_EQUAL(2, vec[2].key);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_full_cycle)
    {
      // Starting from first permutation {1,2,3}, calling next_permutation
      // repeatedly should visit all 6 permutations and wrap around.
      std::vector<int> vec{1, 2, 3};
      int              count = 0;
      bool             found = true;

      do {
        ++count;
        auto result = etl::ranges::next_permutation(vec);
        found       = result.found;
      } while (found);

      // 3! = 6 permutations
      CHECK_EQUAL(6, count);
      // Should wrap back to {1, 2, 3}
      std::vector<int> expected{1, 2, 3};
      CHECK(vec == expected);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_matches_std)
    {
      std::vector<int> data_std{1, 2, 3};
      std::vector<int> data_etl = data_std;

      bool std_result = std::next_permutation(data_std.begin(), data_std.end());
      auto etl_result = etl::ranges::next_permutation(data_etl);

      CHECK_EQUAL(std_result, etl_result.found);
      bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_matches_std_all_permutations)
    {
      std::vector<int> data_std{1, 2, 3, 4};
      std::vector<int> data_etl = data_std;

      bool complete = false;
      while (!complete)
      {
        bool std_result = std::next_permutation(data_std.begin(), data_std.end());
        auto etl_result = etl::ranges::next_permutation(data_etl);

        CHECK_EQUAL(std_result, etl_result.found);
        bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
        CHECK(are_equal);

        complete = !std_result;
      }
    }

    //*************************************************************************
    TEST(ranges_next_permutation_duplicates)
    {
      std::vector<int> data_std{1, 2, 2};
      std::vector<int> data_etl = data_std;

      bool std_result = std::next_permutation(data_std.begin(), data_std.end());
      auto etl_result = etl::ranges::next_permutation(data_etl);

      CHECK_EQUAL(std_result, etl_result.found);
      bool are_equal = std::equal(data_std.begin(), data_std.end(), data_etl.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(ranges_next_permutation_two_elements)
    {
      std::vector<int> vec{1, 2};
      std::vector<int> expected{2, 1};

      auto result = etl::ranges::next_permutation(vec);

      CHECK(result.found == true);
      CHECK(vec == expected);
    }
#endif
  }
} // namespace
