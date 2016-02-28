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

#include <set>
#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>

#include "../src/multiset.h"

static const size_t SIZE = 10;

#define TEST_GREATER_THAN
#ifdef TEST_GREATER_THAN
typedef etl::multiset<int, SIZE, std::greater<int> > Data;
typedef etl::imultiset<int, std::greater<int> >      IData;
typedef std::multiset<int, std::greater<int> >       Compare_Data;
#else
typedef etl::multiset<int, SIZE, std::less<int> > Data;
typedef etl::multiset<int, std::less<int> >       IData;
typedef std::multiset<int, std::less<int> >       Compare_Data;
#endif

typedef Data::iterator Data_iterator;
typedef Data::const_iterator Data_const_iterator;
typedef Compare_Data::iterator Compare_Data_iterator;
typedef Compare_Data::const_iterator Compare_Data_const_iterator;

//*************************************************************************
static std::ostream& operator << (std::ostream& os, const Data_iterator& it)
{
  os << (*it);

  return os;
}

//*************************************************************************
static std::ostream& operator << (std::ostream& os, const Data_const_iterator& it)
{
  os << (*it);

  return os;
}

//*************************************************************************
static std::ostream& operator << (std::ostream& os, const Compare_Data_iterator& it)
{
  os << (*it);

  return os;
}

