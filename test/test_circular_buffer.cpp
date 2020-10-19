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

      data.push(ItemM(0));


      //CHECK(data.begin() != data.end());
      //CHECK(data.cbegin() != data.cend());
      //CHECK_EQUAL(compare.size(), data.size());

      //bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      //CHECK(isEqual);
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
    TEST(test_copy_constructor)
    {
      Compare test{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data1;
      data1.push(test.begin(), test.end());

      Data data2(data1);
            
      CHECK(data2.begin()  != data2.end());
      CHECK(data2.cbegin() != data2.cend());
      CHECK_EQUAL(data1.size(), data2.size());

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);
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
//
//    ////*************************************************************************
//    //TEST(test_get)
//    //{
//    //  Data data1       = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  const Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//
//    //  CHECK_EQUAL(data1[3], etl::get<3>(data1));
//    //  CHECK_EQUAL(data2[3], etl::get<3>(data2));
//
//    //  // The following line should fail with a compile error.
//    //  //int i = etl::get<11>(data2);
//    //}
//
//    ////*************************************************************************
//    //TEST(test_assign)
//    //{
//    //  int initial[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
//    //  int source[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
//    //  int check1[]  = { 0, 1, 2, 3, 4, -1, -1, -1, -1, -1 };
//    //  int check2[]  = { 0, 1, 2, 3, 4, 99, 99, 99, 99, 99 };
//
//    //  Data data;
//
//    //  // Initial data.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  bool isEqual = std::equal(data.begin(), data.end(), std::begin(initial));
//    //  CHECK(isEqual);
//
//    //  // Assign smaller.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  data.assign(&source[0], &source[5]);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check1));
//    //  CHECK(isEqual);
//
//    //  // Assign smaller + default.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  data.assign(&source[0], &source[5], 99);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check2));
//    //  CHECK(isEqual);
//    //}
//
//    ////*************************************************************************
//    //TEST(test_insert_value)
//    //{
//    //  int initial[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  int check1[]  = { 99, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
//    //  int check2[]  = { 0, 1, 2, 3, 4, 99, 5, 6, 7, 8 };
//    //  int check3[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 99 };
//
//    //  Data data;
//    //  Data::iterator result;
//
//    //  // Insert beginning.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.insert_at(0, 99);
//    //  CHECK_EQUAL(data[0], *result);
//    //  bool isEqual = std::equal(data.begin(), data.end(), std::begin(check1));
//    //  CHECK(isEqual);
//
//    //  // Insert middle.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.insert_at(5, 99);
//    //  CHECK_EQUAL(data[5], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check2));
//    //  CHECK(isEqual);
//
//    //  // Insert end.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.insert_at(9, 99);
//    //  CHECK_EQUAL(data[9], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check3));
//    //  CHECK(isEqual);
//    //}
//
//    ////*************************************************************************
//    //TEST(test_insert_range)
//    //{
//    //  int source1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
//    //  int source2[] = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
//    //  int check1[]  = { 12, 11, 10, 0, 1, 2, 3, 4, 5, 6 };
//    //  int check2[]  = { 0, 1, 2, 3, 12, 11, 10, 4, 5, 6 };
//    //  int check3[]  = { 0, 1, 2, 3, 4, 5, 6, 12, 11, 10 };
//    //  int check4[]  = { 12, 11, 10, 9, 8, 7, 6, 5, 4, 3 };
//    //  int check5[]  = { 0, 1, 2, 3, 12, 11, 10, 9, 8, 7, 6 };
//
//    //  Data data;
//    //  Data::iterator result;
//
//    //  // Insert smaller, beginning.
//    //  data.assign(std::begin(source1), std::end(source1));
//    //  result = data.insert_at(0, &source2[0], &source2[3]);
//    //  CHECK_EQUAL(data[0], *result);
//    //  bool isEqual = std::equal(data.begin(), data.end(), std::begin(check1));
//    //  CHECK(isEqual);
//
//    //  // Insert smaller, middle.
//    //  data.assign(std::begin(source1), std::end(source1));
//    //  result = data.insert_at(4, &source2[0], &source2[3]);
//    //  CHECK_EQUAL(data[4], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check2));
//    //  CHECK(isEqual);
//
//    //  // Insert smaller, end.
//    //  data.assign(std::begin(source1), std::end(source1));
//    //  result = data.insert_at(7, &source2[0], &source2[3]);
//    //  CHECK_EQUAL(data[7], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check3));
//    //  CHECK(isEqual);
//
//    //  // Insert larger, beginning.
//    //  data.assign(std::begin(source1), std::end(source1));
//    //  result = data.insert_at(0, &source2[0], &source2[13]);
//    //  CHECK_EQUAL(data[0], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check4));
//    //  CHECK(isEqual);
//
//    //  // Insert larger, middle.
//    //  data.assign(std::begin(source1), std::end(source1));
//    //  result = data.insert_at(4, &source2[0], &source2[13]);
//    //  CHECK_EQUAL(data[4], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check5));
//    //  CHECK(isEqual);
//    //}
//
//    ////*************************************************************************
//    //TEST(test_erase_single)
//    //{
//    //  int initial[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  int check1a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 9 };
//    //  int check1b[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 99 };
//    //  int check2a[] = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 9 };
//    //  int check2b[] = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 99 };
//    //  int check3a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  int check3b[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 99 };
//
//    //  Data data;
//    //  Data::iterator result;
//
//    //  // Erase beginning.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_at(0);
//    //  CHECK_EQUAL(data[0], *result);
//    //  bool isEqual = std::equal(data.begin(), data.end(), std::begin(check1a));
//    //  CHECK(isEqual);
//
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_at(0, 99);
//    //  CHECK_EQUAL(data[0], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check1b));
//    //  CHECK(isEqual);
//
//    //  // Erase middle.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_at(5);
//    //  CHECK_EQUAL(data[5], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check2a));
//    //  CHECK(isEqual);
//
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_at(5, 99);
//    //  CHECK_EQUAL(data[5], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check2b));
//    //  CHECK(isEqual);
//
//    //  // Erase last.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_at(9);
//    //  CHECK_EQUAL(data[9], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check3a));
//    //  CHECK(isEqual);
//
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_at(9, 99);
//    //  CHECK_EQUAL(data[9], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check3b));
//    //  CHECK(isEqual);
//    //}
//
//    ////*************************************************************************
//    //TEST(test_erase_range)
//    //{
//    //  int initial[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  int check1a[] = { 5, 6, 7, 8, 9, 5, 6, 7, 8, 9 };
//    //  int check1b[] = { 5, 6, 7, 8, 9, 99, 99, 99, 99, 99 };
//    //  int check2a[] = { 0, 1, 7, 8, 9, 5, 6, 7, 8, 9 };
//    //  int check2b[] = { 0, 1, 7, 8, 9, 99, 99, 99, 99, 99 };
//    //  int check3a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    //  int check3b[] = { 0, 1, 2, 3, 4, 99, 99, 99, 99, 99 };
//
//    //  Data data;
//    //  Data::iterator result;
//
//    //  // Erase beginning.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_range(0, 5);
//    //  CHECK_EQUAL(data[0], *result);
//    //  bool isEqual = std::equal(data.begin(), data.end(), std::begin(check1a));
//    //  CHECK(isEqual);
//
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_range(0, 5, 99);
//    //  CHECK_EQUAL(data[0], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check1b));
//    //  CHECK(isEqual);
//
//    //  // Erase middle.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_range(2, 7);
//    //  CHECK_EQUAL(data[2], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check2a));
//    //  CHECK(isEqual);
//
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_range(2, 7, 99);
//    //  CHECK_EQUAL(data[2], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check2b));
//    //  CHECK(isEqual);
//
//    //  // Erase last.
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_range(5, 10);
//    //  CHECK_EQUAL(data[5], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check3a));
//    //  CHECK(isEqual);
//
//    //  data.assign(std::begin(initial), std::end(initial));
//    //  result = data.erase_range(5, 10, 99);
//    //  CHECK_EQUAL(data[5], *result);
//    //  isEqual = std::equal(data.begin(), data.end(), std::begin(check3b));
//    //  CHECK(isEqual);
//    //}
//
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
