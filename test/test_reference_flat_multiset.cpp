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

#include <set>
#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>

#include "data.h"

#include "etl/reference_flat_multiset.h"

namespace
{
  static const size_t SIZE = 10UL;

  typedef TestDataDC<std::string>  DC;
  typedef TestDataNDC<std::string> NDC;

  typedef etl::reference_flat_multiset<DC, SIZE>  DataDC;
  typedef etl::reference_flat_multiset<NDC, SIZE> DataNDC;
  typedef etl::ireference_flat_multiset<NDC>      IDataNDC;

  typedef std::multiset<DC>  Compare_DataDC;
  typedef std::multiset<NDC> Compare_DataNDC;

  NDC NX = NDC("@");
  NDC NY = NDC("[");

  NDC N0 = NDC("A");
  NDC N1 = NDC("B");
  NDC N2 = NDC("C");
  NDC N3 = NDC("D");
  NDC N4 = NDC("E");
  NDC N5 = NDC("F");
  NDC N6 = NDC("G");
  NDC N7 = NDC("H");
  NDC N8 = NDC("I");
  NDC N9 = NDC("J");
  NDC N10 = NDC("K");
  NDC N11 = NDC("L");
  NDC N12 = NDC("M");
  NDC N13 = NDC("N");
  NDC N14 = NDC("O");
  NDC N15 = NDC("P");
  NDC N16 = NDC("Q");
  NDC N17 = NDC("R");
  NDC N18 = NDC("S");
  NDC N19 = NDC("T");

  std::vector<NDC> initial_data;
  std::vector<NDC> excess_data;
  std::vector<NDC> different_data;
  std::vector<NDC> multi_data;

  struct Key
  {
    Key(NDC k_)
      : k(k_)
    {
    }

    NDC k;
  };

  bool operator <(const Key& lhs, const NDC& rhs)
  {
    return (lhs.k < rhs);
  }

  bool operator <(const NDC& lhs, const Key& rhs)
  {
    return (lhs < rhs.k);
  }

