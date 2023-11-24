/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#include <iostream>

#include "data.h"

#include "etl/reference_flat_map.h"

namespace
{
  static const size_t SIZE = 10UL;

  typedef TestDataDC<std::string>  DC;
  typedef TestDataNDC<std::string> NDC;

  typedef ETL_OR_STD::pair<const int, DC>  ElementDC;
  typedef ETL_OR_STD::pair<const int, NDC> ElementNDC;

  typedef etl::reference_flat_map<int, DC, SIZE>  DataDC;
  typedef etl::reference_flat_map<int, NDC, SIZE> DataNDC;
  typedef etl::ireference_flat_map<int, DC>       IDataDC;
  typedef etl::ireference_flat_map<int, NDC>      IDataNDC;

  typedef std::map<int, DC>  Compare_DataDC;
  typedef std::map<int, NDC> Compare_DataNDC;

  struct Key
  {
    Key(int k_)
      : k(k_)
    {
    }

    int k;
  };

  bool operator <(const Key& lhs, const int rhs)
  {
    return (lhs.k < rhs);
  }

  bool operator <(const int lhs, const Key& rhs)
  {
    return (lhs < rhs.k);
  }

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

  SUITE(test_reference_flat_map)
  {
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

    DC M0 = DC("A");
    DC M1 = DC("B");
    DC M2 = DC("C");
    DC M3 = DC("D");
    DC M4 = DC("E");
    DC M5 = DC("F");
    DC M6 = DC("G");
    DC M7 = DC("H");
    DC M8 = DC("I");
    DC M9 = DC("J");
    DC M10 = DC("K");
    DC M11 = DC("L");
    DC M12 = DC("M");
    DC M13 = DC("N");
    DC M14 = DC("O");
    DC M15 = DC("P");
    DC M16 = DC("Q");
    DC M17 = DC("R");
    DC M18 = DC("S");
    DC M19 = DC("T");

    std::array<ElementNDC, 10> initial_data =
    {
      ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(3, N3), ElementNDC(4, N4),
      ElementNDC(5, N5), ElementNDC(6, N6), ElementNDC(7, N7), ElementNDC(8, N8), ElementNDC(9, N9)
    };

    std::array<ElementNDC, 11> excess_data =
    {
      ElementNDC(0, N0), ElementNDC(1, N1), ElementNDC(2, N2), ElementNDC(3, N3), ElementNDC(4, N4),
      ElementNDC(5, N5), ElementNDC(6, N6), ElementNDC(7, N7), ElementNDC(8, N8), ElementNDC(9, N9),
      ElementNDC(10, N10)
    };

    std::array<ElementNDC, 10> different_data =
    {
      ElementNDC(10, N10), ElementNDC(11, N11), ElementNDC(12, N12), ElementNDC(13, N13), ElementNDC(14, N14),
      ElementNDC(15, N15), ElementNDC(16, N16), ElementNDC(17, N17), ElementNDC(18, N18), ElementNDC(19, N19)
    };

    std::array<ElementDC, 10> initial_data_dc =
    {
      ElementDC(0, M0), ElementDC(1, M1), ElementDC(2, M2), ElementDC(3, M3), ElementDC(4, M4),
      ElementDC(5, M5), ElementDC(6, M6), ElementDC(7, M7), ElementDC(8, M8), ElementDC(9, M9)
    };

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
      SetupFixture()
      {
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
    TEST(test_destruct_via_ireference_flat_map)
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
      DataDC data;

      CHECK(!data.full());
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at(0), compare_data.at(0));
      CHECK_EQUAL(data.at(1), compare_data.at(1));
      CHECK_EQUAL(data.at(2), compare_data.at(2));
      CHECK_EQUAL(data.at(3), compare_data.at(3));
      CHECK_EQUAL(data.at(4), compare_data.at(4));
      CHECK_EQUAL(data.at(5), compare_data.at(5));
      CHECK_EQUAL(data.at(6), compare_data.at(6));
      CHECK_EQUAL(data.at(7), compare_data.at(7));
      CHECK_EQUAL(data.at(8), compare_data.at(8));
      CHECK_EQUAL(data.at(9), compare_data.at(9));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_using_transparent_comparator)
    {
      std::map<int, NDC> compare_data(initial_data.begin(), initial_data.end());
      etl::reference_flat_map<int, NDC, SIZE, etl::less<>> data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at(Key(0)), compare_data.at(0));
      CHECK_EQUAL(data.at(Key(1)), compare_data.at(1));
      CHECK_EQUAL(data.at(Key(2)), compare_data.at(2));
      CHECK_EQUAL(data.at(Key(3)), compare_data.at(3));
      CHECK_EQUAL(data.at(Key(4)), compare_data.at(4));
      CHECK_EQUAL(data.at(Key(5)), compare_data.at(5));
      CHECK_EQUAL(data.at(Key(6)), compare_data.at(6));
      CHECK_EQUAL(data.at(Key(7)), compare_data.at(7));
      CHECK_EQUAL(data.at(Key(8)), compare_data.at(8));
      CHECK_EQUAL(data.at(Key(9)), compare_data.at(9));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_out_of_bounds)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.at(10), etl::flat_map_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      const DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at(0), compare_data.at(0));
      CHECK_EQUAL(data.at(1), compare_data.at(1));
      CHECK_EQUAL(data.at(2), compare_data.at(2));
      CHECK_EQUAL(data.at(3), compare_data.at(3));
      CHECK_EQUAL(data.at(4), compare_data.at(4));
      CHECK_EQUAL(data.at(5), compare_data.at(5));
      CHECK_EQUAL(data.at(6), compare_data.at(6));
      CHECK_EQUAL(data.at(7), compare_data.at(7));
      CHECK_EQUAL(data.at(8), compare_data.at(8));
      CHECK_EQUAL(data.at(9), compare_data.at(9));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const_using_transparent_comparator)
    {
      const std::map<int, NDC> compare_data(initial_data.begin(), initial_data.end());
      const etl::reference_flat_map<int, NDC, SIZE, etl::less<>> data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at(Key(0)), compare_data.at(0));
      CHECK_EQUAL(data.at(Key(1)), compare_data.at(1));
      CHECK_EQUAL(data.at(Key(2)), compare_data.at(2));
      CHECK_EQUAL(data.at(Key(3)), compare_data.at(3));
      CHECK_EQUAL(data.at(Key(4)), compare_data.at(4));
      CHECK_EQUAL(data.at(Key(5)), compare_data.at(5));
      CHECK_EQUAL(data.at(Key(6)), compare_data.at(6));
      CHECK_EQUAL(data.at(Key(7)), compare_data.at(7));
      CHECK_EQUAL(data.at(Key(8)), compare_data.at(8));
      CHECK_EQUAL(data.at(Key(9)), compare_data.at(9));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const_out_of_bounds)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.at(10), etl::flat_map_out_of_bounds);
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data;

      data.assign(initial_data.begin(), initial_data.end());

      bool isEqual = Check_Equal(data.begin(),
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

      ETL_OR_STD::pair<DataNDC::iterator, bool> result;

      DataNDC::value_type item(0, N0);
      result = data.insert(item);
      compare_data.insert(item);

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
      CHECK(result.second);
      CHECK(*result.first == item);

      DataNDC::value_type item2(2, N2);
      result = data.insert(item2);
      compare_data.insert(item2);

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);
      CHECK(result.second);
      CHECK(*result.first == item2);

      DataNDC::value_type item1(1, N1);
      result = data.insert(item1);
      compare_data.insert(item1);

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);
      CHECK(result.second);
      CHECK(*result.first == item1);

      CHECK(std::is_sorted(data.begin(), data.end()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_changed)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      ETL_OR_STD::pair<DataNDC::iterator, bool> result1;
      ETL_OR_STD::pair<Compare_DataNDC::iterator, bool> result2;

      DataNDC::value_type item(0, N0);
      result1 = data.insert(item);
      result2 = compare_data.insert(item);

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
      CHECK(result1.second);
      CHECK(*result1.first == item);

      DataNDC::value_type item2(0, N2);
      result1 = data.insert(item2);
      result2 = compare_data.insert(item2);

      isEqual = Check_Equal(data.begin(),
                            data.end(),
                            compare_data.begin());

      CHECK(isEqual);
      CHECK(!result1.second);
      CHECK(*result1.first != item2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_multiple)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      DataNDC::value_type item0(0, N0);
      data.insert(item0);
      compare_data.insert(item0);

      DataNDC::value_type item1(1, N1);
      data.insert(item1);
      compare_data.insert(item1);

      DataNDC::value_type item2(2, N2);
      data.insert(item2);
      compare_data.insert(item2);

      // Do it again.
      data.insert(item0);
      compare_data.insert(item0);

      data.insert(item1);
      compare_data.insert(item1);

      data.insert(item2);
      compare_data.insert(item2);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::value_type item10(10, N10);
      CHECK_THROW(data.insert(item10), etl::flat_map_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      Compare_DataNDC compare_data;
      DataNDC data;

      data.insert(initial_data.begin(), initial_data.end());
      compare_data.insert(initial_data.begin(), initial_data.end());

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range_excess)
    {
      DataNDC data;

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::flat_map_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      size_t count_compare = compare_data.erase(5);
      size_t count         = data.erase(5);

      CHECK_EQUAL(count_compare, count);

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key_using_transparent_comparator)
    {
      using CMap = std::map<int, NDC>;
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      size_t count_compare = compare_data.erase(5);
      size_t count = data.erase(Key(5));

      CHECK_EQUAL(count_compare, count);

      bool isEqual = Check_Equal(data.begin(),
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
      DataNDC::iterator i_data_begin = data.begin();

      Compare_DataNDC::iterator i_compare = i_compare_begin;
      DataNDC::iterator i_data = i_data_begin;

      Compare_DataNDC::iterator i_compare_expected = i_compare_begin;
      DataNDC::iterator i_data_expected = i_data_begin;

      std::advance(i_compare, 2);
      std::advance(i_data, 2);

      std::advance(i_compare_expected, 3);
      std::advance(i_data_expected, 3);

      ElementNDC compare_expected = *i_compare_expected;
      ElementNDC data_expected = *i_data_expected;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare);
      DataNDC::iterator i_data_result = data.erase(i_data);

      CHECK(compare_expected == *i_compare_result);
      CHECK(data_expected == *i_data_result);

      bool isEqual = Check_Equal(data.begin(),
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
      DataNDC::iterator i_data_begin            = data.begin();

      Compare_DataNDC::const_iterator i_compare = i_compare_begin;
      DataNDC::const_iterator i_data            = i_data_begin;

      Compare_DataNDC::iterator i_compare_expected = i_compare_begin;
      DataNDC::iterator i_data_expected            = i_data_begin;

      std::advance(i_compare, 2);
      std::advance(i_data,    2);
      
      std::advance(i_compare_expected, 3);
      std::advance(i_data_expected, 3);

      ElementNDC compare_expected = *i_compare_expected;
      ElementNDC data_expected    = *i_data_expected;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare);
      DataNDC::iterator i_data_result            = data.erase(i_data);

      CHECK(compare_expected == *i_compare_result);
      CHECK(data_expected == *i_data_result);

      bool isEqual = Check_Equal(data.begin(),
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
      DataNDC::const_iterator i_data            = data.begin();

      Compare_DataNDC::const_iterator i_compare_end = compare_data.begin();
      DataNDC::const_iterator i_data_end            = data.begin();

      std::advance(i_compare, 2);
      std::advance(i_data,    2);

      std::advance(i_compare_end, 4);
      std::advance(i_data_end,    4);

      ElementNDC compare_expected = *i_compare_end;
      ElementNDC data_expected    = *i_data_end;

      Compare_DataNDC::iterator i_compare_result = compare_data.erase(i_compare, i_compare_end);
      DataNDC::iterator i_data_result            = data.erase(i_data, i_data_end);

      CHECK(compare_expected == *i_compare_result);
      CHECK(data_expected    == *i_data_result);

      bool isEqual = Check_Equal(data.begin(),
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

      bool isEqual = Check_Equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());

      bool isEqual = Check_Equal(data.cbegin(),
                                 data.cend(),
                                 compare_data.cbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());

      bool isEqual = Check_Equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());

      DataNDC data(initial_data.begin(), initial_data.end());

      bool isEqual = Check_Equal(data.crbegin(),
                                 data.crend(),
                                 compare_data.crbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator it = data.find(3);
      CHECK_EQUAL(N3, it->second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_using_transparent_comparator)
    {
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      EMap::iterator it = data.find(Key(3));
      CHECK_EQUAL(N3, it->second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_not_present)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator it = data.find(-1);
      CHECK(data.end() == it);

      it = data.find(10);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_not_present_using_transparent_comparator)
    {
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;
      EMap data(initial_data.begin(), initial_data.end());

      EMap::iterator it = data.find(Key(-1));
      CHECK(data.end() == it);

      it = data.find(10);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator it = data.find(3);
      CHECK_EQUAL(N3, it->second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_using_transparent_comparator)
    {
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;
      const EMap data(initial_data.begin(), initial_data.end());

      EMap::const_iterator it = data.find(Key(3));
      CHECK_EQUAL(N3, it->second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_not_present)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator it = data.find(-1);
      CHECK(data.end() == it);

      it = data.find(10);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const_not_present_using_transparent_comparator)
    {
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;
      const EMap data(initial_data.begin(), initial_data.end());

      EMap::const_iterator it = data.find(Key(-1));
      CHECK(data.end() == it);

      it = data.find(10);
      CHECK(data.end() == it);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare = compare_data.lower_bound(5);
      DataNDC::iterator         i_data    = data.lower_bound(5);

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare), std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lower_bound_using_transparent_comparator)
    {
      using CMap = std::map<int, NDC>;
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.lower_bound(5);
      EMap::iterator i_data    = data.lower_bound(Key(5));

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare), std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      Compare_DataNDC::iterator i_compare = compare_data.upper_bound(5);
      DataNDC::iterator         i_data    = data.upper_bound(5);

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare), std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_upper_bound_using_transparent_comparator)
    {
      using CMap = std::map<int, NDC>;
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      CMap::iterator i_compare = compare_data.upper_bound(5);
      EMap::iterator i_data    = data.upper_bound(Key(5));

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare), std::distance(data.begin(), i_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<Compare_DataNDC::iterator, Compare_DataNDC::iterator> i_compare = compare_data.equal_range(5);
      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> i_data = data.equal_range(5);

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare.first),  std::distance(data.begin(), i_data.first));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare.second), std::distance(data.begin(), i_data.second));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_using_transparent_comparator)
    {
      using CMap = std::map<int, NDC>;
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<CMap::iterator, CMap::iterator> i_compare = compare_data.equal_range(5);
      ETL_OR_STD::pair<EMap::iterator, EMap::iterator> i_data    = data.equal_range(5);

      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare.first), std::distance(data.begin(), i_data.first));
      CHECK_EQUAL(std::distance(compare_data.begin(), i_compare.second), std::distance(data.begin(), i_data.second));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_not_present)
    {
      Compare_DataNDC compare_data(initial_data.begin(), initial_data.end());
      DataNDC data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<Compare_DataNDC::iterator, Compare_DataNDC::iterator> i_compare;

      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> i_data;

      i_data = data.equal_range(-1);
      CHECK(data.begin() == i_data.first);
      CHECK(data.begin() == i_data.second);

      i_data = data.equal_range(99);
      CHECK(data.end() == i_data.first);
      CHECK(data.end() == i_data.second);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_not_present_using_transparent_comparator)
    {
      using CMap = std::map<int, NDC>;
      using EMap = etl::reference_flat_map<int, NDC, SIZE, etl::less<>>;

      CMap compare_data(initial_data.begin(), initial_data.end());
      EMap data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<CMap::iterator, CMap::iterator> i_compare;

      ETL_OR_STD::pair<EMap::iterator, EMap::iterator> i_data;

      i_data = data.equal_range(-1);
      CHECK(data.begin() == i_data.first);
      CHECK(data.begin() == i_data.second);

      i_data = data.equal_range(99);
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
  };
}
