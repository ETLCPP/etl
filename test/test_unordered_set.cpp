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

#include <array>
#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_set>

#include "data.h"

#include "etl/unordered_set.h"
#include "etl/checksum.h"
#include "etl/hash.h"

namespace
{
  typedef TestDataDC<std::string>  DC;
  typedef TestDataNDC<std::string> NDC;
}

namespace etl
{
  template <>
  struct hash<NDC>
  {
    size_t operator ()(const NDC& e) const
    {
      size_t sum = 0U;
      return std::accumulate(e.value.begin(), e.value.end(), sum);
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

  //***************************************************************************
  SUITE(test_unordered_set)
  {
    static const size_t SIZE = 10;

    using ItemM = TestDataM<int>;

    struct simple_hash
    {
      size_t operator ()(const NDC& value) const
      {
        return etl::checksum<size_t>(value.value.begin(), value.value.end());
      }

      size_t operator ()(const ItemM& value) const
      {
        etl::checksum<size_t> sum;

        sum.add(value.valid);
        sum.add(value.value);

        return sum.value();
      }
    };

    using DataM = etl::unordered_set<ItemM, SIZE, SIZE, simple_hash>;

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

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_cpp17_deduced_constructor)
    {
      etl::unordered_set data{ N0, N1, N2, N3, N4, N5, N6, N7, N8, N9 };
      etl::unordered_set<NDC, 10U> check = { N0, N1, N2, N3, N4, N5, N6, N7, N8, N9 };

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

      data1.insert(etl::move(d1));
      data1.insert(etl::move(d2));
      data1.insert(etl::move(d3));
      data1.insert(ItemM(4));

      DataM data2(std::move(data1));

      CHECK(!data1.empty()); // Move does not clear the source.
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_destruct_via_iunordered_set)
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

      for (size_t i = 0UL; i < data.size(); ++i)
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
      DataNDC data;

      data.insert(N0);  // Inserted
      data.insert(N1);  // Inserted
      data.insert(N2);  // Inserted
      data.insert(N3);  // Inserted
      data.insert(N4);  // Inserted
      data.insert(N5);  // Inserted  
      data.insert(N6);  // Inserted
      data.insert(N7);  // Inserted
      data.insert(N8);  // Inserted
      data.insert(N9);  // Inserted
      
      // Try to insert existing item when unordered_set is full should not fail
      CHECK_NO_THROW(data.insert(N0));
      CHECK_NO_THROW(data.insert(N1));
      CHECK_NO_THROW(data.insert(N2));
      CHECK_NO_THROW(data.insert(N3));
      CHECK_NO_THROW(data.insert(N4));
      CHECK_NO_THROW(data.insert(N5));
      CHECK_NO_THROW(data.insert(N6));
      CHECK_NO_THROW(data.insert(N7));
      CHECK_NO_THROW(data.insert(N8));
      CHECK_NO_THROW(data.insert(N9));
      
      CHECK(data.size() == SIZE);
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
    TEST_FIXTURE(SetupFixture, test_erase_single_iterator)
    {
      DataNDC data(initial_data.begin(), initial_data.end());

      DataNDC::iterator idata = data.find(N5);
      DataNDC::iterator inext = idata;
      ++inext;

      DataNDC::iterator iafter = data.erase(idata);
      idata = data.find(N5);

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

      DataNDC::const_iterator idata = data.find(N5);
      DataNDC::const_iterator inext = idata;
      ++inext;

      DataNDC::iterator iafter = data.erase(idata);
      idata = data.find(N5);

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

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_and_erase_bug)
    {
      etl::unordered_set<uint32_t, 5> set;

      set.insert(1);
      set.insert(2);
      set.insert(3);
      set.insert(4);

      auto it = set.find(1);
      set.erase(it);

      it = set.find(4);
      set.erase(it);

      std::vector<std::string> s;

      for (const auto &kv : set)
      {
        std::stringstream ss;
        ss << "set" << " = " << kv;
        s.push_back(ss.str());
      }

      CHECK_EQUAL(2, s.size());
      CHECK_EQUAL("set = 2", s[0]);
      CHECK_EQUAL("set = 3", s[1]);
    }

    TEST(test_parameterized_eq)
    {
      constexpr std::size_t MODULO = 4;
      parameterized_hash hash{MODULO};
      parameterized_equal eq{MODULO};
      // values are equal modulo 4
      etl::unordered_set<std::size_t, 10, 10, parameterized_hash, parameterized_equal> set;
      set.insert(2);

      const auto& constset = set;

      const size_t key = 6;
      CHECK_FALSE(set.insert(key).second);
      CHECK_FALSE(set.insert(std::move(key)).second);

      CHECK(set.find(14) != set.end());
      CHECK(constset.find(14) != constset.end());

      set.erase(14);
      CHECK(set.find(6) == set.end());
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

      std::vector<int> random_keys1 = { 17, 14, 3,  7, 2, 6, 9,  3, 18, 10,  8, 11,  4, 1, 12, 15, 16,  0,  5, 19 };
      std::vector<int> random_keys2 = {  3,  6, 5, 17, 2, 7, 3, 19,  8, 15, 14,  0, 18, 4, 10,  9, 16, 11, 12,  1 };

      // Check that the input data is valid.
      CHECK_EQUAL(random_keys1.size(), random_keys2.size());
      CHECK(std::is_permutation(random_keys1.begin(), random_keys1.end(), random_keys2.begin()));

      //***************************************************
      // Fill ETL
      etl::unordered_set<int, 20, 20, bad_hash> etlset1;
      etl::unordered_set<int, 20, 20, bad_hash> etlset2;

      for (auto i : random_keys1)
      {
        etlset1.insert(i);
      }

      for (auto i : random_keys2)
      {
        etlset2.insert(i);
      }

      //***************************************************
      // Fill STD
      std::unordered_set<int, bad_hash> stdset1;
      std::unordered_set<int, bad_hash> stdset2;

      for (auto i : random_keys1)
      {
        stdset1.insert(i);
      }

      for (auto i : random_keys2)
      {
        stdset2.insert(i);
      }

      //***************************************************
      CHECK_EQUAL((stdset1 == stdset2), (etlset1 == etlset2));
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_copy_construct)
    {
      CustomHashFunction chf(1);
      CustomKeyEq        ceq(2);

      etl::unordered_set<uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> set1(chf, ceq);
      etl::unordered_set<uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> set2(set1);

      CHECK_EQUAL(chf.id, set2.hash_function().id);
      CHECK_EQUAL(ceq.id, set2.key_eq().id);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_assignment)
    {
      CustomHashFunction chf1(1);
      CustomKeyEq        ceq2(2);

      CustomHashFunction chf3(3);
      CustomKeyEq        ceq4(4);

      etl::unordered_set<uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> set1(chf1, ceq2);
      etl::unordered_set<uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> set2(chf3, ceq4);

      set2.operator=(set1);

      CHECK_EQUAL(chf1.id, set2.hash_function().id);
      CHECK_EQUAL(ceq2.id, set2.key_eq().id);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_construction_from_iterators)
    {
      CustomHashFunction chf1(1);
      CustomKeyEq        ceq2(2);

      std::array<uint32_t, 5> data = { 1, 2, 3, 4, 5 };

      etl::unordered_set<uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> set1(data.begin(), data.end(), chf1, ceq2);

      CHECK_EQUAL(chf1.id, set1.hash_function().id);
      CHECK_EQUAL(ceq2.id, set1.key_eq().id);
    }

    //*************************************************************************
    TEST(test_copying_of_hash_and_key_compare_with_construction_from_initializer_list)
    {
      CustomHashFunction chf1(1);
      CustomKeyEq        ceq2(2);

      etl::unordered_set<uint32_t, 5, 5, CustomHashFunction, CustomKeyEq> set1({ 1, 2, 3, 4, 5 }, chf1, ceq2);

      CHECK_EQUAL(chf1.id, set1.hash_function().id);
      CHECK_EQUAL(ceq2.id, set1.key_eq().id);
    }

    //*************************************************************************
    TEST(test_iterator_value_types_bug_584)
    {
      using Set = etl::unordered_set<int, 1, 1>;
      CHECK((!std::is_same<typename Set::const_iterator::value_type, typename Set::iterator::value_type>::value));
    }

    //*************************************************************************
    TEST(test_iterator_value_types_bug_803)
    {
      using Set1 = etl::unordered_set<NDC, SIZE, 5>;
      using Set2 = etl::unordered_set<NDC, 2 * SIZE, 10>;

      Set1 set1(initial_data.begin(), initial_data.end());
      Set2 set2a(initial_data.begin(), initial_data.end());
      Set2 set2b(different_data.begin(), different_data.end());

      CHECK_TRUE(set1 == set2a);
      CHECK_FALSE(set1 == set2b);
    }
  };
}
