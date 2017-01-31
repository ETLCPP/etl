/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove, rlindeman

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

#include <map>
#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>

#include "../src/map.h"

static const size_t SIZE = 10;

#define TEST_GREATER_THAN
#ifdef TEST_GREATER_THAN
typedef etl::map<std::string, int, SIZE, std::greater<std::string> >  Data;
typedef etl::imap<std::string, int, std::greater<std::string> >        IData;
typedef std::map<std::string, int, std::greater<std::string> >        Compare_Data;
#else
typedef etl::map<std::string, int, SIZE, std::less<std::string> >  Data;
typedef etl::imap<std::string, int, std::less<std::string> >       IData;
typedef std::map<std::string, int, std::less<std::string> >        Compare_Data;
#endif

typedef Data::iterator Data_iterator;
typedef Data::const_iterator Data_const_iterator;
typedef Compare_Data::iterator Compare_Data_iterator;
typedef Compare_Data::const_iterator Compare_Data_const_iterator;

//*************************************************************************
static std::ostream& operator << (std::ostream& os, const Data_iterator& it)
{
  os << (*it).first << " " << (*it).second;

  return os;
}

//*************************************************************************
static std::ostream& operator << (std::ostream& os, const Data_const_iterator& it)
{
  os << (*it).first << " " << (*it).second;

  return os;
}

//*************************************************************************
static std::ostream& operator << (std::ostream& os, const Compare_Data_iterator& it)
{
  os << (*it).first << " " << (*it).second;

  return os;
}

//*************************************************************************
static std::ostream& operator << (std::ostream& os, const Compare_Data_const_iterator& it)
{
  os << (*it).first << " " << (*it).second;

  return os;
}

