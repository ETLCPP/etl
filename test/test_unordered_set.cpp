/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#include "UnitTest++.h"

#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>
#include <numeric>

#include "data.h"

#include "unordered_set.h"
#include "checksum.h"

namespace
{
  SUITE(test_unordered_set)
  {
    static const size_t SIZE = 10;

    typedef TestDataDC<std::string>  DC;
    typedef TestDataNDC<std::string> NDC;

    struct simple_hash
    {
      size_t operator ()(const NDC& value) const
      {
        return etl::checksum<size_t>(value.value.begin(), value.value.end());
      }
    };

    typedef etl::unordered_set<DC,  SIZE, SIZE / 2, simple_hash> DataDC;
    typedef etl::unordered_set<NDC, SIZE, SIZE / 2, simple_hash> DataNDC;
    typedef etl::iunordered_set<NDC, simple_hash> IDataNDC;

    NDC N0  = NDC("FF");
    NDC N1  = NDC("FG");
    NDC N2  = NDC("FH");
    NDC N3  = NDC("FI");
    NDC N4  = NDC("FJ");
    NDC N5  = NDC("FK");
    NDC N6  = NDC("FL");
    NDC N7  = NDC("FM");
    NDC N8  = NDC("FN");
    NDC N9  = NDC("FO");
    NDC N10 = NDC("FP");
    NDC N11 = NDC("FQ");
    NDC N12 = NDC("FR");
    NDC N13 = NDC("FS");
    NDC N14 = NDC("FT");
    NDC N15 = NDC("FU");
    NDC N16 = NDC("FV");
    NDC N17 = NDC("FW");
    NDC N18 = NDC("FX");
    NDC N19 = NDC("FY");

    std::vector<NDC> initial_data;
    std::vector<NDC> excess_data;
    std::vector<NDC> different_data;

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

        initial_data.assign(std::begin(n), std::end(n));
        excess_data.assign(std::begin(n2), std::end(n2));
        different_data.assign(std::begin(n3), std::end(n3));
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

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
      CHECK(data.full());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC other_data;

      other_data = data;

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                other_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_interface)
    {
      DataNDC data1(initial_data.begin(), initial_data.end());
      DataNDC data2;

      IDataNDC& idata1 = data1;
      IDataNDC& idata2 = data2;

      idata2 = idata1;

      bool isEqual = std::equal(data1.begin(),
                                data1.end(),
                                data2.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      DataNDC data(initial_data.begin(), initial_data.end());
      DataNDC other_data(data);

      other_data = other_data;

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                other_data.begin());

      CHECK(isEqual);
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

      for (size_t i = 0; i < 10; ++i)
      {
        idata = data.find(initial_data[i]);
        CHECK(idata != data.end());
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value)
    {
      DataNDC data;

      data.insert(N0);  // Inserted
      data.insert(N2);  // Inserted
      data.insert(N1);  // Inserted
      data.insert(N11); // Duplicate hash. Inserted
      data.insert(N3);  // Inserted

      CHECK_EQUAL(5U, data.size());

      DataNDC::iterator idata;

      idata = data.find(N0);
      CHECK(idata != data.end());
      CHECK(*idata == N0);

      idata = data.find(N1);
      CHECK(idata != data.end());
      CHECK(*idata == N1);

      idata = data.find(N2);
      CHECK(idata != data.end());
      CHECK(*idata == N2);

      idata = data.find(N11);
      CHECK(idata != data.end());
      CHECK(*idata == N11);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_value_excess)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.insert(N10), etl::unordered_set_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range)
    {
      DataNDC data;

      data.insert(initial_data.begin(), initial_data.end());

      for (size_t i = 0; i < data.size(); ++i)
      {
        DataNDC::iterator idata = data.find(initial_data[i]);
        CHECK(idata != data.end());
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_range_excess)
    {
      DataNDC data;

      CHECK_THROW(data.insert(excess_data.begin(), excess_data.end()), etl::unordered_set_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_key)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      size_t count = data.erase(N5);

      CHECK_EQUAL(1U, count);

      DataNDC::iterator idata = data.find(N5);
      CHECK(idata == data.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::const_iterator idata = data.find(N5);
      DataNDC::const_iterator inext = idata;
      ++inext;

      DataNDC::const_iterator iafter = data.erase(idata);
      idata = data.find(N5);

      CHECK(idata == data.end());
      CHECK(inext == iafter);

      // Test that erase really does erase from the pool.
      CHECK(!data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator idata = data.begin();
      std::advance(idata, 2);

      DataNDC::iterator idata_end = data.begin();
      std::advance(idata_end, 5);

      data.erase(idata, idata_end);

      CHECK_EQUAL(initial_data.size() - 3, data.size());
      CHECK(!data.full());
      CHECK(!data.empty());

      idata = data.find(N8);
      CHECK(idata != data.end());

      idata = data.find(N0);
      CHECK(idata != data.end());

      idata = data.find(N1);
      CHECK(idata == data.end());

      idata = data.find(N2);
      CHECK(idata == data.end());

      idata = data.find(N3);
      CHECK(idata != data.end());

      idata = data.find(N4);
      CHECK(idata != data.end());

      idata = data.find(N5);
      CHECK(idata != data.end());

      idata = data.find(N6);
      CHECK(idata == data.end());

      idata = data.find(N7);
      CHECK(idata != data.end());

      idata = data.find(N8);
      CHECK(idata != data.end());

      idata = data.find(N9);
      CHECK(idata != data.end());
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

      size_t count = data.count(N5);
      CHECK_EQUAL(1U, count);

      count = data.count(N12);
      CHECK_EQUAL(0U, count);
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

      CHECK_EQUAL(simple_hash()(NDC(std::string("ABCDEF"))), hash_function(NDC(std::string("ABCDEF"))));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_key_eq_function)
    {
      DataNDC data;
      DataNDC::key_equal key_eq = data.key_eq();

      CHECK(key_eq(NDC(std::string("ABCDEF")), NDC(std::string("ABCDEF"))));
      CHECK(!key_eq(NDC(std::string("ABCDEF")), NDC(std::string("ABCDEG"))));
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
  };
}
