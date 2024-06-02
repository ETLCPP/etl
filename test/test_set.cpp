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

#include <set>
#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>

#include "etl/set.h"
#include "etl/checksum.h"

#include "data.h"

static const size_t MAX_SIZE = 10UL;

#define TEST_GREATER_THAN
#ifdef TEST_GREATER_THAN
  using Data = etl::set<int, MAX_SIZE, std::greater<int>>;
  using IData = etl::iset<int, std::greater<int>>;
  using Compare_Data = std::set<int, std::greater<int>>;
#else
  using Data = etl::set<int, MAX_SIZE, std::less<int>>;
  using IData = etl::iset<int, std::less<int>>;
  using Compare_Data = std::set<int, std::less<int>>;
#endif

using ItemM = TestDataM<int>;

struct simple_hash
{
  size_t operator ()(const ItemM& value) const
  {
    etl::checksum<size_t> sum;

    sum.add(value.valid);
    sum.add(value.value);

    return sum.value();
  }
};

using DataM = etl::set<ItemM, MAX_SIZE>;

using Data_iterator               = Data::iterator;
using Data_const_iterator         = Data::const_iterator;
using Compare_Data_iterator       = Compare_Data::iterator;
using Compare_Data_const_iterator = Compare_Data::const_iterator;

namespace
{
  struct Key
  {
    Key(int k_)
      : k(k_)
    {
    }

    int k;
  };

  bool operator <(const Key& lhs, const int& rhs)
  {
    return (lhs.k < rhs);
  }

  bool operator <(const int& lhs, const Key& rhs)
  {
    return (lhs < rhs.k);
  }

  //bool operator <(const Key& lhs, const Key& rhs)
  //{
  //  return (lhs.k < rhs.k);
  //}

