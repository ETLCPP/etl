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

#include <limits>
#include <type_traits>
#include <bitset>

#include "etl/private/bitset_legacy.h"

namespace
{
  SUITE(test_bitset_legacy)
  {
    //*************************************************************************
    TEST(test_constructor)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_value)
    {
      std::bitset<60> compare(0x123456731234567ULL);
      etl::bitset<60> data(0x123456731234567ULL);

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_excess_value)
    {
      std::bitset<60> compare(0x8765432187654321ULL);
      etl::bitset<60> data(0x8765432187654321ULL);

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_char_string)
    {
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data("110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_wchar_t_string)
    {
      std::bitset<60> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data(L"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_char16_t_string)
    {
      std::bitset<60> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data(u"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_char32_t_string)
    {
      std::bitset<60> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data(U"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
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

      for (size_t i = 0UL; i < data.size(); ++i)
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

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_base_set)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      etl::ibitset& idata = data;

      compare.set();
      idata.set();

      CHECK_EQUAL(compare.size(),  idata.size());
      CHECK_EQUAL(compare.count(), idata.count());
      CHECK_EQUAL(compare.none(),  idata.none());
      CHECK_EQUAL(compare.any(),   idata.any());
      CHECK_EQUAL(compare.all(),   idata.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), idata.test(i));
      }
    }

    //*************************************************************************
    TEST(test_char_string_set)
    {
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.set("110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_wchar_t_string_set)
    {
      std::bitset<60> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.set(L"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_char16_t_string_set)
    {
      std::bitset<60> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.set(u"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_char32_t_string_set)
    {
      std::bitset<60> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.set(U"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_from_char_string)
    {
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.from_string("110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_from_wchar_t_string)
    {
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.from_string(L"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_from_char16_t_6string)
    {
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.from_string(u"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_from_char32_t_string)
    {
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

      data.from_string(U"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_value_u8_min)
    {
      etl::bitset<8> data("00000000");
      uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u8_max)
    {
      etl::bitset<8> data("11111111");
      uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_min)
    {
      etl::bitset<8> data("10000000");
      int8_t value = data.value<int8_t>();

      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_max)
    {
      etl::bitset<8> data("01111111");
      int8_t value = data.value<int8_t>();

      CHECK_EQUAL(std::numeric_limits<int8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_min)
    {
      etl::bitset<16> data("0000000000000000");
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_max)
    {
      etl::bitset<16> data("1111111111111111");
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_min)
    {
      etl::bitset<16> data("1000000000000000");
      int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_max)
    {
      etl::bitset<16> data("0111111111111111");
      int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_min)
    {
      etl::bitset<32> data("00000000000000000000000000000000");
      uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_max)
    {
      etl::bitset<32> data("11111111111111111111111111111111");
      uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_min)
    {
      etl::bitset<32> data("10000000000000000000000000000000");
      int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_max)
    {
      etl::bitset<32> data("01111111111111111111111111111111");
      int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_min)
    {
      etl::bitset<64> data("0000000000000000000000000000000000000000000000000000000000000000");
      uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_max)
    {
      etl::bitset<64> data("1111111111111111111111111111111111111111111111111111111111111111");
      uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_min)
    {
      etl::bitset<64> data("1000000000000000000000000000000000000000000000000000000000000000");
      int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_max)
    {
      etl::bitset<64> data("0111111111111111111111111111111111111111111111111111111111111111");
      int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_type_too_small)
    {
      etl::bitset<60> data("110001001000110100010101100111001100010010001101000101011001");

      etl::ibitset& idata= data;

      uint32_t value = 0U;

      CHECK_THROW(value = idata.value<uint32_t>(), etl::bitset_type_too_small);

      CHECK_EQUAL(0U, value);
    }

    //*************************************************************************
    TEST(test_position_set)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        compare.set(i, (i % 2) == 0);
        data.set(i, (i % 2) == 0);
      }

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(),  data.none());
      CHECK_EQUAL(compare.any(),   data.any());
      CHECK_EQUAL(compare.all(),   data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_position_set_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset<12> data;
      CHECK_THROW(data.set(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data(0xFFFFFFFFFFFFFFFULL);

      compare.reset();
      etl::bitset<60>& rdata = data.reset();

      CHECK_EQUAL(compare.size(),  rdata.size());
      CHECK_EQUAL(compare.count(), rdata.count());
      CHECK_EQUAL(compare.none(),  rdata.none());
      CHECK_EQUAL(compare.any(),   rdata.any());
      CHECK_EQUAL(compare.all(),   rdata.all());

      for (size_t i = 0UL; i < rdata.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), rdata.test(i));
      }
    }

    //*************************************************************************
    TEST(test_base_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data(0xFFFFFFFFFFFFFFFULL);
      etl::ibitset& idata = data;

      etl::ibitset& ridata = idata.reset();

      compare.reset();

      CHECK_EQUAL(compare.size(),  ridata.size());
      CHECK_EQUAL(compare.count(), ridata.count());
      CHECK_EQUAL(compare.none(),  ridata.none());
      CHECK_EQUAL(compare.any(),   ridata.any());
      CHECK_EQUAL(compare.all(),   ridata.all());

      for (size_t i = 0UL; i < ridata.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), ridata.test(i));
      }
    }


    //*************************************************************************
    TEST(test_position_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data(0xFFFFFFFFFFFFFFFULL);

      for (size_t i = 0UL; i < data.size(); i += 2UL)
      {
        compare.reset(i);
        data.reset(i);
      }

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(),  data.none());
      CHECK_EQUAL(compare.any(),   data.any());
      CHECK_EQUAL(compare.all(),   data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_position_reset_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset<12> data;

      CHECK_THROW(data.reset(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_position_flip_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset<12> data;

      CHECK_THROW(data.flip(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_position_test_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset<12> data;

      CHECK_THROW(data.test(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_index_operator_read)
    {
      std::bitset<60> compare(0x3123456731234567ULL);
      etl::bitset<60> data(0x3123456731234567ULL);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare[i], data[i]);
      }
    }

    //*************************************************************************
    TEST(test_index_operator_write)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        compare[i] = (i % 2) == 0;
        data[i]    = (i % 2) == 0;
      }

      for (size_t i = 0UL; i < data.size(); ++i)
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
      for (size_t i = 0UL; i < data.size(); ++i)
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

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_base_flip)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      etl::ibitset& idata = data;

      compare.flip();
      idata.flip();

      for (size_t i = 0UL; i < idata.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), idata.test(i));
      }
    }

    //*************************************************************************
    TEST(test_flip_position)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

      compare.flip(3);
      data.flip(3);

      for (size_t i = 0UL; i < data.size(); ++i)
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

      for (size_t i = 0UL; i < data.size(); ++i)
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

      CHECK_EQUAL(bc, bd);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_assignment_operator)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data1(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data2;

      data2 = data1;

      for (size_t i = 0UL; i < data2.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_assignment_operator_iterface)
    {
      etl::bitset<60> data1(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data2;

      etl::ibitset& idata1 = data1;
      etl::ibitset& idata2 = data2;

      idata2 = idata1;

      for (size_t i = 0UL; i < data2.size(); ++i)
      {
        CHECK_EQUAL(data1.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      etl::bitset<60> data1(0x123456781234567ULL);
      etl::bitset<60> data2(0x123456781234567ULL);
      etl::bitset<60> data3;

      CHECK(data1 == data2);
      CHECK(!(data1 == data3));
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      etl::bitset<60> data1(0x123456781234567ULL);
      etl::bitset<60> data2(0x123456781234567ULL);
      etl::bitset<60> data3;

      CHECK(!(data1 != data2));
      CHECK(data1 != data3);
    }

    //*************************************************************************
    TEST(test_shift_left_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2;
      etl::bitset<60> shift1(0x2468ACF0UL);
      etl::bitset<60> shift2(0x48D159E0UL);
      etl::bitset<60> shift11(0x91A2B3C000ULL);

      data2 = data1;
      data2 <<= 1U;
      CHECK(data2 == shift1);

      data2 = data1;
      data2 <<= 2U;
      CHECK(data2 == shift2);

      data2 = data1;
      data2 <<= 11U;
      CHECK(data2 == shift11);
    }

    //*************************************************************************
    TEST(test_shift_left_operator_shift_element_size)
    {
      etl::bitset<60> data(0x12345678UL);
      etl::bitset<60> shift(0x1234567800UL);

      data <<= 8U;
      CHECK_EQUAL_HEX(shift.value<uint64_t>(), data.value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_left_operator_overflow)
    {
      etl::bitset<31> data(0x7FFFFFFFUL);
      etl::bitset<31> shifted(0x7FFFFFFEUL);

      data <<= 1U;
      CHECK(data == shifted);
    }

    //*************************************************************************
    TEST(test_shift_left_copy_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2;
      etl::bitset<60> shift1(0x2468ACF0UL);
      etl::bitset<60> shift2(0x48D159E0UL);
      etl::bitset<60> shift11(0x91A2B3C000ULL);

      data2 = data1 << 1U;
      CHECK(data2 == shift1);

      data2 = data1 << 2U;
      CHECK(data2 == shift2);

      data2 = data1 << 11U;
      CHECK(data2 == shift11);
    }

    //*************************************************************************
    TEST(test_shift_left_operator_all_shifts_full_size)
    {
      constexpr uint64_t value = 0x0123456789ABCDEFULL;

      for (int shift = 0; shift < 64; ++shift)
      {
        etl::bitset<64> data(value);

        CHECK_EQUAL_HEX((value << shift), (data << shift).value<uint64_t>());
      }

      etl::bitset<64> data(value);
      CHECK_EQUAL_HEX(0, (data << 64).value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_left_operator_all_shifts_partial_size)
    {
      constexpr uint64_t value = 0x0123456789ABCDEFULL;
      constexpr uint64_t mask = 0x0FFFFFFFFFFFFFFFULL;

      for (int shift = 0; shift < 64; ++shift)
      {
        etl::bitset<60> data(value);

        CHECK_EQUAL_HEX(((value << shift) & mask), (data << shift).value<uint64_t>());
      }

      etl::bitset<64> data(value);
      CHECK_EQUAL_HEX(0, (data << 64).value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2;
      etl::bitset<60> shift1(0x91A2B3CUL);
      etl::bitset<60> shift2(0x48D159EUL);
      etl::bitset<60> shift11(0x2468AUL);

      data2 = data1;
      data2 >>= 1U;
      CHECK(data2 == shift1);

      data2 = data1;
      data2 >>= 2U;
      CHECK(data2 == shift2);

      data2 = data1;
      data2 >>= 11U;
      CHECK(data2 == shift11);
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_full_size)
    {
      constexpr uint64_t value = 0x0123456789ABCDEFULL;
      
      for (int shift = 0; shift < 64; ++shift)
      {
        etl::bitset<64> data(value);

        CHECK_EQUAL_HEX((value >> shift), (data >> shift).value<uint64_t>());
      }

      etl::bitset<64> data(value);
      CHECK_EQUAL_HEX(0, (data >> 64).value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_partial_size)
    {
      constexpr uint64_t value = 0x0123456789ABCDEFULL;
      constexpr uint64_t mask  = 0x0FFFFFFFFFFFFFFFULL;

      for (int shift = 0; shift < 64; ++shift)
      {
        etl::bitset<60> data(value);

        CHECK_EQUAL_HEX(((value >> shift) & mask), (data >> shift).value<uint64_t>());
      }

      etl::bitset<64> data(value);
      CHECK_EQUAL_HEX(0, (data >> 64).value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_operator_shift_element_size)
    {
      etl::bitset<60> data(0x12345678UL);
      etl::bitset<60> shift(0x123456UL);

      data >>= 8U;
      CHECK_EQUAL_HEX(shift.value<uint64_t>(), data.value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_copy_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2;
      etl::bitset<60> shift1(0x91A2B3CUL);
      etl::bitset<60> shift2(0x48D159EUL);
      etl::bitset<60> shift11(0x2468AUL);

      data2 = data1 >> 1U;
      CHECK(data2 == shift1);

      data2 = data1 >> 2U;
      CHECK(data2 == shift2);

      data2 = data1 >> 11U;
      CHECK(data2 == shift11);
    }

    //*************************************************************************
    TEST(test_and_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3;
      etl::bitset<60> data4(0x12345678UL & 0x23456789UL);

      data3 = data1 & data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_and_equals_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3(0x12345678UL & 0x23456789UL);

      data2 &= data1;
      CHECK(data2 == data3);
    }

    //*************************************************************************
    TEST(test_or_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3;
      etl::bitset<60> data4(0x12345678UL | 0x23456789UL);

      data3 = data1 | data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_or_equals_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3(0x12345678UL | 0x23456789UL);

      data2 |= data1;
      CHECK(data2 == data3);
    }

    //*************************************************************************
    TEST(test_xor_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3;
      etl::bitset<60> data4(0x12345678UL ^ 0x23456789UL);

      data3 = data1 ^ data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_xor_equals_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3(0x12345678UL ^ 0x23456789UL);

      data2 ^= data1;
      CHECK(data2 == data3);
    }

    //*************************************************************************
    TEST(test_index_operator)
    {
      etl::bitset<60> data1(0x123456781234567ULL);
      etl::bitset<60> data2;

      for (size_t i = 0UL; i < data1.size(); ++i)
      {
        data2[i] = data1[i];
      }

      CHECK(data1 == data2);
    }

    //*************************************************************************
    TEST(test_base_index_operator)
    {
      etl::bitset<60> data1(0x123456781234567ULL);
      etl::bitset<60> data2;

      etl::ibitset& idata1 = data1;
      etl::ibitset& idata2 = data2;

      for (size_t i = 0UL; i < idata1.size(); ++i)
      {
        idata2[i] = idata1[i];
      }

      CHECK(data1 == data2);
    }

    //*************************************************************************
    TEST(test_big_bitset)
    {
      std::bitset<10000> compare;
      etl::bitset<10000> data;

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        compare.set(i, (i % 2) == 0);
        data.set(i, (i % 2) == 0);
      }

      CHECK_EQUAL(compare.test(64), data.test(64));

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }

      compare.flip();
      data.flip();

      for (size_t i = 0UL; i < data.size(); ++i)
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

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_find_first)
    {
      etl::bitset<6> data;

      data.set("000000");
      CHECK_EQUAL(0U, data.find_first(false));
      CHECK_EQUAL(etl::ibitset::npos, data.find_first(true));

      data.set("111111");
      CHECK_EQUAL(etl::ibitset::npos, data.find_first(false));
      CHECK_EQUAL(0U, data.find_first(true));

      data.set("000001");
      CHECK_EQUAL(1U, data.find_first(false));
      CHECK_EQUAL(0U, data.find_first(true));

      data.set("100000");
      CHECK_EQUAL(0U, data.find_first(false));
      CHECK_EQUAL(5U, data.find_first(true));

      data.set("100001");
      CHECK_EQUAL(1U, data.find_first(false));
      CHECK_EQUAL(0U, data.find_first(true));

      data.set("001110");
      CHECK_EQUAL(0U, data.find_first(false));
      CHECK_EQUAL(1U, data.find_first(true));

      data.set("110001");
      CHECK_EQUAL(1U, data.find_first(false));
      CHECK_EQUAL(0U, data.find_first(true));
    }

    //*************************************************************************
    TEST(test_find_next)
    {
      etl::bitset<6> data;

      data.set("000000");
      CHECK_EQUAL(0U, data.find_next(false, 0));
      CHECK_EQUAL(1U, data.find_next(false, 1));
      CHECK_EQUAL(etl::ibitset::npos, data.find_next(true, 2));

      data.set("111111");
      CHECK_EQUAL(0U, data.find_next(true,  0));
      CHECK_EQUAL(1U, data.find_next(true,  1));
      CHECK_EQUAL(etl::ibitset::npos, data.find_next(false, 2));

      data.set("001110");
      CHECK_EQUAL(0U, data.find_next(false, 0));
      CHECK_EQUAL(1U, data.find_next(true,  0));
      CHECK_EQUAL(4U, data.find_next(false, 1));

      data.set("110001");
      CHECK_EQUAL(0U, data.find_next(true,  0));
      CHECK_EQUAL(1U, data.find_next(false, 0));
      CHECK_EQUAL(4U, data.find_next(true,  1));
    }

    //*************************************************************************
    TEST(test_find_next_multi_byte_bitset_github_issue_336)
    {
      etl::bitset<24> data;
      data.set(12);
      data.set(22);
      CHECK_EQUAL(12U, data.find_next(true,  3));
      CHECK_EQUAL(12U, data.find_next(true, 10));

      // set first ten bytes
      data.set("1111111111");
      CHECK_EQUAL(10U, data.find_next(false, 3));
      CHECK_EQUAL(10U, data.find_next(false, 9));
    }

    //*************************************************************************
    TEST(test_find_next_github_issue_336)
    {
      etl::bitset<16> bits16;
      bits16.set(12);

      etl::bitset<24> bits24;
      bits24.set(12);

      CHECK_EQUAL(12, bits16.find_first(true));
      CHECK_EQUAL(12, bits16.find_next(true, 4));
      CHECK_EQUAL(etl::ibitset::npos, bits16.find_next(true, 13));

      CHECK_EQUAL(12, bits24.find_first(true));
      CHECK_EQUAL(12, bits24.find_next(true, 4));
      CHECK_EQUAL(etl::ibitset::npos, bits24.find_next(true, 13));
    }

    //*************************************************************************
    TEST(test_find_next_github_issue_617)
    {
      etl::bitset<64> bs1(0xC000000000000031ULL);
      size_t bs1fnt1 = bs1.find_next(true, 10);
      size_t bs1fnt2 = bs1.find_next(true, 59);
      CHECK_EQUAL(62U, bs1fnt1);
      CHECK_EQUAL(62U, bs1fnt2);

      etl::bitset<64> bs2(0x3FFFFFFFFFFFFFCEULL);
      size_t bs2fnf1 = bs2.find_next(false, 10);
      size_t bs2fnf2 = bs2.find_next(false, 59);
      CHECK_EQUAL(62U, bs2fnf1);
      CHECK_EQUAL(62U, bs2fnf2);
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
    TEST(test_span)
    {
      using span_t = etl::ibitset::span_type;

      etl::bitset<32> b(0x12345678UL);

      span_t s = b.span();
      CHECK_EQUAL(0x78U, s[0]);
      CHECK_EQUAL(0x56U, s[1]);
      CHECK_EQUAL(0x34U, s[2]);
      CHECK_EQUAL(0x12U, s[3]);

      s[2] = 0x9AU;
      uint32_t value = b.value<uint32_t>();
      CHECK_EQUAL(0x129A5678UL, value);
    }

    //*************************************************************************
    TEST(test_const_span)
    {
      using span_t = etl::ibitset::const_span_type;

      const etl::bitset<32> b(0x12345678UL);

      span_t s = b.span();
      CHECK_EQUAL(0x78U, s[0]);
      CHECK_EQUAL(0x56U, s[1]);
      CHECK_EQUAL(0x34U, s[2]);
      CHECK_EQUAL(0x12U, s[3]);
    }

    //*************************************************************************
    TEST(test_to_string)
    {
      etl::bitset<32> b(0x12345678UL);

      etl::string<32> text = b.to_string('.', '*');
      std::string stdtext = b.to_string<std::string>('.', '*');

      CHECK_THROW(etl::string<32> text1 = b.to_string<etl::string<30>>('.', '*'), etl::bitset_overflow);   
      CHECK_EQUAL("...*..*...**.*...*.*.**..****...", text.c_str());
      CHECK_EQUAL("...*..*...**.*...*.*.**..****...", stdtext.c_str());
    }

    //*************************************************************************
    TEST(test_issue_497_count_inverted_bits)
    {
      etl::bitset<5U> bits;
      std::bitset<5U> stdbits;
      etl::bitset<5U> negbits = ~bits;
      std::bitset<5U> stdnegbits = ~stdbits;

      CHECK_EQUAL(stdbits.count(),    bits.count());
      CHECK_EQUAL(stdbits.all(),      bits.all());
      CHECK_EQUAL(stdnegbits.count(), negbits.count());
      CHECK_EQUAL(stdnegbits.all(),   negbits.all());
    }

    //*************************************************************************
    TEST(test_issue_497_shifted_zero_sized_bitset)
    {
      etl::bitset<0> bits;
      std::bitset<0> stdbits;

      CHECK_EQUAL(stdbits.size(), bits.size());
      CHECK_EQUAL(stdbits.none(), bits.none());
      CHECK_EQUAL(stdbits.all(),  bits.all());
      CHECK_EQUAL(stdbits.any(),  bits.any());

      CHECK_EQUAL(stdbits.count(),     bits.count());
      CHECK_EQUAL(stdbits.to_ulong(),  bits.to_ulong());
      CHECK_EQUAL(stdbits.to_ullong(), bits.to_ullong());

      etl::bitset<0> shiftbits = bits << 1; // No exception.
    }
  };
}
