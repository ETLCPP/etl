/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "UnitTest++/UnitTest++.h"

#include "etl/array.h"

#include <array>
#include <algorithm>
#include <iterator>

#include "etl/integral_limits.h"

namespace
{
  SUITE(test_array)
  {
    static const size_t SIZE = 10;

    typedef etl::array<int, SIZE> Data;
    typedef std::array<int, SIZE> Compare_Data;

    Compare_Data compare_data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    Compare_Data swap_data    = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    //*************************************************************************
    TEST(test_constructor)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.size(), size_t(SIZE));
      CHECK_EQUAL(data.max_size(), SIZE);
    }

#if !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      etl::array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data compare = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }
#endif

    //*************************************************************************
    TEST(test_assignment)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data other_data;

      other_data = data;

      bool isEqual = std::equal(data.begin(), data.end(), other_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_at)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::array_out_of_range);
    }

    //*************************************************************************
    TEST(test_at_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::array_out_of_range);
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST(test_index_operator_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST(test_front)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int& ref = data.front();
      CHECK(ref == compare_data.front());

      ++ref;
      CHECK(ref == compare_data.front() + 1);
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const int& ref = data.front();
      CHECK(ref == compare_data.front());
    }

    //*************************************************************************
    TEST(test_back)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int& ref = data.back();
      CHECK(ref == compare_data.back());

      ++ref;
      CHECK(ref == compare_data.back() + 1);
    }

    //*************************************************************************
    TEST(test_back_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const int& ref = data.back();
      CHECK(ref == compare_data.back());
    }

    //*************************************************************************
    TEST(test_data)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.begin(), data.end(), data.data());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_data_const)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.begin(), data.end(), data.data());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_begin)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.begin(), &data[0]);
    }

    //*************************************************************************
    TEST(test_end)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.end(), &data[SIZE]);
    }

    //*************************************************************************
    TEST(test_cbegin)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.cbegin(), &data[0]);
    }

    //*************************************************************************
    TEST(test_cend)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.cend(), &data[SIZE]);
    }

    //*************************************************************************
    TEST(test_rbegin)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.rbegin() == Data::reverse_iterator(&data[SIZE]));
    }

    //*************************************************************************
    TEST(test_rend)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.rend() == Data::reverse_iterator(&data[0]));
    }

    //*************************************************************************
    TEST(test_crbegin)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.crbegin() == Data::const_reverse_iterator(&data[SIZE]));
    }

    //*************************************************************************
    TEST(test_crend)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.crend() == Data::const_reverse_iterator(&data[0]));
    }

    //*************************************************************************
    TEST(test_iterator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_iterator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.crbegin(), data.crend(), compare_data.crbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_empty)
    {
      Data data;

      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST(test_size)
    {
      Data data;

      CHECK_EQUAL(SIZE, data.size());
    }

    //*************************************************************************
    TEST(test_max_size)
    {
      Data data;

      CHECK_EQUAL(SIZE, data.max_size());
    }


    //*************************************************************************
    TEST(test_fill)
    {
      Data data;
      data.fill(1);

      Compare_Data compare;
      compare.fill(1);

      bool isEqual = std::equal(data.begin(), data.end(), compare.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_swap)
    {
      Data data1 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
      Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      swap(data1, data2);

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data1.begin()));
      CHECK(std::equal(swap_data.begin(), swap_data.end(), data2.begin()));
    }

    //*************************************************************************
    TEST(test_get)
    {
      Data data1       = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data1[3], etl::get<3>(data1));
      CHECK_EQUAL(data2[3], etl::get<3>(data2));

      // The following line should fail with a compile error.
      //int i = etl::get<11>(data2);
    }

    //*************************************************************************
    TEST(test_assign)
    {
      int initial[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      int source[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      int check1[]  = { 0, 1, 2, 3, 4, -1, -1, -1, -1, -1 };
      int check2[]  = { 0, 1, 2, 3, 4, 99, 99, 99, 99, 99 };

      Data data;

      // Initial data.
      data.assign(std::begin(initial), std::end(initial));
      bool isEqual = std::equal(data.begin(), data.end(), std::begin(initial));
      CHECK(isEqual);

      // Assign smaller.
      data.assign(std::begin(initial), std::end(initial));
      data.assign(&source[0], &source[5]);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check1));
      CHECK(isEqual);

      // Assign smaller + default.
      data.assign(std::begin(initial), std::end(initial));
      data.assign(&source[0], &source[5], 99);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check2));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_insert_value)
    {
      int initial[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      int check1[]  = { 99, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
      int check2[]  = { 0, 1, 2, 3, 4, 99, 5, 6, 7, 8 };
      int check3[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 99 };

      Data data;
      Data::iterator result;

      // Insert beginning.
      data.assign(std::begin(initial), std::end(initial));
      result = data.insert_at(0, 99);
      CHECK_EQUAL(data[0], *result);
      bool isEqual = std::equal(data.begin(), data.end(), std::begin(check1));
      CHECK(isEqual);

      // Insert middle.
      data.assign(std::begin(initial), std::end(initial));
      result = data.insert_at(5, 99);
      CHECK_EQUAL(data[5], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check2));
      CHECK(isEqual);

      // Insert end.
      data.assign(std::begin(initial), std::end(initial));
      result = data.insert_at(9, 99);
      CHECK_EQUAL(data[9], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check3));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_insert_range)
    {
      int source1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      int source2[] = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
      int check1[]  = { 12, 11, 10, 0, 1, 2, 3, 4, 5, 6 };
      int check2[]  = { 0, 1, 2, 3, 12, 11, 10, 4, 5, 6 };
      int check3[]  = { 0, 1, 2, 3, 4, 5, 6, 12, 11, 10 };
      int check4[]  = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3 };
      int check5[]  = { 0, 1, 2, 3, 12, 11, 10, 9, 8, 7, 6 };

      Data data;
      Data::iterator result;

      // Insert smaller, beginning.
      data.assign(std::begin(source1), std::end(source1));
      result = data.insert_at(0, &source2[0], &source2[3]);
      CHECK_EQUAL(data[0], *result);
      bool isEqual = std::equal(data.begin(), data.end(), std::begin(check1));
      CHECK(isEqual);

      // Insert smaller, middle.
      data.assign(std::begin(source1), std::end(source1));
      result = data.insert_at(4, &source2[0], &source2[3]);
      CHECK_EQUAL(data[4], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check2));
      CHECK(isEqual);

      // Insert smaller, end.
      data.assign(std::begin(source1), std::end(source1));
      result = data.insert_at(7, &source2[0], &source2[3]);
      CHECK_EQUAL(data[7], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check3));
      CHECK(isEqual);

      // Insert larger, beginning.
      data.assign(std::begin(source1), std::end(source1));
      result = data.insert_at(0, &source2[0], &source2[13]);
      CHECK_EQUAL(data[0], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check4));
      CHECK(isEqual);

      // Insert larger, middle.
      data.assign(std::begin(source1), std::end(source1));
      result = data.insert_at(4, &source2[0], &source2[13]);
      CHECK_EQUAL(data[4], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check5));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_erase_single)
    {
      int initial[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      int check1a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 9 };
      int check1b[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 99 };
      int check2a[] = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 9 };
      int check2b[] = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 99 };
      int check3a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      int check3b[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 99 };

      Data data;
      Data::iterator result;

      // Erase beginning.
      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_at(0);
      CHECK_EQUAL(data[0], *result);
      bool isEqual = std::equal(data.begin(), data.end(), std::begin(check1a));
      CHECK(isEqual);

      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_at(0, 99);
      CHECK_EQUAL(data[0], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check1b));
      CHECK(isEqual);

      // Erase middle.
      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_at(5);
      CHECK_EQUAL(data[5], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check2a));
      CHECK(isEqual);

      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_at(5, 99);
      CHECK_EQUAL(data[5], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check2b));
      CHECK(isEqual);

      // Erase last.
      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_at(9);
      CHECK_EQUAL(data[9], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check3a));
      CHECK(isEqual);

      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_at(9, 99);
      CHECK_EQUAL(data[9], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check3b));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_erase_range)
    {
      int initial[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      int check1a[] = { 5, 6, 7, 8, 9, 5, 6, 7, 8, 9 };
      int check1b[] = { 5, 6, 7, 8, 9, 99, 99, 99, 99, 99 };
      int check2a[] = { 0, 1, 7, 8, 9, 5, 6, 7, 8, 9 };
      int check2b[] = { 0, 1, 7, 8, 9, 99, 99, 99, 99, 99 };
      int check3a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      int check3b[] = { 0, 1, 2, 3, 4, 99, 99, 99, 99, 99 };

      Data data;
      Data::iterator result;

      // Erase beginning.
      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_range(0, 5);
      CHECK_EQUAL(data[0], *result);
      bool isEqual = std::equal(data.begin(), data.end(), std::begin(check1a));
      CHECK(isEqual);

      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_range(0, 5, 99);
      CHECK_EQUAL(data[0], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check1b));
      CHECK(isEqual);

      // Erase middle.
      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_range(2, 7);
      CHECK_EQUAL(data[2], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check2a));
      CHECK(isEqual);

      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_range(2, 7, 99);
      CHECK_EQUAL(data[2], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check2b));
      CHECK(isEqual);

      // Erase last.
      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_range(5, 10);
      CHECK_EQUAL(data[5], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check3a));
      CHECK(isEqual);

      data.assign(std::begin(initial), std::end(initial));
      result = data.erase_range(5, 10, 99);
      CHECK_EQUAL(data[5], *result);
      isEqual = std::equal(data.begin(), data.end(), std::begin(check3b));
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_equal)
    {
      Data data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data1 == data2);
    }

    //*************************************************************************
    TEST(test_not_equal)
    {
      Data data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data2 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

      CHECK(data1 != data2);
    }

    //*************************************************************************
    TEST(test_less_than)
    {
      Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
      Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      CHECK(lesser    < data);
      CHECK(!(data    < data));
      CHECK(!(greater < data));
    }

    //*************************************************************************
    TEST(test_less_than_equal)
    {
      Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
      Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      CHECK(lesser    <= data);
      CHECK(data      <= data);
      CHECK(!(greater <= data));
    }

    //*************************************************************************
    TEST(test_greater_than)
    {
      Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
      Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      CHECK(greater  > data);
      CHECK(!(data   > data));
      CHECK(!(lesser > data));
    }

    //*************************************************************************
    TEST(test_greater_than_equal)
    {
      Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
      Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      CHECK(greater  >= data);
      CHECK(data     >= data);
      CHECK(!(lesser >= data));
    }
  };
}