  SUITE(test_set)
  {
    //*************************************************************************
#include "etl/private/diagnostic_null_dereference_push.h"
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
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    struct SetupFixture
    {
      std::vector<int> initial_data;
      std::vector<int> excess_data;
      std::vector<int> different_data;
      std::vector<int> random_data;
      std::vector<int> initial_data_even;
      std::vector<int> test_data;

      SetupFixture()
      {
        int n[] =
        {
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
       };

        Data::value_type n2[] =
        {
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
       };

        int n3[] =
        {
          10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
       };

        int n4[] =
        {
          6, 5, 0, 8, 9, 2, 1, 3, 7, 4,
        };

        int n_even[] =
        {
          0, 2, 4, 6, 8, 10, 12, 14, 16, 18,
        };

        int n5[] =
        {
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
        } ;

        initial_data.assign(std::begin(n), std::end(n));
        excess_data.assign(std::begin(n2), std::end(n2));
        different_data.assign(std::begin(n3), std::end(n3));
        random_data.assign(std::begin(n4), std::end(n4));
        initial_data_even.assign(std::begin(n_even), std::end(n_even));
        test_data.assign(std::begin(n5), std::end(n5));
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      Data data;

      CHECK_EQUAL(data.size(), 0UL);
      CHECK(data.empty());
      CHECK_EQUAL(MAX_SIZE, data.available());
      CHECK_EQUAL(MAX_SIZE, data.capacity());
      CHECK_EQUAL(MAX_SIZE, data.max_size());
      CHECK(data.begin() == data.end());
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_cpp17_deduced_constructor)
    {
      etl::set data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::set<int, 10U> check = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

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

      CHECK_EQUAL(initial_data.size(), data1.size());
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

      data1.insert(etl::move(d1));
      data1.insert(etl::move(d2));
      data1.insert(etl::move(d3));
      data1.insert(ItemM(4));

      DataM data2(std::move(data1));

      CHECK(!data1.empty()); // Move does not clear the source.

      CHECK(data2.find(ItemM(1)) != data2.end());
      CHECK(data2.find(ItemM(2)) != data2.end());
      CHECK(data2.find(ItemM(3)) != data2.end());
      CHECK(data2.find(ItemM(4)) != data2.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_iset)
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

      bool isEqual = std::equal(data.begin(), data.end(), compare_data.begin());
      CHECK(isEqual);
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_initializer_list)
    {
      Compare_Data compare_data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(!data.empty());

      bool isEqual = std::equal(data.begin(), data.end(), compare_data.begin());
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

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      data = data;
#include "etl/private/diagnostic_pop.h" 

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 otherData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      DataM data1;

      ItemM d1(1);
      ItemM d2(2);
      ItemM d3(3);

      data1.insert(etl::move(d1));
      data1.insert(etl::move(d2));
      data1.insert(etl::move(d3));
      data1.insert(ItemM(4));

      DataM data2;
      data2.insert(ItemM(5));

      data2 = std::move(data1);

      CHECK(!data1.empty()); // Move does not clear the source.
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

      ETL_OR_STD::pair<Data::iterator, bool> data_result =
        data.insert(0);
      ETL_OR_STD::pair<Compare_Data::iterator, bool> compare_result =
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

      ETL_OR_STD::pair<Data::iterator, bool> data_result = data.insert(2);
      ETL_OR_STD::pair<Compare_Data::iterator, bool> compare_result = compare_data.insert(2);

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

      ETL_OR_STD::pair<Data::iterator, bool> data_result =
        data.insert(2);
      ETL_OR_STD::pair<Compare_Data::iterator, bool> compare_result =
        compare_data.insert(2);

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
    TEST_FIXTURE(SetupFixture, test_insert_moved_value)
    {
      DataM data;

      ItemM d1(1);
      ItemM d2(2);
      ItemM d3(3);

      data.insert(etl::move(d1));
      data.insert(etl::move(d2));
      data.insert(etl::move(d3));
      data.insert(ItemM(4));

      CHECK(!bool(d1));
      CHECK(!bool(d2));
      CHECK(!bool(d3));

      CHECK_EQUAL(1, data.find(ItemM(1))->value);
      CHECK_EQUAL(2, data.find(ItemM(2))->value);
      CHECK_EQUAL(3, data.find(ItemM(3))->value);
      CHECK_EQUAL(4, data.find(ItemM(4))->value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_existing_value_when_full)
    {
      Compare_Data compare_data;
      Data data;
      ETL_OR_STD::pair<Data::iterator, bool> data_result;
      ETL_OR_STD::pair<Compare_Data::iterator, bool> compare_result;

      for (size_t i = 0; i < MAX_SIZE; ++i)
      {
        data_result = data.insert(i);
        compare_result = compare_data.insert(i);

        // Check that both return successful return results
        CHECK_EQUAL(*data_result.first, *compare_result.first);
      }

      // Try to insert when set is full should throw etl::set_full
      CHECK_THROW(data_result = data.insert(MAX_SIZE), etl::set_full);

      // Try adding a duplicate (should return iterator pointing to duplicate) not throw error
      for (size_t i = 0; i < MAX_SIZE; ++i)
      {
        data_result = data.insert(i);
        compare_result = compare_data.insert(i);

        // Check that both return successful return results
        CHECK_EQUAL(*data_result.first, *compare_result.first);
      }



      // Check that elements in set are the same
      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());
      CHECK(isEqual);

    }

    ////*************************************************************************
    //TEST_FIXTURE(SetupFixture, test_emplace_value)
    //{
    //  Compare_Data compare_data;
    //  Data data;

    //  ETL_OR_STD::pair<Data::iterator, bool> data_result = data.emplace(0);
    //  ETL_OR_STD::pair<Compare_Data::iterator, bool> compare_result = compare_data.emplace(0);

    //  // Check that both return successful return results
    //  CHECK_EQUAL(*data_result.first, *compare_result.first);

    //  // Try adding a duplicate (should return iterator pointing to duplicate)
    //  data_result = data.insert(0);
    //  compare_result = compare_data.insert(0);

    //  // Check that both return successful return results
    //  CHECK_EQUAL(*data_result.first, *compare_result.first);

    //  // Check that elements in set are the same
    //  bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());
    //  CHECK(isEqual);

    //  data.insert(2);
    //  compare_data.insert(2);

    //  isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

    //  CHECK(isEqual);

    //  data.insert(1);
    //  compare_data.insert(1);

    //  isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

    //  CHECK(isEqual);
    //}

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      Compare_Data compare_data(random_data.begin(), random_data.end());
      Data data(random_data.begin(), random_data.end());

      ETL_OR_STD::pair<Data::iterator, Data::iterator> data_result = data.equal_range(2);
      Data::iterator data_lb = data.lower_bound(2);
      ETL_OR_STD::pair<Compare_Data::iterator, Compare_Data::iterator> compare_result = compare_data.equal_range(2);
      Compare_Data::iterator compare_data_lb = compare_data.lower_bound(2);

      // Check that both return the same return results
      CHECK_EQUAL(*data_lb, *compare_data_lb);
      CHECK_EQUAL(*data_result.first, *compare_result.first);
      CHECK_EQUAL(*data_result.second, *compare_result.second);

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_equal_range)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> data_result =
        data.equal_range(2);
      ETL_OR_STD::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> compare_result =
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

      bool isEqual = Check_Equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_value_using_transparent_comparator)
    {
      using CMap = std::set<int, std::less<int>>;
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      compare_data.erase(5);
      data.erase(Key(5));

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

      CHECK_EQUAL(*i_compare1, *i_data1);

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

      CHECK_EQUAL(data.size(), 0UL);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count)
    {
      const Data data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.count(3), 1UL);

      CHECK_EQUAL(data.count(11), 0UL);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count_using_transparent_comparator)
    {
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;
      const EMap data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.count(Key(3)), 1UL);

