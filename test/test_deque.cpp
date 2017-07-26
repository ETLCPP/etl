/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include "UnitTest++.h"
#include "ExtraCheckMacros.h"

#include "deque.h"

#include "data.h"

#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cstring>

namespace
{
  SUITE(test_deque)
  {
    const size_t SIZE = 14;

    typedef TestDataDC<std::string>  DC;
    typedef TestDataNDC<std::string> NDC;

    typedef etl::deque<int, SIZE>    DataInt;
    typedef etl::ideque<int>         IDataInt;
    typedef etl::deque<DC, SIZE>     DataDC;
    typedef etl::deque<NDC, SIZE>    DataNDC;
    typedef etl::ideque<NDC>         IDataNDC;

    typedef std::deque<NDC>          Compare_Data;
    typedef std::deque<DC>           Compare_DataDC;

    NDC N0 = NDC("0");
    NDC N1 = NDC("1");
    NDC N2 = NDC("2");
    NDC N3 = NDC("3");
    NDC N4 = NDC("4");
    NDC N5 = NDC("5");
    NDC N6 = NDC("6");
    NDC N7 = NDC("7");
    NDC N8 = NDC("8");
    NDC N9 = NDC("9");
    NDC N10 = NDC("10");
    NDC N11 = NDC("11");
    NDC N12 = NDC("12");
    NDC N13 = NDC("13");
    NDC N14 = NDC("14");
    NDC N15 = NDC("15");
    NDC N16 = NDC("16");
    NDC N17 = NDC("17");
    NDC N999 = NDC("999");

    std::vector<NDC> blank_data = { N999, N999, N999, N999, N999, N999, N999, N999, N999, N999, N999, N999, N999, N999 };
    std::vector<NDC> initial_data = { N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13 };
    std::vector<NDC> initial_data_excess = { N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11, N12, N13, N14 };
    std::vector<NDC> initial_data_under = { N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11 };
    std::vector<NDC> initial_data_small = { N0, N1, N2, N3, N4, N5, N6, N7, N8, N9 };
    std::vector<NDC> insert_data = { N10, N11, N12, N13, N14 };
    std::vector<DC>  initial_data_dc = { DC("0"), DC("1"), DC("2"), DC("3"), DC("4"), DC("5"), DC("6"), DC("7"), DC("8"), DC("9"), DC("10"), DC("11"), DC("12"), DC("13") };
    std::vector<int> int_data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
    std::vector<int> int_data2 = { 15, 16, 17, 18 };


    //*************************************************************************
    TEST(test_constructor)
    {
      DataDC data;

      CHECK_EQUAL(SIZE, data.max_size());
    }

    //*************************************************************************
    TEST(test_constructor_fill)
    {
      Compare_Data compare_data(SIZE, N999);
      DataNDC data(SIZE, N999);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_constructor_fill_excess)
    {
      CHECK_THROW(DataNDC(SIZE + 1, N999), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_constructor_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      DataNDC deque1(initial_data.begin(), initial_data.end());
      DataNDC deque2(deque1);

      CHECK_EQUAL(deque1.size(), deque2.size());
      CHECK(std::equal(deque1.begin(), deque1.end(), deque2.begin()));
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      DataNDC deque1(initial_data.begin(), initial_data.end());
      DataNDC deque2;

      deque2 = deque1;

      CHECK_EQUAL(deque1.size(), deque2.size());
      CHECK(std::equal(deque1.begin(), deque1.end(), deque2.begin()));
    }

    //*************************************************************************
    TEST(test_assignment_interface)
    {
      DataNDC deque1(initial_data.begin(), initial_data.end());
      DataNDC deque2;

      IDataNDC& ideque1 = deque1;
      IDataNDC& ideque2 = deque2;

      ideque2 = ideque1;

      CHECK_EQUAL(deque1.size(), deque2.size());
      CHECK(std::equal(deque1.begin(), deque1.end(), deque2.begin()));
    }

    //*************************************************************************
    TEST(test_self_assignment)
    {
      DataNDC deque1(initial_data.begin(), initial_data.end());
      DataNDC deque2(deque1);

      deque2 = deque2;

      CHECK_EQUAL(deque1.size(), deque2.size());
      CHECK(std::equal(deque1.begin(), deque1.end(), deque2.begin()));
    }

    //*************************************************************************
    TEST(test_assign_range)
    {
      DataNDC data;

      CHECK_NO_THROW(data.assign(initial_data.begin(), initial_data.end()));

      CHECK_EQUAL(initial_data.size(), data.size());
      CHECK(std::equal(initial_data.begin(), initial_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_assign_range_excess)
    {
      DataNDC data;

      CHECK_THROW(data.assign(initial_data_excess.begin(), initial_data_excess.end()), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_assign_fill)
    {
      Compare_Data compare_data;
      DataNDC data;

      compare_data.assign(SIZE, N999);

      CHECK_NO_THROW(data.assign(SIZE, N999));

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_assign_fill_excess)
    {
      DataNDC data;

      CHECK_THROW(data.assign(SIZE + 1, N999), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_at)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

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
      const DataNDC data(initial_data.begin(), initial_data.end());

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
      DataNDC data(initial_data.begin(), initial_data.end());

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
      const DataNDC data(initial_data.begin(), initial_data.end());

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
      DataNDC data;

      data.push_front(N1);
      CHECK_EQUAL(N1, data.front());

      data.push_front(N2);
      CHECK_EQUAL(N2, data.front());

      data.push_front(N3);
      CHECK_EQUAL(N3, data.front());

      data.push_front(N4);
      CHECK_EQUAL(N4, data.front());

      data.push_front(N5);
      CHECK_EQUAL(N5, data.front());

      data.push_front(N6);
      CHECK_EQUAL(N6, data.front());
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      DataNDC data;
      const DataNDC& ctestDeque = data;

      data.push_front(N1);
      CHECK_EQUAL(N1, ctestDeque.front());

      data.push_front(N2);
      CHECK_EQUAL(N2, ctestDeque.front());

      data.push_front(N3);
      CHECK_EQUAL(N3, ctestDeque.front());

      data.push_front(N4);
      CHECK_EQUAL(N4, ctestDeque.front());

      data.push_front(N5);
      CHECK_EQUAL(N5, ctestDeque.front());

      data.push_front(N6);
      CHECK_EQUAL(N6, ctestDeque.front());
    }

    //*************************************************************************
    TEST(test_back)
    {
      DataNDC data;

      data.push_back(N1);
      CHECK_EQUAL(N1, data.back());

      data.push_back(N2);
      CHECK_EQUAL(N2, data.back());

      data.push_back(N3);
      CHECK_EQUAL(N3, data.back());

      data.push_back(N4);
      CHECK_EQUAL(N4, data.back());

      data.push_back(N5);
      CHECK_EQUAL(N5, data.back());

      data.push_back(N6);
      CHECK_EQUAL(N6, data.back());
    }

    //*************************************************************************
    TEST(test_back_const)
    {
      DataNDC data;
      const DataNDC& ctestDeque = data;

      data.push_back(N1);
      CHECK_EQUAL(N1, ctestDeque.back());

      data.push_back(N2);
      CHECK_EQUAL(N2, ctestDeque.back());

      data.push_back(N3);
      CHECK_EQUAL(N3, ctestDeque.back());

      data.push_back(N4);
      CHECK_EQUAL(N4, ctestDeque.back());

      data.push_back(N5);
      CHECK_EQUAL(N5, ctestDeque.back());

      data.push_back(N6);
      CHECK_EQUAL(N6, ctestDeque.back());
    }

    //*************************************************************************
    TEST(test_iterator_comparison)
    {
      DataNDC data(SIZE, N0);

      DataNDC::iterator first = data.begin() + 1;
      DataNDC::iterator second = data.begin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_iterator_comparison)
    {
      DataNDC data(SIZE, N0);

      DataNDC::const_iterator first  = data.cbegin() + 1;
      DataNDC::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_iterator_comparison_rollover_left)
    {
      DataNDC data(SIZE, N0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(N1);
      data.push_front(N1);
      data.push_front(N1);
      data.push_front(N1);

      DataNDC::const_iterator first  = data.cbegin() + 1;
      DataNDC::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_iterator_comparison_rollover_left)
    {
      DataNDC data(SIZE, N0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(N1);
      data.push_front(N1);
      data.push_front(N1);
      data.push_front(N1);

      DataNDC::const_iterator first  = data.cbegin() + 1;
      DataNDC::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_iterator_comparison_rollover_right)
    {
      DataNDC data(SIZE, N0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N1);
      data.push_back(N1);
      data.push_back(N1);
      data.push_back(N1);

      DataNDC::iterator first  = data.begin() + 1;
      DataNDC::iterator second = data.begin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_iterator_comparison_rollover_right)
    {
      DataNDC data(SIZE, N0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N1);
      data.push_back(N1);
      data.push_back(N1);
      data.push_back(N1);

      DataNDC::const_iterator first  = data.cbegin() + 1;
      DataNDC::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_reverse_iterator_comparison)
    {
      DataNDC data(SIZE, N0);

      DataNDC::reverse_iterator first  = data.rbegin() + 1;
      DataNDC::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator_comparison)
    {
      DataNDC data(SIZE, N0);

      DataNDC::const_reverse_iterator first  = data.crbegin() + 1;
      DataNDC::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_reverse_iterator_comparison_rollover_left)
    {
      DataNDC data(SIZE, N0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(N1);
      data.push_front(N1);
      data.push_front(N1);
      data.push_front(N1);

      DataNDC::reverse_iterator first  = data.rbegin() + 1;
      DataNDC::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator_comparison_rollover_left)
    {
      DataNDC data(SIZE, N0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(N1);
      data.push_front(N1);
      data.push_front(N1);
      data.push_front(N1);

      DataNDC::const_reverse_iterator first  = data.crbegin() + 1;
      DataNDC::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_reverse_iterator_comparison_rollover_right)
    {
      DataNDC data(SIZE, N0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N1);
      data.push_back(N1);
      data.push_back(N1);
      data.push_back(N1);

      DataNDC::reverse_iterator first  = data.rbegin() + 1;
      DataNDC::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator_comparison_rollover_right)
    {
      DataNDC data(SIZE, N0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N1);
      data.push_back(N1);
      data.push_back(N1);
      data.push_back(N1);

      DataNDC::const_reverse_iterator first  = data.crbegin() + 1;
      DataNDC::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(test_empty)
    {
      DataNDC data;

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST(test_full)
    {
      DataDC data;
      data.resize(SIZE);

      CHECK(data.full());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      DataDC data;

      data.resize(SIZE);
      data.clear();
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST(test_insert_value_begin)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.insert(compare_data.begin(), N14);
      DataNDC::iterator      position  = data.insert(data.begin(), N14);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(test_emplace_value_begin)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.emplace(compare_data.begin(), N14.value);
      DataNDC::iterator      position  = data.emplace(data.begin(), N14.value);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(test_insert_value_end)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.insert(compare_data.end(), N14);
      DataNDC::iterator      position  = data.insert(data.end(), N14);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(test_emplace_value_end)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.emplace(compare_data.end(), N14.value);
      DataNDC::iterator      position  = data.emplace(data.end(), N14.value);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(test_insert_value)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.insert(compare_data.begin() + 3, N14);
      DataNDC::iterator         position  = data.insert(data.begin() + 3, N14);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));

      compare_data.assign(initial_data_under.begin(), initial_data_under.end());
      data.assign(compare_data.begin(), compare_data.end());

      cposition = compare_data.insert(compare_data.begin() + 4, N14);
      position  = data.insert(data.begin() + 4, N14);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(test_emplace_value)
    {
      Compare_Data compare_data(initial_data_under.begin(), initial_data_under.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      Compare_Data::iterator cposition = compare_data.emplace(compare_data.begin() + 3, N14.value);
      DataNDC::iterator         position = data.emplace(data.begin() + 3, N14.value);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), cposition), std::distance(data.begin(), position));

      compare_data.assign(initial_data_under.begin(), initial_data_under.end());
      data.assign(compare_data.begin(), compare_data.end());

      cposition = compare_data.emplace(compare_data.begin() + 4, N14.value);
      position = data.emplace(data.begin() + 4, N14.value);

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
          DataNDC data(compare_data.begin(), compare_data.end());

          compare_data.insert(compare_data.begin() + offset, insert_size, N14);
          data.insert(data.begin() + offset, insert_size, N14);

          CHECK_EQUAL(compare_data.size(), data.size());
          CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
        }
      }
    }

    //*************************************************************************
    TEST(test_insert_n_value_excess)
    {
      DataNDC data(initial_data_under.begin(), initial_data_under.end());

      size_t insert_size = SIZE - initial_data_under.size() + 1;

      CHECK_THROW(data.insert(data.begin(),     insert_size, N14), etl::deque_full);
      CHECK_THROW(data.insert(data.end(),       insert_size, N14), etl::deque_full);
      CHECK_THROW(data.insert(data.begin() + 6, insert_size, N14), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_insert_range)
    {
      size_t max_insert = SIZE - initial_data_small.size();

      for (size_t insert_size = 1; insert_size <= max_insert; ++insert_size)
      {
        Compare_Data range(insert_data.begin(), insert_data.begin() + insert_size);

        for (size_t offset = 0; offset <= initial_data_small.size(); ++offset)
        {
          Compare_Data compare_data(initial_data_small.begin(), initial_data_small.end());
          DataNDC data(blank_data.begin(), blank_data.end());
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
      Compare_Data range = { N12, N13, N14, N15 };
      DataNDC data(initial_data_under.begin(), initial_data_under.end());

      CHECK_THROW(data.insert(data.begin(),     range.begin(), range.end()), etl::deque_full);
      CHECK_THROW(data.insert(data.end(),       range.begin(), range.end()), etl::deque_full);
      CHECK_THROW(data.insert(data.begin() + 6, range.begin(), range.end()), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_erase_begin)
    {
      Compare_Data compare_data = { N0, N0, N0, N0, N0, N0, N0, N0, N0, N1, N2, N3 };
      DataNDC data(compare_data.begin(), compare_data.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N4);
      data.push_back(N5);
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);
      data.push_back(N9);
      data.push_back(N10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(N4);
      compare_data.push_back(N5);
      compare_data.push_back(N6);
      compare_data.push_back(N7);
      compare_data.push_back(N8);
      compare_data.push_back(N9);
      compare_data.push_back(N10);

      DataNDC::iterator i_next          = data.erase(data.begin());
      Compare_Data::iterator i_cnext = compare_data.erase(compare_data.begin());

      CHECK_EQUAL(DataNDC::difference_type(data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_end)
    {
      Compare_Data compare_data = { N0, N0, N0, N0, N0, N0, N0, N0, N0, N1, N2, N3 };
      DataNDC data(compare_data.begin(), compare_data.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N4);
      data.push_back(N5);
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);
      data.push_back(N9);
      data.push_back(N10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(N4);
      compare_data.push_back(N5);
      compare_data.push_back(N6);
      compare_data.push_back(N7);
      compare_data.push_back(N8);
      compare_data.push_back(N9);
      compare_data.push_back(N10);

      DataNDC::iterator i_erase = data.end() - 1;
      DataNDC::iterator i_next = data.erase(i_erase);

      Compare_Data::iterator i_cerase = compare_data.end() - 1;
      Compare_Data::iterator i_cnext = compare_data.erase(i_cerase);

      CHECK_EQUAL(DataNDC::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_middle)
    {
      std::vector<NDC> initial = { N0, N0, N0, N0, N0, N0, N0, N0, N0, N1, N2, N3 };

      Compare_Data compare_data(initial.begin(), initial.end());
      DataNDC data(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N4);
      data.push_back(N5);
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);
      data.push_back(N9);
      data.push_back(N10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(N4);
      compare_data.push_back(N5);
      compare_data.push_back(N6);
      compare_data.push_back(N7);
      compare_data.push_back(N8);
      compare_data.push_back(N9);
      compare_data.push_back(N10);

      // Erase near beginning.
      DataNDC::iterator i_erase = data.begin() + 2;
      DataNDC::iterator i_next = data.erase(i_erase);

      Compare_Data::iterator i_cerase = compare_data.begin() + 2;
      Compare_Data::iterator i_cnext = compare_data.erase(i_cerase);

      CHECK_EQUAL(DataNDC::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
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
      data.push_back(N4);
      data.push_back(N5);
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);
      data.push_back(N9);
      data.push_back(N10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(N4);
      compare_data.push_back(N5);
      compare_data.push_back(N6);
      compare_data.push_back(N7);
      compare_data.push_back(N8);
      compare_data.push_back(N9);
      compare_data.push_back(N10);

      // Erase near end.
      i_erase = data.begin() + 3;
      i_next = data.erase(i_erase);

      i_cerase = compare_data.begin() + 3;
      i_cnext = compare_data.erase(i_cerase);

      CHECK_EQUAL(DataNDC::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_range_begin)
    {
      std::vector<NDC> initial = { N0, N0, N0, N0, N0, N0, N0, N0, N0, N1, N2, N3 };

      Compare_Data compare_data(initial.begin(), initial.end());
      DataNDC data(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N4);
      data.push_back(N5);
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);
      data.push_back(N9);
      data.push_back(N10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(N4);
      compare_data.push_back(N5);
      compare_data.push_back(N6);
      compare_data.push_back(N7);
      compare_data.push_back(N8);
      compare_data.push_back(N9);
      compare_data.push_back(N10);

      DataNDC::iterator i_next       = data.erase(data.begin(), data.begin() + 3);
      Compare_Data::iterator i_cnext = compare_data.erase(compare_data.begin(), compare_data.begin() + 3);

      CHECK_EQUAL(DataNDC::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_range_end)
    {
      std::vector<NDC> initial = { N0, N0, N0, N0, N0, N0, N0, N0, N0, N1, N2, N3 };

      Compare_Data compare_data(initial.begin(), initial.end());
      DataNDC      data(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N4);
      data.push_back(N5);
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);
      data.push_back(N9);
      data.push_back(N10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(N4);
      compare_data.push_back(N5);
      compare_data.push_back(N6);
      compare_data.push_back(N7);
      compare_data.push_back(N8);
      compare_data.push_back(N9);
      compare_data.push_back(N10);

      DataNDC::iterator i_next = data.erase(data.end() - 3, data.end());
      Compare_Data::iterator i_cnext = compare_data.erase(compare_data.end() - 3, compare_data.end());

      CHECK_EQUAL(DataNDC::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_erase_range_middle)
    {
      std::vector<NDC> initial = { N0, N0, N0, N0, N0, N0, N0, N0, N0, N1, N2, N3 };

      Compare_Data compare_data(initial.begin(), initial.end());
      DataNDC data(initial.begin(), initial.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N4);
      data.push_back(N5);
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);
      data.push_back(N9);
      data.push_back(N10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(N4);
      compare_data.push_back(N5);
      compare_data.push_back(N6);
      compare_data.push_back(N7);
      compare_data.push_back(N8);
      compare_data.push_back(N9);
      compare_data.push_back(N10);

      DataNDC::iterator         i_next  = data.erase(data.begin() + 1, data.begin() + 3);
      Compare_Data::iterator i_cnext = compare_data.erase(compare_data.begin() + 1, compare_data.begin() + 3);

      CHECK_EQUAL(DataNDC::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
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
      data.push_back(N4);
      data.push_back(N5);
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);
      data.push_back(N9);
      data.push_back(N10);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(N4);
      compare_data.push_back(N5);
      compare_data.push_back(N6);
      compare_data.push_back(N7);
      compare_data.push_back(N8);
      compare_data.push_back(N9);
      compare_data.push_back(N10);

      i_next  = data.erase(data.begin() + 3, data.begin() + 5);
      i_cnext = compare_data.erase(compare_data.begin() + 3, compare_data.begin() + 5);

      CHECK_EQUAL(DataNDC::difference_type(compare_data.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_cnext), std::distance(data.begin(), i_next));
    }

    //*************************************************************************
    TEST(test_push_back_null)
    {
      Compare_DataDC compare_data = { DC("1"), DC("2"), DC("3"), DC("4"), DC("5") };
      DataDC data;

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = DC("1"));
      CHECK_EQUAL(size_t(1), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = DC("2"));
      CHECK_EQUAL(size_t(2), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = DC("3"));
      CHECK_EQUAL(size_t(3), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = DC("4"));
      CHECK_EQUAL(size_t(4), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = DC("5"));
      CHECK_EQUAL(size_t(5), data.size());

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_back)
    {
      Compare_Data compare_data = { N1, N2, N3, N4, N5 };
      DataNDC data;

      CHECK_NO_THROW(data.push_back(N1));
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 4, data.begin()));

      CHECK_NO_THROW(data.push_back(N2));
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 3, data.begin()));

      CHECK_NO_THROW(data.push_back(N3));
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 2, data.begin()));

      CHECK_NO_THROW(data.push_back(N4));
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 1, data.begin()));

      CHECK_NO_THROW(data.push_back(N5));
      CHECK_EQUAL(size_t(5), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_emplace_back)
    {
      Compare_Data compare_data = { N1, N2, N3, N4, N5 };
      DataNDC data;

      CHECK_NO_THROW(data.emplace_back("1"));
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 4, data.begin()));

      CHECK_NO_THROW(data.emplace_back("2"));
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 3, data.begin()));

      CHECK_NO_THROW(data.emplace_back("3"));
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 2, data.begin()));

      CHECK_NO_THROW(data.emplace_back("4"));
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 1, data.begin()));

      CHECK_NO_THROW(data.emplace_back("5"));
      CHECK_EQUAL(size_t(5), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_back_excess)
    {
      DataNDC data;

      for (size_t i = 0; i < SIZE; ++i)
      {
        CHECK_NO_THROW(data.push_back(N0));
      }

      CHECK_THROW(data.push_back(N999), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_pop_back)
    {
      Compare_Data compare_data = { N1, N2, N3, N4, N5 };
      DataNDC data;

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
    TEST(test_pop_back_exception)
    {
      Compare_Data compare_data = { N1, N2, N3, N4, N5 };
      DataNDC data;

      data.assign(compare_data.begin(), compare_data.end());

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.pop_back();

      CHECK_THROW(data.pop_back(), etl::deque_empty);
    }

    //*************************************************************************
    TEST(test_push_front_null)
    {
      Compare_DataDC compare_data = { DC("5"), DC("4"), DC("3"), DC("2"), DC("1") };
      DataDC data;

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = DC("1"));
      CHECK_EQUAL(size_t(1), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = DC("2"));
      CHECK_EQUAL(size_t(2), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = DC("3"));
      CHECK_EQUAL(size_t(3), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = DC("4"));
      CHECK_EQUAL(size_t(4), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = DC("5"));
      CHECK_EQUAL(size_t(5), data.size());

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_front)
    {
      Compare_Data compare_data = { N5, N4, N3, N2, N1 };
      DataNDC data;

      CHECK_NO_THROW(data.push_front(N1));
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin() + 4, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(N2));
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin() + 3, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(N3));
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin() + 2, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(N4));
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin() + 1, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(N5));
      CHECK_EQUAL(size_t(5), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_emplace_front)
    {
      Compare_Data compare_data = { N5, N4, N3, N2, N1 };
      DataNDC data;

      CHECK_NO_THROW(data.emplace_front("1"));
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin() + 4, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.emplace_front("2"));
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin() + 3, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.emplace_front("3"));
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin() + 2, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.emplace_front("4"));
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin() + 1, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.emplace_front("5"));
      CHECK_EQUAL(size_t(5), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_front_excess)
    {
      DataNDC data;

      for (size_t i = 0; i < SIZE; ++i)
      {
        CHECK_NO_THROW(data.push_front(N1));
      }

      CHECK_THROW(data.push_front(N999), etl::deque_full);
    }

    //*************************************************************************
    TEST(test_pop_front_exception)
    {
      Compare_Data compare_data = { N1, N2, N3, N4, N5 };
      DataNDC data;

      data.assign(compare_data.begin(), compare_data.end());

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();

      CHECK_THROW(data.pop_front(), etl::deque_empty);
    }

    //*************************************************************************
    TEST(test_push_front_push_back)
    {
      Compare_Data compare_data = { N1, N2, N3, N4, N5};
      DataNDC data;

      CHECK_NO_THROW(data.push_back(N3));
      CHECK_NO_THROW(data.push_front(N2));
      CHECK_NO_THROW(data.push_back(N4));
      CHECK_NO_THROW(data.push_front(N1));
      CHECK_NO_THROW(data.push_back(N5));

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_push_back_pop_front_push_back)
    {
      Compare_Data compare_data = { N6, N7, N8 };
      DataNDC data;

      data.push_back(N1);
      data.push_back(N2);
      data.push_back(N3);
      data.push_back(N4);
      data.push_back(N5);
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(N6);
      data.push_back(N7);
      data.push_back(N8);

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_pop_front)
    {
      Compare_Data compare_data = { N1, N2, N3, N4, N5 };
      DataNDC data;

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
      Compare_DataDC compare_data(initial_data_dc.begin(), initial_data_dc.end());
      DataDC data(initial_data_dc.begin(), initial_data_dc.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(DC("14"));
      data.push_back(DC("15"));
      data.push_back(DC("16"));
      data.push_back(DC("17"));
      data.resize(SIZE);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(DC("14"));
      compare_data.push_back(DC("15"));
      compare_data.push_back(DC("16"));
      compare_data.push_back(DC("17"));
      compare_data.resize(SIZE);

      CHECK_EQUAL(size_t(SIZE), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_resize_down)
    {
      Compare_DataDC compare_data(initial_data_dc.begin(), initial_data_dc.end());
      DataDC data(initial_data_dc.begin(), initial_data_dc.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(DC("14"));
      data.push_back(DC("15"));
      data.push_back(DC("16"));
      data.push_back(DC("17"));
      data.resize(SIZE / 2);

      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.pop_front();
      compare_data.push_back(DC("14"));
      compare_data.push_back(DC("15"));
      compare_data.push_back(DC("16"));
      compare_data.push_back(DC("17"));
      compare_data.resize(SIZE / 2);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_resize_value)
    {
      Compare_Data compare_data = { N1, N2, N3, N3, N3 };
      DataNDC data;

      data.push_front(N1);
      data.push_back(N2);
      data.resize(SIZE, N3);

      CHECK_EQUAL(size_t(SIZE), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_resize_excess)
    {
      DataDC data;

      CHECK_THROW(data.resize(SIZE + 1), etl::deque_out_of_bounds);
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      Compare_Data same      = { N1, N2, N3, N4, N5, N6 };
      Compare_Data different = { N6, N5, N4, N3, N2, N1 };

      DataNDC deque1(same.begin(), same.end());
      DataNDC deque2(deque1);

      CHECK(deque1 == deque2);

      // Change deque2's data.
      std::copy(different.begin(), different.end(), deque2.begin());

      CHECK(!(deque1 == deque2));
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      Compare_Data same      = { N1, N2, N3, N4, N5, N6 };
      Compare_Data different = { N6, N5, N4, N3, N2, N1 };

      DataNDC deque1(same.begin(), same.end());
      DataNDC deque2(deque1);

      CHECK(!(deque1 != deque2));

      // Change deque2's data.
      std::copy(different.begin(), different.end(), deque2.begin());

      CHECK(deque1 != deque2);
    }

    //*************************************************************************
    TEST(test_iterator_comparison_empty)
     {
       DataNDC data;

       CHECK(data.begin()   == data.end());
       CHECK(data.cbegin()  == data.cend());
       CHECK(data.rbegin()  == data.rend());
       CHECK(data.crbegin() == data.crend());
     }

    //*************************************************************************
    TEST(test_memcpy_repair)
    {
      DataInt data(int_data1.begin(), int_data1.end());
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.insert(data.end(), int_data2.begin(), int_data2.end());

      char buffer[sizeof(DataInt)];

      memcpy(&buffer, &data, sizeof(data));

      DataInt& rdata(*reinterpret_cast<DataInt*>(buffer));
      rdata.repair();

      // Check that the memcpy'd vector is the same.
      CHECK_EQUAL(data.size(), rdata.size());
      CHECK(!rdata.empty());
      CHECK(rdata.full());

      bool is_equal = std::equal(rdata.begin(),
                                 rdata.end(),
                                 data.begin());

      CHECK(is_equal);

      // Modify the original and check that the memcpy'd vector is not the same.
      std::reverse(data.begin(), data.end());

      is_equal = std::equal(rdata.begin(),
                            rdata.end(),
                            data.begin());

      CHECK(!is_equal);
    }

    //*************************************************************************
    TEST(test_memcpy_repair_virtual)
    {
      DataInt data(int_data1.begin(), int_data1.end());
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.insert(data.end(), int_data2.begin(), int_data2.end());

      char buffer[sizeof(DataInt)];

      memcpy(&buffer, &data, sizeof(data));

      IDataInt& idata(*reinterpret_cast<DataInt*>(buffer));
      idata.repair();

      // Check that the memcpy'd vector is the same.
      CHECK_EQUAL(data.size(), idata.size());
      CHECK(!idata.empty());
      CHECK(idata.full());

      bool is_equal = std::equal(idata.begin(),
                                 idata.end(),
                                 data.begin());

      CHECK(is_equal);

      // Modify the original and check that the memcpy'd vector is not the same.
      std::reverse(data.begin(), data.end());

      is_equal = std::equal(idata.begin(),
                            idata.end(),
                            data.begin());

      CHECK(!is_equal);
    }
  };
}
