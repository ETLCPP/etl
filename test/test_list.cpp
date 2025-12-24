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

#include "etl/list.h"

#include "data.h"

#include <algorithm>
#include <array>
#include <list>
#include <vector>
#include <functional>

namespace
{
  SUITE(test_list)
  {
    typedef TestDataDC<std::string>  ItemDC;
    typedef TestDataNDC<std::string> ItemNDC;
    typedef TestDataM<uint32_t>      ItemM;

    const size_t SIZE = 10UL;

    typedef etl::list<ItemDC, SIZE>      DataDC;
    typedef etl::list<ItemNDC, SIZE>     DataNDC;
    typedef etl::list<ItemNDC, 2 * SIZE> DataNDC2;
    typedef etl::ilist<ItemNDC>          IDataNDC;

    typedef etl::list<int, SIZE> DataInt;

    typedef etl::list<ItemM, SIZE>  DataM;
    typedef etl::ilist<ItemM>       IDataM;

    typedef std::list<ItemNDC>   CompareData;
    typedef std::vector<ItemNDC> InitialData;

    InitialData stable_sort_data;
    InitialData unsorted_data;
    InitialData sorted_data;
    InitialData non_unique_data;
    InitialData small_data;

    InitialData merge_data0;
    InitialData merge_data1;
    InitialData merge_data2;
    InitialData merge_data3;
    InitialData merge_data4;

    bool are_equal;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        stable_sort_data = { ItemNDC("1", 1), ItemNDC("2", 2), ItemNDC("3", 3), ItemNDC("2", 4), ItemNDC("0", 5), ItemNDC("2", 6), ItemNDC("7", 7), ItemNDC("4", 8), ItemNDC("4", 9), ItemNDC("8", 10) };
        unsorted_data    = { ItemNDC("1"), ItemNDC("0"), ItemNDC("3"), ItemNDC("2"), ItemNDC("5"), ItemNDC("4"), ItemNDC("7"), ItemNDC("6"), ItemNDC("9"), ItemNDC("8") };
        sorted_data      = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5"), ItemNDC("6"), ItemNDC("7"), ItemNDC("8"), ItemNDC("9") };
        non_unique_data  = { ItemNDC("0"), ItemNDC("0"), ItemNDC("1"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("3"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5") };
        small_data       = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5") };

        merge_data0 = { ItemNDC("1"), ItemNDC("1"), ItemNDC("3"), ItemNDC("3"), ItemNDC("5"), ItemNDC("7"), ItemNDC("8") };
        merge_data1 = { ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("3"), ItemNDC("6"), ItemNDC("9"), ItemNDC("9") };
        merge_data2 = { ItemNDC("0"), ItemNDC("2"), ItemNDC("3"), ItemNDC("3"), ItemNDC("6"), ItemNDC("7"), ItemNDC("7") };
        merge_data3 = { ItemNDC("0"), ItemNDC("2"), ItemNDC("3"), ItemNDC("3"), ItemNDC("6"), ItemNDC("7") };
        merge_data4 = { ItemNDC("0"), ItemNDC("2"), ItemNDC("3"), ItemNDC("3"), ItemNDC("6"), ItemNDC("7"), ItemNDC("8"), ItemNDC("9") };
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataNDC data;

      CHECK_EQUAL(data.size(), 0UL);
      CHECK(data.empty());
      CHECK_EQUAL(data.max_size(), SIZE);
      CHECK(data.begin() == data.end());
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      etl::list data{ ItemNDC("1", 1), ItemNDC("2", 2), ItemNDC("3", 3), ItemNDC("2", 4), ItemNDC("0", 5), ItemNDC("2", 6), ItemNDC("7", 7), ItemNDC("4", 8), ItemNDC("4", 9), ItemNDC("8", 10) };
      etl::list< ItemNDC, 10> check = { ItemNDC("1", 1), ItemNDC("2", 2), ItemNDC("3", 3), ItemNDC("2", 4), ItemNDC("0", 5), ItemNDC("2", 6), ItemNDC("7", 7), ItemNDC("4", 8), ItemNDC("4", 9), ItemNDC("8", 10) };

      CHECK(!data.empty());
      CHECK(data.full());
      CHECK(data.begin() != data.end());
      CHECK_EQUAL(0U, data.available());
      CHECK_EQUAL(10U, data.capacity());
      CHECK_EQUAL(10U, data.size());
      CHECK_EQUAL(10U, data.max_size());
      CHECK(data == check);
    }
#endif

    //*************************************************************************
    TEST(test_destruct_via_ilist)
    {
      int current_count = ItemNDC::get_instance_count();

      DataNDC* pdata = new DataNDC(sorted_data.begin(), sorted_data.end());
      CHECK_EQUAL(int(current_count + sorted_data.size()), ItemNDC::get_instance_count());

      IDataNDC* pidata = pdata;
      delete pidata;
      CHECK_EQUAL(current_count, ItemNDC::get_instance_count());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      const size_t INITIAL_SIZE = 4UL;
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
      const size_t INITIAL_SIZE = 4UL;
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

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_initializer_list)
    {
      DataNDC data = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3") };

      CHECK_EQUAL(4U, data.size());

      DataNDC::const_iterator i_item = data.begin();

      CHECK_EQUAL(ItemNDC("0"), *i_item++);
      CHECK_EQUAL(ItemNDC("1"), *i_item++);
      CHECK_EQUAL(ItemNDC("2"), *i_item++);
      CHECK_EQUAL(ItemNDC("3"), *i_item++);
    }
#endif

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
    TEST(test_move_constructor)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data1;
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      DataM data2(std::move(data1));

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      DataM::iterator itr = data2.begin();

      ItemM pr = std::move(*itr++);

      CHECK_EQUAL(1U, pr.value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(4U, (*itr++).value);
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
    TEST_FIXTURE(SetupFixture, test_front_insert_iterator)
    {
      DataInt data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
      DataInt expected = {81, 64, 49, 36, 25, 16, 9, 4, 1, 0};
      DataInt transformed;

      auto squared = [](int value)
      {
        return value * value;
      };

      etl::transform(data.cbegin(), data.cend(), etl::front_inserter(transformed), squared);

      CHECK_EQUAL(expected.size(), transformed.size());

      bool transformed_equals_expected = std::equal(transformed.begin(),
                                                    transformed.end(),
                                                    expected.begin());
      CHECK(transformed_equals_expected);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE     = 8UL;

      DataDC data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE     = 8UL;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE, VALUE);

      CompareData compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE, VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE);

      CHECK_THROW(data.resize(data.max_size() + 1, VALUE), etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE     = 2UL;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE, VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_zero)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE = 0UL;
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
      CHECK_EQUAL(0U, data.size());

      // Do it again to check that clear() didn't screw up the internals.
      data.assign(sorted_data.begin(), sorted_data.end());
      CHECK_EQUAL(SIZE, data.size());
      data.clear();
      CHECK_EQUAL(0UL, data.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear_pod)
    {
      DataInt data(SIZE, 1);

      data.clear();
      CHECK(data.empty());

      // Do it again to check that clear() didn't screw up the internals.
      data.resize(SIZE);
      CHECK_EQUAL(SIZE, data.size());
      data.clear();
      CHECK_EQUAL(0UL, data.size());
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
      const size_t INITIAL_SIZE = 4UL;
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
      const size_t INITIAL_SIZE  = 4UL;
      const ItemNDC VALUE        = ItemNDC("1");
      const ItemNDC INSERT_VALUE = ItemNDC("2");

      CompareData compare_data(INITIAL_SIZE, VALUE);
      DataNDC data(INITIAL_SIZE, VALUE);

      size_t offset = 2UL;

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, offset);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert(i_data, INSERT_VALUE);
      compare_data.insert(i_compare_data, INSERT_VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

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
    TEST_FIXTURE(SetupFixture, test_insert_position_value_move)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data1;
      data1.insert(data1.begin(), std::move(p1));
      data1.insert(data1.begin(), std::move(p2));
      data1.insert(data1.begin(), std::move(p3));
      data1.insert(data1.begin(), std::move(p4));

      CHECK_EQUAL(4U, data1.size());

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      DataM::const_iterator itr = data1.begin();

      CHECK_EQUAL(4U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(1U, (*itr++).value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_position_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE = ItemNDC("1");
      const std::string INSERT_VALUE = "2";

      CompareData compare_data(INITIAL_SIZE, VALUE);
      DataNDC data(INITIAL_SIZE, VALUE);

      //size_t offset = 2UL;

      DataNDC::iterator i_data = data.begin();
      //std::advance(i_data, offset);

      //CompareData::iterator i_compare_data = compare_data.begin();
      //std::advance(i_compare_data, offset);

      data.emplace(i_data, INSERT_VALUE);
      //compare_data.emplace(i_compare_data, INSERT_VALUE);

      //CHECK_EQUAL(compare_data.size(), data.size());

      //are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      //CHECK(are_equal);

      //offset = 0;

      //i_data = data.begin();
      //std::advance(i_data, offset);

      //i_compare_data = compare_data.begin();
      //std::advance(i_compare_data, offset);

      //data.emplace(i_data, INSERT_VALUE);
      //compare_data.emplace(i_compare_data, INSERT_VALUE);

      //are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      //CHECK(are_equal);

      //i_data = data.end();
      //i_compare_data = compare_data.end();

      //data.emplace(i_data, VALUE);
      //compare_data.emplace(i_compare_data, VALUE);

      //are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      //CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_position_default_constructor)
    {
      //std::list<ItemDC> compare_data;
      //DataDC data;

      //size_t offset = 2UL;

      //DataDC::iterator i_data = data.begin();
      //std::advance(i_data, offset);

      //std::list<ItemDC>::iterator i_compare_data = compare_data.begin();
      //std::advance(i_compare_data, offset);

      //data.emplace(i_data);
      //compare_data.emplace(i_compare_data);

      //CHECK_EQUAL(compare_data.size(), data.size());

      //are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      //CHECK(are_equal);

      //offset = 0;

      //i_data = data.begin();
      //std::advance(i_data, offset);

      //i_compare_data = compare_data.begin();
      //std::advance(i_compare_data, offset);

      //data.emplace(i_data);
      //compare_data.emplace(i_compare_data);

      //are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      //CHECK(are_equal);

      //i_data = data.end();
      //i_compare_data = compare_data.end();

      //data.emplace(i_data);
      //compare_data.emplace(i_compare_data);

      //are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      //CHECK(are_equal);
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

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      compare_data.assign(test1.begin(), test1.end());
      data.assign(test1.begin(), test1.end());

      compare_data.insert(compare_data.end(), test2.begin(), test2.end());
      data.insert(data.end(), test2.begin(), test2.end());

      CHECK_EQUAL(compare_data.size(), data.size());

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

      CHECK_EQUAL(compare_data.size(), data.size());

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

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_move)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data;
      data.push_front(std::move(p1));
      data.push_front(std::move(p2));
      data.push_front(std::move(p3));
      data.push_front(std::move(p4));

      CHECK_EQUAL(4U, data.size());

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      DataM::const_iterator itr = data.begin();

      CHECK_EQUAL(4U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(1U, (*itr++).value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_front)
    {
      std::list<ItemDC> compare_data;
      etl::list<ItemDC, 7U> data;

      compare_data.emplace_front();
      compare_data.emplace_front("1");
      compare_data.emplace_front("2");
      compare_data.emplace_front("3");
      compare_data.emplace_front("4");
      compare_data.emplace_front("5");
      compare_data.emplace_front("6");

      CHECK_NO_THROW(data.emplace_front());
      CHECK_NO_THROW(data.emplace_front("1"));
      CHECK_NO_THROW(data.emplace_front("2"));
      CHECK_NO_THROW(data.emplace_front("3"));
      CHECK_NO_THROW(data.emplace_front("4"));
      CHECK_NO_THROW(data.emplace_front("5"));
      CHECK_NO_THROW(data.emplace_front("6"));

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_front_return)
    {
      DataNDC data;

      data.emplace_front("24");
      auto& front = data.emplace_front("42");
      CHECK_EQUAL(front, data.front());
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

      for (size_t i = 0UL; i < 2UL * data.max_size(); ++i)
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

      for (size_t i = 0UL; i < 2UL * data.max_size(); ++i)
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

      for (size_t i = 0UL; i < 2UL * data.max_size(); ++i)
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

      for (size_t i = 0UL; i < 2UL * data.max_size(); ++i)
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

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_move)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data;
      data.push_back(std::move(p1));
      data.push_back(std::move(p2));
      data.push_back(std::move(p3));
      data.push_back(std::move(p4));

      CHECK_EQUAL(4U, data.size());

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      DataM::const_iterator itr = data.begin();

      CHECK_EQUAL(1U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(4U, (*itr++).value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back)
    {
      std::list<ItemDC> compare_data;
      etl::list<ItemDC, 7U> data;

      compare_data.emplace_back();
      compare_data.emplace_back("1");
      compare_data.emplace_back("2");
      compare_data.emplace_back("3");
      compare_data.emplace_back("4");
      compare_data.emplace_back("5");
      compare_data.emplace_back("6");

      CHECK_NO_THROW(data.emplace_back());
      CHECK_NO_THROW(data.emplace_back("1"));
      CHECK_NO_THROW(data.emplace_back("2"));
      CHECK_NO_THROW(data.emplace_back("3"));
      CHECK_NO_THROW(data.emplace_back("4"));
      CHECK_NO_THROW(data.emplace_back("5"));
      CHECK_NO_THROW(data.emplace_back("6"));

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back_return)
    {
      DataNDC data;

      data.emplace_back("24");
      auto& back = data.emplace_back("42");
      CHECK_EQUAL(back, data.back());
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

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_front_exception)
    {
      DataNDC data;

      CHECK_THROW(data.pop_front(), etl::list_empty);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back_exception)
    {
      DataNDC data;

      CHECK_THROW(data.pop_back(), etl::list_empty);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_iterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, 2);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 2);

      i_compare_data = compare_data.erase(i_compare_data);
      i_data         = data.erase(i_data);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase(compare_data.begin());
      i_data         = data.erase(data.begin());

      CHECK_EQUAL(compare_data.size(), data.size());

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

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      are_equal = i_data == data.end();
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_const_iterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::const_iterator i_data = data.begin();
      std::advance(i_data, 2);

      CompareData::const_iterator i_compare_data = compare_data.cbegin();
      std::advance(i_compare_data, 2);

      i_compare_data = compare_data.erase(i_compare_data);
      i_data = data.erase(i_data);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase(compare_data.cbegin());
      i_data = data.erase(data.cbegin());

      CHECK_EQUAL(compare_data.size(), data.size());

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

      CHECK_EQUAL(compare_data.size(), data.size());

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

      DataNDC::const_iterator i_data_1 = data.cbegin();
      std::advance(i_data_1, 2);

      DataNDC::const_iterator i_data_2 = data.cbegin();
      std::advance(i_data_2, 4);

      CompareData::const_iterator i_compare_data_1 = compare_data.begin();
      std::advance(i_compare_data_1, 2);

      CompareData::const_iterator i_compare_data_2 = compare_data.begin();
      std::advance(i_compare_data_2, 4);

      CompareData::iterator citr = compare_data.erase(i_compare_data_1, i_compare_data_2);
      CHECK(citr == i_compare_data_2);

      DataNDC::iterator ditr = data.erase(i_data_1, i_data_2);
      CHECK(ditr == i_data_2);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_all)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator itr = data.erase(data.cbegin(), data.cend());
      CHECK(itr == data.cbegin());
      CHECK(data.empty());

      // Check that it is still in a valid state.
      data.push_back(ItemNDC("1"));
      CHECK(!data.empty());
      CHECK_EQUAL(1U, data.size());
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
      DataNDC data(sorted_data.begin(), sorted_data.end());
      DataNDC other_data;

      other_data = data;

      CHECK_EQUAL(data.size(), other_data.size());

      are_equal = std::equal(data.begin(), data.end(), other_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST(test_move_assignment)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data1;
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      DataM data2;
      data2.push_back(ItemM(5U));
      data2 = std::move(data1);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      DataM::const_iterator itr = data2.begin();

      CHECK_EQUAL(1U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(4U, (*itr++).value);
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
    TEST(test_move_assignment_interface)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data1;
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      DataM data2;
      data2.push_back(ItemM(5U));

      IDataM& idata1 = data1;
      IDataM& idata2 = data2;

      idata2 = std::move(idata1);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      DataM::const_iterator itr = data2.begin();

      CHECK_EQUAL(1U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(4U, (*itr++).value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());
      DataNDC other_data = data;

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_data = other_data;
#include "etl/private/diagnostic_pop.h" 

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

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique_trivial)
    {
      CompareData compare_data;
      DataNDC data;

      compare_data.unique();
      data.unique();

      CHECK_EQUAL(compare_data.size(), data.size());

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

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove_if)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));
      data.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));

      CHECK_EQUAL(compare_data.size(), data.size());

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

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(data.size(), size_t(std::distance(data.begin(), data.end())));

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

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_is_stable)
    {
      CompareData compare_data(stable_sort_data.begin(), stable_sort_data.end());
      DataNDC data(stable_sort_data.begin(), stable_sort_data.end());

      compare_data.sort();
      data.sort();

      CompareData::const_iterator citr = compare_data.begin();
      DataNDC::const_iterator     ditr = data.begin();

      while (ditr != data.end())
      {
        CHECK_EQUAL(citr->index, ditr->index);

        ++citr;
        ++ditr;
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_trivial)
    {
      CompareData compare_data;
      DataNDC data;

      compare_data.sort();
      data.sort();

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_same)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      DataNDC data(unsorted_data.begin(), unsorted_data.end());

      CompareData::iterator compare_from;
      CompareData::iterator compare_to;

      DataNDC::iterator from;
      DataNDC::iterator to;

      // Move to the beginning.
      compare_from = compare_data.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.begin();
      compare_data.splice(compare_to, compare_data, compare_from);

      from = data.begin();
      std::advance(from, 4);
      to = data.begin();
      data.splice(to, data, from);

      CHECK_EQUAL(compare_data.size(), data.size());

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
      data.splice(to, data, from);

      CHECK_EQUAL(compare_data.size(), data.size());

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
      data.splice(to, data, from);

      CHECK_EQUAL(compare_data.size(), data.size());

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
      data.splice(to, data, from);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_different)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      CompareData compare_data2(unsorted_data.begin(), unsorted_data.end());

      DataNDC2 data(unsorted_data.begin(), unsorted_data.end());
      DataNDC2 data2(unsorted_data.begin(), unsorted_data.end());

      CompareData::iterator compare_from;
      CompareData::iterator compare_to;

      DataNDC2::iterator from;
      DataNDC2::iterator to;

      // Move to the beginning.
      compare_from = compare_data2.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.begin();
      compare_data.splice(compare_to, compare_data2, compare_from);

      from = data2.begin();
      std::advance(from, 4);
      to = data.begin();
      data.splice(to, data2, from);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      // Move to the end.
      compare_data.assign(unsorted_data.begin(), unsorted_data.end());
      compare_data2.assign(unsorted_data.begin(), unsorted_data.end());

      data.assign(unsorted_data.begin(), unsorted_data.end());
      data2.assign(unsorted_data.begin(), unsorted_data.end());

      compare_from = compare_data2.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.end();
      compare_data.splice(compare_to, compare_data2, compare_from);

      from = data2.begin();
      std::advance(from, 4);
      to = data.end();
      data.splice(to, data2, from);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      // Move nearby.
      compare_data.assign(unsorted_data.begin(), unsorted_data.end());
      compare_data2.assign(unsorted_data.begin(), unsorted_data.end());

      data.assign(unsorted_data.begin(), unsorted_data.end());
      data2.assign(unsorted_data.begin(), unsorted_data.end());

      compare_from = compare_data2.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.begin();
      std::advance(compare_to, 6);
      compare_data.splice(compare_to, compare_data2, compare_from);

      from = data2.begin();
      std::advance(from, 4);
      to = data.begin();
      std::advance(to, 6);
      data.splice(to, data2, from);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      // Move to same place.
      compare_data.assign(unsorted_data.begin(), unsorted_data.end());
      compare_data2.assign(unsorted_data.begin(), unsorted_data.end());

      data.assign(unsorted_data.begin(), unsorted_data.end());
      data2.assign(unsorted_data.begin(), unsorted_data.end());

      compare_from = compare_data2.begin();
      std::advance(compare_from, 4);
      compare_to = compare_data.begin();
      std::advance(compare_to, 4);
      compare_data.splice(compare_to, compare_data2, compare_from);

      from = data2.begin();
      std::advance(from, 4);
      to = data.begin();
      std::advance(to, 4);
      data.splice(to, data2, from);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_range_same)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      DataNDC data(unsorted_data.begin(), unsorted_data.end());

      CompareData::iterator compare_begin;
      CompareData::iterator compare_end;
      CompareData::iterator compare_to;

      DataNDC::iterator begin;
      DataNDC::iterator end;
      DataNDC::iterator to;

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
      data.splice(to, data, begin, end);

      CHECK_EQUAL(compare_data.size(), data.size());

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
      data.splice(to, data, begin, end);

      CHECK_EQUAL(compare_data.size(), data.size());

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
      data.splice(to, data, begin, end);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to same place.
      begin = data.begin();
      std::advance(begin, 2);
      end = begin;
      std::advance(end, 3);
      to = data.begin();
      std::advance(to, 2);

      DataNDC data2(data);
      data.splice(to, data, begin, end);

      CHECK_EQUAL(data.size(), data2.size());

      are_equal = std::equal(data.begin(), data.end(), data2.begin());
      CHECK(are_equal);

      // Move to illegal place.
      begin = data.begin();
      std::advance(begin, 2);
      end = begin;
      std::advance(end, 3);
      to = data.begin();
      std::advance(to, 4);

      CHECK_THROW(data.splice(to, data, begin, end), etl::list_iterator);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_range_different)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      CompareData compare_data2(unsorted_data.begin(), unsorted_data.end());
      DataNDC2 data(unsorted_data.begin(), unsorted_data.end());
      DataNDC2 data2(unsorted_data.begin(), unsorted_data.end());

      CompareData::iterator compare_begin;
      CompareData::iterator compare_end;
      CompareData::iterator compare_to;

      DataNDC2::iterator begin;
      DataNDC2::iterator end;
      DataNDC2::iterator to;

      // Move to the beginning.
      compare_begin = compare_data2.begin();
      std::advance(compare_begin, 3);
      compare_end = compare_begin;
      std::advance(compare_end, 3);
      compare_to = compare_data.begin();
      compare_data.splice(compare_to, compare_data2, compare_begin, compare_end);

      begin = data2.begin();
      std::advance(begin, 3);
      end = begin;
      std::advance(end, 3);
      to = data.begin();
      data.splice(to, data2, begin, end);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      // Move to the end.
      compare_data.assign(unsorted_data.begin(), unsorted_data.end());
      compare_data2.assign(unsorted_data.begin(), unsorted_data.end());

      data.assign(unsorted_data.begin(), unsorted_data.end());
      data2.assign(unsorted_data.begin(), unsorted_data.end());

      compare_begin = compare_data2.begin();
      std::advance(compare_begin, 3);
      compare_end = compare_begin;
      std::advance(compare_end, 3);
      compare_to = compare_data.end();
      compare_data.splice(compare_to, compare_data2, compare_begin, compare_end);

      begin = data2.begin();
      std::advance(begin, 3);
      end = begin;
      std::advance(end, 3);
      to = data.end();
      data.splice(to, data2, begin, end);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      // Move nearby.
      compare_data.assign(unsorted_data.begin(), unsorted_data.end());
      compare_data2.assign(unsorted_data.begin(), unsorted_data.end());

      data.assign(unsorted_data.begin(), unsorted_data.end());
      data2.assign(unsorted_data.begin(), unsorted_data.end());

      compare_begin = compare_data2.begin();
      std::advance(compare_begin, 2);
      compare_end = compare_begin;
      std::advance(compare_end, 3);
      compare_to = compare_data.begin();
      std::advance(compare_to, 7);
      compare_data.splice(compare_to, compare_data2, compare_begin, compare_end);

      begin = data2.begin();
      std::advance(begin, 2);
      end = begin;
      std::advance(end, 3);
      to = data.begin();
      std::advance(to, 7);
      data.splice(to, data2, begin, end);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_list_different)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      CompareData compare_data2(unsorted_data.begin(), unsorted_data.end());

