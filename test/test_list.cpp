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

#include "../list.h"

#include <algorithm>
#include <array>
#include <list>
#include <vector>

namespace 
{		
  SUITE(TestList)
  {
    const size_t SIZE = 10;

    typedef etl::list<int, SIZE> Data;

    typedef std::list<int>   CompareData;
    typedef std::vector<int> InitialData;

    InitialData unsorted_data;
    InitialData sorted_data;
    InitialData non_unique_data;

    bool are_equal;

    //*************************************************************************
    struct SetupFixture
    {
      SetupFixture()
      {
        unsorted_data   = { 1, 0, 3, 2, 5, 4, 7, 6, 9, 8 };
        sorted_data     = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        non_unique_data = { 0, 0, 1, 1, 2, 3, 3, 3, 4, 5  };
      }
    };

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, DefaultConstructor)
    {
      Data data;

      CHECK_EQUAL(data.size(), size_t(0));
      CHECK(data.empty());
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstructorSize)
    {
      const size_t INITIAL_SIZE = 4;
      Data data(INITIAL_SIZE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstructorSizeExcess)
    {
      CHECK_THROW(Data data(SIZE + 1), etl::list_full_exception);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstructorSizeValue)
    {
      const size_t INITIAL_SIZE = 4;
      const int INITIAL_VALUE = 1;

      std::array<int, INITIAL_SIZE> compare_data;
      compare_data.fill(INITIAL_VALUE);

      Data data(INITIAL_SIZE, INITIAL_VALUE);

      CHECK(data.size() == INITIAL_SIZE);
      CHECK(!data.empty());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstructorRange)
    {
      Data data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(SIZE, data.size());
      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, CopyConstructor)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());
      Data other_data(data);

      CHECK_EQUAL(data.size(), other_data.size());

      CHECK(std::equal(data.begin(), data.end(), other_data.begin()));

      other_data.pop_front();
      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(compare_data.size() - 1, other_data.size());

      other_data.pop_back();
      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK_EQUAL(compare_data.size() - 2, other_data.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Iterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstIterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ReverseIterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ConstReverseIterator)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(compare_data.begin(), compare_data.end());

