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

#include "etl/map.h"

#include "data.h"

static const size_t MAX_SIZE = 10UL;

#define TEST_GREATER_THAN
#ifdef TEST_GREATER_THAN
using Data         = etl::map<std::string, int, MAX_SIZE, std::greater<std::string>>;
using IData        = etl::imap<std::string, int, std::greater<std::string>>;
using Compare_Data = std::map<std::string, int, std::greater<std::string>>;
#else
using Data = etl::map<std::string, int, MAX_SIZE, std::less<std::string>>;
using IData = etl::imap<std::string, int, std::less<std::string>>;
using Compare_Data = std::map<std::string, int, std::less<std::string>>;
#endif

using ItemM = TestDataM<int>;
using DataM = etl::map<std::string, ItemM, MAX_SIZE>;

using Data_iterator               = Data::iterator;
using Data_const_iterator         = Data::const_iterator;
using Compare_Data_iterator       = Compare_Data::iterator;
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

  SUITE(test_map)
  {
    //*************************************************************************
#include "etl/private/diagnostic_null_dereference_push.h"
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
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    struct SetupFixture
    {
      // Maps of predefined data from which to construct maps used in each test
      std::map<std::string, int> initial_data;
      std::map<std::string, int> excess_data;
      std::map<std::string, int> different_data;
      std::map<std::string, int> random_data;
      std::map<std::string, int> initial_data_even;
      std::map<std::string, int> test_data;

      SetupFixture()
      {
        // Create a map of initial data
        initial_data["0"] = 0;
        initial_data["1"] = 1;
        initial_data["2"] = 2;
        initial_data["3"] = 3;
        initial_data["4"] = 4;
        initial_data["5"] = 5;
        initial_data["6"] = 6;
        initial_data["7"] = 7;
        initial_data["8"] = 8;
        initial_data["9"] = 9;

        // Create a map of excess data
        excess_data["0"] = 0;
        excess_data["1"] = 1;
        excess_data["2"] = 2;
        excess_data["3"] = 3;
        excess_data["4"] = 4;
        excess_data["5"] = 5;
        excess_data["6"] = 6;
        excess_data["7"] = 7;
        excess_data["8"] = 8;
        excess_data["9"] = 9;
        excess_data["10"] = 10;

        // Create a map of different data
        different_data["10"] = 10;
        different_data["11"] = 11;
        different_data["12"] = 12;
        different_data["13"] = 13;
        different_data["14"] = 14;
        different_data["15"] = 15;
        different_data["16"] = 16;
        different_data["17"] = 17;
        different_data["18"] = 18;
        different_data["19"] = 19;

        // Create a map of random data
        random_data["6"] = 6;
        random_data["5"] = 5;
        random_data["0"] = 0;
        random_data["8"] = 8;
        random_data["9"] = 9;
        random_data["2"] = 2;
        random_data["1"] = 1;
        random_data["3"] = 3;
        random_data["7"] = 7;
        random_data["4"] = 4;

        //even values
        initial_data_even["00"] = 0;
        initial_data_even["02"] = 2;
        initial_data_even["04"] = 4;
        initial_data_even["06"] = 6;
        initial_data_even["08"] = 8;
        initial_data_even["10"] = 10;
        initial_data_even["12"] = 12;
        initial_data_even["14"] = 14;
        initial_data_even["16"] = 16;
        initial_data_even["18"] = 18;

        //test set
        test_data["00"] = 0;
        test_data["01"] = 1;
        test_data["02"] = 2;
        test_data["03"] = 3;
        test_data["04"] = 4;
        test_data["05"] = 5;
        test_data["06"] = 6;
        test_data["07"] = 7;
        test_data["08"] = 8;
        test_data["09"] = 9;
        test_data["10"] = 10;
        test_data["11"] = 11;
        test_data["12"] = 12;
        test_data["13"] = 13;
        test_data["14"] = 14;
        test_data["15"] = 15;
        test_data["16"] = 16;
        test_data["17"] = 17;
        test_data["18"] = 18;
        test_data["19"] = 19;
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      Data data;

      CHECK(data.size() == 0UL);
      CHECK(data.empty());
      CHECK(data.capacity() == MAX_SIZE);
      CHECK(data.max_size() == MAX_SIZE);
      CHECK(data.begin() == data.end());
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_cpp17_deduced_constructor)
    {
      etl::map data{ std::pair<std::string, int>("0", 0), std::pair<std::string, int>("1", 1), std::pair<std::string, int>("2", 2), std::pair<std::string, int>("3", 3), std::pair<std::string, int>("4", 4),
                     std::pair<std::string, int>("5", 5), std::pair<std::string, int>("6", 6), std::pair<std::string, int>("7", 7), std::pair<std::string, int>("8", 8), std::pair<std::string, int>("9", 9) };
      etl::map<std::string, int, 10U> check = { std::pair<std::string, int>("0", 0), std::pair<std::string, int>("1", 1), std::pair<std::string, int>("2", 2), std::pair<std::string, int>("3", 3), std::pair<std::string, int>("4", 4),
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

      CHECK(1 == data2.at("1").value);
      CHECK(2 == data2.at("2").value);
      CHECK(3 == data2.at("3").value);
      CHECK(4 == data2.at("4").value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_imap)
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

      size_t d = std::distance(data.begin(), data.end());

      CHECK(d == MAX_SIZE);
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

      CHECK(1 == data2.at("1").value);
      CHECK(2 == data2.at("2").value);
      CHECK(3 == data2.at("3").value);
      CHECK(4 == data2.at("4").value);
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
    TEST_FIXTURE(SetupFixture, test_index)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      CHECK(data["0"] == compare_data["0"]);
      CHECK(data["1"] == compare_data["1"]);
      CHECK(data["2"] == compare_data["2"]);
      CHECK(data["3"] == compare_data["3"]);
      CHECK(data["4"] == compare_data["4"]);
      CHECK(data["5"] == compare_data["5"]);
      CHECK(data["6"] == compare_data["6"]);
      CHECK(data["7"] == compare_data["7"]);
      CHECK(data["8"] == compare_data["8"]);
      CHECK(data["9"] == compare_data["9"]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_value_changed)
    {
      Compare_Data compare_data;
      Data data;

      data["0"]         = 0;
      compare_data["0"] = 0;

      data["0"]         = 1;
      compare_data["0"] = 1;

      CHECK(data["0"] == compare_data["0"]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK(data.at("0") == compare_data.at("0"));
      CHECK(data.at("1") == compare_data.at("1"));
      CHECK(data.at("2") == compare_data.at("2"));
      CHECK(data.at("3") == compare_data.at("3"));
      CHECK(data.at("4") == compare_data.at("4"));
      CHECK(data.at("5") == compare_data.at("5"));
      CHECK(data.at("6") == compare_data.at("6"));
      CHECK(data.at("7") == compare_data.at("7"));
      CHECK(data.at("8") == compare_data.at("8"));
      CHECK(data.at("9") == compare_data.at("9"));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_using_transparent_comparator)
    {
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CHECK(data.at(Key("0")) == compare_data.at("0"));
      CHECK(data.at(Key("1")) == compare_data.at("1"));
      CHECK(data.at(Key("2")) == compare_data.at("2"));
      CHECK(data.at(Key("3")) == compare_data.at("3"));
      CHECK(data.at(Key("4")) == compare_data.at("4"));
      CHECK(data.at(Key("5")) == compare_data.at("5"));
      CHECK(data.at(Key("6")) == compare_data.at("6"));
      CHECK(data.at(Key("7")) == compare_data.at("7"));
      CHECK(data.at(Key("8")) == compare_data.at("8"));
      CHECK(data.at(Key("9")) == compare_data.at("9"));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_exception)
    {
      Data data(initial_data.begin(), initial_data.end());
      data.erase("5");

      CHECK_THROW(data.at("-1"), etl::map_out_of_bounds);
      CHECK_THROW(data.at("5"), etl::map_out_of_bounds);
      CHECK_THROW(data.at("10"), etl::map_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      CHECK(data.at("0") == compare_data.at("0"));
      CHECK(data.at("1") == compare_data.at("1"));
      CHECK(data.at("2") == compare_data.at("2"));
      CHECK(data.at("3") == compare_data.at("3"));
      CHECK(data.at("4") == compare_data.at("4"));
      CHECK(data.at("5") == compare_data.at("5"));
      CHECK(data.at("6") == compare_data.at("6"));
      CHECK(data.at("7") == compare_data.at("7"));
      CHECK(data.at("8") == compare_data.at("8"));
      CHECK(data.at("9") == compare_data.at("9"));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const_using_transparent_comparator)
    {
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      const CMap compare_data(initial_data.begin(), initial_data.end());
      const EMap data(initial_data.begin(), initial_data.end());

      CHECK(data.at(Key("0")) == compare_data.at("0"));
      CHECK(data.at(Key("1")) == compare_data.at("1"));
      CHECK(data.at(Key("2")) == compare_data.at("2"));
      CHECK(data.at(Key("3")) == compare_data.at("3"));
      CHECK(data.at(Key("4")) == compare_data.at("4"));
      CHECK(data.at(Key("5")) == compare_data.at("5"));
      CHECK(data.at(Key("6")) == compare_data.at("6"));
      CHECK(data.at(Key("7")) == compare_data.at("7"));
      CHECK(data.at(Key("8")) == compare_data.at("8"));
      CHECK(data.at(Key("9")) == compare_data.at("9"));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const_exception)
    {
      const Data data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.at("10"), etl::map_out_of_bounds);
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

      ETL_OR_STD::pair<Data::iterator, bool> data_result =
        data.insert(Data::value_type(std::string("0"), 0));
      ETL_OR_STD::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(ETL_OR_STD::make_pair(std::string("0"), 0));

      // Check that both return successful return results
      CHECK(data_result.first->first == compare_result.first->first);
      CHECK(data_result.second == compare_result.second);

      // Try adding a duplicate (should return iterator pointing to duplicate)
      data_result = data.insert(Data::value_type(std::string("0"), 0));
      compare_result = compare_data.insert(ETL_OR_STD::make_pair(std::string("0"), 0));

      // Check that both return successful return results
      CHECK(data_result.first->first == compare_result.first->first);
      CHECK(data_result.second == compare_result.second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());
      CHECK(isEqual);

      data.insert(ETL_OR_STD::make_pair(std::string("2"), 2));
      compare_data.insert(ETL_OR_STD::make_pair(std::string("2"), 2));

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);

      data.insert(ETL_OR_STD::make_pair(std::string("1"), 1));
      compare_data.insert(ETL_OR_STD::make_pair(std::string("1"), 1));

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value2)
    {
      Compare_Data compare_data;
      Data data;

      data.insert(Data::value_type(std::string("0"), 0));
      compare_data.insert(ETL_OR_STD::make_pair(std::string("0"), 0));

      data.insert(Data::value_type(std::string("1"), 1));
      compare_data.insert(ETL_OR_STD::make_pair(std::string("1"), 1));

      data.insert(Data::value_type(std::string("-1"), -1));
      compare_data.insert(ETL_OR_STD::make_pair(std::string("-1"), -1));

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_moved_value)
    {
      DataM data;

      ItemM d1(1);
      ItemM d2(2);
      ItemM d3(3);

      data.insert(DataM::value_type(std::string("1"), etl::move(d1)));
      data.insert(DataM::value_type(std::string("2"), etl::move(d2)));
      data.insert(DataM::value_type(std::string("3"), etl::move(d3)));
      data.insert(DataM::value_type(std::string("4"), ItemM(4)));

      CHECK(!bool(d1));
      CHECK(!bool(d2));
      CHECK(!bool(d3));

      CHECK(1 == data.at("1").value);
      CHECK(2 == data.at("2").value);
      CHECK(3 == data.at("3").value);
      CHECK(4 == data.at("4").value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_changed)
    {
      Compare_Data compare_data;
      Data data;

      data.insert(Data::value_type(std::string("0"), 0));
      compare_data.insert(ETL_OR_STD::make_pair(std::string("0"), 0));

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      data.insert(ETL_OR_STD::make_pair(std::string("0"), 1));
      compare_data.insert(ETL_OR_STD::make_pair(std::string("0"), 1));

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_hint_value)
    {
      Compare_Data compare_data;
      Data data;

      ETL_OR_STD::pair<Data::iterator, bool> data_result =
        data.insert(Data::value_type(std::string("2"), 2));
      ETL_OR_STD::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(ETL_OR_STD::make_pair(std::string("2"), 2));

      // Check that both return successful return results
      CHECK(data_result.first->first == compare_result.first->first);
      CHECK(data_result.second == compare_result.second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(data_result.first, ETL_OR_STD::make_pair(std::string("1"), 1));
      compare_data.insert(compare_result.first, ETL_OR_STD::make_pair(std::string("1"), 1));

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

      ETL_OR_STD::pair<Data::iterator, bool> data_result =
        data.insert(Data::value_type(std::string("2"), 2));
      ETL_OR_STD::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(ETL_OR_STD::make_pair(std::string("2"), 2));

      // Check that both return successful return results
      CHECK(data_result.first->first == compare_result.first->first);
      CHECK(data_result.second == compare_result.second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(Data::const_iterator(data_result.first),
        ETL_OR_STD::make_pair(std::string("1"), 1));
      compare_data.insert(Compare_Data::const_iterator(compare_result.first),
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

      CHECK_THROW(data.insert(ETL_OR_STD::make_pair(std::string("10"), 10)), etl::map_full);
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

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::map_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      Compare_Data compare_data(random_data.begin(), random_data.end());
      Data data(random_data.begin(), random_data.end());

      ETL_OR_STD::pair<Data::iterator, Data::iterator> data_result = data.equal_range("2");
      Data::iterator data_lb = data.lower_bound("2");
      ETL_OR_STD::pair<Compare_Data::iterator, Compare_Data::iterator> compare_result = compare_data.equal_range("2");
      Compare_Data::iterator compare_data_lb = compare_data.lower_bound("2");

      // Check that both return the same return results
      CHECK(data_lb->first == compare_data_lb->first);
      CHECK(data_lb->second == compare_data_lb->second);
      CHECK(data_result.first->first == compare_result.first->first);
      CHECK(data_result.first->second == compare_result.first->second);
      CHECK(data_result.second->first == compare_result.second->first);
      CHECK(data_result.second->second == compare_result.second->second);

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_using_transparent_comparator)
    {
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(random_data.begin(), random_data.end());
      EMap data(random_data.begin(), random_data.end());

      ETL_OR_STD::pair<EMap::iterator, EMap::iterator> data_result = data.equal_range(Key("2"));
      EMap::iterator data_lb = data.lower_bound("2");
      ETL_OR_STD::pair<CMap::iterator, CMap::iterator> compare_result = compare_data.equal_range("2");
      CMap::iterator compare_data_lb = compare_data.lower_bound("2");

      // Check that both return the same return results
      CHECK(data_lb->first == compare_data_lb->first);
      CHECK(data_lb->second == compare_data_lb->second);
      CHECK(data_result.first->first == compare_result.first->first);
      CHECK(data_result.first->second == compare_result.first->second);
      CHECK(data_result.second->first == compare_result.second->first);
      CHECK(data_result.second->second == compare_result.second->second);

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_equal_range)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> data_result =
        data.equal_range("2");
      ETL_OR_STD::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> compare_result =
        compare_data.equal_range("2");

      // Check that both return the same return results
      CHECK(data_result.first->first == compare_result.first->first);
      CHECK(data_result.first->second == compare_result.first->second);
      CHECK(data_result.second->first == compare_result.second->first);
      CHECK(data_result.second->second == compare_result.second->second);

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_equal_range_using_transparent_comparator)
    {
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      const CMap compare_data(random_data.begin(), random_data.end());
      const EMap data(random_data.begin(), random_data.end());

      ETL_OR_STD::pair<EMap::const_iterator, EMap::const_iterator> data_result = data.equal_range(Key("2"));
      EMap::const_iterator data_lb = data.lower_bound("2");
      ETL_OR_STD::pair<CMap::const_iterator, CMap::const_iterator> compare_result = compare_data.equal_range("2");
      CMap::const_iterator compare_data_lb = compare_data.lower_bound("2");

      // Check that both return the same return results
      CHECK(data_lb->first == compare_data_lb->first);
      CHECK(data_lb->second == compare_data_lb->second);
      CHECK(data_result.first->first == compare_result.first->first);
      CHECK(data_result.first->second == compare_result.first->second);
      CHECK(data_result.second->first == compare_result.second->first);
      CHECK(data_result.second->second == compare_result.second->second);

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_value)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      compare_data.erase("5");
      data.erase("5");

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_value_using_transparent_comparator)
    {
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      compare_data.erase("5");
      data.erase(Key("5"));

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator i_compare = compare_data.begin();
      Data::iterator i_data            = data.begin();

      std::advance(i_compare, 2);
      std::advance(i_data,    2);

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

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

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

      std::advance(i_compare, 2);
      std::advance(i_data,    2);

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

      Compare_Data::const_iterator i_compare = compare_data.cbegin();
      Data::const_iterator i_data = data.cbegin();

      Compare_Data::const_iterator i_compare_end = compare_data.cbegin();
      Data::const_iterator i_data_end = data.cbegin();

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

      std::advance(i_compare_end, 6);
      std::advance(i_data_end, 6);

      compare_data.erase(i_compare, i_compare_end);
      data.erase(i_data, i_data_end);

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

      CHECK(data.size() == 0UL);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count)
    {
      const Data data(initial_data.begin(), initial_data.end());

      CHECK(data.count("3") == 1UL);

      CHECK(data.count("A") == 0UL);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count_using_transparent_comparator)
    {
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      const EMap data(initial_data.begin(), initial_data.end());

      CHECK(data.count(Key("3")) == 1UL);

      CHECK(data.count(Key("A")) == 0UL);
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
      Data data(initial_data.begin(), initial_data.end());

      Data::iterator it = data.find("3");
      CHECK(3 == it->second);

      it = data.find("A");
      CHECK(data.end() == it);

      it = data.find("!");
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_using_transparent_comparator)
    {
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      EMap data(initial_data.begin(), initial_data.end());

      EMap::iterator it = data.find(Key("3"));
      CHECK(3 == it->second);

      it = data.find(Key("A"));
      CHECK(data.end() == it);

      it = data.find(Key("!"));
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const Data data(initial_data.begin(), initial_data.end());

      Data::const_iterator it = data.find("3");
      CHECK(3 == it->second);

      it = data.find("A");
      CHECK(data.end() == it);

      it = data.find("!");
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_using_transparent_comparator)
    {
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      const EMap data(initial_data.begin(), initial_data.end());

      EMap::const_iterator it = data.find(Key("3"));
      CHECK(3 == it->second);

      it = data.find(Key("A"));
      CHECK(data.end() == it);

      it = data.find(Key("!"));
      CHECK(data.end() == it);
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

      Compare_Data::iterator i_compare = compare_data.lower_bound("8");
      Data::iterator i_data = data.lower_bound("8");
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
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.lower_bound("8");
      EMap::iterator i_data = data.lower_bound(Key("8"));
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
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      const CMap compare_data(initial_data.begin(), initial_data.end());
      const EMap data(initial_data.begin(), initial_data.end());

      CMap::const_iterator i_compare = compare_data.lower_bound("8");
      EMap::const_iterator i_data = data.lower_bound(Key("8"));
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

      Compare_Data::iterator i_compare = compare_data.upper_bound("2");
      Data::iterator i_data = data.upper_bound("2");
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
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.upper_bound("2");
      EMap::iterator i_data = data.upper_bound(Key("2"));
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

      Compare_Data::const_iterator i_compare = compare_data.upper_bound("7");
      Data::const_iterator i_data = data.upper_bound("7");
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
      using CMap = std::map<std::string, int, std::less<std::string>>;
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;

      const CMap compare_data(initial_data.begin(), initial_data.end());
      const EMap data(initial_data.begin(), initial_data.end());

      CMap::const_iterator i_compare = compare_data.upper_bound("2");
      EMap::const_iterator i_data = data.upper_bound(Key("2"));
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
      using EMap = etl::map<std::string, int, MAX_SIZE, etl::less<>>;
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
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST_FIXTURE(SetupFixture, test_map_template_deduction)
    {
      using Pair = std::pair<const std::string, int>;

      etl::map data { Pair{"0", 0}, Pair{"1", 1}, Pair{"2", 2}, Pair{"3", 3}, Pair{"4", 4}, Pair{"5", 5} };

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same_v<Pair, Type>));

      CHECK_EQUAL(0, data.at("0"));
      CHECK_EQUAL(1, data.at("1"));
      CHECK_EQUAL(2, data.at("2"));
      CHECK_EQUAL(3, data.at("3"));
      CHECK_EQUAL(4, data.at("4"));
      CHECK_EQUAL(5, data.at("5"));
    }
#endif

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST_FIXTURE(SetupFixture, test_make_map)
    {
      using Pair = ETL_OR_STD::pair<const std::string, int>;

      auto data = etl::make_map<std::string, int, std::less<std::string>>(Pair{ "0", 0 }, Pair{ "1", 1 }, Pair{ "2", 2 }, Pair{ "3", 3 }, Pair{ "4", 4 }, Pair{ "5", 5 });

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same<Pair, Type>::value));

      CHECK_EQUAL(0, data.at("0"));
      CHECK_EQUAL(1, data.at("1"));
      CHECK_EQUAL(2, data.at("2"));
      CHECK_EQUAL(3, data.at("3"));
      CHECK_EQUAL(4, data.at("4"));
      CHECK_EQUAL(5, data.at("5"));
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains)
    {
      Data data(initial_data.begin(), initial_data.end());

      CHECK(data.contains(std::string("1")));
      CHECK(!data.contains(std::string("99")));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains_with_transparent_comparator)
    {
      etl::map<std::string, int, MAX_SIZE, etl::less<>> data(initial_data.begin(), initial_data.end());

      CHECK(data.contains(std::string("1")));
      CHECK(data.contains(Key("1")));

      CHECK(!data.contains(std::string("99")));
      CHECK(!data.contains(Key("99")));
    }
  };
}