  SUITE(test_flat_multiset)
  {
    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        NDC n[] =
        {
          N0, N1, N2, N3, N4, N5, N6, N7, N8, N9
        };

        NDC n2[] =
        {
          N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10
        };

        NDC n3[] =
        {
          N10, N11, N12, N13, N14, N15, N16, N17, N18, N19
        };

        NDC n4[] =
        {
          N0, N0, N1, N2, N3, N1, N3, N3, N4, N2
        };

        initial_data.assign(std::begin(n), std::end(n));
        excess_data.assign(std::begin(n2), std::end(n2));
        different_data.assign(std::begin(n3), std::end(n3));
        multi_data.assign(std::begin(n4), std::end(n4));
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataDC data;

      CHECK_EQUAL(data.size(), 0UL);
      CHECK(data.empty());
      CHECK_EQUAL(data.capacity(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST(test_destruct_via_ireference_flat_multiset)
    {
      int current_count = NDC::get_instance_count();

      DataNDC* pdata = new DataNDC(initial_data.begin(), initial_data.end());

      IDataNDC* pidata = pdata;
      delete pidata;

      CHECK_EQUAL(current_count, NDC::get_instance_count());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      const DataNDC constData(initial_data.begin(), initial_data.end());

      CHECK(data.begin() == std::begin(data));
      CHECK(constData.begin() == std::begin(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      const DataNDC constData(initial_data.begin(), initial_data.end());

      CHECK(data.end() == std::end(data));
      CHECK(constData.end() == std::end(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty)
    {
      DataNDC data;
      data.insert(initial_data.begin(), initial_data.end());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full)
    {
      DataNDC data;

      CHECK(!data.full());
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data;

      data.assign(initial_data.begin(), initial_data.end());

      bool isEqual = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      data.insert(N0);
      compare_data.insert(N0);

      bool isEqual = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      data.insert(N2);
      compare_data.insert(N2);

      isEqual = std::equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);

      data.insert(N1);
      compare_data.insert(N1);

      isEqual = std::equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_multiple)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      data.insert(N0);
      compare_data.insert(N0);

      data.insert(N1);
      compare_data.insert(N1);

      data.insert(N2);
      compare_data.insert(N2);

      // Do it again.
      data.insert(N0);
      compare_data.insert(N0);

      data.insert(N1);
      compare_data.insert(N1);

      data.insert(N2);
      compare_data.insert(N2);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.insert(N10), etl::flat_multiset_full);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      data.insert(initial_data.begin(), initial_data.end());
      compare_data.insert(initial_data.begin(), initial_data.end());

      bool isEqual = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range_excess)
    {
      DataNDC data;

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::flat_multiset_full);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      size_t count_compare = compare_data.erase(N5);
      size_t count         = data.erase(N5);

      CHECK_EQUAL(count_compare, count);

      bool isEqual = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key_using_transparent_comparator)
    {
      using CSet = std::set<NDC>;
      CSet compare_data(initial_data.begin(), initial_data.end());

      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      ESet data(initial_data.begin(), initial_data.end());

      size_t count_compare = compare_data.erase(N5);
      size_t count = data.erase(Key(N5));

      CHECK_EQUAL(count_compare, count);

      bool isEqual = std::equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare_begin = compare_data.begin();
      DataNDC::iterator         i_data_begin    = data.begin();

      Compare_DataNDC::iterator i_compare = i_compare_begin;
      DataNDC::iterator         i_data    = i_data_begin;

      Compare_DataNDC::iterator i_compare_expected = i_compare_begin;
      DataNDC::iterator         i_data_expected    = i_data_begin;

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

      std::advance(i_compare_expected, 3);
      std::advance(i_data_expected, 3);

      NDC compare_expected = *i_compare_expected;
      NDC data_expected    = *i_data_expected;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare);
      DataNDC::iterator         i_data_result    = data.erase(i_data);

      CHECK(compare_expected == *i_compare_result);
      CHECK(data_expected    == *i_data_result);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_const_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare_begin = compare_data.begin();
      DataNDC::iterator         i_data_begin    = data.begin();

      Compare_DataNDC::const_iterator i_compare = i_compare_begin;
      DataNDC::const_iterator         i_data    = i_data_begin;

      Compare_DataNDC::iterator i_compare_expected = i_compare_begin;
      DataNDC::iterator         i_data_expected    = i_data_begin;

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

      std::advance(i_compare_expected, 3);
      std::advance(i_data_expected, 3);

      NDC compare_expected = *i_compare_expected;
      NDC data_expected    = *i_data_expected;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare);
      DataNDC::iterator         i_data_result    = data.erase(i_data);

      CHECK(compare_expected == *i_compare_result);
      CHECK(data_expected    == *i_data_result);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::const_iterator i_compare = compare_data.begin();
      DataNDC::const_iterator         i_data    = data.begin();

      Compare_DataNDC::const_iterator i_compare_end = compare_data.begin();
      DataNDC::const_iterator         i_data_end    = data.begin();

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

      std::advance(i_compare_end, 4);
      std::advance(i_data_end, 4);

      NDC compare_expected = *i_compare_end;
      NDC data_expected    = *i_data_end;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare, i_compare_end);
      DataNDC::iterator         i_data_result    = data.erase(i_data, i_data_end);

      CHECK(compare_expected == *i_compare_result);
      CHECK(data_expected    == *i_data_result);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());
      data.clear();

