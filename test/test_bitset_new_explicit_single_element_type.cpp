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

  //*************************************************************************
  template <size_t Active_Bits, typename TElement>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits, TElement> generate_shift_left_bitset(ull value, size_t shift)
  {
    ull mask = 0ULL;
    
    if (shift < 64U)
    {
      mask = etl::integral_limits<ull>::max >> shift;
    }
     
    value &= mask;

    etl::bitset<Active_Bits, TElement> data(value);

    if (shift >= 64U)
    {
      data.reset();
    }
    else if (shift != 0U)
    {
      data <<= shift;
    }

    return data;
  }

  //*************************************************************************
  template <size_t Active_Bits, typename TElement>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits, TElement> generate_shift_left_bitset_copy(ull value, size_t shift)
  {
    etl::bitset<Active_Bits, TElement> data1(value);
    etl::bitset<Active_Bits, TElement> data2;

    if ((shift != 0U) && (shift != 64U))
    {
      data2 = data1 << shift;
    }

    return data2;
  }

  //*************************************************************************
  template <size_t Active_Bits, typename TElement>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits, TElement> generate_shift_right_bitset(ull value, size_t shift)
  {
    etl::bitset<Active_Bits, TElement> data(value);

    if (shift >= 64U)
    {
      data.reset();
    }
    else if (shift != 0U)
    {
      data >>= shift;
    }

    return data;
  }

  //*************************************************************************
  template <size_t Active_Bits, typename TElement>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits, TElement> generate_shift_right_bitset_copy(ull value, size_t shift)
  {
    etl::bitset<Active_Bits, TElement> data1(value);
    etl::bitset<Active_Bits, TElement> data2;

    if (shift != 0U)
    {
      data2 = data1 >> shift;
    }

    return data2;
  }

  SUITE(test_bitset_new_implicit_element_type)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      ETL_CONSTEXPR14 std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data;

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_default_constructor_single_element)
    {
      etl::bitset<8,  int8_t>  data8;
      etl::bitset<16, int16_t> data16;
      etl::bitset<32, int32_t> data32;
      etl::bitset<64, int64_t> data64;

      CHECK_EQUAL(1U, data8.Number_Of_Elements);
      CHECK_EQUAL(1U, data16.Number_Of_Elements);
      CHECK_EQUAL(1U, data32.Number_Of_Elements);
      CHECK_EQUAL(1U, data64.Number_Of_Elements);
    }

    //*************************************************************************
    TEST(test_construct_from_value)
    {
      ETL_CONSTEXPR14 std::bitset<64> compare(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(0x123456731234567ULL);

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
      ETL_CONSTEXPR14 std::bitset<64> compare(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data_copy(data);

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
      ETL_CONSTEXPR14 std::bitset<64> compare(0x8765432187654321ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(0x8765432187654321ULL);

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
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data("110001001000110100010101100111001100010010001101000101011001");

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
      std::bitset<64> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data(L"110001001000110100010101100111001100010010001101000101011001");

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
      std::bitset<64> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data(u"110001001000110100010101100111001100010010001101000101011001");

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
      std::bitset<64> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data(U"110001001000110100010101100111001100010010001101000101011001");

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
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001111100001");
      etl::bitset<64, int64_t> data("110001001000110100010101100111001100010010001101000101011001111100001");

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
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_set_helper()
    {
      etl::bitset<64, int64_t> data;
      data.set();

      return data;
    }
     
    TEST(test_set)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(test_set_helper());

      compare.set();

      ETL_CONSTEXPR14 auto size  = data.size();
      ETL_CONSTEXPR14 auto count = data.count();
      ETL_CONSTEXPR14 auto none  = data.none();
      ETL_CONSTEXPR14 auto any   = data.any();
      ETL_CONSTEXPR14 auto all   = data.all();

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
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data;

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
      std::bitset<64> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data;

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
      std::bitset<64> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data;

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
      std::bitset<64> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data;

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
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data;

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
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data;

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
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data;

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
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64, int64_t> data;

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
      ETL_CONSTEXPR14 etl::bitset<8> data((unsigned long long)etl::integral_limits<uint8_t>::min);
      ETL_CONSTEXPR14 uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u8_max)
    {
      ETL_CONSTEXPR14 etl::bitset<8> data((unsigned long long)etl::integral_limits<uint8_t>::max);
      ETL_CONSTEXPR14 uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_min)
    {
      ETL_CONSTEXPR14 etl::bitset<8> data((unsigned long long)etl::integral_limits<int8_t>::min);
      ETL_CONSTEXPR14 int8_t value = data.value<int8_t>();

      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_max)
    {
      ETL_CONSTEXPR14 etl::bitset<8> data((unsigned long long)etl::integral_limits<int8_t>::max);
      ETL_CONSTEXPR14 int8_t value = data.value<int8_t>();

      CHECK_EQUAL(std::numeric_limits<int8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_min)
    {
      ETL_CONSTEXPR14 etl::bitset<16> data((unsigned long long)etl::integral_limits<uint16_t>::min);
      ETL_CONSTEXPR14 uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_max)
    {
      ETL_CONSTEXPR14 etl::bitset<16> data((unsigned long long)etl::integral_limits<uint16_t>::max);
      ETL_CONSTEXPR14 uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_min)
    {
      ETL_CONSTEXPR14 etl::bitset<16> data((unsigned long long)etl::integral_limits<int16_t>::min);
      ETL_CONSTEXPR14 int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_max)
    {
      ETL_CONSTEXPR14 etl::bitset<16> data((unsigned long long)etl::integral_limits<int16_t>::max);
      ETL_CONSTEXPR14 int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_min)
    {
      ETL_CONSTEXPR14 etl::bitset<32> data((unsigned long long)etl::integral_limits<int32_t>::min);
      ETL_CONSTEXPR14 int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_max)
    {
      ETL_CONSTEXPR14 etl::bitset<32> data((unsigned long long)etl::integral_limits<int32_t>::max);
      ETL_CONSTEXPR14 int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_min)
    {
      ETL_CONSTEXPR14 etl::bitset<32> data((unsigned long long)etl::integral_limits<int32_t>::min);
      ETL_CONSTEXPR14 int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_max)
    {
      ETL_CONSTEXPR14 etl::bitset<32> data((unsigned long long)etl::integral_limits<int32_t>::max);
      ETL_CONSTEXPR14 int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_min)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::min);
      ETL_CONSTEXPR14 int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_max)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::max);
      ETL_CONSTEXPR14 int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_min)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::min);
      ETL_CONSTEXPR14 int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_max)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::max);
      ETL_CONSTEXPR14 int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_position_set)
    {
      std::bitset<64> compare;
      etl::bitset<64, int64_t> data;

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
      etl::bitset<12, uint16_t> data;

      CHECK_THROW(data.set(13), etl::bitset_overflow);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_reset_helper()
    {
      etl::bitset<64, int64_t> data(0xFFFFFFFFFFFFFFFULL);
      data.reset();

      return data;
    }
    
    TEST(test_reset)
    {
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(test_reset_helper());

      compare.reset();
      
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
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_position_reset_helper()
    {
      etl::bitset<64, int64_t> data(0xFFFFFFFFFFFFFFFULL);
      data.reset(1);
      data.reset(3);
      data.reset(7);
      data.reset(13);

      return data;
    }

    TEST(test_position_reset)
    {
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(test_position_reset_helper());

      compare.reset(1);
      compare.reset(3);
      compare.reset(7);
      compare.reset(13);

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
      etl::bitset<12, uint16_t> data;

      CHECK_THROW(data.reset(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_position_flip_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset<12, uint16_t> data;

      CHECK_THROW(data.flip(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_position_test_invalid_position_raises_bitset_overflow_github_issue_722)
    {
      etl::bitset<12, uint16_t> data;

      CHECK_THROW(data.test(13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_index_operator_read)
    {
      ETL_CONSTEXPR14 std::bitset<64> compare(0x3123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(0x3123456731234567ULL);

      ETL_CONSTEXPR14 bool bc0 = compare[1U];
      ETL_CONSTEXPR14 bool bd0 = data[1U];
      CHECK_EQUAL(bc0, bd0);

      ETL_CONSTEXPR14 bool bc3 = compare[3U];
      ETL_CONSTEXPR14 bool bd3 = data[3U];
      CHECK_EQUAL(bc3, bd3);

      ETL_CONSTEXPR14 bool bc7 = compare[7U];
      ETL_CONSTEXPR14 bool bd7 = data[7U];
      CHECK_EQUAL(bc7, bd7);

      ETL_CONSTEXPR14 bool bc13 = compare[13U];
      ETL_CONSTEXPR14 bool bd13 = data[13U];
      CHECK_EQUAL(bc13, bd13);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_index_operator_write_helper()
    {
      etl::bitset<64, int64_t> data;

      data[1U]  = true;
      data[3U]  = true;
      data[7U]  = true;
      data[13U] = true;

      return data;
    }

    TEST(test_index_operator_write)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(test_index_operator_write_helper());

      CHECK(data[1U]);
      CHECK(data[3U]);
      CHECK(data[7U]);
      CHECK(data[13U]);
    }

    //*************************************************************************
    TEST(test_any)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF));

      ETL_CONSTEXPR14 bool bd1 = data1.any();
      ETL_CONSTEXPR14 bool bd2 = data2.any();
      ETL_CONSTEXPR14 bool bd3 = data3.any();
      ETL_CONSTEXPR14 bool bd4 = data4.any();

      CHECK_FALSE(bd1);
      CHECK_TRUE(bd2);
      CHECK_TRUE(bd3);
      CHECK_TRUE(bd4);
    }

    //*************************************************************************
    TEST(test_any_with_mask)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(ull(0xF000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF));

      ETL_CONSTEXPR14 ull mask = ull(0x0FFFFFFFFFFFFFFF);

      ETL_CONSTEXPR14 bool bd1 = data1.any(mask);
      ETL_CONSTEXPR14 bool bd2 = data2.any(mask);
      ETL_CONSTEXPR14 bool bd3 = data3.any(mask);
      ETL_CONSTEXPR14 bool bd4 = data4.any(mask);

      CHECK_FALSE(bd1);
      CHECK_TRUE(bd2);
      CHECK_TRUE(bd3);
      CHECK_TRUE(bd4);
    }

    //*************************************************************************
    TEST(test_none)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF));

      ETL_CONSTEXPR14 bool bd1 = data1.none();
      ETL_CONSTEXPR14 bool bd2 = data2.none();
      ETL_CONSTEXPR14 bool bd3 = data3.none();
      ETL_CONSTEXPR14 bool bd4 = data4.none();

      CHECK_TRUE(bd1);
      CHECK_FALSE(bd2);
      CHECK_FALSE(bd3);
      CHECK_FALSE(bd4);
    }

    //*************************************************************************
    TEST(test_none_with_mask)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(ull(0xF000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF));

      ETL_CONSTEXPR14 ull mask = ull(0x0FFFFFFFFFFFFFFF);

      ETL_CONSTEXPR14 bool bd1 = data1.none(mask);
      ETL_CONSTEXPR14 bool bd2 = data2.none(mask);
      ETL_CONSTEXPR14 bool bd3 = data3.none(mask);
      ETL_CONSTEXPR14 bool bd4 = data4.none(mask);

      CHECK_TRUE(bd1);
      CHECK_FALSE(bd2);
      CHECK_FALSE(bd3);
      CHECK_FALSE(bd4);
    }

    //*************************************************************************
    TEST(test_all)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(ull(0xFFFFFFFFFFFFFFFF));

      ETL_CONSTEXPR14 bool bd1 = data1.all();
      ETL_CONSTEXPR14 bool bd2 = data2.all();
      ETL_CONSTEXPR14 bool bd3 = data3.all();
      ETL_CONSTEXPR14 bool bd4 = data4.all();

      CHECK_FALSE(bd1);
      CHECK_FALSE(bd2);
      CHECK_FALSE(bd3);
      CHECK_TRUE(bd4);
    }

    //*************************************************************************
    TEST(test_all_with_mask)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(ull(0x0FFFFFFFFFFFFFFF));

      ETL_CONSTEXPR14 ull mask = ull(0x0FFFFFFFFFFFFFFF);

      ETL_CONSTEXPR14 bool bd1 = data1.all(mask);
      ETL_CONSTEXPR14 bool bd2 = data2.all(mask);
      ETL_CONSTEXPR14 bool bd3 = data3.all(mask);
      ETL_CONSTEXPR14 bool bd4 = data4.all(mask);

      CHECK_FALSE(bd1);
      CHECK_FALSE(bd2);
      CHECK_FALSE(bd3);
      CHECK_TRUE(bd4);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_flip_helper()
    {
      etl::bitset<64, int64_t> data;
      data.flip();

      return data;
    }

    TEST(test_flip)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(test_flip_helper());

      compare.flip();

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_flip_position_helper()
    {
      etl::bitset<64, int64_t> data;
      data.flip(1);
      data.flip(3);
      data.flip(7);
      data.flip(13);

      return data;
    }
    
    TEST(test_flip_position)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(test_flip_position_helper());

      compare.flip(1);
      compare.flip(3);
      compare.flip(7);
      compare.flip(13);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_flip_reference_helper()
    {
      etl::bitset<64, int64_t> data;
      data[1].flip();
      data[3].flip();
      data[7].flip();
      data[13].flip();

      return data;
    }

    TEST(test_flip_reference)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data(test_flip_reference_helper());

      compare[1].flip();
      compare[3].flip();
      compare[7].flip();
      compare[13].flip();

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_invert_reference)
    {
      std::bitset<64> compare;
      etl::bitset<64, int64_t> data;

      bool bc = ~compare[3];
      bool bd = ~data[3];
      CHECK_EQUAL(bc, bd);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_assignment_operator_helper(etl::bitset<64, int64_t> to, const etl::bitset<64, int64_t>& from)
    {
      to = from;

      return to;
    }

    TEST(test_assignment_operator)
    {
      ETL_CONSTEXPR14 std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0xFFFFFFFFFFFFFFFULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(test_assignment_operator_helper(etl::bitset<64, int64_t>(), data1));

      for (size_t i = 0UL; i < data2.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3;

      ETL_CONSTEXPR14 bool equal     =  (data1 == data2);
      ETL_CONSTEXPR14 bool not_equal = !(data1 == data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3;

      ETL_CONSTEXPR14 bool equal = !(data1 != data2);
      ETL_CONSTEXPR14 bool not_equal = (data1 != data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_shift_left_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift1(0x2468ACF0ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift2(0x48D159E0ULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift11(0x91A2B3C000ULL);

      CHECK((generate_shift_left_bitset<64, int64_t>(0x12345678ULL, 1U)) == shift1);
      CHECK((generate_shift_left_bitset<64, int64_t>(0x12345678ULL, 2U)) == shift2);
      CHECK((generate_shift_left_bitset<64, int64_t>(0x12345678ULL, 11U)) == shift11);
    }

    //*************************************************************************
    TEST(test_shift_left_operator_overflow)
    {
      ETL_CONSTEXPR14 etl::bitset<32, int32_t> data(generate_shift_left_bitset<32, int32_t>(0x7FFFFFFFULL, 1U));
      ETL_CONSTEXPR14 etl::bitset<32, int32_t> shifted(0xFFFFFFFEUL);

      CHECK_EQUAL_HEX(shifted.value<int32_t>(), data.value<int32_t>());
    }

    //*************************************************************************
    TEST(test_shift_left_copy_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift8(0x1234567800ULL);
      CHECK_EQUAL_HEX(shift8.value<int64_t>(), (generate_shift_left_bitset_copy<64, int64_t>(0x12345678UL, 8U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift16(0x123456780000ULL);
      CHECK_EQUAL_HEX(shift16.value<int64_t>(), (generate_shift_left_bitset_copy<64, int64_t>(0x12345678UL, 16U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift24(0x12345678000000ULL);
      CHECK_EQUAL_HEX(shift24.value<int64_t>(), (generate_shift_left_bitset_copy<64, int64_t>(0x12345678UL, 24U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift64(0x0000000000000000ULL);
      CHECK_EQUAL_HEX(shift64.value<int64_t>(), (generate_shift_left_bitset_copy<64, int64_t>(0x12345678UL, 64U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_left_operator_all_shifts_full_size)
    {
      uint64_t value = 0x0123456789ABCDEFULL;

      CHECK_EQUAL_HEX((value <<  0U), (generate_shift_left_bitset<64, uint64_t>(value,  0U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  1U), (generate_shift_left_bitset<64, uint64_t>(value,  1U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  2U), (generate_shift_left_bitset<64, uint64_t>(value,  2U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  3U), (generate_shift_left_bitset<64, uint64_t>(value,  3U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  4U), (generate_shift_left_bitset<64, uint64_t>(value,  4U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  5U), (generate_shift_left_bitset<64, uint64_t>(value,  5U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  6U), (generate_shift_left_bitset<64, uint64_t>(value,  6U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  7U), (generate_shift_left_bitset<64, uint64_t>(value,  7U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  8U), (generate_shift_left_bitset<64, uint64_t>(value,  8U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value <<  9U), (generate_shift_left_bitset<64, uint64_t>(value,  9U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 10U), (generate_shift_left_bitset<64, uint64_t>(value, 10U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 11U), (generate_shift_left_bitset<64, uint64_t>(value, 11U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 12U), (generate_shift_left_bitset<64, uint64_t>(value, 12U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 13U), (generate_shift_left_bitset<64, uint64_t>(value, 13U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 14U), (generate_shift_left_bitset<64, uint64_t>(value, 14U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 15U), (generate_shift_left_bitset<64, uint64_t>(value, 15U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 16U), (generate_shift_left_bitset<64, uint64_t>(value, 16U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 17U), (generate_shift_left_bitset<64, uint64_t>(value, 17U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 18U), (generate_shift_left_bitset<64, uint64_t>(value, 18U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 19U), (generate_shift_left_bitset<64, uint64_t>(value, 19U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 20U), (generate_shift_left_bitset<64, uint64_t>(value, 20U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 21U), (generate_shift_left_bitset<64, uint64_t>(value, 21U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 22U), (generate_shift_left_bitset<64, uint64_t>(value, 22U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 23U), (generate_shift_left_bitset<64, uint64_t>(value, 23U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 24U), (generate_shift_left_bitset<64, uint64_t>(value, 24U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 25U), (generate_shift_left_bitset<64, uint64_t>(value, 25U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 26U), (generate_shift_left_bitset<64, uint64_t>(value, 26U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 27U), (generate_shift_left_bitset<64, uint64_t>(value, 27U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 28U), (generate_shift_left_bitset<64, uint64_t>(value, 28U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 29U), (generate_shift_left_bitset<64, uint64_t>(value, 29U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 30U), (generate_shift_left_bitset<64, uint64_t>(value, 30U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 31U), (generate_shift_left_bitset<64, uint64_t>(value, 31U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 32U), (generate_shift_left_bitset<64, uint64_t>(value, 32U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 33U), (generate_shift_left_bitset<64, uint64_t>(value, 33U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 34U), (generate_shift_left_bitset<64, uint64_t>(value, 34U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 35U), (generate_shift_left_bitset<64, uint64_t>(value, 35U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 36U), (generate_shift_left_bitset<64, uint64_t>(value, 36U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 37U), (generate_shift_left_bitset<64, uint64_t>(value, 37U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 38U), (generate_shift_left_bitset<64, uint64_t>(value, 38U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 39U), (generate_shift_left_bitset<64, uint64_t>(value, 39U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 40U), (generate_shift_left_bitset<64, uint64_t>(value, 40U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 41U), (generate_shift_left_bitset<64, uint64_t>(value, 41U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 42U), (generate_shift_left_bitset<64, uint64_t>(value, 42U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 43U), (generate_shift_left_bitset<64, uint64_t>(value, 43U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 44U), (generate_shift_left_bitset<64, uint64_t>(value, 44U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 45U), (generate_shift_left_bitset<64, uint64_t>(value, 45U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 46U), (generate_shift_left_bitset<64, uint64_t>(value, 46U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 47U), (generate_shift_left_bitset<64, uint64_t>(value, 47U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 48U), (generate_shift_left_bitset<64, uint64_t>(value, 48U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 49U), (generate_shift_left_bitset<64, uint64_t>(value, 49U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 50U), (generate_shift_left_bitset<64, uint64_t>(value, 50U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 51U), (generate_shift_left_bitset<64, uint64_t>(value, 51U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 52U), (generate_shift_left_bitset<64, uint64_t>(value, 52U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 53U), (generate_shift_left_bitset<64, uint64_t>(value, 53U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 54U), (generate_shift_left_bitset<64, uint64_t>(value, 54U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 55U), (generate_shift_left_bitset<64, uint64_t>(value, 55U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 56U), (generate_shift_left_bitset<64, uint64_t>(value, 56U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 57U), (generate_shift_left_bitset<64, uint64_t>(value, 57U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 58U), (generate_shift_left_bitset<64, uint64_t>(value, 58U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 59U), (generate_shift_left_bitset<64, uint64_t>(value, 59U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 60U), (generate_shift_left_bitset<64, uint64_t>(value, 60U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 61U), (generate_shift_left_bitset<64, uint64_t>(value, 61U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 62U), (generate_shift_left_bitset<64, uint64_t>(value, 62U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 63U), (generate_shift_left_bitset<64, uint64_t>(value, 63U).value<uint64_t>()));
      CHECK_EQUAL_HEX(0ULL,           (generate_shift_left_bitset<64, uint64_t>(value, 64U).value<uint64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift1(0x91A2B3CULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift2(0x48D159EULL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift11(0x2468AULL);

      CHECK_EQUAL_HEX(shift1.value<int64_t>(),  (generate_shift_right_bitset<64, int64_t>(0x12345678ULL, 1U).value<int64_t>()));
      CHECK_EQUAL_HEX(shift2.value<int64_t>(),  (generate_shift_right_bitset<64, int64_t>(0x12345678ULL, 2U).value<int64_t>()));
      CHECK_EQUAL_HEX(shift11.value<int64_t>(), (generate_shift_right_bitset<64, int64_t>(0x12345678ULL, 11U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_copy_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift8(0x123456ULL);
      CHECK_EQUAL_HEX(shift8.value<int64_t>(), (generate_shift_right_bitset_copy<64, int64_t>(0x12345678UL, 8U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift16(0x1234ULL);
      CHECK_EQUAL_HEX(shift16.value<int64_t>(), (generate_shift_right_bitset_copy<64, int64_t>(0x12345678UL, 16U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift24(0x12ULL);
      CHECK_EQUAL_HEX(shift24.value<int64_t>(), (generate_shift_right_bitset_copy<64, int64_t>(0x12345678UL, 24U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> shift60(0x00ULL);
      CHECK_EQUAL_HEX(shift60.value<int64_t>(), (generate_shift_right_bitset_copy<64, int64_t>(0x12345678UL, 60U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_full_size)
    {
      int64_t value = 0x0123456789ABCDEFULL;

      CHECK_EQUAL_HEX((value >> 0U), (generate_shift_right_bitset<64, int64_t>(value, 0U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 1U), (generate_shift_right_bitset<64, int64_t>(value, 1U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 2U), (generate_shift_right_bitset<64, int64_t>(value, 2U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 3U), (generate_shift_right_bitset<64, int64_t>(value, 3U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 4U), (generate_shift_right_bitset<64, int64_t>(value, 4U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 5U), (generate_shift_right_bitset<64, int64_t>(value, 5U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 6U), (generate_shift_right_bitset<64, int64_t>(value, 6U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 7U), (generate_shift_right_bitset<64, int64_t>(value, 7U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 8U), (generate_shift_right_bitset<64, int64_t>(value, 8U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 9U), (generate_shift_right_bitset<64, int64_t>(value, 9U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 10U), (generate_shift_right_bitset<64, int64_t>(value, 10U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 11U), (generate_shift_right_bitset<64, int64_t>(value, 11U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 12U), (generate_shift_right_bitset<64, int64_t>(value, 12U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 13U), (generate_shift_right_bitset<64, int64_t>(value, 13U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 14U), (generate_shift_right_bitset<64, int64_t>(value, 14U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 15U), (generate_shift_right_bitset<64, int64_t>(value, 15U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 16U), (generate_shift_right_bitset<64, int64_t>(value, 16U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 17U), (generate_shift_right_bitset<64, int64_t>(value, 17U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 18U), (generate_shift_right_bitset<64, int64_t>(value, 18U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 19U), (generate_shift_right_bitset<64, int64_t>(value, 19U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 20U), (generate_shift_right_bitset<64, int64_t>(value, 20U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 21U), (generate_shift_right_bitset<64, int64_t>(value, 21U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 22U), (generate_shift_right_bitset<64, int64_t>(value, 22U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 23U), (generate_shift_right_bitset<64, int64_t>(value, 23U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 24U), (generate_shift_right_bitset<64, int64_t>(value, 24U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 25U), (generate_shift_right_bitset<64, int64_t>(value, 25U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 26U), (generate_shift_right_bitset<64, int64_t>(value, 26U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 27U), (generate_shift_right_bitset<64, int64_t>(value, 27U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 28U), (generate_shift_right_bitset<64, int64_t>(value, 28U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 29U), (generate_shift_right_bitset<64, int64_t>(value, 29U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 30U), (generate_shift_right_bitset<64, int64_t>(value, 30U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 31U), (generate_shift_right_bitset<64, int64_t>(value, 31U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 32U), (generate_shift_right_bitset<64, int64_t>(value, 32U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 33U), (generate_shift_right_bitset<64, int64_t>(value, 33U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 34U), (generate_shift_right_bitset<64, int64_t>(value, 34U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 35U), (generate_shift_right_bitset<64, int64_t>(value, 35U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 36U), (generate_shift_right_bitset<64, int64_t>(value, 36U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 37U), (generate_shift_right_bitset<64, int64_t>(value, 37U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 38U), (generate_shift_right_bitset<64, int64_t>(value, 38U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 39U), (generate_shift_right_bitset<64, int64_t>(value, 39U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 40U), (generate_shift_right_bitset<64, int64_t>(value, 40U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 41U), (generate_shift_right_bitset<64, int64_t>(value, 41U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 42U), (generate_shift_right_bitset<64, int64_t>(value, 42U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 43U), (generate_shift_right_bitset<64, int64_t>(value, 43U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 44U), (generate_shift_right_bitset<64, int64_t>(value, 44U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 45U), (generate_shift_right_bitset<64, int64_t>(value, 45U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 46U), (generate_shift_right_bitset<64, int64_t>(value, 46U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 47U), (generate_shift_right_bitset<64, int64_t>(value, 47U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 48U), (generate_shift_right_bitset<64, int64_t>(value, 48U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 49U), (generate_shift_right_bitset<64, int64_t>(value, 49U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 50U), (generate_shift_right_bitset<64, int64_t>(value, 50U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 51U), (generate_shift_right_bitset<64, int64_t>(value, 51U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 52U), (generate_shift_right_bitset<64, int64_t>(value, 52U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 53U), (generate_shift_right_bitset<64, int64_t>(value, 53U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 54U), (generate_shift_right_bitset<64, int64_t>(value, 54U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 55U), (generate_shift_right_bitset<64, int64_t>(value, 55U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 56U), (generate_shift_right_bitset<64, int64_t>(value, 56U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 57U), (generate_shift_right_bitset<64, int64_t>(value, 57U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 58U), (generate_shift_right_bitset<64, int64_t>(value, 58U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 59U), (generate_shift_right_bitset<64, int64_t>(value, 59U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 60U), (generate_shift_right_bitset<64, int64_t>(value, 60U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 61U), (generate_shift_right_bitset<64, int64_t>(value, 61U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 62U), (generate_shift_right_bitset<64, int64_t>(value, 62U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 63U), (generate_shift_right_bitset<64, int64_t>(value, 63U).value<int64_t>()));
      CHECK_EQUAL_HEX(0ULL, (generate_shift_right_bitset<64, int64_t>(value, 64U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_partial_size)
    {
      int64_t value = 0x0123456789ABCDEFULL;
      int64_t mask  = 0x0FFFFFFFFFFFFFFFULL;

      CHECK_EQUAL_HEX((value >> 0U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 0U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 1U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 1U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 2U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 2U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 3U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 3U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 4U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 4U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 5U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 5U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 6U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 6U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 7U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 7U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 8U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 8U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 9U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 9U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 10U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 10U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 11U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 11U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 12U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 12U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 13U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 13U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 14U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 14U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 15U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 15U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 16U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 16U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 17U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 17U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 18U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 18U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 19U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 19U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 20U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 20U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 21U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 21U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 22U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 22U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 23U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 23U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 24U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 24U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 25U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 25U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 26U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 26U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 27U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 27U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 28U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 28U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 29U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 29U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 30U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 30U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 31U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 31U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 32U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 32U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 33U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 33U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 34U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 34U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 35U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 35U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 36U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 36U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 37U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 37U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 38U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 38U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 39U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 39U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 40U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 40U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 41U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 41U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 42U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 42U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 43U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 43U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 44U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 44U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 45U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 45U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 46U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 46U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 47U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 47U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 48U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 48U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 49U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 49U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 50U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 50U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 51U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 51U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 52U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 52U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 53U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 53U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 54U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 54U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 55U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 55U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 56U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 56U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 57U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 57U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 58U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 58U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 59U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 59U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 60U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 60U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 61U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 61U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 62U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 62U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 63U) & mask, (generate_shift_right_bitset<64, int64_t>(value, 63U).value<int64_t>()));
      CHECK_EQUAL_HEX(0ULL, (generate_shift_right_bitset<64, int64_t>(value, 64U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_and_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(0x23456789UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(0x12345678UL & 0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3 = data1 & data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_and_equals_operator_helper(etl::bitset<64, int64_t> data1, const etl::bitset<64, int64_t>& data2)
    {
      data1 &= data2;

      return data1;
    }
    
    TEST(test_and_equals_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(0x23456789UL);
      
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(test_and_equals_operator_helper(data1, data2));

      CHECK((data1 & data2) == data3);
    }

    //*************************************************************************
    TEST(test_or_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(0x23456789UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(0x12345678UL | 0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3 = data1 | data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_or_equals_operator_helper(etl::bitset<64, int64_t> data1, const etl::bitset<64, int64_t>& data2)
    {
      data1 |= data2;

      return data1;
    }

    TEST(test_or_equals_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(test_or_equals_operator_helper(data1, data2));

      CHECK((data1 | data2) == data3);
    }

    //*************************************************************************
    TEST(test_xor_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(0x23456789UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data4(0x12345678UL ^ 0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3 = data1 ^ data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64, int64_t> test_xor_equals_operator_helper(etl::bitset<64, int64_t> data1, const etl::bitset<64, int64_t>& data2)
    {
      data1 ^= data2;

      return data1;
    }

    TEST(test_xor_equals_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data2(0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64, int64_t> data3(test_xor_equals_operator_helper(data1, data2));

      CHECK((data1 ^ data2) == data3);
    }

    //*************************************************************************
    TEST(test_find_first)
    {
      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs1(ull(0x00));
      ETL_CONSTEXPR14 size_t bs1fff = bs1.find_first(false);
      ETL_CONSTEXPR14 size_t bs1fft = bs1.find_first(true);
      CHECK_EQUAL(0U, bs1fff);
      CHECK_EQUAL(etl::bitset<>::npos, bs1fft);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs2(ull(0xFF));
      ETL_CONSTEXPR14 size_t bs2fff = bs2.find_first(false);
      ETL_CONSTEXPR14 size_t bs2fft = bs2.find_first(true);
      CHECK_EQUAL(etl::bitset<>::npos, bs2fff);
      CHECK_EQUAL(0U, bs2fft);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs3(ull(0x01));
      ETL_CONSTEXPR14 size_t bs3fff = bs3.find_first(false);
      ETL_CONSTEXPR14 size_t bs3fft = bs3.find_first(true);
      CHECK_EQUAL(1U, bs3fff);
      CHECK_EQUAL(0U, bs3fft);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs4(ull(0x20));
      ETL_CONSTEXPR14 size_t bs4fff = bs4.find_first(false);
      ETL_CONSTEXPR14 size_t bs4fft = bs4.find_first(true);
      CHECK_EQUAL(0U, bs4fff);
      CHECK_EQUAL(5U, bs4fft);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs5(ull(0x21));
      ETL_CONSTEXPR14 size_t bs5fff = bs5.find_first(false);
      ETL_CONSTEXPR14 size_t bs5fft = bs5.find_first(true);
      CHECK_EQUAL(1U, bs5fff);
      CHECK_EQUAL(0U, bs5fft);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs6(ull(0x0E));
      ETL_CONSTEXPR14 size_t bs6fff = bs6.find_first(false);
      ETL_CONSTEXPR14 size_t bs6fft = bs6.find_first(true);
      CHECK_EQUAL(0U, bs6fff);
      CHECK_EQUAL(1U, bs6fft);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs7(ull(0x31));
      ETL_CONSTEXPR14 size_t bs7fff = bs7.find_first(false);
      ETL_CONSTEXPR14 size_t bs7fft = bs7.find_first(true);
      CHECK_EQUAL(1U, bs7fff);
      CHECK_EQUAL(0U, bs7fft);
    }

    //*************************************************************************
    TEST(test_find_next)
    {
      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs1(ull(0x00));
      ETL_CONSTEXPR14 size_t bs1fnf0 = bs1.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs1fnf1 = bs1.find_next(false, 1);
      ETL_CONSTEXPR14 size_t bs1fnt2 = bs1.find_next(true, 2);
      CHECK_EQUAL(0U, bs1fnf0);
      CHECK_EQUAL(1U, bs1fnf1);
      CHECK_EQUAL(etl::bitset<>::npos, bs1fnt2);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs2(ull(0xFF));
      ETL_CONSTEXPR14 size_t bs2fnt0 = bs2.find_next(true, 0);
      ETL_CONSTEXPR14 size_t bs2fnt1 = bs2.find_next(true, 1);
      ETL_CONSTEXPR14 size_t bs2fnf2 = bs2.find_next(false, 2);
      CHECK_EQUAL(0U, bs2fnt0);
      CHECK_EQUAL(1U, bs2fnt1);
      CHECK_EQUAL(etl::bitset<>::npos, bs2fnf2);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs3(ull(0x0E));
      ETL_CONSTEXPR14 size_t bs3fnf0 = bs3.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs3fnt1 = bs3.find_next(true, 1);
      ETL_CONSTEXPR14 size_t bs3fnf2 = bs3.find_next(false, 2);
      CHECK_EQUAL(0U, bs3fnf0);
      CHECK_EQUAL(1U, bs3fnt1);
      CHECK_EQUAL(4U, bs3fnf2);

      ETL_CONSTEXPR14 etl::bitset<8, int8_t> bs4(ull(0x31));
      ETL_CONSTEXPR14 size_t bs4fnt0 = bs4.find_next(true, 0);
      ETL_CONSTEXPR14 size_t bs4fnf0 = bs4.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs4fnt1 = bs4.find_next(true, 1);
      CHECK_EQUAL(0U, bs4fnt0);
      CHECK_EQUAL(1U, bs4fnf0);
      CHECK_EQUAL(4U, bs4fnt1);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 std::pair<etl::bitset<8, int8_t>, etl::bitset<8, int8_t>> test_swap_helper()
    {
      etl::bitset<8, int8_t> data1(ull(0x2A));
      etl::bitset<8, int8_t> data2(ull(0x15));

      swap(data1, data2);

      return std::pair<etl::bitset<8, int8_t>, etl::bitset<8, int8_t>>(data1, data2);
    }

    TEST(test_swap)
    {
      ETL_CONSTEXPR14 etl::bitset<8, int8_t> compare1(0x2A);
      ETL_CONSTEXPR14 etl::bitset<8, int8_t> compare2(0x15);

      ETL_CONSTEXPR14 std::pair<etl::bitset<8, int8_t>, etl::bitset<8, int8_t>> swapped = test_swap_helper();

      CHECK(swapped.first == compare2);
      CHECK(swapped.second == compare1);
    }

    //*************************************************************************
    TEST(test_span)
    {
      using span_t = etl::bitset<32, uint32_t>::span_type;

      etl::bitset<32, uint32_t> b(0x12345678UL);

      span_t s = b.span();
      CHECK_EQUAL(0x12345678UL, s[0]);
    }

    //*************************************************************************
    TEST(test_const_span)
    {
      using span_t = etl::bitset<32, int32_t>::const_span_type;

      const etl::bitset<32, int32_t> b(0x12345678UL);

      span_t s = b.span();
      CHECK_EQUAL(0x12345678UL, s[0]);
    }

    //*************************************************************************
    TEST(test_to_string)
    {
      using bs32 = etl::bitset<32, int32_t>;

      ETL_CONSTEXPR14 bs32 b(0x12345678UL);

      etl::string<32> text = b.to_string('.', '*');
      std::string stdtext  = b.to_string<std::string>('.', '*');

      //CHECK_THROW(b.to_string<etl::string<30>>('.', '*'), etl::bitset_overflow);
      CHECK_EQUAL(std::string("...*..*...**.*...*.*.**..****..."), std::string(text.c_str()));
      CHECK_EQUAL(std::string("...*..*...**.*...*.*.**..****..."), std::string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_wstring)
    {
      using bs32 = etl::bitset<32, int32_t>;

      ETL_CONSTEXPR14 bs32 b(0x12345678UL);

      etl::wstring<32> text = b.to_string<etl::wstring<32>>(L'.', L'*');
      std::wstring stdtext = b.to_string<std::wstring>(L'.', L'*');

      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(text.c_str()));
      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u16string)
    {
      using bs32 = etl::bitset<32, int32_t>;

      ETL_CONSTEXPR14 bs32 b(0x12345678UL);

      etl::u16string<32> text = b.to_string<etl::u16string<32>>(u'.', u'*');
      std::u16string stdtext = b.to_string<std::u16string>(u'.', u'*');

      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(text.c_str()));
      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u32string)
    {
      using bs32 = etl::bitset<32, int32_t>;

      ETL_CONSTEXPR14 bs32 b(0x12345678UL);

      etl::u32string<32> text = b.to_string<etl::u32string<32>>(U'.', U'*');
      std::u32string stdtext = b.to_string<std::u32string>(U'.', U'*');

      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(text.c_str()));
      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(stdtext.c_str()));
    }
  };
}
