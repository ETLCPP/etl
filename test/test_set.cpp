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
#include <vector>

#include "../src/set.h"

static const size_t SIZE = 10;

#define TEST_GREATER_THAN
#ifdef TEST_GREATER_THAN
typedef etl::set<int, SIZE, std::greater<int> >  Data;
typedef etl::iset<int, std::greater<int> >       IData;
typedef std::set<int, std::greater<int> >        Compare_Data;
#else
typedef etl::set<int, SIZE, std::less<int> >  Data;
typedef std::set<int, std::less<int> >        Compare_Data;
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
  SUITE(test_set)
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
      std::vector<int> initial_data;
      std::vector<int> excess_data;
      std::vector<int> different_data;
      std::vector<int> random_data;

      SetupFixture()
      {
        int n[] =
        {
          0,
          1,
          2,
          3,
          4,
          5,
          6,
          7,
          8,
          9,
       };

        Data::value_type n2[] =
        {
          0,
          1,
          2,
          3,
          4,
          5,
          6,
          7,
          8,
          9,
          10,
       };

        int n3[] =
        {
          10,
          11,
          12,
          13,
          14,
          15,
          16,
          17,
          18,
          19,
       };

        int n4[] =
        {
          6,
          5,
          0,
          8,
          9,
          2,
          1,
          3,
          7,
          4,
        };

        initial_data.assign(std::begin(n), std::end(n));
        excess_data.assign(std::begin(n2), std::end(n2));
        different_data.assign(std::begin(n3), std::end(n3));
        random_data.assign(std::begin(n4), std::end(n4));
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

      bool isEqual = Check_Equal(data1.begin(),
                                 data1.end(),
                                 data2.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data otherData(data);

      data = data;

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 otherData.begin());

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

      std::pair<Data::iterator, bool> data_result =
        data.insert(0);
      std::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(0);

      // Check that both return successful return results
      CHECK_EQUAL(*data_result.first, *compare_result.first);

      // Try adding a duplicate (should return iterator pointing to duplicate)
      data_result = data.insert(0);
      compare_result = compare_data.insert(0);

      // Check that both return successful return results
      CHECK_EQUAL(*data_result.first, *compare_result.first);

      // Check that elements in set are the same
      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());
      CHECK(isEqual);

      data.insert(2);
      compare_data.insert(2);

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);

      data.insert(1);
      compare_data.insert(1);

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

      std::pair<Data::iterator, bool> data_result = data.insert(2);
      std::pair<Compare_Data::iterator, bool> compare_result = compare_data.insert(2);

      // Check that both return successful return results
      CHECK_EQUAL(*data_result.first, *compare_result.first);

      // Check that elements in set are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(data_result.first, 1);
      compare_data.insert(compare_result.first, 1);

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
        data.insert(2);
      std::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(2);

      // Check that both return successful return results
      CHECK_EQUAL(*data_result.first, *compare_result.first);

      // Check that elements in set are the same
      bool isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());
      CHECK(isEqual);

      data.insert(Data::const_iterator(data_result.first), 1);
      compare_data.insert(Compare_Data::const_iterator(compare_result.first), 1);

      isEqual = Check_Equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      Data data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.insert(10), etl::set_full);
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

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::set_full);
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      Compare_Data compare_data(random_data.begin(), random_data.end());
      Data data(random_data.begin(), random_data.end());

      std::pair<Data::iterator, Data::iterator> data_result =
        data.equal_range(2);
      Data::iterator data_lb = data.lower_bound(2);
      std::pair<Compare_Data::iterator, Compare_Data::iterator> compare_result =
        compare_data.equal_range(2);
      Compare_Data::iterator compare_data_lb = compare_data.lower_bound(2);

      // Check that both return the same return results
      CHECK_EQUAL(*data_lb, *compare_data_lb);
      CHECK_EQUAL(*data_result.first, *compare_result.first);
      CHECK_EQUAL(*data_result.second, *compare_result.second);

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
        data.equal_range(2);
      std::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> compare_result =
        compare_data.equal_range(2);

      // Check that both return the same return results
      CHECK_EQUAL(*data_result.first, *compare_result.first);
      CHECK_EQUAL(*data_result.second, *compare_result.second);

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

      compare_data.erase(5);
      data.erase(5);

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

      CHECK_EQUAL(data.count(3), size_t(1));

      CHECK_EQUAL(data.count(11), size_t(0));
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

      Data::iterator it = data.find(3);
      CHECK_EQUAL(3, *it);

      it = data.find(11);
      CHECK_EQUAL(data.end(), it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const Data data(initial_data.begin(), initial_data.end());

      Data::const_iterator it = data.find(3);
      CHECK_EQUAL(3, *it);

      it = data.find(11);
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

      Compare_Data::iterator i_compare = compare_data.lower_bound(8);
      Data::iterator i_data = data.lower_bound(8);
      CHECK_EQUAL(*i_compare, *i_data);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.lower_bound(-1);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(-1);
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.lower_bound(11);
      i_data = data.lower_bound(11);
      CHECK_EQUAL(*i_compare, *i_data);
#else
      i_compare = compare_data.lower_bound(-1);
      i_data = data.lower_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.lower_bound(11);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(11);
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

      i_compare = compare_data.lower_bound(11);
      i_data = data.lower_bound(11);
      CHECK_EQUAL(*i_compare, *i_data);
#else
      i_compare = compare_data.lower_bound(-1);
      i_data = data.lower_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.lower_bound(11);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.lower_bound(11);
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator i_compare = compare_data.upper_bound(2);
      Data::iterator i_data = data.upper_bound(2);
      CHECK_EQUAL(*i_compare, *i_data);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.upper_bound(-1);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(-1);
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.upper_bound(11);
      i_data = data.upper_bound(11);
      CHECK_EQUAL(*i_compare, *i_data);
#else
      i_compare = compare_data.upper_bound(-1);
      i_data = data.upper_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.upper_bound(11);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(11);
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator i_compare = compare_data.upper_bound(7);
      Data::const_iterator i_data = data.upper_bound(7);
      CHECK_EQUAL(*i_compare, *i_data);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.upper_bound(-1);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(-1);
      CHECK_EQUAL(data.end(), i_data);

      i_compare = compare_data.upper_bound(11);
      i_data = data.upper_bound(11);
      CHECK_EQUAL(*i_compare, *i_data);
#else
      i_compare = compare_data.upper_bound(-1);
      i_data = data.upper_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.upper_bound(11);
      CHECK_EQUAL(compare_data.end(), i_compare);

      i_data = data.upper_bound(11);
      CHECK_EQUAL(data.end(), i_data);
#endif
    }

  };
}
