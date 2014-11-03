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
  SUITE(TestVector)
  {
    static const size_t SIZE = 10;

    typedef etl::vector<int, SIZE>  Data;
    typedef std::vector<int>        TestData;

    TestData initial_data;
    TestData less_data;
    TestData greater_data;
    TestData shorter_data;
    TestData different_data;

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
    TEST_FIXTURE(SetupFixture, DefaultConstructor)
    {
      Data data;

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.capacity(), SIZE);
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstructorSize)
    {
      const size_t INITIAL_SIZE = 5;
      Data data(INITIAL_SIZE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstructorSizeValue)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;

      std::array<int, INITIAL_SIZE> testData;
      testData.fill(INITIAL_VALUE);

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstructorSizeExcess)
    {
      CHECK_THROW(Data data(SIZE + 1), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstructorRange)
    {
      TestData testData(initial_data.begin(), initial_data.end());

      Data data(testData.begin(), testData.end());

      CHECK(data.size() == SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Iterator)
    {
      TestData testData(initial_data.begin(), initial_data.end());

      Data data(testData.begin(), testData.end());

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstIterator)
    {
      TestData testData(initial_data.begin(), initial_data.end());

      Data data(testData.begin(), testData.end());

      bool isEqual = std::equal(data.cbegin(),
                                data.cend(),
                                testData.cbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ReverseIterator)
    {
      TestData testData(initial_data.begin(), initial_data.end());

      Data data(testData.begin(), testData.end());

      bool isEqual = std::equal(data.rbegin(),
                                data.rend(),
                                testData.rbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstReverseIterator)
    {
      TestData testData(initial_data.begin(), initial_data.end());

      Data data(testData.begin(), testData.end());

      bool isEqual = std::equal(data.crbegin(),
                                data.crend(),
                                testData.crbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ResizeUp)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ResizeUpValue)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 8;
      const int INITIAL_VALUE = 1;

      Data data(INITIAL_SIZE, INITIAL_VALUE);
      data.resize(NEW_SIZE, INITIAL_VALUE);

      std::array<int, NEW_SIZE> testData;
      testData.fill(INITIAL_VALUE);

      bool isEqual = std::equal(data.begin(), data.end(), testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ResizeExcess)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = SIZE + 1;

      Data data(INITIAL_SIZE);

      CHECK_THROW(data.resize(NEW_SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ResizeDown)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t NEW_SIZE = 2;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Indexing)
    {
      const size_t INITIAL_SIZE = 5;
      TestData testData(initial_data.begin(), initial_data.end());

      Data data(testData.begin(), testData.end());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], testData[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, IndexingConst)
    {
      const size_t INITIAL_SIZE = 5;
      const TestData testData(initial_data.begin(), initial_data.end());

      const Data data(testData.begin(), testData.end());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], testData[i]);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Clear)
    {
      const size_t INITIAL_SIZE = 5;
      TestData testData(initial_data.begin(), initial_data.end());

      Data data(testData.begin(), testData.end());
      data.clear();

      CHECK_EQUAL(data.size(), size_t(0));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, AssignRange)
    {
      const size_t INITIAL_SIZE = 5;
      TestData testData(initial_data.begin(), initial_data.end());

      Data data;

      data.assign(testData.begin(), testData.end());

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, AssignSizeValue)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;
      std::array<int, INITIAL_SIZE> testData;
      testData.fill(INITIAL_VALUE);

      Data data;
      data.assign(INITIAL_SIZE, INITIAL_VALUE);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, AssignSizeValueExcess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t EXCESS_SIZE = SIZE + 1;
      const int INITIAL_VALUE = 1;
      std::array<int, INITIAL_SIZE> testData;
      testData.fill(INITIAL_VALUE);

      Data data;
      
      CHECK_THROW(data.assign(EXCESS_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, InsertPositionValue)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;
      const int INSERT_VALUE = 2;

      TestData testData(INITIAL_SIZE, INITIAL_VALUE);
      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      data.insert(data.begin() + offset, INITIAL_VALUE);
      testData.insert(testData.begin() + offset, INITIAL_VALUE);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);

      offset = 0;

      data.insert(data.begin() + offset, INITIAL_VALUE);
      testData.insert(testData.begin() + offset, INITIAL_VALUE);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           testData.begin());

      CHECK(isEqual);

      offset = data.size();

      data.insert(data.begin() + offset, INITIAL_VALUE);
      testData.insert(testData.begin() + offset, INITIAL_VALUE);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, InsertPositionValueExcess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const int INITIAL_VALUE = 1;
      const int INSERT_VALUE = 2;

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = 0;

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, InsertPositionNValue)
    {
      const size_t INITIAL_SIZE = 5;
      const size_t INSERT_SIZE  = 3;
      const int INITIAL_VALUE   = 1;
      const int INSERT_VALUE    = 2;

      TestData testData;
      Data data;

      size_t offset = 2;

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      testData.assign(INITIAL_SIZE, INITIAL_VALUE);
      data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
      testData.insert(testData.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);

      offset = 0;

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      testData.assign(INITIAL_SIZE, INITIAL_VALUE);
      data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
      testData.insert(testData.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           testData.begin());

      CHECK(isEqual);

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      testData.assign(INITIAL_SIZE, INITIAL_VALUE);
      offset = data.size();
      data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE);
      testData.insert(testData.begin() + offset, INSERT_SIZE, INITIAL_VALUE);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, InsertPositionNValueExcess)
    {
      const size_t INITIAL_SIZE = SIZE;
      const size_t INSERT_SIZE = 4;
      const int INITIAL_VALUE = 1;
      const int INSERT_VALUE = 2;

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      size_t offset = 2;

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = 0;
     
      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);

      offset = data.size();

      CHECK_THROW(data.insert(data.begin() + offset, INSERT_SIZE, INITIAL_VALUE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, InsertPositionRange)
    {
      const size_t INITIAL_SIZE = 5;
      const int INITIAL_VALUE = 1;

      TestData testData;
      Data data;

      size_t offset = 2;

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      testData.assign(INITIAL_SIZE, INITIAL_VALUE);
      data.insert(data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);
      testData.insert(testData.begin() + offset, initial_data.begin(), initial_data.begin() + 3);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);

      offset = 0;

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      testData.assign(INITIAL_SIZE, INITIAL_VALUE);
      data.insert(data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);
      testData.insert(testData.begin() + offset, initial_data.begin(), initial_data.begin() + 3);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           testData.begin());

      CHECK(isEqual);

      data.assign(INITIAL_SIZE, INITIAL_VALUE);
      testData.assign(INITIAL_SIZE, INITIAL_VALUE);
      offset = data.size();
      data.insert(data.begin() + offset, initial_data.begin(), initial_data.begin() + 3);
      testData.insert(testData.begin() + offset, initial_data.begin(), initial_data.begin() + 3);

      isEqual = std::equal(data.begin(),
                           data.end(),
                           testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, InsertPositionRangeExcess)
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
    TEST_FIXTURE(SetupFixture, PushBack)
    {
      TestData testData;
      Data data;

      for (size_t i = 0; i < SIZE; ++i)
      {
        testData.push_back(i);
      }

      for (size_t i = 0; i < SIZE; ++i)
      {
        data.push_back(i);
      }

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushBackNull)
    {
      TestData testData;
      Data data;

      testData.push_back(1);

      data.push_back();
      data[0] = 1;

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushBackExcess)
    {
      Data data;
    
      for (size_t i = 0; i < SIZE; ++i)
      {
        data.push_back(i);
      }

      CHECK_THROW(data.push_back(SIZE), etl::vector_full);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PopBack)
    {
      TestData testData(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      testData.pop_back();
      testData.pop_back();

      data.pop_back();
      data.pop_back();

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, EraseSingle)
    {
      TestData testData(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      testData.erase(testData.begin() + 2);

      data.erase(data.begin() + 2);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, EraseRange)
    {
      TestData testData(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      testData.erase(testData.begin() + 2, testData.begin() + 4);

      data.erase(data.begin() + 2, data.begin() + 4);

      bool isEqual = std::equal(data.begin(),
                                data.end(),
                                testData.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Front)
    {
      TestData testData(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK(data.front()== testData.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Back)
    {
      TestData testData(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());

      CHECK(data.back() == testData.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Assignment)
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
    TEST_FIXTURE(SetupFixture, At)
    {
      TestData testData(initial_data.begin(), initial_data.end());
      Data data(initial_data.begin(), initial_data.end());


    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, AtConst)
    {
      const TestData testData(initial_data.begin(), initial_data.end());
      const Data data(initial_data.begin(), initial_data.end());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), testData.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::vector_out_of_bounds);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Full)
    {
      Data data;
      data.resize(data.max_size());

      CHECK(data.full());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Begin)
    {
      Data data(10);
      const Data constData(10);

      CHECK_EQUAL(data.begin(), std::begin(data));
      CHECK_EQUAL(constData.begin(), std::begin(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, End)
    {
      Data data(10);
      const Data constData(10);

      CHECK_EQUAL(data.end(), std::end(data));
      CHECK_EQUAL(constData.end(), std::end(constData));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Equal)
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
    TEST_FIXTURE(SetupFixture, NotEqual)
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
    TEST_FIXTURE(SetupFixture, LessThan)
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
    TEST_FIXTURE(SetupFixture, LessThanEqual)
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
    TEST_FIXTURE(SetupFixture, GreaterThan)
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
    TEST_FIXTURE(SetupFixture, GreaterThanEqual)
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
  };
}