      DataNDC2 data(unsorted_data.begin(), unsorted_data.end());
      DataNDC2 data2(unsorted_data.begin(), unsorted_data.end());

      CompareData::iterator compare_to;
      DataNDC2::iterator to;

      // Move to the beginning.
      compare_to = compare_data.begin();
      compare_data.splice(compare_to, compare_data2);

      to = data.begin();
      data.splice(to, data2);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      // Move to the end.
      compare_data.assign(unsorted_data.begin(), unsorted_data.end());
      compare_data2.assign(unsorted_data.begin(), unsorted_data.end());

      data.assign(unsorted_data.begin(), unsorted_data.end());
      data2.assign(unsorted_data.begin(), unsorted_data.end());

      compare_to = compare_data.end();
      compare_data.splice(compare_to, compare_data2);

      to = data.end();
      data.splice(to, data2);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      // Move nearby.
      compare_data.assign(unsorted_data.begin(), unsorted_data.end());
      compare_data2.assign(unsorted_data.begin(), unsorted_data.end());

      data.assign(unsorted_data.begin(), unsorted_data.end());
      data2.assign(unsorted_data.begin(), unsorted_data.end());

      compare_to = compare_data.begin();
      std::advance(compare_to, 7);
      compare_data.splice(compare_to, compare_data2);

