/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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

#include "etl/intrusive_list.h"

#include <algorithm>
#include <array>
#include <list>
#include <vector>
#include <string>
#include <functional>

typedef TestDataDC<std::string>  ItemDC;
typedef TestDataNDC<std::string> ItemNDC;

namespace
{
  typedef etl::bidirectional_link<0> FirstLink;
  typedef etl::bidirectional_link<1> SecondLink;

  //***************************************************************************
  class ItemDCNode : public FirstLink, public SecondLink
  {
  public:

    ItemDCNode(const std::string& text)
      : data(text)
    {
    }

    ItemDC data;
  };

  //***************************************************************************
  class ItemNDCNode : public FirstLink, public SecondLink
  {
  public:

    ItemNDCNode(const std::string& text, int index = 0)
      : data(text, index)
    {
    }

    bool operator <(const ItemNDCNode& other) const
    {
      return data < other.data;
    }

    bool operator >(const ItemNDCNode& other) const
    {
      return other.data < data;
    }

    ItemNDC data;
  };

  //***************************************************************************
//  bool operator ==(const ItemDCNode& lhs, const ItemDCNode& rhs)
//  {
//    return lhs.data == rhs.data;
//  }

  bool operator ==(const ItemNDCNode& lhs, const ItemNDCNode& rhs)
  {
    return lhs.data == rhs.data;
  }

//  bool operator !=(const ItemDCNode& lhs, const ItemDCNode& rhs)
//  {
//    return lhs.data != rhs.data;
//  }

//  bool operator !=(const ItemNDCNode& lhs, const ItemNDCNode& rhs)
//  {
//    return lhs.data != rhs.data;
//  }

  std::ostream& operator << (std::ostream& os, const ItemNDCNode& node)
  {
    os << node.data;
    return os;
  }

  //***************************************************************************
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

  //***************************************************************************
  typedef etl::intrusive_list<ItemDCNode,  FirstLink>  DataDC0;
  typedef etl::intrusive_list<ItemDCNode,  SecondLink> DataDC1;
  typedef etl::intrusive_list<ItemNDCNode, FirstLink>  DataNDC0;
  typedef etl::intrusive_list<ItemNDCNode, SecondLink> DataNDC1;

  typedef std::vector<ItemNDCNode> InitialDataNDC;
}

