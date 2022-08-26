/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

Permission is hereby granted, free of charge, to any person obtaining data copy
of this software and associated documentation ciles(the "Software"), to deal
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
#include <list>
#include <ostream>

#include "etl/circular_iterator.h"

//template <typename TIterator>
//std::ostream& operator << (std::ostream& os, const etl::circular_iterator<TIterator>& ci)
//{
//  os << ci.begin() "," << ci.end();
//
//  return os;
//}

namespace 
{		
  using Data = std::list<int>;

  SUITE(test_cixed_iterator)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::circular_iterator<const int*> ci;

      CHECK_EQUAL(0U, size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(0U, ci.size());
    }

    //*************************************************************************
    TEST(test_constructor_from_iterators)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_iterator> ci(std::begin(data), std::end(data));

      CHECK(data.begin() == ci.begin());
      CHECK(data.end()   == ci.end());
      CHECK_EQUAL(data.size(), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(data.size(), ci.size());
    }

    //*************************************************************************
    TEST(test_constructor_from_reverse_iterators)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_reverse_iterator> ci(std::rbegin(data), std::rend(data));

      CHECK(data.rbegin() == ci.begin());
      CHECK(data.rend()   == ci.end());
      CHECK_EQUAL(data.size(), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(data.size(), ci.size());
    }

    //*************************************************************************
    TEST(test_constructor_from_span)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const etl::span<const int> sp(std::begin(data), std::end(data));

      etl::circular_iterator<const int*> ci(sp);

      CHECK(std::begin(data) == ci.begin());
      CHECK(std::end(data)   == ci.end());
      CHECK_EQUAL(sp.size(), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(sp.size(), ci.size());
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const etl::circular_iterator<const int*> ci1(std::begin(data), std::end(data));
      etl::circular_iterator<const int*> ci2(ci1);

      CHECK(ci1.begin() == ci2.begin());
      CHECK(ci1.end()   == ci2.end());
      CHECK_EQUAL(ci1.size(), ci2.size());
    }

    //*************************************************************************
    TEST(test_assignment_from_circular_iterator)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_iterator> ci1(std::begin(data), std::end(data));
      etl::circular_iterator<Data::const_iterator>ci2;
      ci2 = ci1;

