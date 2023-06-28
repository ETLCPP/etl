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

#include "data.h"

#include "etl/array.h"

#include <array>
#include <algorithm>
#include <iterator>
#include <type_traits>

#include "etl/integral_limits.h"

namespace
{
  using Moveable = TestDataM<int>;

  SUITE(test_array)
  {
    static const size_t SIZE = 10UL;

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

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
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

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW({ int d = data.at(data.size()); (void)d; }, etl::array_out_of_range);
    }

    //*************************************************************************
    TEST(test_at_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW({ int d = data.at(data.size()); (void)d; }, etl::array_out_of_range);
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST(test_index_operator_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0UL; i < data.size(); ++i)
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

      CHECK_EQUAL(data.end(), data.data() + SIZE);
    }

    //*************************************************************************
    TEST(test_cbegin)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.cbegin(), data.data());
    }

    //*************************************************************************
    TEST(test_cend)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.cend(), data.data() + SIZE);
    }

    //*************************************************************************
    TEST(test_rbegin)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.rbegin() == Data::reverse_iterator(data.data() + SIZE));
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

      CHECK(data.crbegin() == Data::const_reverse_iterator(data.data() + SIZE));
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
      Data data = { 0 };

      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST(test_size)
    {
      Data data = { 0 };

      CHECK_EQUAL(SIZE, data.size());
    }

    //*************************************************************************
    TEST(test_max_size)
    {
      Data data = { 0 };

      CHECK_EQUAL(SIZE, data.max_size());
    }


    //*************************************************************************
    TEST(test_fill)
    {
      Data data = { 0 };
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

      Data data = { 0 };

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

      Data data = { 0 };
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

      Data data = { 0 };
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

      Data data = { 0 };
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

      Data data = { 0 };
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

    //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST(test_array_template_deduction)
    {
      etl::array data{ char(0), short(1), int(2), long(3), 4, 5, 6, 7, 8, 9 };

      using Type = std::remove_reference_t<decltype(data[0])>;
      CHECK((std::is_same_v<long, Type>));

      CHECK_EQUAL(0, data[0]);
      CHECK_EQUAL(1, data[1]);
      CHECK_EQUAL(2, data[2]);
      CHECK_EQUAL(3, data[3]);
      CHECK_EQUAL(4, data[4]);
      CHECK_EQUAL(5, data[5]);
      CHECK_EQUAL(6, data[6]);
      CHECK_EQUAL(7, data[7]);
      CHECK_EQUAL(8, data[8]);
      CHECK_EQUAL(9, data[9]);
    }
#endif

    //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST
    TEST(test_array_template_deduction_for_movable)
    {
      etl::array data{ Moveable(0), Moveable(1), Moveable(2), Moveable(3), Moveable(4), Moveable(5), Moveable(6), Moveable(7), Moveable(8), Moveable(9) };

      using Type = std::remove_reference_t<decltype(data[0])>;
      CHECK((std::is_same_v<Moveable, Type>));

      CHECK_EQUAL(Moveable(0), data[0]);
      CHECK_EQUAL(Moveable(1), data[1]);
      CHECK_EQUAL(Moveable(2), data[2]);
      CHECK_EQUAL(Moveable(3), data[3]);
      CHECK_EQUAL(Moveable(4), data[4]);
      CHECK_EQUAL(Moveable(5), data[5]);
      CHECK_EQUAL(Moveable(6), data[6]);
      CHECK_EQUAL(Moveable(7), data[7]);
      CHECK_EQUAL(Moveable(8), data[8]);
      CHECK_EQUAL(Moveable(9), data[9]);
    }
#endif

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST(test_make_array)
    {
      auto data = etl::make_array<char>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

      using Type = etl::remove_reference_t<decltype(data[0])>;
      CHECK((std::is_same<char, Type>::value));

      CHECK_EQUAL(0, data[0]);
      CHECK_EQUAL(1, data[1]);
      CHECK_EQUAL(2, data[2]);
      CHECK_EQUAL(3, data[3]);
      CHECK_EQUAL(4, data[4]);
      CHECK_EQUAL(5, data[5]);
      CHECK_EQUAL(6, data[6]);
      CHECK_EQUAL(7, data[7]);
      CHECK_EQUAL(8, data[8]);
      CHECK_EQUAL(9, data[9]);
    }
#endif

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST(test_make_array_for_movable)
    {
      auto data = etl::make_array<Moveable>(Moveable(0), Moveable(1), Moveable(2), Moveable(3), Moveable(4), Moveable(5), Moveable(6), Moveable(7), Moveable(8), Moveable(9));

      using Type = etl::remove_reference_t<decltype(data[0])>;
      CHECK((std::is_same<Moveable, Type>::value));

      CHECK_EQUAL(Moveable(0), data[0]);
      CHECK_EQUAL(Moveable(1), data[1]);
      CHECK_EQUAL(Moveable(2), data[2]);
      CHECK_EQUAL(Moveable(3), data[3]);
      CHECK_EQUAL(Moveable(4), data[4]);
      CHECK_EQUAL(Moveable(5), data[5]);
      CHECK_EQUAL(Moveable(6), data[6]);
      CHECK_EQUAL(Moveable(7), data[7]);
      CHECK_EQUAL(Moveable(8), data[8]);
      CHECK_EQUAL(Moveable(9), data[9]);
    }
#endif

#if ETL_USING_CPP14
    //*************************************************************************
    using Array = etl::array<int, 10U>;

    //*********************************
    constexpr int BeginEnd(const Array& data) noexcept
    {
      return *(data.begin() + 5);
    }

    //*********************************
    constexpr int CBeginCEnd(const Array& data) noexcept
    {
      return *(data.cbegin() + 5);
    }

#if ETL_USING_CPP20 && ETL_USING_STL
    //*********************************
    constexpr int RBeginREnd(const Array& data) noexcept
    {
      return *(data.rbegin() + 5);
    }

    //*********************************
    constexpr int CRBeginCREnd(const Array& data) noexcept
    {
      return *(data.crbegin() + 5);
    }
#endif

    //*********************************
    constexpr int DataSize(const Array& data) noexcept
    {
      return *(data.data() + 5);
    }

    //*********************************
    constexpr Array Fill(int i) noexcept
    {
      Array a{};

      a.fill(i);

      return a;
    }

    //*********************************
#if ETL_USING_CPP20 && ETL_USING_STL
    constexpr Array Swap(Array data1, Array data2) noexcept
    {
      data1.swap(data2);

      return data1;
    }
#endif

    TEST(test_cpp14_constexpr)
    {
      constexpr Array data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      
      // [] operator
      constexpr int i0 = data[0];
      constexpr int i1 = data[1];
      constexpr int i2 = data[2];
      constexpr int i3 = data[3];
      constexpr int i4 = data[4];
      constexpr int i5 = data[5];
      constexpr int i6 = data[6];
      constexpr int i7 = data[7];
      constexpr int i8 = data[8];
      constexpr int i9 = data[9];
      CHECK_EQUAL(data[0], i0);
      CHECK_EQUAL(data[1], i1);
      CHECK_EQUAL(data[2], i2);
      CHECK_EQUAL(data[3], i3);
      CHECK_EQUAL(data[4], i4);
      CHECK_EQUAL(data[5], i5);
      CHECK_EQUAL(data[6], i6);
      CHECK_EQUAL(data[7], i7);
      CHECK_EQUAL(data[8], i8);
      CHECK_EQUAL(data[9], i9);

      // front & back
      constexpr int f0 = data.front();
      constexpr int b9 = data.back();
      CHECK_EQUAL(data[0], f0);
      CHECK_EQUAL(data[9], b9);

      // begin & end
      constexpr int b5 = BeginEnd(data);
      CHECK_EQUAL(data[5], b5);

      // cbegin & cend
      constexpr int cb5 = CBeginCEnd(data);
      CHECK_EQUAL(data[5], cb5);

#if ETL_USING_CPP20 && ETL_USING_STL
      // rbegin & rend
      constexpr int rb5 = RBeginREnd(data);
      CHECK_EQUAL(data[4], rb5);

      // crbegin & crend
      constexpr int crb5 = CRBeginCREnd(data);
      CHECK_EQUAL(data[4], crb5);
#endif

      // data
      constexpr int d5 = DataSize(data);
      CHECK_EQUAL(data[5], d5);

      // empty
      constexpr bool e = data.empty();
      CHECK_FALSE(e);

      // size
      constexpr size_t s = data.size();
      CHECK_EQUAL(data.size(), s);

      // max_size
      constexpr size_t ms = data.max_size();
      CHECK_EQUAL(data.max_size(), ms);

      // fill
      constexpr Array a = Fill(5);
      CHECK_EQUAL(5, a[0]);
      CHECK_EQUAL(5, a[1]);
      CHECK_EQUAL(5, a[2]);
      CHECK_EQUAL(5, a[3]);
      CHECK_EQUAL(5, a[4]);
      CHECK_EQUAL(5, a[5]);
      CHECK_EQUAL(5, a[6]);
      CHECK_EQUAL(5, a[7]);
      CHECK_EQUAL(5, a[8]);
      CHECK_EQUAL(5, a[9]);

#if ETL_USING_CPP20 && ETL_USING_STL
      // swap
      constexpr Array data1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      constexpr Array data2{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
      constexpr Array data3 = Swap(data1, data2);
      CHECK_ARRAY_EQUAL(data2.data(), data3.data(), data2.size());
#endif
    }
#endif
  };
}