namespace
{
  SUITE(test_intrusive_list)
  {
    InitialDataNDC stable_sort_data;
    InitialDataNDC unsorted_data;
    InitialDataNDC sorted_data;
    InitialDataNDC sorted_data2;
    InitialDataNDC non_unique_data;
    InitialDataNDC unique_data;
    InitialDataNDC small_data;
    InitialDataNDC merge_data0;
    InitialDataNDC merge_data1;
    InitialDataNDC merge_data2;
    InitialDataNDC merge_data3;
    InitialDataNDC merge_data4;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        stable_sort_data = { ItemNDCNode("1", 1), ItemNDCNode("2", 2), ItemNDCNode("3", 3), ItemNDCNode("2", 4), ItemNDCNode("0", 5), ItemNDCNode("2", 6), ItemNDCNode("7", 7), ItemNDCNode("4", 8), ItemNDCNode("4", 9), ItemNDCNode("8", 10) };
        unsorted_data    = { ItemNDCNode("1"), ItemNDCNode("0"), ItemNDCNode("3"), ItemNDCNode("2"), ItemNDCNode("5"), ItemNDCNode("4"), ItemNDCNode("7"), ItemNDCNode("6"), ItemNDCNode("9"), ItemNDCNode("8") };
        sorted_data      = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5"), ItemNDCNode("6"), ItemNDCNode("7"), ItemNDCNode("8"), ItemNDCNode("9") };
        sorted_data2     = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5"), ItemNDCNode("6"), ItemNDCNode("7"), ItemNDCNode("8"), ItemNDCNode("9") };
        non_unique_data  = { ItemNDCNode("0"), ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5") };
        unique_data      = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5") };
        small_data       = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4"), ItemNDCNode("5") };

        merge_data0 = { ItemNDCNode("1"), ItemNDCNode("1"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("5"), ItemNDCNode("7"), ItemNDCNode("8") };
        merge_data1 = { ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("6"), ItemNDCNode("9"), ItemNDCNode("9") };
        merge_data2 = { ItemNDCNode("0"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("6"), ItemNDCNode("7"), ItemNDCNode("7") };
        merge_data3 = { ItemNDCNode("0"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("6"), ItemNDCNode("7") };
        merge_data4 = { ItemNDCNode("0"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("3"), ItemNDCNode("6"), ItemNDCNode("7"), ItemNDCNode("8"), ItemNDCNode("9") };
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataNDC0 data0;
      DataNDC1 data1;

      CHECK(data0.empty());
      CHECK(data1.empty());

      CHECK(data0.begin() == data0.end());
      CHECK(data1.begin() == data1.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());

      CHECK(!data0.empty());
      CHECK_EQUAL(sorted_data.size(), data0.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_begin_end)
    {
      DataNDC0 data0;

      CHECK(data0.begin() == data0.end());

      DataNDC0::const_iterator begin = data0.begin();
      DataNDC0::const_iterator end   = data0.end();
      CHECK(begin == end);

      CHECK(data0.cbegin() == data0.cend());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin(), sorted_data.end());

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator_arrow_operator)
    {
      ItemNDCNode item1("1");
      ItemNDCNode item2("2");
      DataNDC0 data;
      data.push_back(item1);
      data.push_back(item2);

      auto iter = data.begin();
      CHECK(*(iter.operator->()) == item1);
      ++iter;
      CHECK(*(iter.operator->()) == item2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin(), sorted_data.end());

      are_equal = std::equal(data0.cbegin(), data0.cend(), sorted_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator_arrow_operator)
    {
      ItemNDCNode item1("1");
      ItemNDCNode item2("2");
      DataNDC0 data;
      data.push_back(item1);
      data.push_back(item2);

      auto iter = data.cbegin();
      CHECK(*(iter.operator->()) == item1);
      ++iter;
      CHECK(*(iter.operator->()) == item2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      FirstLink& fl0 = sorted_data[0];
      FirstLink& fl1 = sorted_data[1];
      FirstLink& fl2 = sorted_data[2];
      FirstLink& fl3 = sorted_data[3];
      FirstLink& fl4 = sorted_data[4];
      FirstLink& fl5 = sorted_data[5];
      FirstLink& fl6 = sorted_data[6];
      FirstLink& fl7 = sorted_data[7];
      FirstLink& fl8 = sorted_data[8];
      FirstLink& fl9 = sorted_data[9];

      SecondLink& sl0 = sorted_data[0];
      SecondLink& sl1 = sorted_data[1];
      SecondLink& sl2 = sorted_data[2];
      SecondLink& sl3 = sorted_data[3];
      SecondLink& sl4 = sorted_data[4];
      SecondLink& sl5 = sorted_data[5];
      SecondLink& sl6 = sorted_data[6];
      SecondLink& sl7 = sorted_data[7];
      SecondLink& sl8 = sorted_data[8];
      SecondLink& sl9 = sorted_data[9];

      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      
      CHECK_TRUE(fl0.is_linked());
      CHECK_TRUE(fl1.is_linked());
      CHECK_TRUE(fl2.is_linked());
      CHECK_TRUE(fl3.is_linked());
      CHECK_TRUE(fl4.is_linked());
      CHECK_TRUE(fl5.is_linked());
      CHECK_TRUE(fl6.is_linked());
      CHECK_TRUE(fl7.is_linked());
      CHECK_TRUE(fl8.is_linked());
      CHECK_TRUE(fl9.is_linked());

      CHECK_FALSE(sl0.is_linked());
      CHECK_FALSE(sl1.is_linked());
      CHECK_FALSE(sl2.is_linked());
      CHECK_FALSE(sl3.is_linked());
      CHECK_FALSE(sl4.is_linked());
      CHECK_FALSE(sl5.is_linked());
      CHECK_FALSE(sl6.is_linked());
      CHECK_FALSE(sl7.is_linked());
      CHECK_FALSE(sl8.is_linked());
      CHECK_FALSE(sl9.is_linked());
      
      DataNDC1 data1(sorted_data.begin(), sorted_data.end());

      CHECK_TRUE(fl0.is_linked());
      CHECK_TRUE(fl1.is_linked());
      CHECK_TRUE(fl2.is_linked());
      CHECK_TRUE(fl3.is_linked());
      CHECK_TRUE(fl4.is_linked());
      CHECK_TRUE(fl5.is_linked());
      CHECK_TRUE(fl6.is_linked());
      CHECK_TRUE(fl7.is_linked());
      CHECK_TRUE(fl8.is_linked());
      CHECK_TRUE(fl9.is_linked());

      CHECK_TRUE(sl0.is_linked());
      CHECK_TRUE(sl1.is_linked());
      CHECK_TRUE(sl2.is_linked());
      CHECK_TRUE(sl3.is_linked());
      CHECK_TRUE(sl4.is_linked());
      CHECK_TRUE(sl5.is_linked());
      CHECK_TRUE(sl6.is_linked());
      CHECK_TRUE(sl7.is_linked());
      CHECK_TRUE(sl8.is_linked());
      CHECK_TRUE(sl9.is_linked());

      data0.clear();

      CHECK_FALSE(fl0.is_linked());
      CHECK_FALSE(fl1.is_linked());
      CHECK_FALSE(fl2.is_linked());
      CHECK_FALSE(fl3.is_linked());
      CHECK_FALSE(fl4.is_linked());
      CHECK_FALSE(fl5.is_linked());
      CHECK_FALSE(fl6.is_linked());
      CHECK_FALSE(fl7.is_linked());
      CHECK_FALSE(fl8.is_linked());
      CHECK_FALSE(fl9.is_linked());

      CHECK_TRUE(sl0.is_linked());
      CHECK_TRUE(sl1.is_linked());
      CHECK_TRUE(sl2.is_linked());
      CHECK_TRUE(sl3.is_linked());
      CHECK_TRUE(sl4.is_linked());
      CHECK_TRUE(sl5.is_linked());
      CHECK_TRUE(sl6.is_linked());
      CHECK_TRUE(sl7.is_linked());
      CHECK_TRUE(sl8.is_linked());
      CHECK_TRUE(sl9.is_linked());

      data1.clear();

      CHECK_FALSE(fl0.is_linked());
      CHECK_FALSE(fl1.is_linked());
      CHECK_FALSE(fl2.is_linked());
      CHECK_FALSE(fl3.is_linked());
      CHECK_FALSE(fl4.is_linked());
      CHECK_FALSE(fl5.is_linked());
      CHECK_FALSE(fl6.is_linked());
      CHECK_FALSE(fl7.is_linked());
      CHECK_FALSE(fl8.is_linked());
      CHECK_FALSE(fl9.is_linked());

      CHECK_FALSE(sl0.is_linked());
      CHECK_FALSE(sl1.is_linked());
      CHECK_FALSE(sl2.is_linked());
      CHECK_FALSE(sl3.is_linked());
      CHECK_FALSE(sl4.is_linked());
      CHECK_FALSE(sl5.is_linked());
      CHECK_FALSE(sl6.is_linked());
      CHECK_FALSE(sl7.is_linked());
      CHECK_FALSE(sl8.is_linked());
      CHECK_FALSE(sl9.is_linked());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      bool are_equal;

      DataNDC0 data0;

      // Do it twice. We should only get one copy.
      data0.assign(sorted_data.begin(), sorted_data.end());
      data0.assign(sorted_data.begin(), sorted_data.end());

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range_two_lists_same)
    {
      bool are_equal;

      DataNDC0 data0;
      DataNDC1 data1;

      data0.assign(sorted_data.begin(), sorted_data.end());
      data1.assign(sorted_data.begin(), sorted_data.end());

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
    }

    ////*************************************************************************
    TEST_FIXTURE(SetupFixture, test_two_lists_different)
    {
      bool are_equal;

      std::list<ItemNDCNode> compare0;
      std::list<ItemNDCNode> compare1;

      DataNDC0 data0;
      DataNDC1 data1;

      static ItemNDCNode node0("0");
      static ItemNDCNode node1("1");
      static ItemNDCNode node2("2");
      static ItemNDCNode node3("3");
      static ItemNDCNode node4("4");
      static ItemNDCNode node5("5");
      static ItemNDCNode node6("6");
      static ItemNDCNode node7("7");

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
      CHECK_EQUAL(6U, data0.size());
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
      CHECK_EQUAL(6U, data1.size());
      CHECK_EQUAL(6, std::distance(data1.begin(), data1.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      bool are_equal;

      ItemNDCNode INSERT_VALUE1 = ItemNDCNode("1");
      ItemNDCNode INSERT_VALUE2 = ItemNDCNode("2");

      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      DataNDC1 data1(sorted_data.begin(), sorted_data.end());

      size_t offset = 2;

      DataNDC0::iterator i_data = data0.begin();
      std::advance(i_data, offset);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data0.insert(i_data, INSERT_VALUE1);
      compare_data.insert(i_compare_data, INSERT_VALUE1);

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), size_t(std::distance(data1.begin(), data1.end())));

      offset = 0;

      i_data = data0.begin();
      std::advance(i_data, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      std::vector<ItemNDCNode> temp(data0.begin(), data0.end());

      data0.insert(i_data, INSERT_VALUE2);
      compare_data.insert(i_compare_data, INSERT_VALUE2);

      temp.assign(data0.begin(), data0.end());

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), size_t(std::distance(data1.begin(), data1.end())));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      bool are_equal;

      std::vector<ItemNDCNode> test1 = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4") };
      std::vector<ItemNDCNode> test2 = { ItemNDCNode("5"), ItemNDCNode("6"), ItemNDCNode("7"), ItemNDCNode("8"), ItemNDCNode("9") };
      std::vector<ItemNDCNode> compare(test2);
      compare.insert(compare.end(), test1.begin(), test1.end());

      DataNDC0 data0(test1.begin(), test1.end());
      DataNDC1 data1(test1.begin(), test1.end());

      data0.insert(data0.begin(), test2.begin(), test2.end());

      are_equal = std::equal(data0.begin(), data0.end(), compare.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), test1.begin());
      CHECK(are_equal);
      CHECK_EQUAL(test1.size(), data1.size());
      CHECK_EQUAL(test1.size(), size_t(std::distance(data1.begin(), data1.end())));

      compare.assign(test1.begin(), test1.end());
      data0.assign(test1.begin(), test1.end());

      std::vector<ItemNDCNode>::iterator icd = compare.begin();
      DataNDC0::iterator id = data0.begin();

      std::advance(icd, 3);
      std::advance(id, 3);

      compare.insert(icd, test2.begin(), test2.end());
      data0.insert(id, test2.begin(), test2.end());

      std::vector<ItemNDCNode> d(data0.begin(), data0.end());
      std::vector<ItemNDCNode> c(compare.begin(), compare.end());

      are_equal = std::equal(data0.begin(), data0.end(), compare.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), test1.begin());
      CHECK(are_equal);
      CHECK_EQUAL(test1.size(), data1.size());
      CHECK_EQUAL(test1.size(), size_t(std::distance(data1.begin(), data1.end())));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front)
    {
      bool are_equal;

      std::list<ItemNDCNode> compare_data;
      DataNDC0 data0;

      static ItemNDCNode node1("1");
      static ItemNDCNode node2("2");
      static ItemNDCNode node3("3");
      static ItemNDCNode node4("4");
      static ItemNDCNode node5("5");
      static ItemNDCNode node6("6");

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
      CHECK_EQUAL(6U, data0.size());
      CHECK_EQUAL(6, std::distance(data0.begin(), data0.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_pop_front)
    {
      DataNDC0 data0;
      DataNDC1 data1;

      static ItemNDCNode node1("1");
      static ItemNDCNode node2("2");
      static ItemNDCNode node3("3");
      static ItemNDCNode node4("4");
      static ItemNDCNode node5("5");
      static ItemNDCNode node6("6");

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

      CHECK_EQUAL(6U, data0.size());
      CHECK_EQUAL(6, std::distance(data0.begin(), data0.end()));
      CHECK(!data0.empty());

      data0.pop_front();
      data0.pop_front();
      data0.pop_front();
      data0.pop_front();
      data0.pop_front();

      CHECK_EQUAL(1U, data0.size());
      CHECK_EQUAL(1, std::distance(data0.begin(), data0.end()));
      CHECK(!data0.empty());

      data0.pop_front();

      CHECK_EQUAL(0U, data0.size());
      CHECK_EQUAL(0, std::distance(data0.begin(), data0.end()));
      CHECK(data0.empty());

      CHECK_EQUAL(6U, data1.size());
      CHECK_EQUAL(6, std::distance(data1.begin(), data1.end()));
      CHECK(!data1.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_already_linked_value)
    {
      DataNDC0 data0;

      static ItemNDCNode node1("1");

      data0.push_front(node1);
      CHECK_THROW(data0.push_front(node1), etl::intrusive_list_value_is_already_linked);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_already_linked_value)
    {
      DataNDC0 data0;

      static ItemNDCNode node1("1");

      data0.push_front(node1);
      CHECK_THROW(data0.push_back(node1), etl::intrusive_list_value_is_already_linked);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      bool are_equal;

      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      DataNDC1 data1(sorted_data.begin(), sorted_data.end());

      DataNDC0::iterator i_data = data0.begin();
      std::advance(i_data, 3);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 3);

      i_compare_data = compare_data.erase(i_compare_data);
      i_data         = data0.erase(i_data);

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase(compare_data.begin());
      i_data         = data0.erase(data0.begin());

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), size_t(std::distance(data1.begin(), data1.end())));

      are_equal = *i_data == *i_compare_data;
      CHECK(are_equal);

      // Move to the last value and erase.
      i_compare_data = compare_data.begin();
      i_compare_data = compare_data.erase(i_compare_data);

      i_data = data0.begin();
      i_data = data0.erase(i_data);

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), size_t(std::distance(data1.begin(), data1.end())));

      are_equal = *i_data == *i_compare_data;
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      bool are_equal;

      FirstLink& fl0 = sorted_data[0];
      FirstLink& fl1 = sorted_data[1];
      FirstLink& fl2 = sorted_data[2];
      FirstLink& fl3 = sorted_data[3];
      FirstLink& fl4 = sorted_data[4];
      FirstLink& fl5 = sorted_data[5];
      FirstLink& fl6 = sorted_data[6];
      FirstLink& fl7 = sorted_data[7];
      FirstLink& fl8 = sorted_data[8];
      FirstLink& fl9 = sorted_data[9];

      SecondLink& sl0 = sorted_data[0];
      SecondLink& sl1 = sorted_data[1];
      SecondLink& sl2 = sorted_data[2];
      SecondLink& sl3 = sorted_data[3];
      SecondLink& sl4 = sorted_data[4];
      SecondLink& sl5 = sorted_data[5];
      SecondLink& sl6 = sorted_data[6];
      SecondLink& sl7 = sorted_data[7];
      SecondLink& sl8 = sorted_data[8];
      SecondLink& sl9 = sorted_data[9];

      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      DataNDC1 data1(sorted_data.begin(), sorted_data.end());

      DataNDC0::iterator i_data_1 = data0.begin();
      std::advance(i_data_1, 3);

      DataNDC0::iterator i_data_2 = data0.begin();
      std::advance(i_data_2, 7);

      std::vector<ItemNDCNode>::iterator i_compare_data_1 = compare_data.begin();
      std::advance(i_compare_data_1, 3);

      std::vector<ItemNDCNode>::iterator i_compare_data_2 = compare_data.begin();
      std::advance(i_compare_data_2, 7);

      std::vector<ItemNDCNode>::iterator i_compare_result = compare_data.erase(i_compare_data_1, i_compare_data_2);

      DataNDC0::iterator i_result = data0.erase(i_data_1, i_data_2);

      CHECK_TRUE(fl0.is_linked());
      CHECK_TRUE(fl1.is_linked());
      CHECK_TRUE(fl2.is_linked());
      CHECK_FALSE(fl3.is_linked());
      CHECK_FALSE(fl4.is_linked());
      CHECK_FALSE(fl5.is_linked());
      CHECK_FALSE(fl6.is_linked());
      CHECK_TRUE(fl7.is_linked());
      CHECK_TRUE(fl8.is_linked());
      CHECK_TRUE(fl9.is_linked());

      CHECK_TRUE(sl0.is_linked());
      CHECK_TRUE(sl1.is_linked());
      CHECK_TRUE(sl2.is_linked());
      CHECK_TRUE(sl3.is_linked());
      CHECK_TRUE(sl4.is_linked());
      CHECK_TRUE(sl5.is_linked());
      CHECK_TRUE(sl6.is_linked());
      CHECK_TRUE(sl7.is_linked());
      CHECK_TRUE(sl8.is_linked());
      CHECK_TRUE(sl9.is_linked());

      CHECK_EQUAL(*i_compare_result, *i_result);

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), size_t(std::distance(data1.begin(), data1.end())));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range_end)
    {
      bool are_equal;

      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      DataNDC1 data1(sorted_data.begin(), sorted_data.end());

      DataNDC0::iterator i_data = data0.begin();
      std::advance(i_data, 5);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 5);

