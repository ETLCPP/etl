///******************************************************************************
//The MIT License(MIT)
//
//Embedded Template Library.
//https://github.com/ETLCPP/etl
//https://www.etlcpp.com
//
//Copyright(c) 2020 jwellbelove
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
//******************************************************************************/

#include "UnitTest++/UnitTest++.h"

#include "etl/circular_buffer.h"
#include "etl/integral_limits.h"

#include "data.h"

#include <algorithm>
#include <iterator>
#include <vector>
#include <string>

namespace
{
  SUITE(test_circular_buffer)
  {
    static const size_t SIZE = 10;

    using ItemM    = TestDataM<std::string>;
    using DataM    = etl::circular_buffer<ItemM, 0>;
    using CompareM = std::vector<ItemM>;

    using Ndc     = TestDataNDC<std::string>;
    using Data    = etl::circular_buffer<Ndc, 0>;
    using Compare = std::vector<Ndc>;

    using BufferM_t = etl::uninitialized_buffer_of<ItemM, SIZE + 1>;
    using Buffer_t  = etl::uninitialized_buffer_of<Ndc, SIZE + 1>;

    BufferM_t bufferm1;
    BufferM_t bufferm2;

    Buffer_t buffer1;
    Buffer_t buffer2;

    //*************************************************************************
    TEST(test_default_constructor)
    {
      Data data(buffer1.raw, SIZE);

      CHECK_EQUAL(0U,   data.size());
      CHECK_EQUAL(SIZE, data.max_size());
      CHECK_EQUAL(SIZE, data.capacity());
      CHECK_EQUAL(SIZE, data.available());
      CHECK(data.begin()   == data.end());
      CHECK(data.cbegin()  == data.cend());
      CHECK(data.rbegin()  == data.rend());
      CHECK(data.crbegin() == data.crend());
    }

#if ETL_USING_STL
    //*************************************************************************
    TEST(test_constructor_from_literal)
    {
      Data data({ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") }, buffer1.raw, SIZE);
      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };


      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = etl::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_constructor_from_literal_excess)
    {
      Data data({ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") }, buffer1.raw, SIZE);
      Compare compare = { Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12")  };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }
#endif

#if !defined(ETL_TEMPLATE_DEDUCTION_GUIDE_TESTS_DISABLED) && ETL_USING_STL
    //*************************************************************************
    TEST(test_cpp17_deduced_constructor)
    {
      Data data({ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") }, buffer1.raw, SIZE);
      Compare compare{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_cpp17_deduced_constructor_excess)
    {
      Data data({ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") }, buffer1.raw, SIZE);
      Compare compare{ Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12")  };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(data.begin(), data.end(), compare.begin());
      CHECK(isEqual);
    }
#endif

    //*************************************************************************
    TEST(test_push)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      for (auto v : test)
      {
        data.push(v);
      }

      Compare compare{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(SIZE - data.size(), data.available());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_move_push)
    {
      DataM data(bufferm1.raw, SIZE);
      CompareM compare;

      for (uint32_t i = 0; i < SIZE; ++i)
      {
        data.push(ItemM(std::to_string(i)));
        compare.push_back(ItemM(std::to_string(i)));
      }

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_full_range)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_full_range_reverse_iterator)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_excess_range)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare{ Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12")  };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_excess_range_reverse_iterator)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare{ Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12")  };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_at_start_of_buffer)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4") };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_at_start_of_buffer_reverse_iterator)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4") };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_at_end_of_buffer)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());
      data.pop(5);

      Compare compare{ Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10") };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_at_end_of_buffer_reverse_iterator)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());
      data.pop(5);

      Compare compare{ Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10") };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_middle_of_buffer)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4") };
      Compare input2{ Ndc("5"), Ndc("6"), Ndc("7") };
      Data data(buffer1.raw, SIZE);
      data.push(input1.begin(), input1.end());
      data.pop(3);
      data.push(input2.begin(), input2.end());

      Compare compare{ Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7") };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_middle_of_buffer_reverse_iterator)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4") };
      Compare input2{ Ndc("5"), Ndc("6"), Ndc("7") };
      Data data(buffer1.raw, SIZE);
      data.push(input1.begin(), input1.end());
      data.pop(3);
      data.push(input2.begin(), input2.end());

      Compare compare{ Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7") };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_overlap_start_and_end_of_buffer)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Compare input2{ Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(input1.begin(), input1.end());
      data.pop(7);
      data.push(input2.begin(), input2.end());

      Compare compare{ Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };

      CHECK(data.begin()  != data.end());
      CHECK(data.cbegin() != data.cend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_push_short_range_overlap_start_and_end_of_buffer_reverse_iterator)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Compare input2{ Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(input1.begin(), input1.end());
      data.pop(7);
      data.push(input2.begin(), input2.end());

      Compare compare{ Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };

      CHECK(data.rbegin()  != data.rend());
      CHECK(data.crbegin() != data.crend());
      CHECK_EQUAL(compare.size(), data.size());

      bool isEqual = std::equal(compare.rbegin(), compare.rend(), data.rbegin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_available)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      for (auto v : test)
      {
        data.push(v);
        CHECK_EQUAL(SIZE - data.size(), data.available());
      }
    }

    //*************************************************************************
    TEST(test_front)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(input1.begin(), input1.end());

      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      Ndc& ref1 = data.front();
      CHECK(ref1 == compare.front());

      Ndc& ref2 = data.front();
      ref2 = compare.back();
      CHECK(ref2 == compare.back());
    }

    //*************************************************************************
    TEST(test_front_const)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(input1.begin(), input1.end());

      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      const Ndc& ref = data.front();
      CHECK(ref == compare.front());
    }

    //*************************************************************************
    TEST(test_back)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(input1.begin(), input1.end());

      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      Ndc& ref1 = data.back();
      CHECK(ref1 == compare.back());

      Ndc& ref2 = data.back();
      ref2 = compare.front();
      CHECK(ref2 == compare.front());
    }

    //*************************************************************************
    TEST(test_back_const)
    {
      Compare input{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(input.begin(), input.end());

      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      const Ndc& ref = data.back();
      CHECK(ref == compare.back());
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      // Overrun by 3
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Compare input2{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(input1.begin(), input1.end());

      for (int i = 0; i < SIZE; ++i)
      {
        CHECK_EQUAL(input1[i + 3], data[i]);
      }

      for (int i = 0; i < SIZE; ++i)
      {
        data[i] = input2[i];
      }

      for (int i = 0; i < SIZE; ++i)
      {
        CHECK_EQUAL(input2[i], data[i]);
      }
    }

    //*************************************************************************
    TEST(test_index_operator_const)
    {
      // Overrun by 3
      Compare input{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(input.begin(), input.end());

      for (int i = 0; i < SIZE; ++i)
      {
        CHECK_EQUAL(input[i + 3], data[i]);
      }
    }

    //*************************************************************************
    TEST(test_random_iterator_plus)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      for (int step = 1; step < int(data.size()); ++step)
      {
        Data::const_iterator itr = data.begin();

        int offset = 0;

        while ((offset += step) < int(data.size() - 1))
        {
          itr = itr + step;
          CHECK_EQUAL(compare[offset], *itr);
        }
      }
    }

    //*************************************************************************
    TEST(test_random_iterator_plus_rollover)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare = { Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12")  };

      for (int step = 1; step < int(data.size()); ++step)
      {
        Data::const_iterator itr = data.begin();

        int offset = 0;

        while ((offset += step) < int(data.size() - 1))
        {
          itr = itr + step;
          CHECK_EQUAL(compare[offset], *itr);
        }
      }
    }

    //*************************************************************************
    TEST(test_random_iterator_plus_equals)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      for (int step = 1; step < int(data.size()); ++step)
      {
        Data::const_iterator itr = data.begin();

        int offset = 0;

        while ((offset += step) < int(data.size() - 1))
        {
          itr += + step;
          CHECK_EQUAL(compare[offset], *itr);
        }
      }
    }

    //*************************************************************************
    TEST(test_random_iterator_plus_equals_rollover)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare = { Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12")  };

      for (int step = 1; step < int(data.size()); ++step)
      {
        Data::const_iterator itr = data.begin();

        int offset = 0;

        while ((offset += step) < int(data.size() - 1))
        {
          itr += step;
          CHECK_EQUAL(compare[offset], *itr);
        }
      }
    }

    //*************************************************************************
    TEST(test_random_iterator_minus)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      for (int step = 1; step < int(data.size()); ++step)
      {
        Data::const_iterator itr = data.end();

        int offset = int(data.size());

        while ((offset -= step) > 0)
        {
          itr = itr - step;
          CHECK_EQUAL(compare[offset], *itr);
        }
      }
    }

    //*************************************************************************
    TEST(test_random_iterator_minus_rollover)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare = { Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12")  };

      for (int step = 1; step < int(data.size()); ++step)
      {
        Data::const_iterator itr = data.end();

        int offset = int(data.size());

        while ((offset -= step) > 0)
        {
          itr = itr - step;
          CHECK_EQUAL(compare[offset], *itr);
        }
      }
    }

    //*************************************************************************
    TEST(test_random_iterator_minus_equals)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Compare compare = { Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };

      for (int step = 1; step < int(data.size()); ++step)
      {
        Data::const_iterator itr = data.end();

        int offset = int(data.size());

        while ((offset -= step) > 0)
        {
          itr -= step;
          CHECK_EQUAL(compare[offset], *itr);
        }
      }
    }

