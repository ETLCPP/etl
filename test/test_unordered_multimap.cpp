/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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
#include <numeric>
#include <functional>
#include <unordered_map>

#include "data.h"

#include "etl/unordered_multimap.h"

namespace etl
{
  template <>
  struct hash<std::string>
  {
    size_t operator ()(const std::string& e) const
    {
      size_t sum = 0U;
      return std::accumulate(e.begin(), e.end(), sum);
    }
  };
}

namespace
{
  //*************************************************************************
  struct simple_hash
  {
    size_t operator ()(const std::string& text) const
    {
      return std::accumulate(text.begin(), text.end(), 0);
    }
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

  typedef TestDataDC<std::string>  DC;
  typedef TestDataNDC<std::string> NDC;

  typedef ETL_OR_STD::pair<std::string, DC>  ElementDC;
  typedef ETL_OR_STD::pair<std::string, NDC> ElementNDC;

  //***************************************************************************
  struct CustomHashFunction
  {
    CustomHashFunction()
      : id(0)
    {
    }

    CustomHashFunction(int id_)
      : id(id_)
    {
    }

    size_t operator ()(uint32_t e) const
    {
      return size_t(e);
    }

    int id;
  };

  //***************************************************************************
  struct CustomKeyEq
  {
    CustomKeyEq()
      : id(0)
    {
    }

    CustomKeyEq(int id_)
      : id(id_)
    {
    }

    size_t operator ()(uint32_t lhs, uint32_t rhs) const
    {
      return (lhs == rhs);
    }

    int id;
  };

  //*************************************************************************
  // Hasher whose hash behaviour depends on provided data.
  struct parameterized_hash
  {
    size_t modulus;

    parameterized_hash(size_t modulus_ = 2) : modulus(modulus_){}

    size_t operator()(size_t val) const
    {
      return val % modulus;
    }
  };

  //*************************************************************************
  // Equality checker whose behaviour depends on provided data.
  struct parameterized_equal
  {
    size_t modulus;

    // Hasher whose hash behaviour depends on provided data.
    parameterized_equal(size_t modulus_ = 2) : modulus(modulus_){}

    bool operator()(size_t lhs, size_t rhs) const
    {
      return (lhs % modulus) == (rhs % modulus);
    }
  };

