/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove

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

#include "../intrusive_forward_list.h"

#include <algorithm>
#include <array>
#include <forward_list>
#include <vector>
#include <list>
#include <string>

typedef TestDataDC<std::string>  ItemDC;
typedef TestDataNDC<std::string> ItemNDC;
namespace
{
  class ItemDCNode : public etl::intrusive_forward_list_node<2>
  {
  public:

    ItemDCNode(const std::string& text)
      : data(text)
    {
    }

    ItemDC data;
  };

  const size_t INDEXES = 2;

  class ItemNDCNode : public etl::intrusive_forward_list_node<INDEXES>
  {
  public:

    ItemNDCNode(const std::string& text)
      : data(text)
    {
    }

    bool operator <(const ItemNDCNode& other) const
    {
      return data < other.data;
    }

    ItemNDC data;
  };

  bool operator ==(const ItemDCNode& lhs, const ItemDCNode& rhs)
  {
    return lhs.data == rhs.data;
  }

  bool operator ==(const ItemNDCNode& lhs, const ItemNDCNode& rhs)
  {
    return lhs.data == rhs.data;
  }

  std::ostream& operator << (std::ostream& os, const ItemNDCNode& node)
  {
    os << node.data;
    return os;
  }

  struct CompareItemNDCNode
  {
    bool operator ()(const ItemNDCNode& lhs, const ItemNDCNode& rhs) const
    {
      return lhs.data < rhs.data;
    }
  };

  struct EqualItemNDCNode
  {
    bool operator ()(const ItemNDCNode& lhs, const ItemNDCNode& rhs) const
    {
      return lhs.data == rhs.data;
    }
  };
}