      are_equal = std::equal(data.crbegin(), data.crend(), compare_data.crbegin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ResizeUp)
    {
      const size_t INITIAL_SIZE = 4;
      const size_t NEW_SIZE     = 8;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ResizeUpValue)
    {
      const size_t INITIAL_SIZE = 4;
      const size_t NEW_SIZE     = 8;
      const int VALUE = 1;

      Data data(INITIAL_SIZE, VALUE);
      data.resize(NEW_SIZE, VALUE);

      CompareData compare_data(INITIAL_SIZE, VALUE);
      compare_data.resize(NEW_SIZE, VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ResizeExcess)
    {
      const size_t INITIAL_SIZE = 4;
      Data data(INITIAL_SIZE);

      CHECK_THROW(data.resize(data.max_size() + 1), etl::list_full_exception);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, ResizeDown)
    {
      const size_t INITIAL_SIZE = 4;
      const size_t NEW_SIZE     = 2;

      Data data(INITIAL_SIZE);
      data.resize(NEW_SIZE);

      CHECK_EQUAL(data.size(), NEW_SIZE);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Clear)
    {
      Data data(sorted_data.begin(), sorted_data.end());
      data.clear();

      CHECK_EQUAL(data.size(), 0);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, AssignRange)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data;

      // Do it twice. We should only get one copy.
      data.assign(compare_data.begin(), compare_data.end());
      data.assign(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, AssignSizeValue)
    {
      const size_t INITIAL_SIZE = 4;
      const int VALUE = 1;

      CompareData compare_data(INITIAL_SIZE, VALUE);
      Data data;

      // Do it twice. We should only get one copy.
      data.assign(INITIAL_SIZE, VALUE);
      data.assign(INITIAL_SIZE, VALUE);

      CHECK_EQUAL(compare_data.size(), data.size());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, AssignSizeValueExcess)
    {
      const size_t EXCESS_SIZE = SIZE + 1;
      const int VALUE = 1;

      Data data;

      CHECK_THROW(data.assign(data.max_size() + 1, VALUE), etl::list_full_exception);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, InsertPositionValue)
    {
      const size_t INITIAL_SIZE = 4;
      const int VALUE = 1;
      const int INSERT_VALUE = 2;

      CompareData compare_data(INITIAL_SIZE, VALUE);
      Data data(INITIAL_SIZE, VALUE);

      size_t offset = 2;

      Data::iterator i_data = data.begin();
      std::advance(i_data, offset);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert(i_data, INSERT_VALUE);
      compare_data.insert(i_compare_data, INSERT_VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      offset = 0;

      i_data = data.begin();
      std::advance(i_data, offset);

      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, offset);

      data.insert(i_data, INSERT_VALUE);
      compare_data.insert(i_compare_data, INSERT_VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      i_data     = data.end();
      i_compare_data = compare_data.end();

      data.insert(i_data, VALUE);
      compare_data.insert(i_compare_data, VALUE);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, InsertRange)
    {
      std::vector<int> test1 = { 1, 2, 3, 4, 5 };
      std::vector<int> test2 = { 6, 7, 8, 9, 10 };

      CompareData compare_data(test1.begin(), test1.end());
      Data data(test1.begin(), test1.end());

      compare_data.insert(compare_data.begin(), test2.begin(), test2.end());
      data.insert(data.begin(), test2.begin(), test2.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      compare_data.assign(test1.begin(), test1.end());
      data.assign(test1.begin(), test1.end());

      compare_data.insert(compare_data.end(), test2.begin(), test2.end());
      data.insert(data.end(), test2.begin(), test2.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);

      compare_data.assign(test1.begin(), test1.end());
      data.assign(test1.begin(), test1.end());

      CompareData::iterator icd = compare_data.begin();
      Data::iterator         id = data.begin();

      std::advance(icd, 3);
      std::advance(id, 3);

      compare_data.insert(icd, test2.begin(), test2.end());
      data.insert(id, test2.begin(), test2.end());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushFront)
    {
      CompareData compare_data;
      Data data;

      compare_data.push_front(1);
      compare_data.push_front(2);
      compare_data.push_front(3);
      compare_data.push_front(4);
      compare_data.push_front(5);
      compare_data.push_front(6);

#ifdef CHECK_NO_THROW
      CHECK_NO_THROW(data.push_front(1));
      CHECK_NO_THROW(data.push_front(2));
      CHECK_NO_THROW(data.push_front(3));
      CHECK_NO_THROW(data.push_front(4));
      CHECK_NO_THROW(data.push_front(5));
      CHECK_NO_THROW(data.push_front(6));
#else
      data.push_front(1);
      data.push_front(2);
      data.push_front(3);
      data.push_front(4);
      data.push_front(5);
      data.push_front(6);
#endif

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushFrontExcess)
    {  
      bool gotException = false;
      Data data;

#ifdef CHECK_NO_THROW
      CHECK_NO_THROW(data.push_front(1));
      CHECK_NO_THROW(data.push_front(2));
      CHECK_NO_THROW(data.push_front(3));
      CHECK_NO_THROW(data.push_front(4));
      CHECK_NO_THROW(data.push_front(5));
      CHECK_NO_THROW(data.push_front(6));
      CHECK_NO_THROW(data.push_front(7));
      CHECK_NO_THROW(data.push_front(8));
      CHECK_NO_THROW(data.push_front(9));
      CHECK_NO_THROW(data.push_front(10));
#else
      data.push_front(1);
      data.push_front(2);
      data.push_front(3);
      data.push_front(4);
      data.push_front(5);
      data.push_front(6);
      data.push_front(7);
      data.push_front(8);
      data.push_front(9);
      data.push_front(10);
#endif

      CHECK_THROW(data.push_front(11) , etl::list_full_exception);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushFrontPopFront)
    {
      Data data;

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
#ifdef CHECK_NO_THROW
        CHECK_NO_THROW(data.push_front(i));
#else
        data.push_back(i);
#endif
        data.pop_front();
      }
      
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushFrontPopBack)
    {
      Data data;

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
#ifdef CHECK_NO_THROW
        CHECK_NO_THROW(data.push_front(i));
#else
        data.push_back(i);
#endif
        data.pop_back();
      }

      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushBackPopFront)
    {
      Data data;

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
#ifdef CHECK_NO_THROW
        CHECK_NO_THROW(data.push_front(i));
#else
        data.push_back(i);
#endif
        data.pop_front();
      }
      
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushBackPopBack)
    {
      Data data;

      for (size_t i = 0; i < 2 * data.max_size(); ++i)
      {
#ifdef CHECK_NO_THROW
        CHECK_NO_THROW(data.push_front(i));
#else
        data.push_back(i);
#endif
        data.pop_back();
      }
      
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushBack)
    {
      CompareData compare_data;
      Data data;

      compare_data.push_back(1);
      compare_data.push_back(2);
      compare_data.push_back(3);
      compare_data.push_back(4);
      compare_data.push_back(5);
      compare_data.push_back(6);

#ifdef CHECK_NO_THROW
      CHECK_NO_THROW(data.push_back(1));
      CHECK_NO_THROW(data.push_back(2));
      CHECK_NO_THROW(data.push_back(3));
      CHECK_NO_THROW(data.push_back(4));
      CHECK_NO_THROW(data.push_back(5));
      CHECK_NO_THROW(data.push_back(6));
#else
      data.push_back(1);
      data.push_back(2);
      data.push_back(3);
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
#endif

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }


    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PushBackExcess)
    {
      bool gotException = false;
      Data data;

      data.push_back(1);
      data.push_back(2);
      data.push_back(3);
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);
      data.push_back(9);
      data.push_back(10);

      CHECK_THROW(data.push_back(11), etl::list_full_exception);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, PopBack)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());

      compare_data.pop_back();
      compare_data.pop_back();

      data.pop_back();
      data.pop_back();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, EraseSingle)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());

      Data::iterator i_data = data.begin();
      std::advance(i_data, 2);

      CompareData::iterator i_compare_data = compare_data.begin();
      std::advance(i_compare_data, 2);

      i_compare_data = compare_data.erase(i_compare_data);
      i_data         = data.erase(i_data);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(*i_compare_data == *i_data);

      i_compare_data = compare_data.erase(compare_data.begin());
      i_data         = data.erase(data.begin());

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(i_data == data.begin());

      // Move to the last value and erase.
      i_compare_data = compare_data.begin();
      std::advance(i_compare_data, compare_data.size() - 1);
      i_compare_data = compare_data.erase(i_compare_data);

      i_data = data.begin();
      std::advance(i_data, data.size() - 1);
      i_data = data.erase(i_data);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
      CHECK(i_data == data.end());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, EraseRange)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());

      Data::iterator i_data_1 = data.begin();
      std::advance(i_data_1, 2);

      Data::iterator i_data_2 = data.begin();
      std::advance(i_data_2, 4);

      CompareData::iterator i_compare_data_1 = compare_data.begin();
      std::advance(i_compare_data_1, 2);

      CompareData::iterator i_compare_data_2 = compare_data.begin();
      std::advance(i_compare_data_2, 4);

      compare_data.erase(i_compare_data_1, i_compare_data_2);

      data.erase(i_data_1, i_data_2);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, EraseAll)
    {
      Data data(sorted_data.begin(), sorted_data.end());

      data.erase(data.begin(), data.end());

      CHECK(data.empty());

      // Check that it is still in a valid state.
      data.push_back(1);
      CHECK(!data.empty());
      CHECK_EQUAL(1, data.size());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Front)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(compare_data.front(), data.front());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Back)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());

      CHECK_EQUAL(compare_data.back(), data.back());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Assignment)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());
      Data other_data;

      other_data = data;

      CHECK_EQUAL(data.size(), other_data.size());

      are_equal = std::equal(data.begin(), data.end(), other_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Unique)
    {
      CompareData compare_data(non_unique_data.begin(), non_unique_data.end());
      Data data(non_unique_data.begin(), non_unique_data.end());

      compare_data.unique();
      data.unique();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Remove)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());

      compare_data.remove(7);
      data.remove(7);

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, RemoveIf)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());

      compare_data.remove_if(std::bind2nd(std::equal_to<int>(), 7));
      data.remove_if(std::bind2nd(std::equal_to<int>(), 7));

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Reverse)
    {
      CompareData compare_data(sorted_data.begin(), sorted_data.end());
      Data data(sorted_data.begin(), sorted_data.end());

      compare_data.reverse();
      data.reverse();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, Sort)
    {
      CompareData compare_data(unsorted_data.begin(), unsorted_data.end());
      Data data(unsorted_data.begin(), unsorted_data.end());

      compare_data.sort();
      data.sort();

      are_equal = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(are_equal);
    }
  };
}