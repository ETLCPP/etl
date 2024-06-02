/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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
#include <numeric>

#include "etl/vector.h"

namespace
{
  static const size_t SIZE = 10;

  int buffer1[SIZE];
  int buffer2[SIZE];
  int buffer3[SIZE];
  int buffer4[SIZE];
  int buffer5[SIZE];

  SUITE(test_vector_external_buffer)
  {
    typedef etl::vector_ext<int> Data;
    typedef etl::ivector<int>    IData;
    typedef std::vector<int>     Compare_Data;

    Compare_Data initial_data;
    Compare_Data less_data;
    Compare_Data greater_data;
    Compare_Data shorter_data;
    Compare_Data different_data;
    Compare_Data insert_data;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        int n[]         = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int n_insert[]  = { 11, 12, 13 };
        int n_less[]    = { 0, 1, 2, 3, 3, 5, 6, 7, 8, 9 };
        int n_greater[] = { 0, 1, 2, 4, 4, 5, 6, 7, 8, 9 };

        initial_data.assign(std::begin(n), std::end(n));
        insert_data.assign(std::begin(n_insert), std::end(n_insert));
        less_data.assign(std::begin(n_less), std::end(n_less));
        greater_data.assign(std::begin(n_greater), std::end(n_greater));
        shorter_data.assign(std::begin(n_greater), std::end(n_greater) - 1);
        different_data.assign(initial_data.rbegin(), initial_data.rend());

        std::fill_n(buffer1, SIZE, -1);
        std::fill_n(buffer2, SIZE, -1);
        std::fill_n(buffer3, SIZE, -1);
        std::fill_n(buffer4, SIZE, -1);
        std::fill_n(buffer5, SIZE, -1);
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_default_constructor)
    {
      Data data(buffer1, SIZE);

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.capacity(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
      CHECK(data.begin() == data.end());
    }

    //*************************************************************************
    TEST(test_iterator_comparison_empty)
    {
      Data data(buffer1, SIZE);

      CHECK(data.begin()   == data.end());
      CHECK(data.cbegin()  == data.cend());
      CHECK(data.rbegin()  == data.rend());
      CHECK(data.crbegin() == data.crend());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      const size_t INITIAL_SIZE = 5;
      Data data(INITIAL_SIZE, buffer1, SIZE);

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

      Data data(INITIAL_SIZE, INITIAL_VALUE, buffer1, SIZE);

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
      CHECK_THROW(Data data(SIZE + 1, buffer1, SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(test_constructor_initializer_list)
    {
      Compare_Data compare_data = { 0, 1, 2, 3 };
      std::initializer_list<int> il = { 0, 1, 2, 3 };
      Data data(il, buffer1, SIZE);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_constructor)
    {
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);
      Data data2(data, buffer2, SIZE);
      CHECK(data2 == data);

      data2[2] = -1;
      CHECK(data2 != data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_constructor)
    {
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);
      Data data2(std::move(data), buffer2, SIZE);
      CHECK(data.size() == 0);
      CHECK(data2.size() == initial_data.size());
      CHECK(data2 != data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);
      Data other_data(buffer2, SIZE);

      other_data = data;

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 other_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);
      Data other_data(buffer2, SIZE);

      other_data = std::move(data);

      CHECK(data.size() == 0);
      CHECK(other_data.size() == initial_data.size());
      CHECK(data != other_data);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_iterface)
    {
      Data data1(initial_data.begin(), initial_data.end(), buffer1, SIZE);
      Data data2(buffer2, SIZE);

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
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);
      Data other_data(data, buffer1, SIZE);

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
      Data data(10, buffer1, SIZE);
      const Data constData(10, buffer1, SIZE);

      CHECK_EQUAL(&data[0], data.begin());
      CHECK_EQUAL(&constData[0], constData.begin());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      Data data(10, buffer1, SIZE);
      const Data constData(10, buffer1, SIZE);

      CHECK_EQUAL(&data[10], data.end());
      CHECK_EQUAL(&constData[10], constData.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      Data data(INITIAL_SIZE, buffer1, SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;
      const int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE, buffer1, SIZE);
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

      Data data(INITIAL_SIZE, buffer1, SIZE);

      CHECK_THROW(data.resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      Data data(INITIAL_SIZE, buffer1, SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;
      const int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, buffer1, SIZE);
      data.resize(NEW_SIZE, INITIAL_VALUE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_up)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      Data data(INITIAL_SIZE, buffer1, SIZE);

      int* pbegin = &data.front();
      int* pend = &data.back() + 1;
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

      Data data(INITIAL_SIZE, buffer1, SIZE);

      CHECK_THROW(data.resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_down)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      Data data(INITIAL_SIZE, buffer1, SIZE);

      int* pbegin = &data.front();
      int* pend = &data.back() + 1;
      int* pmax = pbegin + data.max_size();

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
    TEST_FIXTURE(SetupFixture, test_uninitialized_resize_after_contruct)
    {
      int c_buffer[SIZE];
      std::iota(c_buffer, c_buffer + SIZE, 1);

      int compare_buffer[SIZE];
      std::iota(compare_buffer, compare_buffer + SIZE, 1);

      Data data(c_buffer, SIZE);
      data.uninitialized_resize(SIZE);

      CHECK_EQUAL(SIZE, data.size());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_buffer[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_empty)
    {
      Data data(buffer1, SIZE);
      data.resize(data.max_size());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_full)
    {
      Data data(buffer1, SIZE);

      CHECK(!data.full());
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

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
      const Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

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
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

      CHECK_EQUAL(compare_data.front(), data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

      CHECK_EQUAL(compare_data.front(), data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

      CHECK_EQUAL(compare_data.back(), data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

      CHECK_EQUAL(compare_data.back(), data.back());
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      bool is_equal = std::equal(data.data(),
                                 data.data() + data.size(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      bool is_equal = std::equal(data.data(),
                                 data.data() + data.size(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(buffer1, SIZE);

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

      Data data(buffer1, SIZE);
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

      Data data(buffer1, SIZE);

      CHECK_THROW(data.assign(EXCESS_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      Compare_Data compare_data;
      Data data(buffer1, SIZE);

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
      Data data(buffer1, SIZE);

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
      Data data(buffer1, SIZE);

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
      Data data(buffer1, SIZE);

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
      Data data(buffer1, SIZE);

      data.emplace_back(24);
      auto back = data.emplace_back(42);
      CHECK_EQUAL(back, data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

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
      Data data(buffer1, SIZE);

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
        Data data(buffer1, SIZE);

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);

        data.insert(data.begin() + offset, INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
#include "etl/private/diagnostic_array_bounds_push.h"
    TEST_FIXTURE(SetupFixture, test_insert_position_value_excess)
    {
      const size_t INITIAL_SIZE     = SIZE;
      const int INITIAL_VALUE       = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE, buffer1, SIZE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);
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
        Data data(buffer1, SIZE);

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
        compare_data.insert(compare_data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(),
                                   data.end(),
                                   compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t INSERT_SIZE  = 4;
      const int INITIAL_VALUE   = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE, buffer1, SIZE);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      const size_t INITIAL_SIZE = 5;

      for (size_t offset = 0; offset <= INITIAL_SIZE; ++offset)
      {
        Compare_Data compare_data;
        Data data(buffer1, SIZE);

        data.resize(SIZE, -1);

        data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        compare_data.assign(initial_data.begin(), initial_data.begin() + INITIAL_SIZE);
        data.insert(data.begin() + offset, insert_data.begin(), insert_data.end());
        compare_data.insert(compare_data.begin() + offset, insert_data.begin(), insert_data.end());

        CHECK_EQUAL(compare_data.size(), data.size());

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
      const int INITIAL_VALUE   = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE, buffer1, SIZE);

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
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

      compare_data.erase(compare_data.begin() + 2);

      data.erase(data.begin() + 2);

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
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

      compare_data.erase(compare_data.begin() + 2, compare_data.begin() + 4);

      data.erase(data.begin() + 2, data.begin() + 4);

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

      Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);
      data.clear();

      CHECK_EQUAL(data.size(), size_t(0));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      bool is_equal = std::equal(data.begin(),
                                 data.end(),
                                 compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      bool is_equal = std::equal(data.cbegin(),
                                 data.cend(),
                                 compare_data.cbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      bool is_equal = std::equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      bool is_equal = std::equal(data.crbegin(),
                                 data.crend(),
                                 compare_data.crbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator2)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end(), buffer1, SIZE);

      bool is_equal = std::equal(data.rbegin(),
                                 data.rend(),
                                 compare_data.rbegin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_equal)
    {
      const Data initial1(initial_data.begin(), initial_data.end(), buffer1, SIZE);
      const Data initial2(initial_data.begin(), initial_data.end(), buffer2, SIZE);

      CHECK(initial1 == initial2);

      const Data different(different_data.begin(), different_data.end(), buffer3, SIZE);

      CHECK(!(initial1 == different));

      const Data shorter(shorter_data.begin(), shorter_data.end(), buffer4, SIZE);

      CHECK(!(shorter == initial1));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_not_equal)
    {
      const Data initial1(initial_data.begin(), initial_data.end(), buffer1, SIZE);
      const Data initial2(initial_data.begin(), initial_data.end(), buffer2, SIZE);

      CHECK(!(initial1 != initial2));

      const Data different(different_data.begin(), different_data.end(), buffer3, SIZE);

      CHECK(initial1 != different);

      const Data shorter(shorter_data.begin(), shorter_data.end(), buffer4, SIZE);

      CHECK(shorter != initial1);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than)
    {
      const Data less(less_data.begin(), less_data.end(), buffer1, SIZE);
      const Data initial(initial_data.begin(), initial_data.end(), buffer2, SIZE);

      CHECK((less < initial) == (less_data < initial_data));

      const Data greater(greater_data.begin(), greater_data.end(), buffer3, SIZE);

      CHECK((greater < initial) == (greater_data < initial_data));

      const Data shorter(shorter_data.begin(), shorter_data.end(), buffer4, SIZE);

      CHECK((shorter < initial) == (shorter_data < initial_data));
      CHECK((initial < shorter) == (initial_data < shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_less_than_or_equal)
    {
      const Data less(less_data.begin(), less_data.end(), buffer1, SIZE);
      const Data initial(initial_data.begin(), initial_data.end(), buffer2, SIZE);

      CHECK((less <= initial) == (less_data <= initial_data));

      const Data greater(greater_data.begin(), greater_data.end(), buffer3, SIZE);

      CHECK((greater <= initial) == (greater_data <= initial_data));

      const Data shorter(shorter_data.begin(), shorter_data.end(), buffer4, SIZE);

      CHECK((shorter <= initial) == (shorter_data <= initial_data));
      CHECK((initial <= shorter) == (initial_data <= shorter_data));

      const Data initial2(initial_data.begin(), initial_data.end(), buffer5, SIZE);
      CHECK((initial <= initial2) == (initial_data <= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than)
    {
      const Data less(less_data.begin(), less_data.end(), buffer1, SIZE);
      const Data initial(initial_data.begin(), initial_data.end(), buffer2, SIZE);

      CHECK((less > initial) == (less_data > initial_data));

      const Data greater(greater_data.begin(), greater_data.end(), buffer3, SIZE);

      CHECK((greater > initial) == (greater_data > initial_data));

      const Data shorter(shorter_data.begin(), shorter_data.end(), buffer4, SIZE);

      CHECK((shorter > initial) == (shorter_data > initial_data));
      CHECK((initial > shorter) == (initial_data > shorter_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_greater_than_or_equal)
    {
      const Data less(less_data.begin(), less_data.end(), buffer1, SIZE);
      const Data initial(initial_data.begin(), initial_data.end(), buffer2, SIZE);

      CHECK((less >= initial) == (less_data >= initial_data));

      const Data greater(greater_data.begin(), greater_data.end(), buffer3, SIZE);

      CHECK((greater >= initial) == (greater_data >= initial_data));

      const Data shorter(shorter_data.begin(), shorter_data.end(), buffer4, SIZE);

      CHECK((shorter >= initial) == (shorter_data >= initial_data));
      CHECK((initial >= shorter) == (initial_data > shorter_data));

      const Data initial2(initial_data.begin(), initial_data.end(), buffer5, SIZE);
      CHECK((initial >= initial2) == (initial_data >= initial_data));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_memcpy_repair)
    {
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

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
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_memcpy_repair_virtual)
    {
      Data data(initial_data.begin(), initial_data.end(), buffer1, SIZE);

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

      S sbuffer1[SIZE] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
      S sbuffer2[SIZE] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

      const S raw[6] = { 1, 2, 3, 4, 5, 6 };

      etl::vector_ext<S> dest(std::begin(raw), std::end(raw), sbuffer1, SIZE);
      etl::vector_ext<S> src((size_t) 2, S(8), sbuffer2, SIZE);

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

      S sbuffer1[SIZE] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

      const S raw[6] = { 1, 2, 3, 4, 5, 6 };

      etl::vector_ext<S> dest(ETL_OR_STD11::begin(raw), ETL_OR_STD11::end(raw), sbuffer1, SIZE);

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
  };
}
