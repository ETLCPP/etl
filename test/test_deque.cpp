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

#include "../deque.h"

#include <vector>
#include <algorithm>

const size_t SIZE = 6;

typedef etl::deque<int, SIZE> Data;
typedef std::vector<int>      CompareData;

namespace FixedDequeTest
{		
	SUITE(TestDeque)
	{
    //*************************************************************************
		TEST(Contruct)
		{
      Data data;

      CHECK_EQUAL(SIZE, data.capacity());
      CHECK_EQUAL(SIZE, data.max_size());
		}

    //*************************************************************************
    TEST(ContructFill)
    {
      CompareData compare_data = { 5, 5, 5, 5, 5, 5};
      Data data(SIZE, 5);

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(ContructFillExcess)
    {
      CHECK_THROW(Data(SIZE + 1, 5), etl::deque_full);
    }

    //*************************************************************************
    TEST(ContructRange)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(ContructRangeExcess)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5, 6, 7 };
      
      CHECK_THROW(Data data(compare_data.begin(), compare_data.end()), etl::deque_full);
    }

    //*************************************************************************
		TEST(CopyContruct)
		{
      CompareData original = { 1, 2, 3, 4, 5, 6 };
      CompareData modified = { 6, 5, 4, 3, 2, 1 };
     
      Data testDeque1(original.begin(), original.end());
      Data testDeque2(testDeque1);

      CHECK_EQUAL(testDeque1.size(), testDeque2.size());
      CHECK(std::equal(original.begin(), original.end(), testDeque2.begin()));

      // Change testDeque2's data.
      std::copy(modified.begin(), modified.end(), testDeque2.begin());
      CHECK(std::equal(original.begin(), original.end(), testDeque1.begin()));
      CHECK(std::equal(modified.begin(), modified.end(), testDeque2.begin()));
		}

    //*************************************************************************
		TEST(Assignment)
		{
      CompareData original = { 1, 2, 3, 4, 5, 6 };
      CompareData modified = { 6, 5, 4, 3, 2, 1 };
     
      Data testDeque1(original.begin(), original.end());
      Data testDeque2;
      
      testDeque2 = testDeque1;

      CHECK_EQUAL(testDeque1.size(), testDeque2.size());
      CHECK(std::equal(original.begin(), original.end(), testDeque2.begin()));

      // Change testDeque2's data.
      std::copy(modified.begin(), modified.end(), testDeque2.begin());
      CHECK(std::equal(original.begin(), original.end(), testDeque1.begin()));
      CHECK(std::equal(modified.begin(), modified.end(), testDeque2.begin()));
		}

    //*************************************************************************
		TEST(StdSwap)
		{
      CompareData compare_data1 = { 1, 2, 3, 4, 5, 6 };
      CompareData compare_data2 = { 6, 5, 4 };
     
      Data data1(compare_data1.begin(), compare_data1.end());
      Data data2(compare_data2.begin(), compare_data2.end());

      std::swap(data1, data2);

      CHECK_EQUAL(compare_data2.size(), data1.size());
      CHECK_EQUAL(compare_data1.size(), data2.size());

      CHECK_EQUAL(std::distance(compare_data2.begin(), compare_data2.end()), std::distance(data1.begin(), data1.end()));
      CHECK_EQUAL(std::distance(compare_data1.begin(), compare_data1.end()), std::distance(data2.begin(), data2.end()));
      
      CHECK(std::equal(compare_data2.begin(), compare_data2.end(), data1.begin()));
      CHECK(std::equal(compare_data1.begin(), compare_data1.end(), data2.begin()));
		}

    //*************************************************************************
    TEST(Clear)
    {
      Data data;

      data.resize(SIZE);
      data.clear();
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST(AssignFromRange)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data;

      CHECK_NO_THROW(data.assign(compare_data.begin(), compare_data.end()));

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(AssignFromRangeExcess)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5, 6, 7 };
      Data data;

