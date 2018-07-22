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

#include "../include/etl/stl/alternate/algorithm.h"

#include <algorithm>
#include <vector>
#include <list>

namespace
{
  int dataEQ[10] = { 1, 1, 3, 3, 5, 5, 7, 7, 9, 9 };
  std::list<int> dataEQL = { 1, 1, 3, 3, 5, 5, 7, 7, 9, 9 };

  int dataS[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  std::list<int> dataSL = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

  int dataA[10] = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  typedef std::vector<int> Vector;
  Vector dataV = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  typedef std::list<int> List;
  List dataL = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

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

  std::ostream& operator << (std::ostream& os, const Data& data_)
  {
    os << data_.a << "," << data_.b;
    return os;
  }

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
    TEST(distance_non_random)
    {
      ptrdiff_t d1 = std::distance(dataL.begin(), dataL.end());
      ptrdiff_t d2 = etlstd::distance(dataL.begin(), dataL.end());

      CHECK_EQUAL(d1, d2);
    }

    //*************************************************************************
    TEST(distance_random)
    {
      ptrdiff_t d1 = std::distance(dataV.begin(), dataV.end());
      ptrdiff_t d2 = etlstd::distance(dataV.begin(), dataV.end());

      CHECK_EQUAL(d1, d2);
    }

    //*************************************************************************
    TEST(advance_non_random)
    {
      List::const_iterator itr1 = dataL.begin();
      List::const_iterator itr2 = dataL.begin();
      
      std::advance(itr1, 4);
      std::advance(itr2, 4);

      CHECK_EQUAL(*itr1, *itr2);
    }

    //*************************************************************************
    TEST(advance_random)
    {
      Vector::const_iterator itr1 = dataV.begin();
      Vector::const_iterator itr2 = dataV.begin();

      std::advance(itr1, 4);
      std::advance(itr2, 4);

      CHECK_EQUAL(*itr1, *itr2);
    }

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
        int* lb2 = etlstd::lower_bound(std::begin(dataS), std::end(dataS), i);

        CHECK_EQUAL(lb1, lb2);
      }
    }

    //*************************************************************************
    TEST(lower_bound_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        std::list<int>::iterator lb1 = std::lower_bound(std::begin(dataSL), std::end(dataSL), i);
        std::list<int>::iterator lb2 = etlstd::lower_bound(std::begin(dataSL), std::end(dataSL), i);

        CHECK_EQUAL(std::distance(std::begin(dataSL), lb1), std::distance(std::begin(dataSL), lb2));
      }
    }

    //*************************************************************************
    TEST(upper_bound_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        int* lb1 = std::upper_bound(std::begin(dataS), std::end(dataS), i);
        int* lb2 = etlstd::upper_bound(std::begin(dataS), std::end(dataS), i);

        CHECK_EQUAL(std::distance(std::begin(dataS), lb1), std::distance(std::begin(dataS), lb2));
      }
    }

    //*************************************************************************
    TEST(upper_bound_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        std::list<int>::iterator lb1 = std::upper_bound(std::begin(dataSL), std::end(dataSL), i);
        std::list<int>::iterator lb2 = etlstd::upper_bound(std::begin(dataSL), std::end(dataSL), i);

        CHECK_EQUAL(std::distance(std::begin(dataSL), lb1), std::distance(std::begin(dataSL), lb2));
      }
    }

    //*************************************************************************
    TEST(equal_range_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        std::pair<int*, int*> lb1 = std::equal_range(std::begin(dataEQ), std::end(dataEQ), i);
        std::pair<int*, int*> lb2 = etlstd::equal_range(std::begin(dataEQ), std::end(dataEQ), i);

        CHECK_EQUAL(std::distance(std::begin(dataEQ), lb1.first), std::distance(std::begin(dataEQ), lb2.first));
        CHECK_EQUAL(std::distance(lb1.first, lb1.second), std::distance(lb2.first, lb2.second));
      }
    }

    //*************************************************************************
    TEST(equal_range_non_random_iterator)
    {
      for (int i = 0; i < 11; ++i)
      {
        std::pair<std::list<int>::iterator, std::list<int>::iterator> lb1 = std::equal_range(std::begin(dataEQL), std::end(dataEQL), i);
        std::pair<std::list<int>::iterator, std::list<int>::iterator> lb2 = etlstd::equal_range(std::begin(dataEQL), std::end(dataEQL), i);

        CHECK_EQUAL(std::distance(std::begin(dataEQL), lb1.first), std::distance(std::begin(dataEQL), lb2.first));
        CHECK_EQUAL(std::distance(lb1.first, lb1.second), std::distance(lb2.first, lb2.second));
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
  };
}
