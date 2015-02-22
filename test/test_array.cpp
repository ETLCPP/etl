/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl

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

#include "../array.h"

#include <array>
#include <algorithm>
#include <iterator>

#include "../integral_limits.h"

namespace
{
  SUITE(test_array)
  {
    static const size_t SIZE = 10;

    typedef etl::array<int, SIZE> Data;
    typedef std::array<int, SIZE> Compare_Data;

    Compare_Data compare_data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    Compare_Data swap_data    = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    //*************************************************************************
    TEST(test_constructor)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.size(), size_t(SIZE));
      CHECK_EQUAL(data.max_size(), SIZE);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data other_data;

      other_data = data;

      bool isEqual = std::equal(data.begin(), data.end(), other_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_at)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::array_out_of_range);
    }

    //*************************************************************************
    TEST(test_at_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.at(i), compare_data.at(i));
      }

      CHECK_THROW(data.at(data.size()), etl::array_out_of_range);
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST(test_index_operator_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data[i], compare_data[i]);
      }
    }

    //*************************************************************************
    TEST(test_front)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int& ref = data.front();
      CHECK(ref == compare_data.front());

      ++ref;
      CHECK(ref == compare_data.front() + 1);
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const int& ref = data.front();
      CHECK(ref == compare_data.front());
    }

    //*************************************************************************
    TEST(test_back)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      int& ref = data.back();
      CHECK(ref == compare_data.back());

      ++ref;
      CHECK(ref == compare_data.back() + 1);
    }

    //*************************************************************************
    TEST(test_back_const)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const int& ref = data.back();
      CHECK(ref == compare_data.back());
    }

    //*************************************************************************
    TEST(test_data)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.begin(), data.end(), data.data());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_data_const)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.begin(), data.end(), data.data());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_begin)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.begin(), &data[0]);
    }

    //*************************************************************************
    TEST(test_end)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.end(), &data[SIZE]);
    }

    //*************************************************************************
    TEST(test_cbegin)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.cbegin(), &data[0]);
    }

    //*************************************************************************
    TEST(test_cend)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data.cend(), &data[SIZE]);
    }
    
    //*************************************************************************
    TEST(test_rbegin)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.rbegin() == Data::reverse_iterator(&data[SIZE]));
    }

    //*************************************************************************
    TEST(test_rend)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.rend() == Data::reverse_iterator(&data[0]));
    }

    //*************************************************************************
    TEST(test_crbegin)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.crbegin() == Data::const_reverse_iterator(&data[SIZE]));
    }

    //*************************************************************************
    TEST(test_crend)
    {
      const Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data.crend() == Data::const_reverse_iterator(&data[0]));
    }
    
    //*************************************************************************
    TEST(test_iterator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.cbegin(), data.cend(), compare_data.cbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_reverse_iterator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.rbegin(), data.rend(), compare_data.rbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_const_reverse_iterator)
    {
      Data data = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      bool isEqual = std::equal(data.crbegin(), data.crend(), compare_data.crbegin());

      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_empty)
    {
      Data data;

      CHECK(!data.empty());
    }

    //*************************************************************************
    TEST(test_size)
    {
      Data data;

      CHECK_EQUAL(SIZE, data.size());
    }

    //*************************************************************************
    TEST(test_max_size)
    {
      Data data;

      CHECK_EQUAL(SIZE, data.max_size());
    }


    //*************************************************************************
    TEST(test_fill)
    {
      Data data;
      data.fill(1);

      Compare_Data compare_data;
      compare_data.fill(1);

      bool isEqual = std::equal(data.begin(), data.end(), compare_data.begin());

      CHECK(isEqual);
    }
    
    //*************************************************************************
    TEST(test_swap)
    {
      Data data1 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
      Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      swap(data1, data2);

      CHECK(std::equal(compare_data.begin(), compare_data.end(), data1.begin()));
      CHECK(std::equal(swap_data.begin(), swap_data.end(), data2.begin()));
    }

    //*************************************************************************
    TEST(test_get)
    {
      Data data1       = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      const Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK_EQUAL(data1[3], etl::get<3>(data1));
      CHECK_EQUAL(data2[3], etl::get<3>(data2));

      // The following line should fail with a compile error.
      //int i = etl::get<11>(data2);
    }

    //*************************************************************************
    TEST(test_equal)
    {
      Data data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      CHECK(data1 == data2);
    }

    //*************************************************************************
    TEST(test_not_equal)
    {
      Data data1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data data2 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

      CHECK(data1 != data2);
    }

    //*************************************************************************
    TEST(test_less_than)
    {
      Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
      Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      CHECK(lesser    < data);
      CHECK(!(data    < data));
      CHECK(!(greater < data));
    }

    //*************************************************************************
    TEST(test_less_than_equal)
    {
      Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
      Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      CHECK(lesser    <= data);
      CHECK(data      <= data);
      CHECK(!(greater <= data));
    }

    //*************************************************************************
    TEST(test_greater_than)
    {
      Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
      Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      CHECK(greater  > data);
      CHECK(!(data   > data));
      CHECK(!(lesser > data));
    }

    //*************************************************************************
    TEST(test_greater_than_equal)
    {
      Data data    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
      Data greater = { 0, 1, 2, 3, 5, 5, 6, 7, 8, 9 };
      Data lesser  = { 0, 1, 2, 3, 4, 4, 6, 7, 8, 9 };

      CHECK(greater  >= data);
      CHECK(data     >= data);
      CHECK(!(lesser >= data));
    }
  };
}
