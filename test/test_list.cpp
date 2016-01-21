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

#include <UnitTest++/UnitTest++.h>
#include "ExtraCheckMacros.h"

#include "../list.h"

#include "data.h"

#include <algorithm>
#include <array>
#include <list>
#include <vector>

namespace
{
  SUITE(test_list)
  {
    typedef TestDataDC<std::string>  ItemDC;
    typedef TestDataNDC<std::string> ItemNDC;

    const size_t SIZE = 10;

    typedef etl::list<ItemDC, SIZE>  DataDC;
    typedef etl::list<ItemNDC, SIZE> DataNDC;
    typedef etl::ilist<ItemNDC>      IDataNDC;

    typedef std::list<ItemNDC>   CompareData;
    typedef std::vector<ItemNDC> InitialData;

    InitialData unsorted_data;
    InitialData sorted_data;
    InitialData non_unique_data;
    InitialData small_data;

    bool are_equal;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        unsorted_data   = { ItemNDC("1"), ItemNDC("0"), ItemNDC("3"), ItemNDC("2"), ItemNDC("5"), ItemNDC("4"), ItemNDC("7"), ItemNDC("6"), ItemNDC("9"), ItemNDC("8") };
        sorted_data     = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5"), ItemNDC("6"), ItemNDC("7"), ItemNDC("8"), ItemNDC("9") };
        non_unique_data = { ItemNDC("0"), ItemNDC("0"), ItemNDC("1"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("3"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5") };
        small_data      = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5") };
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataNDC data;

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      const size_t INITIAL_SIZE = 4;
      DataDC data(INITIAL_SIZE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      CHECK_THROW(DataDC data(SIZE + 1), etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      const size_t INITIAL_SIZE = 4;
      const ItemNDC INITIAL_VALUE = ItemNDC("1");

      std::array<ItemNDC, INITIAL_SIZE> compare_data = { ItemNDC("1"), ItemNDC("1"), ItemNDC("1"), ItemNDC("1") };

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value_excess)
    {
      CHECK_THROW(DataNDC data(SIZE + 1, ItemNDC("1")), etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(SIZE, data.size());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());

      DataNDC data(sorted_data.begin(), sorted_data.end());
      DataNDC other_data(data);

      CHECK_EQUAL(data.size(), other_data.size());

      CHECK(std::equal(data.begin(), data.end(), other_data.begin()));

      other_data.pop_front();
      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(compare_data.size() - 1, other_data.size());

      other_data.pop_back();
      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(compare_data.size() - 2, other_data.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.crbegin(), data.crend(), compare_data.crbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t INITIAL_SIZE = 4;
      const size_t NEW_SIZE     = 8;

      DataDC data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 4;
      const size_t NEW_SIZE     = 8;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE, VALUE);

      CompareData compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE, VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t INITIAL_SIZE = 4;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE);

      CHECK_THROW(data.resize(data.max_size() + 1, VALUE), etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 4;
      const size_t NEW_SIZE     = 2;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE, VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());
      data.clear();

      CHECK_EQUAL(data.size(), 0);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data;

      // Do it twice. We should only get one copy.
      data.assign(compare_data.begin(), compare_data.end());
      data.assign(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 4;
      const ItemNDC VALUE = ItemNDC("1");

      CompareData compare_data(INITIAL_SIZE, VALUE);
      DataNDC data;

      // Do it twice. We should only get one copy.
      data.assign(INITIAL_SIZE, VALUE);
      data.assign(INITIAL_SIZE, VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data;

      CHECK_THROW(data.assign(data.max_size() + 1, VALUE), etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      const size_t INITIAL_SIZE  = 4;
      const ItemNDC VALUE        = ItemNDC("1");
      const ItemNDC INSERT_VALUE = ItemNDC("2");

      CompareData compare_data(INITIAL_SIZE, VALUE);
      DataNDC data(INITIAL_SIZE, VALUE);

      size_t offset = 2;

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, offset);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert(i_data, INSERT_VALUE);
      compare_data.insert(i_compare_data, INSERT_VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      offset = 0;

      i_data = data.begin();
      std::advance(i_data, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert(i_data, INSERT_VALUE);
      compare_data.insert(i_compare_data, INSERT_VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      i_data     = data.end();
      i_compare_data = compare_data.end();

      data.insert(i_data, VALUE);
      compare_data.insert(i_compare_data, VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      std::vector<ItemNDC> test1 = { ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5") };
      std::vector<ItemNDC> test2 = { ItemNDC("6"), ItemNDC("7"), ItemNDC("8"), ItemNDC("9"), ItemNDC("10") };

      CompareData compare_data(test1.begin(), test1.end());
      DataNDC data(test1.begin(), test1.end());

      compare_data.insert(compare_data.begin(), test2.begin(), test2.end());
      data.insert(data.begin(), test2.begin(), test2.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      compare_data.assign(test1.begin(), test1.end());
      data.assign(test1.begin(), test1.end());

      compare_data.insert(compare_data.end(), test2.begin(), test2.end());
      data.insert(data.end(), test2.begin(), test2.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      compare_data.assign(test1.begin(), test1.end());
      data.assign(test1.begin(), test1.end());

      CompareData::iterator icd = compare_data.begin();
      DataNDC::iterator         id = data.begin();

      std::advance(icd, 3);
      std::advance(id, 3);

      compare_data.insert(icd, test2.begin(), test2.end());
      data.insert(id, test2.begin(), test2.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front)
    {
      CompareData compare_data;
      DataNDC data;

      compare_data.push_front(ItemNDC("1"));
      compare_data.push_front(ItemNDC("2"));
      compare_data.push_front(ItemNDC("3"));
      compare_data.push_front(ItemNDC("4"));
      compare_data.push_front(ItemNDC("5"));
      compare_data.push_front(ItemNDC("6"));

      CHECK_NO_THROW(data.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data.push_front(ItemNDC("4")));
      CHECK_NO_THROW(data.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data.push_front(ItemNDC("6")));

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_excess)
    {
      DataNDC data;

      CHECK_NO_THROW(data.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data.push_front(ItemNDC("4")));
      CHECK_NO_THROW(data.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data.push_front(ItemNDC("6")));
      CHECK_NO_THROW(data.push_front(ItemNDC("7")));
      CHECK_NO_THROW(data.push_front(ItemNDC("8")));
      CHECK_NO_THROW(data.push_front(ItemNDC("9")));
      CHECK_NO_THROW(data.push_front(ItemNDC("10")));

      CHECK_THROW(data.push_front(ItemNDC("11")) , etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_pop_front)
    {
      DataNDC data;

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
        CHECK_NO_THROW(data.push_front(ItemNDC("1")));
        data.pop_front();
      }

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_pop_back)
    {
      DataNDC data;

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
        CHECK_NO_THROW(data.push_front(ItemNDC("1")));
        data.pop_back();
      }

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_pop_front)
    {
      DataNDC data;

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
        CHECK_NO_THROW(data.push_front(ItemNDC("1")));
        data.pop_front();
      }

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_pop_back)
    {
      DataNDC data;

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
        CHECK_NO_THROW(data.push_front(ItemNDC("1")));
        data.pop_back();
      }

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      CompareData compare_data;
      DataNDC data;

      compare_data.push_back(ItemNDC("1"));
      compare_data.push_back(ItemNDC("2"));
      compare_data.push_back(ItemNDC("3"));
      compare_data.push_back(ItemNDC("4"));
      compare_data.push_back(ItemNDC("5"));
      compare_data.push_back(ItemNDC("6"));

      CHECK_NO_THROW(data.push_back(ItemNDC("1")));
      CHECK_NO_THROW(data.push_back(ItemNDC("2")));
      CHECK_NO_THROW(data.push_back(ItemNDC("3")));
      CHECK_NO_THROW(data.push_back(ItemNDC("4")));
      CHECK_NO_THROW(data.push_back(ItemNDC("5")));
      CHECK_NO_THROW(data.push_back(ItemNDC("6")));

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      DataNDC data;

      data.push_back(ItemNDC("1"));
      data.push_back(ItemNDC("2"));
      data.push_back(ItemNDC("3"));
      data.push_back(ItemNDC("4"));
      data.push_back(ItemNDC("5"));
      data.push_back(ItemNDC("6"));
      data.push_back(ItemNDC("7"));
      data.push_back(ItemNDC("8"));
      data.push_back(ItemNDC("9"));
      data.push_back(ItemNDC("10"));

      CHECK_THROW(data.push_back(ItemNDC("11")), etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.pop_back();
      compare_data.pop_back();

      data.pop_back();
      data.pop_back();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, 2);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 2);

      i_compare_data = compare_data.erase(i_compare_data);
      i_data         = data.erase(i_data);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase(compare_data.begin());
      i_data         = data.erase(data.begin());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      are_equal = i_data == data.begin();
      CHECK(are_equal);

      // Move to the last value and erase.
      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, compare_data.size() - 1);
      i_compare_data = compare_data.erase(i_compare_data);

      i_data = data.begin();
      std::advance(i_data, data.size() - 1);
      i_data = data.erase(i_data);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      are_equal = i_data == data.end();
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data_1 = data.begin();
      std::advance(i_data_1, 2);

      DataNDC::iterator i_data_2 = data.begin();
      std::advance(i_data_2, 4);

      CompareData::iterator i_compare_data_1 = compare_data.begin();
      std::advance(i_compare_data_1, 2);

      CompareData::iterator i_compare_data_2 = compare_data.begin();
      std::advance(i_compare_data_2, 4);

      compare_data.erase(i_compare_data_1, i_compare_data_2);

      data.erase(i_data_1, i_data_2);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_all)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());

      data.erase(data.begin(), data.end());

      CHECK(data.empty());

      // Check that it is still in a valid state.
      data.push_back(ItemNDC("1"));
      CHECK(!data.empty());
      CHECK_EQUAL(1, data.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(compare_data.front(), data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      const CompareData compare_data(sorted_data.begin(), sorted_data.end());
      const DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(compare_data.front(), data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(compare_data.back(), data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      const CompareData compare_data(sorted_data.begin(), sorted_data.end());
      const DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(compare_data.back(), data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());
      DataNDC other_data;

      other_data = data;

      CHECK_EQUAL(data.size(), other_data.size());

      are_equal = std::equal(data.begin(), data.end(), other_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_interface)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data1(sorted_data.begin(), sorted_data.end());
      DataNDC data2;

      IDataNDC& idata1 = data1;
      IDataNDC& idata2 = data2;

      idata2 = idata1;

      CHECK_EQUAL(data1.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), data2.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());
      DataNDC other_data = data;

      other_data = other_data;

      CHECK_EQUAL(data.size(), other_data.size());

      are_equal = std::equal(data.begin(), data.end(), other_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique)
    {
      CompareData compare_data(non_unique_data.begin(), non_unique_data.end());
      DataNDC data(non_unique_data.begin(), non_unique_data.end());

      compare_data.unique();
      data.unique();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.remove(ItemNDC("7"));
      data.remove(ItemNDC("7"));

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove_if)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.remove_if(std::bind2nd(std::equal_to<ItemNDC>(), ItemNDC("7")));
      data.remove_if(std::bind2nd(std::equal_to<ItemNDC>(), ItemNDC("7")));

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.reverse();
      data.reverse();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      DataNDC data(unsorted_data.begin(), unsorted_data.end());

      compare_data.sort();
      data.sort();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      DataNDC data(unsorted_data.begin(), unsorted_data.end());

      CompareData::iterator compare_from;
      CompareData::iterator compare_to;

      DataNDC::const_iterator from;
      DataNDC::const_iterator to;

      // Move to the beginning.
      compare_from = compare_data.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.begin();
      compare_data.splice(compare_to, compare_data, compare_from);

      from = data.begin();
      std::advance(from, 4);
      to = data.begin();
      data.move(from, to);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to the end.
      compare_from = compare_data.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.end();
      compare_data.splice(compare_to, compare_data, compare_from);

      from = data.begin();
      std::advance(from, 4);
      to = data.end();
      data.move(from, to);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move nearby.
      compare_from = compare_data.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.begin();
      std::advance(compare_to, 6);
      compare_data.splice(compare_to, compare_data, compare_from);

      from = data.begin();
      std::advance(from, 4);
      to = data.begin();
      std::advance(to, 6);
      data.move(from, to);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to same place.
      compare_from = compare_data.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.begin();
      std::advance(compare_to, 4);
      compare_data.splice(compare_to, compare_data, compare_from);

      from = data.begin();
      std::advance(from, 4);
      to = data.begin();
      std::advance(to, 4);
      data.move(from, to);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_range)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      DataNDC data(unsorted_data.begin(), unsorted_data.end());

      CompareData::iterator compare_begin;
      CompareData::iterator compare_end;
      CompareData::iterator compare_to;

      DataNDC::const_iterator begin;
      DataNDC::const_iterator end;
      DataNDC::const_iterator to;

      // Move to the beginning.
      compare_begin = compare_data.begin();
      std::advance(compare_begin, 3);
      compare_end = compare_begin;
      std::advance(compare_end, 3);
      compare_to = compare_data.begin();
      compare_data.splice(compare_to, compare_data, compare_begin, compare_end);

      begin = data.begin();
      std::advance(begin, 3);
      end = begin;
      std::advance(end, 3);
      to = data.begin();
      data.move(begin, end, to);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to the end.
      compare_begin = compare_data.begin();
      std::advance(compare_begin, 3);
      compare_end = compare_begin;
      std::advance(compare_end, 3);
      compare_to = compare_data.end();
      compare_data.splice(compare_to, compare_data, compare_begin, compare_end);

      begin = data.begin();
      std::advance(begin, 3);
      end = begin;
      std::advance(end, 3);
      to = data.end();
      data.move(begin, end, to);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move nearby.
      compare_begin = compare_data.begin();
      std::advance(compare_begin, 2);
      compare_end = compare_begin;
      std::advance(compare_end, 3);
      compare_to = compare_data.begin();
      std::advance(compare_to, 7);
      compare_data.splice(compare_to, compare_data, compare_begin, compare_end);

      begin = data.begin();
      std::advance(begin, 2);
      end = begin;
      std::advance(end, 3);
      to = data.begin();
      std::advance(to, 7);
      data.move(begin, end, to);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to illegal place.
      begin = data.begin();
      std::advance(begin, 2);
      end = begin;
      std::advance(end, 3);
      to = data.begin();
      std::advance(to, 4);

      CHECK_THROW(data.move(begin, end, to), etl::list_iterator);
    }
  };
}
