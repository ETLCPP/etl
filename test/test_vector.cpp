/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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
  SUITE(test_vector)
  {
    static const size_t SIZE = 10;

    typedef etl::vector<int, SIZE> Data;
    typedef etl::ivector<int>      IData;
    typedef std::vector<int>       Compare_Data;

    Compare_Data initial_data;
    Compare_Data less_data;
    Compare_Data greater_data;
    Compare_Data shorter_data;
    Compare_Data different_data;
    Compare_Data insert_data;
    Compare_Data blank_data;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        int n[]         = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int n_insert[]  = { 11, 12, 13 };
        int n_less[]    = { 0, 1, 2, 3, 3, 5, 6, 7, 8, 9 };
        int n_greater[] = { 0, 1, 2, 4, 4, 5, 6, 7, 8, 9 };
        int n_blank[]   = { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 };

        initial_data.assign(std::begin(n), std::end(n));
        insert_data.assign(std::begin(n_insert), std::end(n_insert));
        less_data.assign(std::begin(n_less), std::end(n_less));
        greater_data.assign(std::begin(n_greater), std::end(n_greater));
        shorter_data.assign(std::begin(n_greater), std::end(n_greater) - 1);
        different_data.assign(initial_data.rbegin(), initial_data.rend());
        blank_data.assign(std::begin(n_blank), std::end(n_blank));
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
      CHECK(data.begin() == data.end());
    }

