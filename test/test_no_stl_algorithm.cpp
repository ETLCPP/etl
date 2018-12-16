/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#undef min
#undef max

#include "etl/stl/alternate/algorithm.h"

#include <algorithm>
#include <vector>
#include <list>
#include <memory>

#include "no_stl_test_iterators.h"

namespace
{
  const size_t SIZE = 10;

  int dataEQ[SIZE] = { 1, 1, 3, 3, 5, 5, 7, 7, 9, 9 };
  std::list<int> dataEQL = { 1, 1, 3, 3, 5, 5, 7, 7, 9, 9 };

  int dataS[SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  std::list<int> dataSL = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  int dataA[SIZE] = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  typedef std::vector<int> Vector;
  Vector dataV = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  typedef std::list<int> List;
  List dataL = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  int dataD1[SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int dataD2[SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  class Data
  {
  public:

    Data()
      : a(0),
        b(0)
    {
    }

    Data(int a, int b)
      : a(a),
        b(b)
    {
    }

    int a;
    int b;
  };

  Data dataD[10] = { Data(1, 2), Data(2, 1), Data(3, 4), Data(4, 3), Data(5, 6), Data(6, 5), Data(7, 8), Data(8, 7), Data(9, 10), Data(10, 9) };

  bool operator ==(const Data& lhs, const Data& rhs)
  {
    return (lhs.a == rhs.a) && (lhs.b == rhs.b);
  }

  struct StructDataPredicate
  {
    bool operator ()(const Data& lhs, const Data& rhs) const
    {
      return lhs.a < rhs.a;
    }
  };

  struct StructDataEquality
  {
    bool operator ()(const Data& lhs, const Data& rhs) const
    {
      return lhs.a == rhs.a;
    }
  };

//  std::ostream& operator << (std::ostream& os, const Data& data_)
//  {
//    os << data_.a << "," << data_.b;
//    return os;
//  }

  struct Greater : public std::binary_function<int, int, bool>
  {
    bool operator()(int a, int b) const
    {
      return a > b;
    }
  };

  SUITE(test_no_stl_algorithm)
  {
    //*************************************************************************
    TEST(min)
    {
      int a = 1;
      int b = 2;

      CHECK_EQUAL((std::min(a, b)), (etlstd::min(a, b)));
      CHECK_EQUAL((std::min(b, a)), (etlstd::min(b, a)));
    }

    //*************************************************************************
    TEST(min_compare)
    {
      int a = 1;
      int b = 2;

      CHECK_EQUAL((std::min(a, b, Greater())), (etlstd::min(a, b, Greater())));
      CHECK_EQUAL((std::min(b, a, Greater())), (etlstd::min(b, a, Greater())));
    }

    //*************************************************************************
    TEST(max)
    {
      int a = 1;
      int b = 2;

      CHECK_EQUAL((std::max(a, b)), (etlstd::max(a, b)));
      CHECK_EQUAL((std::max(b, a)), (etlstd::max(b, a)));
    }

    //*************************************************************************
    TEST(max_compare)
    {
      int a = 1;
      int b = 2;

      CHECK_EQUAL((std::max(a, b, Greater())), (etlstd::max(a, b, Greater())));
      CHECK_EQUAL((std::max(b, a, Greater())), (etlstd::max(b, a, Greater())));
    }

    //*************************************************************************
    TEST(copy_pod_pointer)
    {
      int data1[10];
      int data2[10];

      std::copy(std::begin(dataA), std::end(dataA), std::begin(data1));
      etlstd::copy(std::begin(dataA), std::end(dataA), std::begin(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_non_pod_pointer)
    {
      Data data1[10];
      Data data2[10];

      std::copy(std::begin(dataD), std::end(dataD), std::begin(data1));
      etlstd::copy(std::begin(dataD), std::end(dataD), std::begin(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_non_random_iterator)
    {
      List data1(dataL.size());
      List data2(dataL.size());

      std::copy(std::begin(dataA), std::end(dataA), std::begin(data1));
      etlstd::copy(std::begin(dataA), std::end(dataA), std::begin(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_n_pod_pointer)
    {
      int data1[10];
      int data2[10];

      std::copy_n(std::begin(dataA), 10, std::begin(data1));
      etlstd::copy_n(std::begin(dataA), 10, std::begin(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_n_non_pod_pointer)
    {
      Data data1[10];
      Data data2[10];

      std::copy_n(std::begin(dataD), 10, std::begin(data1));
      etlstd::copy_n(std::begin(dataD), 10, std::begin(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_n_non_random_iterator)
    {
      List data1(dataL.size());
      List data2(dataL.size());

      std::copy_n(std::begin(dataA), 10, std::begin(data1));
      etlstd::copy_n(std::begin(dataA), 10, std::begin(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_backward_pod_pointer)
    {
      int data1[10];
      int data2[10];

      std::copy_backward(std::begin(dataA), std::end(dataA), std::end(data1));
      etlstd::copy_backward(std::begin(dataA), std::end(dataA), std::end(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_backward_non_pod_pointer)
    {
      Data data1[10];
      Data data2[10];

      std::copy_backward(std::begin(dataD), std::end(dataD), std::end(data1));
      etlstd::copy_backward(std::begin(dataD), std::end(dataD), std::end(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(copy_backward_non_random_iterator)
    {
      List data1(dataL.size());
      List data2(dataL.size());

      std::copy_backward(std::begin(dataA), std::end(dataA), std::end(data1));
      etlstd::copy_backward(std::begin(dataA), std::end(dataA), std::end(data2));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(lower_bound_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::lower_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etlstd::lower_bound(random_iterator<int>(std::begin(dataS)), random_iterator<int>(std::end(dataS)), i);

        CHECK_EQUAL(lb1, lb2);
      }
    }

    //*************************************************************************
    TEST(lower_bound_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::lower_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etlstd::lower_bound(non_random_iterator<int>(std::begin(dataS)), non_random_iterator<int>(std::end(dataS)), i);

        CHECK_EQUAL(std::distance(std::begin(dataS), lb1), std::distance(std::begin(dataS), lb2));
      }
    }

    //*************************************************************************
    TEST(upper_bound_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::upper_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etlstd::upper_bound(random_iterator<int>(std::begin(dataS)), random_iterator<int>(std::end(dataS)), i);

        CHECK_EQUAL(std::distance(std::begin(dataS), lb1), std::distance(std::begin(dataS), lb2));
      }
    }

    //*************************************************************************
    TEST(upper_bound_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::upper_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etlstd::upper_bound(non_random_iterator<int>(std::begin(dataS)), non_random_iterator<int>(std::end(dataS)), i);

        CHECK_EQUAL(std::distance(std::begin(dataS), lb1), std::distance(std::begin(dataS), lb2));
      }
    }

    //*************************************************************************
    TEST(equal_range_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        std::pair<int*, int*>    lb1 = std::equal_range(std::begin(dataEQ), std::end(dataEQ), i);
        etlstd::pair<random_iterator<int>, random_iterator<int>> lb2 = etlstd::equal_range(random_iterator<int>(std::begin(dataEQ)), random_iterator<int>(std::end(dataEQ)), i);

        CHECK_EQUAL(std::distance(std::begin(dataEQ), lb1.first), std::distance<int*>(std::begin(dataEQ), lb2.first));
        CHECK_EQUAL(std::distance(lb1.first, lb1.second), std::distance<int*>(lb2.first, lb2.second));
      }
    }

    //*************************************************************************
    TEST(equal_range_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        std::pair<int*, int*>    lb1 = std::equal_range(std::begin(dataEQ), std::end(dataEQ), i);
        etlstd::pair<non_random_iterator<int>, non_random_iterator<int>> lb2 = etlstd::equal_range(non_random_iterator<int>(std::begin(dataEQ)), non_random_iterator<int>(std::end(dataEQ)), i);

        CHECK_EQUAL(std::distance(std::begin(dataEQ), lb1.first), std::distance<int*>(std::begin(dataEQ), lb2.first));
        CHECK_EQUAL(std::distance(lb1.first, lb1.second), std::distance<int*>(lb2.first, lb2.second));
      }
    }

    //*************************************************************************
    TEST(fill_non_char)
    {
      int data1[10];
      int data2[10];

      std::fill(std::begin(data1), std::end(data1), 0x12345678);
      etlstd::fill(std::begin(data2), std::end(data2), 0x12345678);

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(fill_char)
    {
      unsigned char data1[10];
      unsigned char data2[10];

      std::fill(std::begin(data1), std::end(data1), char(0x12));
      etlstd::fill(std::begin(data2), std::end(data2), char(0x12));

      bool isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(swap)
    {
      int a = 1;
      int b = 2;

      etlstd::swap(a, b);
      CHECK_EQUAL(2, a);
      CHECK_EQUAL(1, b);
    }

    //*************************************************************************
    TEST(iter_swap)
    {
      int a = 1;
      int b = 2;

      etlstd::iter_swap(&a, &b);
      CHECK_EQUAL(2, a);
      CHECK_EQUAL(1, b);
    }

    //*************************************************************************
    TEST(equal)
    {
      CHECK(etlstd::equal(std::begin(dataV), std::end(dataV), std::begin(dataL)));
      CHECK(!etlstd::equal(std::begin(dataSL), std::end(dataSL), std::begin(dataL)));
    }

    //*************************************************************************
    TEST(lexicographical_compare)
    {
      std::string text1("Hello World");
      std::string text2("Hello Xorld");

      bool t1 = std::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.begin() + 7);
      bool t2 = etlstd::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.begin() + 7);

      CHECK(t1 == t2);
    }

    //*************************************************************************
    TEST(lexicographical_compare_greater)
    {
      std::string text1("Hello World");
      std::string text2("Hello Xorld");

      bool t1 = std::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.begin() + 7, Greater());
      bool t2 = etlstd::lexicographical_compare(text1.begin(), text1.end(), text2.begin(), text2.begin() + 7, Greater());

      CHECK(t1 == t2);
    }

    //*************************************************************************
    TEST(search)
    {
      std::string haystack = "ABCDFEGHIJKLMNOPQRSTUVWXYZ";
      std::string needle   = "KLMNO";

      std::string::iterator itr1 = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin());
      std::string::iterator itr2 = etlstd::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin());

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(search_predicate)
    {
      std::string haystack = "ABCDFEGHIJKLMNOPQRSTUVWXYZ";
      std::string needle   = "KLMNO";

      std::string::iterator itr1 = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin(), std::equal_to<char>());
      std::string::iterator itr2 = etlstd::search(haystack.begin(), haystack.end(), needle.begin(), needle.begin(), std::equal_to<char>());

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(heap)
    {
      Vector data1 = dataV;
      Vector data2 = dataV;

      std::make_heap(data1.begin(), data1.end());
      etlstd::make_heap(data2.begin(), data2.end());

      bool isEqual;

      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      std::pop_heap(data1.begin(), data1.end());
      etlstd::pop_heap(data2.begin(), data2.end());

      data1.pop_back();
      data2.pop_back();

      CHECK(std::is_heap(data1.begin(), data1.end()));
      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      CHECK(std::is_heap(data2.begin(), data2.end()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      data1.push_back(5);
      data2.push_back(5);

      std::push_heap(data1.begin(), data1.end());
      etlstd::push_heap(data2.begin(), data2.end());

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
      etlstd::make_heap(data2.begin(), data2.end(), Greater());

      bool isEqual;

      CHECK(std::is_heap(data2.begin(), data2.end(), Greater()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);

      std::pop_heap(data1.begin(), data1.end(), Greater());
      etlstd::pop_heap(data2.begin(), data2.end(), Greater());

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
      etlstd::push_heap(data2.begin(), data2.end(), Greater());

      CHECK(std::is_heap(data2.begin(), data2.end(), Greater()));

      isEqual = std::equal(std::begin(data1), std::end(data1), std::begin(data2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(find)
    {
      int* itr1 = std::find(std::begin(dataA), std::end(dataA), 5);
      int* itr2 = etlstd::find(std::begin(dataA), std::end(dataA), 5);

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
      int* itr2 = etlstd::find_if(std::begin(dataA), std::end(dataA), predicate());

      CHECK(itr1 == itr2);
    }

    //*************************************************************************
    TEST(count)
    {
      size_t c1 = std::count(std::begin(dataEQ), std::end(dataEQ), 5);
      size_t c2 = etlstd::count(std::begin(dataEQ), std::end(dataEQ), 5);

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
      size_t c2 = etlstd::count_if(std::begin(dataEQ), std::end(dataEQ), predicate());

      CHECK(c1 == c2);
    }

    //*************************************************************************
    TEST(fill_n)
    {
      int* p1 = std::fill_n(std::begin(dataD1), SIZE, 5);
      int* p2 = etlstd::fill_n(std::begin(dataD2), SIZE, 5);

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

      int* p1 = std::transform(std::begin(dataS), std::end(dataS), std::begin(dataD1), Function());
      int* p2 = etlstd::transform(std::begin(dataS), std::end(dataS), std::begin(dataD2), Function());

      CHECK(p2 == std::end(dataD2));

      bool isEqual = std::equal(std::begin(dataD1), std::end(dataD1), std::begin(dataD2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(transform2)
    {
      struct Function
      {
        int operator()(int d1, int d2) const
        {
          return d1 + d2;
        }
      };

      int* p1 = std::transform(std::begin(dataS), std::end(dataS), std::begin(dataA), std::begin(dataD1), Function());
      int* p2 = etlstd::transform(std::begin(dataS), std::end(dataS), std::begin(dataA), std::begin(dataD2), Function());

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
      etlstd::move(data1.begin(), data1.end(), std::back_inserter(data2));

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
      etlstd::move_backward(data1.begin(), data1.end(), data2.end());

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
  };
}
