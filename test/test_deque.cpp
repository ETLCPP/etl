/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#include <UnitTest++/UnitTest++.h>

#include "../deque.h"

#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>
#include <numeric>

const size_t SIZE = 14;

typedef etl::deque<int, SIZE> Data;
typedef std::deque<int>       Compare_Data;

std::vector<int> blank_data          = { 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999 };
std::vector<int> initial_data        = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
std::vector<int> initial_data_excess = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
std::vector<int> initial_data_under  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
std::vector<int> initial_data_small  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

namespace
{
	SUITE(test_deque)
	{
    //*************************************************************************
		TEST(test_constructor)
		{
      Data data;

      CHECK_EQUAL(SIZE, data.max_size());
		}

    //*************************************************************************
    TEST(test_constructor_fill)
    {
      Compare_Data compare_data(SIZE, 99);
      Data data(SIZE, 99);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_constructor_fill_excess)
    {
      CHECK_THROW(Data(SIZE + 1, 99), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_constructor_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_constructor_range_excess)
    {
      CHECK_THROW(Data data(initial_data_excess.begin(), initial_data_excess.end()), etl::deque_full);
    }

    //*************************************************************************
		TEST(test_copy_constructor)
		{
      Data deque1(initial_data.begin(), initial_data.end());
      Data deque2(deque1);

      CHECK_EQUAL(deque1.size(), deque2.size());
      CHECK(std::equal(deque1.begin(), deque1.end(), deque2.begin()));
		}

    //*************************************************************************
		TEST(test_assignment)
		{
      Data deque1(initial_data.begin(), initial_data.end());
      Data deque2;

      deque2 = deque1;

      CHECK_EQUAL(deque1.size(), deque2.size());
      CHECK(std::equal(deque1.begin(), deque1.end(), deque2.begin()));
		}

    //*************************************************************************
    TEST(test_self_assignment)
    {
      Data deque1(initial_data.begin(), initial_data.end());
      Data deque2(deque1);

      deque2 = deque2;

      CHECK_EQUAL(deque1.size(), deque2.size());
      CHECK(std::equal(deque1.begin(), deque1.end(), deque2.begin()));
    }

    //*************************************************************************
    TEST(test_assign_range)
    {
      Data data;

      CHECK_NO_THROW(data.assign(initial_data.begin(), initial_data.end()));

      CHECK_EQUAL(initial_data.size(), data.size());
      CHECK(std::equal(initial_data.begin(), initial_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_assign_range_excess)
    {
      Data data;

      CHECK_THROW(data.assign(initial_data_excess.begin(), initial_data_excess.end()), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_assign_fill)
    {
      Compare_Data compare_data;
      Data data;

      compare_data.assign(SIZE, 99);

      CHECK_NO_THROW(data.assign(SIZE, 99));

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_assign_fill_excess)
    {
      Data data;

      CHECK_THROW(data.assign(SIZE + 1, 99), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_at)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(compare_data.at(0), data.at(0));
      CHECK_EQUAL(compare_data.at(1), data.at(1));
      CHECK_EQUAL(compare_data.at(2), data.at(2));
      CHECK_EQUAL(compare_data.at(3), data.at(3));
      CHECK_EQUAL(compare_data.at(4), data.at(4));
      CHECK_EQUAL(compare_data.at(5), data.at(5));
    }

    //*************************************************************************
    TEST(test_at_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(compare_data.at(0), data.at(0));
      CHECK_EQUAL(compare_data.at(1), data.at(1));
      CHECK_EQUAL(compare_data.at(2), data.at(2));
      CHECK_EQUAL(compare_data.at(3), data.at(3));
      CHECK_EQUAL(compare_data.at(4), data.at(4));
      CHECK_EQUAL(compare_data.at(5), data.at(5));
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(compare_data[0], data[0]);
      CHECK_EQUAL(compare_data[1], data[1]);
      CHECK_EQUAL(compare_data[2], data[2]);
      CHECK_EQUAL(compare_data[3], data[3]);
      CHECK_EQUAL(compare_data[4], data[4]);
      CHECK_EQUAL(compare_data[5], data[5]);
    }

    //*************************************************************************
    TEST(test_index_operator_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(compare_data[0], data[0]);
      CHECK_EQUAL(compare_data[1], data[1]);
      CHECK_EQUAL(compare_data[2], data[2]);
      CHECK_EQUAL(compare_data[3], data[3]);
      CHECK_EQUAL(compare_data[4], data[4]);
      CHECK_EQUAL(compare_data[5], data[5]);
    }

    //*************************************************************************
    TEST(test_front)
    {
      Data data;

      data.push_front(1);
      CHECK_EQUAL(1, data.front());

      data.push_front(2);
      CHECK_EQUAL(2, data.front());

      data.push_front(3);
      CHECK_EQUAL(3, data.front());

      data.push_front(4);
      CHECK_EQUAL(4, data.front());

      data.push_front(5);
      CHECK_EQUAL(5, data.front());

      data.push_front(6);
      CHECK_EQUAL(6, data.front());
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      Data data;
      const Data& ctestDeque = data;

      data.push_front(1);
      CHECK_EQUAL(1, ctestDeque.front());

      data.push_front(2);
      CHECK_EQUAL(2, ctestDeque.front());

      data.push_front(3);
      CHECK_EQUAL(3, ctestDeque.front());

      data.push_front(4);
      CHECK_EQUAL(4, ctestDeque.front());

      data.push_front(5);
      CHECK_EQUAL(5, ctestDeque.front());

      data.push_front(6);
      CHECK_EQUAL(6, ctestDeque.front());
    }

    //*************************************************************************
    TEST(test_back)
    {
      Data data;

      data.push_back(1);
      CHECK_EQUAL(1, data.back());

      data.push_back(2);
      CHECK_EQUAL(2, data.back());

      data.push_back(3);
      CHECK_EQUAL(3, data.back());

      data.push_back(4);
      CHECK_EQUAL(4, data.back());

      data.push_back(5);
      CHECK_EQUAL(5, data.back());

      data.push_back(6);
      CHECK_EQUAL(6, data.back());
    }

    //*************************************************************************
    TEST(test_back_const)
    {
      Data data;
      const Data& ctestDeque = data;

      data.push_back(1);
      CHECK_EQUAL(1, ctestDeque.back());

      data.push_back(2);
      CHECK_EQUAL(2, ctestDeque.back());

      data.push_back(3);
      CHECK_EQUAL(3, ctestDeque.back());

      data.push_back(4);
      CHECK_EQUAL(4, ctestDeque.back());

      data.push_back(5);
      CHECK_EQUAL(5, ctestDeque.back());

      data.push_back(6);
      CHECK_EQUAL(6, ctestDeque.back());
    }

    //*************************************************************************
    TEST(test_iterator_comparison)
    {
      Data data(SIZE, 0);

      Data::iterator first = data.begin() + 1;
      Data::iterator second = data.begin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_iterator_comparison)
    {
      Data data(SIZE, 0);

      Data::const_iterator first = data.cbegin() + 1;
      Data::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_iterator_comparison_rollover_left)
    {
      Data data(SIZE, 0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);

      Data::const_iterator first = data.cbegin() + 1;
      Data::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_iterator_comparison_rollover_left)
    {
      Data data(SIZE, 0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);

      Data::const_iterator first = data.cbegin() + 1;
      Data::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_iterator_comparison_rollover_right)
    {
      Data data(SIZE, 0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);

      Data::iterator first = data.begin() + 1;
      Data::iterator second = data.begin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_iterator_comparison_rollover_right)
    {
      Data data(SIZE, 0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);

      Data::const_iterator first = data.cbegin() + 1;
      Data::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_reverse_iterator_comparison)
    {
      Data data(SIZE, 0);

      Data::reverse_iterator first = data.rbegin() + 1;
      Data::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator_comparison)
    {
      Data data(SIZE, 0);

      Data::const_reverse_iterator first = data.crbegin() + 1;
      Data::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_reverse_iterator_comparison_rollover_left)
    {
      Data data(SIZE, 0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);

      Data::reverse_iterator first = data.rbegin() + 1;
      Data::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator_comparison_rollover_left)
    {
      Data data(SIZE, 0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);

      Data::const_reverse_iterator first = data.crbegin() + 1;
      Data::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_reverse_iterator_comparison_rollover_right)
    {
      Data data(SIZE, 0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);

      Data::reverse_iterator first = data.rbegin() + 1;
      Data::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator_comparison_rollover_right)
    {
      Data data(SIZE, 0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);

      Data::const_reverse_iterator first = data.crbegin() + 1;
      Data::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_empty)
    {
      Data data;

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST(test_full)
    {
      Data data;
      data.resize(SIZE);

      CHECK(data.full());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      Data data;

      data.resize(SIZE);
      data.clear();
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST(test_insert_value_begin)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      Data data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.insert(compare_data.begin(), 14);
      Data::iterator         position  = data.insert(data.begin(), 14);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(test_insert_value_end)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      Data data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.insert(compare_data.end(), 14);
      Data::iterator         position = data.insert(data.end(), 14);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(test_insert_value)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      Data data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.insert(compare_data.begin() + 3, 14);
      Data::iterator         position  = data.insert(data.begin() + 3, 14);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));

      compare_data.assign(initial_data_under.begin(), initial_data_under.end());
      data.assign(compare_data.begin(), compare_data.end());

      cposition = compare_data.insert(compare_data.begin() + 4, 14);
      position  = data.insert(data.begin() + 4, 14);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(test_insert_n_value_position)
    {
      size_t max_insert = SIZE - initial_data_small.size();

      for (size_t insert_size = 1; insert_size <= max_insert; ++insert_size)
      {
        for (size_t offset = 0; offset <= initial_data_small.size(); ++offset)
        {
          Compare_Data compare_data(initial_data_small.begin(), initial_data_small.end());
          Data data(compare_data.begin(), compare_data.end());

          compare_data.insert(compare_data.begin() + offset, insert_size, 14);
          data.insert(data.begin() + offset, insert_size, 14);

          bool equal = std::equal(compare_data.begin(), compare_data.end(), data.begin());

          CHECK_EQUAL(compare_data.size(), data.size());
          CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
        }
      }
    }

    //*************************************************************************
    TEST(test_insert_n_value_excess)
    {
      Data data(initial_data_under.begin(), initial_data_under.end());

      size_t insert_size = SIZE - initial_data_under.size() + 1;

      CHECK_THROW(data.insert(data.begin(),     insert_size, 14), etl::deque_full);
      CHECK_THROW(data.insert(data.end(),       insert_size, 14), etl::deque_full);
      CHECK_THROW(data.insert(data.begin() + 6, insert_size, 14), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_insert_range)
    {
      size_t max_insert = SIZE - initial_data_small.size();

      for (size_t insert_size = 1; insert_size <= max_insert; ++insert_size)
      {
        Compare_Data range(insert_size);
        std::iota(range.begin(), range.end(), 10);

        for (size_t offset = 0; offset <= initial_data_small.size(); ++offset)
        {
          Compare_Data compare_data(initial_data_small.begin(), initial_data_small.end());
          Data data(blank_data.begin(), blank_data.end());
          data.assign(compare_data.begin(), compare_data.end());

          compare_data.insert(compare_data.begin() + offset, range.begin(), range.end());
          data.insert(data.begin() + offset, range.begin(), range.end());

          CHECK_EQUAL(compare_data.size(), data.size());
          CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
        }
      }
    }

    //*************************************************************************
    TEST(test_insert_range_excess)
    {
      Compare_Data range = { 12, 13, 14, 15 };
      Data data(initial_data_under.begin(), initial_data_under.end());

      CHECK_THROW(data.insert(data.begin(),     range.begin(), range.end()), etl::deque_full);
      CHECK_THROW(data.insert(data.end(),       range.begin(), range.end()), etl::deque_full);
      CHECK_THROW(data.insert(data.begin() + 6, range.begin(), range.end()), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_erase_begin)
    {
      Compare_Data compare_data = {0, 0,  0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };
      Data data(compare_data.begin(), compare_data.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);
      compare_data.push_back(7);
      compare_data.push_back(8);
      compare_data.push_back(9);
      compare_data.push_back(10);

      Data::iterator i_next          = data.erase(data.begin());
      Compare_Data::iterator i_cnext = compare_data.erase(compare_data.begin());

      CHECK_EQUAL(Data::difference_type(data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_end)
    {
      Compare_Data compare_data = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };
      Data data(compare_data.begin(), compare_data.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);
      compare_data.push_back(7);
      compare_data.push_back(8);
      compare_data.push_back(9);
      compare_data.push_back(10);

      Data::iterator i_erase = data.end() - 1;
      Data::iterator i_next = data.erase(i_erase);

      Compare_Data::iterator i_cerase = compare_data.end() - 1;
      Compare_Data::iterator i_cnext = compare_data.erase(i_cerase);

      CHECK_EQUAL(Data::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_middle)
    {
      std::vector<int> initial = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };

      Compare_Data compare_data(initial.begin(), initial.end());
      Data data(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);
      compare_data.push_back(7);
      compare_data.push_back(8);
      compare_data.push_back(9);
      compare_data.push_back(10);

      // Erase near beginning.
      Data::iterator i_erase = data.begin() + 2;
      Data::iterator i_next = data.erase(i_erase);

      Compare_Data::iterator i_cerase = compare_data.begin() + 2;
      Compare_Data::iterator i_cnext = compare_data.erase(i_cerase);

      CHECK_EQUAL(Data::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));

      compare_data.assign(initial.begin(), initial.end());
      data.assign(initial.begin(), initial.end());

      // Cause rollover.
      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);
      compare_data.push_back(7);
      compare_data.push_back(8);
      compare_data.push_back(9);
      compare_data.push_back(10);

      // Erase near end.
      i_erase = data.begin() + 3;
      i_next = data.erase(i_erase);

      i_cerase = compare_data.begin() + 3;
      i_cnext = compare_data.erase(i_cerase);

      CHECK_EQUAL(Data::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_range_begin)
    {
      std::vector<int> initial = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };

      Compare_Data compare_data(initial.begin(), initial.end());
      Data data(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);
      compare_data.push_back(7);
      compare_data.push_back(8);
      compare_data.push_back(9);
      compare_data.push_back(10);

      Data::iterator i_next          = data.erase(data.begin(), data.begin() + 3);
      Compare_Data::iterator i_cnext = compare_data.erase(compare_data.begin(), compare_data.begin() + 3);

      CHECK_EQUAL(Data::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_range_end)
    {
      std::vector<int> initial = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };

      Compare_Data compare_data(initial.begin(), initial.end());
      Data data(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);
      compare_data.push_back(7);
      compare_data.push_back(8);
      compare_data.push_back(9);
      compare_data.push_back(10);

      Data::iterator i_next = data.erase(data.end() - 3, data.end());
      Compare_Data::iterator i_cnext = compare_data.erase(compare_data.end() - 3, compare_data.end());

      CHECK_EQUAL(Data::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_range_middle)
    {
      std::vector<int> initial = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3 };

      Compare_Data compare_data(initial.begin(), initial.end());
      Data data(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);
      compare_data.push_back(7);
      compare_data.push_back(8);
      compare_data.push_back(9);
      compare_data.push_back(10);

      Data::iterator         i_next  = data.erase(data.begin() + 1, data.begin() + 3);
      Compare_Data::iterator i_cnext = compare_data.erase(compare_data.begin() + 1, compare_data.begin() + 3);

      CHECK_EQUAL(Data::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));

      compare_data.assign(initial.begin(), initial.end());
      data.assign(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);
      compare_data.push_back(7);
      compare_data.push_back(8);
      compare_data.push_back(9);
      compare_data.push_back(10);

      i_next  = data.erase(data.begin() + 3, data.begin() + 5);
      i_cnext = compare_data.erase(compare_data.begin() + 3, compare_data.begin() + 5);

      CHECK_EQUAL(Data::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_push_back_null)
    {
      Compare_Data compare_data = { 1, 2, 3, 4, 5 };
      Data data;

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 1);
      CHECK_EQUAL(size_t(1), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 2);
      CHECK_EQUAL(size_t(2), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 3);
      CHECK_EQUAL(size_t(3), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 4);
      CHECK_EQUAL(size_t(4), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 5);
      CHECK_EQUAL(size_t(5), data.size());

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_back)
    {
      Compare_Data compare_data = { 1, 2, 3, 4, 5};
      Data data;

      CHECK_NO_THROW(data.push_back(1));
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 4, data.begin()));

      CHECK_NO_THROW(data.push_back(2));
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 3, data.begin()));

      CHECK_NO_THROW(data.push_back(3));
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 2, data.begin()));