namespace
{
  SUITE(test_map)
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
      // Maps of predefined data from which to construct maps used in each test
      std::map<std::string, int> initial_data;
      std::map<std::string, int> excess_data;
      std::map<std::string, int> different_data;
      std::map<std::string, int> random_data;

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
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      Data data;

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.capacity(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      size_t d = std::distance(data.begin(), data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
    }

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

      other_data = other_data;

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                other_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      Data data(initial_data.begin(), initial_data.end());
      const Data constData(data);

      CHECK_EQUAL(data.begin(), std::begin(data));
      CHECK_EQUAL(constData.begin(), std::begin(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      Data data(initial_data.begin(), initial_data.end());
      const Data constData(data);

      CHECK_EQUAL(data.end(), std::end(data));
      CHECK_EQUAL(constData.end(), std::end(constData));
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

      CHECK_EQUAL(data["0"], compare_data["0"]);
      CHECK_EQUAL(data["1"], compare_data["1"]);
      CHECK_EQUAL(data["2"], compare_data["2"]);
      CHECK_EQUAL(data["3"], compare_data["3"]);
      CHECK_EQUAL(data["4"], compare_data["4"]);
      CHECK_EQUAL(data["5"], compare_data["5"]);
      CHECK_EQUAL(data["6"], compare_data["6"]);
      CHECK_EQUAL(data["7"], compare_data["7"]);
      CHECK_EQUAL(data["8"], compare_data["8"]);
      CHECK_EQUAL(data["9"], compare_data["9"]);
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

      CHECK_EQUAL(data["0"], compare_data["0"]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at("0"), compare_data.at("0"));
      CHECK_EQUAL(data.at("1"), compare_data.at("1"));
      CHECK_EQUAL(data.at("2"), compare_data.at("2"));
      CHECK_EQUAL(data.at("3"), compare_data.at("3"));
      CHECK_EQUAL(data.at("4"), compare_data.at("4"));
      CHECK_EQUAL(data.at("5"), compare_data.at("5"));
      CHECK_EQUAL(data.at("6"), compare_data.at("6"));
      CHECK_EQUAL(data.at("7"), compare_data.at("7"));
      CHECK_EQUAL(data.at("8"), compare_data.at("8"));
      CHECK_EQUAL(data.at("9"), compare_data.at("9"));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_exception)
    {
      Data data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.at("10"), etl::map_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at("0"), compare_data.at("0"));
      CHECK_EQUAL(data.at("1"), compare_data.at("1"));
      CHECK_EQUAL(data.at("2"), compare_data.at("2"));
      CHECK_EQUAL(data.at("3"), compare_data.at("3"));
      CHECK_EQUAL(data.at("4"), compare_data.at("4"));
      CHECK_EQUAL(data.at("5"), compare_data.at("5"));
      CHECK_EQUAL(data.at("6"), compare_data.at("6"));
      CHECK_EQUAL(data.at("7"), compare_data.at("7"));
      CHECK_EQUAL(data.at("8"), compare_data.at("8"));
      CHECK_EQUAL(data.at("9"), compare_data.at("9"));
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

      std::pair<Data::iterator, bool> data_result =
        data.insert(Data::value_type(std::string("0"), 0));
      std::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(std::make_pair(std::string("0"), 0));

      // Check that both return successful return results
      CHECK_EQUAL(data_result.first->first, compare_result.first->first);
      CHECK_EQUAL(data_result.second, compare_result.second);

      // Try adding a duplicate (should return iterator pointing to duplicate)
      data_result = data.insert(Data::value_type(std::string("0"), 0));
      compare_result = compare_data.insert(std::make_pair(std::string("0"), 0));

      // Check that both return successful return results
      CHECK_EQUAL(data_result.first->first, compare_result.first->first);
      CHECK_EQUAL(data_result.second, compare_result.second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());
      CHECK(isEqual);

      data.insert(std::make_pair(std::string("2"), 2));
      compare_data.insert(std::make_pair(std::string("2"), 2));

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);

      data.insert(std::make_pair(std::string("1"), 1));
      compare_data.insert(std::make_pair(std::string("1"), 1));

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
      compare_data.insert(std::make_pair(std::string("0"), 0));

      data.insert(Data::value_type(std::string("1"), 1));
      compare_data.insert(std::make_pair(std::string("1"), 1));

      data.insert(Data::value_type(std::string("-1"), -1));
      compare_data.insert(std::make_pair(std::string("-1"), -1));

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_changed)
    {
      Compare_Data compare_data;
      Data data;

      data.insert(Data::value_type(std::string("0"), 0));
      compare_data.insert(std::make_pair(std::string("0"), 0));

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      data.insert(std::make_pair(std::string("0"), 1));
      compare_data.insert(std::make_pair(std::string("0"), 1));

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

      std::pair<Data::iterator, bool> data_result =
        data.insert(Data::value_type(std::string("2"), 2));
      std::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(std::make_pair(std::string("2"), 2));

      // Check that both return successful return results
      CHECK_EQUAL(data_result.first->first, compare_result.first->first);
      CHECK_EQUAL(data_result.second, compare_result.second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(data_result.first, std::make_pair(std::string("1"), 1));
      compare_data.insert(compare_result.first, std::make_pair(std::string("1"), 1));

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

      std::pair<Data::iterator, bool> data_result =
        data.insert(Data::value_type(std::string("2"), 2));
      std::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(std::make_pair(std::string("2"), 2));

      // Check that both return successful return results
      CHECK_EQUAL(data_result.first->first, compare_result.first->first);
      CHECK_EQUAL(data_result.second, compare_result.second);

      // Check that elements in map are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(Data::const_iterator(data_result.first),
        std::make_pair(std::string("1"), 1));
      compare_data.insert(Compare_Data::const_iterator(compare_result.first),
        std::make_pair(std::string("1"), 1));

      isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      Data data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.insert(std::make_pair(std::string("10"), 10)), etl::map_full);
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

      std::pair<Data::iterator, Data::iterator> data_result =
        data.equal_range("2");
      Data::iterator data_lb = data.lower_bound("2");
      std::pair<Compare_Data::iterator, Compare_Data::iterator> compare_result =
        compare_data.equal_range("2");
      Compare_Data::iterator compare_data_lb = compare_data.lower_bound("2");

