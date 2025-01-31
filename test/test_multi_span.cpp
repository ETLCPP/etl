/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#include "etl/multi_span.h"

#include <iterator>
#include <vector>
#include <array>

namespace
{
  constexpr int Sentinal1 = 91;
  constexpr int Sentinal2 = 92;
  constexpr int Sentinal3 = 93;
  constexpr int Sentinal4 = 94;

  const std::array<int, 5> data1 = { 0, 1, 2, 3, Sentinal1 };
  const std::array<int, 4> data2 = { 4, 5, 6, Sentinal2 };
  const std::array<int, 2> data3 = { 7, Sentinal3 };
  const std::array<int, 3> data4 = { 8, 9, Sentinal4 };

  int data5[] = { 0, 0, 0, 0 };
  int data6[] = { 0, 0, 0 };
  int data7[] = { 0 };
  int data8[] = { 0, 0 };

  struct Data
  {
    int i;
  };

  Data struct_data1[] = {Data{1}, Data{2}, Data{3}};
  Data struct_data2[] = {Data{4}, Data{5}, Data{6}};

  SUITE(test_multi_span)
  {
    //*************************************************************************
    TEST(test_construct_from_span_list)
    {
      std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      etl::multi_span<const int>::span_list_type list(span_list.data(), span_list.size());

      etl::multi_span<const int> ms_int(list);

      CHECK(!ms_int.empty());
      CHECK_EQUAL(5U, ms_int.size_spans());
      CHECK_EQUAL(40U, ms_int.size_bytes());
      CHECK_EQUAL(10U, ms_int.size());
    }

    //*************************************************************************
    TEST(test_construct_from_container)
    {
      std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      etl::multi_span<const int> ms_int(span_list);

      CHECK(!ms_int.empty());
      CHECK_EQUAL(5U, ms_int.size_spans());
      CHECK_EQUAL(40U, ms_int.size_bytes());
      CHECK_EQUAL(10U, ms_int.size());
    }

    //*************************************************************************
    TEST(test_construct_from_const_container)
    {
      const std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      etl::multi_span<const int> ms_int(span_list);

      CHECK(!ms_int.empty());
      CHECK_EQUAL(5U, ms_int.size_spans());
      CHECK_EQUAL(40U, ms_int.size_bytes());
      CHECK_EQUAL(10U, ms_int.size());
    }

    //*************************************************************************
    TEST(test_construct_from_iterators)
    {
      std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      etl::multi_span<const int> ms_int(span_list.data(), span_list.size());

      CHECK(!ms_int.empty());
      CHECK_EQUAL(5U, ms_int.size_spans());
      CHECK_EQUAL(40U, ms_int.size_bytes());
      CHECK_EQUAL(10U, ms_int.size());
    }

    //*************************************************************************
    TEST(test_construct_from_iterators_and_length)
    {
      std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      etl::multi_span<const int> ms_int(span_list.data(), span_list.size());

      CHECK(!ms_int.empty());
      CHECK_EQUAL(5U, ms_int.size_spans());
      CHECK_EQUAL(40U, ms_int.size_bytes());
      CHECK_EQUAL(10U, ms_int.size());
    }

    //*************************************************************************
    TEST(test_constructor_empty_1)
    {
      std::vector<etl::span<const int>> span_list =
      {
        etl::span<const int>() // Empty span.
      };

      etl::multi_span<const int> ms_int(span_list.data(), span_list.size());

      CHECK(ms_int.empty());
      CHECK_EQUAL(1U, ms_int.size_spans());
      CHECK_EQUAL(0U, ms_int.size_bytes());
      CHECK_EQUAL(0U, ms_int.size());
    }

    //*************************************************************************
    TEST(test_constructor_empty_2)
    {
      std::vector<etl::span<const int>> span_list =
      {
      };

      etl::multi_span<const int> ms_int(span_list);

      CHECK(ms_int.empty());
      CHECK_EQUAL(0U, ms_int.size_spans());
      CHECK_EQUAL(0U, ms_int.size_bytes());
      CHECK_EQUAL(0U, ms_int.size());
    }

    //*************************************************************************
    TEST(test_iterator_copy_from_multi_span)
    {
      std::vector<etl::span<const int>> span_list =
      {
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      etl::multi_span<const int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int> result;

      etl::copy(ms_int.begin(), ms_int.end(), std::back_inserter(result));
      CHECK_EQUAL(expected.size(), result.size());
      CHECK(std::equal(expected.begin(), expected.end(), result.begin()));
    }

    //*************************************************************************
    TEST(test_iterator_copy_to_multi_span)
    {
      std::vector<etl::span<int>> span_list =
      {
        etl::span<int>(data5),
        etl::span<int>(data6),
        etl::span<int>(),      // Empty span.
        etl::span<int>(data7),
        etl::span<int>(data8)
      };

      etl::multi_span<int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      std::copy(expected.begin(), expected.end(), ms_int.begin());
      CHECK(std::equal(expected.begin(), expected.end(), ms_int.begin()));
    }

    //*************************************************************************
    TEST(test_member_pointer_operator)
    {
      std::vector<etl::span<Data>> span_list =
      {
        etl::span<Data>(struct_data1),
        etl::span<Data>(struct_data2)
      };

      etl::multi_span<Data> ms_data(span_list.data(), span_list.size());

      etl::multi_span<Data>::iterator itr = ms_data.begin();
      CHECK_EQUAL(struct_data1[0].i, itr->i);
      ++itr;
      CHECK_EQUAL(struct_data1[1].i, itr->i);
      ++itr;
      CHECK_EQUAL(struct_data1[2].i, itr->i);
      ++itr;
      CHECK_EQUAL(struct_data2[0].i, itr->i);
      ++itr;
      CHECK_EQUAL(struct_data2[1].i, itr->i);
      ++itr;
      CHECK_EQUAL(struct_data2[2].i, itr->i);
    }

    //*************************************************************************
    TEST(test_multi_span_with_empty_spans)
    {
      std::vector<etl::span<Data>> span_list =
      {
        etl::span<Data>(),
        etl::span<Data>(struct_data1),
        etl::span<Data>(),
        etl::span<Data>(struct_data2),
        etl::span<Data>()
      };

      etl::multi_span<Data> ms_data(span_list.data(), span_list.size());

      etl::multi_span<Data>::iterator itr = ms_data.begin();

      CHECK_EQUAL(struct_data1[0].i, (*itr).i);
      ++itr;
      CHECK_EQUAL(struct_data1[1].i, (*itr).i);
      ++itr;      
      CHECK_EQUAL(struct_data1[2].i, (*itr).i);
      ++itr;      
      CHECK_EQUAL(struct_data2[0].i, (*itr).i);
      ++itr;      
      CHECK_EQUAL(struct_data2[1].i, (*itr).i);
      ++itr;      
      CHECK_EQUAL(struct_data2[2].i, (*itr).i);
      ++itr;
      CHECK(ETL_NULLPTR == itr.operator->());
    }

    //*************************************************************************
    TEST(test_increment_iterator_read)
    {
      std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      etl::multi_span<const int> ms_int(span_list.data(), span_list.size());
      
      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::iterator exp_itr = expected.begin();

      etl::multi_span<const int>::iterator ms_itr     = ms_int.begin();
      etl::multi_span<const int>::iterator ms_end_itr = ms_int.end();

      while (ms_itr != ms_end_itr)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        ++ms_itr;
        ++exp_itr;
      }
    }

    //*************************************************************************
    TEST(test_increment_iterator_write)
    {
      std::vector<etl::span<int>> span_list = 
      { 
        etl::span<int>(data5),
        etl::span<int>(data6),
        etl::span<int>(),      // Empty span.
        etl::span<int>(data7),
        etl::span<int>(data8)
      };

      etl::multi_span<int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::iterator exp_itr = expected.begin();

      etl::multi_span<int>::iterator ms_itr     = ms_int.begin();
      etl::multi_span<int>::iterator ms_end_itr = ms_int.end();

      while (ms_itr != ms_end_itr)
      {
        //  Fill the multi span
        *ms_itr++ = *exp_itr++;
      }

      ms_itr  = ms_int.begin();
      exp_itr = expected.begin();

      while (ms_itr != ms_end_itr)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        ++ms_itr;
        ++exp_itr;
      }
    }

