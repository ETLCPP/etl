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

#include "etl/list.h"
#include "etl/pool.h"

#include "data.h"
#include "etl/container.h"

#include <algorithm>
#include <array>
#include <list>
#include <vector>
#include <functional>

namespace
{
  SUITE(test_list)
  {
    typedef TestDataDC<std::string> ItemDC;
    typedef TestDataNDC<std::string> ItemNDC;

    const size_t SIZE = 10UL;

    typedef etl::list_ext<ItemDC>  DataDC;
    typedef etl::list_ext<ItemNDC> DataNDC;
    typedef etl::ilist<ItemNDC>   IDataNDC;

    typedef std::list<ItemNDC>   CompareData;
    typedef std::vector<ItemNDC> InitialData;

    typedef etl::list_ext<int> DataInt;

    typedef etl::pool<DataNDC::pool_type, SIZE> Pool;
    typedef etl::pool<DataNDC::pool_type, SIZE * 2> Pool2;
    typedef etl::pool<DataNDC::pool_type, SIZE * 4> Pool4;

    InitialData stable_sort_data;
    InitialData unsorted_data;
    InitialData unsorted_data2;
    InitialData sorted_data;
    InitialData non_unique_data;
    InitialData small_data;
    InitialData half_data;

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
        stable_sort_data = { ItemNDC("1", 1), ItemNDC("2", 2), ItemNDC("1", 3), ItemNDC("2", 4), ItemNDC("1", 5) };
        unsorted_data    = { ItemNDC("1"), ItemNDC("0"), ItemNDC("3"), ItemNDC("2"), ItemNDC("5") };
        unsorted_data2   = { ItemNDC("1"), ItemNDC("0"), ItemNDC("3"), ItemNDC("2"), ItemNDC("5"), ItemNDC("4"), ItemNDC("7"), ItemNDC("6"), ItemNDC("9"), ItemNDC("8") };
        sorted_data      = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4") };
        non_unique_data  = { ItemNDC("0"), ItemNDC("0"), ItemNDC("1"), ItemNDC("1"), ItemNDC("2") };
        small_data       = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5") };
        half_data        = { ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4") };

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

      CHECK_EQUAL(data.max_size(), 0U);
      CHECK_EQUAL(data.size(), 0U);
      CHECK(data.empty());
      CHECK(data.has_shared_pool());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_no_pool_exception)
    {
      DataNDC data;

      CHECK_THROW(data.push_back(ItemNDC("1")), etl::list_no_pool);
      CHECK_THROW(data.full(), etl::list_no_pool);
      CHECK_THROW(data.available(), etl::list_no_pool);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor_pool)
    {
      Pool pool;
      DataNDC data(pool);

      CHECK_EQUAL(data.size(), 0UL);
      CHECK(data.empty());
      CHECK(!data.full());
      CHECK_EQUAL(data.available(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
      CHECK(data.begin() == data.end());

      CHECK(pool.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor_shared_pool)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      CHECK_EQUAL(data1.size(), 0UL);
      CHECK(data1.empty());
      CHECK_EQUAL(data1.max_size(), SIZE);
      CHECK(data1.begin() == data1.end());

      CHECK_EQUAL(data2.size(), 0UL);
      CHECK(data2.empty());
      CHECK_EQUAL(data2.max_size(), SIZE);
      CHECK(data2.begin() == data2.end());

      CHECK(pool.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_shared_pool)
    {
      Pool pool;
      const size_t INITIAL_SIZE = SIZE / 2UL;
      DataDC data1(INITIAL_SIZE, pool);
      DataDC data2(INITIAL_SIZE, pool);

      CHECK_EQUAL(INITIAL_SIZE, data1.size());
      CHECK(!data1.empty());

      CHECK_EQUAL(INITIAL_SIZE, data2.size());
      CHECK(!data2.empty());

      CHECK_EQUAL(SIZE - (INITIAL_SIZE * 2), data1.available());
      CHECK_EQUAL(SIZE - (INITIAL_SIZE * 2), data2.available());

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      Pool pool;
      CHECK_THROW(DataDC data(SIZE + 1, pool), etl::list_full);

      CHECK(pool.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      Pool pool;
      const size_t INITIAL_SIZE = SIZE / 2UL;
      const ItemNDC INITIAL_VALUE = ItemNDC("1");

      std::array<ItemNDC, INITIAL_SIZE> compare_data = { ItemNDC("1"), ItemNDC("1"), ItemNDC("1"), ItemNDC("1"), ItemNDC("1") };

      DataNDC data1(INITIAL_SIZE, INITIAL_VALUE, pool);
      DataNDC data2(INITIAL_SIZE, INITIAL_VALUE, pool);

      CHECK(data1.size() == INITIAL_SIZE);
      CHECK(!data1.empty());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK(data2.size() == INITIAL_SIZE);
      CHECK(!data2.empty());

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value_excess)
    {
      Pool pool;
      CHECK_THROW(DataNDC data(SIZE + 1, ItemNDC("1"), pool), etl::list_full);

      CHECK(pool.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Pool pool;
      DataNDC data1(half_data.begin(), half_data.end(), pool);
      DataNDC data2(half_data.begin(), half_data.end(), pool);

      CHECK_EQUAL(SIZE / 2UL, data1.size());
      CHECK(!data1.empty());

      CHECK_EQUAL(SIZE / 2UL, data2.size());
      CHECK(!data2.empty());

      CHECK(pool.full());
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_initializer_list)
    {
      Pool pool;
      DataNDC data1({ ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4") }, pool);
      DataNDC data2({ ItemNDC("0"), ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4") }, pool);

      CHECK_EQUAL(5U, data1.size());
      CHECK_EQUAL(5U, data2.size());

      DataNDC::const_iterator i_item = data1.begin();

      CHECK_EQUAL(ItemNDC("0"), *i_item++);
      CHECK_EQUAL(ItemNDC("1"), *i_item++);
      CHECK_EQUAL(ItemNDC("2"), *i_item++);
      CHECK_EQUAL(ItemNDC("3"), *i_item++);
      CHECK_EQUAL(ItemNDC("4"), *i_item++);

      i_item = data2.begin();

      CHECK_EQUAL(ItemNDC("0"), *i_item++);
      CHECK_EQUAL(ItemNDC("1"), *i_item++);
      CHECK_EQUAL(ItemNDC("2"), *i_item++);
      CHECK_EQUAL(ItemNDC("3"), *i_item++);
      CHECK_EQUAL(ItemNDC("4"), *i_item++);

      CHECK(pool.full());
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_implicit_pool)
    {
      CompareData compare_data(half_data.begin(), half_data.end());

      Pool pool;
      DataNDC data(half_data.begin(), half_data.end(), pool);
      DataNDC other_data(data);

      CHECK_EQUAL(data.size(), other_data.size());

      CHECK(std::equal(data.begin(), data.end(), other_data.begin()));

      CHECK(pool.full());

      other_data.pop_front();
      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(compare_data.size() - 1, other_data.size());

      other_data.pop_back();
      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(compare_data.size() - 2, other_data.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor_explicit_pool)
    {
      CompareData compare_data(half_data.begin(), half_data.end());

      Pool pool;
      DataNDC data(half_data.begin(), half_data.end(), pool);
      DataNDC other_data(data, pool);

      CHECK_EQUAL(data.size(), other_data.size());

      CHECK(std::equal(data.begin(), data.end(), other_data.begin()));

      CHECK(pool.full());

      other_data.pop_front();
      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(compare_data.size() - 1, other_data.size());

      other_data.pop_back();
      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(compare_data.size() - 2, other_data.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_constructor_implicit_pool)
    {
      Pool2 pool;
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
      Pool2 pool;
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
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end(), pool);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end(), pool);

      are_equal = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end(), pool);

      are_equal = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(compare_data.begin(), compare_data.end(), pool);

      are_equal = std::equal(data.crbegin(), data.crend(), compare_data.crbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      Pool pool;
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE     = 8UL;

      DataDC data(INITIAL_SIZE, pool);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      Pool pool;
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE     = 8UL;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE, pool);
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
      Pool pool;
      const size_t INITIAL_SIZE = 5UL;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE, pool);

      CHECK_THROW(data.resize(data.max_size() + 1, VALUE), etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      Pool pool;
      const size_t INITIAL_SIZE = 4UL;
      const size_t NEW_SIZE     = 2UL;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data(INITIAL_SIZE, VALUE, pool);
      data.resize(NEW_SIZE, VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      Pool pool;
      DataNDC data1(half_data.begin(), half_data.end(), pool);
      DataNDC data2(half_data.begin(), half_data.end(), pool);

      data1.clear();
      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(half_data.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), half_data.begin());
      CHECK(are_equal);

      // Do it again to check that clear() didn't screw up the internals.
      data1.assign(half_data.begin(), half_data.end());
      CHECK_EQUAL(SIZE / 2UL, data1.size());
      data1.clear();
      CHECK_EQUAL(0U, data1.size());
      CHECK_EQUAL(half_data.size(), data2.size());

      are_equal = std::equal(data2.begin(), data2.end(), half_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear_pod)
    {
      Pool pool;
      DataInt data1(SIZE / 2UL, 1, pool);
      DataInt data2(SIZE / 2UL, 2, pool);

      data1.clear();
      CHECK_EQUAL(0UL, data1.size());
      CHECK_EQUAL(SIZE / 2UL, data2.size());

      // Do it again to check that clear() didn't screw up the internals.
      data1.resize(SIZE / 2UL);
      CHECK_EQUAL(SIZE / 2UL, data1.size());
      CHECK_EQUAL(SIZE / 2UL, data2.size());
      data1.clear();
      CHECK_EQUAL(0UL, data1.size());
      CHECK_EQUAL(SIZE / 2UL, data2.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Pool pool;
      CompareData compare_data(half_data.begin(), half_data.end());
      DataNDC data1(pool);
      DataNDC data2(pool);

      CHECK_EQUAL(pool.available(), data1.available());
      CHECK_EQUAL(pool.available(), data2.available());

      data1.assign(compare_data.begin(), compare_data.end());
      CHECK(!data1.empty());
      CHECK(data2.empty());
      CHECK_EQUAL(pool.available(), data1.available());
      CHECK_EQUAL(pool.available(), data2.available());
      data2.assign(compare_data.begin(), compare_data.end());
      CHECK(!data1.empty());
      CHECK(!data2.empty());
      CHECK_EQUAL(pool.available(), data1.available());
      CHECK_EQUAL(pool.available(), data2.available());

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      Pool pool;
      const size_t INITIAL_SIZE = SIZE / 2;
      const ItemNDC VALUE = ItemNDC("1");

      CompareData compare_data(INITIAL_SIZE, VALUE);
      DataNDC data1(pool);
      DataNDC data2(pool);

      data1.assign(INITIAL_SIZE, VALUE);
      data2.assign(INITIAL_SIZE, VALUE);

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      Pool pool;
      const ItemNDC VALUE = ItemNDC("1");

      DataNDC data1(pool);
      DataNDC data2(pool);

      data1.assign(SIZE, VALUE);

      CHECK_THROW(data2.assign(SIZE + 1, VALUE), etl::list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      Pool pool;
      const size_t INITIAL_SIZE  = (SIZE / 2) - 3;
      const ItemNDC VALUE        = ItemNDC("1");
      const ItemNDC INSERT_VALUE = ItemNDC("2");

      CompareData compare_data(INITIAL_SIZE, VALUE);
      DataNDC data1(INITIAL_SIZE, VALUE, pool);
      DataNDC data2(INITIAL_SIZE, VALUE, pool);

      size_t offset = 2UL;

      DataNDC::iterator i_data1 = data1.begin();
      std::advance(i_data1, offset);

      DataNDC::iterator i_data2 = data2.begin();
      std::advance(i_data2, offset);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data1.insert(i_data1, INSERT_VALUE);
      data2.insert(i_data2, INSERT_VALUE);
      compare_data.insert(i_compare_data, INSERT_VALUE);
      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

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

      data1.insert(i_data1, INSERT_VALUE);
      data2.insert(i_data2, INSERT_VALUE);
      compare_data.insert(i_compare_data, INSERT_VALUE);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);

      i_data1        = data1.end();
      i_data2        = data2.end();
      i_compare_data = compare_data.end();

      data1.insert(i_data1, VALUE);
      data2.insert(i_data2, VALUE);
      compare_data.insert(i_compare_data, VALUE);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_position_value)
    {
      Pool pool;
      const size_t INITIAL_SIZE = 4UL;
      const ItemNDC VALUE = ItemNDC("1");
      const std::string INSERT_VALUE = "2";

      CompareData compare_data(INITIAL_SIZE, VALUE);
      DataNDC data(INITIAL_SIZE, VALUE, pool);

      size_t offset = 2UL;

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, offset);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.emplace(i_data, INSERT_VALUE);
      compare_data.emplace(i_compare_data, INSERT_VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      offset = 0;

      i_data = data.begin();
      std::advance(i_data, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.emplace(i_data, INSERT_VALUE);
      compare_data.emplace(i_compare_data, INSERT_VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      i_data = data.end();
      i_compare_data = compare_data.end();

      data.emplace(i_data, VALUE);
      compare_data.emplace(i_compare_data, VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      Pool pool;
      std::vector<ItemNDC> test1 = { ItemNDC("1"), ItemNDC("2"), ItemNDC("3"), ItemNDC("4"), ItemNDC("5") };
      std::vector<ItemNDC> test2 = { ItemNDC("6"), ItemNDC("7"), ItemNDC("8"), ItemNDC("9"), ItemNDC("10") };

      CompareData compare_data(test1.begin(), test1.end());
      DataNDC data(test1.begin(), test1.end(), pool);

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
      Pool pool;
      CompareData compare_data1;
      CompareData compare_data2;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data1.push_front(ItemNDC("0"));
      compare_data1.push_front(ItemNDC("1"));
      compare_data1.push_front(ItemNDC("2"));
      compare_data1.push_front(ItemNDC("3"));
      compare_data1.push_front(ItemNDC("4"));

      compare_data2.push_front(ItemNDC("5"));
      compare_data2.push_front(ItemNDC("6"));
      compare_data2.push_front(ItemNDC("7"));
      compare_data2.push_front(ItemNDC("8"));
      compare_data2.push_front(ItemNDC("9"));

      CHECK_NO_THROW(data1.push_front(ItemNDC("0")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("4")));

      CHECK_NO_THROW(data2.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("6")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("7")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("8")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("9")));

      CHECK_EQUAL(compare_data1.size(), data1.size());
      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_front)
    {
      Pool pool;
      CompareData compare_data1;
      CompareData compare_data2;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data1.emplace_front("0");
      compare_data1.emplace_front("1");
      compare_data1.emplace_front("2");
      compare_data1.emplace_front("3");
      compare_data1.emplace_front("4");

      compare_data2.emplace_front("5");
      compare_data2.emplace_front("6");
      compare_data2.emplace_front("7");
      compare_data2.emplace_front("8");
      compare_data2.emplace_front("9");

      CHECK_NO_THROW(data1.emplace_front("0"));
      CHECK_NO_THROW(data1.emplace_front("1"));
      CHECK_NO_THROW(data1.emplace_front("2"));
      CHECK_NO_THROW(data1.emplace_front("3"));
      CHECK_NO_THROW(data1.emplace_front("4"));

      CHECK_NO_THROW(data2.emplace_front("5"));
      CHECK_NO_THROW(data2.emplace_front("6"));
      CHECK_NO_THROW(data2.emplace_front("7"));
      CHECK_NO_THROW(data2.emplace_front("8"));
      CHECK_NO_THROW(data2.emplace_front("9"));

      CHECK_EQUAL(compare_data1.size(), data1.size());
      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data2.begin());
      CHECK(are_equal);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_excess)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      CHECK_NO_THROW(data1.push_front(ItemNDC("0")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("1")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("2")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("3")));
      CHECK_NO_THROW(data1.push_front(ItemNDC("4")));

      CHECK_NO_THROW(data2.push_front(ItemNDC("5")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("6")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("7")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("8")));
      CHECK_NO_THROW(data2.push_front(ItemNDC("9")));

      CHECK_THROW(data1.push_front(ItemNDC("10")) , etl::list_full);
      CHECK_THROW(data2.push_front(ItemNDC("10")) , etl::list_full);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_pop_front)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      for (size_t i = 0UL; i < data1.max_size(); ++i)
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
    TEST_FIXTURE(SetupFixture, test_push_front_pop_back)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      for (size_t i = 0UL; i < data1.max_size(); ++i)
      {
        CHECK_NO_THROW(data1.push_front(ItemNDC("1")));
        CHECK_NO_THROW(data2.push_front(ItemNDC("2")));
        data1.pop_back();
        data2.pop_back();
      }

      CHECK(data1.empty());
      CHECK(data2.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_pop_front)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      for (size_t i = 0UL; i < data1.max_size(); ++i)
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
    TEST_FIXTURE(SetupFixture, test_push_back_pop_back)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      for (size_t i = 0UL; i < data1.max_size(); ++i)
      {
        CHECK_NO_THROW(data1.push_front(ItemNDC("1")));
        CHECK_NO_THROW(data2.push_front(ItemNDC("2")));
        data1.pop_back();
        data2.pop_back();
      }

      CHECK(data1.empty());
      CHECK(data2.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      Pool pool;
      CompareData compare_data1;
      CompareData compare_data2;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data1.push_back(ItemNDC("0"));
      compare_data1.push_back(ItemNDC("1"));
      compare_data1.push_back(ItemNDC("2"));
      compare_data1.push_back(ItemNDC("3"));
      compare_data1.push_back(ItemNDC("4"));

      compare_data2.push_back(ItemNDC("5"));
      compare_data2.push_back(ItemNDC("6"));
      compare_data2.push_back(ItemNDC("7"));
      compare_data2.push_back(ItemNDC("8"));
      compare_data2.push_back(ItemNDC("9"));

      CHECK_NO_THROW(data1.push_back(ItemNDC("0")));
      CHECK_NO_THROW(data1.push_back(ItemNDC("1")));
      CHECK_NO_THROW(data1.push_back(ItemNDC("2")));
      CHECK_NO_THROW(data1.push_back(ItemNDC("3")));
      CHECK_NO_THROW(data1.push_back(ItemNDC("4")));

      CHECK_NO_THROW(data2.push_back(ItemNDC("5")));
      CHECK_NO_THROW(data2.push_back(ItemNDC("6")));
      CHECK_NO_THROW(data2.push_back(ItemNDC("7")));
      CHECK_NO_THROW(data2.push_back(ItemNDC("8")));
      CHECK_NO_THROW(data2.push_back(ItemNDC("9")));

      CHECK_EQUAL(compare_data1.size(), data1.size());
      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back)
    {
      Pool pool;
      CompareData compare_data1;
      CompareData compare_data2;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data1.emplace_back(ItemNDC("0"));
      compare_data1.emplace_back(ItemNDC("1"));
      compare_data1.emplace_back(ItemNDC("2"));
      compare_data1.emplace_back(ItemNDC("3"));
      compare_data1.emplace_back(ItemNDC("4"));

      compare_data2.emplace_back(ItemNDC("5"));
      compare_data2.emplace_back(ItemNDC("6"));
      compare_data2.emplace_back(ItemNDC("7"));
      compare_data2.emplace_back(ItemNDC("8"));
      compare_data2.emplace_back(ItemNDC("9"));

      CHECK_NO_THROW(data1.emplace_back(ItemNDC("0")));
      CHECK_NO_THROW(data1.emplace_back(ItemNDC("1")));
      CHECK_NO_THROW(data1.emplace_back(ItemNDC("2")));
      CHECK_NO_THROW(data1.emplace_back(ItemNDC("3")));
      CHECK_NO_THROW(data1.emplace_back(ItemNDC("4")));

      CHECK_NO_THROW(data2.emplace_back(ItemNDC("5")));
      CHECK_NO_THROW(data2.emplace_back(ItemNDC("6")));
      CHECK_NO_THROW(data2.emplace_back(ItemNDC("7")));
      CHECK_NO_THROW(data2.emplace_back(ItemNDC("8")));
      CHECK_NO_THROW(data2.emplace_back(ItemNDC("9")));

      CHECK_EQUAL(compare_data1.size(), data1.size());
      CHECK_EQUAL(compare_data2.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), compare_data1.begin());
      CHECK(are_equal);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      data1.push_back(ItemNDC("0"));
      data1.push_back(ItemNDC("1"));
      data1.push_back(ItemNDC("2"));
      data1.push_back(ItemNDC("3"));
      data1.push_back(ItemNDC("4"));
      data1.push_back(ItemNDC("5"));
      data1.push_back(ItemNDC("6"));
      data1.push_back(ItemNDC("7"));
      data1.push_back(ItemNDC("8"));
      data1.push_back(ItemNDC("9"));

      CHECK_THROW(data1.push_back(ItemNDC("10")), etl::list_full);
      CHECK_THROW(data2.push_back(ItemNDC("11")), etl::list_full);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      Pool pool;
      CompareData compare_data(half_data.begin(), half_data.end());
      DataNDC data1(half_data.begin(), half_data.end(), pool);
      DataNDC data2(half_data.begin(), half_data.end(), pool);

      compare_data.pop_back();
      compare_data.pop_back();

      data1.pop_back();
      data1.pop_back();

      data2.pop_back();
      data2.pop_back();

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_front_exception)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      CHECK_THROW(data1.pop_front(), etl::list_empty);
      CHECK_THROW(data2.pop_front(), etl::list_empty);

      CHECK(pool.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back_exception)
    {
      Pool pool;
      DataNDC data1(pool);
      DataNDC data2(pool);

      CHECK_THROW(data1.pop_back(), etl::list_empty);
      CHECK_THROW(data2.pop_back(), etl::list_empty);

      CHECK(pool.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_iterator)
    {
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end(), pool);

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
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end(), pool);

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, 2);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 2);

      i_compare_data = compare_data.erase(i_compare_data);
      i_data = data.erase(i_data);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase(compare_data.begin());
      i_data = data.erase(data.begin());

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
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end(), pool);

      DataNDC::const_iterator i_data_1 = data.cbegin();
      std::advance(i_data_1, 2);

      DataNDC::const_iterator i_data_2 = data.cbegin();
      std::advance(i_data_2, 4);

      CompareData::const_iterator i_compare_data_1 = compare_data.cbegin();
      std::advance(i_compare_data_1, 2);

      CompareData::const_iterator i_compare_data_2 = compare_data.cbegin();
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
      Pool pool;
      DataNDC data1(half_data.begin(), half_data.end(), pool);
      DataNDC data2(half_data.begin(), half_data.end(), pool);

      DataNDC::iterator itr = data1.erase(data1.cbegin(), data1.cend());
      CHECK(itr == data1.cbegin());
      CHECK(data1.empty());
      CHECK(!data2.empty());

      data2.erase(data2.begin(), data2.end());

      CHECK(data1.empty());
      CHECK(data2.empty());

      CHECK(pool.empty());

      // Check that it is still in a valid state.
      data1.push_back(ItemNDC("1"));
      CHECK(!data1.empty());
      CHECK_EQUAL(1U, data1.size());

      data2.push_back(ItemNDC("2"));
      CHECK(!data2.empty());
      CHECK_EQUAL(1U, data2.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      Pool pool;
      DataNDC data1(half_data.begin(), half_data.end(), pool);
      DataNDC data2(half_data.begin(), half_data.end(), pool);
      CompareData compare_data(half_data.begin(), half_data.end());

      CHECK_EQUAL(compare_data.front(), data1.front());
      CHECK_EQUAL(compare_data.front(), data2.front());

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      Pool pool;
      const DataNDC data1(half_data.begin(), half_data.end(), pool);
      const DataNDC data2(half_data.begin(), half_data.end(), pool);
      const CompareData compare_data(half_data.begin(), half_data.end());

      CHECK_EQUAL(compare_data.front(), data1.front());
      CHECK_EQUAL(compare_data.front(), data2.front());

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      Pool pool;
      DataNDC data1(half_data.begin(), half_data.end(), pool);
      DataNDC data2(half_data.begin(), half_data.end(), pool);
      CompareData compare_data(half_data.begin(), half_data.end());

      CHECK_EQUAL(compare_data.back(), data1.back());
      CHECK_EQUAL(compare_data.back(), data2.back());

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      Pool pool;
      const DataNDC data1(half_data.begin(), half_data.end(), pool);
      const DataNDC data2(half_data.begin(), half_data.end(), pool);
      const CompareData compare_data(half_data.begin(), half_data.end());

      CHECK_EQUAL(compare_data.back(), data1.back());
      CHECK_EQUAL(compare_data.back(), data2.back());

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Pool pool;
      DataNDC data1(half_data.begin(), half_data.end(), pool);
      DataNDC data2(pool);
      CompareData compare_data(half_data.begin(), half_data.end());

      data2 = data1;

      CHECK_EQUAL(data1.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), data2.begin());
      CHECK(are_equal);

      CHECK(pool.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      Pool4 pool;
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
    TEST_FIXTURE(SetupFixture, test_assignment_interface)
    {
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(pool);

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
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC other_data(pool);

      other_data = data;
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
      Pool pool;
      CompareData compare_data(non_unique_data.begin(), non_unique_data.end());
      DataNDC data1(non_unique_data.begin(), non_unique_data.end(), pool);
      DataNDC data2(non_unique_data.begin(), non_unique_data.end(), pool);

      compare_data.unique();
      data1.unique();
      data2.unique();

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique_trivial)
    {
      Pool pool;
      CompareData compare_data;
      DataNDC data(pool);

      compare_data.unique();
      data.unique();

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove)
    {
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      compare_data.remove(ItemNDC("7"));
      data1.remove(ItemNDC("7"));
      data2.remove(ItemNDC("7"));

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove_if)
    {
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      compare_data.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));
      data1.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));
      data2.remove_if(std::bind(std::equal_to<ItemNDC>(), std::placeholders::_1, ItemNDC("7")));

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse)
    {
      Pool pool;
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data1(sorted_data.begin(), sorted_data.end(), pool);
      DataNDC data2(sorted_data.begin(), sorted_data.end(), pool);

      compare_data.reverse();
      data1.reverse();
      data2.reverse();

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(data1.size(), size_t(std::distance(data1.begin(), data1.end())));

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      CHECK_EQUAL(compare_data.size(), data2.size());
      CHECK_EQUAL(data2.size(), size_t(std::distance(data2.begin(), data2.end())));

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort)
    {
      Pool pool;
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      DataNDC data1(unsorted_data.begin(), unsorted_data.end(), pool);
      DataNDC data2(unsorted_data.begin(), unsorted_data.end(), pool);

      compare_data.sort();
      data1.sort();
      data2.sort();

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_is_stable)
    {
      Pool pool;
      CompareData compare_data(stable_sort_data.begin(), stable_sort_data.end());
      DataNDC data1(stable_sort_data.begin(), stable_sort_data.end(), pool);
      DataNDC data2(stable_sort_data.begin(), stable_sort_data.end(), pool);

      compare_data.sort();
      data1.sort();
      data2.sort();

      CompareData::const_iterator citr = compare_data.begin();
      DataNDC::const_iterator     ditr1 = data1.begin();
      DataNDC::const_iterator     ditr2 = data2.begin();

      while (ditr1 != data1.end())
      {
        CHECK_EQUAL(citr->index, ditr1->index);
        CHECK_EQUAL(citr->index, ditr2->index);

        ++citr;
        ++ditr1;
        ++ditr2;
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_trivial)
    {
      Pool pool;
      CompareData compare_data;
      DataNDC data1(pool);
      DataNDC data2(pool);

      compare_data.sort();
      data1.sort();
      data2.sort();

      CHECK_EQUAL(compare_data.size(), data1.size());
      CHECK_EQUAL(compare_data.size(), data2.size());

      are_equal = std::equal(data1.begin(), data1.end(), compare_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data2.begin(), data2.end(), compare_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_same)
    {
      Pool2 pool;
      CompareData compare_data(unsorted_data2.begin(), unsorted_data2.end());
      DataNDC data(unsorted_data2.begin(), unsorted_data2.end(), pool);

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
      Pool4 pool;
      CompareData compare_data(unsorted_data2.begin(), unsorted_data2.end());
      CompareData compare_data2(unsorted_data2.begin(), unsorted_data2.end());

      DataNDC data(unsorted_data2.begin(), unsorted_data2.end(), pool);
      DataNDC data2(unsorted_data2.begin(), unsorted_data2.end(), pool);

      CompareData::iterator compare_from;
      CompareData::iterator compare_to;

      DataNDC::iterator from;
      DataNDC::iterator to;

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
      compare_data.assign(unsorted_data2.begin(), unsorted_data2.end());
      compare_data2.assign(unsorted_data2.begin(), unsorted_data2.end());

      data.assign(unsorted_data2.begin(), unsorted_data2.end());
      data2.assign(unsorted_data2.begin(), unsorted_data2.end());

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
      compare_data.assign(unsorted_data2.begin(), unsorted_data2.end());
      compare_data2.assign(unsorted_data2.begin(), unsorted_data2.end());

      data.assign(unsorted_data2.begin(), unsorted_data2.end());
      data2.assign(unsorted_data2.begin(), unsorted_data2.end());

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
      compare_data.assign(unsorted_data2.begin(), unsorted_data2.end());
      compare_data2.assign(unsorted_data2.begin(), unsorted_data2.end());

      data.assign(unsorted_data2.begin(), unsorted_data2.end());
      data2.assign(unsorted_data2.begin(), unsorted_data2.end());

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
      Pool2 pool;
      CompareData compare_data(unsorted_data2.begin(), unsorted_data2.end());
      DataNDC data(unsorted_data2.begin(), unsorted_data2.end(), pool);

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

      DataNDC data2(data, data.get_pool());
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
      Pool4 pool;
      CompareData compare_data(unsorted_data2.begin(), unsorted_data2.end());
      CompareData compare_data2(unsorted_data2.begin(), unsorted_data2.end());
      DataNDC data(unsorted_data2.begin(), unsorted_data2.end(), pool);
      DataNDC data2(unsorted_data2.begin(), unsorted_data2.end(), pool);

      CompareData::iterator compare_begin;
      CompareData::iterator compare_end;
      CompareData::iterator compare_to;

      DataNDC::iterator begin;
      DataNDC::iterator end;
      DataNDC::iterator to;

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
      compare_data.assign(unsorted_data2.begin(), unsorted_data2.end());
      compare_data2.assign(unsorted_data2.begin(), unsorted_data2.end());

      data.assign(unsorted_data2.begin(), unsorted_data2.end());
      data2.assign(unsorted_data2.begin(), unsorted_data2.end());

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
      compare_data.assign(unsorted_data2.begin(), unsorted_data2.end());
      compare_data2.assign(unsorted_data2.begin(), unsorted_data2.end());

      data.assign(unsorted_data2.begin(), unsorted_data2.end());
      data2.assign(unsorted_data2.begin(), unsorted_data2.end());

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
      Pool4 pool;
      CompareData compare_data(unsorted_data2.begin(), unsorted_data2.end());
      CompareData compare_data2(unsorted_data2.begin(), unsorted_data2.end());

      DataNDC data(unsorted_data2.begin(), unsorted_data2.end(), pool);
      DataNDC data2(unsorted_data2.begin(), unsorted_data2.end(), pool);

      CompareData::iterator compare_to;
      DataNDC::iterator to;

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
      compare_data.assign(unsorted_data2.begin(), unsorted_data2.end());
      compare_data2.assign(unsorted_data2.begin(), unsorted_data2.end());

      data.assign(unsorted_data2.begin(), unsorted_data2.end());
      data2.assign(unsorted_data2.begin(), unsorted_data2.end());

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
      compare_data.assign(unsorted_data2.begin(), unsorted_data2.end());
      compare_data2.assign(unsorted_data2.begin(), unsorted_data2.end());

      data.assign(unsorted_data2.begin(), unsorted_data2.end());
      data2.assign(unsorted_data2.begin(), unsorted_data2.end());

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
      Pool4 pool;
      DataNDC data0(merge_data0.begin(), merge_data0.end(), pool);
      DataNDC data1(merge_data1.begin(), merge_data1.end(), pool);

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
      Pool4 pool;
      DataNDC data0(merge_data0.begin(), merge_data0.end(), pool);
      DataNDC data2(merge_data2.begin(), merge_data2.end(), pool);

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
      Pool4 pool;
      DataNDC data0(merge_data0.begin(), merge_data0.end(), pool);
      DataNDC data3(merge_data3.begin(), merge_data3.end(), pool);

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
      Pool4 pool;
      DataNDC data0(merge_data0.begin(), merge_data0.end(), pool);
      DataNDC data4(merge_data4.begin(), merge_data4.end(), pool);

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
      Pool4 pool;
      DataNDC data0(merge_data0.begin(), merge_data0.end(), pool);
      DataNDC data1(merge_data1.begin(), merge_data1.end(), pool);

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
      Pool pool;
      DataNDC data0(unsorted_data.begin(), unsorted_data.end(), pool);
      DataNDC data1(unsorted_data.begin(), unsorted_data.end(), pool);

      CHECK_THROW(data0.merge(data1), etl::list_unsorted);
    }
  };
}