      to = data.begin();
      std::advance(to, 7);
      data.splice(to, data2);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_1)
    {
      DataNDC2 data0(merge_data0.begin(), merge_data0.end());
      DataNDC2 data1(merge_data1.begin(), merge_data1.end());


      CompareData compare0(merge_data0.begin(), merge_data0.end());
      CompareData compare1(merge_data1.begin(), merge_data1.end());

      data0.merge(data1);
      compare0.merge(compare1);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare0.size(), data0.size());
      CHECK_EQUAL(compare1.size(), data1.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_2)
    {
      DataNDC2 data0(merge_data0.begin(), merge_data0.end());
      DataNDC2 data2(merge_data2.begin(), merge_data2.end());

      CompareData compare0(merge_data0.begin(), merge_data0.end());
      CompareData compare2(merge_data2.begin(), merge_data2.end());

      data0.merge(data2);
      compare0.merge(compare2);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare0.size(), data0.size());
      CHECK_EQUAL(compare2.size(), data2.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_3)
    {
      DataNDC2 data0(merge_data0.begin(), merge_data0.end());
      DataNDC2 data3(merge_data3.begin(), merge_data3.end());

      CompareData compare0(merge_data0.begin(), merge_data0.end());
      CompareData compare3(merge_data3.begin(), merge_data3.end());

      data0.merge(data3);
      compare0.merge(compare3);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare0.size(), data0.size());
      CHECK_EQUAL(compare3.size(), data3.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_4)
    {
      DataNDC2 data0(merge_data0.begin(), merge_data0.end());
      DataNDC2 data4(merge_data4.begin(), merge_data4.end());

      CompareData compare0(merge_data0.begin(), merge_data0.end());
      CompareData compare4(merge_data4.begin(), merge_data4.end());

      data0.merge(data4);
      compare0.merge(compare4);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare0.size(), data0.size());
      CHECK_EQUAL(compare4.size(), data4.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_1_reverse_order)
    {
      DataNDC2 data0(merge_data0.begin(), merge_data0.end());
      DataNDC2 data1(merge_data1.begin(), merge_data1.end());

      data0.reverse();
      data1.reverse();

      CompareData compare0(merge_data0.begin(), merge_data0.end());
      CompareData compare1(merge_data1.begin(), merge_data1.end());

      compare0.reverse();
      compare1.reverse();

      data0.merge(data1, std::greater<ItemNDC>());
      compare0.merge(compare1, std::greater<ItemNDC>());

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare0.size(), data0.size());
      CHECK_EQUAL(compare1.size(), data1.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_exception)
    {
      DataNDC2 data0(unsorted_data.begin(), unsorted_data.end());
      DataNDC2 data1(unsorted_data.begin(), unsorted_data.end());

      CHECK_THROW(data0.merge(data1), etl::list_unsorted);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_move)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      ItemM p5(5U);
      ItemM p6(6U);
      ItemM p7(7U);
      ItemM p8(8U);

      DataM data1;
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      DataM data2;
      data2.push_back(std::move(p5));
      data2.push_back(std::move(p6));
      data2.push_back(std::move(p7));
      data2.push_back(std::move(p8));

      data1.merge(std::move(data2));

      CHECK_EQUAL(8U, data1.size());
      CHECK_EQUAL(0U, data2.size());

      DataM::const_iterator itr = data1.begin();

      CHECK_EQUAL(1U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(4U, (*itr++).value);
      CHECK_EQUAL(5U, (*itr++).value);
      CHECK_EQUAL(6U, (*itr++).value);
      CHECK_EQUAL(7U, (*itr++).value);
      CHECK_EQUAL(8U, (*itr++).value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_move_range_same)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);
      ItemM p5(5U);
      ItemM p6(6U);
      ItemM p7(7U);
      ItemM p8(8U);

      DataM data;
      data.push_back(std::move(p1));
      data.push_back(std::move(p2));
      data.push_back(std::move(p3));
      data.push_back(std::move(p4));
      data.push_back(std::move(p5));
      data.push_back(std::move(p6));
      data.push_back(std::move(p7));
      data.push_back(std::move(p8));

      DataM::iterator begin;
      DataM::iterator end;
      DataM::iterator to;

      // Move nearby.
      begin = data.begin();
      std::advance(begin, 4);
      end = begin;
      std::advance(end, 2);
      to = data.begin();
      std::advance(to, 2);
      data.splice(to, std::move(data), begin, end);

      CHECK_EQUAL(8U, data.size());

      DataM::const_iterator itr = data.begin();

      CHECK_EQUAL(1U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(5U, (*itr++).value);
      CHECK_EQUAL(6U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(4U, (*itr++).value);
      CHECK_EQUAL(7U, (*itr++).value);
      CHECK_EQUAL(8U, (*itr++).value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_move_range_different)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);
      ItemM p5(5U);
      ItemM p6(6U);
      ItemM p7(7U);
      ItemM p8(8U);

      DataM data1;
      data1.push_back(std::move(p1));
      data1.push_back(std::move(p2));
      data1.push_back(std::move(p3));
      data1.push_back(std::move(p4));

      DataM data2;
      data2.push_back(std::move(p5));
      data2.push_back(std::move(p6));
      data2.push_back(std::move(p7));
      data2.push_back(std::move(p8));

      DataM::iterator begin;
      DataM::iterator end;
      DataM::iterator to;

      // Move.
      begin = data2.begin();
      std::advance(begin, 1);
      end = begin;
      std::advance(end, 2);
      to = data1.begin();
      std::advance(to, 2);
      data1.splice(to, std::move(data2), begin, end);

      CHECK_EQUAL(6U, data1.size());
      CHECK_EQUAL(2U, data2.size());

      DataM::const_iterator itr = data1.begin();

      CHECK_EQUAL(1U, (*itr++).value); // 1
      CHECK_EQUAL(2U, (*itr++).value); // 2
      CHECK_EQUAL(6U, (*itr++).value); // 7
      CHECK_EQUAL(7U, (*itr++).value); // 6
      CHECK_EQUAL(3U, (*itr++).value); // 3
      CHECK_EQUAL(4U, (*itr++).value); // 4
    }

    //*************************************************************************
    TEST(test_same_type_non_iterator)
    {
      etl::list<int, 10> l(10, 1);
      CHECK(l.size() == 10);
      l.assign(5, 2);
      CHECK(l.size() == 5);
      l.insert(l.begin(), 5, 3);
      CHECK(l.size() == l.max_size());
    }
    
    //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST(test_forward_list_template_deduction)
    {
      etl::list data{ ItemNDC("A"), ItemNDC("B"), ItemNDC("C"), ItemNDC("D"), ItemNDC("E"), ItemNDC("F") };

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same_v<ItemNDC, Type>));

      decltype(data)::const_iterator itr = data.begin();

      CHECK_EQUAL(ItemNDC("A"), *itr++);
      CHECK_EQUAL(ItemNDC("B"), *itr++);
      CHECK_EQUAL(ItemNDC("C"), *itr++);
      CHECK_EQUAL(ItemNDC("D"), *itr++);
      CHECK_EQUAL(ItemNDC("E"), *itr++);
      CHECK_EQUAL(ItemNDC("F"), *itr++);
    }
#endif

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST(test_make_list)
    {
      auto data = etl::make_list<ItemNDC>(ItemNDC("A"), ItemNDC("B"), ItemNDC("C"), ItemNDC("D"), ItemNDC("E"), ItemNDC("F"));

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same<ItemNDC, Type>::value));

      decltype(data)::const_iterator itr = data.begin();

      CHECK_EQUAL(ItemNDC("A"), *itr++);
      CHECK_EQUAL(ItemNDC("B"), *itr++);
      CHECK_EQUAL(ItemNDC("C"), *itr++);
      CHECK_EQUAL(ItemNDC("D"), *itr++);
      CHECK_EQUAL(ItemNDC("E"), *itr++);
      CHECK_EQUAL(ItemNDC("F"), *itr++);
    }
#endif
  };
}