      CHECK_NO_THROW(data.push_back(4));
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 1, data.begin()));

      CHECK_NO_THROW(data.push_back(5));
      CHECK_EQUAL(size_t(5), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_back_excess)
    {
      Data data;

      for (size_t i = 0; i < SIZE; ++i)
      {
        CHECK_NO_THROW(data.push_back(i));
      }

      CHECK_THROW(data.push_back(999), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_pop_back)
    {
      Compare_Data compare_data = { 1, 2, 3, 4, 5 };
      Data data;

      data.assign(compare_data.begin(), compare_data.end());

      data.pop_back();
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 1, data.begin()));

      data.pop_back();
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 2, data.begin()));

      data.pop_back();
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 3, data.begin()));

      data.pop_back();
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 4, data.begin()));

      data.pop_back();
      CHECK_EQUAL(size_t(0), data.size());
    }

    //*************************************************************************
    TEST(test_push_front_null)
    {
      Compare_Data compare_data = {5,  4, 3, 2, 1 };
      Data data;

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 1);
      CHECK_EQUAL(size_t(1), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 2);
      CHECK_EQUAL(size_t(2), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 3);
      CHECK_EQUAL(size_t(3), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 4);
      CHECK_EQUAL(size_t(4), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 5);
      CHECK_EQUAL(size_t(5), data.size());

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_front)
    {
      Compare_Data compare_data = { 5, 4, 3, 2, 1 };
      Data data;

      CHECK_NO_THROW(data.push_front(1));
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin() + 4, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(2));
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin() + 3, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(3));
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin() + 2, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(4));
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin() + 1, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(5));
      CHECK_EQUAL(size_t(5), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_front_excess)
    {
      Data data;

      for (size_t i = 0; i < SIZE; ++i)
      {
        CHECK_NO_THROW(data.push_front(i));
      }

      CHECK_THROW(data.push_front(999), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_push_front_push_back)
    {
      Compare_Data compare_data = { 1, 2, 3, 4, 5};
      Data data;

      CHECK_NO_THROW(data.push_back(3));
      CHECK_NO_THROW(data.push_front(2));
      CHECK_NO_THROW(data.push_back(4));
      CHECK_NO_THROW(data.push_front(1));
      CHECK_NO_THROW(data.push_back(5));

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_back_pop_front_push_back)
    {
      Compare_Data compare_data = { 6, 7, 8 };
      Data data;

      data.push_back(1);
      data.push_back(2);
      data.push_back(3);
      data.push_back(4);
      data.push_back(5);
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_pop_front)
    {
      Compare_Data compare_data = { 1, 2, 3, 4, 5 };
      Data data;

      data.assign(compare_data.begin(), compare_data.end());

      data.pop_front();
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin() + 1, compare_data.end(), data.begin()));

      data.pop_front();
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin() + 2, compare_data.end(), data.begin()));

      data.pop_front();
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin() + 3, compare_data.end(), data.begin()));

      data.pop_front();
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin() + 4, compare_data.end(), data.begin()));

      data.pop_front();
      CHECK_EQUAL(size_t(0), data.size());
    }

    //*************************************************************************
    TEST(test_resize_up)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(14);
      data.push_back(15);
      data.push_back(16);
      data.push_back(17);
      data.resize(SIZE);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(14);
      compare_data.push_back(15);
      compare_data.push_back(16);
      compare_data.push_back(17);
      compare_data.resize(SIZE);

      CHECK_EQUAL(size_t(SIZE), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_resize_down)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(14);
      data.push_back(15);
      data.push_back(16);
      data.push_back(17);
      data.resize(SIZE / 2);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(14);
      compare_data.push_back(15);
      compare_data.push_back(16);
      compare_data.push_back(17);
      compare_data.resize(SIZE / 2);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_resize_value)
    {
      Compare_Data compare_data = { 1, 2, 3, 3, 3 };
      Data data;

      data.push_front(1);
      data.push_back(2);
      data.resize(SIZE, 3);

      CHECK_EQUAL(size_t(SIZE), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_resize_excess)
    {
      Data data;

      CHECK_THROW(data.resize(SIZE + 1), etl::deque_out_of_bounds);
    }

    //*************************************************************************
		TEST(test_equality_operator)
		{
      Compare_Data same      = { 1, 2, 3, 4, 5, 6 };
      Compare_Data different = { 6, 5, 4, 3, 2, 1 };

      Data deque1(same.begin(), same.end());
      Data deque2(deque1);

      CHECK(deque1 == deque2);

      // Change deque2's data.
      std::copy(different.begin(), different.end(), deque2.begin());

      CHECK(!(deque1 == deque2));
		}

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      Compare_Data same      = { 1, 2, 3, 4, 5, 6 };
      Compare_Data different = { 6, 5, 4, 3, 2, 1 };

      Data deque1(same.begin(), same.end());
      Data deque2(deque1);

      CHECK(!(deque1 != deque2));

      // Change deque2's data.
      std::copy(different.begin(), different.end(), deque2.begin());

      CHECK(deque1 != deque2);
    }
	};
}