      CHECK_EQUAL(data.count(Key(11)), 0UL);
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
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_using_transparent_comparator)
    {
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      EMap::iterator it = data.find(Key(3));
      CHECK_EQUAL(3, *it);

      it = data.find(11);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const Data data(initial_data.begin(), initial_data.end());

      Data::const_iterator it = data.find(3);
      CHECK_EQUAL(3, *it);

      it = data.find(11);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_using_transparent_comparator)
    {
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;
      const EMap data(initial_data.begin(), initial_data.end());

      EMap::const_iterator it = data.find(Key(3));
      CHECK_EQUAL(3, *it);

      it = data.find(11);
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

      Compare_Data::iterator i_compare = compare_data.lower_bound(8);
      Data::iterator i_data = data.lower_bound(8);
      CHECK_EQUAL(*i_compare, *i_data);

#ifdef TEST_GREATER_THAN
      i_compare = compare_data.lower_bound(-1);
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound(-1);
      CHECK(data.end() == i_data);

      i_compare = compare_data.lower_bound(11);
      i_data = data.lower_bound(11);
      CHECK(i_data != data.end());

      if ((i_data != data.end()) && (i_compare != compare_data.end()))
      {
        CHECK(*i_compare == *i_data);
      }
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
    TEST_FIXTURE(SetupFixture, test_lower_bound_using_transparent_comparator)
    {
      using CMap = std::set<int, std::less<int>>;
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.lower_bound(8);
      EMap::iterator i_data = data.lower_bound(Key(8));
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.lower_bound(-1);
      i_data = data.lower_bound(Key(-1));
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.lower_bound(11);
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound(Key(11));
      CHECK(data.end() == i_data);
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
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound(-1);
      CHECK(data.end() == i_data);

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
    TEST_FIXTURE(SetupFixture, test_lower_bound_const_using_transparent_comparator)
    {
      using CMap = std::set<int, std::less<int>>;
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;

      const CMap compare_data(initial_data.begin(), initial_data.end());
      const EMap data(initial_data.begin(), initial_data.end());

      CMap::const_iterator i_compare = compare_data.lower_bound(4);
      EMap::const_iterator i_data = data.lower_bound(Key(4));
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.lower_bound(-1);
      i_data = data.lower_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.lower_bound(11);
      CHECK(compare_data.end() == i_compare);

      i_data = data.lower_bound(11);
      CHECK(data.end() == i_data);
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
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound(-1);
      CHECK(data.end() == i_data);

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
    TEST_FIXTURE(SetupFixture, test_upper_bound_using_transparent_comparator)
    {
      using CMap = std::set<int, std::less<int>>;
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.upper_bound(2);
      EMap::iterator i_data = data.upper_bound(Key(2));
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.upper_bound(-1);
      i_data = data.upper_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.upper_bound(11);
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound(11);
      CHECK(data.end() == i_data);
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
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound(-1);
      CHECK(data.end() == i_data);

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
    TEST_FIXTURE(SetupFixture, test_upper_bound_const_using_transparent_comparator)
    {
      using CMap = std::set<int, std::less<int>>;
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::const_iterator i_compare = compare_data.upper_bound(7);
      EMap::const_iterator i_data = data.upper_bound(Key(7));
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.upper_bound(-1);
      i_data = data.upper_bound(-1);
      CHECK_EQUAL(*i_compare, *i_data);

      i_compare = compare_data.upper_bound(11);
      CHECK(compare_data.end() == i_compare);

      i_data = data.upper_bound(11);
      CHECK(data.end() == i_data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_key_compare)
    {
      const Data data(initial_data.begin(), initial_data.end());

      Data::key_compare compare = data.key_comp();

      Data::key_type a(1);
      Data::key_type b(2);

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
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;
      const EMap data(initial_data.begin(), initial_data.end());

      EMap::key_compare compare = data.key_comp();

      int a(1);
      Key b(2);

      CHECK(compare(a, b));
      CHECK(!compare(b, a));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_value_compare)
    {
      const Data data(initial_data.begin(), initial_data.end());

      Data::value_compare compare = data.value_comp();

      Data::value_type a(1);
      Data::value_type b(2);

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

        std::vector<int> tab(test_data.begin(), test_data.end());

        //make sure both data and compare contain same elements
        std::vector<int> data_elements(data.begin(), data.end());
        std::vector<int> compare_data_elements(compare.begin(), compare.end());

        CHECK(data_elements == compare_data_elements);
        CHECK_EQUAL(data_elements.size(), MAX_SIZE);

        for(std::vector<int>::iterator it = tab.begin() ; it != tab.end() ; ++it)
        {
            int i = *it;

            //lower_bound
            CHECK_EQUAL(compare.lower_bound(i) == compare.end(), data.lower_bound(i) == data.end());

            //if both end, or none
            if((compare.lower_bound(i) == compare.end()) == (data.lower_bound(i) == data.end()))
            {
                //if both are not end
                if(compare.lower_bound(i) != compare.end())
                {
                    CHECK_EQUAL(*compare.lower_bound(i), *data.lower_bound(i));
                }

                ETL_OR_STD::pair<Compare_Data::const_iterator, Compare_Data::const_iterator> stlret = compare.equal_range(i);
                ETL_OR_STD::pair<Data::const_iterator, Data::const_iterator> etlret = data.equal_range(i);

                CHECK_EQUAL(stlret.first == compare.end(), etlret.first == data.end());
                
                if((stlret.first != compare.end()) && (etlret.first != data.end()))
                {
                    CHECK_EQUAL(*stlret.first, *etlret.first);
                }
                
                CHECK_EQUAL(stlret.second == compare.end(), etlret.second == data.end());
                
                if((stlret.second != compare.end()) && (etlret.second != data.end()))
                {
                    CHECK_EQUAL(*stlret.second, *etlret.second);
                }
            }

            //upper_bound
            CHECK_EQUAL(compare.upper_bound(i) == compare.end(), data.upper_bound(i) == data.end());

            //if both end, or none
            if((compare.upper_bound(i) == compare.end()) == (data.upper_bound(i) == data.end()))
            {
                //if both are not end
                if(compare.upper_bound(i) != compare.end())
                {
                    CHECK_EQUAL(*compare.upper_bound(i), *data.upper_bound(i));
                }
            }
        }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_compare_lower_upper_bound_using_transparent_comparator)
    {
      using CMap = std::set<int, std::less<int>>;
      using EMap = etl::set<int, MAX_SIZE, etl::less<>>;

      CMap compare(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      std::vector<int> tab(test_data.begin(), test_data.end());

      //make sure both data and compare contain same elements
      std::vector<int> data_elements(data.begin(), data.end());
      std::vector<int> compare_data_elements(compare.begin(), compare.end());

      CHECK(data_elements == compare_data_elements);
      CHECK_EQUAL(data_elements.size(), MAX_SIZE);

      for (std::vector<int>::iterator it = tab.begin(); it != tab.end(); ++it)
      {
        int i = *it;

        //lower_bound
        CHECK_EQUAL(compare.lower_bound(i) == compare.end(), data.lower_bound(i) == data.end());

        //if both end, or none
        if ((compare.lower_bound(i) == compare.end()) == (data.lower_bound(i) == data.end()))
        {
          //if both are not end
          if (compare.lower_bound(i) != compare.end())
          {
            CHECK_EQUAL(*compare.lower_bound(i), *data.lower_bound(i));
          }

          ETL_OR_STD::pair<CMap::const_iterator, CMap::const_iterator> stlret = compare.equal_range(i);
          ETL_OR_STD::pair<EMap::const_iterator, EMap::const_iterator> etlret = data.equal_range(Key(i));

          CHECK_EQUAL(stlret.first == compare.end(), etlret.first == data.end());
          
          if ((stlret.first != compare.end()) && (etlret.first != data.end()))
          {
            CHECK_EQUAL(*stlret.first, *etlret.first);
          }
          
          CHECK_EQUAL(stlret.second == compare.end(), etlret.second == data.end());
          
          if ((stlret.second != compare.end()) && (etlret.second != data.end()))
          {
            CHECK_EQUAL(*stlret.second, *etlret.second);
          }
        }

        //upper_bound
        CHECK_EQUAL(compare.upper_bound(i) == compare.end(), data.upper_bound(i) == data.end());

        //if both end, or none
        if ((compare.upper_bound(i) == compare.end()) == (data.upper_bound(i) == data.end()))
        {
          //if both are not end
          if (compare.upper_bound(i) != compare.end())
          {
            CHECK_EQUAL(*compare.upper_bound(i), *data.upper_bound(i));
          }
        }
      }
    }

    //*************************************************************************
#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    TEST_FIXTURE(SetupFixture, test_set_template_deduction)
    {
      etl::set data{ std::string("A"), std::string("B"), std::string("C"), std::string("D"), std::string("E"), std::string("F") };

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same_v<std::string, Type>));

      decltype(data)::const_iterator itr = data.begin();

      CHECK_EQUAL("A", *itr);
      ++itr;
      CHECK_EQUAL("B", *itr);
      ++itr;
      CHECK_EQUAL("C", *itr);
      ++itr;
      CHECK_EQUAL("D", *itr);
      ++itr;
      CHECK_EQUAL("E", *itr);
      ++itr;
      CHECK_EQUAL("F", *itr);
    }
#endif

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST_FIXTURE(SetupFixture, test_make_set)
    {
      auto data = etl::make_set< std::string>(std::string("A"), std::string("B"), std::string("C"), std::string("D"), std::string("E"), std::string("F"));

      auto v = *data.begin();
      using Type = decltype(v);
      CHECK((std::is_same<std::string, Type>::value));

      decltype(data)::const_iterator itr = data.begin();

      CHECK_EQUAL("A", *itr);
      ++itr;
      CHECK_EQUAL("B", *itr);
      ++itr;
      CHECK_EQUAL("C", *itr);
      ++itr;
      CHECK_EQUAL("D", *itr);
      ++itr;
      CHECK_EQUAL("E", *itr);
      ++itr;
      CHECK_EQUAL("F", *itr);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains)
    {
      std::array<int, 6U> initial = { 1, 2, 3, 4, 5, 6 };
      etl::set<int, 6U, etl::less<>> data(initial.begin(), initial.end());

      CHECK(data.contains(1));
      CHECK(!data.contains(99));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_contains_using_transparent_comparator)
    {
      std::array<int, 6U> initial = { 1, 2, 3, 4, 5, 6 };
      etl::set<int, 6U, etl::less<>> data(initial.begin(), initial.end());

      CHECK(data.contains(1));
      CHECK(data.contains(Key(1)));

      CHECK(!data.contains(99));
      CHECK(!data.contains(Key(99)));
    }
  };
}