      // Check that both return the same return results
      CHECK_EQUAL(data_lb->first, compare_data_lb->first);
      CHECK_EQUAL(data_lb->second, compare_data_lb->second);
      CHECK_EQUAL(data_result.first->first, compare_result.first->first);
      CHECK_EQUAL(data_result.first->second, compare_result.first->second);
      CHECK_EQUAL(data_result.second->first, compare_result.second->first);
      CHECK_EQUAL(data_result.second->second, compare_result.second->second);

      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_equal_range)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      std::pair<Data::const_iterator, Data::const_iterator> data_result =
        data.equal_range("2");
      std::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> compare_result =
        compare_data.equal_range("2");

      // Check that both return the same return results
      CHECK_EQUAL(data_result.first->first, compare_result.first->first);
      CHECK_EQUAL(data_result.first->second, compare_result.first->second);
      CHECK_EQUAL(data_result.second->first, compare_result.second->first);
      CHECK_EQUAL(data_result.second->second, compare_result.second->second);

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

      compare_data.erase("5");
      data.erase("5");

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

      std::advance(i_compare, 2);
      std::advance(i_data,    2);

      compare_data.erase(i_compare);
      data.erase(i_data);

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

      CHECK_EQUAL(i_compare1->second, i_data1->second);

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

      CHECK_EQUAL(data.size(), size_t(0));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count)
    {
      const Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.count("3"), size_t(1));

      CHECK_EQUAL(data.count("A"), size_t(0));
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
      CHECK_EQUAL(3, it->second);

      it = data.find("A");
      CHECK_EQUAL(data.end(), it);

      it = data.find("!");
      CHECK_EQUAL(data.end(), it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const Data data(initial_data.begin(), initial_data.end());

      Data::const_iterator it = data.find("3");
      CHECK_EQUAL(3, it->second);

      it = data.find("A");
      CHECK_EQUAL(data.end(), it);

      it = data.find("!");
      CHECK_EQUAL(data.end(), it);
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
      CHECK_EQUAL(i_compare->second, i_data->second);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.lower_bound(".");
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(".");
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.lower_bound("A");
      i_data = data.lower_bound("A");
      CHECK_EQUAL(i_compare->second, i_data->second);
#else
      i_compare = compare_data.lower_bound(".");
      i_data = data.lower_bound(".");
      CHECK_EQUAL(i_compare->second, i_data->second);

      i_compare = compare_data.lower_bound("A");
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound("A");
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator i_compare = compare_data.lower_bound("4");
      Data::const_iterator i_data = data.lower_bound("4");
      CHECK_EQUAL(i_compare->second, i_data->second);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.lower_bound(".");
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(".");
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.lower_bound("A");
      i_data = data.lower_bound("A");
      CHECK_EQUAL(i_compare->second, i_data->second);
#else
      i_compare = compare_data.lower_bound(".");
      i_data = data.lower_bound(".");
      CHECK_EQUAL(i_compare->second, i_data->second);

      i_compare = compare_data.lower_bound("A");
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound("A");
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator i_compare = compare_data.upper_bound("2");
      Data::iterator i_data = data.upper_bound("2");
      CHECK_EQUAL(i_compare->second, i_data->second);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.upper_bound(".");
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(".");
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.upper_bound("A");
      i_data = data.upper_bound("A");
      CHECK_EQUAL(i_compare->second, i_data->second);
#else
      i_compare = compare_data.upper_bound(".");
      i_data = data.upper_bound(".");
      CHECK_EQUAL(i_compare->second, i_data->second);

      i_compare = compare_data.upper_bound("A");
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound("A");
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator i_compare = compare_data.upper_bound("7");
      Data::const_iterator i_data = data.upper_bound("7");
      CHECK_EQUAL(i_compare->second, i_data->second);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.upper_bound(".");
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(".");
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.upper_bound("A");
      i_data = data.upper_bound("A");
      CHECK_EQUAL(i_compare->second, i_data->second);
#else
      i_compare = compare_data.upper_bound(".");
      i_data = data.upper_bound(".");
      CHECK_EQUAL(i_compare->second, i_data->second);

      i_compare = compare_data.upper_bound("A");
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound("A");
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

  };
}
