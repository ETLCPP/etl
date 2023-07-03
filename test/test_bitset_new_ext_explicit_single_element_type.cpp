/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#include "etl/private/bitset_new.h"
#include "etl/string.h"
#include "etl/wstring.h"
#include "etl/u16string.h"
#include "etl/u32string.h"

namespace
{
  using ull = unsigned long long;

  SUITE(test_bitset_new_ext_explicit_element_type)
  {
    //*************************************************************************
    TEST(test_default_constructor_from_array)
    {
      etl::bitset_ext<64, int64_t>::element_type buffer[etl::bitset_ext<64, int64_t>::Number_Of_Elements];
      std::bitset<64> compare;
      etl::bitset_ext<64, int64_t> data(buffer);

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_default_constructor_from_buffer_type)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, int64_t> data(buffer);

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_default_constructor_single_element)
    {
      etl::bitset_ext<8,  int8_t>::buffer_type  buffer8;
      etl::bitset_ext<16, int16_t>::buffer_type buffer16;
      etl::bitset_ext<32, int32_t>::buffer_type buffer32;
      etl::bitset_ext<64, int64_t>::buffer_type buffer64;

      etl::bitset_ext<8,  int8_t>  data8(buffer8);
      etl::bitset_ext<16, int16_t> data16(buffer16);
      etl::bitset_ext<32, int32_t> data32(buffer32);
      etl::bitset_ext<64, int64_t> data64(buffer64);

      CHECK_EQUAL(1U, data8.Number_Of_Elements);
      CHECK_EQUAL(1U, data16.Number_Of_Elements);
      CHECK_EQUAL(1U, data32.Number_Of_Elements);
      CHECK_EQUAL(1U, data64.Number_Of_Elements);
    }

