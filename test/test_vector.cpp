/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#include <UnitTest++/UnitTest++.h>

#include <vector>
#include <array>
#include <algorithm>

#include "../vector.h"

namespace
{		
  SUITE(test_vector)
  {
    static const size_t SIZE = 10;

    typedef etl::vector<int, SIZE>  Data;
    typedef std::vector<int>        Compare_Data;

    Compare_Data initial_data;
    Compare_Data less_data;
    Compare_Data greater_data;
    Compare_Data shorter_data;
    Compare_Data different_data;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        int n[]         = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        int n_less[]    = { 0, 1, 2, 3, 3, 5, 6, 7, 8, 9 };
        int n_greater[] = { 0, 1, 2, 4, 4, 5, 6, 7, 8, 9 };

        initial_data.assign(std::begin(n), std::end(n));
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

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
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

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Data data(initial_data.begin(), initial_data.end());
      Data otherData;

      otherData = data;

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                otherData.begin());

      CHECK(isEqual);
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

      bool isEqual = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
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

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_index_const)
    {
      const Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_at)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      for (size_t i = 0; i < data.size(); ++i)
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

      for (size_t i = 0; i < data.size(); ++i)
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

      bool isEqual = std::equal(data.data(),
        data.data() + data.size(),
        compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_data_const)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      const Data data(compare_data.begin(), compare_data.end());

      bool isEqual = std::equal(data.data(),
                                data.data() + data.size(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assign_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data;

      data.assign(compare_data.begin(), compare_data.end());

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
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

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
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

      for (size_t i = 0; i < SIZE; ++i)
      {
        compare_data.push_back(i);
      }

      for (size_t i = 0; i < SIZE; ++i)
      {
        data.push_back(i);
      }

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_null)
    {
      Compare_Data compare_data;
      Data data;

      compare_data.push_back(1);

      data.push_back();
      data[0] = 1;

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_push_back_excess)
    {
      Data data;

      for (size_t i = 0; i < SIZE; ++i)
      {
        data.push_back(i);
      }

      CHECK_THROW(data.push_back(SIZE), etl::vector_full);
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

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;

      Compare_Data compare_data(INITIAL_SIZE, INITIAL_VALUE);
      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      data.insert(data.begin() + offset, INITIAL_VALUE);
      compare_data.insert(compare_data.begin() + offset, INITIAL_VALUE);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);

      offset = 0;

      data.insert(data.begin() + offset, INITIAL_VALUE);
      compare_data.insert(compare_data.begin() + offset, INITIAL_VALUE);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           compare_data.begin());

      CHECK(isEqual);

      offset = data.size();

      data.insert(data.begin() + offset, INITIAL_VALUE);
      compare_data.insert(compare_data.begin() + offset, INITIAL_VALUE);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t INSERT_SIZE = 3;
      const int INITIAL_VALUE = 1;

      Compare_Data compare_data;
      Data data;

      size_t offset = 2;

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_data.assign(INITIAL_SIZE, INITIAL_VALUE);
      data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
      compare_data.insert(compare_data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);

      offset = 0;

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_data.assign(INITIAL_SIZE, INITIAL_VALUE);
      data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
      compare_data.insert(compare_data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           compare_data.begin());

      CHECK(isEqual);

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_data.assign(INITIAL_SIZE, INITIAL_VALUE);
      offset = data.size();
      data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
      compare_data.insert(compare_data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_n_value_excess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t INSERT_SIZE = 4;
      const int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;

      Compare_Data compare_data;
      Data data;

      size_t offset = 2;

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_data.assign(INITIAL_SIZE, INITIAL_VALUE);
      data.insert(data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);
      compare_data.insert(compare_data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);

      offset = 0;

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_data.assign(INITIAL_SIZE, INITIAL_VALUE);
      data.insert(data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);
      compare_data.insert(compare_data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           compare_data.begin());

      CHECK(isEqual);

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      compare_data.assign(INITIAL_SIZE, INITIAL_VALUE);
      offset = data.size();
      data.insert(data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);
      compare_data.insert(compare_data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_insert_position_range_excess)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, initial_data.begin(), initial_data.end()), etl::vector_full);
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_single)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      compare_data.erase(compare_data.begin() + 2);

      data.erase(data.begin() + 2);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_erase_range)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      compare_data.erase(compare_data.begin() + 2, compare_data.begin() + 4);

      data.erase(data.begin() + 2, data.begin() + 4);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
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

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool isEqual = std::equal(data.cbegin(),
                                data.cend(),
                                compare_data.cbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_reverse_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool isEqual = std::equal(data.rbegin(),
                                data.rend(),
                                compare_data.rbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_const_reverse_iterator)
    {
      Compare_Data compare_data(initial_data.begin(), initial_data.end());

      Data data(compare_data.begin(), compare_data.end());

      bool isEqual = std::equal(data.crbegin(),
                                data.crend(),
                                compare_data.crbegin());

      CHECK(isEqual);
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
    TEST(test_swap)
    {
      std::vector<int> first = { 1, 2, 3, 4, 5, 6 };
      std::vector<int> second = { 6, 5, 4 };

      Data data1(first.begin(), first.end());
      Data data2(second.begin(), second.end());

      swap(data1, data2);

      CHECK_EQUAL(second.size(), data1.size());
      CHECK_EQUAL(first.size(), data2.size());

      CHECK_EQUAL(std::distance(second.begin(), second.end()), std::distance(data1.begin(), data1.end()));
      CHECK_EQUAL(std::distance(first.begin(), first.end()), std::distance(data2.begin(), data2.end()));

      CHECK(std::equal(second.begin(), second.end(), data1.begin()));
      CHECK(std::equal(first.begin(), first.end(), data2.begin()));
    }
  };
}