#if ETL_USING_CPP17 && ETL_HAS_INITIALIZER_LIST && !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED)
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      etl::vector data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      etl::vector<int, 10U> check = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

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
    TEST(test_iterator_comparison_empty)
    {
      Data data;

      CHECK(data.begin()   == data.end());
      CHECK(data.cbegin()  == data.cend());
      CHECK(data.rbegin()  == data.rend());
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
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;

      std::array<int, INITIAL_SIZE> compare_data;
      compare_data.fill(INITIAL_VALUE);

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_excess)
    {
      CHECK_THROW(Data data(SIZE + 1), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(test_constructor_initializer_list)
    {
      Compare_Data compare_data = { 0, 1, 2, 3 };
      Data data = { 0, 1, 2, 3 };

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

      data2[2] = -1;
      CHECK(data2 != data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_constructor)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data data2(std::move(data));
      CHECK(data.size() == 0);
      CHECK(data2.size() == initial_data.size());
      CHECK(data2 != data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data other_data;

      other_data = data;

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data other_data;
      other_data.push_back(1);

      other_data = std::move(data);

      CHECK(data.size() == 0);
      CHECK(other_data.size() == initial_data.size());
      CHECK(data != other_data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface)
    {
      Data data1(initial_data.begin(), initial_data.end());
      Data data2;

      IData& idata1 = data1;
      IData& idata2 = data2;

      idata2 = idata1;

      bool is_equal = std::equal(data1.begin(),
                                 data1.end(),
                                 data2.begin());

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

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

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
    TEST_FIXTURE(SetupFixture, test_end)
    {
      Data data(10);
      const Data constData(10);

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
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;
      const int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE);
      data.resize(NEW_SIZE, INITIAL_VALUE);

      std::array<int, NEW_SIZE> compare_data;
      compare_data.fill(INITIAL_VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

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
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;
      const int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_up)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      Data data(INITIAL_SIZE);

      int* pbegin = &data.front();
      int* pend   = &data.back() + 1;
      int* pmax = pbegin + data.max_size();

      constexpr int Pattern = 0x12345678UL;

      // Fill free space with a pattern.
      std::fill(pend, pmax, Pattern);

      data.uninitialized_resize(NEW_SIZE);

      for (int* p = pbegin; p != pend; ++p)
      {
        CHECK_EQUAL(*p, 0);
      }

      for (int* p = pend; p != pmax; ++p)
      {
        CHECK_EQUAL(*p, Pattern);
      }

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_up_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = SIZE + 1;

      Data data(INITIAL_SIZE);

      CHECK_THROW(data.uninitialized_resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_down)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      Data data(INITIAL_SIZE);

      int* pbegin = &data.front();
      int* pend   = &data.back() + 1;
      int* pmax   = pbegin + data.max_size();

      constexpr int Pattern = 0x12345678UL;

      // Fill free space with a pattern.
      std::fill(pend, pmax, Pattern);

      data.uninitialized_resize(NEW_SIZE);

      pend = &data.back() + 1;

      for (int* p = pbegin; p < pend; ++p)
      {
        CHECK_EQUAL(*p, 0);
      }

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reserve)
    {
      Data data;

      CHECK_NO_THROW(data.reserve(data.max_size()));
      CHECK_THROW(data.reserve(data.max_size() + 1), etl::vector_out_of_bounds);
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
    TEST_FIXTURE(SetupFixture, test_full)
    {
      Data data;

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
    TEST_FIXTURE(SetupFixture, test_front)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

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
    TEST_FIXTURE(SetupFixture, test_back)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

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
    TEST_FIXTURE(SetupFixture, test_data)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.data(),
        data.data() + data.size(),
        compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.data(),
                                data.data() + data.size(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data;

      data.assign(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;
      std::array<int, INITIAL_SIZE> compare_data;
      compare_data.fill(INITIAL_VALUE);

      Data data;
      data.assign(INITIAL_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t EXCESS_SIZE = SIZE + 1;
      const int INITIAL_VALUE = 1;
      std::array<int, INITIAL_SIZE> compare_data;
      compare_data.fill(INITIAL_VALUE);

      Data data;

      CHECK_THROW(data.assign(EXCESS_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      Compare_Data compare_data;
      Data data;

      for (int i = 0; i < int(SIZE); ++i)
      {
        compare_data.push_back(i);
      }

      for (int i = 0; i < int(SIZE); ++i)
      {
        data.push_back(i);
      }

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_literal)
    {
      Compare_Data compare_data;
      Data data;

      compare_data.push_back(1);
      compare_data.push_back(2);
      compare_data.push_back(3);
      compare_data.push_back(4);

      data.push_back(1);
      data.push_back(2);
      data.push_back(3);
      data.push_back(4);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      Data data;

      for (int i = 0; i < int(SIZE); ++i)
      {
        data.push_back(i);
      }

      CHECK_THROW(data.push_back(SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back)
    {
      Compare_Data compare_data;
      Data data;

      for (int i = 0; i < int(SIZE); ++i)
      {
        compare_data.emplace_back(i);
      }

      for (int i = 0; i < int(SIZE); ++i)
      {
        data.emplace_back(i);
      }

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back_return)
    {
      Data data;

      data.emplace_back(24);
      auto back = data.emplace_back(42);
      CHECK_EQUAL(back, data.back());
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

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

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
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE   = 1;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.insert(data.cbegin() + offset, INITIAL_VALUE);
        compare_data.insert(compare_data.cbegin() + offset, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.cbegin(),
                                   data.cend(),
                                   compare_data.cbegin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.emplace(data.cbegin() + offset, INITIAL_VALUE);
        compare_data.emplace(compare_data.cbegin() + offset, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.cbegin(),
                                   data.cend(),
                                   compare_data.cbegin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST(test_emplace_default)
    {
      const int Initial = 1;
      const int Default = 2;

      struct S
      {
        S()
          : value(Default)
        {
        }

        S(int v)
          : value(v)
        {
        }

        bool operator ==(const S& rhs) const
        {
          return value == rhs.value;
        }

        int value;
      };

      // First fill with Initial values.
      etl::vector<S, SIZE> data;
      data.resize(SIZE, S(Initial));
      data.clear();

      // Then emplace Default values.
      for (size_t i = 0; i < SIZE; ++i)
      {
        data.emplace(data.end());
      }

      // Compare with an array of default values.
      std::array<S, SIZE> compare_data;
      compare_data.fill(S());

      CHECK_TRUE(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_emplace_back_default)
    {
      const int Initial = 1;
      const int Default = 2;

      struct S
      {
        S()
          : value(Default)
        {
        }

        S(int v)
          : value(v)
        {
        }

        bool operator ==(const S& rhs) const
        {
          return value == rhs.value;
        }

        int value;
      };

      // First fill with Initial values.
      etl::vector<S, SIZE> data;
      data.resize(SIZE, S(Initial));
      data.clear();

      // Then emplace Default values.
      for (size_t i = 0; i < SIZE; ++i)
      {
        data.emplace_back();
      }

      // Compare with an array of default values.
      std::array<S, SIZE> compare_data;
      compare_data.fill(S());

      CHECK_TRUE(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
#include "etl/private/diagnostic_array_bounds_push.h"
    TEST_FIXTURE(SetupFixture, test_insert_position_value_excess)
    {
      const size_t INITIAL_SIZE     = SIZE;
      const int INITIAL_VALUE       = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.cbegin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.cbegin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.cbegin() + offset, INITIAL_VALUE), etl::vector_full);
    }
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value)
    {
      const size_t INITIAL_SIZE     = 5;
      const size_t INSERT_SIZE      = 3;
      const int INITIAL_VALUE       = 11;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.cbegin() + offset, INSERT_SIZE, INITIAL_VALUE);
        compare_data.insert(compare_data.cbegin() + offset, INSERT_SIZE, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.cbegin(),
                                   data.cend(),
                                   compare_data.cbegin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t INSERT_SIZE  = 4;
      const int INITIAL_VALUE   = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.cbegin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.cbegin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.cbegin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.cbegin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      const size_t INITIAL_SIZE = 5;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data;

        data.resize(SIZE, -1);

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.cbegin() + offset, insert_data.cbegin(), insert_data.cend());
        compare_data.insert(compare_data.cbegin() + offset, insert_data.cbegin(), insert_data.cend());

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.cbegin(),
                                   data.cend(),
                                   compare_data.cbegin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE   = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.cbegin() + offset, initial_data.cbegin(), initial_data.cend()), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.cbegin() + offset, initial_data.cbegin(), initial_data.cend()), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.cbegin() + offset, initial_data.cbegin(), initial_data.cend()), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.cbegin() + offset, initial_data.cbegin(), initial_data.cend()), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator const_cdi = compare_data.begin() + 2U;
      int compare_value = *(const_cdi + 1U);
      Compare_Data::iterator cdi = compare_data.erase(const_cdi);
      CHECK_EQUAL(compare_value, *cdi);

      Data::iterator const_di = data.begin() + 2U;
      int data_value = *(const_di + 1U);
      Data::iterator di = data.erase(const_di);
      CHECK_EQUAL(data_value, *di);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
        data.end(),
        compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_const_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator const_cdi = compare_data.cbegin() + 2U;
      int compare_value = *(const_cdi + 1U);
      Compare_Data::iterator cdi = compare_data.erase(const_cdi);
      CHECK_EQUAL(compare_value, *cdi);

      Data::const_iterator const_di = data.cbegin() + 2U;
      int data_value = *(const_di + 1U);
      Data::iterator di = data.erase(const_di);
      CHECK_EQUAL(data_value, *di);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      Compare_Data::const_iterator const_cdi = compare_data.cbegin() + 2U;
      int compare_value = *(const_cdi + 2U);
      Compare_Data::iterator cdi = compare_data.erase(const_cdi, const_cdi + 2U);
      CHECK_EQUAL(compare_value, *cdi);

      Data::const_iterator const_di = data.cbegin() + 2U;
      int data_value = *(const_di + 2U);
      Data::iterator di = data.erase(const_di, const_di + 2U);
      CHECK_EQUAL(data_value, *di);

      CHECK_EQUAL(compare_data.size(), data.size());

      bool is_equal = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

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
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.cbegin(),
                                 data.cend(),
                                 compare_data.cbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.crbegin(),
                                 data.crend(),
                                 compare_data.crbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator2)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      bool is_equal = std::equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

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

      is_equal = std::equal(rdata.begin(),
                            rdata.end(),
                            data.begin());

      CHECK(!is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_memcpy_repair_virtual)
    {
      Data data(initial_data.begin(), initial_data.end());

      char buffer[sizeof(Data)];

      memcpy(&buffer, (const void*)&data, sizeof(data));

      IData& idata(*reinterpret_cast<Data*>(buffer));
      idata.repair();

      // Check that the memcpy'd vector is the same.
      CHECK_EQUAL(data.size(), idata.size());
      CHECK(!idata.empty());
      CHECK(idata.full());

      bool is_equal = std::equal(idata.begin(),
                                 idata.end(),
                                 data.begin());

      CHECK(is_equal);

      // Modify the original and check that the memcpy'd vector is not the same.
      std::reverse(data.begin(), data.end());

      is_equal = std::equal(idata.begin(),
                            idata.end(),
                            data.begin());

      CHECK(!is_equal);
    }

    //*************************************************************************
    TEST(test_insert_bug)
    {
      struct S
      {
        virtual ~S()
        {

        }

        S(int i_): i(i_){}

        int i;
      };

      const S raw[6] = { 1, 2, 3, 4, 5, 6 };

      etl::vector<S, 10> dest(ETL_OR_STD11::begin(raw), ETL_OR_STD11::end(raw));
      etl::vector<S, 10> src((size_t) 2, S(8));

      dest.insert(dest.begin(), src.begin(), src.end());

      CHECK(dest.size() == 8);
      CHECK_EQUAL(src[0].i, dest[0].i);
      CHECK_EQUAL(src[1].i, dest[1].i);
      CHECK_EQUAL(raw[0].i, dest[2].i);
      CHECK_EQUAL(raw[1].i, dest[3].i);
      CHECK_EQUAL(raw[2].i, dest[4].i);
      CHECK_EQUAL(raw[3].i, dest[5].i);
      CHECK_EQUAL(raw[4].i, dest[6].i);
      CHECK_EQUAL(raw[5].i, dest[7].i);
    }

    //*************************************************************************
    TEST(test_insert_n_bug)
    {
      struct S
      {
        virtual ~S()
        {

        }

        S(int i_): i(i_){}

        int i;
      };

      const S raw[6] = { 1, 2, 3, 4, 5, 6 };

      etl::vector<S, 10> dest(ETL_OR_STD11::begin(raw), ETL_OR_STD11::end(raw));

      dest.insert(dest.begin(), 2, S(8));

      CHECK(dest.size() == 8);
      CHECK_EQUAL(8,        dest[0].i);
      CHECK_EQUAL(8,        dest[1].i);
      CHECK_EQUAL(raw[0].i, dest[2].i);
      CHECK_EQUAL(raw[1].i, dest[3].i);
      CHECK_EQUAL(raw[2].i, dest[4].i);
      CHECK_EQUAL(raw[3].i, dest[5].i);
      CHECK_EQUAL(raw[4].i, dest[6].i);
      CHECK_EQUAL(raw[5].i, dest[7].i);
    }

    //*************************************************************************
    TEST(test_two_parameter_constructor_same_type_not_iterator)
    {
      // No compilation error.
      etl::vector<int, 10> v(5, 5);
    }

    //*************************************************************************
    TEST(test_two_parameter_assign_same_type_not_iterator)
    {
      // No compilation error.
      etl::vector<int, 10> v;
      v.assign(5, 5);
    }

    //*************************************************************************
#include "etl/private/diagnostic_uninitialized_push.h"
    TEST(test_three_parameter_insert_same_type_not_iterator)
    {
      // No compilation error.
      etl::vector<int, 10> v;
      v.insert(v.end(), 5, 5);
    }
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    TEST(remove)
    {
      const int raw[10] = { 1, 8, 2, 7, 7, 7, 4, 5, 10, 9 };
      etl::vector<int, 10> data(ETL_OR_STD11::begin(raw), ETL_OR_STD11::end(raw));
      std::array<int, 7>   expected = { 1, 8, 2, 4, 5, 10, 9 };

      etl::erase(data, 7);

      CHECK_EQUAL(expected.size(), data.size());

      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
    TEST(remove_if)
    {
      const int raw[10] = { 1, 8, 2, 7, 7, 7, 4, 5, 10, 9 };
      etl::vector<int, 10> data(ETL_OR_STD11::begin(raw), ETL_OR_STD11::end(raw));
      std::array<int, 4>  expected = { 1, 2, 4, 5 };

      etl::erase_if(data, [](int value) { return value >= 7; });
            
      bool is_same = std::equal(expected.begin(), expected.end(), data.begin());
      CHECK(is_same);
    }

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST(test_make_vector)
    {
      auto data = etl::make_vector<char>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

      using Type = typename std::remove_reference<decltype(data[0])>::type;
      CHECK((std::is_same<int, Type>::value));

      CHECK_EQUAL(0, data[0]);
      CHECK_EQUAL(1, data[1]);
      CHECK_EQUAL(2, data[2]);
      CHECK_EQUAL(3, data[3]);
      CHECK_EQUAL(4, data[4]);
      CHECK_EQUAL(5, data[5]);
      CHECK_EQUAL(6, data[6]);
      CHECK_EQUAL(7, data[7]);
      CHECK_EQUAL(8, data[8]);
      CHECK_EQUAL(9, data[9]);
    }
#endif

    //*************************************************************************
    TEST(test_fill)
    {
      etl::vector<int, 10> data(initial_data.begin(), initial_data.end());

      data.fill(99);

      CHECK(std::equal(blank_data.begin(), blank_data.end(), data.begin()));
    }
  };
}
