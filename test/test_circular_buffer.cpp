///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2020 jwellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#include "UnitTest++/UnitTest++.h"

#include "etl/circular_buffer.h"
#include "etl/integral_limits.h"

#include "data.h"

#include <algorithm>
#include <iterator>
#include <vector>

namespace
{
  SUITE(test_circular_buffer)
  {
    static const size_t SIZE = 10;

    using Data    = etl::circular_buffer<int, SIZE>;
    using Compare = std::vector<int>;

    using ItemM    = TestDataM<int>;
    using DataM    = etl::circular_buffer<ItemM, SIZE>;
    using CompareM = std::vector<ItemM>;

    //*************************************************************************
    TEST(test_default_constructor)
    {
      Data data;

      CHECK_EQUAL(0U,   data.size());
      CHECK_EQUAL(SIZE, data.max_size());
      CHECK_EQUAL(SIZE, data.capacity());
      CHECK(data.begin()   == data.end());
      CHECK(data.cbegin()  == data.cend());
      CHECK(data.rbegin()  == data.rend());
      CHECK(data.crbegin() == data.crend());
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_constructor_from_literal)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Compare compare = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };


      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = etl::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_from_literal_excess)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      Compare compare = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }
#endif

#if !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED) && ETL_USING_STL
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      Data data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data compare{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_cpp17_deduced_constructor_excess)
    {
      Data data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      Data compare{ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }
#endif

    //*************************************************************************
    TEST(test_push)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      for (auto v : test)
      {
        data.push(v);
      }

      Compare compare{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_move_push)
    {
      DataM data;
      CompareM compare;

      for (uint32_t i = 0; i < SIZE; ++i)
      {
        data.push(ItemM(i));
        compare.push_back(ItemM(i));
      }

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_full_range)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      data.push(test.begin(), test.end());

      Compare compare{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_full_range_reverse_iterator)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      data.push(test.begin(), test.end());

      Compare compare{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_excess_range)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      Data data;
      data.push(test.begin(), test.end());

      Compare compare{ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_excess_range_reverse_iterator)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      Data data;
      data.push(test.begin(), test.end());

      Compare compare{ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_at_start_of_buffer)
    {
      Compare test{ 0, 1, 2, 3, 4 };
      Data data;
      data.push(test.begin(), test.end());

      Compare compare{ 0, 1, 2, 3, 4 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_at_start_of_buffer_reverse_iterator)
    {
      Compare test{ 0, 1, 2, 3, 4 };
      Data data;
      data.push(test.begin(), test.end());

      Compare compare{ 0, 1, 2, 3, 4 };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_at_end_of_buffer)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      Data data;
      data.push(test.begin(), test.end());
      data.pop(5);

      Compare compare{ 6, 7, 8, 9, 10 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_at_end_of_buffer_reverse_iterator)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
      Data data;
      data.push(test.begin(), test.end());
      data.pop(5);

      Compare compare{ 6, 7, 8, 9, 10 };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_middle_of_buffer)
    {
      Compare test1{ 0, 1, 2, 3, 4 };
      Compare test2{ 5, 6, 7 };
      Data data;
      data.push(test1.begin(), test1.end());
      data.pop(3);
      data.push(test2.begin(), test2.end());

      Compare compare{ 3, 4, 5, 6, 7 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_middle_of_buffer_reverse_iterator)
    {
      Compare test1{ 0, 1, 2, 3, 4 };
      Compare test2{ 5, 6, 7 };
      Data data;
      data.push(test1.begin(), test1.end());
      data.pop(3);
      data.push(test2.begin(), test2.end());

      Compare compare{ 3, 4, 5, 6, 7 };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_overlap_start_and_end_of_buffer)
    {
      Compare test1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Compare test2{ 10, 11, 12 };
      Data data;
      data.push(test1.begin(), test1.end());
      data.pop(7);
      data.push(test2.begin(), test2.end());

      Compare compare{ 7, 8, 9, 10, 11, 12 };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_overlap_start_and_end_of_buffer_reverse_iterator)
    {
      Compare test1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Compare test2{ 10, 11, 12 };
      Data data;
      data.push(test1.begin(), test1.end());
      data.pop(7);
      data.push(test2.begin(), test2.end());

      Compare compare{ 7, 8, 9, 10, 11, 12 };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      Compare test1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      data.push(test1.begin(), test1.end());

      Compare compare = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const int& ref = data.front();
      CHECK(ref == compare.front());
    }

    //*************************************************************************
    TEST(test_back_const)
    {
      Compare test1{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      data.push(test1.begin(), test1.end());

      Compare compare = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const int& ref = data.back();
      CHECK(ref == compare.back());
    }

    //*************************************************************************
    TEST(test_random_iterator_plus)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      data.push(test.begin(), test.end());

      Data::const_iterator itr = data.begin();

      itr = itr + 3U;
      CHECK_EQUAL(3U, *itr);

      itr = itr + 3U;
      CHECK_EQUAL(6U, *itr);

      itr = itr + 3U;
      CHECK_EQUAL(9U, *itr);
    }

    //*************************************************************************
    TEST(test_random_iterator_plus_rollover)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      Data data;
      data.push(test.begin(), test.end());

      Data::const_iterator itr = data.begin();

      itr = itr + 3U;
      CHECK_EQUAL(6U, *itr);

      itr = itr + 3U;
      CHECK_EQUAL(9U, *itr);

      itr = itr + 3U;
      CHECK_EQUAL(12U, *itr);
    }

    //*************************************************************************
    TEST(test_random_iterator_plus_equals)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      data.push(test.begin(), test.end());

      Data::const_iterator itr = data.begin();

      itr += 3U;
      CHECK_EQUAL(3U, *itr);

      itr += 3U;
      CHECK_EQUAL(6U, *itr);

      itr += 3U;
      CHECK_EQUAL(9U, *itr);
    }

    //*************************************************************************
    TEST(test_random_iterator_plus_equals_rollover)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      Data data;
      data.push(test.begin(), test.end());

      Data::const_iterator itr = data.begin();

      itr += 3U;
      CHECK_EQUAL(6U, *itr);

      itr += 3U;
      CHECK_EQUAL(9U, *itr);

      itr += 3U;
      CHECK_EQUAL(12U, *itr);
    }

    //*************************************************************************
    TEST(test_random_iterator_minus)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      data.push(test.begin(), test.end());

      Data::const_iterator itr = data.end();

      itr = itr - 3U;
      CHECK_EQUAL(7U, *itr);

      itr = itr - 3U;
      CHECK_EQUAL(4U, *itr);

      itr = itr - 3U;
      CHECK_EQUAL(1U, *itr);
    }

    //*************************************************************************
    TEST(test_random_iterator_minus_rollover)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      Data data;
      data.push(test.begin(), test.end());

      Data::const_iterator itr = data.end();

      itr = itr - 3U;
      CHECK_EQUAL(10U, *itr);

      itr = itr - 3U;
      CHECK_EQUAL(7U, *itr);

      itr = itr - 3U;
      CHECK_EQUAL(4U, *itr);
    }

    //*************************************************************************
    TEST(test_random_iterator_minus_equals)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data;
      data.push(test.begin(), test.end());

      Data::const_iterator itr = data.end();

      itr -= 3U;
      CHECK_EQUAL(7U, *itr);

      itr -= 3U;
      CHECK_EQUAL(4U, *itr);

      itr -= 3U;
      CHECK_EQUAL(1U, *itr);
    }

    //*************************************************************************
    TEST(test_random_iterator_minus_equals_rollover)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
      Data data;
      data.push(test.begin(), test.end());

      Data::const_iterator itr = data.end();

      itr -= 3U;
      CHECK_EQUAL(10U, *itr);

      itr -= 3U;
      CHECK_EQUAL(7U, *itr);

      itr -= 3U;
      CHECK_EQUAL(4U, *itr);
    }

