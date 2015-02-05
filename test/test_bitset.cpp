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

#include <limits>
#include <type_traits>
#include <bitset>

#define ETL_IN_UNIT_TEST

#include "../bitset.h"

#undef min
#undef max

namespace
{
  SUITE(test_bitset)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_value)
    {
      std::bitset<60> compare(0x123456731234567);
      etl::bitset<60> data(0x123456731234567);

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_excess_value)
    {
      std::bitset<60> compare(0x8765432187654321);
      etl::bitset<60> data(0x8765432187654321);

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_string)
    {                               
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data("110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_excess_string)
    {
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001111100001");
      etl::bitset<60> data("110001001000110100010101100111001100010010001101000101011001111100001");

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(),  data.none());
      CHECK_EQUAL(compare.any(),   data.any());
      CHECK_EQUAL(compare.all(),   data.all());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_set)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      compare.set();
      data.set();

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(),  data.none());
      CHECK_EQUAL(compare.any(),   data.any());
      CHECK_EQUAL(compare.all(),   data.all());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_string_set)
    {
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.set("110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_position_set)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      for (size_t i = 0; i < data.size(); ++i)
      {
        compare.set(i, (i % 2) == 0);
        data.set(i, (i % 2) == 0);
      }

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(),  data.none());
      CHECK_EQUAL(compare.any(),   data.any());
      CHECK_EQUAL(compare.all(),   data.all());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFF);
      etl::bitset<60> data(0xFFFFFFFFFFFFFFF);

      compare.reset();
      data.reset();

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(),  data.none());
      CHECK_EQUAL(compare.any(),   data.any());
      CHECK_EQUAL(compare.all(),   data.all());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_position_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFF);
      etl::bitset<60> data(0xFFFFFFFFFFFFFFF);

      for (size_t i = 0; i < data.size(); i += 2)
      {
        compare.reset(i);
        data.reset(i);
      }

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(),  data.none());
      CHECK_EQUAL(compare.any(),   data.any());
      CHECK_EQUAL(compare.all(),   data.all());

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_index_operator_read)
    {
      std::bitset<60> compare(0x3123456731234567);
      etl::bitset<60> data(0x3123456731234567);

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare[i], data[i]);
      }
    }

    //*************************************************************************
    TEST(test_index_operator_write)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      for (size_t i = 0; i < data.size(); ++i)
      {
        compare[i] = (i % 2) == 0;
        data[i]    = (i % 2) == 0;
      }

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare[i], data[i]);
      }
    }

    //*************************************************************************
    TEST(test_any)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      // Set all bits
      compare.set();
      data.set();

      // Reset them all.
      for (size_t i = 0; i < data.size(); ++i)
      {
        compare.reset(i);
        data.reset(i);
      }

      CHECK_EQUAL(compare.any(), data.any());

      compare.set(3);
      data.set(3);

      CHECK_EQUAL(compare.any(), data.any());
    }

    //*************************************************************************
    TEST(test_none)
    {
      std::bitset<62> compare;
      etl::bitset<62> data;

      CHECK_EQUAL(compare.none(), data.none());

      compare.set(3);
      data.set(3);

      CHECK_EQUAL(compare.none(), data.none());
    }

    //*************************************************************************
    TEST(test_all)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      CHECK_EQUAL(compare.all(), data.all());

      compare.set();
      data.set();

      CHECK_EQUAL(compare.all(), data.all());
    }

    //*************************************************************************
    TEST(test_flip)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      compare.flip();
      data.flip();

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_flip_position)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      compare.flip(3);
      data.flip(3);

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_flip_reference)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      compare[3].flip();
      data[3].flip();

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_invert_reference)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      bool bc = ~compare[3];
      bool bd = ~data[3];

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_assignment_operator)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFF);
      etl::bitset<60> data1(0xFFFFFFFFFFFFFFF);
      etl::bitset<60> data2;

      data2 = data1;

      for (size_t i = 0; i < data2.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      etl::bitset<60> data1(0x123456781234567);
      etl::bitset<60> data2(0x123456781234567);
      etl::bitset<60> data3;

      CHECK(data1 == data2);
      CHECK(!(data1 == data3));
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      etl::bitset<60> data1(0x123456781234567);
      etl::bitset<60> data2(0x123456781234567);
      etl::bitset<60> data3;

      CHECK(!(data1 != data2));
      CHECK(data1 != data3);
    }

    //*************************************************************************
    TEST(test_shift_left_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2;
      etl::bitset<60> shift1(0x2468ACF0);
      etl::bitset<60> shift2(0x48D159E0);
      etl::bitset<60> shift11(0x91A2B3C000);

      data2 = data1;
      data2 <<= 1;
      CHECK(data2 == shift1);

      data2 = data1;
      data2 <<= 2;
      CHECK(data2 == shift2);

      data2 = data1;
      data2 <<= 11;
      CHECK(data2 == shift11);
    }

    //*************************************************************************
    TEST(test_shift_left_copy_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2;
      etl::bitset<60> shift1(0x2468ACF0);
      etl::bitset<60> shift2(0x48D159E0);
      etl::bitset<60> shift11(0x91A2B3C000);

      data2 = data1 << 1;  
      CHECK(data2 == shift1);

      data2 = data1 << 2;
      CHECK(data2 == shift2);

      data2 = data1 << 11;
      CHECK(data2 == shift11);
    }

    //*************************************************************************
    TEST(test_shift_right_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2;
      etl::bitset<60> shift1(0x91A2B3C);
      etl::bitset<60> shift2(0x48D159E);
      etl::bitset<60> shift11(0x2468A);

      data2 = data1;
      data2 >>= 1;
      CHECK(data2 == shift1);

      data2 = data1;
      data2 >>= 2;
      CHECK(data2 == shift2);

      data2 = data1;
      data2 >>= 11;
      CHECK(data2 == shift11);
    }

    //*************************************************************************
    TEST(test_shift_right_copy_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2;
      etl::bitset<60> shift1(0x91A2B3C);
      etl::bitset<60> shift2(0x48D159E);
      etl::bitset<60> shift11(0x2468A);

      data2 = data1 >> 1;
      CHECK(data2 == shift1);

      data2 = data1 >> 2;
      CHECK(data2 == shift2);

      data2 = data1 >> 11;
      CHECK(data2 == shift11);
    }

    //*************************************************************************
    TEST(test_and_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2(0x23456789);
      etl::bitset<60> data3;
      etl::bitset<60> data4(0x12345678 & 0x23456789);

      data3 = data1 & data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_and_equals_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2(0x23456789);
      etl::bitset<60> data3(0x12345678 & 0x23456789);

      data2 &= data1;
      CHECK(data2 == data3);
    }

    //*************************************************************************
    TEST(test_or_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2(0x23456789);
      etl::bitset<60> data3;
      etl::bitset<60> data4(0x12345678 | 0x23456789);

      data3 = data1 | data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_or_equals_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2(0x23456789);
      etl::bitset<60> data3(0x12345678 | 0x23456789);

      data2 |= data1;
      CHECK(data2 == data3);
    }

    //*************************************************************************
    TEST(test_xor_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2(0x23456789);
      etl::bitset<60> data3;
      etl::bitset<60> data4(0x12345678 ^ 0x23456789);

      data3 = data1 ^ data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_xor_equals_operator)
    {
      etl::bitset<60> data1(0x12345678);
      etl::bitset<60> data2(0x23456789);
      etl::bitset<60> data3(0x12345678 ^ 0x23456789);

      data2 ^= data1;
      CHECK(data2 == data3);
    }
  
    //*************************************************************************
    TEST(test_big_bitset)
    {
      std::bitset<10000> compare;
      etl::bitset<10000> data;

      for (size_t i = 0; i < data.size(); ++i)
      {
        compare.set(i, (i % 2) == 0);
        data.set(i, (i % 2) == 0);
      }

      CHECK_EQUAL(compare.test(64), data.test(64));

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }

      compare.flip();
      data.flip();

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    } 

    //*************************************************************************
    TEST(test_small_bitset)
    {
      std::bitset<7> compare;
      etl::bitset<7> data;

      compare.flip();
      data.flip();

      for (size_t i = 0; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_find_first)
    {
      etl::bitset<6> data;

      data.set("000000");
      CHECK_EQUAL(0, data.find_first(false));
      CHECK_EQUAL(6, data.find_first(true));

      data.set("111111");
      CHECK_EQUAL(6, data.find_first(false));
      CHECK_EQUAL(0, data.find_first(true));

      data.set("000001");
      CHECK_EQUAL(1, data.find_first(false));
      CHECK_EQUAL(0, data.find_first(true));

      data.set("100000");
      CHECK_EQUAL(0, data.find_first(false));
      CHECK_EQUAL(5, data.find_first(true));

      data.set("100001");
      CHECK_EQUAL(1, data.find_first(false));
      CHECK_EQUAL(0, data.find_first(true));

      data.set("001110");
      CHECK_EQUAL(0, data.find_first(false));
      CHECK_EQUAL(1, data.find_first(true));

      data.set("110001");
      CHECK_EQUAL(1, data.find_first(false));
      CHECK_EQUAL(0, data.find_first(true));
    }

    //*************************************************************************
    TEST(test_find_next)
    {
      etl::bitset<6> data;

      data.set("000000");
      CHECK_EQUAL(0, data.find_next(false, 0));
      CHECK_EQUAL(1, data.find_next(false, 1));
      CHECK_EQUAL(6, data.find_next(true,  2));

      data.set("111111");
      CHECK_EQUAL(0, data.find_next(true,  0));
      CHECK_EQUAL(1, data.find_next(true,  1));
      CHECK_EQUAL(6, data.find_next(false, 2));

      data.set("001110");
      CHECK_EQUAL(0, data.find_next(false, 0));
      CHECK_EQUAL(1, data.find_next(true,  0));
      CHECK_EQUAL(4, data.find_next(false, 1));

      data.set("110001");
      CHECK_EQUAL(0, data.find_next(true,  0));
      CHECK_EQUAL(1, data.find_next(false, 0));
      CHECK_EQUAL(4, data.find_next(true,  1));
    }


    //*************************************************************************
    TEST(test_swap)
    {
      etl::bitset<6> compare1("101010");
      etl::bitset<6> compare2("010101");

      etl::bitset<6> data1(compare1);
      etl::bitset<6> data2(compare2);

      swap(data1, data2);

      CHECK(data1 == compare2);
      CHECK(data2 == compare1);
    }

    //*************************************************************************
    TEST(test_iterator)
    {
      typedef etl::bitset<16> Bitset;

      Bitset data("1111000011001010");
      Bitset data2(data);

      int i;

      // Read
      Bitset::iterator b1 = data.begin();
      Bitset::iterator e1 = data.end();

      CHECK_EQUAL(16, std::distance(b1, e1));

      i = 0;
      while (b1 != e1)
      {
        CHECK_EQUAL(data[i], *b1);
        ++b1;
        ++i;
      }

      Bitset::iterator b2 = data2.begin();
      Bitset::iterator e2 = data2.end();

      CHECK_EQUAL(16, std::distance(b2, e2));

      // Write
      i = 0;
      while (b2 != e2)
      {
        *b2 = !*b2;
        CHECK_EQUAL(!data[i], *b2);
        ++b2;
        ++i;
      }
    }

    //*************************************************************************
    TEST(test_const_iterator)
    {
      typedef etl::bitset<16> Bitset;
      Bitset data("1111000011001010");

      // const_iterator
      Bitset::const_iterator b1 = data.cbegin();
      Bitset::const_iterator e1 = data.cend();

      CHECK_EQUAL(16, std::distance(b1, e1));

      int i = 0;
      while (b1 != e1)
      {
        CHECK_EQUAL(data[i], *b1);
        ++b1;
        ++i;
      }
    }
  };
}
