/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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
    const size_t SIZE = 20UL;

    typedef TestDataDC<std::string>  ItemDC;
    typedef TestDataNDC<std::string> ItemNDC;

    typedef etl::forward_list_ext<ItemDC>  DataDC;
    typedef etl::forward_list_ext<ItemNDC> DataNDC;
    typedef etl::iforward_list<ItemNDC>   IDataNDC;

    typedef etl::forward_list_ext<int> DataInt;

    typedef std::forward_list<ItemDC> CompareDataDC;
    typedef std::forward_list<ItemNDC> CompareDataNDC;
    typedef std::vector<ItemNDC> InitialDataNDC;

    typedef etl::pool<DataInt::pool_type, SIZE * 2> PoolInt2;

    typedef etl::pool<DataDC::pool_type, SIZE> PoolDC;
    typedef etl::pool<DataDC::pool_type, SIZE * 2> PoolDC2;
    typedef etl::pool<DataDC::pool_type, SIZE * 4> PoolDC4;

    typedef etl::pool<DataNDC::pool_type, SIZE> PoolNDC;
    typedef etl::pool<DataNDC::pool_type, SIZE * 2> PoolNDC2;
    typedef etl::pool<DataNDC::pool_type, SIZE * 4> PoolNDC4;

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
      PoolDC pool;
      DataDC data(pool);

      CHECK_EQUAL(data.size(), 0UL);
      CHECK(data.empty());
      CHECK(!data.full());
      CHECK_EQUAL(data.available(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
      CHECK(data.begin() == data.end());
      CHECK(data.has_shared_pool());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_no_pool_exception)
    {
      DataNDC data;

      CHECK_THROW(data.push_front(ItemNDC("1")), etl::forward_list_no_pool);
      CHECK_THROW(data.full(), etl::forward_list_no_pool);
      CHECK_THROW(data.available(), etl::forward_list_no_pool);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      PoolDC pool;
      const size_t INITIAL_SIZE = 4UL;
      DataDC data1(INITIAL_SIZE, pool);
      DataDC data2(INITIAL_SIZE, pool);

      CHECK(!data1.empty());
      CHECK(!data2.empty());
      CHECK_EQUAL(INITIAL_SIZE, data1.size());
      CHECK_EQUAL(INITIAL_SIZE, data2.size());

      CHECK_EQUAL(SIZE - (INITIAL_SIZE * 2), data1.available());
      CHECK_EQUAL(SIZE - (INITIAL_SIZE * 2), data2.available());

      CHECK_EQUAL((2 * INITIAL_SIZE), pool.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      PoolDC pool;
      CHECK_THROW(DataDC data(SIZE + 1, pool), etl::forward_list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      PoolNDC pool;
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC INITIAL_VALUE("1");

      std::array<ItemNDC, INITIAL_SIZE> compare_data = { ItemNDC("1"), ItemNDC("1"), ItemNDC("1"), ItemNDC("1") };

      DataNDC data1(INITIAL_SIZE, INITIAL_VALUE, pool);
      DataNDC data2(INITIAL_SIZE, INITIAL_VALUE, pool);

      CHECK(!data1.empty());
      CHECK(!data2.empty());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      PoolNDC pool;
      CompareDataNDC compare(sorted_data.begin(), sorted_data.end());
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      CHECK(!data1.empty());
      CHECK(!data2.empty());

      are_equal = std::equal(data1.begin(), data1.end(), compare.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare.begin());
      CHECK(are_equal);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_initializer_list)
    {
      PoolNDC pool;
      CompareDataNDC compare = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3") };
      DataNDC data1({ ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3") }, pool);
      DataNDC data2({ ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3") }, pool);

      CHECK(!data1.empty());
      CHECK(!data2.empty());

      are_equal = std::equal(data1.begin(), data1.end(), compare.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare.begin());
      CHECK(are_equal);
    }
#endif

    //*************************************************************************
    TEST(test_destruct_via_iforward_list)
    {
      PoolNDC pool;
      int current_count = ItemNDC::get_instance_count();

      DataNDC* pdata1 = new DataNDC(sorted_data.begin(), sorted_data.end(), pool);
      CHECK_EQUAL(int(current_count + sorted_data.size()), ItemNDC::get_instance_count());

      DataNDC* pdata2 = new DataNDC(sorted_data.begin(), sorted_data.end(), pool);
      CHECK_EQUAL(int(current_count + sorted_data.size() + sorted_data.size()), ItemNDC::get_instance_count());

      IDataNDC* pidata1 = pdata1;
      delete pidata1;
      CHECK_EQUAL(current_count + sorted_data.size(), uint32_t(ItemNDC::get_instance_count()));

      IDataNDC* pidata2 = pdata2;
      delete pidata2;
      CHECK_EQUAL(current_count, ItemNDC::get_instance_count());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_implicit_pool)
    {
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(unsorted_data.begin(), unsorted_data.end(), pool);
      DataNDC other_data1(data1);
      DataNDC other_data2(data2);

      CHECK(std::equal(data1.begin(), data1.end(), other_data1.begin()));
      CHECK(std::equal(data2.begin(), data2.end(), other_data2.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_explicit_pool)
    {
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(unsorted_data.begin(), unsorted_data.end(), pool);
      DataNDC other_data1(data1, pool);
      DataNDC other_data2(data2, pool);

      CHECK(std::equal(data1.begin(), data1.end(), other_data1.begin()));
      CHECK(std::equal(data2.begin(), data2.end(), other_data2.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_constructor_implicit_pool)
    {
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(unsorted_data.begin(), unsorted_data.end(), data1.get_pool());
      DataNDC other_data1(std::move(data1));
      DataNDC other_data2(std::move(data2));

      CHECK_EQUAL(0U, data1.size());
      CHECK(data1.empty());
      CHECK_EQUAL(pool.max_size(), data1.max_size());
      CHECK(data1.begin() == data1.end());

      CHECK_EQUAL(0U, data2.size());
      CHECK(data2.empty());
      CHECK_EQUAL(pool.max_size(), data2.max_size());
      CHECK(data2.begin() == data2.end());

      CHECK_EQUAL(sorted_data.size(), other_data1.size());
      CHECK_EQUAL(unsorted_data.size(), other_data2.size());

      CHECK(std::equal(sorted_data.begin(), sorted_data.end(), other_data1.begin()));
      CHECK(std::equal(unsorted_data.begin(), unsorted_data.end(), other_data2.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_constructor_explicit_pool)
    {
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(unsorted_data.begin(), unsorted_data.end(), data1.get_pool());
      DataNDC other_data1(std::move(data1), pool);
      DataNDC other_data2(std::move(data2), pool);

      CHECK_EQUAL(0U, data1.size());
      CHECK(data1.empty());
      CHECK_EQUAL(pool.max_size(), data1.max_size());
      CHECK(data1.begin() == data1.end());

      CHECK_EQUAL(0U, data2.size());
      CHECK(data2.empty());
      CHECK_EQUAL(pool.max_size(), data2.max_size());
      CHECK(data2.begin() == data2.end());

      CHECK_EQUAL(sorted_data.size(), other_data1.size());
      CHECK_EQUAL(unsorted_data.size(), other_data2.size());

      CHECK(std::equal(sorted_data.begin(), sorted_data.end(), other_data1.begin()));
      CHECK(std::equal(unsorted_data.begin(), unsorted_data.end(), other_data2.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      PoolNDC2 pool;

      CompareDataNDC compare_data1(sorted_data.begin(), sorted_data.end());
      CompareDataNDC compare_data2(unsorted_data.begin(), unsorted_data.end());
      DataNDC data1(compare_data1.begin(), compare_data1.end(), pool);
      DataNDC data2(compare_data2.begin(), compare_data2.end(), pool);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      PoolNDC2 pool;

      CompareDataNDC compare_data1(sorted_data.begin(), sorted_data.end());
      CompareDataNDC compare_data2(unsorted_data.begin(), unsorted_data.end());
      const DataNDC data1(compare_data1.begin(), compare_data1.end(), pool);
      const DataNDC data2(compare_data2.begin(), compare_data2.end(), pool);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      PoolNDC2 pool;

      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE = 8UL;
      const ItemDC VALUE1("1");
      const ItemDC VALUE2("2");

      DataDC data1(INITIAL_SIZE, VALUE1, pool);
      data1.resize(NEW_SIZE);

      DataDC data2(INITIAL_SIZE, VALUE2, pool);
      data2.resize(NEW_SIZE);

      CompareDataDC compare_data1(INITIAL_SIZE, VALUE1);
      compare_data1.resize(NEW_SIZE);

      CompareDataDC compare_data2(INITIAL_SIZE, VALUE2);
      compare_data2.resize(NEW_SIZE);

      CHECK_EQUAL(size_t(std::distance(compare_data1.begin(), compare_data1.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data2.begin(), compare_data2.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      PoolNDC2 pool;

      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE     = 8UL;
      const ItemNDC VALUE1("1");
      const ItemNDC VALUE2("2");

      DataNDC data1(INITIAL_SIZE, VALUE1, pool);
      data1.resize(NEW_SIZE, VALUE1);

      DataNDC data2(INITIAL_SIZE, VALUE2, pool);
      data2.resize(NEW_SIZE, VALUE2);

      CompareDataNDC compare_data1(INITIAL_SIZE, VALUE1);
      compare_data1.resize(NEW_SIZE, VALUE1);

      CompareDataNDC compare_data2(INITIAL_SIZE, VALUE2);
      compare_data2.resize(NEW_SIZE, VALUE2);

      CHECK_EQUAL(size_t(std::distance(compare_data1.begin(), compare_data1.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data2.begin(), compare_data2.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      PoolNDC2 pool;

      const size_t INITIAL_SIZE = 4UL;
      DataDC data1(INITIAL_SIZE, pool);
      DataDC data2(INITIAL_SIZE, pool);

      CHECK_THROW(data1.resize(data1.max_size() + 1), etl::forward_list_full);
      CHECK_THROW(data2.resize(data2.max_size() + 1), etl::forward_list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      PoolNDC2 pool;

      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE = 2UL;
      const ItemDC VALUE1("1");
      const ItemDC VALUE2("2");

      DataDC data1(INITIAL_SIZE, VALUE1, pool);
      data1.resize(NEW_SIZE);

      DataDC data2(INITIAL_SIZE, VALUE2, pool);
      data2.resize(NEW_SIZE);

      CompareDataDC compare_data1(INITIAL_SIZE, VALUE1);
      compare_data1.resize(NEW_SIZE, VALUE1);

      CompareDataDC compare_data2(INITIAL_SIZE, VALUE2);
      compare_data2.resize(NEW_SIZE, VALUE2);

      CHECK_EQUAL(size_t(std::distance(compare_data1.begin(), compare_data1.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data2.begin(), compare_data2.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      PoolNDC2 pool;
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE = 2UL;
      const ItemNDC VALUE1("1");
      const ItemNDC VALUE2("2");

      DataNDC data1(INITIAL_SIZE, VALUE1, pool);
      data1.resize(NEW_SIZE, VALUE1);

      DataNDC data2(INITIAL_SIZE, VALUE2, pool);
      data2.resize(NEW_SIZE, VALUE2);

      CompareDataNDC compare_data1(INITIAL_SIZE, VALUE1);
      compare_data1.resize(NEW_SIZE, VALUE1);

      CompareDataNDC compare_data2(INITIAL_SIZE, VALUE2);
      compare_data2.resize(NEW_SIZE, VALUE2);

      CHECK_EQUAL(size_t(std::distance(compare_data1.begin(), compare_data1.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data2.begin(), compare_data2.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      PoolNDC2 pool;

      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      data1.clear();
      CHECK(data1.empty());
      CHECK(!data2.empty());

      data2.clear();
      CHECK(data1.empty());
      CHECK(data2.empty());

      // Do it again to check that clear() didn't screw up the internals.
      data1.assign(sorted_data.begin(), sorted_data.end());
      CHECK_EQUAL(sorted_data.size(), data1.size());
      data2.assign(sorted_data.begin(), sorted_data.end());
      CHECK_EQUAL(sorted_data.size(), data2.size());
      data1.clear();
      CHECK_EQUAL(0UL, data1.size());
      data2.clear();
      CHECK_EQUAL(0UL, data2.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear_pod)
    {
      PoolInt2 pool;
      DataInt data1(SIZE, 1, pool);
      DataInt data2(SIZE, 2, pool);

      data1.clear();
      CHECK(data1.empty());
      CHECK(!data2.empty());

      data2.clear();
      CHECK(data1.empty());
      CHECK(data2.empty());

      // Do it again to check that clear() didn't screw up the internals.
      data1.resize(SIZE);
      CHECK_EQUAL(SIZE, data1.size());
      data1.clear();
      CHECK_EQUAL(0UL, data1.size());

      data2.resize(SIZE);
      CHECK_EQUAL(SIZE, data2.size());
      data2.clear();
      CHECK_EQUAL(0UL, data2.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      PoolNDC2 pool;
      DataNDC data1(SIZE, ItemNDC("1"), pool);
      DataNDC data2(SIZE, ItemNDC("2"), pool);

      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());

      // Do it twice. We should only get one copy.
      data1.assign(compare_data.begin(), compare_data.end());
      data1.assign(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(pool.available(), data1.available());
      CHECK_EQUAL(pool.available(), data2.available());

      data2.assign(compare_data.begin(), compare_data.end());
      data2.assign(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(pool.available(), data1.available());
      CHECK_EQUAL(pool.available(), data2.available());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE0("0");
      const ItemNDC VALUE1("1");
      const ItemNDC VALUE2("2");

      PoolNDC2 pool;
      DataNDC data1(SIZE, VALUE0, pool);
      DataNDC data2(SIZE, VALUE0, pool);

      CompareDataNDC compare_data1(INITIAL_SIZE, VALUE1);
      CompareDataNDC compare_data2(INITIAL_SIZE, VALUE2);

      // Do it twice. We should only get one copy.
      data1.assign(INITIAL_SIZE, VALUE1);
      data1.assign(INITIAL_SIZE, VALUE1);

      data2.assign(INITIAL_SIZE, VALUE2);
      data2.assign(INITIAL_SIZE, VALUE2);

      CHECK_EQUAL(size_t(std::distance(compare_data1.begin(), compare_data1.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data2.begin(), compare_data2.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const ItemNDC VALUE("1");

      PoolNDC pool;
      DataNDC data(pool);

      CHECK_THROW(data.assign(data.max_size() + 1, VALUE), etl::forward_list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_position_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE("1");
      const ItemNDC INSERT_VALUE("2");

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);

      PoolNDC2 pool;
      DataNDC data1(INITIAL_SIZE, VALUE, pool);
      DataNDC data2(INITIAL_SIZE, VALUE, pool);

      size_t offset = 2UL;

      DataNDC::iterator i_data1 = data1.begin();
      std::advance(i_data1, offset);

      DataNDC::iterator i_data2 = data2.begin();
      std::advance(i_data2, offset);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data1.insert_after(i_data1, INSERT_VALUE);
      data2.insert_after(i_data2, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, INSERT_VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      offset = 0;

      i_data1 = data1.begin();
      std::advance(i_data1, offset);

      i_data2 = data2.begin();
      std::advance(i_data2, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data1.insert_after(i_data1, INSERT_VALUE);
      data2.insert_after(i_data2, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, INSERT_VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_position_size_value)
    {
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE("1");
      const ItemNDC INSERT_VALUE("2");

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      PoolNDC2 pool;
      DataNDC data1(INITIAL_SIZE, VALUE, pool);
      DataNDC data2(INITIAL_SIZE, VALUE, pool);

      size_t offset = 2UL;

      DataNDC::iterator i_data1 = data1.begin();
      std::advance(i_data1, offset);

      DataNDC::iterator i_data2 = data2.begin();
      std::advance(i_data2, offset);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data1.insert_after(i_data1, 2, INSERT_VALUE);
      data2.insert_after(i_data2, 2, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, 2, INSERT_VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      offset = 0;

      i_data1 = data1.begin();
      std::advance(i_data1, offset);

      i_data2 = data2.begin();
      std::advance(i_data2, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data1.insert_after(i_data1, 2, INSERT_VALUE);
      data2.insert_after(i_data2, 2, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, 2, INSERT_VALUE);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_range)
    {
      std::vector<ItemNDC> test1 = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4") };
      std::vector<ItemNDC> test2 = { ItemNDC("5"), ItemNDC("6"), ItemNDC("7"), ItemNDC("8"), ItemNDC("9") };

      CompareDataNDC compare_data(test1.begin(), test1.end());
      PoolNDC2 pool;
      DataNDC data1(test1.begin(), test1.end(), pool);
      DataNDC data2(test1.begin(), test1.end(), pool);

      compare_data.insert_after(compare_data.before_begin(), test2.begin(), test2.end());
      data1.insert_after(data1.before_begin(), test2.begin(), test2.end());
      data2.insert_after(data2.before_begin(), test2.begin(), test2.end());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      compare_data.assign(test1.begin(), test1.end());
      data1.assign(test1.begin(), test1.end());
      data2.assign(test1.begin(), test1.end());

      CompareDataNDC::iterator icd = compare_data.begin();
      DataNDC::iterator         id1 = data1.begin();
      DataNDC::iterator         id2 = data2.begin();

      std::advance(icd, 3);
      std::advance(id1, 3);
      std::advance(id2, 3);

      compare_data.insert_after(icd, test2.begin(), test2.end());
      data1.insert_after(id1, test2.begin(), test2.end());
      data2.insert_after(id2, test2.begin(), test2.end());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front)
    {
      CompareDataNDC compare_data;
      PoolNDC2 pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data.push_front(ItemNDC("1"));
      compare_data.push_front(ItemNDC("2"));
      compare_data.push_front(ItemNDC("3"));
      compare_data.push_front(ItemNDC("4"));
      compare_data.push_front(ItemNDC("5"));
      compare_data.push_front(ItemNDC("6"));

      CHECK_NO_THROW(data1.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("4")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("6")));

      CHECK_NO_THROW(data2.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("4")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("6")));

      CHECK_EQUAL(6U, data1.size());
      CHECK_EQUAL(6, std::distance(data1.begin(), data1.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(6U, data2.size());
      CHECK_EQUAL(6, std::distance(data2.begin(), data2.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_front)
    {
      CompareDataNDC compare_data;
      PoolNDC2 pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data.emplace_front("1");
      compare_data.emplace_front("2");
      compare_data.emplace_front("3");
      compare_data.emplace_front("4");
      compare_data.emplace_front("5");
      compare_data.emplace_front("6");

      CHECK_NO_THROW(data1.emplace_front("1"));
      CHECK_NO_THROW(data1.emplace_front("2"));
      CHECK_NO_THROW(data1.emplace_front("3"));
      CHECK_NO_THROW(data1.emplace_front("4"));
      CHECK_NO_THROW(data1.emplace_front("5"));
      CHECK_NO_THROW(data1.emplace_front("6"));

      CHECK_NO_THROW(data2.emplace_front("1"));
      CHECK_NO_THROW(data2.emplace_front("2"));
      CHECK_NO_THROW(data2.emplace_front("3"));
      CHECK_NO_THROW(data2.emplace_front("4"));
      CHECK_NO_THROW(data2.emplace_front("5"));
      CHECK_NO_THROW(data2.emplace_front("6"));

      CHECK_EQUAL(6U, data1.size());
      CHECK_EQUAL(6, std::distance(data1.begin(), data1.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(6U, data2.size());
      CHECK_EQUAL(6, std::distance(data2.begin(), data2.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_after)
    {
      CompareDataNDC compare_data;
      PoolNDC2 pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

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

      DataNDC::iterator itd1;
      DataNDC::iterator itd2;

      itd1 = data1.before_begin();
      itd2 = data2.before_begin();
      CHECK_NO_THROW(data1.emplace_after(itd1, "1"));
      CHECK_NO_THROW(data1.emplace_after(itd1, "2"));
      CHECK_NO_THROW(data2.emplace_after(itd2, "1"));
      CHECK_NO_THROW(data2.emplace_after(itd2, "2"));

      ++itd1;
      ++itd2;
      CHECK_NO_THROW(data1.emplace_after(itd1, "3"));
      CHECK_NO_THROW(data1.emplace_after(itd1, "4"));
      CHECK_NO_THROW(data2.emplace_after(itd2, "3"));
      CHECK_NO_THROW(data2.emplace_after(itd2, "4"));

      ++itd1;
      ++itd2;
      CHECK_NO_THROW(data1.emplace_after(itd1, "5"));
      CHECK_NO_THROW(data1.emplace_after(itd1, "6"));
      CHECK_NO_THROW(data2.emplace_after(itd2, "5"));
      CHECK_NO_THROW(data2.emplace_after(itd2, "6"));

      CHECK_EQUAL(6U, data1.size());
      CHECK_EQUAL(6, std::distance(data1.begin(), data1.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(6U, data2.size());
      CHECK_EQUAL(6, std::distance(data2.begin(), data2.end()));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      PoolNDC2 pool;
      const DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      const DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      CHECK_EQUAL(ItemNDC("0"), data1.front());
      CHECK_EQUAL(ItemNDC("0"), data2.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_before_begin_const)
    {
      PoolNDC2 pool;
      const DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      const DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      DataNDC::const_iterator itr1 = data1.before_begin();
      ++itr1;

      DataNDC::const_iterator itr2 = data2.before_begin();
      ++itr2;

      are_equal = std::equal(data1.begin(), data1.end(), itr1);
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), itr2);
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_excess)
    {
      PoolNDC pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      CHECK_NO_THROW(data1.push_front(ItemNDC("0")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("4")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("6")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("7")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("8")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("9")));

      CHECK_NO_THROW(data2.push_front(ItemNDC("0")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("4")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("6")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("7")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("8")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("9")));

      CHECK_THROW(data1.push_front(ItemNDC("10")), etl::forward_list_full);
      CHECK_THROW(data2.push_front(ItemNDC("10")), etl::forward_list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_pop_front)
    {
      PoolNDC pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      for (size_t i = 0UL; i < 2UL * data1.max_size(); ++i)
      {
        CHECK_NO_THROW(data1.push_front(ItemNDC("1")));
        CHECK_NO_THROW(data2.push_front(ItemNDC("2")));
        data1.pop_front();
        data2.pop_front();
      }

      CHECK(data1.empty());
      CHECK(data2.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_front_empty)
    {
      PoolNDC pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      CHECK_NO_THROW(data1.push_front(ItemNDC("0")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("3")));

      CHECK_NO_THROW(data2.push_front(ItemNDC("0")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("3")));

      data1.pop_front();
      data1.pop_front();
      data1.pop_front();
      data1.pop_front();

      data2.pop_front();
      data2.pop_front();
      data2.pop_front();
      data2.pop_front();

      CHECK_THROW(data1.pop_front(), etl::forward_list_empty);
      CHECK_THROW(data2.pop_front(), etl::forward_list_empty);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_single)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      DataNDC::iterator i_data1 = data1.begin();
      std::advance(i_data1, 2);

      DataNDC::iterator i_data2 = data2.begin();
      std::advance(i_data2, 2);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 2);

      i_compare_data = compare_data.erase_after(i_compare_data);
      i_data1        = data1.erase_after(i_data1);
      i_data2        = data2.erase_after(i_data2);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data1);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data2);

      i_compare_data = compare_data.erase_after(compare_data.begin());
      i_data1        = data1.erase_after(data1.begin());
      i_data2        = data2.erase_after(data2.begin());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = *i_data1 == *i_compare_data;
      CHECK(are_equal);

      are_equal = *i_data2 == *i_compare_data;
      CHECK(are_equal);


      // Move to the last value and erase.
      i_compare_data = compare_data.begin();
      //std::advance(i_compare_data, compare_data.size() - 1);
      i_compare_data = compare_data.erase_after(i_compare_data);

      i_data1 = data1.begin();
      i_data2 = data2.begin();
      //std::advance(i_data, data.size() - 1);
      i_data1 = data1.erase_after(i_data1);
      i_data2 = data2.erase_after(i_data2);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = *i_data1 == *i_compare_data;
      CHECK(are_equal);

      are_equal = *i_data2 == *i_compare_data;
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_range)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      DataNDC::iterator i_data1_1 = data1.begin();
      std::advance(i_data1_1, 2);

      DataNDC::iterator i_data1_2 = data1.begin();
      std::advance(i_data1_2, 4);

      DataNDC::iterator i_data2_1 = data2.begin();
      std::advance(i_data2_1, 2);

      DataNDC::iterator i_data2_2 = data2.begin();
      std::advance(i_data2_2, 4);

      CompareDataNDC::iterator i_compare_data_1 = compare_data.begin();
      std::advance(i_compare_data_1, 2);

      CompareDataNDC::iterator i_compare_data_2 = compare_data.begin();
      std::advance(i_compare_data_2, 4);

      CompareDataNDC::iterator i_compare_result = compare_data.erase_after(i_compare_data_1, i_compare_data_2);

      DataNDC::iterator i_result1 = data1.erase_after(i_data1_1, i_data1_2);
      DataNDC::iterator i_result2 = data2.erase_after(i_data2_1, i_data2_2);

      CHECK_EQUAL(*i_compare_result, *i_result1);
      CHECK_EQUAL(*i_compare_result, *i_result2);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_range_end)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      DataNDC::iterator i_data1 = data1.begin();
      std::advance(i_data1, 4);

      DataNDC::iterator i_data2 = data2.begin();
      std::advance(i_data2, 4);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 4);

      compare_data.erase_after(i_compare_data, compare_data.end());

      DataNDC::iterator i_result1 = data1.erase_after(i_data1, data1.end());
      DataNDC::iterator i_result2 = data2.erase_after(i_data2, data2.end());

      CHECK(i_result1 == data1.end());
      CHECK(i_result2 == data2.end());

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_all)
    {
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      data1.erase_after(data1.before_begin(), data1.end());

      CHECK(data1.empty());
      CHECK(!data2.empty());

      data2.erase_after(data2.before_begin(), data2.end());

      CHECK(data1.empty());
      CHECK(data2.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      CHECK_EQUAL(compare_data.front(), data1.front());
      CHECK_EQUAL(compare_data.front(), data2.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC4 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC other_data1(pool);
      DataNDC other_data2(pool);

      other_data1 = data1;
      other_data2 = data2;

      CHECK_EQUAL(data1.size(), other_data1.size());
      CHECK_EQUAL(data2.size(), other_data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), other_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), other_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      PoolNDC4 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), data1.get_pool());
      DataNDC other_data1(pool);
      DataNDC other_data2(pool);

      other_data1 = std::move(data1);
      other_data2 = std::move(data2);

      CHECK_EQUAL(0U, data1.size());
      CHECK(data1.empty());
      CHECK_EQUAL(pool.max_size(), data1.max_size());
      CHECK(data1.begin() == data1.end());

      CHECK_EQUAL(0U, data2.size());
      CHECK(data2.empty());
      CHECK_EQUAL(pool.max_size(), data2.max_size());
      CHECK(data2.begin() == data2.end());

      CHECK_EQUAL(sorted_data.size(), other_data1.size());
      CHECK_EQUAL(sorted_data.size(), other_data2.size());

      are_equal = std::equal(sorted_data.begin(), sorted_data.end(), other_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(sorted_data.begin(), sorted_data.end(), other_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment_through_interface)
    {
      PoolNDC4 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), data1.get_pool());
      DataNDC other_data1(pool);
      DataNDC other_data2(pool);

      IDataNDC& idata1 = data1;
      IDataNDC& idata2 = data2;
      IDataNDC& iother_data1 = other_data1;
      IDataNDC& iother_data2 = other_data2;

      iother_data1 = std::move(idata1);
      iother_data2 = std::move(idata2);

      CHECK_EQUAL(0U, data1.size());
      CHECK(data1.empty());
      CHECK_EQUAL(pool.max_size(), data1.max_size());
      CHECK(data1.begin() == data1.end());

      CHECK_EQUAL(0U, data2.size());
      CHECK(data2.empty());
      CHECK_EQUAL(pool.max_size(), data2.max_size());
      CHECK(data2.begin() == data2.end());

      CHECK_EQUAL(sorted_data.size(), iother_data1.size());
      CHECK_EQUAL(sorted_data.size(), iother_data2.size());

      are_equal = std::equal(sorted_data.begin(), sorted_data.end(), iother_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(sorted_data.begin(), sorted_data.end(), iother_data2.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_interface)
    {
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(pool);

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
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC other_data(data1, pool);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_data = other_data;
#include "etl/private/diagnostic_pop.h" 

      CHECK_EQUAL(data1.size(), other_data.size());

      are_equal = std::equal(data1.begin(), data1.end(), other_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique_empty)
    {
      CompareDataNDC compare_data;
      PoolNDC2 pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data.unique();
      data1.unique();
      data2.unique();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique)
    {
      CompareDataNDC compare_data(non_unique_data.begin(), non_unique_data.end());
      PoolNDC2 pool;
      DataNDC data1(non_unique_data.begin(), non_unique_data.end(), pool);
      DataNDC data2(non_unique_data.begin(), non_unique_data.end(), pool);

      compare_data.unique();
      data1.unique();
      data2.unique();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      compare_data.remove(ItemNDC("7"));
      data1.remove(ItemNDC("7"));
      data2.remove(ItemNDC("7"));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove_if)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      compare_data.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));
      data1.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));
      data2.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      compare_data.reverse();
      data1.reverse();
      data2.reverse();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(data1.size(), size_t(std::distance(data1.begin(), data1.end())));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());
      CHECK_EQUAL(data2.size(), size_t(std::distance(data2.begin(), data2.end())));

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_empty)
    {
      CompareDataNDC compare_data;
      PoolNDC2 pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data.reverse();
      data1.reverse();
      data2.reverse();

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort)
    {
      CompareDataNDC compare_data(unsorted_data.begin(), unsorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(unsorted_data.begin(), unsorted_data.end(), pool);
      DataNDC data2(unsorted_data.begin(), unsorted_data.end(), pool);

      compare_data.sort();
      data1.sort();
      data2.sort();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort)
    {
      std::list<ItemNDC> compare_data(stable_sort_data.begin(), stable_sort_data.end());
      PoolNDC2 pool;
      DataNDC data1(stable_sort_data.begin(), stable_sort_data.end(), pool);
      DataNDC data2(stable_sort_data.begin(), stable_sort_data.end(), pool);

      compare_data.sort();
      data1.sort();
      data2.sort();

      std::list<ItemNDC>::const_iterator citr = compare_data.begin();
      DataNDC::const_iterator d1itr = data1.begin();
      DataNDC::const_iterator d2itr = data2.begin();

      while (d1itr != data1.end())
      {
        CHECK_EQUAL(citr->index, d1itr->index);

        ++citr;
        ++d1itr;
      }

      citr = compare_data.begin();

      while (d2itr != data2.end())
      {
        CHECK_EQUAL(citr->index, d2itr->index);

        ++citr;
        ++d2itr;
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_empty)
    {
      CompareDataNDC compare_data;
      PoolNDC2 pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data.sort();
      data1.sort();
      data2.sort();

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      DataNDC::const_iterator i_from_before1;
      DataNDC::const_iterator i_to_before1;
      DataNDC::const_iterator i_from_before2;
      DataNDC::const_iterator i_to_before2;
      CompareDataNDC::const_iterator i_compare_from_before;
      CompareDataNDC::const_iterator i_compare_to_before;

      // Move to the beginning.
      i_from_before1 = data1.begin();
      std::advance(i_from_before1, 5);

      i_to_before1 = data1.before_begin();

      i_from_before1 = data1.begin();
      std::advance(i_from_before1, 5);

      i_to_before1 = data1.before_begin();

      i_from_before2 = data2.begin();
      std::advance(i_from_before2, 5);

      i_to_before2 = data2.before_begin();

      i_from_before2 = data2.begin();
      std::advance(i_from_before2, 5);

      i_to_before2 = data2.before_begin();

      i_compare_from_before = compare_data.begin();
      std::advance(i_compare_from_before, 5);

      i_compare_to_before = compare_data.before_begin();

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_from_before);
      data1.move_after(i_from_before1, i_to_before1);
      data2.move_after(i_from_before2, i_to_before2);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to the end.
      i_from_before1 = data1.begin();
      std::advance(i_from_before1, 5);

      i_to_before1 = data1.begin();
      std::advance(i_to_before1, 9);

      i_from_before2 = data2.begin();
      std::advance(i_from_before2, 5);

      i_to_before2 = data2.begin();
      std::advance(i_to_before2, 9);

      i_compare_from_before = compare_data.begin();
      std::advance(i_compare_from_before, 5);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 9);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_from_before);
      data1.move_after(i_from_before1, i_to_before1);
      data2.move_after(i_from_before2, i_to_before2);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to nearby.
      i_from_before1 = data1.begin();
      std::advance(i_from_before1, 3);

      i_to_before1 = data1.begin();
      std::advance(i_to_before1, 7);

      i_from_before2 = data2.begin();
      std::advance(i_from_before2, 3);

      i_to_before2 = data2.begin();
      std::advance(i_to_before2, 7);

      i_compare_from_before = compare_data.begin();
      std::advance(i_compare_from_before, 3);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 7);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_from_before);
      data1.move_after(i_from_before1, i_to_before1);
      data2.move_after(i_from_before2, i_to_before2);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to same.
      i_from_before1 = data1.begin();
      std::advance(i_from_before1, 3);

      i_to_before1 = data1.begin();
      std::advance(i_to_before1, 3);

      i_from_before2 = data2.begin();
      std::advance(i_from_before2, 3);

      i_to_before2 = data2.begin();
      std::advance(i_to_before2, 3);

      i_compare_from_before = compare_data.begin();
      std::advance(i_compare_from_before, 3);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 3);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_from_before);
      data1.move_after(i_from_before1, i_to_before1);
      data2.move_after(i_from_before2, i_to_before2);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_range)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      PoolNDC2 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      DataNDC::const_iterator i_first_before1;
      DataNDC::const_iterator i_last1;
      DataNDC::const_iterator i_to_before1;
      DataNDC::const_iterator i_first_before2;
      DataNDC::const_iterator i_last2;
      DataNDC::const_iterator i_to_before2;
      CompareDataNDC::const_iterator i_compare_first_before;
      CompareDataNDC::const_iterator i_compare_last;
      CompareDataNDC::const_iterator i_compare_to_before;

      // Move to the beginning.
      i_first_before1 = data1.begin();
      std::advance(i_first_before1, 4);

      i_last1 = i_first_before1;
      std::advance(i_last1, 4);

      i_to_before1 = data1.before_begin();

      i_first_before2 = data2.begin();
      std::advance(i_first_before2, 4);

      i_last2 = i_first_before2;
      std::advance(i_last2, 4);

      i_to_before2 = data2.before_begin();

      i_compare_first_before = compare_data.begin();
      std::advance(i_compare_first_before, 4);

      i_compare_last = i_compare_first_before;
      std::advance(i_compare_last, 4);

      i_compare_to_before = compare_data.before_begin();

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_first_before, i_compare_last);
      data1.move_after(i_first_before1, i_last1, i_to_before1);
      data2.move_after(i_first_before2, i_last2, i_to_before2);

      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data1.size());
      CHECK_EQUAL(size_t(std::distance(compare_data.begin(), compare_data.end())), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to the end.
      i_first_before1 = data1.begin();
      std::advance(i_first_before1, 4);

      i_last1 = i_first_before1;
      std::advance(i_last1, 4);

      i_to_before1 = data1.begin();
      std::advance(i_to_before1, 9);

      i_first_before2 = data2.begin();
      std::advance(i_first_before2, 4);

      i_last2 = i_first_before2;
      std::advance(i_last2, 4);

      i_to_before2 = data2.begin();
      std::advance(i_to_before2, 9);

      i_compare_first_before = compare_data.begin();
      std::advance(i_compare_first_before, 4);

      i_compare_last = i_compare_first_before;
      std::advance(i_compare_last, 4);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 9);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_first_before, i_compare_last);
      data1.move_after(i_first_before1, i_last1, i_to_before1);
      data2.move_after(i_first_before2, i_last2, i_to_before2);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to nearby.
      i_first_before1 = data1.begin();
      std::advance(i_first_before1, 4);

      i_last1 = i_first_before1;
      std::advance(i_last1, 4);

      i_to_before1 = data1.begin();
      std::advance(i_to_before1, 2);

      i_first_before2 = data2.begin();
      std::advance(i_first_before2, 4);

      i_last2 = i_first_before2;
      std::advance(i_last2, 4);

      i_to_before2 = data2.begin();
      std::advance(i_to_before2, 2);

      i_compare_first_before = compare_data.begin();
      std::advance(i_compare_first_before, 4);

      i_compare_last = i_compare_first_before;
      std::advance(i_compare_last, 4);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 2);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_first_before, i_compare_last);
      data1.move_after(i_first_before1, i_last1, i_to_before1);
      data2.move_after(i_first_before2, i_last2, i_to_before2);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to same.
      i_first_before1 = data1.begin();
      std::advance(i_first_before1, 4);

      i_last1 = i_first_before1;
      std::advance(i_last1, 4);

      i_to_before1 = data1.begin();
      std::advance(i_to_before1, 4);

      i_first_before2 = data2.begin();
      std::advance(i_first_before2, 4);

      i_last2 = i_first_before2;
      std::advance(i_last2, 4);

      i_to_before2 = data2.begin();
      std::advance(i_to_before2, 4);

      i_compare_first_before = compare_data.begin();
      std::advance(i_compare_first_before, 4);

      i_compare_last = i_compare_first_before;
      std::advance(i_compare_last, 4);

      i_compare_to_before = compare_data.begin();
      std::advance(i_compare_to_before, 4);

      compare_data.splice_after(i_compare_to_before, compare_data, i_compare_first_before, i_compare_last);
      data1.move_after(i_first_before1, i_last1, i_to_before1);
      data2.move_after(i_first_before2, i_last2, i_to_before2);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      // Move to illegal place.
      i_first_before1 = data1.begin();
      std::advance(i_first_before1, 2);
      i_last1 = i_first_before1;
      std::advance(i_last1, 3);
      i_to_before1 = i_first_before1;
      std::advance(i_to_before1, 2);

      i_first_before2 = data2.begin();
      std::advance(i_first_before2, 2);
      i_last2 = i_first_before2;
      std::advance(i_last2, 3);
      i_to_before2 = i_first_before2;
      std::advance(i_to_before2, 2);

      CHECK_THROW(data1.move_after(i_first_before1, i_last1, i_to_before1), etl::forward_list_iterator);
      CHECK_THROW(data2.move_after(i_first_before2, i_last2, i_to_before2), etl::forward_list_iterator);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_binary_comparisons)
    {
      PoolDC4 pool;
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data3(unsorted_data.begin(), unsorted_data.end(), pool);

      CHECK(data1 == data2);
      CHECK(data1 != data3);
      CHECK(data1 < data3);
      CHECK(data3 > data1);
    }
  };
}
