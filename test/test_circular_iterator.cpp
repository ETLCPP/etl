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
#include <forward_list>
#include <array>

#include "etl/iterator.h"
#include "etl/circular_iterator.h"

#if ETL_USING_STL
  #if  ETL_USING_CPP14
    #define ETL_OR_STD_R std
  #else
    // STL doesn't have rbegin() or rend() before C++14
    #define ETL_OR_STD_R etl
  #endif
#else
  #define ETL_OR_STD_R etl
#endif

namespace 
{		
  using DataE = std::array<int, 10U>;
  using DataL = std::list<int>;
  using DataF = std::forward_list<int>;

  using ConstPointer        = const int*;
  using ConstReversePointer = ETL_OR_STD::reverse_iterator<const int*>;

  SUITE(test_circular_iterator)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::circular_iterator<ConstPointer> ci;

      CHECK_EQUAL(0U, size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(0U, ci.size());
    }

    //*************************************************************************
    TEST(test_construct_from_iterators_for_random_access_iterator)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

      CHECK(std::begin(data) == ci.begin());
      CHECK(std::end(data)   == ci.end());
      CHECK(std::begin(data) == ci.current());
      CHECK_EQUAL(ETL_OR_STD17::size(data), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(ETL_OR_STD17::size(data), ci.size());
    }

    //*************************************************************************
    TEST(test_construct_from_start_plus_iterators_for_random_access_iterator)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      ConstPointer start = std::begin(data);
      start++;

      etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data), start);

      CHECK(std::begin(data) == ci.begin());
      CHECK(std::end(data)   == ci.end());
      CHECK(start            == ci.current());
      CHECK_EQUAL(ETL_OR_STD17::size(data), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(ETL_OR_STD17::size(data), ci.size());
    }

    //*************************************************************************
    TEST(test_construct_from_iterators_for_bidirectional_iterator)
    {
      const DataL data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataL::const_iterator> ci(std::begin(data), std::end(data));

      CHECK(data.begin() == ci.begin());
      CHECK(data.end()   == ci.end());
      CHECK(data.begin() == ci.current());
      CHECK_EQUAL(data.size(), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(data.size(), ci.size());
    }

    //*************************************************************************
    TEST(test_construct_from_start_plus_iterators_for_bidirectional_iterator)
    {
      const DataL data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      DataL::const_iterator start = std::begin(data);
      start++;

      etl::circular_iterator<DataL::const_iterator> ci(std::begin(data), std::end(data), start);

      CHECK(data.begin() == ci.begin());
      CHECK(data.end()   == ci.end());
      CHECK(start        == ci.current());
      CHECK_EQUAL(data.size(), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(data.size(), ci.size());
    }

    //*************************************************************************
    TEST(test_construct_from_iterators_for_forward_iterator)
    {
      const DataF data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataF::const_iterator> ci(std::begin(data), std::end(data));

      CHECK(data.begin() == ci.begin());
      CHECK(data.end()   == ci.end());
      CHECK(data.begin() == ci.current());
      CHECK_EQUAL(size_t(std::distance(data.begin(), data.end())), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(size_t(std::distance(data.begin(), data.end())), ci.size());
    }

    //*************************************************************************
    TEST(test_construct_from_start_plus_iterators_for_forward_iterator)
    {
      const DataF data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      DataF::const_iterator start = std::begin(data);
      start++;

      etl::circular_iterator<DataF::const_iterator> ci(std::begin(data), std::end(data), start);

      CHECK(data.begin() == ci.begin());
      CHECK(data.end()   == ci.end());
      CHECK(start        == ci.current());
      CHECK_EQUAL(size_t(std::distance(data.begin(), data.end())), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(size_t(std::distance(data.begin(), data.end())), ci.size());
    }

    //*************************************************************************
    TEST(test_construct_from_reverse_iterators)
    {
      const DataL data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataL::const_reverse_iterator> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      CHECK(data.rbegin() == ci.begin());
      CHECK(data.rend()   == ci.end());
      CHECK_EQUAL(data.size(), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(data.size(), ci.size());
    }

    //*************************************************************************
    TEST(test_construct_from_start_plus_reverse_iterators)
    {
      const DataL data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      DataL::const_reverse_iterator start = ETL_OR_STD_R::rbegin(data);
      start++;

      etl::circular_iterator<DataL::const_reverse_iterator> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data), start);

      CHECK(data.rbegin() == ci.begin());
      CHECK(data.rend()   == ci.end());
      CHECK(start         == ci.current());
      CHECK_EQUAL(data.size(), size_t(std::distance(ci.begin(), ci.end())));
      CHECK_EQUAL(data.size(), ci.size());
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const etl::circular_iterator<ConstPointer> ci1(std::begin(data), std::end(data));
      etl::circular_iterator<ConstPointer> ci2(ci1);

      CHECK(ci1.begin() == ci2.begin());
      CHECK(ci1.end()   == ci2.end());
      CHECK_EQUAL(ci1.size(), ci2.size());
    }

    //*************************************************************************
    TEST(test_assignment_from_circular_iterator)
    {
      const DataL data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataL::const_iterator> ci1(std::begin(data), std::end(data));
      etl::circular_iterator<DataL::const_iterator>ci2;
      ci2.operator=(ci1);

      CHECK(ci1.begin() == ci2.begin());
      CHECK(ci1.end()   == ci2.end());
      CHECK_EQUAL(ci1.size(), ci2.size());
    }

    //*************************************************************************
    TEST(test_pre_increment_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

      etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *++ci);
      }
    }

    //*************************************************************************
    TEST(test_pre_increment_for_bidirectional_iterator)
    {
      const DataL data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

      etl::circular_iterator<DataL::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *++ci);
      }
    }

    //*************************************************************************
    TEST(test_pre_increment_for_forward_iterator)
    {
      const DataF data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

      etl::circular_iterator<DataF::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *++ci);
      }
    }

    //*************************************************************************
    TEST(test_post_increment_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci++);
      }
    }

    //*************************************************************************
    TEST(test_post_increment_for_bidirectional_iterator)
    {
      const DataL data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataL::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci++);
      }
    }

    //*************************************************************************
    TEST(test_post_increment_for_forward_iterator)
    {
      const DataF data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataF::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci++);
      }
    }

    //*************************************************************************
    TEST(test_pre_increment_reverse_iterator_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 8, 7, 6, 5, 4, 3, 2, 1, 0, 9 };

      etl::circular_iterator<ConstReversePointer> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *++ci);
      }
    }

    //*************************************************************************
    TEST(test_pre_increment_reverse_iterator_for_bidirectional_iterator)
    {
      const DataL data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 8, 7, 6, 5, 4, 3, 2, 1, 0, 9 };

      etl::circular_iterator<DataL::const_reverse_iterator> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *++ci);
      }
    }

    //*************************************************************************
    TEST(test_post_increment_reverse_iterator_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

      etl::circular_iterator<ConstReversePointer> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci++);
      }
    }

    //*************************************************************************
    TEST(test_post_increment_reverse_iterator_for_bidirectional_iterator)
    {
      const DataL data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

      etl::circular_iterator<DataL::const_reverse_iterator> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci++);
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

      etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *--ci);
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement_for_bidirectional_iterator)
    {
      const DataL data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

      etl::circular_iterator<DataL::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *--ci);
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement_reverse_iterator_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<ConstReversePointer> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *--ci);
      }
    }

    //*************************************************************************
    TEST(test_pre_decrement_reverse_iterator_for_bidirectional_iterator)
    {
      const DataL data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataL::const_reverse_iterator> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *--ci);
      }
    }

    //*************************************************************************
    TEST(test_post_decrement_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci--);
      }
    }

    //*************************************************************************
    TEST(test_post_decrement_for_bidirectional_iterator)
    {
      const DataL data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      etl::circular_iterator<DataL::const_iterator> ci(std::begin(data), std::end(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci--);
      }
    }

    //*************************************************************************
    TEST(test_post_decrement_reverse_iterator_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

      etl::circular_iterator<ConstReversePointer> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci--);
      }
    }

    //*************************************************************************
    TEST(test_post_decrement_reverse_iterator_for_bidirectional_iterator)
    {
      const DataL data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

      etl::circular_iterator<DataL::const_reverse_iterator> ci(ETL_OR_STD_R::rbegin(data), ETL_OR_STD_R::rend(data));

      for (int i = 0; i < 20; ++i)
      {
        CHECK_EQUAL(expected[i % expected.size()], *ci--);
      }
    }

    //*************************************************************************
    TEST(test_member_dereference_operator_for_random_access_iterator)
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
    TEST(test_member_dereference_operator_for_bidirectional_iterator)
    {
      struct Test
      {
        int data;
        int b;
      };

      std::list<Test> test = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
      std::list<Test>::iterator itr = test.begin();

      etl::circular_iterator<std::list<Test>::iterator> ci(std::begin(test), std::end(test));

      CHECK_EQUAL(itr->b, ci->b);

      *ci = { 7, 8 };
      CHECK_EQUAL(itr->b, ci->b);

      ++itr;
      ++ci;
      CHECK_EQUAL(itr->b, ci->b);

      ++itr;
      ++ci;
      CHECK_EQUAL(itr->b, ci->b);
    }

    //*************************************************************************
    TEST(test_member_dereference_operator_for_forward_iterator)
    {
      struct Test
      {
        int data;
        int b;
      };

      std::forward_list<Test> test = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
      std::forward_list<Test>::iterator itr = test.begin();

      etl::circular_iterator<std::forward_list<Test>::iterator> ci(std::begin(test), std::end(test));

      CHECK_EQUAL(itr->b, ci->b);

      *ci = { 7, 8 };
      CHECK_EQUAL(itr->b, ci->b);

      ++itr;
      ++ci;
      CHECK_EQUAL(itr->b, ci->b);

      ++itr;
      ++ci;
      CHECK_EQUAL(itr->b, ci->b);
    }

    //*************************************************************************
    TEST(test_conversion_operator_for_random_access_iterator)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

      CHECK_EQUAL(data, ci.current());

      ++ci;
      CHECK_EQUAL(data + 1, ci.current());

      ++ci;
      CHECK_EQUAL(data + 2, ci.current());
    }

    //*************************************************************************
    TEST(test_conversion_operator_for_bidirectional_iterator)
    {
      DataL data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataL::const_iterator> ci(std::begin(data), std::end(data));
      DataL::const_iterator itr = std::begin(data);

      CHECK(itr == ci.current());

      ++itr;
      ++ci;
      CHECK(itr == ci.current());

      ++itr;
      ++ci;
      CHECK(itr == ci.current());
    }

    //*************************************************************************
    TEST(test_conversion_operator_for_forward_iterator)
    {
      DataF data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<DataF::const_iterator> ci(std::begin(data), std::end(data));
      DataF::const_iterator itr = std::begin(data);

      CHECK(itr == ci.current());

      ++itr;
      ++ci;
      CHECK(itr == ci.current());

      ++itr;
      ++ci;
      CHECK(itr == ci.current());
    }

    //*************************************************************************
    TEST(test_operator_plus_equals_for_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const DataE expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % 10], *ci);
          ci += step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_plus_for_random_access_iterator)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(data[i % 10], *ci);
          ci = ci + step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_minus_equals_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const int expected[] = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      etl::circular_iterator<ConstPointer> initial(std::begin(data), std::end(data));
      etl::circular_iterator<ConstPointer> ci;

      for (int step = 1; step < 20; ++step)
      {
        ci = initial;
        
        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % ETL_OR_STD17::size(data)], *ci);
          ci -= step;
        }
      }
    }

    //*************************************************************************
    TEST(test_operator_minus_random_access_iterator)
    {
      const int data[]     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const int expected[] = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      for (int step = 1; step < 20; ++step)
      {
        etl::circular_iterator<ConstPointer> ci(std::begin(data), std::end(data));

        for (int i = 0; i < 20; i += step)
        {
          CHECK_EQUAL(expected[i % ETL_OR_STD17::size(data)], *ci);
          ci = ci - step;
        }
      }
    }

    //*************************************************************************
    TEST(test_equality)
    {
      const int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      etl::circular_iterator<ConstPointer> ci1(std::begin(data), std::end(data));
      etl::circular_iterator<ConstPointer> ci2(std::begin(data), std::end(data));
      etl::circular_iterator<ConstPointer> ci3(std::begin(data), std::end(data));

      ci1 += 5;
      ci2 += 5;
      ci3 += 4;

      CHECK(ci1 == ci2);
      CHECK(ci2 == ci1);
      CHECK(ci1 != ci3);
      CHECK(ci3 != ci1);
    }
  };
}