      CHECK_EQUAL(data.size(), 0UL);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());

      bool isEqual = std::equal(data.cbegin(),
                                data.cend(),
                                compare_data.cbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());

      bool isEqual = std::equal(data.rbegin(),
                                data.rend(),
                                compare_data.rbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());

      bool isEqual = std::equal(data.crbegin(),
                                data.crend(),
                                compare_data.crbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator it = data.find(N3);
      CHECK_EQUAL(N3, *it);

      it = data.find(N19);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_using_transparent_comparator)
    {
      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      ESet data(initial_data.begin(), initial_data.end());

      ESet::iterator it = data.find(Key(N3));
      CHECK_EQUAL(N3, *it);

      it = data.find(N19);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_not_present)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator it = data.find(NX);
      CHECK(data.end() == it);

      it = data.find(NY);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_not_present_using_transparent_comparator)
    {
      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      ESet data(initial_data.begin(), initial_data.end());

      ESet::iterator it = data.find(Key(NX));
      CHECK(data.end() == it);

      it = data.find(NY);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator it = data.find(N3);
      CHECK_EQUAL(N3, *it);

      it = data.find(N19);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_using_transparent_comparator)
    {
      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      const ESet data(initial_data.begin(), initial_data.end());

      ESet::const_iterator it = data.find(N3);
      CHECK_EQUAL(N3, *it);

      it = data.find(N19);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_not_present)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator it = data.find(NX);
      CHECK(data.end() == it);

      it = data.find(NY);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_not_present_using_transparent_comparator)
    {
      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      const ESet data(initial_data.begin(), initial_data.end());

      ESet::const_iterator it = data.find(Key(NX));
      CHECK(data.end() == it);

      it = data.find(NY);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare = compare_data.lower_bound(N5);
      DataNDC::iterator         i_data    = data.lower_bound(N5);

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare), std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound_using_transparent_comparator)
    {
      using CSet = std::set<NDC>;
      CSet compare_data(initial_data.begin(), initial_data.end());

      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      ESet data(initial_data.begin(), initial_data.end());

      CSet::iterator i_compare = compare_data.lower_bound(N5);
      ESet::iterator i_data    = data.lower_bound(Key(N5));

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare), std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare = compare_data.upper_bound(N5);
      DataNDC::iterator         i_data    = data.upper_bound(N5);

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare), std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_using_transparent_comparator)
    {
      using CSet = std::set<NDC>;
      CSet compare_data(initial_data.begin(), initial_data.end());

      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      ESet data(initial_data.begin(), initial_data.end());

      CSet::iterator i_compare = compare_data.upper_bound(N5);
      ESet::iterator i_data    = data.upper_bound(Key(N5));

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare), std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<Compare_DataNDC::iterator, Compare_DataNDC::iterator> i_compare = compare_data.equal_range(N5);
      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> i_data = data.equal_range(N5);

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare.first),  std::distance(data.begin(), i_data.first));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare.second), std::distance(data.begin(), i_data.second));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_using_transparent_comparator)
    {
      using CSet = std::set<NDC>;
      CSet compare_data(initial_data.begin(), initial_data.end());

      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      ESet data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<CSet::iterator, CSet::iterator> i_compare = compare_data.equal_range(N5);
      ETL_OR_STD::pair<ESet::iterator, ESet::iterator> i_data = data.equal_range(Key(N5));

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare.first), std::distance(data.begin(), i_data.first));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare.second), std::distance(data.begin(), i_data.second));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_not_present)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> i_data;

      i_data = data.equal_range(NX);
      CHECK(data.begin() == i_data.first);
      CHECK(data.begin() == i_data.second);

      i_data = data.equal_range(NY);
      CHECK(data.end() == i_data.first);
      CHECK(data.end() == i_data.second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_not_present_using_transparent_comparator)
    {
      using ESet = etl::reference_flat_multiset<NDC, SIZE, etl::less<>>;
      ESet data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<ESet::iterator, ESet::iterator> i_data;

      i_data = data.equal_range(Key(NX));
      CHECK(data.begin() == i_data.first);
      CHECK(data.begin() == i_data.second);

      i_data = data.equal_range(Key(NY));
      CHECK(data.end() == i_data.first);
      CHECK(data.end() == i_data.second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      const DataNDC initial1(initial_data.begin(), initial_data.end());
      const DataNDC initial2(initial_data.begin(), initial_data.end());

      CHECK(initial1 == initial2);

      const DataNDC different(different_data.begin(), different_data.end());

      CHECK(!(initial1 == different));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      const DataNDC initial1(initial_data.begin(), initial_data.end());
      const DataNDC initial2(initial_data.begin(), initial_data.end());

      CHECK(!(initial1 != initial2));

      const DataNDC different(different_data.begin(), different_data.end());

      CHECK(initial1 != different);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_multi)
    {
      Compare_DataNDC compare_data(multi_data.begin(), multi_data.end());
      DataNDC data(multi_data.begin(), multi_data.end());

      ETL_OR_STD::pair<Compare_DataNDC::iterator, Compare_DataNDC::iterator> compare_range;
      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> test_range;

      compare_range = compare_data.equal_range(N0);
      test_range = data.equal_range(N0);
      CHECK_EQUAL(std::distance(compare_range.first, compare_range.second), std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(N1);
      test_range = data.equal_range(N1);
      CHECK_EQUAL(std::distance(compare_range.first, compare_range.second), std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(N2);
      test_range = data.equal_range(N2);
      CHECK_EQUAL(std::distance(compare_range.first, compare_range.second), std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(N3);
      test_range = data.equal_range(N3);
      CHECK_EQUAL(std::distance(compare_range.first, compare_range.second), std::distance(test_range.first, test_range.second));

      compare_range = compare_data.equal_range(N4);
      test_range = data.equal_range(N4);
      CHECK_EQUAL(std::distance(compare_range.first, compare_range.second), std::distance(test_range.first, test_range.second));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count)
    {
      Compare_DataNDC compare_data(multi_data.begin(), multi_data.end());
      DataNDC data(multi_data.begin(), multi_data.end());

      CHECK_EQUAL(compare_data.count(N0), data.count(N0));
      CHECK_EQUAL(compare_data.count(N1), data.count(N1));
      CHECK_EQUAL(compare_data.count(N2), data.count(N2));
      CHECK_EQUAL(compare_data.count(N3), data.count(N3));
      CHECK_EQUAL(compare_data.count(N4), data.count(N4));
    }
  };
}