namespace 
{		
  SUITE(test_forward_list)
  {   
    typedef etl::intrusive_forward_list<ItemDCNode>  DataDC;
    typedef etl::intrusive_forward_list<ItemNDCNode> DataNDC;

    typedef std::vector<ItemNDCNode> InitialDataNDC;

    InitialDataNDC unsorted_data;
    InitialDataNDC sorted_data;
    InitialDataNDC non_unique_data;
    InitialDataNDC unique_data;
    InitialDataNDC small_data;

    bool are_equal;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        unsorted_data   = { ItemNDCNode("1"), ItemNDCNode("0"), ItemNDCNode("3"), ItemNDCNode("2"), ItemNDCNode("5"), ItemNDCNode("4"), ItemNDCNode("7"), ItemNDCNode("6"), ItemNDCNode("9"), ItemNDCNode("8") };
        sorted_data     = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5"), ItemNDCNode("6"), ItemNDCNode("7"), ItemNDCNode("8"), ItemNDCNode("9") };
        non_unique_data = { ItemNDCNode("0"), ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5") };
        unique_data     = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5") };
        small_data      = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5") };
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataNDC data0(0);
      DataNDC data1(1);

      CHECK(data0.empty());
      CHECK(data1.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());

      CHECK(!data0.empty());
      CHECK_EQUAL(sorted_data.size(), data0.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());

      are_equal = std::equal(data0.cbegin(), data0.cend(), sorted_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());
      data0.clear();

      CHECK(data0.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      DataNDC data0(0);

      // Do it twice. We should only get one copy.
      data0.assign(sorted_data.begin(), sorted_data.end());
      data0.assign(sorted_data.begin(), sorted_data.end());

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range_two_lists_same)
    {
      DataNDC data0(0);
      DataNDC data1(1);

      data0.assign(sorted_data.begin(), sorted_data.end());
      data1.assign(sorted_data.begin(), sorted_data.end());

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_two_lists_different)
    {
      std::list<ItemNDCNode> compare0;
      std::list<ItemNDCNode> compare1;
      
      DataNDC data0(0);
      DataNDC data1(1);

      ItemNDCNode node0("0");
      ItemNDCNode node1("1");
      ItemNDCNode node2("2");
      ItemNDCNode node3("3");
      ItemNDCNode node4("4");
      ItemNDCNode node5("5");
      ItemNDCNode node6("6");
      ItemNDCNode node7("7");

      compare0.push_front(node0);
      compare0.push_front(node1);
      compare0.push_front(node2);
      compare0.push_front(node4);
      compare0.push_front(node6);
      compare0.push_front(node7);
      
      data0.push_front(node0);
      data0.push_front(node1);
      data0.push_front(node2);
      data0.push_front(node4);
      data0.push_front(node6);
      data0.push_front(node7);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);
      CHECK_EQUAL(6, data0.size());
      CHECK_EQUAL(6, std::distance(data0.begin(), data0.end()));

      compare1.push_front(node0);
      compare1.push_front(node1);
      compare1.push_front(node3);
      compare1.push_front(node4);
      compare1.push_front(node5);
      compare1.push_front(node7);

      data1.push_front(node0);
      data1.push_front(node1);
      data1.push_front(node3);
      data1.push_front(node4);
      data1.push_front(node5);
      data1.push_front(node7);

      are_equal = std::equal(data1.begin(), data1.end(), compare1.begin());
      CHECK(are_equal);
      CHECK_EQUAL(6, data1.size());
      CHECK_EQUAL(6, std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_position_value)
    {
      ItemNDCNode INSERT_VALUE1 = ItemNDCNode("1");
      ItemNDCNode INSERT_VALUE2 = ItemNDCNode("2");

      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());
      DataNDC data1(1, sorted_data.begin(), sorted_data.end());

      size_t offset = 2;

      DataNDC::iterator i_data = data0.begin();
      std::advance(i_data, offset);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset + 1);

      data0.insert_after(i_data, INSERT_VALUE1);
      compare_data.insert(i_compare_data, INSERT_VALUE1);

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), std::distance(data0.begin(), data0.end()));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), std::distance(data1.begin(), data1.end()));

      offset = 0;

      i_data = data0.begin();
      std::advance(i_data, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset + 1);

      std::vector<ItemNDCNode> temp(data0.begin(), data0.end());

      data0.insert_after(i_data, INSERT_VALUE2);
      compare_data.insert(i_compare_data, INSERT_VALUE2);

      temp.assign(data0.begin(), data0.end());

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), std::distance(data0.begin(), data0.end()));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_range)
    {
      std::vector<ItemNDCNode> test1 = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4") };
      std::vector<ItemNDCNode> test2 = { ItemNDCNode("5"), ItemNDCNode("6"), ItemNDCNode("7"), ItemNDCNode("8"), ItemNDCNode("9") };
      std::vector<ItemNDCNode> compare(test2);
      compare.insert(compare.end(), test1.begin(), test1.end());

      DataNDC data0(0, test1.begin(), test1.end());
      DataNDC data1(1, test1.begin(), test1.end());

      data0.insert_after(data0.before_begin(), test2.begin(), test2.end());

      are_equal = std::equal(data0.begin(), data0.end(), compare.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), test1.begin());
      CHECK(are_equal);
      CHECK_EQUAL(test1.size(), data1.size());
      CHECK_EQUAL(test1.size(), std::distance(data1.begin(), data1.end()));

      compare.assign(test1.begin(), test1.end());
      data0.assign(test1.begin(), test1.end());

      std::vector<ItemNDCNode>::iterator icd = compare.begin();
      DataNDC::iterator id = data0.begin();

      std::advance(icd, 4);
      std::advance(id, 3);

      compare.insert(icd, test2.begin(), test2.end());
      data0.insert_after(id, test2.begin(), test2.end());

      std::vector<ItemNDCNode> out(data0.begin(), data0.end());

      are_equal = std::equal(data0.begin(), data0.end(), compare.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), test1.begin());
      CHECK(are_equal);
      CHECK_EQUAL(test1.size(), data1.size());
      CHECK_EQUAL(test1.size(), std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front)
    {
      std::list<ItemNDCNode> compare_data;
      DataNDC data0(0);

      ItemNDCNode node1("1");
      ItemNDCNode node2("2");
      ItemNDCNode node3("3");
      ItemNDCNode node4("4");
      ItemNDCNode node5("5");
      ItemNDCNode node6("6");

      compare_data.push_front(node1);
      compare_data.push_front(node2);
      compare_data.push_front(node3);
      compare_data.push_front(node4);
      compare_data.push_front(node5);
      compare_data.push_front(node6);

      CHECK_NO_THROW(data0.push_front(node1));
      CHECK_NO_THROW(data0.push_front(node2));
      CHECK_NO_THROW(data0.push_front(node3));
      CHECK_NO_THROW(data0.push_front(node4));
      CHECK_NO_THROW(data0.push_front(node5));
      CHECK_NO_THROW(data0.push_front(node6));

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(6, data0.size());
      CHECK_EQUAL(6, std::distance(data0.begin(), data0.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_pop_front)
    {
      DataNDC data0(0);
      DataNDC data1(1);

      ItemNDCNode node1("1");
      ItemNDCNode node2("2");
      ItemNDCNode node3("3");
      ItemNDCNode node4("4");
      ItemNDCNode node5("5");
      ItemNDCNode node6("6");

      data0.push_front(node1);
      data0.push_front(node2);
      data0.push_front(node3);
      data0.push_front(node4);
      data0.push_front(node5);
      data0.push_front(node6);

      data1.push_front(node1);
      data1.push_front(node2);
      data1.push_front(node3);
      data1.push_front(node4);
      data1.push_front(node5);
      data1.push_front(node6);

      CHECK_EQUAL(6, data0.size());
      CHECK_EQUAL(6, std::distance(data0.begin(), data0.end()));
      CHECK(!data0.empty());

      data0.pop_front();
      data0.pop_front();
      data0.pop_front();
      data0.pop_front();
      data0.pop_front();

      CHECK_EQUAL(1, data0.size());
      CHECK_EQUAL(1, std::distance(data0.begin(), data0.end()));
      CHECK(!data0.empty());

      data0.pop_front();

      CHECK_EQUAL(0, data0.size());
      CHECK_EQUAL(0, std::distance(data0.begin(), data0.end()));
      CHECK(data0.empty());

      CHECK_EQUAL(6, data1.size());
      CHECK_EQUAL(6, std::distance(data1.begin(), data1.end()));
      CHECK(!data1.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_single)
    {
      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());
      DataNDC data1(1, sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data = data0.begin();
      std::advance(i_data, 2);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 3);

      i_compare_data = compare_data.erase(i_compare_data);
      i_data         = data0.erase_after(i_data);

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase(compare_data.begin() + 1);
      i_data         = data0.erase_after(data0.begin());

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), std::distance(data0.begin(), data0.end()));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), std::distance(data1.begin(), data1.end()));

      are_equal = *i_data == *i_compare_data;
      CHECK(are_equal);

      // Move to the last value and erase.
      i_compare_data = compare_data.begin() + 1;
      i_compare_data = compare_data.erase(i_compare_data);

      i_data = data0.begin();
      i_data = data0.erase_after(i_data);

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), std::distance(data0.begin(), data0.end()));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), std::distance(data1.begin(), data1.end()));

      are_equal = *i_data == *i_compare_data;
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_range)
    {
      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());
      DataNDC data1(1, sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data_1 = data0.begin();
      std::advance(i_data_1, 2);

      DataNDC::iterator i_data_2 = data0.begin();
      std::advance(i_data_2, 4);

      std::vector<ItemNDCNode>::iterator i_compare_data_1 = compare_data.begin();
      std::advance(i_compare_data_1, 3);

      std::vector<ItemNDCNode>::iterator i_compare_data_2 = compare_data.begin();
      std::advance(i_compare_data_2, 4);

      std::vector<ItemNDCNode>::iterator i_compare_result = compare_data.erase(i_compare_data_1, i_compare_data_2);

      DataNDC::iterator i_result = data0.erase_after(i_data_1, i_data_2);

      CHECK_EQUAL(*i_compare_result, *i_result);

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), std::distance(data0.begin(), data0.end()));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_range_end)
    {
      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());
      DataNDC data1(1, sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data = data0.begin();
      std::advance(i_data, 4);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 5);

      std::vector<ItemNDCNode>::iterator i_compare_result = compare_data.erase(i_compare_data, compare_data.end());

      DataNDC::iterator i_result = data0.erase_after(i_data, data0.end());

      CHECK(i_result == data0.end());

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), std::distance(data0.begin(), data0.end()));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_all)
    {
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());

      data0.erase_after(data0.before_begin(), data0.end());

      CHECK(data0.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(sorted_data.front(), data0.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique)
    {
      DataNDC data0(0, non_unique_data.begin(), non_unique_data.end());
      DataNDC data1(1, non_unique_data.begin(), non_unique_data.end());

      data0.unique(EqualItemNDCNode());

      // data0 should have changed.
      are_equal = std::equal(data0.begin(), data0.end(), unique_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(unique_data.size(), data0.size());
      CHECK_EQUAL(unique_data.size(), std::distance(data0.begin(), data0.end()));

      // data1 should not have changed.
      are_equal = std::equal(data1.begin(), data1.end(), non_unique_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(non_unique_data.size(), data1.size());
      CHECK_EQUAL(non_unique_data.size(), std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove)
    {
      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());
      DataNDC data1(1, sorted_data.begin(), sorted_data.end());

      std::vector<ItemNDCNode>::iterator i_item = std::find(compare_data.begin(), compare_data.end(), ItemNDCNode("7"));
      compare_data.erase(i_item);
      data0.remove(ItemNDCNode("7"));

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), std::distance(data0.begin(), data0.end()));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove_if)
    {
      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());
      DataNDC data1(1, sorted_data.begin(), sorted_data.end());

      std::vector<ItemNDCNode>::iterator i_item = std::find(compare_data.begin(), compare_data.end(), ItemNDCNode("7"));
      compare_data.erase(i_item);
      data0.remove_if(std::bind2nd(std::equal_to<ItemNDCNode>(), ItemNDCNode("7")));

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), std::distance(data0.begin(), data0.end()));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse)
    {
      DataNDC data0(0, sorted_data.begin(), sorted_data.end());
      DataNDC data1(1, sorted_data.begin(), sorted_data.end());

      data0.reverse(); // Just reverse one of them.

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.rbegin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort)
    {
      DataNDC data0(0, unsorted_data.begin(), unsorted_data.end());
      DataNDC data1(1, unsorted_data.begin(), unsorted_data.end());

      data0.sort(); // Just sort one of them.

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), unsorted_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_compare)
    {
      DataNDC data0(0, unsorted_data.begin(), unsorted_data.end());
      DataNDC data1(1, unsorted_data.begin(), unsorted_data.end());

      data0.sort(CompareItemNDCNode()); // Just sort one of them.

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), unsorted_data.begin());
      CHECK(are_equal);
    }
  };
}
