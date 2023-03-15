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

#include "etl/forward_list.h"

#include <algorithm>
#include <array>
#include <forward_list>
#include <vector>
#include <string>
#include <list>
#include <functional>

namespace
{
  SUITE(test_forward_list)
  {
    const size_t SIZE = 10UL;

    typedef TestDataDC<std::string>  ItemDC;
    typedef TestDataNDC<std::string> ItemNDC;
    typedef TestDataM<uint32_t>      ItemM;

    typedef etl::forward_list<ItemDC, SIZE>  DataDC;
    typedef etl::forward_list<ItemNDC, SIZE> DataNDC;
    typedef etl::iforward_list<ItemNDC>      IDataNDC;

    typedef etl::forward_list<ItemM, SIZE>  DataM;
    typedef etl::iforward_list<ItemM>       IDataM;

    typedef etl::forward_list<int, SIZE> DataInt;

    typedef std::forward_list<ItemDC> CompareDataDC;
    typedef std::forward_list<ItemNDC> CompareDataNDC;
    typedef std::vector<ItemNDC> InitialDataNDC;

    InitialDataNDC stable_sort_data;
    InitialDataNDC unsorted_data;
    InitialDataNDC sorted_data;
    InitialDataNDC non_unique_data;
    InitialDataNDC small_data;

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
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataDC data;

      CHECK(data.empty());
      CHECK_EQUAL(data.max_size(), SIZE);
      CHECK(data.begin() == data.end());
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      etl::forward_list data{ ItemNDC("1", 1), ItemNDC("2", 2), ItemNDC("3", 3), ItemNDC("2", 4), ItemNDC("0", 5), ItemNDC("2", 6), ItemNDC("7", 7), ItemNDC("4", 8), ItemNDC("4", 9), ItemNDC("8", 10) };
      etl::forward_list< ItemNDC, 10> check = { ItemNDC("1", 1), ItemNDC("2", 2), ItemNDC("3", 3), ItemNDC("2", 4), ItemNDC("0", 5), ItemNDC("2", 6), ItemNDC("7", 7), ItemNDC("4", 8), ItemNDC("4", 9), ItemNDC("8", 10) };

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
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      const size_t INITIAL_SIZE = 4UL;
      DataDC data(INITIAL_SIZE);

      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      CHECK_THROW(DataDC data(SIZE + 1), etl::forward_list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC INITIAL_VALUE("1");

      std::array<ItemNDC, INITIAL_SIZE> compare_data = { ItemNDC("1"), ItemNDC("1"), ItemNDC("1"), ItemNDC("1") };

      DataNDC data(INITIAL_SIZE, INITIAL_VALUE);

      CHECK(!data.empty());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      CompareDataNDC compare(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK(!data.empty());

      are_equal = std::equal(data.begin(), data.end(), compare.begin());

      CHECK(are_equal);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_initializer_list)
    {
      CompareDataNDC compare = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3") };
      DataNDC data = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3") };

      CHECK(!data.empty());

      are_equal = std::equal(data.begin(), data.end(), compare.begin());
      CHECK(are_equal);
    }
#endif

    //*************************************************************************
    TEST(test_destruct_via_iforward_list)
    {
      int current_count = ItemNDC::get_instance_count();

      DataNDC* pdata = new DataNDC(sorted_data.begin(), sorted_data.end());
      CHECK_EQUAL(int(current_count + sorted_data.size()), ItemNDC::get_instance_count());

      IDataNDC* pidata = pdata;
      delete pidata;
      CHECK_EQUAL(current_count, ItemNDC::get_instance_count());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());
      DataNDC other_data(data);

      CHECK(std::equal(data.begin(), data.end(), other_data.begin()));
    }

    //*************************************************************************
    TEST(test_move_constructor)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data1;
      data1.push_front(std::move(p1));
      data1.push_front(std::move(p2));
      data1.push_front(std::move(p3));
      data1.push_front(std::move(p4));

      CHECK(!bool(p1));
      CHECK(!bool(p2));
      CHECK(!bool(p3));
      CHECK(!bool(p4));

      DataM data2(std::move(data1));

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      DataM::const_iterator itr = data2.begin();

      CHECK_EQUAL(4U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(1U, (*itr++).value);
    }

    //*************************************************************************
    TEST(test_move_assignment)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data1;
      data1.push_front(std::move(p1));
      data1.push_front(std::move(p2));
      data1.push_front(std::move(p3));
      data1.push_front(std::move(p4));

      DataM data2;
      data2 = std::move(data1);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      DataM::const_iterator itr = data2.begin();

      CHECK_EQUAL(4U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(1U, (*itr++).value);
    }

    //*************************************************************************
    TEST(test_move_assignment_interface)
    {
      ItemM p1(1U);
      ItemM p2(2U);
      ItemM p3(3U);
      ItemM p4(4U);

      DataM data1;
      data1.push_front(std::move(p1));
      data1.push_front(std::move(p2));
      data1.push_front(std::move(p3));
      data1.push_front(std::move(p4));

      DataM data2;

      IDataM& idata1 = data1;
      IDataM& idata2 = data2;

      idata2 = std::move(idata1);

      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(4U, data2.size());

      DataM::const_iterator itr = data2.begin();

      CHECK_EQUAL(4U, (*itr++).value);
      CHECK_EQUAL(3U, (*itr++).value);
      CHECK_EQUAL(2U, (*itr++).value);
      CHECK_EQUAL(1U, (*itr++).value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      const DataNDC data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE = 8UL;
      const ItemDC VALUE("1");

      DataDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE);

      CompareDataDC compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE     = 8UL;
      const ItemNDC VALUE("1");

      DataNDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE, VALUE);

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE, VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t INITIAL_SIZE = 4UL;
      DataDC data(INITIAL_SIZE);

      CHECK_THROW(data.resize(data.max_size() + 1), etl::forward_list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE = 2UL;
      const ItemDC VALUE("1");

      DataDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE);

      CompareDataDC compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE = 2UL;
      const ItemNDC VALUE("1");

      DataNDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE, VALUE);

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE, VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_zero)
    {
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE = 0UL;
      const ItemDC VALUE("1");

      DataDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE);

      CompareDataDC compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());
      data.clear();
      CHECK(data.empty());

      // Do it again to check that clear() didn't screw up the internals.
      data.assign(sorted_data.begin(), sorted_data.end());
      CHECK_EQUAL(SIZE, data.size());
      data.clear();
      CHECK_EQUAL(size_t(0UL), data.size());
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
      CHECK_EQUAL(size_t(0UL), data.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data;

      // Do it twice. We should only get one copy.
      data.assign(compare_data.begin(), compare_data.end());
      data.assign(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE("1");

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      DataNDC data;

      // Do it twice. We should only get one copy.
      data.assign(INITIAL_SIZE, VALUE);
      data.assign(INITIAL_SIZE, VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const ItemNDC VALUE("1");

      DataNDC data;

      CHECK_THROW(data.assign(data.max_size() + 1, VALUE), etl::forward_list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_position_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE("1");
      const ItemNDC INSERT_VALUE("2");

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      DataNDC data(INITIAL_SIZE, VALUE);

      size_t offset = 2UL;

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, offset);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert_after(i_data, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, INSERT_VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      offset = 0;

      i_data = data.begin();
      std::advance(i_data, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert_after(i_data, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, INSERT_VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_position_size_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE("1");
      const ItemNDC INSERT_VALUE("2");

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      DataNDC data(INITIAL_SIZE, VALUE);

      size_t offset = 2UL;

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, offset);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert_after(i_data, 2, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, 2, INSERT_VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      offset = 0;

      i_data = data.begin();
      std::advance(i_data, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert_after(i_data, 2, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, 2, INSERT_VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_range)
    {
      std::vector<ItemNDC> test1 = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4") };
      std::vector<ItemNDC> test2 = { ItemNDC("5"), ItemNDC("6"), ItemNDC("7"), ItemNDC("8"), ItemNDC("9") };

      CompareDataNDC compare_data(test1.begin(), test1.end());
      DataNDC data(test1.begin(), test1.end());

      compare_data.insert_after(compare_data.before_begin(), test2.begin(), test2.end());
      data.insert_after(data.before_begin(), test2.begin(), test2.end());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      compare_data.assign(test1.begin(), test1.end());
      data.assign(test1.begin(), test1.end());

      CompareDataNDC::iterator icd = compare_data.begin();
      DataNDC::iterator         id = data.begin();

      std::advance(icd, 3);
      std::advance(id, 3);

      compare_data.insert_after(icd, test2.begin(), test2.end());
      data.insert_after(id, test2.begin(), test2.end());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front)
    {
      CompareDataNDC compare_data;
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

      CHECK_EQUAL(6U, data.size());
      CHECK_EQUAL(6, std::distance(data.begin(), data.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

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
      CompareDataNDC compare_data;
      DataNDC data;

      compare_data.emplace_front("1");
      compare_data.emplace_front("2");
      compare_data.emplace_front("3");
      compare_data.emplace_front("4");
      compare_data.emplace_front("5");
      compare_data.emplace_front("6");

      CHECK_NO_THROW(data.emplace_front("1"));
      CHECK_NO_THROW(data.emplace_front("2"));
      CHECK_NO_THROW(data.emplace_front("3"));
      CHECK_NO_THROW(data.emplace_front("4"));
      CHECK_NO_THROW(data.emplace_front("5"));
      CHECK_NO_THROW(data.emplace_front("6"));

      CHECK_EQUAL(6U, data.size());
      CHECK_EQUAL(6, std::distance(data.begin(), data.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

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
    TEST_FIXTURE(SetupFixture, test_emplace_after)
    {
      CompareDataNDC compare_data;
      DataNDC data;

      CompareDataNDC::iterator itc;

      itc = compare_data.before_begin();
      compare_data.emplace_after(itc, "1");
      compare_data.emplace_after(itc, "2");

      ++itc;
      compare_data.emplace_after(itc, "3");
      compare_data.emplace_after(itc, "4");

      ++itc;
      compare_data.emplace_after(itc, "5");
      compare_data.emplace_after(itc, "6");

      DataNDC::iterator itd;

      itd = data.before_begin();
      CHECK_NO_THROW(data.emplace_after(itd, "1"));
      CHECK_NO_THROW(data.emplace_after(itd, "2"));

      ++itd;
      CHECK_NO_THROW(data.emplace_after(itd, "3"));
      CHECK_NO_THROW(data.emplace_after(itd, "4"));

      ++itd;
      CHECK_NO_THROW(data.emplace_after(itd, "5"));
      CHECK_NO_THROW(data.emplace_after(itd, "6"));

      CHECK_EQUAL(6U, data.size());
      CHECK_EQUAL(6, std::distance(data.begin(), data.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      const DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(ItemNDC("0"), data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_before_begin_const)
    {
      const DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::const_iterator itr = data.before_begin();
      ++itr;

      are_equal = std::equal(data.begin(), data.end(), itr);

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_excess)
    {
      DataNDC data;

      CHECK_NO_THROW(data.push_front(ItemNDC("0")));
      CHECK_NO_THROW(data.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data.push_front(ItemNDC("4")));
      CHECK_NO_THROW(data.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data.push_front(ItemNDC("6")));
      CHECK_NO_THROW(data.push_front(ItemNDC("7")));
      CHECK_NO_THROW(data.push_front(ItemNDC("8")));
      CHECK_NO_THROW(data.push_front(ItemNDC("9")));

      CHECK_THROW(data.push_front(ItemNDC("10")) , etl::forward_list_full);
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
    TEST_FIXTURE(SetupFixture, test_pop_front_empty)
    {
      DataNDC data;

      CHECK_NO_THROW(data.push_front(ItemNDC("0")));
      CHECK_NO_THROW(data.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data.push_front(ItemNDC("3")));

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();

      CHECK_THROW(data.pop_front(), etl::forward_list_empty);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_single)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, 2);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 2);

      i_compare_data = compare_data.erase_after(i_compare_data);
      i_data         = data.erase_after(i_data);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase_after(compare_data.begin());
      i_data         = data.erase_after(data.begin());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = *i_data == *i_compare_data;
      CHECK(are_equal);

      // Move to the last value and erase.
      i_compare_data = compare_data.begin();
      //std::advance(i_compare_data, compare_data.size() - 1);
      i_compare_data = compare_data.erase_after(i_compare_data);

      i_data = data.begin();
      //std::advance(i_data, data.size() - 1);
      i_data = data.erase_after(i_data);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      are_equal = *i_data == *i_compare_data;
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_range)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data_1 = data.begin();
      std::advance(i_data_1, 2);

      DataNDC::iterator i_data_2 = data.begin();
      std::advance(i_data_2, 4);

      CompareDataNDC::iterator i_compare_data_1 = compare_data.begin();
      std::advance(i_compare_data_1, 2);

      CompareDataNDC::iterator i_compare_data_2 = compare_data.begin();
      std::advance(i_compare_data_2, 4);

      CompareDataNDC::iterator i_compare_result = compare_data.erase_after(i_compare_data_1, i_compare_data_2);

      DataNDC::iterator i_result = data.erase_after(i_data_1, i_data_2);

      CHECK_EQUAL(*i_compare_result, *i_result);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_range_end)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, 4);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 4);

      compare_data.erase_after(i_compare_data, compare_data.end());

      DataNDC::iterator i_result = data.erase_after(i_data, data.end());

      CHECK(i_result == data.end());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_all)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());

      data.erase_after(data.before_begin(), data.end());

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(compare_data.front(), data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
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
      DataNDC data1(sorted_data.begin(), sorted_data.end());
      DataNDC data2;

      IDataNDC& idata1 = data1;
      IDataNDC& idata2 = data2;

      idata2 = idata1;

      bool isEqual = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());
      DataNDC other_data(data);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_data = other_data;
#include "etl/private/diagnostic_pop.h" 

      CHECK_EQUAL(data.size(), other_data.size());

      are_equal = std::equal(data.begin(), data.end(), other_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique_empty)
    {
      CompareDataNDC compare_data;
      DataNDC data;

      compare_data.unique();
      data.unique();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique)
    {
      CompareDataNDC compare_data(non_unique_data.begin(), non_unique_data.end());
      DataNDC data(non_unique_data.begin(), non_unique_data.end());

      compare_data.unique();
      data.unique();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.remove(ItemNDC("7"));
      data.remove(ItemNDC("7"));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove_if)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));
      data.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.reverse();
      data.reverse();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());
      CHECK_EQUAL(data.size(), size_t(std::distance(data.begin(), data.end())));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_empty)
    {
      CompareDataNDC compare_data;
      DataNDC data;

      compare_data.reverse();
      data.reverse();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort)
    {
      CompareDataNDC compare_data(unsorted_data.begin(), unsorted_data.end());
      DataNDC data(unsorted_data.begin(), unsorted_data.end());

      compare_data.sort();
      data.sort();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort)
    {
      std::list<ItemNDC> compare_data(stable_sort_data.begin(), stable_sort_data.end());
      DataNDC data(stable_sort_data.begin(), stable_sort_data.end());

      compare_data.sort();
      data.sort();

      std::list<ItemNDC>::const_iterator citr = compare_data.begin();
      DataNDC::const_iterator        ditr = data.begin();

      while (ditr != data.end())
      {
        CHECK_EQUAL(citr->index, ditr->index);

        ++citr;
        ++ditr;
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_empty)
    {
      CompareDataNDC compare_data;
      DataNDC data;

      compare_data.sort();
      data.sort();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::const_iterator i_from_before;
      DataNDC::const_iterator i_to_before;
      CompareDataNDC::const_iterator i_compare_from_before;
      CompareDataNDC::const_iterator i_compare_to_before;

      // Move to the beginning.
      i_from_before = data.begin();
      std::advance(i_from_before, 5);

      i_to_before = data.before_begin();

      i_compare_from_before = compare_data.begin();
      std::advance(i_compare_from_before, 5);

      i_compare_to_before = compare_data.before_begin();

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_from_before);
      data.move_after(i_from_before, i_to_before);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to the end.
      i_from_before = data.begin();
      std::advance(i_from_before, 5);

      i_to_before = data.begin();
      std::advance(i_to_before, 9);

      i_compare_from_before = compare_data.begin();
      std::advance(i_compare_from_before, 5);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 9);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_from_before);
      data.move_after(i_from_before, i_to_before);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to nearby.
      i_from_before = data.begin();
      std::advance(i_from_before, 3);

      i_to_before = data.begin();
      std::advance(i_to_before, 7);

      i_compare_from_before = compare_data.begin();
      std::advance(i_compare_from_before, 3);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 7);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_from_before);
      data.move_after(i_from_before, i_to_before);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to same.
      i_from_before = data.begin();
      std::advance(i_from_before, 3);

      i_to_before = data.begin();
      std::advance(i_to_before, 3);

      i_compare_from_before = compare_data.begin();
      std::advance(i_compare_from_before, 3);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 3);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_from_before);
      data.move_after(i_from_before, i_to_before);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_range)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::const_iterator i_first_before;
      DataNDC::const_iterator i_last;
      DataNDC::const_iterator i_to_before;
      CompareDataNDC::const_iterator i_compare_first_before;
      CompareDataNDC::const_iterator i_compare_last;
      CompareDataNDC::const_iterator i_compare_to_before;

      // Move to the beginning.
      i_first_before = data.begin();
      std::advance(i_first_before, 4);

      i_last = i_first_before;
      std::advance(i_last, 4);

      i_to_before = data.before_begin();

      i_compare_first_before = compare_data.begin();
      std::advance(i_compare_first_before, 4);

      i_compare_last = i_compare_first_before;
      std::advance(i_compare_last, 4);

      i_compare_to_before = compare_data.before_begin();

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_first_before, i_compare_last);
      data.move_after(i_first_before, i_last, i_to_before);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to the end.
      i_first_before = data.begin();
      std::advance(i_first_before, 4);

      i_last = i_first_before;
      std::advance(i_last, 4);

      i_to_before = data.begin();
      std::advance(i_to_before, 9);

      i_compare_first_before = compare_data.begin();
      std::advance(i_compare_first_before, 4);

      i_compare_last = i_compare_first_before;
      std::advance(i_compare_last, 4);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 9);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_first_before, i_compare_last);
      data.move_after(i_first_before, i_last, i_to_before);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to nearby.
      i_first_before = data.begin();
      std::advance(i_first_before, 4);

      i_last = i_first_before;
      std::advance(i_last, 4);

      i_to_before = data.begin();
      std::advance(i_to_before, 2);

      i_compare_first_before = compare_data.begin();
      std::advance(i_compare_first_before, 4);

      i_compare_last = i_compare_first_before;
      std::advance(i_compare_last, 4);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 2);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_first_before, i_compare_last);
      data.move_after(i_first_before, i_last, i_to_before);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to same.
      i_first_before = data.begin();
      std::advance(i_first_before, 4);

      i_last = i_first_before;
      std::advance(i_last, 4);

      i_to_before = data.begin();
      std::advance(i_to_before, 4);

      i_compare_first_before = compare_data.begin();
      std::advance(i_compare_first_before, 4);

      i_compare_last = i_compare_first_before;
      std::advance(i_compare_last, 4);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 4);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_first_before, i_compare_last);
      data.move_after(i_first_before, i_last, i_to_before);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to illegal place.
      i_first_before = data.begin();
      std::advance(i_first_before, 2);
      i_last = i_first_before;
      std::advance(i_last, 3);
      i_to_before = i_first_before;
      std::advance(i_to_before, 2);

      CHECK_THROW(data.move_after(i_first_before, i_last, i_to_before), etl::forward_list_iterator);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_binary_comparisons)
    {
      DataNDC data1(sorted_data.begin(), sorted_data.end());
      DataNDC data2(sorted_data.begin(), sorted_data.end());
      DataNDC data3(unsorted_data.begin(), unsorted_data.end());

      CHECK(data1 == data2);
      CHECK(data1 != data3);
      CHECK(data1 < data3);
      CHECK(data3 > data1);
    }

    //*************************************************************************
    TEST(test_two_parameter_same_type_non_iterator)
    {
      // No compile error.
      etl::forward_list<int, 10> fl(10, 1);
      CHECK(fl.size() == 10);
      fl.assign(5, 2);
      CHECK(fl.size() == 5);
      fl.insert_after(fl.before_begin(), 5, 3);
      CHECK(fl.size() == fl.max_size());
    }
    
    //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST(test_forward_list_template_deduction)
    {
      etl::forward_list data{ ItemNDC("A"), ItemNDC("B"), ItemNDC("C"), ItemNDC("D"), ItemNDC("E"), ItemNDC("F") };

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
    TEST(test_make_forward_list)
    {
      auto data = etl::make_forward_list<ItemNDC>(ItemNDC("A"), ItemNDC("B"), ItemNDC("C"), ItemNDC("D"), ItemNDC("E"), ItemNDC("F"));

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
