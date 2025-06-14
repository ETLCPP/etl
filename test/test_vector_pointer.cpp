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

#include <vector>
#include <array>
#include <algorithm>
#include <cstring>

#include "etl/vector.h"

namespace
{
  SUITE(test_vector_pointer)
  {
    static const size_t SIZE = 10;

    typedef etl::vector<int*, SIZE>       Data;
    typedef etl::vector<const int*, SIZE> CData;
    typedef etl::ivector<int*>            IData;
    typedef etl::ivector<const int*>      CIData;
    typedef std::vector<int*>             Compare_Data;
    typedef std::vector<const int*>       CCompare_Data;

    Compare_Data initial_data;
    Compare_Data less_data;
    Compare_Data greater_data;
    Compare_Data shorter_data;
    Compare_Data different_data;
    Compare_Data insert_data;

    int n0 = 0;
    int n1 = 1;
    int n2 = 2;
    int n3 = 3;
    int n4 = 4;
    int n5 = 5;
    int n6 = 6;
    int n7 = 7;
    int n8 = 8;
    int n9 = 9;
    int n11 = 11;
    int n12 = 12;
    int n13 = 13;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        int* n[]         = { &n0, &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9 };
        int* n_insert[]  = { &n11, &n12, &n13 };
        int* n_less[]    = { &n0, &n1, &n2, &n3, &n3, &n5, &n6, &n7, &n8, &n9 };
        int* n_greater[] = { &n0, &n1, &n2, &n4, &n4, &n5, &n6, &n7, &n8, &n9 };

        initial_data.assign(std::begin(n), std::end(n));
        insert_data.assign(std::begin(n_insert), std::end(n_insert));
        less_data.assign(std::begin(n_less), std::end(n_less));
        greater_data.assign(std::begin(n_greater), std::end(n_greater));
        shorter_data.assign(std::begin(n_greater), std::end(n_greater) - 1);
        different_data.assign(initial_data.rbegin(), initial_data.rend());
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

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      etl::vector data{ &n0, &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9 };
      etl::vector<int*, 10U> check = { &n0, &n1, &n2, &n3, &n4, &n5, &n6, &n7, &n8, &n9 };

      CHECK(!data.empty());
      CHECK(data.full());
      CHECK(data.begin() != data.end());
      CHECK_EQUAL(0U, data.available());
      CHECK_EQUAL(10U, data.capacity());
      CHECK_EQUAL(10U, data.size());
      CHECK_EQUAL(10U, data.max_size());
      CHECK(data == check);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_default_constructor)
    {
      CData data;

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.capacity(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST(test_iterator_comparison_empty)
    {
      Data data;

      CHECK(data.begin()   == data.end());
      CHECK(data.cbegin()  == data.cend());
      CHECK(data.rbegin()  == data.rend());
      CHECK(data.crbegin() == data.crend());
    }

    //*************************************************************************
    TEST(test_const_iterator_comparison_empty)
    {
      CData data;

      CHECK(data.begin() == data.end());
      CHECK(data.cbegin() == data.cend());
      CHECK(data.rbegin() == data.rend());
      CHECK(data.crbegin() == data.crend());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      const size_t INITIAL_SIZE = 5;
      Data data(INITIAL_SIZE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_constructor_size)
    {
      const size_t INITIAL_SIZE = 5;
      CData data(INITIAL_SIZE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      int INITIAL_VALUE = 1;

      std::array<int*, INITIAL_SIZE> compare_data;
      compare_data.fill(&INITIAL_VALUE);

      Data data(INITIAL_SIZE, &INITIAL_VALUE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_constructor_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      int INITIAL_VALUE = 1;

      std::array<const int*, INITIAL_SIZE> compare_data;
      compare_data.fill(&INITIAL_VALUE);

      CData data(INITIAL_SIZE, &INITIAL_VALUE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      CHECK_THROW(Data data(SIZE + 1), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_constructor_size_excess)
    {
      CHECK_THROW(CData data(SIZE + 1), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_constructor_range)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      CData data(compare_data.begin(), compare_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(test_constructor_initializer_list)
    {
      int a = 0;
      int b = 1;
      int c = 3;
      int d = 4;

      Compare_Data compare_data = { &a, &b, &c, &d };
      Data data = { &a, &b, &c, &d };

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_const_constructor_initializer_list)
    {
      int a = 0;
      int b = 1;
      int c = 3;
      int d = 4;

      CCompare_Data compare_data = { &a, &b, &c, &d };
      CData data = { &a, &b, &c, &d };

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data data2(data);
      CHECK(data2 == data);

      data2[2] = nullptr;
      CHECK(data2 != data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_copy_constructor)
    {
      CData data(initial_data.begin(), initial_data.end());
      CData data2(data);
      CHECK(data2 == data);

      data2[2] = nullptr;
      CHECK(data2 != data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_constructor)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data data2(std::move(data));

      CHECK_EQUAL(0U, data.size());
      CHECK_EQUAL(initial_data.size(), data2.size());

      CHECK_EQUAL(initial_data.size(), data2.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data other_data;

      other_data = data;

      CHECK_EQUAL(initial_data.size(), data.size());
      CHECK_EQUAL(initial_data.size(), other_data.size());

      bool is_equal = std::equal(data.begin(), data.end(), other_data.begin());
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_from_pointer_range)
    {
      Data data(initial_data.data(), initial_data.data() + initial_data.size());
      Data other_data;

      other_data = data;

      CHECK_EQUAL(initial_data.size(), data.size());
      CHECK_EQUAL(initial_data.size(), other_data.size());

      bool is_equal = std::equal(data.begin(), data.end(), other_data.begin());
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_assignment)
    {
      CData data(initial_data.begin(), initial_data.end());
      CData other_data;

      other_data = data;

      CHECK_EQUAL(initial_data.size(), data.size());
      CHECK_EQUAL(initial_data.size(), other_data.size());

      bool is_equal = std::equal(data.begin(), data.end(), other_data.begin());
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data other_data;
      other_data.push_back(nullptr);

      other_data = std::move(data);

      CHECK_EQUAL(0U, data.size());
      CHECK_EQUAL(initial_data.size(), other_data.size());

      bool is_equal = std::equal(data.begin(), data.end(), other_data.begin());
      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface)
    {
      Data data1(initial_data.begin(), initial_data.end());
      Data data2;

      IData& idata1 = data1;
      IData& idata2 = data2;

      idata2 = idata1;

      bool is_equal = std::equal(data1.begin(), data1.end(), data2.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_assignment_iterface)
    {
      CData data1(initial_data.begin(), initial_data.end());
      CData data2;

      CIData& idata1 = data1;
      CIData& idata2 = data2;

      idata2 = idata1;

      bool is_equal = std::equal(data1.begin(), data1.end(), data2.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_self_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data other_data(data);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_data = other_data;
#include "etl/private/diagnostic_pop.h" 

      bool is_equal = std::equal(data.begin(), data.end(), other_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_self_assignment)
    {
      CData data(initial_data.begin(), initial_data.end());
      CData other_data(data);

#include "etl/private/diagnostic_self_assign_overloaded_push.h" 
      other_data = other_data;
#include "etl/private/diagnostic_pop.h" 

      bool is_equal = std::equal(data.begin(), data.end(), other_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      Data data(10);
      const Data constData(10);

      CHECK_EQUAL(&data[0], data.begin());
      CHECK_EQUAL(&constData[0], constData.begin());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_begin)
    {
      CData data(10);
      const CData constData(10);

      CHECK_EQUAL(&data[0], data.begin());
      CHECK_EQUAL(&constData[0], constData.begin());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      Data data(10);
      const Data constData(10);

      CHECK_EQUAL(&data[10], data.end());
      CHECK_EQUAL(&constData[10], constData.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_end)
    {
      CData data(10);
      const CData constData(10);

      CHECK_EQUAL(&data[10], data.end());
      CHECK_EQUAL(&constData[10], constData.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_resize_up)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      CData data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;
      int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, &INITIAL_VALUE);
      data.resize(NEW_SIZE, &INITIAL_VALUE);

      std::array<int*, NEW_SIZE> compare_data;
      compare_data.fill(&INITIAL_VALUE);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_resize_up_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;
      int INITIAL_VALUE = 1;

      CData data(INITIAL_SIZE, &INITIAL_VALUE);
      data.resize(NEW_SIZE, &INITIAL_VALUE);

      std::array<int*, NEW_SIZE> compare_data;
      compare_data.fill(&INITIAL_VALUE);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = SIZE + 1;

      Data data(INITIAL_SIZE);

      CHECK_THROW(data.resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_resize_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = SIZE + 1;

      CData data(INITIAL_SIZE);

      CHECK_THROW(data.resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_resize_down)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      CData data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;
      int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE, &INITIAL_VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_resize_down_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;
      int INITIAL_VALUE = 1;

      CData data(INITIAL_SIZE);
      data.resize(NEW_SIZE, &INITIAL_VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty)
    {
      Data data;
      data.resize(data.max_size());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_empty)
    {
      CData data;
      data.resize(data.max_size());

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
    TEST_FIXTURE(SetupFixture, test_const_full)
    {
      CData data;

      CHECK(!data.full());
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_index)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      CData data(compare_data.begin(), compare_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_index_const)
    {
      const CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      const CData data(compare_data.begin(), compare_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_at)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      CData data(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_at_const)
    {
      const CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      const CData data(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK(data.front() == compare_data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_front)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      CData data(initial_data.begin(), initial_data.end());

      CHECK(data.front() == compare_data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      CHECK(data.front() == compare_data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_front_const)
    {
      const CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      const CData data(initial_data.begin(), initial_data.end());

      CHECK(data.front() == compare_data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == compare_data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_back)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      CData data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == compare_data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == compare_data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_back_const)
    {
      const CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      const CData data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == compare_data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.data(), data.data() + data.size(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_data)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      CData data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.data(), data.data() + data.size(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.data(), data.data() + data.size(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_data_const)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      const CData data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.data(), data.data() + data.size(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data;

      data.assign(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_assign_range)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      CData data;

      data.assign(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      int INITIAL_VALUE = 1;
      std::array<int*, INITIAL_SIZE> compare_data;
      compare_data.fill(&INITIAL_VALUE);

      Data data;
      data.assign(INITIAL_SIZE, &INITIAL_VALUE);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_assign_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      int INITIAL_VALUE = 1;
      std::array<const int*, INITIAL_SIZE> compare_data;
      compare_data.fill(&INITIAL_VALUE);

      CData data;
      data.assign(INITIAL_SIZE, &INITIAL_VALUE);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t EXCESS_SIZE = SIZE + 1;
      int INITIAL_VALUE = 1;
      std::array<int*, INITIAL_SIZE> compare_data;
      compare_data.fill(&INITIAL_VALUE);

      Data data;

      CHECK_THROW(data.assign(EXCESS_SIZE, &INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_assign_size_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t EXCESS_SIZE = SIZE + 1;
      int INITIAL_VALUE = 1;
      std::array<const int*, INITIAL_SIZE> compare_data;
      compare_data.fill(&INITIAL_VALUE);

      CData data;

      CHECK_THROW(data.assign(EXCESS_SIZE, &INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      Compare_Data compare_data;
      Data data;

      int d;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        compare_data.push_back(&d);
      }

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.push_back(&d);
      }

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_push_back)
    {
      CCompare_Data compare_data;
      CData data;

      const int d = 0;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        compare_data.push_back(&d);
      }

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.push_back(&d);
      }

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      Data data;

      int d;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.push_back(&d);
      }

      CHECK_THROW(data.push_back(&d), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_push_back_excess)
    {
      CData data;

      const int d = 0;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.push_back(&d);
      }

      CHECK_THROW(data.push_back(&d), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back)
    {
      Compare_Data compare_data;
      Data data;

      int d;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        compare_data.emplace_back(&d);
      }

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.emplace_back(&d);
      }

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back_return)
    {
      Data data;
      int d1 = 22;
      int d2 = 42;

      data.emplace_back(&d1);
      CHECK_EQUAL(&d2, data.emplace_back(&d2));
    }

    //*************************************************************************
    TEST(test_emplace_default)
    {
      static int initial = 0;

      // First fill with Initial values.
      etl::vector<int*, SIZE> data;
      data.resize(SIZE, &initial);
      data.clear();

      // Then emplace Default values.
      for (size_t i = 0; i < SIZE; ++i)
      {
        data.emplace(data.begin());
      }

      // Compare with an array of default values.
      std::array<int*, SIZE> compare_data;
      compare_data.fill(nullptr);

      CHECK_TRUE(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_emplace_back_default)
    {
      static int initial = 0;

      // First fill with initial values.
      etl::vector<int*, SIZE> data;
      data.resize(SIZE, &initial);
      data.clear();

      // Then emplace default values.
      for (size_t i = 0; i < SIZE; ++i)
      {
        data.emplace_back();
      }

      // Compare with an array of default values.
      std::array<int*, SIZE> compare_data;
      compare_data.fill(nullptr);

      CHECK_TRUE(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      compare_data.pop_back();
      compare_data.pop_back();

      data.pop_back();
      data.pop_back();

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_pop_back)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      CData data(initial_data.begin(), initial_data.end());

      compare_data.pop_back();
      compare_data.pop_back();

      data.pop_back();
      data.pop_back();

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back_exception)
    {
      Data data;

      data.resize(2);

      data.pop_back();
      data.pop_back();

      CHECK_THROW(data.pop_back(), etl::vector_empty);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_pop_back_exception)
    {
      CData data;

      data.resize(2);

      data.pop_back();
      data.pop_back();

      CHECK_THROW(data.pop_back(), etl::vector_empty);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      int INITIAL_VALUE   = 1;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.insert(data.begin() + offset, &INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, &INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_insert_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      int INITIAL_VALUE = 1;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CCompare_Data compare_data;
        CData data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.insert(data.begin() + offset, &INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, &INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
#include "etl/private/diagnostic_array_bounds_push.h"
    TEST_FIXTURE(SetupFixture, test_insert_position_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, &INITIAL_VALUE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, &INITIAL_VALUE), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, &INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, &INITIAL_VALUE), etl::vector_full);
    }
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      int INITIAL_VALUE = 1;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.emplace(data.begin() + offset, &INITIAL_VALUE);
        compare_data.emplace(compare_data.begin() + offset, &INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value)
    {
      const size_t INITIAL_SIZE     = 5;
      const size_t INSERT_SIZE      = 3;
      int INITIAL_VALUE = 11;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE);

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_insert_position_n_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t INSERT_SIZE = 3;
      int INITIAL_VALUE = 11;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CCompare_Data compare_data;
        CData data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE);

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t INSERT_SIZE  = 4;
      int INITIAL_VALUE   = 1;

      Data data(INITIAL_SIZE, &INITIAL_VALUE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_insert_position_n_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t INSERT_SIZE = 4;
      const int INITIAL_VALUE = 1;

      CData data(INITIAL_SIZE, &INITIAL_VALUE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, &INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      const size_t INITIAL_SIZE = 5;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, insert_data.begin(), insert_data.end());
        compare_data.insert(compare_data.begin() + offset, insert_data.begin(), insert_data.end());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_pointer_range)
    {
      const size_t INITIAL_SIZE = 5;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.assign(initial_data.data(), initial_data.data() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.data() + offset, insert_data.data(), insert_data.data() + insert_data.size());
        compare_data.insert(compare_data.begin() + offset, insert_data.begin(), insert_data.end());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_insert_position_range)
    {
      const size_t INITIAL_SIZE = 5;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        CCompare_Data compare_data;
        CData data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, insert_data.begin(), insert_data.end());
        compare_data.insert(compare_data.begin() + offset, insert_data.begin(), insert_data.end());

        bool is_equal = std::equal(data.begin(),
          data.end(),
          compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_excess)
    {
      const size_t INITIAL_SIZE = 5;
      int INITIAL_VALUE   = 1;

      Data data(INITIAL_SIZE, &INITIAL_VALUE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_insert_position_range_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;

      CData data(INITIAL_SIZE, &INITIAL_VALUE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator citr = compare_data.erase(compare_data.begin() + 2);
      Data::iterator ditr = data.erase(data.begin() + 2);

      CHECK(*citr == *ditr);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_erase_single_iterator)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      CData data(initial_data.begin(), initial_data.end());

      CCompare_Data::iterator citr = compare_data.erase(compare_data.begin() + 2);
      CData::iterator ditr = data.erase(data.begin() + 2);

      CHECK(*citr == *ditr);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_erase_single_const_iterator)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      CData data(initial_data.begin(), initial_data.end());

      CCompare_Data::iterator citr = compare_data.erase(compare_data.cbegin() + 2);
      CData::iterator ditr = data.erase(data.cbegin() + 2);

      CHECK(*citr == *ditr);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator citr = compare_data.erase(compare_data.cbegin() + 2, compare_data.cbegin() + 4);
      Data::iterator ditr = data.erase(data.cbegin() + 2, data.cbegin() + 4);

      CHECK(*citr == *ditr);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_erase_range)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());
      CData data(initial_data.begin(), initial_data.end());

      compare_data.erase(compare_data.begin() + 2, compare_data.begin() + 4);

      data.erase(data.begin() + 2, data.begin() + 4);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
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
    TEST_FIXTURE(SetupFixture, test_const_clear)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      CData data(compare_data.begin(), compare_data.end());
      data.clear();

      CHECK_EQUAL(data.size(), size_t(0));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator_const)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      CData data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator_const)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      CData data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator_const)
    {
      CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      CData data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.crbegin(), data.crend(), compare_data.crbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator_const)
    {
      const CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      const CData data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.crbegin(), data.crend(), compare_data.crbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator2)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator2_const)
    {
      const CCompare_Data compare_data(initial_data.begin(), initial_data.end());

      const CData data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      const Data initial1(initial_data.begin(), initial_data.end());
      const Data initial2(initial_data.begin(), initial_data.end());

      CHECK(initial1 == initial2);

      const Data different(different_data.begin(), different_data.end());

      CHECK(!(initial1 == different));

      const Data shorter(shorter_data.begin(), shorter_data.end());

      CHECK(!(shorter == initial1));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_equal)
    {
      const CData initial1(initial_data.begin(), initial_data.end());
      const CData initial2(initial_data.begin(), initial_data.end());

      CHECK(initial1 == initial2);

      const CData different(different_data.begin(), different_data.end());

      CHECK(!(initial1 == different));

      const CData shorter(shorter_data.begin(), shorter_data.end());

      CHECK(!(shorter == initial1));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      const Data initial1(initial_data.begin(), initial_data.end());
      const Data initial2(initial_data.begin(), initial_data.end());

      CHECK(!(initial1 != initial2));

      const Data different(different_data.begin(), different_data.end());

      CHECK(initial1 != different);

      const Data shorter(shorter_data.begin(), shorter_data.end());

      CHECK(shorter != initial1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_not_equal)
    {
      const CData initial1(initial_data.begin(), initial_data.end());
      const CData initial2(initial_data.begin(), initial_data.end());

      CHECK(!(initial1 != initial2));

      const CData different(different_data.begin(), different_data.end());

      CHECK(initial1 != different);

      const CData shorter(shorter_data.begin(), shorter_data.end());

      CHECK(shorter != initial1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than)
    {
      const Data less(less_data.begin(), less_data.end());
      const Data initial(initial_data.begin(), initial_data.end());

      CHECK((less < initial) == (less_data < initial_data));

      const Data greater(greater_data.begin(), greater_data.end());

      CHECK((greater < initial) == (greater_data < initial_data));

      const Data shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter < initial) == (shorter_data < initial_data));
      CHECK((initial < shorter) == (initial_data < shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_less_than)
    {
      const CData less(less_data.begin(), less_data.end());
      const CData initial(initial_data.begin(), initial_data.end());

      CHECK((less < initial) == (less_data < initial_data));

      const CData greater(greater_data.begin(), greater_data.end());

      CHECK((greater < initial) == (greater_data < initial_data));

      const CData shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter < initial) == (shorter_data < initial_data));
      CHECK((initial < shorter) == (initial_data < shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than_or_equal)
    {
      const Data less(less_data.begin(), less_data.end());
      const Data initial(initial_data.begin(), initial_data.end());

      CHECK((less <= initial) == (less_data <= initial_data));

      const Data greater(greater_data.begin(), greater_data.end());

      CHECK((greater <= initial) == (greater_data <= initial_data));

      const Data shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter <= initial) == (shorter_data <= initial_data));
      CHECK((initial <= shorter) == (initial_data <= shorter_data));

      const Data initial2(initial_data.begin(), initial_data.end());
      CHECK((initial <= initial2) == (initial_data <= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_less_than_or_equal)
    {
      const CData less(less_data.begin(), less_data.end());
      const CData initial(initial_data.begin(), initial_data.end());

      CHECK((less <= initial) == (less_data <= initial_data));

      const CData greater(greater_data.begin(), greater_data.end());

      CHECK((greater <= initial) == (greater_data <= initial_data));

      const CData shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter <= initial) == (shorter_data <= initial_data));
      CHECK((initial <= shorter) == (initial_data <= shorter_data));

      const CData initial2(initial_data.begin(), initial_data.end());
      CHECK((initial <= initial2) == (initial_data <= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than)
    {
      const Data less(less_data.begin(), less_data.end());
      const Data initial(initial_data.begin(), initial_data.end());

      CHECK((less > initial) == (less_data > initial_data));

      const Data greater(greater_data.begin(), greater_data.end());

      CHECK((greater > initial) == (greater_data > initial_data));

      const Data shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter > initial) == (shorter_data > initial_data));
      CHECK((initial > shorter) == (initial_data > shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_greater_than)
    {
      const CData less(less_data.begin(), less_data.end());
      const CData initial(initial_data.begin(), initial_data.end());

      CHECK((less > initial) == (less_data > initial_data));

      const CData greater(greater_data.begin(), greater_data.end());

      CHECK((greater > initial) == (greater_data > initial_data));

      const CData shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter > initial) == (shorter_data > initial_data));
      CHECK((initial > shorter) == (initial_data > shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than_or_equal)
    {
      const Data less(less_data.begin(), less_data.end());
      const Data initial(initial_data.begin(), initial_data.end());

      CHECK((less >= initial) == (less_data >= initial_data));

      const Data greater(greater_data.begin(), greater_data.end());

      CHECK((greater >= initial) == (greater_data >= initial_data));

      const Data shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter >= initial) == (shorter_data >= initial_data));
      CHECK((initial >= shorter) == (initial_data > shorter_data));

      const Data initial2(initial_data.begin(), initial_data.end());
      CHECK((initial >= initial2) == (initial_data >= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_greater_than_or_equal)
    {
      const CData less(less_data.begin(), less_data.end());
      const CData initial(initial_data.begin(), initial_data.end());

      CHECK((less >= initial) == (less_data >= initial_data));

      const CData greater(greater_data.begin(), greater_data.end());

      CHECK((greater >= initial) == (greater_data >= initial_data));

      const CData shorter(shorter_data.begin(), shorter_data.end());

      CHECK((shorter >= initial) == (shorter_data >= initial_data));
      CHECK((initial >= shorter) == (initial_data > shorter_data));

      const CData initial2(initial_data.begin(), initial_data.end());
      CHECK((initial >= initial2) == (initial_data >= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_memcpy_repair)
    {
      Data data(initial_data.begin(), initial_data.end());

      char buffer[sizeof(Data)];

      memcpy(&buffer, (const void*)&data, sizeof(data));

      Data& rdata(*reinterpret_cast<Data*>(buffer));
      rdata.repair();

      // Check that the memcpy'd vector is the same.
      CHECK_EQUAL(data.size(), rdata.size());
      CHECK(!rdata.empty());
      CHECK(rdata.full());

      bool is_equal = std::equal(rdata.begin(),
                                 rdata.end(),
                                 data.begin());

      CHECK(is_equal);

      // Modify the original and check that the memcpy'd vector is not the same.
      std::reverse(data.begin(), data.end());

      is_equal = std::equal(rdata.begin(), rdata.end(), data.begin());

      CHECK(!is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_memcpy_repair)
    {
      CData data(initial_data.begin(), initial_data.end());

      char buffer[sizeof(CData)];

      memcpy(&buffer, (const void*)&data, sizeof(data));

      CData& rdata(*reinterpret_cast<CData*>(buffer));
      rdata.repair();

      // Check that the memcpy'd vector is the same.
      CHECK_EQUAL(data.size(), rdata.size());
      CHECK(!rdata.empty());
      CHECK(rdata.full());

      bool is_equal = std::equal(rdata.begin(), rdata.end(), data.begin());

      CHECK(is_equal);

      // Modify the original and check that the memcpy'd vector is not the same.
      std::reverse(data.begin(), data.end());

      is_equal = std::equal(rdata.begin(), rdata.end(), data.begin());

      CHECK(!is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_ivector_of_pointer_to_pointer)
    {
      int i1 = 1;
      etl::vector<int*, 10> consttest;
      consttest.push_back(&i1);
      const etl::ivector<int*>& ct = consttest;

      int* i2 = ct[0];

      CHECK(i1 == *i2);
      CHECK(&i1 == i2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_ivector_of_pointer_to_const_pointer)
    {
      int i1 = 1;
      etl::vector<const int*, 10> consttest;
      consttest.push_back(&i1);
      const etl::ivector<const int*>& ct = consttest;

      const int* i2 = ct[0];

      CHECK(i1 == *i2);
      CHECK(&i1 == i2);
    }

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST(test_make_vector)
    {
      const int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      auto data = etl::make_vector<const int*>( &values[0], &values[1], &values[2], &values[3], &values[4], &values[5], &values[6], &values[7], &values[8], &values[9] );

      CHECK_EQUAL(0, *data[0]);
      CHECK_EQUAL(1, *data[1]);
      CHECK_EQUAL(2, *data[2]);
      CHECK_EQUAL(3, *data[3]);
      CHECK_EQUAL(4, *data[4]);
      CHECK_EQUAL(5, *data[5]);
      CHECK_EQUAL(6, *data[6]);
      CHECK_EQUAL(7, *data[7]);
      CHECK_EQUAL(8, *data[8]);
      CHECK_EQUAL(9, *data[9]);
    }
#endif
  };
}
