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
#include "ExtraCheckMacros.h"

#include "data.h"

#include "../forward_list.h"

#include <algorithm>
#include <array>
#include <forward_list>
#include <vector>
#include <string>

namespace 
{		
  SUITE(test_forward_list)
  {
    const size_t SIZE = 10;

    typedef TestDataDC<std::string>  ItemDC;
    typedef TestDataNDC<std::string> ItemNDC;

    typedef etl::forward_list<ItemDC, SIZE>  DataDC;
    typedef etl::forward_list<ItemNDC, SIZE> DataNDC;

    typedef std::forward_list<ItemNDC> CompareDataNDC;
    typedef std::vector<ItemNDC> InitialDataNDC;

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

      CHECK(data.empty());
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      const size_t INITIAL_SIZE = 4;
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
      const size_t INITIAL_SIZE = 4;
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
      DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK(!data.empty());
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
      DataNDC data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 4;
      const size_t NEW_SIZE     = 8;
      const ItemNDC VALUE("1");

      DataNDC data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE, VALUE);

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE, VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t INITIAL_SIZE = 4;
      DataDC data(INITIAL_SIZE);

      CHECK_THROW(data.resize(data.max_size() + 1), etl::forward_list_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());
      data.clear();

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data;

      // Do it twice. We should only get one copy.
      data.assign(compare_data.begin(), compare_data.end());
      data.assign(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 4;
      const ItemNDC VALUE("1");

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      DataNDC data;

      // Do it twice. We should only get one copy.
      data.assign(INITIAL_SIZE, VALUE);
      data.assign(INITIAL_SIZE, VALUE);

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
      const size_t INITIAL_SIZE = 4;
      const ItemNDC VALUE("1");
      const ItemNDC INSERT_VALUE("2");

      CompareDataNDC compare_data(INITIAL_SIZE, VALUE);
      DataNDC data(INITIAL_SIZE, VALUE);

      size_t offset = 2;

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, offset);

      CompareDataNDC::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert_after(i_data, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, INSERT_VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      offset = 0;

      i_data = data.begin();
      std::advance(i_data, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert_after(i_data, INSERT_VALUE);
      compare_data.insert_after(i_compare_data, INSERT_VALUE);

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

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

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

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
        CHECK_NO_THROW(data.push_front(ItemNDC("1")));
        data.pop_front();
      }
      
      CHECK(data.empty());
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

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase_after(compare_data.begin());
      i_data         = data.erase_after(data.begin());

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

      compare_data.erase_after(i_compare_data_1, i_compare_data_2);

      data.erase_after(i_data_1, i_data_2);

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
    TEST_FIXTURE(SetupFixture, test_unique)
    {
      CompareDataNDC compare_data(non_unique_data.begin(), non_unique_data.end());
      DataNDC data(non_unique_data.begin(), non_unique_data.end());

      compare_data.unique();
      data.unique();

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

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove_if)
    {
      CompareDataNDC compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      compare_data.remove_if(std::bind2nd(std::equal_to<ItemNDC>(), ItemNDC("7")));
      data.remove_if(std::bind2nd(std::equal_to<ItemNDC>(), ItemNDC("7")));

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

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }
  };
}
