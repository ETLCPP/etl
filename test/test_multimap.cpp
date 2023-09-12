/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove, rlindeman

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

#include <map>
#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>

#include "etl/multimap.h"

#include "data.h"

static const size_t MAX_SIZE = 10UL;

#define TEST_GREATER_THAN
#ifdef TEST_GREATER_THAN
using Data = etl::multimap<std::string, int, MAX_SIZE, std::greater<std::string>>;
using IData = etl::imultimap<std::string, int, std::greater<std::string>>;
using Compare_Data = std::multimap<std::string, int, std::greater<std::string>>;
#else
using Data = etl::multimap<std::string, int, MAX_SIZE, std::less<std::string>>;
using IData = etl::imultimap<std::string, int, std::less<std::string>>;
using Compare_Data = std::multimap<std::string, int, std::less<std::string>>;
#endif

using ItemM = TestDataM<int>;
using DataM = etl::multimap<std::string, ItemM, MAX_SIZE>;

using Data_iterator = Data::iterator;
using Data_const_iterator = Data::const_iterator;
using Compare_Data_iterator = Compare_Data::iterator;
using Compare_Data_const_iterator = Compare_Data::const_iterator;

namespace
{
  struct Key
  {
    Key(const char* k_)
      : k(k_)
    {
    }

    std::string k;
  };

  bool operator <(const Key& lhs, const std::string& rhs)
  {
    return (lhs.k < rhs);
  }

  bool operator <(const std::string& lhs, const Key& rhs)
  {
    return (lhs < rhs.k);
  }