      CHECK_THROW(data.assign(compare_data.begin(), compare_data.end()), etl::deque_full);
    }

    //*************************************************************************
    TEST(AssignFill)
    {
      CompareData compare_data = { 5, 5, 5, 5, 5, 5 };

      Data data;

      CHECK_NO_THROW(data.assign(SIZE, 5));

      CHECK_EQUAL(compare_data.size(), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(AssignFillExcess)
    {
      Data data;

      CHECK_THROW(data.assign(SIZE + 1, 5), etl::deque_full);
    }

    //*************************************************************************
    TEST(PushBackNoValue)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data;

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 1);
      CHECK_EQUAL(size_t(1), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 2);
      CHECK_EQUAL(size_t(2), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 3);
      CHECK_EQUAL(size_t(3), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 4);
      CHECK_EQUAL(size_t(4), data.size());

      CHECK_NO_THROW(data.push_back());
      CHECK_NO_THROW(data.back() = 5);
      CHECK_EQUAL(size_t(5), data.size());

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(push_back)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5};
      Data data;

      CHECK_NO_THROW(data.push_back(1));
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 4, data.begin()));

      CHECK_NO_THROW(data.push_back(2));
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 3, data.begin()));

      CHECK_NO_THROW(data.push_back(3));
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 2, data.begin()));

      CHECK_NO_THROW(data.push_back(4));
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 1, data.begin()));

      CHECK_NO_THROW(data.push_back(5));
      CHECK_EQUAL(size_t(5), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(PushBackExcess)
    {
      Data data;

      CHECK_NO_THROW(data.push_back(1));
      CHECK_NO_THROW(data.push_back(2));
      CHECK_NO_THROW(data.push_back(3));
      CHECK_NO_THROW(data.push_back(4));
      CHECK_NO_THROW(data.push_back(5));
      CHECK_NO_THROW(data.push_back(6));
      CHECK_THROW(data.push_back(7), etl::deque_full);
    }

    //*************************************************************************
    TEST(PushFrontNoValue)
    {
      CompareData compare_data = {5,  4, 3, 2, 1 };
      Data data;

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 1);
      CHECK_EQUAL(size_t(1), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 2);
      CHECK_EQUAL(size_t(2), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 3);
      CHECK_EQUAL(size_t(3), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 4);
      CHECK_EQUAL(size_t(4), data.size());

      CHECK_NO_THROW(data.push_front());
      CHECK_NO_THROW(data.front() = 5);
      CHECK_EQUAL(size_t(5), data.size());

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(PushFront)
    {
      CompareData compare_data = { 5, 4, 3, 2, 1 };
      Data data;

      CHECK_NO_THROW(data.push_front(1));
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin() + 4, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(2));
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin() + 3, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(3));
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin() + 2, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(4));
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin() + 1, compare_data.end(), data.begin()));

      CHECK_NO_THROW(data.push_front(5));
      CHECK_EQUAL(size_t(5), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(PushFrontExcess)
    {
      Data data;

      for (size_t i = 0; i < SIZE; ++i)
      {
        CHECK_NO_THROW(data.push_front(i));
      }

      CHECK_THROW(data.push_front(SIZE), etl::deque_full);
    }

    //*************************************************************************
    TEST(PushFrontPushBack)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5};
      Data data;

      CHECK_NO_THROW(data.push_back(3));
      CHECK_NO_THROW(data.push_front(2));
      CHECK_NO_THROW(data.push_back(4));
      CHECK_NO_THROW(data.push_front(1));
      CHECK_NO_THROW(data.push_back(5));

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(PopFront)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data;

      data.assign(compare_data.begin(), compare_data.end());

      data.pop_front();
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin() + 1, compare_data.end(), data.begin()));

      data.pop_front();
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin() + 2, compare_data.end(), data.begin()));

      data.pop_front();
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin() + 3, compare_data.end(), data.begin()));

      data.pop_front();
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin() + 4, compare_data.end(), data.begin()));

      data.pop_front();
      CHECK_EQUAL(size_t(0), data.size());
    }

    //*************************************************************************
    TEST(PopBack)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data;

      data.assign(compare_data.begin(), compare_data.end());

      data.pop_back();
      CHECK_EQUAL(size_t(4), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 1, data.begin()));

      data.pop_back();
      CHECK_EQUAL(size_t(3), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 2, data.begin()));

      data.pop_back();
      CHECK_EQUAL(size_t(2), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 3, data.begin()));

      data.pop_back();
      CHECK_EQUAL(size_t(1), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end() - 4, data.begin()));

      data.pop_back();
      CHECK_EQUAL(size_t(0), data.size());
    }

    //*************************************************************************
    TEST(Resize)
    {
      CompareData compare_data = { 1, 2, 0, 0, 0 };
      Data data;

      data.push_front(1);
      data.push_back(2);
      data.resize(SIZE);

      CHECK_EQUAL(size_t(SIZE), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(ResizeWithValue)
    {
      CompareData compare_data = { 1, 2, 3, 3, 3 };
      Data data;

      data.push_front(1);
      data.push_back(2);
      data.resize(SIZE, 3);

      CHECK_EQUAL(size_t(SIZE), data.size());
      CHECK(std::equal(compare_data.begin(), compare_data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(ResizeExcess)
    {
      Data data;

      CHECK_THROW(data.resize(SIZE + 1), etl::deque_out_of_bounds);
    }

    //*************************************************************************
    TEST(Full)
    {
      Data data;
      data.resize(SIZE);

      CHECK(data.full());
    }

    //*************************************************************************
    TEST(empty)
    {
      Data data;
      
      CHECK(data.empty());
    }

    //*************************************************************************
    TEST(Front)
    {
      Data data;

      data.push_front(1);
      CHECK_EQUAL(1, data.front());

      data.push_front(2);
      CHECK_EQUAL(2, data.front());

      data.push_front(3);
      CHECK_EQUAL(3, data.front());

      data.push_front(4);
      CHECK_EQUAL(4, data.front());

      data.push_front(5);
      CHECK_EQUAL(5, data.front());
    }

    //*************************************************************************
    TEST(FrontEmpty)
    {
      Data data;

      CHECK_THROW(int i = data.front(), etl::deque_empty);
    }

    //*************************************************************************
    TEST(FrontConst)
    {
      Data data;
      const Data& ctestDeque = data;

      data.push_front(1);
      CHECK_EQUAL(1, ctestDeque.front());

      data.push_front(2);
      CHECK_EQUAL(2, ctestDeque.front());

      data.push_front(3);
      CHECK_EQUAL(3, ctestDeque.front());

      data.push_front(4);
      CHECK_EQUAL(4, ctestDeque.front());

      data.push_front(5);
      CHECK_EQUAL(5, ctestDeque.front());
    }

    //*************************************************************************
    TEST(Back)
    {
      Data data;

      data.push_back(1);
      CHECK_EQUAL(1, data.back());

      data.push_back(2);
      CHECK_EQUAL(2, data.back());

      data.push_back(3);
      CHECK_EQUAL(3, data.back());

      data.push_back(4);
      CHECK_EQUAL(4, data.back());

      data.push_back(5);
      CHECK_EQUAL(5, data.back());
    }

    //*************************************************************************
    TEST(BackConst)
    {
      Data data;
      const Data& ctestDeque = data;

      data.push_back(1);
      CHECK_EQUAL(1, ctestDeque.back());

      data.push_back(2);
      CHECK_EQUAL(2, ctestDeque.back());

      data.push_back(3);
      CHECK_EQUAL(3, ctestDeque.back());

      data.push_back(4);
      CHECK_EQUAL(4, ctestDeque.back());

      data.push_back(5);
      CHECK_EQUAL(5, ctestDeque.back());
    }

    //*************************************************************************
    TEST(BackEmpty)
    {
      Data data;

      CHECK_THROW(int i = data.back(), etl::deque_empty);
    }

    //*************************************************************************
    TEST(Index)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data(compare_data.begin(), compare_data.end());

      CHECK_EQUAL(compare_data[0], data[0]);
      CHECK_EQUAL(compare_data[1], data[1]);
      CHECK_EQUAL(compare_data[2], data[2]);
      CHECK_EQUAL(compare_data[3], data[3]);
      CHECK_EQUAL(compare_data[4], data[4]);
    }

    //*************************************************************************
    TEST(IndexOutOfRange)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data(compare_data.begin(), compare_data.end());

      CHECK_THROW(int i = data[5], etl::deque_out_of_bounds);
    }

    //*************************************************************************
    TEST(IndexConst)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data(compare_data.begin(), compare_data.end());
      const Data& ctestDeque(data);

      CHECK_EQUAL(compare_data[0], ctestDeque[0]);
      CHECK_EQUAL(compare_data[1], ctestDeque[1]);
      CHECK_EQUAL(compare_data[2], ctestDeque[2]);
      CHECK_EQUAL(compare_data[3], ctestDeque[3]);
      CHECK_EQUAL(compare_data[4], ctestDeque[4]);
    }

    //*************************************************************************
    TEST(IndexConstOutOfRange)
    {
      CompareData compare_data = { 1, 2, 3, 4, 5 };
      Data data(compare_data.begin(), compare_data.end());
      const Data& ctestDeque(data);

      CHECK_THROW(int i = ctestDeque[5], etl::deque_out_of_bounds);
    }

    //*************************************************************************
    TEST(InsertValueBegin)
    {
      CompareData original = { 1, 2, 3, 4 };
      CompareData expected = { 5, 1, 2, 3, 4 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.begin(), 5);

      CHECK_EQUAL(expected.size(), data.size());
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
      CHECK_EQUAL(0, std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(InsertValueEnd)
    {
      CompareData original = { 1, 2, 3, 4 };
      CompareData expected = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.end(), 5);

      CHECK_EQUAL(expected.size(), data.size());
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
      CHECK_EQUAL(4, std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(InsertValueMiddle)
    {
      CompareData original = { 1, 2, 3, 4 };
      CompareData expected1 = { 1, 5, 2, 3, 4 };
      CompareData expected2 = { 1, 2, 5, 3, 4 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.begin() + 1, 5);

      CHECK_EQUAL(expected1.size(), data.size());
      CHECK(std::equal(expected1.begin(), expected1.end(), data.begin()));
      CHECK_EQUAL(1, std::distance(data.begin(), position));

      data.assign(original.begin(), original.end());

      position = data.insert(data.begin() + 2, 5);

      CHECK_EQUAL(expected2.size(), data.size());
      CHECK(std::equal(expected2.begin(), expected2.end(), data.begin()));
      CHECK_EQUAL(2, std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(InsertNValueBegin)
    {
      CompareData original = { 1, 2, 3 };
      CompareData expected = { 5, 5, 1, 2, 3 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.begin(), 2, 5);

      CHECK_EQUAL(expected.size(), data.size());
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
      CHECK_EQUAL(0, std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(InsertNValueEnd)
    {
      CompareData original = { 1, 2, 3 };
      CompareData expected = { 1, 2, 3, 5, 5 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.end(), 2, 5);

      CHECK_EQUAL(expected.size(), data.size());
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
      CHECK_EQUAL(3, std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(InsertNValueMiddle)
    {
      CompareData original  = { 1, 2, 3, 4 };
      CompareData expected1 = { 1, 5, 5, 2, 3, 4 };
      CompareData expected2 = { 1, 2, 3, 5, 5, 4 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.begin() + 1, 2, 5);

      CHECK_EQUAL(expected1.size(), data.size());
      CHECK(std::equal(expected1.begin(), expected1.end(), data.begin()));
      CHECK_EQUAL(1, std::distance(data.begin(), position));

      data.assign(data.max_size(), -1);
      data.assign(original.begin(), original.end());

      position = data.insert(data.begin() + 3, 2, 5);

      CHECK_EQUAL(expected2.size(), data.size());
      CHECK(std::equal(expected2.begin(), expected2.end(), data.begin()));
      CHECK_EQUAL(3, std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(InsertNValueExcess)
    {
      CompareData original = { 1, 2, 3, 4 };
      Data data(original.begin(), original.end());

      CHECK_THROW(data.insert(data.begin(), 3, 5), etl::deque_full);
      CHECK_THROW(data.insert(data.end(), 3, 5), etl::deque_full);
      CHECK_THROW(data.insert(data.begin() + 2, 3, 5), etl::deque_full);
    }

    //*************************************************************************
    TEST(InsertRangeBegin)
    {
      CompareData original = { 1, 2, 3 };
      CompareData range = { 4, 5 };
      CompareData expected = { 4, 5, 1, 2, 3 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.begin(), range.begin(), range.end());

      CHECK_EQUAL(expected.size(), data.size());
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
    }

    //*************************************************************************
    TEST(InsertRangeEnd)
    {
      CompareData original = { 1, 2, 3 };
      CompareData range = { 4, 5 };
      CompareData expected = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.end(), range.cbegin(), range.cend());

      CHECK_EQUAL(expected.size(), data.size());
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
    }

    //*************************************************************************
    TEST(InsertRangeMiddle)
    {
      CompareData original = { 1, 2, 3, 4 };
      CompareData range = { 5, 6 };
      CompareData expected1 = { 1, 5, 6, 2, 3, 4 };
      CompareData expected2 = { 1, 2, 3, 5, 6, 4 };
      Data data(original.begin(), original.end());

      Data::iterator position = data.insert(data.begin() + 1, range.begin(), range.end());

      CHECK_EQUAL(expected1.size(), data.size());
      CHECK(std::equal(expected1.begin(), expected1.end(), data.begin()));
      CHECK_EQUAL(1, std::distance(data.begin(), position));

      data.assign(original.begin(), original.end());

      position = data.insert(data.begin() + 3, range.begin(), range.end());

      CHECK_EQUAL(expected2.size(), data.size());
      CHECK(std::equal(expected2.begin(), expected2.end(), data.begin()));
      CHECK_EQUAL(3, std::distance(data.begin(), position));
    }

    //*************************************************************************
    TEST(InsertRangeExcess)
    {
      CompareData original = { 1, 2, 3, 4 };
      CompareData range = { 5, 6, 7 };
      Data data(original.begin(), original.end());

      CHECK_THROW(data.insert(data.begin(), range.begin(), range.end()), etl::deque_full);
      CHECK_THROW(data.insert(data.end(), range.begin(), range.end()), etl::deque_full);
      CHECK_THROW(data.insert(data.begin() + 2, range.begin(), range.end()), etl::deque_full);
    }

    //*************************************************************************
    TEST(EraseBegin)
    {
      CompareData original = { 0, 0, 1, 2, 3, 4 };
      CompareData expected = { 2, 3, 4, 5, 6 };
      Data data(original.begin(), original.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.push_back(5);
      data.push_back(6);

      Data::iterator i_next = data.erase(data.begin());

      CHECK_EQUAL(Data::difference_type(expected.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), data.begin().get_index());
    }

    //*************************************************************************
    TEST(EraseEnd)
    {
      CompareData original = { 0, 0, 1, 2, 3, 4 };
      CompareData expected = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.push_back(5);
      data.push_back(6);

      Data::iterator i_erase = data.end() - 1;
      Data::iterator i_next  = data.erase(i_erase);

      CHECK_EQUAL(Data::difference_type(expected.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), data.end().get_index());
    }

    //*************************************************************************
    TEST(EraseMiddle)
    {
      CompareData original = { 0, 0, 1, 2, 3, 4 };
      CompareData expected1 = { 1, 3, 4, 5, 6 };
      CompareData expected2 = { 1, 2, 3, 4, 6 };

      Data data(original.begin(), original.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.push_back(5);
      data.push_back(6);

      // Erase near beginning.
      Data::iterator i_erase = data.begin() + 1;
      Data::iterator i_next  = data.erase(i_erase);

      CHECK_EQUAL(Data::difference_type(expected1.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected1.begin(), expected1.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), data.begin().get_index() + 1);

      data.assign(original.begin(), original.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.push_back(5);
      data.push_back(6);

      // Erase near end.
      i_next = data.erase(data.begin() + 4);

      CHECK_EQUAL(Data::difference_type(expected2.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected2.begin(), expected2.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), data.begin().get_index() + 4);
    }

    //*************************************************************************
    TEST(EraseRangeBegin)
    {
      CompareData original = { 0, 0, 1, 2, 3, 4 };
      CompareData expected = { 3, 4, 5, 6 };
      Data data(original.begin(), original.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.push_back(5);
      data.push_back(6);
      
      Data::iterator i_next = data.erase(data.begin(), data.begin() + 2);

      CHECK_EQUAL(Data::difference_type(expected.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), data.begin().get_index());
    }

    //*************************************************************************
    TEST(EraseRangeEnd)
    {
      CompareData original = { 0, 0, 1, 2, 3, 4 };
      CompareData expected = { 1, 2, 3, 4 };
      Data data(original.begin(), original.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.push_back(5);
      data.push_back(6);

      Data::iterator i_next = data.erase(data.end() - 2, data.end());

      CHECK_EQUAL(Data::difference_type(expected.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), data.end().get_index());
    }

    //*************************************************************************
    TEST(EraseRangeMiddle)
    {
      CompareData original  = { 0, 0, 1, 2, 3, 4 };
      CompareData expected1 = { 1, 4, 5, 6 };
      CompareData expected2 = { 1, 2, 3, 6 };

      Data data(original.begin(), original.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.push_back(5);
      data.push_back(6);

      Data::iterator i_next = data.erase(data.begin() + 1, data.begin() + 3);

      CHECK_EQUAL(Data::difference_type(expected1.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected1.begin(), expected1.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), (data.begin() + 1).get_index());

      data.assign(original.begin(), original.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.push_back(5);
      data.push_back(6);

      i_next = data.erase(data.begin() + 3, data.begin() + 5);

      CHECK_EQUAL(Data::difference_type(expected2.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected2.begin(), expected2.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), (data.begin() + 3).get_index());

      CompareData original2 = { 0, 0, 0, 0, 1, 2};
      CompareData expected3 = { 1, 2, 5, 6 };

      data.assign(original2.begin(), original2.end());

      // Cause rollover.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(3);
      data.push_back(4);
      data.push_back(5);
      data.push_back(6);

      i_next = data.erase(data.begin() + 2, data.begin() + 4);

      CHECK_EQUAL(Data::difference_type(expected3.size()), std::distance(data.begin(), data.end()));
      CHECK(std::equal(expected3.begin(), expected3.end(), data.begin()));
      CHECK_EQUAL(i_next.get_index(), (data.begin() + 2).get_index());
    }

    //*************************************************************************
    TEST(Sort)
    {
      CompareData original = { 5, 3, 4, 1, 2 };
      CompareData expected = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      std::sort(data.begin(), data.end());

      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
    }

    //*************************************************************************
    TEST(ReverseForwardIterators)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      CompareData expected = { 5, 4, 3, 2, 1 };
      Data data(original.begin(), original.end());

      std::reverse(data.begin(), data.end());

      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
    }

    //*************************************************************************
    TEST(ReverseReverseIterators)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      CompareData expected = { 5, 4, 3, 2, 1 };
      Data data(original.begin(), original.end());

      std::reverse(data.rbegin(), data.rend());

      CHECK(std::equal(expected.begin(), expected.end(), data.begin()));
    }

    //*************************************************************************
    TEST(BeginEnd)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      Data::iterator begin = data.begin();
      Data::iterator end   = data.end();

      CHECK_EQUAL(Data::difference_type(original.size()), std::distance(begin, end));
      CHECK(std::equal(begin, end, data.begin()));
    }

    //*************************************************************************
    TEST(BeginEndConst)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      Data::const_iterator begin = data.begin();
      Data::const_iterator end   = data.end();

      CHECK_EQUAL(Data::difference_type(original.size()), std::distance(begin, end));
      CHECK(std::equal(begin, end, original.begin()));
    }

    //*************************************************************************
    TEST(CBeginCEnd)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      Data::const_iterator begin = data.cbegin();
      Data::const_iterator end   = data.cend();

      CHECK_EQUAL(Data::difference_type(original.size()), std::distance(begin, end));
      CHECK(std::equal(begin, end, original.begin()));
    }

    //*************************************************************************
    TEST(RBeginREnd)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      CompareData expected = { 5, 4, 3, 2, 1 };
      Data data(original.begin(), original.end());

      Data::reverse_iterator begin = data.rbegin();
      Data::reverse_iterator end   = data.rend();

      CHECK_EQUAL(Data::difference_type(expected.size()), std::distance(begin, end));
      CHECK(std::equal(begin, end, expected.begin()));
    }

    //*************************************************************************
    TEST(RBeginREndConst)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      CompareData expected = { 5, 4, 3, 2, 1 };
      Data data(original.begin(), original.end());

      Data::const_reverse_iterator begin = data.rbegin();
      Data::const_reverse_iterator end   = data.rend();

      CHECK_EQUAL(Data::difference_type(expected.size()), std::distance(begin, end));
      CHECK(std::equal(begin, end, expected.begin()));
    }

    //*************************************************************************
    TEST(CRBeginCREnd)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      CompareData expected = { 5, 4, 3, 2, 1 };
      Data data(original.begin(), original.end());

      Data::const_reverse_iterator begin = data.crbegin();
      Data::const_reverse_iterator end   = data.crend();

      CHECK_EQUAL(Data::difference_type(expected.size()), std::distance(begin, end));
      CHECK(std::equal(begin, end, expected.begin()));
    }

    //*************************************************************************
    TEST(Iterators)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      Data::iterator begin = data.begin();
      Data::iterator end   = data.begin();
     
      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(IteratorsRolloverRight)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      // Cause rollover right.
      data.pop_front();
      data.pop_front(); 
      data.pop_front();
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);

      Data::iterator begin = data.begin();
      Data::iterator end   = data.begin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(6, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(7, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(8, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(5, *begin);
      CHECK_EQUAL(8, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(6, *begin);
      CHECK_EQUAL(8, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(7, *begin);
      CHECK_EQUAL(8, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(8, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(6, *end);
    }

    //*************************************************************************
    TEST(IteratorsRolloverLeft)
    {
      CompareData original = { 4, 5, 6, 7, 8 };
      Data data(original.begin(), original.end());

      // Cause rollover left.
      data.pop_back(); 
      data.pop_back();
      data.pop_back();
      data.push_front(3);
      data.push_front(2);
      data.push_front(1);

      Data::iterator begin = data.begin();
      Data::iterator end = data.begin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(ConstIterators)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      Data::const_iterator begin = data.cbegin();
      Data::const_iterator end   = data.cbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(ConstIteratorsRolloverRight)
    {
      CompareData original = { 1, 2, 3, 4, 5 };
      Data data(original.begin(), original.end());

      // Cause rollover right.
      data.pop_front(); 
      data.pop_front(); 
      data.pop_front();
      data.push_back(6);
      data.push_back(7);
      data.push_back(8);

      Data::const_iterator begin = data.cbegin();
      Data::const_iterator end   = data.cbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(6, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(7, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(8, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(5, *begin);
      CHECK_EQUAL(8, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(6, *begin);
      CHECK_EQUAL(8, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(7, *begin);
      CHECK_EQUAL(8, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(8, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(6, *end);
    }

    //*************************************************************************
    TEST(ConstIteratorsRolloverLeft)
    {
      CompareData original = { 4, 5, 6, 7, 8 };
      Data data(original.begin(), original.end());

      // Cause rollover left.
      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(3);
      data.push_front(2);
      data.push_front(1);
            
      Data::const_iterator begin = data.cbegin();
      Data::const_iterator end   = data.cbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(ReverseIterators)
    {
      CompareData original = { 5, 4, 3, 2, 1 };
      Data data(original.begin(), original.end());

      Data::reverse_iterator begin = data.rbegin();
      Data::reverse_iterator end   = data.rbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(ReverseIteratorsRolloverRight)
    {
      CompareData original = { 0, 0, 0, 5, 4 };
      Data data(original.begin(), original.end());

      // Cause rollover right.
      data.pop_front(); 
      data.pop_front();  
      data.pop_front();
      data.push_back(3);
      data.push_back(2);
      data.push_back(1);

      Data::reverse_iterator begin = data.rbegin();
      Data::reverse_iterator end   = data.rbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(ReverseIteratorsRolloverLeft)
    {
      CompareData original = { 2, 1, 0, 0, 0 };
      Data data(original.begin(), original.end());

      // Cause rollover left.
      data.pop_back(); 
      data.pop_back();
      data.pop_back();
      data.push_front(3);
      data.push_front(4);
      data.push_front(5);

      Data::reverse_iterator begin = data.rbegin();
      Data::reverse_iterator end   = data.rbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(ConstReverseIterators)
    {
      CompareData original = { 5, 4, 3, 2, 1 };
      Data data(original.begin(), original.end());

      Data::const_reverse_iterator begin = data.crbegin();
      Data::const_reverse_iterator end   = data.crbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(ConstReverseIteratorsRolloverRight)
    {
      CompareData original = { 0, 0, 0, 5, 4 };
      Data data(original.begin(), original.end());

      // Cause rollover right.
      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(3);
      data.push_back(2);
      data.push_back(1);

      Data::const_reverse_iterator begin = data.crbegin();
      Data::const_reverse_iterator end   = data.crbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(ConstReverseIteratorsRolloverLeft)
    {
      CompareData original = { 2, 1, 0, 0, 0 };
      Data data(original.begin(), original.end());

      // Cause rollover left.
      data.pop_back();
      data.push_front(3);
      data.pop_back();
      data.push_front(4);
      data.pop_back();
      data.push_front(5);

      Data::const_reverse_iterator begin = data.crbegin();
      Data::const_reverse_iterator end   = data.crbegin();

      CHECK_EQUAL(0, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(1, *end);
      ++end;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(2, *end);
      ++end;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
      ++end;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(4, *end);
      ++end;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(3, std::distance(begin, end));
      CHECK_EQUAL(2, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(3, *begin);
      CHECK_EQUAL(5, *end);
      ++begin;
      CHECK_EQUAL(1, std::distance(begin, end));
      CHECK_EQUAL(4, *begin);
      CHECK_EQUAL(5, *end);
      begin -= 3;
      CHECK_EQUAL(4, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(5, *end);
      end -= 2;
      CHECK_EQUAL(2, std::distance(begin, end));
      CHECK_EQUAL(1, *begin);
      CHECK_EQUAL(3, *end);
    }

    //*************************************************************************
    TEST(IteratorsNegativeDistance)
    {
      CompareData original = { 2, 1, 0, 0, 0 };
      Data data(original.begin(), original.end());

      CHECK_EQUAL(-5, std::distance(data.end(), data.begin()));
    }

    //*************************************************************************
    TEST(ConstIteratorsNegativeDistance)
    {
      CompareData original = { 2, 1, 0, 0, 0 };
      Data data(original.begin(), original.end());

      CHECK_EQUAL(-5, std::distance(data.cend(), data.cbegin()));
    }

    //*************************************************************************
    TEST(ReverseIteratorsNegativeDistance)
    {
      CompareData original = { 2, 1, 0, 0, 0 };
      Data data(original.begin(), original.end());

      CHECK_EQUAL(-5, std::distance(data.rend(), data.rbegin()));
    }

    //*************************************************************************
    TEST(ConstReverseIteratorsNegativeDistance)
    {
      CompareData original = { 2, 1, 0, 0, 0 };
      Data data(original.begin(), original.end());

      CHECK_EQUAL(-5, std::distance(data.crend(), data.crbegin()));
    }

    //*************************************************************************
    TEST(IteratorsComparison)
    {
      Data data(6, 0);

      Data::iterator first  = data.begin() + 1;
      Data::iterator second = data.begin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(IteratorsComparisonRolloverRight)
    {
      Data data(6, 0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);

      Data::iterator first  = data.begin() + 1;
      Data::iterator second = data.begin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ConstIteratorsComparison)
    {
      Data data(6, 0);

      Data::const_iterator first  = data.cbegin() + 1;
      Data::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ConstIteratorsComparisonRolloverRight)
    {
      Data data(6, 0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);

      Data::const_iterator first  = data.cbegin() + 1;
      Data::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ConstIteratorsComparisonRolloverLeft)
    {
      Data data(6, 0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);

      Data::const_iterator first  = data.cbegin() + 1;
      Data::const_iterator second = data.cbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ReverseIteratorsComparison)
    {
      Data data(6, 0);

      Data::reverse_iterator first  = data.rbegin() + 1;
      Data::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ReverseIteratorsComparisonRolloverRight)
    {
      Data data(6, 0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);

      Data::reverse_iterator first  = data.rbegin() + 1;
      Data::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ReverseIteratorsComparisonRolloverLeft)
    {
      Data data(6, 0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);

      Data::reverse_iterator first  = data.rbegin() + 1;
      Data::reverse_iterator second = data.rbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ConstReverseIteratorsComparison)
    {
      Data data(6, 0);

      Data::const_reverse_iterator first  = data.crbegin() + 1;
      Data::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ConstReverseIteratorsComparisonRolloverRight)
    {
      Data data(6, 0);

      data.pop_front();
      data.pop_front();
      data.pop_front();
      data.push_back(1);
      data.push_back(1);
      data.push_back(1);

      Data::const_reverse_iterator first  = data.crbegin() + 1;
      Data::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
    TEST(ConstReverseIteratorsComparisonRolloverLeft)
    {
      Data data(6, 0);

      data.pop_back();
      data.pop_back();
      data.pop_back();
      data.push_front(1);
      data.push_front(1);
      data.push_front(1);

      Data::const_reverse_iterator first = data.crbegin() + 1;
      Data::const_reverse_iterator second = data.crbegin() + 4;

      CHECK(first < second);
      CHECK(!(second < first));
    }

    //*************************************************************************
		TEST(OperatorEqual)
		{
      CompareData original = { 1, 2, 3, 4, 5, 6 };
      CompareData modified = { 6, 5, 4, 3, 2, 1 };
     
      Data testDeque1(original.begin(), original.end());
      Data testDeque2(testDeque1);

      CHECK(testDeque1 == testDeque2);

      // Change testDeque2's data.
      std::copy(modified.begin(), modified.end(), testDeque2.begin());

      CHECK(testDeque1 != testDeque2);
		}
	};
}