  SUITE(test_unordered_multimap)
  {
    static const size_t SIZE = 10;

    typedef TestDataDC<std::string>  DC;
    typedef TestDataNDC<std::string> NDC;

    typedef ETL_OR_STD::pair<std::string, DC>  ElementDC;
    typedef ETL_OR_STD::pair<std::string, NDC> ElementNDC;

    typedef etl::unordered_multimap<std::string, DC,  SIZE, SIZE / 2, simple_hash> DataDC;
    typedef etl::unordered_multimap<std::string, NDC, SIZE, SIZE / 2, simple_hash> DataNDC;
    typedef etl::iunordered_multimap<std::string, NDC, simple_hash> IDataNDC;

    using ItemM = TestDataM<int>;
    using DataM = etl::unordered_multimap<std::string, ItemM, SIZE, SIZE, std::hash<std::string>>;

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

    const char* K0  = "FF"; // 0
    const char* K1  = "FG"; // 1
    const char* K2  = "FH"; // 2
    const char* K3  = "FI"; // 3
    const char* K4  = "FJ"; // 4
    const char* K5  = "FK"; // 5
    const char* K6  = "FL"; // 6
    const char* K7  = "FM"; // 7
    const char* K8  = "FN"; // 8
    const char* K9  = "FO"; // 9
    const char* K10 = "FP"; // 0
    const char* K11 = "FQ"; // 1
    const char* K12 = "FR"; // 2
    const char* K13 = "FS"; // 3
    const char* K14 = "FT"; // 4
    const char* K15 = "FU"; // 5
    const char* K16 = "FV"; // 6
    const char* K17 = "FW"; // 7
    const char* K18 = "FX"; // 8
    const char* K19 = "FY"; // 9

    std::string K[] = { K0, K1, K2, K3, K4, K5, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19 };

    std::vector<ElementNDC> initial_data;
    std::vector<ElementNDC> excess_data;
    std::vector<ElementNDC> different_data;
    std::vector<ElementNDC> equal_data;

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
        ElementNDC n[] =
        {
          ElementNDC(K0, N0), ElementNDC(K1, N1), ElementNDC(K2, N2), ElementNDC(K3, N3), ElementNDC(K4, N4),
          ElementNDC(K5, N5), ElementNDC(K6, N6), ElementNDC(K7, N7), ElementNDC(K8, N8), ElementNDC(K9, N9)
        };

        ElementNDC n2[] =
        {
          ElementNDC(K0, N0), ElementNDC(K1, N1), ElementNDC(K2, N2), ElementNDC(K3, N3), ElementNDC(K4, N4),
          ElementNDC(K5, N5), ElementNDC(K6, N6), ElementNDC(K7, N7), ElementNDC(K8, N8), ElementNDC(K9, N9),
          ElementNDC(K10, N10)
        };

        ElementNDC n3[] =
        {
          ElementNDC(K10, N10), ElementNDC(K11, N11), ElementNDC(K12, N12), ElementNDC(K13, N13), ElementNDC(K14, N14),
          ElementNDC(K15, N15), ElementNDC(K16, N16), ElementNDC(K17, N17), ElementNDC(K18, N18), ElementNDC(K19, N19)
        };

        ElementNDC n4[] =
        {
          ElementNDC(K10, N10), ElementNDC(K11, N11), ElementNDC(K11, N12), ElementNDC(K11, N13), ElementNDC(K12, N14)
        };

        initial_data.assign(std::begin(n), std::end(n));
        excess_data.assign(std::begin(n2), std::end(n2));
        different_data.assign(std::begin(n3), std::end(n3));
        equal_data.assign(std::begin(n4), std::end(n4));
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      DataDC data;

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.max_size(), SIZE);
      CHECK(data.begin() == data.end());
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_cpp17_deduced_constructor)
    {
      etl::unordered_multimap data{ ElementNDC(K0, N0), ElementNDC(K1, N1), ElementNDC(K2, N2), ElementNDC(K3, N3), ElementNDC(K4, N4),
                                    ElementNDC(K5, N5), ElementNDC(K6, N6), ElementNDC(K7, N7), ElementNDC(K8, N8), ElementNDC(K9, N9) };
      etl::unordered_multimap<std::string, NDC, 10U, 10U> check = { ElementNDC(K0, N0), ElementNDC(K1, N1), ElementNDC(K2, N2), ElementNDC(K3, N3), ElementNDC(K4, N4),
                                                                    ElementNDC(K5, N5), ElementNDC(K6, N6), ElementNDC(K7, N7), ElementNDC(K8, N8), ElementNDC(K9, N9) };

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
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
      CHECK(data.full());
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

      CHECK_EQUAL(1, data2.find("1")->second.value);
      CHECK_EQUAL(2, data2.find("2")->second.value);
      CHECK_EQUAL(3, data2.find("3")->second.value);
      CHECK_EQUAL(4, data2.find("4")->second.value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_iunordered_multimap)
    {
      int current_count = NDC::get_instance_count();

      DataNDC* pdata = new DataNDC(initial_data.begin(), initial_data.end());

      IDataNDC* pidata = pdata;
      delete pidata;

      CHECK_EQUAL(current_count, NDC::get_instance_count());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC other_data;

      other_data = data;

      CHECK(data == other_data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_interface)
    {
      DataNDC data1(initial_data.begin(), initial_data.end());
      DataNDC data2;

      IDataNDC& idata1 = data1;
      IDataNDC& idata2 = data2;

      idata2 = idata1;

      CHECK(data1 == data2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC other_data(data);

#include "etl/private/diagnostic_self_assign_overloaded_push.h"
      other_data = other_data;
#include "etl/private/diagnostic_pop.h"

      CHECK(data == other_data);
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

      data2 = std::move(data1);

      CHECK_EQUAL(1, data2.find("1")->second.value);
      CHECK_EQUAL(2, data2.find("2")->second.value);
      CHECK_EQUAL(3, data2.find("3")->second.value);
      CHECK_EQUAL(4, data2.find("4")->second.value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty_full)
    {
      DataNDC data;

      CHECK(!data.full());
      CHECK(data.empty());

      data.insert(initial_data.begin(), initial_data.end());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      DataNDC data;

      data.assign(initial_data.begin(), initial_data.end());

      DataNDC::iterator idata;

      for (size_t i = 0UL; i < 10; ++i)
      {
        idata = data.find(K[i]);
        CHECK(idata != data.end());
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value)
    {
      DataNDC data;

      data.insert(DataNDC::value_type(K0,  N0)); // Inserted
      data.insert(DataNDC::value_type(K2,  N2)); // Inserted
      data.insert(DataNDC::value_type(K1,  N1)); // Inserted
      data.insert(DataNDC::value_type(K11, N1)); // Duplicate hash. Inserted
      data.insert(DataNDC::value_type(K1,  N3)); // Duplicate key.  Inserted

      CHECK_EQUAL(5U, data.size());

      DataNDC::iterator idata;

      idata = data.find(K0);

      std::string k = idata->first;
      NDC n = idata->second;

      CHECK(idata != data.end());
      CHECK(idata->first  == K0);
      CHECK(idata->second == N0);

      idata = data.find(K1);
      CHECK(idata != data.end());
      CHECK(idata->first  == K1);
      CHECK(idata->second == N3);

      // The other value with key == K1
      ++idata;
      CHECK(idata != data.end());
      CHECK(idata->first == K1);
      CHECK(idata->second == N1);

      idata = data.find(K2);
      CHECK(idata != data.end());
      CHECK(idata->first  == K2);
      CHECK(idata->second == N2);

      idata = data.find(K11);
      CHECK(idata != data.end());
      CHECK(idata->first  == K11);
      CHECK(idata->second == N1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.insert(ETL_OR_STD::make_pair(K10, N10)), etl::unordered_multimap_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      DataNDC data;

      data.insert(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        DataNDC::iterator idata = data.find(initial_data[i].first);
        CHECK(idata != data.end());
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range_excess)
    {
      DataNDC data;

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::unordered_multimap_full);
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

      CHECK_EQUAL(1, data.find("1")->second.value);
      CHECK_EQUAL(2, data.find("2")->second.value);
      CHECK_EQUAL(3, data.find("3")->second.value);
      CHECK_EQUAL(4, data.find("4")->second.value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key)
    {
      DataNDC data(equal_data.begin(), equal_data.end());

      size_t count = data.erase(K10);

      CHECK_EQUAL(1U, count);

      DataNDC::iterator idata = data.find(K10);
      CHECK(idata == data.end());

      count = data.erase(K11);

      CHECK_EQUAL(3U, count);

      idata = data.find(K11);
      CHECK(idata == data.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_iterator)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator idata = data.find(K5);
      DataNDC::iterator inext = idata;
      ++inext;

      DataNDC::iterator iafter = data.erase(idata);
      idata = data.find(K5);

      CHECK(idata == data.end());
      CHECK(inext == iafter);

      // Test that erase really does erase from the pool.
      CHECK(!data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_const_iterator)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator idata = data.find(K5);
      DataNDC::const_iterator inext = idata;
      ++inext;

      DataNDC::iterator iafter = data.erase(idata);
      idata = data.find(K5);

      CHECK(idata == data.cend());
      CHECK(inext == iafter);

      // Test that erase really does erase from the pool.
      CHECK(!data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator idata = data.cbegin();
      std::advance(idata, 2);

      DataNDC::const_iterator idata_end = data.cbegin();
      std::advance(idata_end, 5);

      data.erase(idata, idata_end);

      CHECK_EQUAL(initial_data.size() - 3, data.size());
      CHECK(!data.full());
      CHECK(!data.empty());

      idata = data.find(K8);
      CHECK(idata != data.end());

      idata = data.find(K0);
      CHECK(idata != data.end());

      idata = data.find(K1);
      CHECK(idata == data.end());

      idata = data.find(K2);
      CHECK(idata == data.end());

      idata = data.find(K3);
      CHECK(idata != data.end());

      idata = data.find(K4);
      CHECK(idata != data.end());

      idata = data.find(K5);
      CHECK(idata != data.end());

      idata = data.find(K6);
      CHECK(idata == data.end());

      idata = data.find(K7);
      CHECK(idata != data.end());

      idata = data.find(K8);
      CHECK(idata != data.end());

      idata = data.find(K9);
      CHECK(idata != data.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range_first_half)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator end = data.cbegin();
      etl::advance(end, data.size() / 2);

      auto itr = data.erase(data.cbegin(), end);

      CHECK_EQUAL(initial_data.size() / 2, data.size());
      CHECK(!data.full());
      CHECK(!data.empty());
      CHECK(itr == end);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range_last_half)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator begin = data.cbegin();
      etl::advance(begin, data.size() / 2);

      auto itr = data.erase(begin, data.cend());

      CHECK_EQUAL(initial_data.size() / 2, data.size());
      CHECK(!data.full());
      CHECK(!data.empty());
      CHECK(itr == data.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range_all)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      auto itr = data.erase(data.cbegin(), data.cend());

      CHECK_EQUAL(0U, data.size());
      CHECK(!data.full());
      CHECK(data.empty());
      CHECK(itr == data.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_clear)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      data.clear();

      CHECK_EQUAL(data.size(), size_t(0));
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count_key)
    {
      DataNDC data(equal_data.begin(), equal_data.end());

      size_t count = data.count(K10);
      CHECK_EQUAL(1U, count);

      count = data.count(K11);
      CHECK_EQUAL(3U, count);

      count = data.count(K1);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_find_const)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator idata = data.find(K3);

      CHECK(idata != data.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      DataNDC data(equal_data.begin(), equal_data.end());

      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> result;

      result = data.equal_range(K10);
      CHECK(result.first  == data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K10);

      result = data.equal_range(K11);
      CHECK(result.first  != data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 3);
      CHECK_EQUAL(result.first->first, K11);

      result = data.equal_range(K12);
      CHECK(result.first  != data.begin());
      CHECK(result.second == data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K12);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_const)
    {
      const DataNDC data(equal_data.begin(), equal_data.end());

      ETL_OR_STD::pair<DataNDC::const_iterator, DataNDC::const_iterator> result;

      result = data.equal_range(K10);
      CHECK(result.first == data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K10);

      result = data.equal_range(K11);
      CHECK(result.first != data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 3);
      CHECK_EQUAL(result.first->first, K11);

      result = data.equal_range(K12);
      CHECK(result.first != data.begin());
      CHECK(result.second == data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K12);
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
    TEST_FIXTURE(SetupFixture, test_hash_function)
    {
      DataNDC data;
      DataNDC::hasher hash_function =  data.hash_function();

      CHECK_EQUAL(simple_hash()(std::string("ABCDEF")), hash_function(std::string("ABCDEF")));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_key_eq_function)
    {
      DataNDC data;
      DataNDC::key_equal key_eq = data.key_eq();

      CHECK(key_eq(std::string("ABCDEF"), std::string("ABCDEF")));
      CHECK(!key_eq(std::string("ABCDEF"), std::string("ABCDEG")));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_load_factor)
    {
      // Empty.
      DataNDC data;
      CHECK_CLOSE(0.0, data.load_factor(), 0.01);

      // Half the buckets used.
      data.assign(initial_data.begin(), initial_data.begin() + (initial_data.size() / 4));
      CHECK_CLOSE(0.4, data.load_factor(), 0.01);

      // All of the buckets used.
      data.clear();
      data.assign(initial_data.begin(), initial_data.end());
      CHECK_CLOSE(2.0, data.load_factor(), 0.01);
    }

    //*************************************************************************
    TEST(test_equality_comparison_fails_when_hash_collisions_occur_582)
    {
      struct bad_hash
      {
        // Force hash collisions
        size_t operator()(int key) const
        {
          return key % 4;
        }
      };

      using etl_map = etl::unordered_multimap<int, std::string, 20, 20, bad_hash>;
      using stl_map = std::unordered_multimap<int, std::string, bad_hash>;

      std::vector<etl_map::value_type> random_keys1 =
      {
        {17, "17"}, {14, "14"}, { 3,  "3"}, { 7,  "7"}, { 2,  "2"},
        { 6,  "6"}, { 9,  "9"}, { 3,  "3"}, {18, "18"}, {10, "10"},
        { 8,  "8"}, {11, "11"}, { 4,  "4"}, { 1,  "1"}, {12, "12"},
        {15, "15"}, {16, "16"}, { 0,  "0"}, { 5,  "5"}, {19, "19"}
      };

      std::vector<etl_map::value_type> random_keys2 =
      {
        { 3,  "3"}, { 6,  "6"}, { 5,  "5"}, {17, "17"}, { 2,  "2"},
        { 7,  "7"}, { 3,  "3"}, {19, "19"}, { 8,  "8"}, {15, "15"},
        {14, "14"}, { 0,  "0"}, {18, "18"}, { 4,  "4"}, {10, "10"},
        { 9,  "9"}, {16, "16"}, {11, "11"}, {12, "12"}, { 1,  "1"}
      };

      // Check that the input data is valid.
      CHECK_EQUAL(random_keys1.size(), random_keys2.size());
      CHECK(std::is_permutation(random_keys1.begin(), random_keys1.end(), random_keys2.begin()));

      //***************************************************
      // Fill ETL
      etl_map etlmap1;
      etl_map etlmap2;

      for (auto i : random_keys1)
      {
        etlmap1.insert(i);
      }

      for (auto i : random_keys2)
      {
        etlmap2.insert(i);
      }

      //***************************************************
      // Fill STD
      stl_map stdmap1;
      stl_map stdmap2;

      for (auto i : random_keys1)
      {
        stdmap1.insert(i);
      }

      for (auto i : random_keys2)
      {
        stdmap2.insert(i);
      }

      //***************************************************
      CHECK_EQUAL((stdmap1 == stdmap2), (etlmap1 == etlmap2));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_and_erase_bug)
    {
      etl::unordered_multimap<uint32_t, char, 5> map;

      map.insert(ETL_OR_STD::make_pair(1, 'b'));
      map.insert(ETL_OR_STD::make_pair(2, 'c'));
      map.insert(ETL_OR_STD::make_pair(3, 'd'));
      map.insert(ETL_OR_STD::make_pair(4, 'e'));

      auto it = map.find(1);
      map.erase(it);

      it = map.find(4);
      map.erase(it);

      std::vector<std::string> s;

      for (const auto &kv : map)
      {
        std::stringstream ss;
        ss << "map[" << kv.first << "] = " << kv.second;
        s.push_back(ss.str());
      }

      CHECK_EQUAL(2, s.size());
      CHECK_EQUAL("map[2] = c", s[0]);
      CHECK_EQUAL("map[3] = d", s[1]);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_copy_construct)
    {
      CustomHashFunction chf(1);
      CustomKeyEq        ceq(2);

      etl::unordered_multimap<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map1(chf, ceq);
      etl::unordered_multimap<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map2(map1);

      CHECK_EQUAL(chf.id, map2.hash_function().id);
      CHECK_EQUAL(ceq.id, map2.key_eq().id);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_assignment)
    {
      CustomHashFunction chf1(1);
      CustomKeyEq        ceq2(2);

      CustomHashFunction chf3(3);
      CustomKeyEq        ceq4(4);

      etl::unordered_multimap<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map1(chf1, ceq2);
      etl::unordered_multimap<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map2(chf3, ceq4);

      map2.operator=(map1);

      CHECK_EQUAL(chf1.id, map2.hash_function().id);
      CHECK_EQUAL(ceq2.id, map2.key_eq().id);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_construction_from_iterators)
    {
      CustomHashFunction chf1(1);
      CustomKeyEq        ceq2(2);

      using value_type = etl::unordered_multimap<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq>::value_type;
      std::array<value_type, 5> data =
      {
        value_type{1, 11},
        value_type{2, 22},
        value_type{3, 33},
        value_type{4, 44},
        value_type{5, 55}
      };

      etl::unordered_multimap<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map1(data.begin(), data.end(), chf1, ceq2);

      CHECK_EQUAL(chf1.id, map1.hash_function().id);
      CHECK_EQUAL(ceq2.id, map1.key_eq().id);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_construction_from_initializer_list)
    {
      CustomHashFunction chf1(1);
      CustomKeyEq        ceq2(2);

      using value_type = etl::unordered_multimap<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq>::value_type;

      etl::unordered_multimap<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map1({ value_type{1, 11}, value_type{2, 22}, value_type{3, 33}, value_type{4, 44}, value_type{5, 55} }, chf1, ceq2);

      CHECK_EQUAL(chf1.id, map1.hash_function().id);
      CHECK_EQUAL(ceq2.id, map1.key_eq().id);
    }

    //*************************************************************************
    TEST(test_iterator_value_types_bug_584)
    {
      using Map = etl::unordered_multimap<int, int, 1, 1>;
      CHECK((!std::is_same<typename Map::const_iterator::value_type, typename Map::iterator::value_type>::value));
    }

    TEST(test_parameterized_eq)
    {
      constexpr std::size_t MODULO = 4;
      parameterized_hash hash{MODULO};
      parameterized_equal eq{MODULO};
      // values are equal modulo 4
      etl::unordered_multimap<std::size_t, int, 10, 10, parameterized_hash, parameterized_equal> map;
      map.insert(etl::make_pair(2, 20));
      map.insert(etl::make_pair(6, 21));
      map.insert(etl::make_pair(10, 22));

      const auto& constmap = map;

      CHECK_EQUAL(constmap.count(6), 3);
      {
        auto range = map.equal_range(6);
        CHECK_EQUAL(std::distance(range.first, range.second), 3);
      }
      {
        auto range = constmap.equal_range(6);
        CHECK_EQUAL(std::distance(range.first, range.second), 3);
      }
    }
  };
}
