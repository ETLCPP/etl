/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 John Wellbelove

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

#include <algorithm>
#include <array>
#include <cstring>
#include <vector>

#include "etl/vector.h"

namespace
{
  SUITE(test_vector_member_function_pointer)
  {
    static const size_t SIZE = 10;

    struct Functions
    {
      size_t f0(size_t a)
      {
        return 0 * a;
      }
      size_t f1(size_t a)
      {
        return 1 * a;
      }
      size_t f2(size_t a)
      {
        return 2 * a;
      }
      size_t f3(size_t a)
      {
        return 3 * a;
      }
      size_t f4(size_t a)
      {
        return 4 * a;
      }
      size_t f5(size_t a)
      {
        return 5 * a;
      }
      size_t f6(size_t a)
      {
        return 6 * a;
      }
      size_t f7(size_t a)
      {
        return 7 * a;
      }
      size_t f8(size_t a)
      {
        return 8 * a;
      }
      size_t f9(size_t a)
      {
        return 9 * a;
      }
      size_t f10(size_t a)
      {
        return 10 * a;
      }
      size_t f11(size_t a)
      {
        return 11 * a;
      }
      size_t f12(size_t a)
      {
        return 12 * a;
      }
    };

    Functions functions;

    using MFPtr = size_t (Functions::*)(size_t);

    using Data         = etl::vector<MFPtr, SIZE>;
    using IData        = etl::ivector<MFPtr>;
    using Compare_Data = std::vector<MFPtr>;

    Compare_Data initial_data;
    Compare_Data less_data;
    Compare_Data greater_data;
    Compare_Data shorter_data;
    Compare_Data different_data;
    Compare_Data insert_data;
    Compare_Data swap_data;
    Compare_Data swap_other_data;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        MFPtr n[]            = {&Functions::f0, &Functions::f1, &Functions::f2, &Functions::f3, &Functions::f4,
                                &Functions::f5, &Functions::f6, &Functions::f7, &Functions::f8, &Functions::f9};
        MFPtr n_insert[]     = {&Functions::f10, &Functions::f11, &Functions::f12};
        MFPtr n_less[]       = {&Functions::f0, &Functions::f1, &Functions::f2, &Functions::f3, &Functions::f3,
                                &Functions::f5, &Functions::f6, &Functions::f7, &Functions::f8, &Functions::f9};
        MFPtr n_greater[]    = {&Functions::f0, &Functions::f1, &Functions::f2, &Functions::f4, &Functions::f4,
                                &Functions::f5, &Functions::f6, &Functions::f7, &Functions::f8, &Functions::f9};
        MFPtr n_swap[]       = {&Functions::f0, &Functions::f1, &Functions::f2, &Functions::f3, &Functions::f4, &Functions::f5};
        MFPtr n_swap_other[] = {&Functions::f6, &Functions::f7, &Functions::f8, &Functions::f9};

        initial_data.assign(std::begin(n), std::end(n));
        insert_data.assign(std::begin(n_insert), std::end(n_insert));
        less_data.assign(std::begin(n_less), std::end(n_less));
        greater_data.assign(std::begin(n_greater), std::end(n_greater));
        shorter_data.assign(std::begin(n_greater), std::end(n_greater) - 1);
        different_data.assign(initial_data.rbegin(), initial_data.rend());
        swap_data.assign(std::begin(n_swap), std::end(n_swap));
        swap_other_data.assign(std::begin(n_swap_other), std::end(n_swap_other));
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
      etl::vector             data{&Functions::f0, &Functions::f1, &Functions::f2, &Functions::f3, &Functions::f4,
                       &Functions::f5, &Functions::f6, &Functions::f7, &Functions::f8, &Functions::f9};
      etl::vector<MFPtr, 10U> check = {&Functions::f0, &Functions::f1, &Functions::f2, &Functions::f3, &Functions::f4,
                                       &Functions::f5, &Functions::f6, &Functions::f7, &Functions::f8, &Functions::f9};

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

      CHECK(data.begin() == data.end());
      CHECK(data.cbegin() == data.cend());
      CHECK(data.rbegin() == data.rend());
      CHECK(data.crbegin() == data.crend());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size)
    {
      const size_t Initial_Size = 5;
      Data         data(Initial_Size);

      CHECK(data.size() == Initial_Size);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constructor_size_value)
    {
      const size_t Initial_Size  = 5;
      MFPtr        Initial_Value = &Functions::f0;

      std::array<MFPtr, Initial_Size> compare_data;
      compare_data.fill(Initial_Value);

      Data data(Initial_Size, Initial_Value);

      CHECK(data.size() == Initial_Size);
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
    TEST_FIXTURE(SetupFixture, test_constructor_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

#if ETL_HAS_INITIALIZER_LIST
    //*************************************************************************
    TEST(test_constructor_initializer_list)
    {
      MFPtr a = &Functions::f0;
      MFPtr b = &Functions::f1;
      MFPtr c = &Functions::f2;
      MFPtr d = &Functions::f3;

      Compare_Data compare_data = {a, b, c, d};
      Data         data         = {a, b, c, d};

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
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data other_data;
      other_data.push_back(MFPtr(0));

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
    TEST_FIXTURE(SetupFixture, test_begin)
    {
      Data       data(10);
      const Data constData(10);

      CHECK_EQUAL(&data[0], data.begin());
      CHECK_EQUAL(&constData[0], constData.begin());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_end)
    {
      Data       data(10);
      const Data constData(10);

      CHECK_EQUAL(data.begin() + data.size(), data.end());
      CHECK_EQUAL(constData.begin() + constData.size(), constData.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up)
    {
      const size_t Initial_Size = 5;
      const size_t NEW_SIZE     = 8;

      Data data(Initial_Size);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_up_value)
    {
      const size_t Initial_Size  = 5;
      const size_t NEW_SIZE      = 8;
      MFPtr        Initial_Value = &Functions::f0;

      Data data(Initial_Size, Initial_Value);
      data.resize(NEW_SIZE, Initial_Value);

      std::array<MFPtr, NEW_SIZE> compare_data;
      compare_data.fill(Initial_Value);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_excess)
    {
      const size_t Initial_Size = 5;
      const size_t NEW_SIZE     = SIZE + 1;

      Data data(Initial_Size);

      CHECK_THROW(data.resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down)
    {
      const size_t Initial_Size = 5;
      const size_t NEW_SIZE     = 2;

      Data data(Initial_Size);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_resize_down_value)
    {
      const size_t Initial_Size  = 5;
      const size_t NEW_SIZE      = 2;
      MFPtr        Initial_Value = &Functions::f0;

      Data data(Initial_Size);
      data.resize(NEW_SIZE, Initial_Value);

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
        CHECK_TRUE(data[i] == compare_data[i]);
      }

      CHECK_THROW(data[data.size()], etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_TRUE(data[i] == compare_data[i]);
      }

      CHECK_THROW(data[data.size()], etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data         data(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_TRUE(data.at(i) == compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data         data(initial_data.begin(), initial_data.end());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_TRUE(data.at(i) == compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data         data(initial_data.begin(), initial_data.end());

      CHECK(data.front() == compare_data.front());

      Data emptyData;
      CHECK_THROW(emptyData.front(), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_front_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data         data(initial_data.begin(), initial_data.end());

      CHECK(data.front() == compare_data.front());

      const Data emptyData;
      CHECK_THROW(emptyData.front(), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data         data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == compare_data.back());

      Data emptyData;
      CHECK_THROW(emptyData.back(), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_back_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());
      const Data         data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == compare_data.back());

      const Data emptyData;
      CHECK_THROW(emptyData.back(), etl::vector_out_of_bounds);
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
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

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
    TEST_FIXTURE(SetupFixture, test_assign_size_value)
    {
      const size_t                    Initial_Size  = 5;
      MFPtr                           Initial_Value = &Functions::f0;
      std::array<MFPtr, Initial_Size> compare_data;
      compare_data.fill(Initial_Value);

      Data data;
      data.assign(Initial_Size, Initial_Value);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_size_value_excess)
    {
      const size_t                    Initial_Size  = SIZE;
      const size_t                    EXCESS_SIZE   = SIZE + 1;
      MFPtr                           Initial_Value = &Functions::f0;
      std::array<MFPtr, Initial_Size> compare_data;
      compare_data.fill(Initial_Value);

      Data data;

      CHECK_THROW(data.assign(EXCESS_SIZE, Initial_Value), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back)
    {
      Compare_Data compare_data;
      Data         data;

      MFPtr fptr = &Functions::f0;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        compare_data.push_back(fptr);
      }

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.push_back(fptr);
      }

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      Data data;

      MFPtr fptr = &Functions::f0;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.push_back(fptr);
      }

      CHECK_THROW(data.push_back(fptr), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back)
    {
      Compare_Data compare_data;
      Data         data;

      MFPtr fptr = &Functions::f0;

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        compare_data.emplace_back(fptr);
      }

      for (size_t i = 0UL; i < SIZE; ++i)
      {
        data.emplace_back(fptr);
      }

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_back_return)
    {
      Data  data;
      MFPtr fptr1 = &Functions::f0;
      MFPtr fptr2 = &Functions::f1;

      data.emplace_back(fptr1);
      CHECK_TRUE(fptr2 == data.emplace_back(fptr2));
    }

    //*************************************************************************
    TEST(test_emplace_default)
    {
      static MFPtr initial = &Functions::f0;

      // First fill with Initial values.
      etl::vector<MFPtr, SIZE> data;
      data.resize(SIZE, initial);
      data.clear();

      // Then emplace Default values.
      for (size_t i = 0; i < SIZE; ++i)
      {
        data.emplace(data.begin());
      }

      // Compare with an array of default values.
      std::array<MFPtr, SIZE> compare_data;
      compare_data.fill(nullptr);

      CHECK_TRUE(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(test_emplace_back_default)
    {
      static MFPtr initial = &Functions::f0;

      // First fill with initial values.
      etl::vector<MFPtr, SIZE> data;
      data.resize(SIZE, initial);
      data.clear();

      // Then emplace default values.
      for (size_t i = 0; i < SIZE; ++i)
      {
        data.emplace_back();
      }

      // Compare with an array of default values.
      std::array<MFPtr, SIZE> compare_data;
      compare_data.fill(nullptr);

      CHECK_TRUE(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_pop_back)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data         data(initial_data.begin(), initial_data.end());

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
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      const size_t Initial_Size  = 5;
      MFPtr        Initial_Value = &Functions::f0;

      for (size_t offset = 0; offset <= Initial_Size; ++offset)
      {
        Compare_Data compare_data;
        Data         data;

        data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
        compare_data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);

        data.insert(data.begin() + offset, Initial_Value);
        compare_data.insert(compare_data.begin() + static_cast<ptrdiff_t>(offset), Initial_Value);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
#include "etl/private/diagnostic_uninitialized_push.h"
    TEST_FIXTURE(SetupFixture, test_insert_position_value_outofbounds)
    {
      const size_t Initial_Size  = 5;
      MFPtr        Initial_Value = &Functions::f0;

      Data data;
      Data data2;
      data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
      CHECK_THROW(data.insert(data2.cbegin(), Initial_Value), etl::vector_out_of_bounds);
    }
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
#include "etl/private/diagnostic_array_bounds_push.h"
    TEST_FIXTURE(SetupFixture, test_insert_position_value_excess)
    {
      const size_t Initial_Size  = SIZE;
      MFPtr        Initial_Value = &Functions::f0;

      Data data(Initial_Size, Initial_Value);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, Initial_Value), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, Initial_Value), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, Initial_Value), etl::vector_full);
    }
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_emplace_position_value)
    {
      const size_t Initial_Size  = 5;
      MFPtr        Initial_Value = &Functions::f0;

      for (size_t offset = 0; offset <= Initial_Size; ++offset)
      {
        Compare_Data compare_data;
        Data         data;

        data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
        compare_data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);

        data.emplace(data.begin() + offset, Initial_Value);
        compare_data.emplace(compare_data.begin() + static_cast<ptrdiff_t>(offset), Initial_Value);

        CHECK_EQUAL(compare_data.size(), data.size());

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
#include "etl/private/diagnostic_uninitialized_push.h"
    TEST_FIXTURE(SetupFixture, test_emplace_position_value_outofbounds)
    {
      const size_t Initial_Size  = 5;
      MFPtr        Initial_Value = &Functions::f0;

      Data data;
      Data data2;
      data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
      CHECK_THROW(data.emplace(data2.cbegin(), Initial_Value), etl::vector_out_of_bounds);
    }
#include "etl/private/diagnostic_pop.h"

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value)
    {
      const size_t Initial_Size  = 5;
      const size_t INSERT_SIZE   = 3;
      MFPtr        Initial_Value = &Functions::f0;

      for (size_t offset = 0; offset <= Initial_Size; ++offset)
      {
        Compare_Data compare_data;
        Data         data;

        data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
        compare_data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
        data.insert(data.begin() + offset, INSERT_SIZE, Initial_Value);
        compare_data.insert(compare_data.begin() + static_cast<ptrdiff_t>(offset), INSERT_SIZE, Initial_Value);

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_outofbounds)
    {
      MFPtr Initial_Value = &Functions::f0;
      Data  data;
      Data  data2;

      CHECK_THROW(data.insert(data2.end(), 1, Initial_Value);, etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      const size_t Initial_Size  = SIZE;
      const size_t INSERT_SIZE   = 4;
      MFPtr        Initial_Value = &Functions::f0;

      Data data(Initial_Size, Initial_Value);

      size_t offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, Initial_Value), etl::vector_full);

      offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, Initial_Value), etl::vector_full);

      offset = 4;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, Initial_Value), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, Initial_Value), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      const size_t Initial_Size = 5;

      for (size_t offset = 0; offset <= Initial_Size; ++offset)
      {
        Compare_Data compare_data;
        Data         data;

        data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
        compare_data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
        data.insert(data.begin() + offset, insert_data.begin(), insert_data.end());
        compare_data.insert(compare_data.begin() + static_cast<ptrdiff_t>(offset), insert_data.begin(), insert_data.end());

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_out_of_bounds)
    {
      Data data;
      Data data2;

      CHECK_THROW(data.insert(data2.end(), insert_data.cbegin(), insert_data.cend());, etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_pointer_range)
    {
      const size_t Initial_Size = 5;

      for (size_t offset = 0; offset <= Initial_Size; ++offset)
      {
        Compare_Data compare_data;
        Data         data;

        data.assign(initial_data.data(), initial_data.data() + Initial_Size);
        compare_data.assign(initial_data.begin(), initial_data.begin() + Initial_Size);
        data.insert(data.data() + offset, insert_data.data(), insert_data.data() + insert_data.size());
        compare_data.insert(compare_data.begin() + static_cast<ptrdiff_t>(offset), insert_data.begin(), insert_data.end());

        bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

        CHECK(is_equal);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_pointer_range_out_of_bounds)
    {
      Data data;

      CHECK_THROW(data.insert(data.data() + data.size() + 1, insert_data.data(), insert_data.data() + insert_data.size());
                  , etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_excess)
    {
      const size_t Initial_Size  = 5;
      MFPtr        Initial_Value = &Functions::f0;

      Data data(Initial_Size, Initial_Value);

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
      Data         data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator citr = compare_data.erase(compare_data.begin() + 2);
      Data::iterator         ditr = data.erase(data.begin() + 2);

      CHECK(*citr == *ditr);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single_outofbounds)
    {
      Data data(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.erase(data.end());, etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data         data(initial_data.begin(), initial_data.end());

      Compare_Data::iterator citr = compare_data.erase(compare_data.cbegin() + 2, compare_data.cbegin() + 4);
      Data::iterator         ditr = data.erase(data.cbegin() + 2, data.cbegin() + 4);

      CHECK(*citr == *ditr);

      bool is_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(is_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range_outofbounds)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data data2(initial_data.begin(), initial_data.end());

      CHECK_THROW(data.erase(data2.begin(), data2.end());, etl::vector_out_of_bounds);
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
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

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
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator2)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

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
    TEST_FIXTURE(SetupFixture, test_const_ivector_of_pointer_to_pointer)
    {
      MFPtr                  i1 = &Functions::f0;
      etl::vector<MFPtr, 10> consttest;
      consttest.push_back(i1);
      const etl::ivector<MFPtr>& ct = consttest;

      MFPtr i2 = ct[0];

      CHECK(i1 == i2);
    }

    //*************************************************************************
#if ETL_HAS_INITIALIZER_LIST
    TEST(test_make_vector)
    {
      MFPtr values[] = {&Functions::f0, &Functions::f1, &Functions::f2, &Functions::f3, &Functions::f4,
                        &Functions::f5, &Functions::f6, &Functions::f7, &Functions::f8, &Functions::f9};

      auto data = etl::make_vector(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9]);

      CHECK_TRUE(&Functions::f0 == data[0]);
      CHECK_TRUE(&Functions::f1 == data[1]);
      CHECK_TRUE(&Functions::f2 == data[2]);
      CHECK_TRUE(&Functions::f3 == data[3]);
      CHECK_TRUE(&Functions::f4 == data[4]);
      CHECK_TRUE(&Functions::f5 == data[5]);
      CHECK_TRUE(&Functions::f6 == data[6]);
      CHECK_TRUE(&Functions::f7 == data[7]);
      CHECK_TRUE(&Functions::f8 == data[8]);
      CHECK_TRUE(&Functions::f9 == data[9]);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_same_capacity)
    {
      Data etl_data(swap_data.begin(), swap_data.end());
      Data etl_data2(swap_other_data.begin(), swap_other_data.end());

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_data.size());
      CHECK(etl_data.max_size() == SIZE);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_other_data.size());
      CHECK(etl_data2.max_size() == SIZE);

      etl_data.swap(etl_data2);

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_data.size());
      CHECK(etl_data2.max_size() == SIZE);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_other_data.size());
      CHECK(etl_data.max_size() == SIZE);

      etl_data.swap(etl_data2);

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_data.size());
      CHECK(etl_data.max_size() == SIZE);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_other_data.size());
      CHECK(etl_data2.max_size() == SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_different_capacity)
    {
      const size_t                   other_size = 6;
      Data                           etl_data(swap_data.begin(), swap_data.end());
      etl::vector<MFPtr, other_size> etl_data2(swap_other_data.begin(), swap_other_data.end());

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_data.size());
      CHECK(etl_data.max_size() == SIZE);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_other_data.size());
      CHECK(etl_data2.max_size() == other_size);

      etl_data.swap(etl_data2);

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_data.size());
      CHECK(etl_data2.max_size() == other_size);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_other_data.size());
      CHECK(etl_data.max_size() == SIZE);

      etl_data.swap(etl_data2);

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_data.size());
      CHECK(etl_data.max_size() == SIZE);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_other_data.size());
      CHECK(etl_data2.max_size() == other_size);
    }

    //*************************************************************************
    TEST(test_swap_insufficient_capacity)
    {
      etl::vector<MFPtr, 4> etl_data(4);
      etl::vector<MFPtr, 6> etl_data2(6);

      CHECK_THROW(etl_data.swap(etl_data2), etl::vector_full);
      CHECK_THROW(etl_data2.swap(etl_data), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_etl_swap_different_capacity)
    {
      const size_t                   other_size = 6;
      Data                           etl_data(swap_data.begin(), swap_data.end());
      etl::vector<MFPtr, other_size> etl_data2(swap_other_data.begin(), swap_other_data.end());

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_data.size());
      CHECK(etl_data.max_size() == SIZE);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_other_data.size());
      CHECK(etl_data2.max_size() == other_size);

      etl::swap(etl_data, etl_data2);

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_data.size());
      CHECK(etl_data2.max_size() == other_size);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_other_data.size());
      CHECK(etl_data.max_size() == SIZE);

      etl::swap(etl_data, etl_data2);

      CHECK(std::equal(swap_data.begin(), swap_data.end(), etl_data.begin()));
      CHECK(etl_data.size() == swap_data.size());
      CHECK(etl_data.max_size() == SIZE);
      CHECK(std::equal(swap_other_data.begin(), swap_other_data.end(), etl_data2.begin()));
      CHECK(etl_data2.size() == swap_other_data.size());
      CHECK(etl_data2.max_size() == other_size);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_functions_can_be_called)
    {
      Data data(initial_data.begin(), initial_data.end());

      for (size_t i = 0; i < data.size(); ++i)
      {
        MFPtr mfptr = data[i];
        CHECK_TRUE(i * i == (functions.*mfptr)(i));
      }
    }
  }
} // namespace
