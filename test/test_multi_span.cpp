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
#include "etl/multi_span.h"

#include <iterator>
#include <vector>
#include <array>

namespace
{
  const int data1[] = { 0, 1, 2, 3 };
  const int data2[] = { 4, 5, 6 };
  const int data3[] = { 7 };
  const int data4[] = { 8, 9 };

  int data5[4];
  int data6[3];
  int data7[1];
  int data8[2];

  struct Data
  {
    int i;
  };

  Data struct_data1[] = {Data{1}, Data{2}, Data{3}};
  Data struct_data2[] = {Data{4}, Data{5}, Data{6}};

  SUITE(test_multi_span)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      std::vector<etl::span<const int>> span_list = 
      { 
        etl::span<const int>(data1),
        etl::span<const int>(data2),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3),
        etl::span<const int>(data4)
      };

      etl::multi_span<const int> ms_int(etl::multi_span<const int>::span_list_type(std::begin(span_list), std::end(span_list)));

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

      etl::multi_span<const int> ms_int(etl::multi_span<const int>::span_list_type(std::begin(span_list), std::end(span_list)));

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
        etl::span<const int>(data1),
        etl::span<const int>(data2),
        etl::span<const int>(),      // Empty span.
        etl::span<const int>(data3),
        etl::span<const int>(data4)
      };

      etl::multi_span<const int> ms_int(etl::multi_span<const int>::span_list_type(std::begin(span_list), std::end(span_list)));

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

      etl::multi_span<int> ms_int(etl::multi_span<int>::span_list_type(std::begin(span_list), std::end(span_list)));

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

      etl::multi_span<Data> ms_data(etl::multi_span<Data>::span_list_type(std::begin(span_list), std::end(span_list)));

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
  };
}