namespace
{
  SUITE(test_multiset)
  {
    //*************************************************************************
    template <typename T1, typename T2>
    bool Check_Equal(T1 begin1, T1 end1, T2 begin2)
    {
      while (begin1 != end1)
      {
        if (*begin1 != *begin2)
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
      // Multisets of predefined data from which to constuct multisets used in
      // each test
      std::multiset<int> initial_data;
      std::multiset<int> excess_data;
      std::multiset<int> different_data;
      std::multiset<int> random_data;

      SetupFixture()
      {
        // Create a multiset of initial data
        initial_data.insert(0);
        initial_data.insert(0);
        initial_data.insert(1);
        initial_data.insert(1);
        initial_data.insert(1);
        initial_data.insert(2);
        initial_data.insert(2);
        initial_data.insert(2);
        initial_data.insert(3);
        initial_data.insert(4);

        // Create a multiset of excess data
        excess_data.insert(0);
        excess_data.insert(1);
        excess_data.insert(2);
        excess_data.insert(2);
        excess_data.insert(3);
        excess_data.insert(3);
        excess_data.insert(3);
        excess_data.insert(4);
        excess_data.insert(5);
        excess_data.insert(6);
        excess_data.insert(10);

        // Create a multiset of different data
        different_data.insert(10);
        different_data.insert(11);
        different_data.insert(12);
        different_data.insert(13);
        different_data.insert(14);
        different_data.insert(15);
        different_data.insert(16);
        different_data.insert(17);
        different_data.insert(18);
        different_data.insert(19);

        // Create a multiset of random data
        random_data.insert(3);
        random_data.insert(3);
        random_data.insert(0);
        random_data.insert(5);
        random_data.insert(6);
        random_data.insert(2);
        random_data.insert(0);
        random_data.insert(2);
        random_data.insert(4);
        random_data.insert(3);
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

      Data::iterator data_result = data.insert(2);
      Compare_Data::iterator compare_result = compare_data.insert(2);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      data_result = data.insert(1);
      compare_result = compare_data.insert(1);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      data_result = data.insert(2);
      compare_result = compare_data.insert(2);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      data_result = data.insert(3);
      compare_result = compare_data.insert(3);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      // Adding this next 2 will trigger a 3 node rotate RL on insert
      data_result = data.insert(2);
      compare_result = compare_data.insert(2);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      // Adding this next 4 will trigger a 2 node rotate left on insert
      data_result = data.insert(4);
      compare_result = compare_data.insert(4);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      data_result = data.insert(0);
      compare_result = compare_data.insert(0);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      // Check that elements in multiset are the same
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

      Data::iterator data_result = data.insert(0);
      Compare_Data::iterator compare_result = compare_data.insert(0);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      // Check that elements in multiset are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(data_result, 1);
      compare_data.insert(compare_result, 1);

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

      Data::iterator data_result = data.insert(2);
      Compare_Data::iterator compare_result = compare_data.insert(2);

      // Check that both return successful return results
      CHECK_EQUAL(*compare_result, *data_result);

      // Check that elements in multiset are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(Data::const_iterator(data_result), 1);
      compare_data.insert(Compare_Data::const_iterator(compare_result), 1);

      isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      Data data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.insert(10), etl::multiset_full);
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

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::multiset_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      Compare_Data compare_data(random_data.begin(), random_data.end());
      Data data(random_data.begin(), random_data.end());

      // Test a number not available
      std::pair<Data::iterator, Data::iterator> data_result =
        data.equal_range(1);
      std::pair<Compare_Data::iterator, Compare_Data::iterator> compare_result =
        compare_data.equal_range(1);

      // Check that both return the same return results
      CHECK_EQUAL(*compare_result.first, *data_result.first);
      CHECK_EQUAL(*compare_result.second, *data_result.second);

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

      // Test a number with several of the same key
      std::pair<Data::const_iterator, Data::const_iterator> data_result =
        data.equal_range(2);
      std::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> compare_result =
        compare_data.equal_range(2);

      // Check that both return the same return results
      CHECK_EQUAL(*compare_result.first, *data_result.first);
      CHECK_EQUAL(*compare_result.second, *data_result.second);

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

      size_t compare_count = compare_data.erase(2);
      size_t data_count = data.erase(2);

      // Check that both return the same return results
      CHECK_EQUAL(compare_count, data_count);

      // Erase another value
      compare_count = compare_data.erase(1);
      data_count = data.erase(1);

      // Check that both return the same return results
      CHECK_EQUAL(compare_count, data_count);

      // Erase another value
      compare_count = compare_data.erase(3);
      data_count = data.erase(3);

      // Check that both return the same return results
      CHECK_EQUAL(compare_count, data_count);

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

      std::advance(i_compare, 8);
      std::advance(i_data, 8);

      Compare_Data::const_iterator i_compare1 = compare_data.erase(i_compare);
      Data::const_iterator i_data1 = data.erase(i_data);

      CHECK_EQUAL(*i_compare1, *i_data1);

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

      CHECK_EQUAL(data.size(), size_t(0));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count)
    {
      const Data data(initial_data.begin(), initial_data.end());
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(compare_data.count(-1), data.count(-1));
      CHECK_EQUAL(compare_data.count(0), data.count(0));
      CHECK_EQUAL(compare_data.count(1), data.count(1));
      CHECK_EQUAL(compare_data.count(2), data.count(2));
      CHECK_EQUAL(compare_data.count(3), data.count(3));
      CHECK_EQUAL(compare_data.count(4), data.count(4));
      CHECK_EQUAL(compare_data.count(99), data.count(99));
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

      Data::iterator i_data = data.find(0);
      Compare_Data::iterator i_compare = compare_data.find(0);

      // Check that both return successful return results
      CHECK_EQUAL(*i_compare, *i_data);

      i_data = data.find(1);
      i_compare = compare_data.find(1);

      // Check that both return successful return results
      CHECK_EQUAL(*i_compare, *i_data);

      i_data = data.find(2);
      i_compare = compare_data.find(2);

      // Check that both return successful return results
      CHECK_EQUAL(*i_compare, *i_data);

      i_data = data.find(3);
      i_compare = compare_data.find(3);

      // Check that both return successful return results
      CHECK_EQUAL(*i_compare, *i_data);

      i_data = data.find(-1);
      i_compare = compare_data.find(-1);

      // Check that both return successful return results
      CHECK_EQUAL(data.end(), i_data);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.find(99);
      i_compare = compare_data.find(99);

      // Check that both return successful return results
      CHECK_EQUAL(data.end(), i_data);
      CHECK_EQUAL(compare_data.end(), i_compare);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Data::const_iterator i_data = data.find(0);
      Compare_Data::const_iterator i_compare = compare_data.find(0);

      // Check that both return successful return results
      CHECK_EQUAL(*i_compare, *i_data);

      i_data = data.find(1);
      i_compare = compare_data.find(1);

      // Check that both return successful return results
      CHECK_EQUAL(*i_compare, *i_data);

      i_data = data.find(2);
      i_compare = compare_data.find(2);

      // Check that both return successful return results
      CHECK_EQUAL(*i_compare, *i_data);

      i_data = data.find(3);
      i_compare = compare_data.find(3);

      // Check that both return successful return results
      CHECK_EQUAL(*i_compare, *i_data);

      i_data = data.find(-1);
      i_compare = compare_data.find(-1);

      // Check that both return successful return results
      CHECK_EQUAL(data.end(), i_data);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.find(99);
      i_compare = compare_data.find(99);

      // Check that both return successful return results
      CHECK_EQUAL(data.end(), i_data);
      CHECK_EQUAL(compare_data.end(), i_compare);
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

      Compare_Data::iterator i_compare = compare_data.lower_bound(2);
      Data::iterator i_data = data.lower_bound(2);
      CHECK_EQUAL(*i_compare, *i_data);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.lower_bound(-1);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(-1);
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.lower_bound(99);
      i_data = data.lower_bound(99);
      CHECK_EQUAL(*i_compare, *i_data);
#else
      i_compare = compare_data.lower_bound(-1);
      i_data = data.lower_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.lower_bound(99);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(99);
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator i_compare = compare_data.lower_bound(4);
      Data::const_iterator i_data = data.lower_bound(4);
      CHECK_EQUAL(*i_compare, *i_data);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.lower_bound(-1);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(-1);
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.lower_bound(99);
      i_data = data.lower_bound(99);
      CHECK_EQUAL(*i_compare, *i_data);
#else
      i_compare = compare_data.lower_bound(-1);
      i_data = data.lower_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.lower_bound(99);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(99);
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator i_compare = compare_data.upper_bound(1);
      Data::iterator i_data = data.upper_bound(1);
      CHECK_EQUAL(*i_compare, *i_data);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.upper_bound(-1);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(-1);
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.upper_bound(99);
      i_data = data.upper_bound(99);
      CHECK_EQUAL(*i_compare, *i_data);
#else
      i_compare = compare_data.upper_bound(-1);
      i_data = data.upper_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.upper_bound(99);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(99);
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator i_compare = compare_data.upper_bound(3);
      Data::const_iterator i_data = data.upper_bound(3);
      CHECK_EQUAL(*i_compare, *i_data);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.upper_bound(-1);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(-1);
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.upper_bound(99);
      i_data = data.upper_bound(99);
      CHECK_EQUAL(*i_compare, *i_data);
#else
      i_compare = compare_data.upper_bound(-1);
      i_data = data.upper_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.upper_bound(99);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(99);
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

  };
}