      CHECK(ci1.begin() == ci2.begin());
      CHECK(ci1.end()   == ci2.end());
      CHECK_EQUAL(ci1.size(), ci2.size());
    }

    //*************************************************************************
    TEST(test_assignment_from_span)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::span<const int> sp(std::begin(data), std::end(data));
      
      etl::circular_iterator<const int*> ci;
      ci = sp;

      CHECK(std::begin(data) == ci.begin());
      CHECK(std::end(data)   == ci.end());
      CHECK_EQUAL(sp.size(), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(sp.size(), ci.size());
    }

    //*************************************************************************
    TEST(test_pre_increment)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 1; i < 21; ++i)
      {
        Data::const_iterator itr = data.begin();
        std::advance(itr, i % 10);
        CHECK_EQUAL(*itr, *++ci);
      }
    }

    //*************************************************************************
    TEST(test_post_increment)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        Data::const_iterator itr = data.begin();
        std::advance(itr, i % 10);
        CHECK_EQUAL(*itr, *ci++);
      }
    }

    //*************************************************************************
    TEST(test_pre_increment_reverse_iterator)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_reverse_iterator> ci(std::rbegin(data), std::rend(data));

      for (int i = 1; i < 21; ++i)
      {
        Data::const_reverse_iterator itr = data.rbegin();
        std::advance(itr, i % 10);
        CHECK_EQUAL(*itr, *++ci);
      }
    }

    //*************************************************************************
    TEST(test_post_increment_reverse_iterator)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_reverse_iterator> ci(std::rbegin(data), std::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        Data::const_reverse_iterator itr = data.rbegin();
        std::advance(itr, i % 10);
        CHECK_EQUAL(*itr, *ci++);
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        Data::const_reverse_iterator itr = data.rbegin();
        std::advance(itr, i % 10);
        CHECK_EQUAL(*itr, *--ci);
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement_reverse_iterator)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_reverse_iterator> ci(std::rbegin(data), std::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        Data::const_iterator itr = data.begin();
        std::advance(itr, i % 10);
        CHECK_EQUAL(*itr, *--ci);
      }
    }

    //*************************************************************************
    TEST(test_post_decrement)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_iterator> ci(std::begin(data), std::end(data));
      ci--;

      for (int i = 0; i < 20; ++i)
      {
        Data::const_reverse_iterator itr = data.rbegin();
        std::advance(itr, i % 10);
        CHECK_EQUAL(*itr, *ci--);
      }
    }

    //*************************************************************************
    TEST(test_post_decrement_reverse_iterator)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<Data::const_reverse_iterator> ci(std::rbegin(data), std::rend(data));
      ci--;

      for (int i = 0; i < 20; ++i)
      {
        Data::const_iterator itr = data.begin();
        std::advance(itr, i % 10);
        CHECK_EQUAL(*itr, *ci--);
      }
    }

    //*************************************************************************
    TEST(test_member_dereference_operator)
    {
      struct Test
      {
        int data;
        int b;
      };

      Test test[] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };

      etl::circular_iterator<Test*> ci(std::begin(test), std::end(test));

      CHECK_EQUAL(test[0].data, ci->data);
      CHECK_EQUAL(test[0].b, ci->b);

      *ci = { 7, 8 };

      CHECK_EQUAL(test[0].data, ci->data);
      CHECK_EQUAL(test[0].b, ci->b);

      ++ci;
      CHECK_EQUAL(test[1].data, ci->data);
      CHECK_EQUAL(test[1].b, ci->b);

      ++ci;
      CHECK_EQUAL(test[2].data, ci->data);
      CHECK_EQUAL(test[2].b, ci->b);
    }

    //*************************************************************************
    TEST(test_conversion_operator)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<const int*> ci(std::begin(data), std::end(data));

      CHECK_EQUAL(data, ci);

      ++ci;
      CHECK_EQUAL(data + 1, ci);

      ++ci;
      CHECK_EQUAL(data + 2, ci);
    }

    //*************************************************************************
    TEST(test_operator_plus_equals)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_iterator<const int*> ci(std::begin(data), std::end(data));

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(data[i % 10], *ci);
          ci += step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_plus)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_iterator<const int*> ci(std::begin(data), std::end(data));

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(data[i % 10], *ci);
          ci = ci + step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_minus_equals)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<const int*> initial(std::begin(data), std::end(data));
      etl::circular_iterator<const int*> ci;
      int step;

      step = 1;
      ci = initial;

      for (int i = 0; i < 20; i += step)
      {
        CHECK_EQUAL(data[(10 - i) % 10], *ci);
        ci -= step;
      }

      //step = 2;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 3;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 4;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 5;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 6;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 7;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 8;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 9;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 10;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 11;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 12;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 13;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 14;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 15;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 16;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 17;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 18;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 19;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}

      //step = 20;
      //ci = initial;

      //for (int i = 0; i < 20; i += step)
      //{
      //  CHECK_EQUAL(data[abs(i - step) % 10], *ci);
      //  ci -= step;
      //}
    }

    ////*************************************************************************
    //TEST(test_operator_minus)
    //{
    //  int compare[] = { 1, 2, 3, 4 };

    //  etl::circular_iterator<const int*> ci = &compare[1];

    //  for (int i = 0; i < 10; ++i)
    //  {
    //    CHECK_EQUAL(compare[1], *ci);
    //    ci = ci - 1;
    //  }
    //}

    ////*************************************************************************
    //TEST(test_assignment)
    //{
    //  int data;
    //  int b;

    //  etl::circular_iterator<int*> ci = &data;
    //  ci = &b;

    //  CHECK_EQUAL(&b, ci);

    //  ci = &data;

    //  CHECK_EQUAL(&data, ci);
    //}

    ////*************************************************************************
    //TEST(test_equality)
    //{
    //  int data;
    //  int b;

    //  etl::circular_iterator<int*> ci1 = &data;
    //  etl::circular_iterator<int*> ci2 = &data;
    //  etl::circular_iterator<int*> ci3 = &b;

    //  CHECK(ci1 == ci2);
    //  CHECK(ci1 != ci3);
    //}
  };
}