//    ////*************************************************************************
//    //TEST(test_swap)
//    //{
//    //  Data data1 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
//    //  Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//
//    //  swap(data1, data2);
//
//    //  CHECK(std::equal(compare_data.begin(), compare_data.end(), data1.begin()));
//    //  CHECK(std::equal(swap_data.begin(), swap_data.end(), data2.begin()));
//    //}



//    ////*************************************************************************
//    //TEST(test_equal)
//    //{
//    //  Data data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//
//    //  CHECK(data1 == data2);
//    //}
//
//    ////*************************************************************************
//    //TEST(test_not_equal)
//    //{
//    //  Data data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  Data data2 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
//
//    //  CHECK(data1 != data2);
//    //}
//
//    ////*************************************************************************
//    //TEST(test_less_than)
//    //{
//    //  Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
//    //  Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };
//
//    //  CHECK(lesser    < data);
//    //  CHECK(!(data    < data));
//    //  CHECK(!(greater < data));
//    //}
//
//    ////*************************************************************************
//    //TEST(test_less_than_equal)
//    //{
//    //  Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
//    //  Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };
//
//    //  CHECK(lesser    <= data);
//    //  CHECK(data      <= data);
//    //  CHECK(!(greater <= data));
//    //}
//
//    ////*************************************************************************
//    //TEST(test_greater_than)
//    //{
//    //  Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
//    //  Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };
//
//    //  CHECK(greater  > data);
//    //  CHECK(!(data   > data));
//    //  CHECK(!(lesser > data));
//    //}
//
//    ////*************************************************************************
//    //TEST(test_greater_than_equal)
//    //{
//    //  Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
//    //  Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };
//
//    //  CHECK(greater  >= data);
//    //  CHECK(data     >= data);
//    //  CHECK(!(lesser >= data));
//    //}
  };
}