    //*************************************************************************
    TEST(test_construct_from_value)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      std::bitset<64> compare(0x123456731234567ULL);
      etl::bitset_ext<64, int64_t> data(0x123456731234567ULL, buffer1);

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL_HEX(compare.to_ullong(), data.to_ullong());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_copy_construct)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      std::bitset<64> compare(0x123456731234567ULL);
      etl::bitset_ext<64, int64_t> data(0x123456731234567ULL, buffer1);
      etl::bitset_ext<64, int64_t> data_copy(data, buffer2);

      CHECK_EQUAL(compare.size(), data_copy.size());
      CHECK_EQUAL(compare.count(), data_copy.count());

      for (size_t i = 0UL; i < data_copy.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data_copy.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_excess_value)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(0x8765432187654321ULL);
      etl::bitset_ext<64, int64_t> data(0x8765432187654321ULL, buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data("110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(L"110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(u"110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(U"110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001111100001");
      etl::bitset_ext<64, int64_t> data("110001001000110100010101100111001100010010001101000101011001111100001", buffer);

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(),  data.none());
      CHECK_EQUAL(compare.any(),   data.any());
      CHECK_EQUAL(compare.all(),   data.all());

      CHECK_EQUAL_HEX(compare.to_ullong(), data.to_ullong());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_set)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, int64_t> data(buffer);

      compare.set();
      data.set();

      auto size  = data.size();
      auto count = data.count();
      auto none  = data.none();
      auto any   = data.any();
      auto all   = data.all();

      CHECK_EQUAL(compare.size(),  size);
      CHECK_EQUAL(compare.count(), count);
      CHECK_EQUAL(compare.none(),  none);
      CHECK_EQUAL(compare.any(),   any);
      CHECK_EQUAL(compare.all(),   all);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_char_string_set)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(buffer);

      data.set("110001001000110100010101100111001100010010001101000101011001");

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
    TEST(test_wchar_t_string_set)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(buffer);

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
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, int64_t> data(buffer);

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
      etl::bitset_ext<8, int8_t>::buffer_type buffer;
      etl::bitset_ext<8, int8_t> data((unsigned long long)etl::integral_limits<uint8_t>::min, buffer);
      uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u8_max)
    {
      etl::bitset_ext<8, int8_t>::buffer_type buffer;
      etl::bitset_ext<8, int8_t> data((unsigned long long)etl::integral_limits<uint8_t>::max, buffer);
      uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_min)
    {
      etl::bitset_ext<8, int8_t>::buffer_type buffer;
      etl::bitset_ext<8, int8_t> data((unsigned long long)etl::integral_limits<int8_t>::min, buffer);
      int8_t value = data.value<int8_t>();

      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_max)
    {
      etl::bitset_ext<8, int8_t>::buffer_type buffer;
      etl::bitset_ext<8, int8_t> data((unsigned long long)etl::integral_limits<int8_t>::max, buffer);
      int8_t value = data.value<int8_t>();

      CHECK_EQUAL(std::numeric_limits<int8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_min)
    {
      etl::bitset_ext<16, int16_t>::buffer_type buffer;
      etl::bitset_ext<16, int16_t> data((unsigned long long)etl::integral_limits<uint16_t>::min, buffer);
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_max)
    {
      etl::bitset_ext<16, int16_t>::buffer_type buffer;
      etl::bitset_ext<16, int16_t> data((unsigned long long)etl::integral_limits<uint16_t>::max, buffer);
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_min)
    {
      etl::bitset_ext<16, int16_t>::buffer_type buffer;
      etl::bitset_ext<16, int16_t> data((unsigned long long)etl::integral_limits<int16_t>::min, buffer);
      int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_max)
    {
      etl::bitset_ext<16, int16_t>::buffer_type buffer;
      etl::bitset_ext<16, int16_t> data((unsigned long long)etl::integral_limits<int16_t>::max, buffer);
      int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_min)
    {
      etl::bitset_ext<32, int32_t>::buffer_type buffer;
      etl::bitset_ext<32, int32_t> data((unsigned long long)etl::integral_limits<int32_t>::min, buffer);
      int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_max)
    {
      etl::bitset_ext<32, int32_t>::buffer_type buffer;
      etl::bitset_ext<32, int32_t> data((unsigned long long)etl::integral_limits<int32_t>::max, buffer);
      int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_min)
    {
      etl::bitset_ext<32, int32_t>::buffer_type buffer;
      etl::bitset_ext<32, int32_t> data((unsigned long long)etl::integral_limits<int32_t>::min, buffer);
      int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_max)
    {
      etl::bitset_ext<32, int32_t>::buffer_type buffer;
      etl::bitset_ext<32, int32_t> data((unsigned long long)etl::integral_limits<int32_t>::max, buffer);
      int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_min)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      etl::bitset_ext<64, int64_t> data((unsigned long long)etl::integral_limits<int64_t>::min, buffer);
      int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_max)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      etl::bitset_ext<64, int64_t> data((unsigned long long)etl::integral_limits<int64_t>::max, buffer);
      int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_min)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      etl::bitset_ext<64, int64_t> data((unsigned long long)etl::integral_limits<int64_t>::min, buffer);
      int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_max)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      etl::bitset_ext<64, int64_t> data((unsigned long long)etl::integral_limits<int64_t>::max, buffer);
      int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_position_set)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, int64_t> data(buffer);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        compare.set(i, (i % 2) == 0);
        data.set(i, (i % 2) == 0);
      }

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
    TEST(test_position_set_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset_ext<12, uint16_t>::buffer_type buffer;
      etl::bitset_ext<12, uint16_t> data(buffer);

      CHECK_THROW(data.set(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_reset)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<64, int64_t> data(0xFFFFFFFFFFFFFFFULL, buffer);

      compare.reset();
      data.reset();
      
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
    TEST(test_position_reset)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<64, int64_t> data(0xFFFFFFFFFFFFFFFULL, buffer);

      compare.reset(1);
      compare.reset(3);
      compare.reset(7);
      compare.reset(13);

      data.reset(1);
      data.reset(3);
      data.reset(7);
      data.reset(13);

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
      etl::bitset_ext<12, uint16_t>::buffer_type buffer;
      etl::bitset_ext<12, uint16_t> data(buffer);

      CHECK_THROW(data.reset(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_position_flip_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset_ext<12, uint16_t>::buffer_type buffer;
      etl::bitset_ext<12, uint16_t> data(buffer);

      CHECK_THROW(data.flip(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_position_test_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset_ext<12, uint16_t>::buffer_type buffer;
      etl::bitset_ext<12, uint16_t> data(buffer);

      CHECK_THROW(data.test(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_index_operator_read)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare(0x3123456731234567ULL);
      etl::bitset_ext<64, int64_t> data(0x3123456731234567ULL, buffer);

      bool bc0 = compare[1U];
      bool bd0 = data[1U];
      CHECK_EQUAL(bc0, bd0);

      bool bc3 = compare[3U];
      bool bd3 = data[3U];
      CHECK_EQUAL(bc3, bd3);

      bool bc7 = compare[7U];
      bool bd7 = data[7U];
      CHECK_EQUAL(bc7, bd7);

      bool bc13 = compare[13U];
      bool bd13 = data[13U];
      CHECK_EQUAL(bc13, bd13);
    }

    //*************************************************************************
    TEST(test_index_operator_write)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      etl::bitset_ext<64, int64_t> data(buffer);

      data[1U]  = true;
      data[3U]  = true;
      data[7U]  = true;
      data[13U] = true;

      CHECK(data[1U]);
      CHECK(data[3U]);
      CHECK(data[7U]);
      CHECK(data[13U]);
    }

    //*************************************************************************
    TEST(test_any)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;
      etl::bitset_ext<64, int64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<64, int64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, int64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

      bool bd1 = data1.any();
      bool bd2 = data2.any();
      bool bd3 = data3.any();
      bool bd4 = data4.any();

      CHECK_FALSE(bd1);
      CHECK_TRUE(bd2);
      CHECK_TRUE(bd3);
      CHECK_TRUE(bd4);
    }

    //*************************************************************************
    TEST(test_any_with_mask)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;
      etl::bitset_ext<64, int64_t> data1(ull(0xF000000000000000), buffer1);
      etl::bitset_ext<64, int64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, int64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

      ull mask = ull(0x0FFFFFFFFFFFFFFF);

      bool bd1 = data1.any(mask);
      bool bd2 = data2.any(mask);
      bool bd3 = data3.any(mask);
      bool bd4 = data4.any(mask);

      CHECK_FALSE(bd1);
      CHECK_TRUE(bd2);
      CHECK_TRUE(bd3);
      CHECK_TRUE(bd4);
    }

    //*************************************************************************
    TEST(test_none)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;
      etl::bitset_ext<64, int64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<64, int64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, int64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

      bool bd1 = data1.none();
      bool bd2 = data2.none();
      bool bd3 = data3.none();
      bool bd4 = data4.none();

      CHECK_TRUE(bd1);
      CHECK_FALSE(bd2);
      CHECK_FALSE(bd3);
      CHECK_FALSE(bd4);
    }

    //*************************************************************************
    TEST(test_none_with_mask)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;
      etl::bitset_ext<64, int64_t> data1(ull(0xF000000000000000), buffer1);
      etl::bitset_ext<64, int64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, int64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

      ull mask = ull(0x0FFFFFFFFFFFFFFF);

      bool bd1 = data1.none(mask);
      bool bd2 = data2.none(mask);
      bool bd3 = data3.none(mask);
      bool bd4 = data4.none(mask);

      CHECK_TRUE(bd1);
      CHECK_FALSE(bd2);
      CHECK_FALSE(bd3);
      CHECK_FALSE(bd4);
    }

    //*************************************************************************
    TEST(test_all)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;
      etl::bitset_ext<64, int64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<64, int64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, int64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

      bool bd1 = data1.all();
      bool bd2 = data2.all();
      bool bd3 = data3.all();
      bool bd4 = data4.all();

      CHECK_FALSE(bd1);
      CHECK_FALSE(bd2);
      CHECK_FALSE(bd3);
      CHECK_TRUE(bd4);
    }

    //*************************************************************************
    TEST(test_all_with_mask)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;
      etl::bitset_ext<64, int64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<64, int64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, int64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, int64_t> data4(ull(0x0FFFFFFFFFFFFFFF), buffer4);

      ull mask = ull(0x0FFFFFFFFFFFFFFF);

      bool bd1 = data1.all(mask);
      bool bd2 = data2.all(mask);
      bool bd3 = data3.all(mask);
      bool bd4 = data4.all(mask);

      CHECK_FALSE(bd1);
      CHECK_FALSE(bd2);
      CHECK_FALSE(bd3);
      CHECK_TRUE(bd4);
    }

    //*************************************************************************
    TEST(test_flip)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, int64_t> data(buffer);

      compare.flip();
      data.flip();

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_flip_position)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, int64_t> data(buffer);

      compare.flip(1);
      compare.flip(3);
      compare.flip(7);
      compare.flip(13);

      data.flip(1);
      data.flip(3);
      data.flip(7);
      data.flip(13);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_flip_reference)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, int64_t> data(buffer);

      compare[1].flip();
      compare[3].flip();
      compare[7].flip();
      compare[13].flip();

      data[1].flip();
      data[3].flip();
      data[7].flip();
      data[13].flip();

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_invert_reference)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, int64_t> data(buffer);

      bool bc = ~compare[3];
      bool bd = ~data[3];
      CHECK_EQUAL(bc, bd);
    }

    //*************************************************************************
    void test_assignment_operator_helper(etl::bitset_ext<64, int64_t>& from, etl::bitset_ext<64, int64_t>& to)
    {
      to = from;
    }

    TEST(test_assignment_operator)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<64, int64_t> data1(0xFFFFFFFFFFFFFFFULL, buffer1);
      etl::bitset_ext<64, int64_t> data2(buffer2);

      test_assignment_operator_helper(data1, data2);

      for (size_t i = 0UL; i < data2.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t> data1(0x123456781234567ULL, buffer1);
      etl::bitset_ext<64, int64_t> data2(0x123456781234567ULL, buffer2);
      etl::bitset_ext<64, int64_t> data3(buffer3);

      bool equal     =  (data1 == data2);
      bool not_equal = !(data1 == data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t> data1(0x123456781234567ULL, buffer1);
      etl::bitset_ext<64, int64_t> data2(0x123456781234567ULL, buffer2);
      etl::bitset_ext<64, int64_t> data3(buffer3);

      bool equal     = !(data1 != data2);
      bool not_equal = (data1 != data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_shift_left_operator)
    {
      int64_t value = 0x0123456789ABCDEFULL;
      uint64_t mask = 0xFFFFFFFFFFFFFFFFULL;

      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;

      etl::bitset_ext<64, int64_t> original(0x0123456789ABCDEFULL, buffer1);

      for (ull shift = 0U; shift < 64U; ++shift)
      {
        etl::bitset_ext<64, int64_t> data(original, buffer2);
        CHECK_EQUAL_HEX(((value & mask) << shift), (data <<= shift).value<int64_t>());
        mask >>= 1;
      }

      etl::bitset_ext<64, int64_t> data(original, buffer2);
      CHECK_EQUAL_HEX(0ULL, (data <<= 64U).value<int64_t>());
    }

    //*************************************************************************
    TEST(test_shift_left_operator_overflow)
    {
      etl::bitset_ext<32, int32_t>::buffer_type buffer1;
      etl::bitset_ext<32, int32_t>::buffer_type buffer2;

      etl::bitset_ext<32, int32_t> data(0xFFFFFFFFULL, buffer1);
      data <<= 1U;
      etl::bitset_ext<32, int32_t> shifted(0xFFFFFFFEUL, buffer2);

      CHECK_EQUAL_HEX(shifted.value<int32_t>(), data.value<int32_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_operator)
    {
      int64_t value = 0x0123456789ABCDEFULL;

      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;

      etl::bitset_ext<64, int64_t> original(0x0123456789ABCDEFULL, buffer1);
      
      for (ull shift = 0U; shift < 64U; ++shift)
      {
        etl::bitset_ext<64, int64_t> data(original, buffer2);
        CHECK_EQUAL_HEX((value >> shift), (data >>= shift).value<int64_t>());
      }

      etl::bitset_ext<64, int64_t> data(original, buffer2);
      CHECK_EQUAL_HEX(0ULL, (data >>= 64U).value<int64_t>());
    }

    //*************************************************************************
    TEST(test_and_equals_operator)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;

      etl::bitset_ext<64, int64_t> data1(0x12345678UL, buffer1);
      etl::bitset_ext<64, int64_t> data2(0x23456789UL, buffer2);
      etl::bitset_ext<64, int64_t> data3(0x12345678UL & 0x23456789UL, buffer3);

      etl::bitset_ext<64, int64_t> data4(data1, buffer4);
      data4 &= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_or_equals_operator)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;

      etl::bitset_ext<64, int64_t> data1(0x12345678UL, buffer1);
      etl::bitset_ext<64, int64_t> data2(0x23456789UL, buffer2);
      etl::bitset_ext<64, int64_t> data3(0x12345678UL | 0x23456789UL, buffer3);

      etl::bitset_ext<64, int64_t> data4(data1, buffer4);
      data4 |= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_xor_equals_operator)
    {
      etl::bitset_ext<64, int64_t>::buffer_type buffer1;
      etl::bitset_ext<64, int64_t>::buffer_type buffer2;
      etl::bitset_ext<64, int64_t>::buffer_type buffer3;
      etl::bitset_ext<64, int64_t>::buffer_type buffer4;

      etl::bitset_ext<64, int64_t> data1(0x12345678UL, buffer1);
      etl::bitset_ext<64, int64_t> data2(0x23456789UL, buffer2);
      etl::bitset_ext<64, int64_t> data3(0x12345678UL ^ 0x23456789UL, buffer3);

      etl::bitset_ext<64, int64_t> data4(data1, buffer4);
      data4 ^= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_find_first)
    {
      etl::bitset_ext<8, int8_t>::buffer_type buffer;

      etl::bitset_ext<8, int8_t> bs1(ull(0x00), buffer);
      size_t bs1fff = bs1.find_first(false);
      size_t bs1fft = bs1.find_first(true);
      CHECK_EQUAL(0U, bs1fff);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs1fft);

      etl::bitset_ext<8, int8_t> bs2(ull(0xFF), buffer);
      size_t bs2fff = bs2.find_first(false);
      size_t bs2fft = bs2.find_first(true);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs2fff);
      CHECK_EQUAL(0U, bs2fft);

      etl::bitset_ext<8, int8_t> bs3(ull(0x01), buffer);
      size_t bs3fff = bs3.find_first(false);
      size_t bs3fft = bs3.find_first(true);
      CHECK_EQUAL(1U, bs3fff);
      CHECK_EQUAL(0U, bs3fft);

      etl::bitset_ext<8, int8_t> bs4(ull(0x20), buffer);
      size_t bs4fff = bs4.find_first(false);
      size_t bs4fft = bs4.find_first(true);
      CHECK_EQUAL(0U, bs4fff);
      CHECK_EQUAL(5U, bs4fft);

      etl::bitset_ext<8, int8_t> bs5(ull(0x21), buffer);
      size_t bs5fff = bs5.find_first(false);
      size_t bs5fft = bs5.find_first(true);
      CHECK_EQUAL(1U, bs5fff);
      CHECK_EQUAL(0U, bs5fft);

      etl::bitset_ext<8, int8_t> bs6(ull(0x0E), buffer);
      size_t bs6fff = bs6.find_first(false);
      size_t bs6fft = bs6.find_first(true);
      CHECK_EQUAL(0U, bs6fff);
      CHECK_EQUAL(1U, bs6fft);

      etl::bitset_ext<8, int8_t> bs7(ull(0x31), buffer);
      size_t bs7fff = bs7.find_first(false);
      size_t bs7fft = bs7.find_first(true);
      CHECK_EQUAL(1U, bs7fff);
      CHECK_EQUAL(0U, bs7fft);
    }

    //*************************************************************************
    TEST(test_find_next)
    {
      etl::bitset_ext<8, int8_t>::buffer_type buffer;

      etl::bitset_ext<8, int8_t> bs1(ull(0x00), buffer);
      size_t bs1fnf0 = bs1.find_next(false, 0);
      size_t bs1fnf1 = bs1.find_next(false, 1);
      size_t bs1fnt2 = bs1.find_next(true, 2);
      CHECK_EQUAL(0U, bs1fnf0);
      CHECK_EQUAL(1U, bs1fnf1);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs1fnt2);

      etl::bitset_ext<8, int8_t> bs2(ull(0xFF), buffer);
      size_t bs2fnt0 = bs2.find_next(true, 0);
      size_t bs2fnt1 = bs2.find_next(true, 1);
      size_t bs2fnf2 = bs2.find_next(false, 2);
      CHECK_EQUAL(0U, bs2fnt0);
      CHECK_EQUAL(1U, bs2fnt1);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs2fnf2);

      etl::bitset_ext<8, int8_t> bs3(ull(0x0E), buffer);
      size_t bs3fnf0 = bs3.find_next(false, 0);
      size_t bs3fnt1 = bs3.find_next(true, 1);
      size_t bs3fnf2 = bs3.find_next(false, 2);
      CHECK_EQUAL(0U, bs3fnf0);
      CHECK_EQUAL(1U, bs3fnt1);
      CHECK_EQUAL(4U, bs3fnf2);

      etl::bitset_ext<8, int8_t> bs4(ull(0x31), buffer);
      size_t bs4fnt0 = bs4.find_next(true, 0);
      size_t bs4fnf0 = bs4.find_next(false, 0);
      size_t bs4fnt1 = bs4.find_next(true, 1);
      CHECK_EQUAL(0U, bs4fnt0);
      CHECK_EQUAL(1U, bs4fnf0);
      CHECK_EQUAL(4U, bs4fnt1);
    }

    //*************************************************************************
    TEST(test_swap)
    {
      etl::bitset_ext<16, int16_t>::buffer_type buffer1;
      etl::bitset_ext<16, int16_t>::buffer_type buffer2;
      etl::bitset_ext<16, int16_t>::buffer_type buffer3;
      etl::bitset_ext<16, int16_t>::buffer_type buffer4;

      etl::bitset_ext<16, int16_t> compare1(0x2A, buffer1);
      etl::bitset_ext<16, int16_t> compare2(0x15, buffer2);
      etl::bitset_ext<16, int16_t> compare1a(0x2A, buffer3);
      etl::bitset_ext<16, int16_t> compare2a(0x15, buffer4);

      swap(compare1a, compare2a);

      CHECK(compare1 == compare2a);
      CHECK(compare2 == compare1a);
    }

    //*************************************************************************
    TEST(test_span)
    {
      etl::bitset_ext<32, int32_t>::buffer_type buffer; 
      etl::bitset_ext<32, uint32_t> data(0x12345678UL, buffer);

      using span_t = etl::bitset_ext<32, uint32_t>::span_type;

      span_t s = data.span();
      CHECK_EQUAL(0x12345678UL, s[0]);
    }

    //*************************************************************************
    TEST(test_const_span)
    {
      etl::bitset_ext<32, int32_t>::buffer_type buffer;
      const etl::bitset_ext<32, uint32_t> data(0x12345678UL, buffer);

      using span_t = const etl::bitset_ext<32, uint32_t>::const_span_type;

      span_t s = data.span();
      CHECK_EQUAL(0x12345678UL, s[0]);
    }

    //*************************************************************************
    TEST(test_to_string)
    {
      using bs32 = etl::bitset_ext<32, int32_t>;

      bs32::buffer_type buffer;
      bs32 data(0x12345678UL, buffer);

      etl::string<32> text = data.to_string('.', '*');
      std::string stdtext  = data.to_string<std::string>('.', '*');

      //CHECK_THROW(b.to_string<etl::string<30>>('.', '*'), etl::bitset_ext_overflow);
      CHECK_EQUAL(std::string("...*..*...**.*...*.*.**..****..."), std::string(text.c_str()));
      CHECK_EQUAL(std::string("...*..*...**.*...*.*.**..****..."), std::string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_wstring)
    {
      using bs32 = etl::bitset_ext<32, int32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      etl::wstring<32> text = b.to_string<etl::wstring<32>>(L'.', L'*');
      std::wstring stdtext = b.to_string<std::wstring>(L'.', L'*');

      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(text.c_str()));
      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u16string)
    {
      using bs32 = etl::bitset_ext<32, int32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      etl::u16string<32> text = b.to_string<etl::u16string<32>>(u'.', u'*');
      std::u16string stdtext = b.to_string<std::u16string>(u'.', u'*');

      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(text.c_str()));
      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u32string)
    {
      using bs32 = etl::bitset_ext<32, int32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      etl::u32string<32> text = b.to_string<etl::u32string<32>>(U'.', U'*');
      std::u32string stdtext = b.to_string<std::u32string>(U'.', U'*');

      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(text.c_str()));
      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(stdtext.c_str()));
    }
  };
}