    //*************************************************************************
    TEST(test_random_iterator_minus_equals_rollover)
    {
      Compare input{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(input.begin(), input.end());

      Compare compare = { Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12")  };

      for (int step = 1; step < int(data.size()); ++step)
      {
        Data::const_iterator itr = data.end();

        int offset = int(data.size());

        while ((offset -= step) > 0)
        {
          itr -= step;
          CHECK_EQUAL(compare[offset], *itr);
        }
      }
    }

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Compare input2{ Ndc("9"), Ndc("8"), Ndc("7"), Ndc("6"), Ndc("5"), Ndc("4"), Ndc("3"), Ndc("2"), Ndc("1"), Ndc("0") };
      Data data1(buffer1.raw, SIZE);
      data1.push(input1.begin(), input1.end());

      // Copy construct from data1
      Data data2(data1, buffer2.raw, SIZE);

      // Now change data1
      data1.clear();
      data1.push(input2.begin(), input2.end());

      CHECK(data2.begin() != data2.end());
      CHECK(data2.cbegin() != data2.cend());
      CHECK_EQUAL(input1.size(), data2.size());

      bool isEqual = std::equal(input1.begin(), input1.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_move_constructor)
    {
      CompareM input1;
      CompareM input2;
      CompareM compare;

      for (uint32_t i = 0; i < SIZE; ++i)
      {
        input1.push_back(ItemM(std::to_string(i)));
        input2.push_back(ItemM(std::to_string(SIZE - i)));
        compare.push_back(ItemM(std::to_string(i)));
      }

      DataM data1(bufferm1.raw, SIZE);
      for (auto&& v : input1)
      {
        data1.push(std::move(v));
      }

      // Move construct from data1
      DataM data2(std::move(data1), bufferm2.raw, SIZE);

      // Now change data1
      data1.clear();
      for (auto&& v : input2)
      {
        data1.push(std::move(v));
      }

      CHECK(data2.begin() != data2.end());
      CHECK(data2.cbegin() != data2.cend());
      CHECK_EQUAL(input1.size(), data2.size());

      bool isEqual = std::equal(compare.begin(), compare.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9") };
      Compare input2{ Ndc("9"), Ndc("8"), Ndc("7"), Ndc("6"), Ndc("5"), Ndc("4"), Ndc("3"), Ndc("2"), Ndc("1"), Ndc("0") };
      Data data1(buffer1.raw, SIZE);
      data1.push(input1.begin(), input1.end());

      // Copy construct from data1
      Data data2(buffer2.raw, SIZE);
      
      data2 = data1;

      // Now change data1
      data1.clear();
      data1.push(input2.begin(), input2.end());

      CHECK(data2.begin() != data2.end());
      CHECK(data2.cbegin() != data2.cend());
      CHECK_EQUAL(input1.size(), data2.size());

      bool isEqual = std::equal(input1.begin(), input1.end(), data2.begin());
      CHECK(isEqual);
    }

    //*************************************************************************
    TEST(test_swap_iterator)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Data::iterator itr1 = data.begin() + 2;
      Data::iterator itr2 = data.begin() + 3;

      const Ndc& original1 = *itr1;
      const Ndc& original2 = *itr2;

      std::swap(itr1, itr2);

      const Ndc& swapped1 = *itr1;
      const Ndc& swapped2 = *itr2;

      CHECK(swapped1 == original2);
      CHECK(swapped2 == original1);
    }

    //*************************************************************************
    TEST(test_swap_const_iterator)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Data::const_iterator itr1 = data.begin() + 2;
      Data::const_iterator itr2 = data.begin() + 3;

      const Ndc& original1 = *itr1;
      const Ndc& original2 = *itr2;

      std::swap(itr1, itr2);

      const Ndc& swapped1 = *itr1;
      const Ndc& swapped2 = *itr2;

      CHECK(swapped1 == original2);
      CHECK(swapped2 == original1);
    }

    //*************************************************************************
    TEST(test_iterator_difference)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Data::iterator begin = data.begin();
      Data::iterator end   = data.begin();

      for (uint32_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(i, end - begin);
        CHECK_EQUAL(i, -(begin - end));
        ++end;
      }

      for (uint32_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.size() - i, end - begin);
        CHECK_EQUAL(data.size() - i, -(begin - end));
        ++begin;
      }
    }

    //*************************************************************************
    TEST(test_const_iterator_difference)
    {
      Compare test{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data(buffer1.raw, SIZE);
      data.push(test.begin(), test.end());

      Data::const_iterator begin = data.begin();
      Data::const_iterator end   = data.begin();

      for (uint32_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(i, end - begin);
        CHECK_EQUAL(i, -(begin - end));
        ++end;
      }

      for (uint32_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(data.size() - i, end - begin);
        CHECK_EQUAL(data.size() - i, -(begin - end));
        ++begin;
      }
    }

    //*************************************************************************
    TEST(test_swap)
    {
      // Over-write by 3
      Compare input{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Compare output{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data1(buffer1.raw, SIZE);
      Data data2(buffer2.raw, SIZE);
      data1.push(input.begin(), input.end());
      data2.push(input.rbegin(), input.rend());
      
      swap(data1, data2);

      CHECK(std::equal(output.rbegin() + 3, output.rend(), data1.begin()));
      CHECK(std::equal(output.begin() + 3, output.end(), data2.begin()));
    }

    //*************************************************************************
    TEST(test_equal)
    {
      // Over-write by 3
      Compare input{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data1(buffer1.raw, SIZE);
      Data data2(buffer1.raw, SIZE);
      data1.push(input.begin(), input.end());
      data2.push(input.begin(), input.end());

      CHECK(data1 == data2);
    }

    //*************************************************************************
    TEST(test_not_equal)
    {
      // Over-write by 3
      Compare input1{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("7"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Compare input2{ Ndc("0"), Ndc("1"), Ndc("2"), Ndc("3"), Ndc("4"), Ndc("5"), Ndc("6"), Ndc("6"), Ndc("8"), Ndc("9"), Ndc("10"), Ndc("11"), Ndc("12") };
      Data data1(buffer1.raw, SIZE);
      Data data2(buffer2.raw, SIZE);
      data1.push(input1.begin(), input1.end());
      data2.push(input2.begin(), input2.end());

      CHECK(data1 != data2);
    }
  };
}