    //*************************************************************************
    TEST(test_decrement_iterator_read)
    {
      std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      etl::multi_span<const int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::iterator exp_itr = expected.begin() + expected.size() - 1;

      etl::multi_span<const int>::iterator ms_itr = ms_int.begin();
      std::advance(ms_itr, ms_int.size() - 1);

      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        if (i != 0)
        {
          --ms_itr;
          --exp_itr;
        }
      }
    }

    //*************************************************************************
    TEST(test_decrement_iterator_write)
    {
      std::vector<etl::span<int>> span_list = 
      { 
        etl::span<int>(data5),
        etl::span<int>(data6),
        etl::span<int>(),      // Empty span.
        etl::span<int>(data7),
        etl::span<int>(data8)
      };

      etl::multi_span<int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::iterator exp_itr = expected.begin() + expected.size() - 1;

      etl::multi_span<int>::iterator ms_itr = ms_int.begin();
      std::advance(ms_itr, ms_int.size() - 1);

      for (size_t i = 0; i < expected.size(); ++i)
      {
        *ms_itr = *exp_itr;

        if (i != 0)
        {
          --ms_itr;
          --exp_itr;
        }
      }

      ms_itr = ms_int.begin();
      std::advance(ms_itr, ms_int.size() - 1);
      exp_itr = expected.begin() + expected.size() - 1;

      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        if (i != 0)
        {
          --ms_itr;
          --exp_itr;
        }
      }
    }

    //*************************************************************************
    TEST(test_increment_const_iterator_read)
    {
      const std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      const etl::multi_span<const int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::iterator exp_itr = expected.begin();

      etl::multi_span<const int>::const_iterator ms_itr     = ms_int.begin();
      etl::multi_span<const int>::const_iterator ms_end_itr = ms_int.end();

      while (ms_itr != ms_end_itr)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        ++ms_itr;
        ++exp_itr;
      }
    }

    //*************************************************************************
    TEST(test_decrement_const_iterator_read)
    {
      const std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      const etl::multi_span<const int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::iterator exp_itr = expected.begin() + expected.size() - 1;

      etl::multi_span<const int>::const_iterator ms_itr = ms_int.begin();
      std::advance(ms_itr, ms_int.size() - 1);

      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        if (i != 0)
        {
          --ms_itr;
          --exp_itr;
        }
      }
    }

    //*************************************************************************
    TEST(test_reverse_increment_iterator_read)
    {
      using span_type       = etl::span<const int>;
      using multi_span_type = etl::multi_span<const int>;

      std::vector<span_type> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      multi_span_type ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::reverse_iterator exp_itr = expected.rbegin();

      multi_span_type::reverse_iterator ms_itr     = ms_int.rbegin();
      multi_span_type::reverse_iterator ms_end_itr = ms_int.rend();

      while (ms_itr != ms_end_itr)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        ++ms_itr;
        ++exp_itr;
      }
    }

    //*************************************************************************
    TEST(test_reverse_increment_iterator_write)
    {
      using span_type       = etl::span<int>;
      using multi_span_type = etl::multi_span<int>;

      std::vector<span_type> span_list = 
      { 
        span_type(data5),
        span_type(data6),
        span_type(),      // Empty span.
        span_type(data7),
        span_type(data8)
      };

      multi_span_type ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::reverse_iterator exp_itr = expected.rbegin();

      multi_span_type::reverse_iterator ms_itr     = ms_int.rbegin();
      multi_span_type::reverse_iterator ms_end_itr = ms_int.rend();

      while (ms_itr != ms_end_itr)
      {
        *ms_itr++ = *exp_itr++;
      }

      while (ms_itr != ms_end_itr)
      {
        CHECK_EQUAL(*++exp_itr, *++ms_itr);
      }
    }

    //*************************************************************************
    TEST(test_reverse_decrement_iterator_read)
    {
      using multi_span_type = etl::multi_span<const int>;

      std::vector<etl::span<const int>> span_list = 
      {
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      multi_span_type ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::reverse_iterator exp_itr = expected.rbegin() + expected.size() - 1;

      multi_span_type::reverse_iterator ms_itr = ms_int.rbegin();
      std::advance(ms_itr, ms_int.size() - 1);

      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        if (i < expected.size() - 1)
        {
          --ms_itr;
          --exp_itr;
        }
      }
    }

    //*************************************************************************
    TEST(test_reverse_decrement_iterator_write)
    {
      using span_type       = etl::span<int>;
      using multi_span_type = etl::multi_span<int>;

      std::vector<etl::span<int>> span_list = 
      {
        span_type(data5),
        span_type(data6),
        span_type(),      // Empty span.
        span_type(data7),
        span_type(data8)
      };

      multi_span_type ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::reverse_iterator exp_itr = expected.rbegin() + expected.size() - 1;

      multi_span_type::reverse_iterator ms_itr = ms_int.rbegin();
      std::advance(ms_itr, ms_int.size() - 1);

      for (size_t i = 0; i < expected.size(); ++i)
      {
        *ms_itr = *exp_itr;

        if (i < expected.size() - 1)
        {
          --ms_itr;
          --exp_itr;
        }
      }

      ms_itr = ms_int.rbegin();
      std::advance(ms_itr, ms_int.size() - 1);
      exp_itr = expected.rbegin() + expected.size() - 1;

      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        if (i < expected.size() - 1)
        {
          --ms_itr;
          --exp_itr;
        }
      }
    }

    //*************************************************************************
    TEST(test_const_reverse_increment_iterator_read)
    {
      using span_type       = etl::span<const int>;
      using multi_span_type = etl::multi_span<const int>;

      const std::vector<span_type> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      const multi_span_type ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::reverse_iterator exp_itr = expected.rbegin();

      multi_span_type::reverse_iterator ms_itr     = ms_int.rbegin();
      multi_span_type::reverse_iterator ms_end_itr = ms_int.rend();

      while (ms_itr != ms_end_itr)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        ++ms_itr;
        ++exp_itr;
      }
    }

    //*************************************************************************
    TEST(test_const_reverse_decrement_iterator_read)
    {
      using multi_span_type = etl::multi_span<const int>;

      const std::vector<etl::span<const int>> span_list = 
      {
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      const multi_span_type ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      std::vector<int>::reverse_iterator exp_itr = expected.rbegin() + expected.size() - 1;

      multi_span_type::reverse_iterator ms_itr = ms_int.rbegin();
      std::advance(ms_itr, ms_int.size() - 1);

      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(*exp_itr, *ms_itr);

        if (i < expected.size() - 1)
        {
          --ms_itr;
          --exp_itr;
        }
      }
    }

    //*************************************************************************
    TEST(test_index_operator_read)
    {
      const std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1.data(), data1.size() - 1),
        etl::span<const int>(data2.data(), data2.size() - 1),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3.data(), data3.size() - 1),
        etl::span<const int>(data4.data(), data4.size() - 1)
      };

      const etl::multi_span<const int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < expected.size(); ++i)
      {
        CHECK_EQUAL(expected[i], ms_int[i]);
      }
    }

    //*************************************************************************
    TEST(test_index_operator_write)
    {
      const std::vector<etl::span<int>> span_list = 
      { 
        etl::span<int>(data5),
        etl::span<int>(data6),
        etl::span<int>(),      // Empty span.
        etl::span<int>(data7),
        etl::span<int>(data8)
      };

      const etl::multi_span<int> ms_int(span_list.data(), span_list.size());

      std::vector<int> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < expected.size(); ++i)
      {
        ms_int[i] = expected[i];

        CHECK_EQUAL(expected[i], ms_int[i]);
      }
    }
  };
}
