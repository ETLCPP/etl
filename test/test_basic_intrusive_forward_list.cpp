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

#include "../basic_intrusive_forward_list.h"

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
  class ItemDCNode : public etl::basic_intrusive_forward_list_node
  {
  public:

    ItemDCNode(const std::string& text)
      : data(text)
    {
    }

    ItemDC data;
  };

  class ItemNDCNode : public etl::basic_intrusive_forward_list_node
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
    typedef etl::basic_intrusive_forward_list DataDC;
    typedef etl::basic_intrusive_forward_list DataNDC;

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
      DataNDC data;
      DataNDC data1;

      CHECK(data.empty());
      CHECK(data1.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK(!data.empty());
    }

    ////*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin_end_empty)
    {
      DataNDC data;

      CHECK(data.begin() == data.end());
    }

    ////*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());

      InitialDataNDC::iterator isorted = sorted_data.begin();
      DataNDC::iterator        idata   = data.begin();

      while (isorted != sorted_data.end())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *isorted;
        CHECK(are_equal);
        ++idata;
        ++isorted;
      }
    }

    ////*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());

      InitialDataNDC::const_iterator isorted = sorted_data.cbegin();
      DataNDC::const_iterator        idata = data.cbegin();

      while (isorted != sorted_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *isorted;
        CHECK(are_equal);
        ++idata;
        ++isorted;
      }
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
      DataNDC data;

      // Do it twice. We should only get one copy.
      data.assign(sorted_data.begin(), sorted_data.end());
      data.assign(sorted_data.begin(), sorted_data.end());

      InitialDataNDC::const_iterator isorted = sorted_data.cbegin();
      DataNDC::const_iterator        idata = data.cbegin();

      while (isorted != sorted_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *isorted;
        CHECK(are_equal);
        ++idata;
        ++isorted;
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_position_value)
    {
      ItemNDCNode INSERT_VALUE1 = ItemNDCNode("1");
      ItemNDCNode INSERT_VALUE2 = ItemNDCNode("2");

      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      size_t offset = 2;

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, offset);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset + 1);

      data.insert_after(i_data, INSERT_VALUE1);
      compare_data.insert(i_compare_data, INSERT_VALUE1);

      InitialDataNDC::const_iterator icompare = compare_data.cbegin();
      DataNDC::const_iterator        idata    = data.cbegin();

      while (icompare != compare_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }
      CHECK_EQUAL(compare_data.size(), std::distance(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_after_range)
    {
      std::vector<ItemNDCNode> test1 = { ItemNDCNode("0"), ItemNDCNode("1"), ItemNDCNode("2"), ItemNDCNode("3"), ItemNDCNode("4") };
      std::vector<ItemNDCNode> test2 = { ItemNDCNode("5"), ItemNDCNode("6"), ItemNDCNode("7"), ItemNDCNode("8"), ItemNDCNode("9") };
      std::vector<ItemNDCNode> compare(test2);
      compare.insert(compare.end(), test1.begin(), test1.end());

      size_t final_size = test1.size() + test2.size();

      DataNDC data(test1.begin(), test1.end());

      data.insert_after(data.before_begin(), test2.begin(), test2.end());

      InitialDataNDC::const_iterator icompare = compare.cbegin();
      DataNDC::const_iterator        idata = data.cbegin();

      while (icompare != compare.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }

      CHECK_EQUAL(final_size, std::distance(data.begin(), data.end()));

      compare.assign(test1.begin(), test1.end());
      data.assign(test1.begin(), test1.end());

      std::vector<ItemNDCNode>::iterator icd = compare.begin();
      DataNDC::iterator id = data.begin();

      std::advance(icd, 4);
      std::advance(id, 3);

      compare.insert(icd, test2.begin(), test2.end());
      data.insert_after(id, test2.begin(), test2.end());

      icompare = compare.cbegin();
      idata    = data.cbegin();

      while (icompare != compare.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }

      CHECK_EQUAL(final_size, std::distance(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front)
    {
      std::list<ItemNDCNode> compare_data;
      DataNDC data;

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

      data.push_front(node1);
      data.push_front(node2);
      data.push_front(node3);
      data.push_front(node4);
      data.push_front(node5);
      data.push_front(node6);

      std::list<ItemNDCNode>::const_iterator icompare = compare_data.cbegin();
      DataNDC::const_iterator idata = data.cbegin();

      while (icompare != compare_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }
      CHECK_EQUAL(6, std::distance(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_front_pop_front)
    {
      DataNDC data;

      ItemNDCNode node1("1");
      ItemNDCNode node2("2");
      ItemNDCNode node3("3");
      ItemNDCNode node4("4");
      ItemNDCNode node5("5");
      ItemNDCNode node6("6");

      data.push_front(node1);
      data.push_front(node2);
      data.push_front(node3);
      data.push_front(node4);
      data.push_front(node5);
      data.push_front(node6);

      CHECK_EQUAL(6, std::distance(data.begin(), data.end()));
      CHECK(!data.empty());

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();

      CHECK_EQUAL(1, std::distance(data.begin(), data.end()));
      CHECK(!data.empty());

      data.pop_front();

      CHECK_EQUAL(0, std::distance(data.begin(), data.end()));
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_single)
    {
      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, 2);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 3);

      i_compare_data = compare_data.erase(i_compare_data);
      i_data         = data.erase_after(i_data);

      std::vector<ItemNDCNode>::const_iterator icompare = compare_data.cbegin();
      DataNDC::const_iterator idata = data.cbegin();

      while (icompare != compare_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }

      CHECK(*i_compare_data == i_data.ref_cast<ItemNDCNode>());

      i_compare_data = compare_data.erase(compare_data.begin() + 1);
      i_data         = data.erase_after(data.begin());

      icompare = compare_data.cbegin();
      idata    = data.cbegin();

      while (icompare != compare_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }

      CHECK_EQUAL(compare_data.size(), std::distance(data.begin(), data.end()));

      // Move to the last value and erase.
      i_compare_data = compare_data.begin() + 1;
      i_compare_data = compare_data.erase(i_compare_data);

      i_data = data.begin();
      i_data = data.erase_after(i_data);

      icompare = compare_data.cbegin();
      idata    = data.cbegin();

      while (icompare != compare_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }

      CHECK_EQUAL(compare_data.size(), std::distance(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_range)
    {
      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data_1 = data.begin();
      std::advance(i_data_1, 2);

      DataNDC::iterator i_data_2 = data.begin();
      std::advance(i_data_2, 5);

      std::vector<ItemNDCNode>::iterator i_compare_data_1 = compare_data.begin();
      std::advance(i_compare_data_1, 3);

      std::vector<ItemNDCNode>::iterator i_compare_data_2 = compare_data.begin();
      std::advance(i_compare_data_2, 5);

      std::vector<ItemNDCNode>::iterator i_compare_result = compare_data.erase(i_compare_data_1, i_compare_data_2);

      DataNDC::iterator i_result = data.erase_after(i_data_1, i_data_2);

      CHECK_EQUAL(*i_compare_result, i_result.ref_cast<ItemNDCNode>());

      std::vector<ItemNDCNode>::const_iterator icompare = compare_data.cbegin();
      DataNDC::const_iterator idata = data.cbegin();

      while (icompare != compare_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }
      
      CHECK_EQUAL(compare_data.size(), std::distance(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_after_range_end)
    {
      std::vector<ItemNDCNode> compare_data(sorted_data.begin(), sorted_data.end());
      DataNDC data(sorted_data.begin(), sorted_data.end());

      DataNDC::iterator i_data = data.begin();
      std::advance(i_data, 4);

      std::vector<ItemNDCNode>::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 5);

      std::vector<ItemNDCNode>::iterator i_compare_result = compare_data.erase(i_compare_data, compare_data.end());

      DataNDC::iterator i_result = data.erase_after(i_data, data.end());

      CHECK(i_result == data.end());

      std::vector<ItemNDCNode>::const_iterator icompare = compare_data.cbegin();
      DataNDC::const_iterator idata = data.cbegin();

      while (icompare != compare_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }

      CHECK_EQUAL(compare_data.size(), std::distance(data.begin(), data.end()));
        
      icompare = compare_data.cbegin();
      idata    = data.cbegin();

      while (icompare != compare_data.cend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *icompare;
        CHECK(are_equal);
        ++idata;
        ++icompare;
      }

      CHECK_EQUAL(compare_data.size(), std::distance(data.begin(), data.end()));
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
      DataNDC data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(sorted_data.front(), data.front<ItemNDCNode>());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse)
    {
      DataNDC data(sorted_data.begin(), sorted_data.end());
      data.reverse();

      InitialDataNDC::const_reverse_iterator isorted = sorted_data.crbegin();
      DataNDC::const_iterator idata = data.cbegin();

      while (isorted != sorted_data.crend())
      {
        const ItemNDCNode& item = idata.ref_cast<ItemNDCNode>();
        are_equal = item == *isorted;
        CHECK(are_equal);
        ++idata;
        ++isorted;
      }
    }
  };
}