      compare_data.erase(i_compare_data, compare_data.end());

      DataNDC0::iterator i_result = data0.erase(i_data, data0.end());

      CHECK(i_result == data0.end());

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), size_t(std::distance(data1.begin(), data1.end())));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_all)
    {
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());

      data0.erase(data0.begin(), data0.end());

      CHECK(data0.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(sorted_data.front(), data0.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_unique)
    {
      bool are_equal;

      DataNDC0 data0(non_unique_data.begin(), non_unique_data.end());
      DataNDC1 data1(non_unique_data.begin(), non_unique_data.end());

      data0.unique(EqualItemNDCNode());

      // data0 should have changed.
      are_equal = std::equal(data0.begin(), data0.end(), unique_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(unique_data.size(), data0.size());
      CHECK_EQUAL(unique_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      // data1 should not have changed.
      are_equal = std::equal(data1.begin(), data1.end(), non_unique_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(non_unique_data.size(), data1.size());
      CHECK_EQUAL(non_unique_data.size(), size_t(std::distance(data1.begin(), data1.end())));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove)
    {
      bool are_equal;

      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      DataNDC1 data1(sorted_data.begin(), sorted_data.end());

      std::vector<ItemNDCNode>::iterator i_item = std::find(compare_data.begin(), compare_data.end(), ItemNDCNode("7"));
      compare_data.erase(i_item);
      data0.remove(ItemNDCNode("7"));

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), size_t(std::distance(data1.begin(), data1.end())));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_remove_if)
    {
      bool are_equal;

      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      DataNDC1 data1(sorted_data.begin(), sorted_data.end());

      std::vector<ItemNDCNode>::iterator i_item = std::find(compare_data.begin(), compare_data.end(), ItemNDCNode("7"));
      compare_data.erase(i_item);
      data0.remove_if(std::bind(std::equal_to<ItemNDCNode>(), std::placeholders::_1, ItemNDCNode("7")));

      are_equal = std::equal(data0.begin(), data0.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK_EQUAL(compare_data.size(), data0.size());
      CHECK_EQUAL(compare_data.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
      CHECK_EQUAL(sorted_data.size(), data1.size());
      CHECK_EQUAL(sorted_data.size(), size_t(std::distance(data1.begin(), data1.end())));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      DataNDC1 data1(sorted_data.begin(), sorted_data.end());

      data0.reverse(); // Just reverse one of them.

      CHECK_EQUAL(data1.size(), data0.size());
      CHECK_EQUAL(data0.size(), size_t(std::distance(data0.begin(), data0.end())));

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.rbegin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), sorted_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort)
    {
      bool are_equal;

      DataNDC0 data0(unsorted_data.begin(), unsorted_data.end());
      DataNDC1 data1(unsorted_data.begin(), unsorted_data.end());

      data0.sort(); // Just sort one of them.

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), unsorted_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_stable_sort)
    {
      std::list<ItemNDCNode> compare_data(stable_sort_data.begin(), stable_sort_data.end());
      DataNDC0 data(stable_sort_data.begin(), stable_sort_data.end());

      compare_data.sort();
      data.sort();

      std::list<ItemNDCNode>::const_iterator citr = compare_data.begin();
      DataNDC0::const_iterator ditr = data.begin();

      while (ditr != data.end())
      {
        CHECK_EQUAL(citr->data.index, ditr->data.index);

        ++citr;
        ++ditr;
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_sort_compare)
    {
      bool are_equal;

      DataNDC0 data0(unsorted_data.begin(), unsorted_data.end());
      DataNDC1 data1(unsorted_data.begin(), unsorted_data.end());

      data0.sort(CompareItemNDCNode()); // Just sort one of them.

      are_equal = std::equal(data0.begin(), data0.end(), sorted_data.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), unsorted_data.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_iterator)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin(),   sorted_data.end());
      DataNDC0 data1(unsorted_data.begin(), unsorted_data.end());

      DataNDC0::iterator idata_source = data1.begin();
      std::advance(idata_source, 2);

      DataNDC0::iterator idata_destination = data0.begin();
      std::advance(idata_destination, 3);

      std::list<ItemNDCNode> compare0(data0.begin(), data0.end());
      std::list<ItemNDCNode> compare1(data1.begin(), data1.end());

      std::list<ItemNDCNode>::iterator icompare_source = compare1.begin();
      std::advance(icompare_source, 2);

      std::list<ItemNDCNode>::iterator icompare_destination = compare0.begin();
      std::advance(icompare_destination, 3);

      data0.splice(idata_destination, data1, idata_source);
      compare0.splice(icompare_destination, compare1, icompare_source);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      are_equal = std::equal(data1.begin(), data1.end(), compare1.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_iterator_same_list)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin(), sorted_data.end());

      DataNDC0::iterator idata_source = data0.begin();
      std::advance(idata_source, 2);

      DataNDC0::iterator idata_destination = data0.begin();
      std::advance(idata_destination, 5);

      std::list<ItemNDCNode> compare0(data0.begin(), data0.end());

      std::list<ItemNDCNode>::iterator icompare_source = compare0.begin();
      std::advance(icompare_source, 2);

      std::list<ItemNDCNode>::iterator icompare_destination = compare0.begin();
      std::advance(icompare_destination, 5);

      data0.splice(idata_destination, data0, idata_source);
      compare0.splice(icompare_destination, compare0, icompare_source);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_list)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin() , sorted_data.end());
      DataNDC0 data1(sorted_data2.begin(), sorted_data2.end());

      DataNDC0::iterator idata_destination = data0.begin();
      std::advance(idata_destination, 3);

      std::list<ItemNDCNode> compare0(data0.begin(), data0.end());
      std::list<ItemNDCNode> compare1(data1.begin(), data1.end());

      std::list<ItemNDCNode>::iterator icompare_destination = compare0.begin();
      std::advance(icompare_destination, 3);

      data0.splice(idata_destination, data1);
      compare0.splice(icompare_destination, compare1);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
      CHECK_EQUAL(data1.size(), compare1.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_list_self)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin(), sorted_data.end());

      DataNDC0::iterator idata_destination = data0.begin();
      std::advance(idata_destination, 3);

      std::list<ItemNDCNode> compare0(sorted_data2.begin(), sorted_data2.end());

      data0.splice(idata_destination, data0);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_range)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin(), sorted_data.end());
      DataNDC0 data1(sorted_data2.begin(), sorted_data2.end());

      DataNDC0::iterator idata_destination = data0.begin();
      std::advance(idata_destination, 3);

      DataNDC0::iterator idata_begin = data1.begin();
      std::advance(idata_begin, 2);

      DataNDC0::iterator idata_end = data1.begin();
      std::advance(idata_end, 7);

      std::list<ItemNDCNode> compare0(data0.begin(), data0.end());
      std::list<ItemNDCNode> compare1(data1.begin(), data1.end());

      std::list<ItemNDCNode>::iterator icompare_destination = compare0.begin();
      std::advance(icompare_destination, 3);

      std::list<ItemNDCNode>::iterator icompare_begin = compare1.begin();
      std::advance(icompare_begin, 2);

      std::list<ItemNDCNode>::iterator icompare_end = compare1.begin();
      std::advance(icompare_end, 7);

      data0.splice(idata_destination, data1, idata_begin, idata_end);
      compare0.splice(icompare_destination, compare1, icompare_begin, icompare_end);
      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
      CHECK_EQUAL(data1.size(), compare1.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_splice_range_self)
    {
      bool are_equal;

      DataNDC0 data0(sorted_data.begin(), sorted_data.end());

      DataNDC0::iterator idata_destination = data0.begin();
      std::advance(idata_destination, 2);

      DataNDC0::iterator idata_begin = data0.begin();
      std::advance(idata_begin, 4);

      DataNDC0::iterator idata_end = data0.begin();
      std::advance(idata_end, 7);

      std::list<ItemNDCNode> compare0(data0.begin(), data0.end());

      std::list<ItemNDCNode>::iterator icompare_destination = compare0.begin();
      std::advance(icompare_destination, 2);

      std::list<ItemNDCNode>::iterator icompare_begin = compare0.begin();
      std::advance(icompare_begin, 4);

      std::list<ItemNDCNode>::iterator icompare_end = compare0.begin();
      std::advance(icompare_end, 7);

      data0.splice(idata_destination, data0, idata_begin, idata_end);
      compare0.splice(icompare_destination, compare0, icompare_begin, icompare_end);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_1)
    {
      bool are_equal;

      DataNDC0 data0(merge_data0.begin(), merge_data0.end());
      DataNDC0 data1(merge_data1.begin(), merge_data1.end());

      std::list<ItemNDCNode> compare0(merge_data0.begin(), merge_data0.end());
      std::list<ItemNDCNode> compare1(merge_data1.begin(), merge_data1.end());

      data0.merge(data1);
      compare0.merge(compare1);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
      CHECK_EQUAL(data1.size(), compare1.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_2)
    {
      bool are_equal;

      DataNDC0 data0(merge_data0.begin(), merge_data0.end());
      DataNDC0 data2(merge_data2.begin(), merge_data2.end());

      std::list<ItemNDCNode> compare0(merge_data0.begin(), merge_data0.end());
      std::list<ItemNDCNode> compare2(merge_data2.begin(), merge_data2.end());

      data0.merge(data2);
      compare0.merge(compare2);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
      CHECK_EQUAL(data2.size(), compare2.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_3)
    {
      bool are_equal;

      DataNDC0 data0(merge_data0.begin(), merge_data0.end());
      DataNDC0 data3(merge_data3.begin(), merge_data3.end());

      std::list<ItemNDCNode> compare0(merge_data0.begin(), merge_data0.end());
      std::list<ItemNDCNode> compare3(merge_data3.begin(), merge_data3.end());

      data0.merge(data3);
      compare0.merge(compare3);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
      CHECK_EQUAL(data3.size(), compare3.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_4)
    {
      bool are_equal;

      DataNDC0 data0(merge_data0.begin(), merge_data0.end());
      DataNDC0 data4(merge_data4.begin(), merge_data4.end());

      std::list<ItemNDCNode> compare0(merge_data0.begin(), merge_data0.end());
      std::list<ItemNDCNode> compare4(merge_data4.begin(), merge_data4.end());

      data0.merge(data4);
      compare0.merge(compare4);

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
      CHECK_EQUAL(data4.size(), compare4.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_merge_0_1_reverse_order)
    {
      bool are_equal;

      DataNDC0 data0(merge_data0.begin(), merge_data0.end());
      DataNDC0 data1(merge_data1.begin(), merge_data1.end());

      data0.reverse();
      data1.reverse();

      std::list<ItemNDCNode> compare0(merge_data0.begin(), merge_data0.end());
      std::list<ItemNDCNode> compare1(merge_data1.begin(), merge_data1.end());

      compare0.reverse();
      compare1.reverse();

      data0.merge(data1,       std::greater<ItemNDCNode>());
      compare0.merge(compare1, std::greater<ItemNDCNode>());

      are_equal = std::equal(data0.begin(), data0.end(), compare0.begin());
      CHECK(are_equal);

      CHECK_EQUAL(data0.size(), compare0.size());
      CHECK_EQUAL(data1.size(), compare1.size());
    }
  };
}