  SUITE(test_multimap)
  {
    //*************************************************************************
    template <typename T1, typename T2>
    bool Check_Equal(T1 begin1, T1 end1, T2 begin2)
    {
      while (begin1 != end1)
      {
        if ((begin1->first != begin2->first) || (begin1->second != begin2->second))
        {
          return false;
        }

        ++begin1;
        ++begin2;
      }

      return true;
    }

    //*************************************************************************
    struct SetupFixture
    {
      // Multimaps of predefined data from which to constuct multimaps used in
      // each test
      std::multimap<std::string, int> initial_data;
      std::multimap<std::string, int> excess_data;
      std::multimap<std::string, int> different_data;
      std::multimap<std::string, int> random_data;
      std::multimap<std::string, int> initial_data_even;
      std::multimap<std::string, int> test_data;

      SetupFixture()
      {
        // Create a map of initial data
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("0", 0));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("0", 1));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("1", 2));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("1", 3));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("1", 4));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("2", 5));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("2", 8));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("2", 7));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("3", 6));
        initial_data.insert(ETL_OR_STD::pair<std::string, int>("4", 9));

        // Create a multimap of excess data
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("0", 0));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("1", 1));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("2", 2));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("2", 3));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("3", 6));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("3", 5));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("3", 4));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("4", 7));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("5", 8));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("6", 9));
        excess_data.insert(ETL_OR_STD::pair<std::string, int>("10", 10));

        // Create a multimap of different data
        different_data.insert(ETL_OR_STD::pair<std::string, int>("10", 10));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("11", 11));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("12", 12));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("13", 13));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("14", 14));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("15", 15));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("16", 16));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("17", 17));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("18", 18));
        different_data.insert(ETL_OR_STD::pair<std::string, int>("19", 19));

        // Create a multimap of random data
        random_data.insert(ETL_OR_STD::pair<std::string, int>("3", 6));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("3", 5));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("0", 0));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("5", 8));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("6", 9));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("2", 2));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("0", 1));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("2", 3));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("4", 7));
        random_data.insert(ETL_OR_STD::pair<std::string, int>("3", 4));


        //even values
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("00", 0));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("02", 2));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("04", 4));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("06", 6));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("08", 8));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("10", 10));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("12", 12));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("14", 14));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("16", 16));
        initial_data_even.insert(ETL_OR_STD::pair<std::string, int>("18", 18));

        //test set
        test_data.insert(ETL_OR_STD::pair<std::string, int>("00", 0));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("01", 1));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("02", 2));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("03", 3));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("04", 4));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("05", 5));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("06", 6));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("07", 7));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("08", 8));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("09", 9));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("10", 10));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("11", 11));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("12", 12));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("13", 13));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("14", 14));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("15", 15));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("16", 16));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("17", 17));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("18", 18));
        test_data.insert(ETL_OR_STD::pair<std::string, int>("19", 19));
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      Data data;

      CHECK(data.size() == size_t(0UL));
      CHECK(data.empty());
      CHECK(data.capacity() == MAX_SIZE);
      CHECK(data.max_size() == MAX_SIZE);
      CHECK(data.begin() == data.end());
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_cpp17_deduced_constructor)
    {
      etl::multimap data{ std::pair<std::string, int>("0", 0), std::pair<std::string, int>("1", 1), std::pair<std::string, int>("2", 2), std::pair<std::string, int>("3", 3), std::pair<std::string, int>("4", 4),
                          std::pair<std::string, int>("5", 5), std::pair<std::string, int>("6", 6), std::pair<std::string, int>("7", 7), std::pair<std::string, int>("8", 8), std::pair<std::string, int>("9", 9) };
      etl::multimap<std::string, int, 10U> check = { std::pair<std::string, int>("0", 0), std::pair<std::string, int>("1", 1), std::pair<std::string, int>("2", 2), std::pair<std::string, int>("3", 3), std::pair<std::string, int>("4", 4),
                                                     std::pair<std::string, int>("5", 5), std::pair<std::string, int>("6", 6), std::pair<std::string, int>("7", 7), std::pair<std::string, int>("8", 8), std::pair<std::string, int>("9", 9) };

      CHECK(!data.empty());
      CHECK(data.full());
      CHECK(data.begin() != data.end());
      CHECK_EQUAL(10U, data.size());
      CHECK_EQUAL(0U, data.available());
      CHECK_EQUAL(10U, data.capacity());
      CHECK_EQUAL(10U, data.max_size());
      CHECK(data == check);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data1(compare_data.begin(), compare_data.end());
      Data data2(data1);

      CHECK(initial_data.size() == data1.size());
      CHECK(data1.size() == data2.size());

      bool isEqual = false;

      isEqual = Check_Equal(data1.begin(),
                            data1.end(),
                            compare_data.begin());
      CHECK(isEqual);

      isEqual = Check_Equal(data2.begin(),
                            data2.end(),
                            compare_data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_constructor)
    {
      DataM data1;

      ItemM d1(1);
      ItemM d2(2);
      ItemM d3(3);

      data1.insert(DataM::value_type(std::string("1"), etl::move(d1)));
      data1.insert(DataM::value_type(std::string("2"), etl::move(d2)));
      data1.insert(DataM::value_type(std::string("3"), etl::move(d3)));
      data1.insert(DataM::value_type(std::string("4"), ItemM(4)));

      DataM data2(std::move(data1));

      CHECK(!data1.empty()); // Move does not clear the source.

      CHECK(1 == data2.find("1")->second.value);
      CHECK(2 == data2.find("2")->second.value);
      CHECK(3 == data2.find("3")->second.value);
      CHECK(4 == data2.find("4")->second.value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_imultimap)
    {
      Data* pdata = new Data(initial_data.begin(), initial_data.end());

      IData* pidata = pdata;
      delete pidata;
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      CHECK(data.size() == MAX_SIZE);
      CHECK(!data.empty());
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_initializer_list)
    {
      Compare_Data compare_data = { Data::value_type(std::string("0"), 0), Data::value_type(std::string("1"), 1),
      Data::value_type(std::string("2"), 2), Data::value_type(std::string("3"), 3) };

      Data data = { Data::value_type(std::string("0"), 0), Data::value_type(std::string("1"), 1),
      Data::value_type(std::string("2"), 2), Data::value_type(std::string("3"), 3) };

      CHECK(compare_data.size() == data.size());
      CHECK(!data.empty());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data otherData;

      otherData = data;

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 otherData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_interface)
    {
      Data data1(initial_data.begin(), initial_data.end());
      Data data2;

      IData& idata1 = data1;
      IData& idata2 = data2;

      idata2 = idata1;

      bool isEqual = std::equal(data1.begin(),
                                data1.end(),
                                data2.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data other_data(data);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_data = other_data;
#include "etl/private/diagnostic_pop.h" 

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                other_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      DataM data1;
      DataM data2;

      ItemM d1(1);
      ItemM d2(2);
      ItemM d3(3);

      data1.insert(DataM::value_type(std::string("1"), etl::move(d1)));
      data1.insert(DataM::value_type(std::string("2"), etl::move(d2)));
      data1.insert(DataM::value_type(std::string("3"), etl::move(d3)));
      data1.insert(DataM::value_type(std::string("4"), ItemM(4)));

      data2.insert(DataM::value_type(std::string("5"), ItemM(5)));
      data2 = std::move(data1);

      CHECK(1 == data2.find("1")->second.value);
      CHECK(2 == data2.find("2")->second.value);
      CHECK(3 == data2.find("3")->second.value);
      CHECK(4 == data2.find("4")->second.value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      Data data(initial_data.begin(), initial_data.end());
      const Data constData(data);

      CHECK(data.begin() == std::begin(data));
      CHECK(constData.begin() == std::begin(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      Data data(initial_data.begin(), initial_data.end());
      const Data constData(data);

      CHECK(data.end() == std::end(data));
      CHECK(constData.end() == std::end(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty)
    {
      Data data;
      data.insert(initial_data.begin(), initial_data.end());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full)
    {
      Data data;

      CHECK(!data.full());
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data;

      data.assign(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value)
    {
      Compare_Data compare_data;
      Data data;

      Data::iterator data_result =
        data.insert(Data::value_type(std::string("2"), 1));
      Compare_Data::iterator compare_result =
        compare_data.insert(ETL_OR_STD::make_pair(std::string("2"), 1));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      data_result = data.insert(Data::value_type(std::string("1"), 1));
      compare_result = compare_data.insert(ETL_OR_STD::make_pair(std::string("1"), 1));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      data_result = data.insert(Data::value_type(std::string("2"), 2));
      compare_result = compare_data.insert(ETL_OR_STD::make_pair(std::string("2"), 2));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      data_result = data.insert(Data::value_type(std::string("3"), 3));
      compare_result = compare_data.insert(ETL_OR_STD::make_pair(std::string("3"), 3));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      // Adding this next 2 will trigger a 3 node rotate RL on insert
      data_result = data.insert(Data::value_type(std::string("2"), 3));
      compare_result = compare_data.insert(ETL_OR_STD::make_pair(std::string("2"), 3));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      // Adding this next 4 will trigger a 2 node rotate left on insert
      data_result = data.insert(Data::value_type(std::string("4"), 4));
      compare_result = compare_data.insert(ETL_OR_STD::make_pair(std::string("4"), 4));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      data_result = data.insert(Data::value_type(std::string("0"), 0));
      compare_result = compare_data.insert(ETL_OR_STD::make_pair(std::string("0"), 0));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_hint_value)
    {
      Compare_Data compare_data;
      Data data;

      Data::iterator data_result =
        data.insert(Data::value_type(std::string("0"), 0));
      Compare_Data::iterator compare_result =
        compare_data.insert(ETL_OR_STD::make_pair(std::string("0"), 0));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(data_result, ETL_OR_STD::make_pair(std::string("1"), 1));
      compare_data.insert(compare_result, ETL_OR_STD::make_pair(std::string("1"), 1));

      isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_const_hint_value)
    {
      Compare_Data compare_data;
      Data data;

      Data::iterator data_result =
        data.insert(Data::value_type(std::string("2"), 0));
      Compare_Data::iterator compare_result =
        compare_data.insert(ETL_OR_STD::make_pair(std::string("2"), 0));

      // Check that both return successful return results
      CHECK(compare_result->first == data_result->first);
      CHECK(compare_result->second == data_result->second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(Data::const_iterator(data_result),
        ETL_OR_STD::make_pair(std::string("1"), 1));
      compare_data.insert(Compare_Data::const_iterator(compare_result),
        ETL_OR_STD::make_pair(std::string("1"), 1));

      isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      Data data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.insert(ETL_OR_STD::make_pair(std::string("10"), 10)), etl::multimap_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      Compare_Data compare_data;
      Data data;

      data.insert(initial_data.begin(), initial_data.end());
      compare_data.insert(initial_data.begin(), initial_data.end());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range_random)
    {
      Compare_Data compare_data;
      Data data;

      data.insert(random_data.begin(), random_data.end());
      compare_data.insert(random_data.begin(), random_data.end());

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range_excess)
    {
      Data data;

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::multimap_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      Compare_Data compare_data(random_data.begin(), random_data.end());
      Data data(random_data.begin(), random_data.end());

      // Test a number not available
      ETL_OR_STD::pair<Data::iterator, Data::iterator> data_result = data.equal_range("1");
      ETL_OR_STD::pair<Compare_Data::iterator, Compare_Data::iterator> compare_result = compare_data.equal_range("1");

      // Check that both return the same return results
      CHECK(compare_result.first->first == data_result.first->first);
      CHECK(compare_result.first->second == data_result.first->second);
      CHECK(compare_result.second->first == data_result.second->first);
      CHECK(compare_result.second->second == data_result.second->second);

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_range_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(random_data.begin(), random_data.end());
      EMap data(random_data.begin(), random_data.end());

      // Test a number not available
      ETL_OR_STD::pair<EMap::iterator, EMap::iterator> data_result = data.equal_range(Key("1"));
      ETL_OR_STD::pair<CMap::iterator, CMap::iterator> compare_result = compare_data.equal_range("1");

      // Check that both return the same return results
      CHECK(compare_result.first->first == data_result.first->first);
      CHECK(compare_result.first->second == data_result.first->second);
      CHECK(compare_result.second->first == data_result.second->first);
      CHECK(compare_result.second->second == data_result.second->second);

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_equal_range)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      // Test a number with several of the same key
      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> data_result = data.equal_range("2");
      ETL_OR_STD::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> compare_result =  compare_data.equal_range("2");

      // Check that both return the same return results
      CHECK(compare_result.first->first == data_result.first->first);
      CHECK(compare_result.first->second == data_result.first->second);
      CHECK(compare_result.second->first == data_result.second->first);
      CHECK(compare_result.second->second == data_result.second->second);

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_equal_range_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      const CMap compare_data(random_data.begin(), random_data.end());
      const EMap data(random_data.begin(), random_data.end());

      // Test a number with several of the same key
      ETL_OR_STD::pair<EMap::const_iterator, EMap::const_iterator> data_result = data.equal_range(Key("2"));
      ETL_OR_STD::pair<CMap::const_iterator, CMap::const_iterator> compare_result = compare_data.equal_range("2");

      // Check that both return the same return results
      CHECK(compare_result.first->first == data_result.first->first);
      CHECK(compare_result.first->second == data_result.first->second);
      CHECK(compare_result.second->first == data_result.second->first);
      CHECK(compare_result.second->second == data_result.second->second);

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_value)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      size_t compare_count = compare_data.erase("2");
      size_t data_count = data.erase("2");

      // Check that both return the same return results
      CHECK(compare_count == data_count);

      // Erase another value
      compare_count = compare_data.erase("1");
      data_count = data.erase("1");

      // Check that both return the same return results
      CHECK(compare_count == data_count);

      // Erase another value
      compare_count = compare_data.erase("3");
      data_count = data.erase("3");

      // Check that both return the same return results
      CHECK(compare_count == data_count);

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_value_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      size_t compare_count = compare_data.erase("2");
      size_t data_count = data.erase(Key("2"));

      // Check that both return the same return results
      CHECK(compare_count == data_count);

      // Erase another value
      compare_count = compare_data.erase("1");
      data_count = data.erase(Key("1"));

      // Check that both return the same return results
      CHECK(compare_count == data_count);

      // Erase another value
      compare_count = compare_data.erase("3");
      data_count = data.erase(Key("3"));

      // Check that both return the same return results
      CHECK(compare_count == data_count);

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator i_compare = compare_data.begin();
      Data::iterator i_data            = data.begin();

      Compare_Data::iterator i_compare1 = compare_data.erase(i_compare);
      Data::iterator i_data1 = data.erase(i_data);

      CHECK(i_compare1->second == i_data1->second);

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_erase_single)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator i_compare = compare_data.cbegin();
      Data::const_iterator i_data = data.cbegin();

      std::advance(i_compare, 8);
      std::advance(i_data, 8);

      Compare_Data::const_iterator i_compare1 = compare_data.erase(i_compare);
      Data::const_iterator i_data1 = data.erase(i_data);

      CHECK(i_compare1->second == i_data1->second);

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      Compare_Data compare_data(random_data.begin(), random_data.end());
      Data data(random_data.begin(), random_data.end());

      Compare_Data::iterator i_compare = compare_data.begin();
      Data::iterator i_data            = data.begin();

      Compare_Data::iterator i_compare_end = compare_data.begin();
      Data::iterator i_data_end            = data.begin();

      std::advance(i_compare, 1);
      std::advance(i_data,    1);

      std::advance(i_compare_end, 4);
      std::advance(i_data_end,    4);

      compare_data.erase(i_compare, i_compare_end);
      data.erase(i_data, i_data_end);

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_erase_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      compare_data.erase(compare_data.cbegin(), compare_data.cend());
      data.erase(data.cbegin(), data.cend());

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());
      data.clear();

      CHECK(data.size() == size_t(0UL));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count)
    {
      const Data data(initial_data.begin(), initial_data.end());
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      CHECK(compare_data.count(".") == data.count("."));
      CHECK(compare_data.count("0") == data.count("0"));
      CHECK(compare_data.count("1") == data.count("1"));
      CHECK(compare_data.count("2") == data.count("2"));
      CHECK(compare_data.count("3") == data.count("3"));
      CHECK(compare_data.count("4") == data.count("4"));
      CHECK(compare_data.count("A") == data.count("A"));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      const EMap data(initial_data.begin(), initial_data.end());
      const CMap compare_data(initial_data.begin(), initial_data.end());

      CHECK(compare_data.count(".") == data.count(Key(".")));
      CHECK(compare_data.count("0") == data.count(Key("0")));
      CHECK(compare_data.count("1") == data.count(Key("1")));
      CHECK(compare_data.count("2") == data.count(Key("2")));
      CHECK(compare_data.count("3") == data.count(Key("3")));
      CHECK(compare_data.count("4") == data.count(Key("4")));
      CHECK(compare_data.count("A") == data.count(Key("A")));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.cbegin(),
                                 data.cend(),
                                 compare_data.cbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool isEqual = Check_Equal(data.crbegin(),
                                 data.crend(),
                                 compare_data.crbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Data::iterator i_data = data.find("0");
      Compare_Data::iterator i_compare = compare_data.find("0");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find("1");
      i_compare = compare_data.find("1");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find("2");
      i_compare = compare_data.find("2");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find("3");
      i_compare = compare_data.find("3");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(".");
      i_compare = compare_data.find(".");

      // Check that both return successful return results
      CHECK(data.end() == i_data);
      CHECK(compare_data.end() == i_compare);

      i_data = data.find("A");
      i_compare = compare_data.find("A");

      // Check that both return successful return results
      CHECK(data.end() == i_data);
      CHECK(compare_data.end() == i_compare);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      EMap data(initial_data.begin(), initial_data.end());
      CMap compare_data(initial_data.begin(), initial_data.end());

      EMap::iterator i_data = data.find(Key("0"));
      CMap::iterator i_compare = compare_data.find("0");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(Key("1"));
      i_compare = compare_data.find("1");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(Key("2"));
      i_compare = compare_data.find("2");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(Key("3"));
      i_compare = compare_data.find("3");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(Key("."));
      i_compare = compare_data.find(".");

      // Check that both return successful return results
      CHECK(data.end() == i_data);
      CHECK(compare_data.end() == i_compare);

      i_data = data.find(Key("A"));
      i_compare = compare_data.find("A");

      // Check that both return successful return results
      CHECK(data.end() == i_data);
      CHECK(compare_data.end() == i_compare);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Data::const_iterator i_data = data.find("0");
      Compare_Data::const_iterator i_compare = compare_data.find("0");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find("1");
      i_compare = compare_data.find("1");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find("2");
      i_compare = compare_data.find("2");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find("3");
      i_compare = compare_data.find("3");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(".");
      i_compare = compare_data.find(".");

      // Check that both return successful return results
      CHECK(data.end() == i_data);
      CHECK(compare_data.end() == i_compare);

      i_data = data.find("A");
      i_compare = compare_data.find("A");

      // Check that both return successful return results
      CHECK(data.end() == i_data);
      CHECK(compare_data.end() == i_compare);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      const EMap data(initial_data.begin(), initial_data.end());
      const CMap compare_data(initial_data.begin(), initial_data.end());

      EMap::const_iterator i_data = data.find(Key("0"));
      CMap::const_iterator i_compare = compare_data.find("0");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(Key("1"));
      i_compare = compare_data.find("1");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(Key("2"));
      i_compare = compare_data.find("2");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(Key("3"));
      i_compare = compare_data.find("3");

      // Check that both return successful return results
      CHECK(i_compare->first == i_data->first);
      CHECK(i_compare->second == i_data->second);

      i_data = data.find(Key("."));
      i_compare = compare_data.find(".");

      // Check that both return successful return results
      CHECK(data.end() == i_data);
      CHECK(compare_data.end() == i_compare);

      i_data = data.find(Key("A"));
      i_compare = compare_data.find("A");

      // Check that both return successful return results
      CHECK(data.end() == i_data);
      CHECK(compare_data.end() == i_compare);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      const Data initial1(initial_data.begin(), initial_data.end());
      const Data initial2(initial_data.begin(), initial_data.end());

      CHECK(initial1 == initial2);

      const Data different(different_data.begin(), different_data.end());

      CHECK(!(initial1 == different));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      const Data initial1(initial_data.begin(), initial_data.end());
      const Data initial2(initial_data.begin(), initial_data.end());

      CHECK(!(initial1 != initial2));

      const Data different(different_data.begin(), different_data.end());

      CHECK(initial1 != different);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator i_compare = compare_data.lower_bound("2");
      Data::iterator i_data = data.lower_bound("2");
      CHECK(i_compare->second == i_data->second);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.lower_bound(".");
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound(".");
      CHECK(data.end() == i_data);

      i_compare = compare_data.lower_bound("A");
      i_data = data.lower_bound("A");
      CHECK(i_compare->second == i_data->second);
#else
      i_compare = compare_data.lower_bound(".");
      i_data = data.lower_bound(".");
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.lower_bound("A");
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound("A");
      CHECK(data.end() == i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.lower_bound("2");
      EMap::iterator i_data = data.lower_bound(Key("2"));
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.lower_bound(".");
      i_data = data.lower_bound(Key("."));
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.lower_bound("A");
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound(Key("A"));
      CHECK(data.end() == i_data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator i_compare = compare_data.lower_bound("4");
      Data::const_iterator i_data = data.lower_bound("4");
      CHECK(i_compare->second == i_data->second);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.lower_bound(".");
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound(".");
      CHECK(data.end() == i_data);

      i_compare = compare_data.lower_bound("A");
      i_data = data.lower_bound("A");
      CHECK(i_compare->second == i_data->second);
#else
      i_compare = compare_data.lower_bound(".");
      i_data = data.lower_bound(".");
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.lower_bound("A");
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound("A");
      CHECK(data.end() == i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound_const_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::const_iterator i_compare = compare_data.lower_bound("4");
      EMap::const_iterator i_data = data.lower_bound(Key("4"));
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.lower_bound(".");
      i_data = data.lower_bound(Key("."));
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.lower_bound("A");
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound(Key("A"));
      CHECK(data.end() == i_data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator i_compare = compare_data.upper_bound("1");
      Data::iterator i_data = data.upper_bound("1");
      CHECK(i_compare->second == i_data->second);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.upper_bound(".");
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound(".");
      CHECK(data.end() == i_data);

      i_compare = compare_data.upper_bound("A");
      i_data = data.upper_bound("A");
      CHECK(i_compare->second == i_data->second);
#else
      i_compare = compare_data.upper_bound(".");
      i_data = data.upper_bound(".");
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.upper_bound("A");
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound("A");
      CHECK(data.end() == i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.upper_bound("1");
      EMap::iterator i_data = data.upper_bound(Key("1"));
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.upper_bound(".");
      i_data = data.upper_bound(Key("."));
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.upper_bound("A");
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound(Key("A"));
      CHECK(data.end() == i_data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator i_compare = compare_data.upper_bound("3");
      Data::const_iterator i_data = data.upper_bound("3");
      CHECK(i_compare->second == i_data->second);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.upper_bound(".");
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound(".");
      CHECK(data.end() == i_data);

      i_compare = compare_data.upper_bound("A");
      i_data = data.upper_bound("A");
      CHECK(i_compare->second == i_data->second);
#else
      i_compare = compare_data.upper_bound(".");
      i_data = data.upper_bound(".");
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.upper_bound("A");
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound("A");
      CHECK(data.end() == i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_const_using_transparent_comparator)
    {
      using CMap = std::multimap<std::string, int, std::less<std::string>>;
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::const_iterator i_compare = compare_data.upper_bound("3");
      EMap::const_iterator i_data = data.upper_bound(Key("3"));
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.upper_bound(".");
      i_data = data.upper_bound(Key("."));
      CHECK(i_compare->second == i_data->second);

      i_compare = compare_data.upper_bound("A");
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound(Key("A"));
      CHECK(data.end() == i_data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_key_compare)
    {
      const Data data(initial_data.begin(), initial_data.end());

      Data::key_compare compare = data.key_comp();

      Data::key_type a("A");
      Data::key_type b("B");

#ifdef TEST_GREATER_THAN
      CHECK(!compare(a, b));
      CHECK(compare(b, a));
#else
      CHECK(compare(a, b));
      CHECK(!compare(b, a));
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_key_compare_using_transparent_comparator)
    {
      using EMap = etl::multimap<std::string, int, MAX_SIZE, etl::less<>>;
      const EMap data(initial_data.begin(), initial_data.end());

      EMap::key_compare compare = data.key_comp();

      std::string a("A");
      Key b("B");

      CHECK(compare(a, b));
      CHECK(!compare(b, a));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_value_compare)
    {
      const Data data(initial_data.begin(), initial_data.end());

      Data::value_compare compare = data.value_comp();

      Data::value_type a(std::string("A"), 0);
      Data::value_type b(std::string("B"), 1);

#ifdef TEST_GREATER_THAN
      CHECK(!compare(a, b));
      CHECK(compare(b, a));
#else
      CHECK(compare(a, b));
      CHECK(!compare(b, a));
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_lower_upper_bound)
    {
        Data data(initial_data_even.begin(), initial_data_even.end());
        Compare_Data compare(initial_data_even.begin(), initial_data_even.end());

        std::vector<ETL_OR_STD::pair<std::string, int> > tab(test_data.begin(), test_data.end());

        //make sure both data and compare contain same elements
        std::vector<ETL_OR_STD::pair<std::string, int> > data_elements(data.begin(), data.end());
        std::vector<ETL_OR_STD::pair<std::string, int> > compare_data_elements(compare.begin(), compare.end());

        CHECK(data_elements == compare_data_elements);
        CHECK(data_elements.size() == MAX_SIZE);

        for(std::vector<ETL_OR_STD::pair<std::string, int> >::iterator it = tab.begin() ; it != tab.end() ; ++it)
        {
            std::string i = it->first;

            //lower_bound
            CHECK((compare.lower_bound(i) == compare.end()) == (data.lower_bound(i) == data.end()));
            //if both end, or none
            if((compare.lower_bound(i) == compare.end()) == (data.lower_bound(i) == data.end()))
            {
                //if both are not end
                if(compare.lower_bound(i) != compare.end())
                {
                    CHECK((*compare.lower_bound(i)) == (*data.lower_bound(i)));
                }

                ETL_OR_STD::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> stlret = compare.equal_range(i);
                ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> etlret = data.equal_range(i);

                CHECK((stlret.first == compare.end()) == (etlret.first == data.end()));
                if((stlret.first != compare.end()) && (etlret.first != data.end()))
                {
                    CHECK((*stlret.first) == (*etlret.first));
                }
                CHECK((stlret.second == compare.end()) == (etlret.second == data.end()));
                if((stlret.second != compare.end()) && (etlret.second != data.end()))
                {
                    CHECK((*stlret.second) == (*etlret.second));
                }
            }

            //upper_bound
            CHECK((compare.upper_bound(i) == compare.end()) == (data.upper_bound(i) == data.end()));
            //if both end, or none
            if((compare.upper_bound(i) == compare.end()) == (data.upper_bound(i) == data.end()))
            {
                //if both are not end
                if(compare.upper_bound(i) != compare.end())
                {
                    CHECK((*compare.upper_bound(i)) == (*data.upper_bound(i)));
                }
            }
        }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_bug)
    {
      using Data = etl::multimap<int, int, 10>;

      int keys[10] = { 3, 2, 1, 0, 3, 0, 3, 0, 2, 2 };

      Data data;

      for (int eltNum = 0; eltNum != 10; ++eltNum)
      {
        data.insert(Data::value_type(keys[eltNum], eltNum));
      }

      data.erase(2);

      int prv = INT_MAX;
      Data::const_reverse_iterator pos;

      bool pass = true;

      for (pos = data.crbegin(); pos != data.crend(); ++pos)
      {
        if (pos->first > prv)
        {
          pass = false;
          break;
        }

        prv = pos->first;
      }

      CHECK(pass);
    }

    //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST_FIXTURE(SetupFixture, test_multimap_template_deduction)
    {
      using Pair = std::pair<const std::string, int>;

      etl::multimap data{ Pair("0", 0), Pair("1", 1), Pair("2", 2), Pair("3", 3), Pair("4", 4), Pair("5", 5) };

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same_v<Pair, Type>));

      decltype(data)::const_iterator itr = data.begin();

      CHECK_EQUAL(0, itr->second);
      ++itr;
      CHECK_EQUAL(1, itr->second);
      ++itr;
      CHECK_EQUAL(2, itr->second);
      ++itr;
      CHECK_EQUAL(3, itr->second);
      ++itr;
      CHECK_EQUAL(4, itr->second);
      ++itr;
      CHECK_EQUAL(5, itr->second);
    }
#endif

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST_FIXTURE(SetupFixture, test_make_multimap)
    {
      using Pair = ETL_OR_STD::pair<const std::string, int>;

      auto data = etl::make_multimap<std::string, int, std::less<std::string>>(Pair("0", 0), Pair("1", 1), Pair("2", 2), Pair("3", 3), Pair("4", 4), Pair("5", 5));

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same<Pair, Type>::value));

      decltype(data)::const_iterator itr = data.begin();

      CHECK_EQUAL(0, itr->second);
      ++itr;
      CHECK_EQUAL(1, itr->second);
      ++itr;
      CHECK_EQUAL(2, itr->second);
      ++itr;
      CHECK_EQUAL(3, itr->second);
      ++itr;
      CHECK_EQUAL(4, itr->second);
      ++itr;
      CHECK_EQUAL(5, itr->second);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains_with_transparent_comparator)
    {
      etl::multimap<std::string, int, MAX_SIZE, etl::less<>> data(initial_data.begin(), initial_data.end());

      CHECK(data.contains(std::string("1")));
      CHECK(data.contains(Key("1")));

      CHECK(!data.contains(std::string("99")));
      CHECK(!data.contains(Key("99")));
    }
  };
}
