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

#include <sstream>

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

#include "etl/unordered_map.h"
#include "etl/hash.h"

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
  // Non-default-constructible hasher
  struct ndc_hash
  {
    int id;
    ndc_hash(int id_) : id(id_){}

    size_t operator()(size_t val) const
    {
      return val;
    }
  };

  //*************************************************************************
  // Non-default-constructible equality checker
  struct ndc_key_eq
  {
    int id;
    ndc_key_eq(int id_) : id(id_){}

    bool operator()(size_t val1, size_t val2) const
    {
      return val1 == val2;
    }
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
  struct transparent_hash
  {
    typedef int is_transparent;

    size_t operator ()(const char* s) const
    {
      size_t sum = 0U;
      size_t length = etl::strlen(s);

      return std::accumulate(s, s + length, sum);
    }

    size_t operator ()(const std::string& text) const
    {
      return std::accumulate(text.begin(), text.end(), 0);
    }
  };

  using DC  = TestDataDC<std::string>;
  using NDC = TestDataNDC<std::string>;

  using ElementDC  = ETL_OR_STD::pair<std::string, DC>;
  using ElementNDC = ETL_OR_STD::pair<std::string, NDC>;
}

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

  SUITE(test_unordered_map)
  {
    static const size_t SIZE = 10;

    using ItemM = TestDataM<int>;
    using DataM = etl::unordered_map<std::string, ItemM, SIZE, SIZE, std::hash<std::string>>;

    using DataDC             = etl::unordered_map<std::string, DC,  SIZE, SIZE / 2, simple_hash>;
    using DataNDC            = etl::unordered_map<std::string, NDC, SIZE, SIZE / 2, simple_hash>;
    using IDataNDC           = etl::iunordered_map<std::string, NDC, simple_hash>;
    using DataNDCTransparent = etl::unordered_map<std::string, NDC, SIZE, SIZE / 2, transparent_hash, etl::equal_to<>>;
    using DataDCTransparent  = etl::unordered_map<std::string, DC,  SIZE, SIZE / 2, transparent_hash, etl::equal_to<>>;

    NDC N0  = NDC("A");
    NDC N1  = NDC("B");
    NDC N2  = NDC("C");
    NDC N3  = NDC("D");
    NDC N4  = NDC("E");
    NDC N5  = NDC("F");
    NDC N6  = NDC("G");
    NDC N7  = NDC("H");
    NDC N8  = NDC("I");
    NDC N9  = NDC("J");
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

    DC M0  = DC("A");
    DC M1  = DC("B");
    DC M2  = DC("C");
    DC M3  = DC("D");
    DC M4  = DC("E");
    DC M5  = DC("F");
    DC M6  = DC("G");
    DC M7  = DC("H");
    DC M8  = DC("I");
    DC M9  = DC("J");
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

    const char* CK0  = "FF"; // 0
    const char* CK1  = "FG"; // 1
    const char* CK2  = "FH"; // 2
    const char* CK3  = "FI"; // 3
    const char* CK4  = "FJ"; // 4
    const char* CK5  = "FK"; // 5
    const char* CK6  = "FL"; // 6
    const char* CK7  = "FM"; // 7
    const char* CK8  = "FN"; // 8
    const char* CK9  = "FO"; // 9
    const char* CK10 = "FP"; // 0
    const char* CK11 = "FQ"; // 1
    const char* CK12 = "FR"; // 2
    const char* CK13 = "FS"; // 3
    const char* CK14 = "FT"; // 4
    const char* CK15 = "FU"; // 5
    const char* CK16 = "FV"; // 6
    const char* CK17 = "FW"; // 7
    const char* CK18 = "FX"; // 8
    const char* CK19 = "FY"; // 9

    std::string K0  = CK0;  // 0
    std::string K1  = CK1;  // 1
    std::string K2  = CK2;  // 2
    std::string K3  = CK3;  // 3
    std::string K4  = CK4;  // 4
    std::string K5  = CK5;  // 5
    std::string K6  = CK6;  // 6
    std::string K7  = CK7;  // 7
    std::string K8  = CK8;  // 8
    std::string K9  = CK9;  // 9
    std::string K10 = CK10; // 0
    std::string K11 = CK11; // 1
    std::string K12 = CK12; // 2
    std::string K13 = CK13; // 3
    std::string K14 = CK14; // 4
    std::string K15 = CK15; // 5
    std::string K16 = CK16; // 6
    std::string K17 = CK17; // 7
    std::string K18 = CK18; // 8
    std::string K19 = CK19; // 9

    std::string K[] = { K0, K1, K2, K3, K4, K5, K6, K7, K8, K9, K10, K11, K12, K13, K14, K15, K16, K17, K18, K19 };

    std::vector<ElementDC> initial_data_dc;

    std::vector<ElementNDC> initial_data;
    std::vector<ElementNDC> excess_data;
    std::vector<ElementNDC> different_data;

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

        ElementDC n4[] =
        {
          ElementDC(K0, M0), ElementDC(K1, M1), ElementDC(K2, M2), ElementDC(K3, M3), ElementDC(K4, M4),
          ElementDC(K5, M5), ElementDC(K6, M6), ElementDC(K7, M7), ElementDC(K8, M8), ElementDC(K9, M9)
        };

        initial_data.assign(std::begin(n), std::end(n));
        excess_data.assign(std::begin(n2), std::end(n2));
        different_data.assign(std::begin(n3), std::end(n3));
        initial_data_dc.assign(std::begin(n4), std::end(n4));
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
      etl::unordered_map data{ ElementNDC(K0, N0), ElementNDC(K1, N1), ElementNDC(K2, N2), ElementNDC(K3, N3), ElementNDC(K4, N4),
                               ElementNDC(K5, N5), ElementNDC(K6, N6), ElementNDC(K7, N7), ElementNDC(K8, N8), ElementNDC(K9, N9) };
      etl::unordered_map<std::string, NDC, 10U, 10U> check = { ElementNDC(K0, N0), ElementNDC(K1, N1), ElementNDC(K2, N2), ElementNDC(K3, N3), ElementNDC(K4, N4),
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
      DataDC data(initial_data_dc.begin(), initial_data_dc.end());

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

      CHECK_EQUAL(1, data2.at("1").value);
      CHECK_EQUAL(2, data2.at("2").value);
      CHECK_EQUAL(3, data2.at("3").value);
      CHECK_EQUAL(4, data2.at("4").value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_iunordered_map)
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

      CHECK(idata2 == idata1);
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

      data2.insert(DataM::value_type(std::string("5"), ItemM(5)));
      data2 = std::move(data1);

      CHECK_EQUAL(1, data2.at("1").value);
      CHECK_EQUAL(2, data2.at("2").value);
      CHECK_EQUAL(3, data2.at("3").value);
      CHECK_EQUAL(4, data2.at("4").value);
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
    TEST_FIXTURE(SetupFixture, test_index_read)
    {
      DataDC data(initial_data_dc.begin(), initial_data_dc.end());

      CHECK_EQUAL(M0, data[K0]);
      CHECK_EQUAL(M1, data[K1]);
      CHECK_EQUAL(M2, data[K2]);
      CHECK_EQUAL(M3, data[K3]);
      CHECK_EQUAL(M4, data[K4]);
      CHECK_EQUAL(M5, data[K5]);
      CHECK_EQUAL(M6, data[K6]);
      CHECK_EQUAL(M7, data[K7]);
      CHECK_EQUAL(M8, data[K8]);
      CHECK_EQUAL(M9, data[K9]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_read_using_transparent_comparator_and_hasher)
    {
      DataDCTransparent data(initial_data_dc.begin(), initial_data_dc.end());

      CHECK_EQUAL(M0, data[CK0]);
      CHECK_EQUAL(M1, data[CK1]);
      CHECK_EQUAL(M2, data[CK2]); 
      CHECK_EQUAL(M3, data[CK3]);
      CHECK_EQUAL(M4, data[CK4]);
      CHECK_EQUAL(M5, data[CK5]);
      CHECK_EQUAL(M6, data[CK6]);
      CHECK_EQUAL(M7, data[CK7]);
      CHECK_EQUAL(M8, data[CK8]);
      CHECK_EQUAL(M9, data[CK9]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_write)
    {
      DataDC data(initial_data_dc.begin(), initial_data_dc.end());

      data[K0] = M9;
      data[K1] = M8;
      data[K2] = M7;
      data[K3] = M6;
      data[K4] = M5;
      data[K5] = M4;
      data[K6] = M3;
      data[K7] = M2;
      data[K8] = M1;
      data[K9] = M0;

      CHECK_EQUAL(M9, data[K0]);
      CHECK_EQUAL(M8, data[K1]);
      CHECK_EQUAL(M7, data[K2]);
      CHECK_EQUAL(M6, data[K3]);
      CHECK_EQUAL(M5, data[K4]);
      CHECK_EQUAL(M4, data[K5]);
      CHECK_EQUAL(M3, data[K6]);
      CHECK_EQUAL(M2, data[K7]);
      CHECK_EQUAL(M1, data[K8]);
      CHECK_EQUAL(M0, data[K9]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_write_using_transparent_comparator_and_hasher)
    {
      DataDCTransparent data(initial_data_dc.begin(), initial_data_dc.end());

      data[CK0] = M9;
      data[CK1] = M8;
      data[CK2] = M7;
      data[CK3] = M6;
      data[CK4] = M5;
      data[CK5] = M4;
      data[CK6] = M3;
      data[CK7] = M2;
      data[CK8] = M1;
      data[CK9] = M0;

      CHECK_EQUAL(M9, data[CK0]);
      CHECK_EQUAL(M8, data[CK1]);
      CHECK_EQUAL(M7, data[CK2]);
      CHECK_EQUAL(M6, data[CK3]);
      CHECK_EQUAL(M5, data[CK4]);
      CHECK_EQUAL(M4, data[CK5]);
      CHECK_EQUAL(M3, data[CK6]);
      CHECK_EQUAL(M2, data[CK7]);
      CHECK_EQUAL(M1, data[CK8]);
      CHECK_EQUAL(M0, data[CK9]);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at(K0), N0);
      CHECK_EQUAL(data.at(K1), N1);
      CHECK_EQUAL(data.at(K2), N2);
      CHECK_EQUAL(data.at(K3), N3);
      CHECK_EQUAL(data.at(K4), N4);
      CHECK_EQUAL(data.at(K5), N5);
      CHECK_EQUAL(data.at(K6), N6);
      CHECK_EQUAL(data.at(K7), N7);
      CHECK_EQUAL(data.at(K8), N8);
      CHECK_EQUAL(data.at(K9), N9);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_using_transparent_comparator_and_hasher)
    {
      DataNDCTransparent data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at(CK0), N0);
      CHECK_EQUAL(data.at(CK1), N1);
      CHECK_EQUAL(data.at(CK2), N2);
      CHECK_EQUAL(data.at(CK3), N3);
      CHECK_EQUAL(data.at(CK4), N4);
      CHECK_EQUAL(data.at(CK5), N5);
      CHECK_EQUAL(data.at(CK6), N6);
      CHECK_EQUAL(data.at(CK7), N7);
      CHECK_EQUAL(data.at(CK8), N8);
      CHECK_EQUAL(data.at(CK9), N9);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at(K0), N0);
      CHECK_EQUAL(data.at(K1), N1);
      CHECK_EQUAL(data.at(K2), N2);
      CHECK_EQUAL(data.at(K3), N3);
      CHECK_EQUAL(data.at(K4), N4);
      CHECK_EQUAL(data.at(K5), N5);
      CHECK_EQUAL(data.at(K6), N6);
      CHECK_EQUAL(data.at(K7), N7);
      CHECK_EQUAL(data.at(K8), N8);
      CHECK_EQUAL(data.at(K9), N9);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const_using_transparent_comparator_and_hasher)
    {
      const DataNDCTransparent data(initial_data.begin(), initial_data.end());

      CHECK_EQUAL(data.at(CK0), N0);
      CHECK_EQUAL(data.at(CK1), N1);
      CHECK_EQUAL(data.at(CK2), N2);
      CHECK_EQUAL(data.at(CK3), N3);
      CHECK_EQUAL(data.at(CK4), N4);
      CHECK_EQUAL(data.at(CK5), N5);
      CHECK_EQUAL(data.at(CK6), N6);
      CHECK_EQUAL(data.at(CK7), N7);
      CHECK_EQUAL(data.at(CK8), N8);
      CHECK_EQUAL(data.at(CK9), N9);
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
      data.insert(DataNDC::value_type(K1,  N3)); // Duplicate key.  Not inserted

      CHECK_EQUAL(4U, data.size());

      DataNDC::iterator idata;

      idata = data.find(K0);
      CHECK(idata != data.end());
      CHECK(idata->first  == K0);
      CHECK(idata->second == N0);

      idata = data.find(K1);
      CHECK(idata != data.end());
      CHECK(idata->first  == K1);
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

      CHECK_THROW(data.insert(ETL_OR_STD::make_pair(K10, N10)), etl::unordered_map_full);
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

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::unordered_map_full);
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

      CHECK_EQUAL(1, data.at("1").value);
      CHECK_EQUAL(2, data.at("2").value);
      CHECK_EQUAL(3, data.at("3").value);
      CHECK_EQUAL(4, data.at("4").value);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      size_t count = data.erase(K5);

      CHECK_EQUAL(1U, count);

      DataNDC::iterator idata = data.find(K5);
      CHECK(idata == data.end());

      // Test that erase really does erase from the pool.
      CHECK(!data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key_using_transparent_comparator)
    {
      DataNDCTransparent data(initial_data.begin(), initial_data.end());

      size_t count = data.erase(CK5);

      CHECK_EQUAL(1U, count);

      DataNDCTransparent::iterator idata = data.find(CK5);
      CHECK(idata == data.end());

      // Test that erase really does erase from the pool.
      CHECK(!data.full());
      CHECK(!data.empty());
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

      CHECK(idata == data.end());
      CHECK(inext == iafter);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator idata     = data.begin();
      std::advance(idata, 2);

      DataNDC::const_iterator idata_end = data.begin();
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
      DataNDC data(initial_data.begin(), initial_data.end());

      size_t count = data.count(K5);
      CHECK_EQUAL(1U, count);

      count = data.count(K12);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_count_key_using_transparent_comparator)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      size_t count = data.count(CK5);
      CHECK_EQUAL(1U, count);

      count = data.count(CK12);
      CHECK_EQUAL(0U, count);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<DataNDC::iterator, DataNDC::iterator> result;

      result = data.equal_range(K0);
      CHECK(result.first  == data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K0);

      result = data.equal_range(K3);
      CHECK(result.first  != data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K3);

      result = data.equal_range(K9);
      CHECK(result.first  != data.begin());
      CHECK(result.second == data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K9);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_using_transparent_comparator)
    {
      DataNDCTransparent data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<DataNDCTransparent::iterator, DataNDCTransparent::iterator> result;

      result = data.equal_range("FF");
      CHECK(result.first  == data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, "FF");

      result = data.equal_range("FJ");
      CHECK(result.first  != data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, "FJ");

      result = data.equal_range("FO");
      CHECK(result.first  != data.begin());
      CHECK(result.second == data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, "FO");
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_const)
    {
      const DataNDC data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<DataNDC::const_iterator, DataNDC::const_iterator> result;

      result = data.equal_range(K0);
      CHECK(result.first == data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K0);

      result = data.equal_range(K3);
      CHECK(result.first != data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K3);

      result = data.equal_range(K9);
      CHECK(result.first != data.begin());
      CHECK(result.second == data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, K9);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal_range_const_using_transparent_comparator)
    {
      const DataNDCTransparent data(initial_data.begin(), initial_data.end());

      ETL_OR_STD::pair<DataNDCTransparent::const_iterator, DataNDCTransparent::const_iterator> result;

      result = data.equal_range("FF");
      CHECK(result.first  == data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, "FF");

      result = data.equal_range("FJ");
      CHECK(result.first  != data.begin());
      CHECK(result.second != data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, "FJ");

      result = data.equal_range("FO");
      CHECK(result.first  != data.begin());
      CHECK(result.second == data.end());
      CHECK_EQUAL(std::distance(result.first, result.second), 1);
      CHECK_EQUAL(result.first->first, "FO");
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

      using etl_map = etl::unordered_map<int, std::string, 20, 20, bad_hash>;
      using stl_map = std::unordered_map<int, std::string, bad_hash>;

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
      etl::unordered_map<uint32_t, char, 5> map;

      map[1] = 'b';
      map[2] = 'c';
      map[3] = 'd';
      map[4] = 'e';

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
      CHECK_EQUAL('c', map[2]);
      CHECK_EQUAL('d', map[3]);
    }

    //*************************************************************************
    TEST(test_ndc_hasher_and_key_eq)
    {
      typedef etl::unordered_map<size_t, int, 10, 10, ndc_hash, ndc_key_eq> Map;
      ndc_hash hasher1(1);
      ndc_hash hasher2(2);
      ndc_key_eq eq1(1);
      ndc_key_eq eq2(2);

      Map map1(hasher1, eq1);
      CHECK_EQUAL(map1.hash_function().id, 1);
      CHECK_EQUAL(map1.key_eq().id, 1);

      Map map2(hasher2, eq2);

      Map copyConstructed(map1);
      CHECK_EQUAL(copyConstructed.hash_function().id, 1);
      CHECK_EQUAL(copyConstructed.key_eq().id, 1);

      Map copyAssigned(hasher2, eq2);
      CHECK_EQUAL(copyAssigned.hash_function().id, 2);
      CHECK_EQUAL(copyAssigned.key_eq().id, 2);
      copyAssigned = map1;
      CHECK_EQUAL(copyAssigned.hash_function().id, 1);
      CHECK_EQUAL(copyAssigned.key_eq().id, 1);

      Map moveConstructed = std::move(map1);
      CHECK_EQUAL(moveConstructed.hash_function().id, 1);
      CHECK_EQUAL(moveConstructed.key_eq().id, 1);

      Map moveAssigned(hasher1, eq1);
      CHECK_EQUAL(moveAssigned.hash_function().id, 1);
      CHECK_EQUAL(moveAssigned.key_eq().id, 1);
      moveAssigned = std::move(map2);
      CHECK_EQUAL(moveAssigned.hash_function().id, 2);
      CHECK_EQUAL(moveAssigned.key_eq().id, 2);

      // make sure that map operations still work
      moveAssigned[5] = 7;
      CHECK_EQUAL(7, moveAssigned[5]);
    }

    //*************************************************************************
    TEST(test_parameterized_eq)
    {
      constexpr std::size_t MODULO = 4;
      parameterized_hash hash{MODULO};
      parameterized_equal eq{MODULO};
      // values are equal modulo 4
      etl::unordered_map<std::size_t, int, 10, 10, parameterized_hash, parameterized_equal> map;
      map.insert(etl::make_pair(2, 3));

      const auto& constmap = map;

      CHECK_EQUAL(map[10], 3);
      CHECK_EQUAL(map.at(10), 3);
      CHECK_EQUAL(constmap.at(10), 3);

      const std::pair<const std::size_t, int> keyvaluepair{6, 7};
      CHECK_FALSE(map.insert(keyvaluepair).second);
      CHECK_FALSE(map.insert(std::move(keyvaluepair)).second);

      CHECK(map.find(14) != map.end());
      CHECK(constmap.find(14) != constmap.end());

      map.erase(14);
      CHECK(map.find(6) == map.end());
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_copy_construct)
    {
      CustomHashFunction chf(1);
      CustomKeyEq        ceq(2);

      etl::unordered_map<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map1(chf, ceq);
      etl::unordered_map<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map2(map1);

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

      etl::unordered_map<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map1(chf1, ceq2);
      etl::unordered_map<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map2(chf3, ceq4);

      map2.operator=(map1);

      CHECK_EQUAL(chf1.id, map2.hash_function().id);
      CHECK_EQUAL(ceq2.id, map2.key_eq().id);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_construction_from_iterators)
    {
      CustomHashFunction chf1(1);
      CustomKeyEq        ceq2(2);

      using value_type = etl::unordered_map<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq>::value_type;
      std::array<value_type, 5> data =
      {
        value_type{1, 11},
        value_type{2, 22},
        value_type{3, 33},
        value_type{4, 44},
        value_type{5, 55}
      };

      etl::unordered_map<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map1(data.begin(), data.end(), chf1, ceq2);

      CHECK_EQUAL(chf1.id, map1.hash_function().id);
      CHECK_EQUAL(ceq2.id, map1.key_eq().id);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_construction_from_initializer_list)
    {
      CustomHashFunction chf1(1);
      CustomKeyEq        ceq2(2);

      using value_type = etl::unordered_map<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq>::value_type;

      etl::unordered_map<uint32_t, uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> map1({ value_type{1, 11}, value_type{2, 22}, value_type{3, 33}, value_type{4, 44}, value_type{5, 55} }, chf1, ceq2);

      CHECK_EQUAL(chf1.id, map1.hash_function().id);
      CHECK_EQUAL(ceq2.id, map1.key_eq().id);
    }

    //*************************************************************************
    TEST(test_iterator_value_types_bug_584)
    {
      using Map = etl::unordered_map<int, int, 1, 1>;
      CHECK((!std::is_same<typename Map::const_iterator::value_type, typename Map::iterator::value_type>::value));
    }

    //*************************************************************************
    TEST(test_iterator_value_types_bug_803)
    {
      using Map1 = etl::unordered_map<std::string, NDC, SIZE, 5>;
      using Map2 = etl::unordered_map<std::string, NDC, 2 * SIZE, 10>;
      
      Map1 map1(initial_data.begin(), initial_data.end());
      Map2 map2a(initial_data.begin(), initial_data.end());
      Map2 map2b(different_data.begin(), different_data.end());
      
      CHECK_TRUE(map1 == map2a);
      CHECK_FALSE(map1 == map2b);
    }

    //*************************************************************************
    TEST(test_contains)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      const char* not_inserted  = "ZZ";

      CHECK_TRUE(data.contains(K0));
      CHECK_FALSE(data.contains(std::string(not_inserted)));
    }

    //*************************************************************************
    TEST(test_contains_with_transparent_comparator)
    {
      DataNDCTransparent data(initial_data.begin(), initial_data.end());

      const char* not_inserted  = "ZZ";

      CHECK_TRUE(data.contains("FF"));
      CHECK_FALSE(data.contains(not_inserted));
    }
  };
}
