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

#include "etl/private/bitset_new.h"

namespace
{
  using ull = unsigned long long;

  SUITE(test_bitset_new)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      constexpr std::bitset<60> compare;
      constexpr etl::bitset<60> data;

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
      constexpr std::bitset<60> compare(0x123456731234567ULL);
      constexpr etl::bitset<60> data(0x123456731234567ULL);

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_copy_construct)
    {
      constexpr std::bitset<60> compare(0x123456731234567ULL);
      constexpr etl::bitset<60> data(0x123456731234567ULL);
      constexpr etl::bitset<60> data_copy(data);

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
      constexpr std::bitset<60> compare(0x8765432187654321ULL);
      constexpr etl::bitset<60> data(0x8765432187654321ULL);

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
    constexpr etl::bitset<60> test_set_helper()
    {
      etl::bitset<60> data;
      data.set();

      return data;
    }
     
    TEST(test_set)
    {
      std::bitset<60> compare;
      constexpr etl::bitset<60> data(test_set_helper());

      compare.set();

      constexpr auto size  = data.size();
      constexpr auto count = data.count();
      constexpr auto none  = data.none();
      constexpr auto any   = data.any();
      constexpr auto all   = data.all();

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
      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<60> data;

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
      constexpr etl::bitset<8> data((unsigned long long)etl::integral_limits<uint8_t>::min);
      constexpr uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u8_max)
    {
      constexpr etl::bitset<8> data((unsigned long long)etl::integral_limits<uint8_t>::max);
      constexpr uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_min)
    {
      constexpr etl::bitset<8> data((unsigned long long)etl::integral_limits<int8_t>::min);
      constexpr int8_t value = data.value<int8_t>();

      CHECK_EQUAL(std::numeric_limits<int8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_max)
    {
      constexpr etl::bitset<8> data((unsigned long long)etl::integral_limits<int8_t>::max);
      constexpr int8_t value = data.value<int8_t>();

      CHECK_EQUAL(std::numeric_limits<int8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_min)
    {
      constexpr etl::bitset<16> data((unsigned long long)etl::integral_limits<uint16_t>::min);
      constexpr uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_max)
    {
      constexpr etl::bitset<16> data((unsigned long long)etl::integral_limits<uint16_t>::max);
      constexpr uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_min)
    {
      constexpr etl::bitset<16> data((unsigned long long)etl::integral_limits<int16_t>::min);
      constexpr int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_max)
    {
      etl::bitset<16> data((unsigned long long)etl::integral_limits<int16_t>::max);
      int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_min)
    {
      constexpr etl::bitset<32> data((unsigned long long)etl::integral_limits<uint32_t>::min);
      constexpr uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_max)
    {
      constexpr etl::bitset<32> data((unsigned long long)etl::integral_limits<uint32_t>::max);
      constexpr uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_min)
    {
      constexpr etl::bitset<32> data((unsigned long long)etl::integral_limits<int32_t>::min);
      constexpr int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_max)
    {
      constexpr etl::bitset<32> data((unsigned long long)etl::integral_limits<int32_t>::max);
      constexpr int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_min)
    {
      constexpr etl::bitset<64> data((unsigned long long)etl::integral_limits<uint64_t>::min);
      constexpr uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_max)
    {
      constexpr etl::bitset<64> data((unsigned long long)etl::integral_limits<uint64_t>::max);
      constexpr uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_min)
    {
      constexpr etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::min);
      constexpr int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_max)
    {
      constexpr etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::max);
      constexpr int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_using_element_type_int8_t)
    {
      std::bitset<60> compare(ull(0x0C48D159CC48D159));
      etl::bitset<60, int8_t> data(ull(0x0C48D159CC48D159));

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }

      CHECK_EQUAL_HEX(compare.to_ullong(), data.to_ullong());
    }

    //*************************************************************************
    TEST(test_using_element_type_int16_t)
    {
      std::bitset<60> compare(ull(0x0C48D159CC48D159));
      etl::bitset<60, int16_t> data(ull(0x0C48D159CC48D159));

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }

      CHECK_EQUAL_HEX(compare.to_ullong(), data.to_ullong());
    }

    //*************************************************************************
    TEST(test_using_element_type_int32_t)
    {
      std::bitset<60> compare(ull(0x0C48D159CC48D159));
      etl::bitset<60, int32_t> data(ull(0x0C48D159CC48D159));

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }

      CHECK_EQUAL_HEX(compare.to_ullong(), data.to_ullong());
    }

    //*************************************************************************
    TEST(test_using_element_type_int64_t)
    {
      std::bitset<60> compare(ull(0x0C48D159CC48D159));
      etl::bitset<60, int64_t> data(ull(0x0C48D159CC48D159));

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());
      CHECK_EQUAL(compare.none(), data.none());
      CHECK_EQUAL(compare.any(), data.any());
      CHECK_EQUAL(compare.all(), data.all());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }

      CHECK_EQUAL_HEX(compare.to_ullong(), data.to_ullong());
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
    constexpr etl::bitset<60> test_reset_helper()
    {
      etl::bitset<60> data(0xFFFFFFFFFFFFFFFULL);
      data.reset();

      return data;
    }
    
    TEST(test_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      constexpr etl::bitset<60> data(test_reset_helper());

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
    constexpr etl::bitset<60> test_position_reset_helper()
    {
      etl::bitset<60> data(0xFFFFFFFFFFFFFFFULL);
      data.reset(1);
      data.reset(3);
      data.reset(7);
      data.reset(13);

      return data;
    }

    TEST(test_position_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      constexpr etl::bitset<60> data(test_position_reset_helper());

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

    ////*************************************************************************
    //TEST(test_index_operator_read)
    //{
    //  std::bitset<60> compare(0x3123456731234567ULL);
    //  etl::bitset<60> data(0x3123456731234567ULL);

    //  for (size_t i = 0UL; i < data.size(); ++i)
    //  {
    //    CHECK_EQUAL(compare[i], data[i]);
    //  }
    //}

    ////*************************************************************************
    //TEST(test_index_operator_write)
    //{
    //  std::bitset<60> compare;
    //  etl::bitset<60> data;

    //  for (size_t i = 0UL; i < data.size(); ++i)
    //  {
    //    compare[i] = (i % 2) == 0;
    //    data[i]    = (i % 2) == 0;
    //  }

    //  for (size_t i = 0UL; i < data.size(); ++i)
    //  {
    //    CHECK_EQUAL(compare[i], data[i]);
    //  }
    //}

    ////*************************************************************************
    //TEST(test_any)
    //{
    //  std::bitset<60> compare;
    //  etl::bitset<60> data;

    //  // Set all bits
    //  compare.set();
    //  data.set();

    //  // Reset them all.
    //  for (size_t i = 0UL; i < data.size(); ++i)
    //  {
    //    compare.reset(i);
    //    data.reset(i);
    //  }

    //  CHECK_EQUAL(compare.any(), data.any());

    //  compare.set(3);
    //  data.set(3);

    //  CHECK_EQUAL(compare.any(), data.any());
    //}

    ////*************************************************************************
    //TEST(test_none)
    //{
    //  std::bitset<62> compare;
    //  etl::bitset<62> data;

    //  CHECK_EQUAL(compare.none(), data.none());

    //  compare.set(3);
    //  data.set(3);

    //  CHECK_EQUAL(compare.none(), data.none());
    //}

    ////*************************************************************************
    //TEST(test_all)
    //{
    //  std::bitset<60> compare;
    //  etl::bitset<60> data;

    //  CHECK_EQUAL(compare.all(), data.all());

    //  compare.set();
    //  data.set();

    //  CHECK_EQUAL(compare.all(), data.all());
    //}

    //*************************************************************************
    constexpr etl::bitset<60> test_flip_helper()
    {
      etl::bitset<60> data;
      data.flip();

      return data;
    }

    TEST(test_flip)
    {
      std::bitset<60> compare;
      constexpr etl::bitset<60> data(test_flip_helper());

      compare.flip();

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    constexpr etl::bitset<60> test_flip_position_helper()
    {
      etl::bitset<60> data;
      data.flip(1);
      data.flip(3);
      data.flip(7);
      data.flip(13);

      return data;
    }
    
    TEST(test_flip_position)
    {
      std::bitset<60> compare;
      constexpr etl::bitset<60> data(test_flip_position_helper());

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
    constexpr etl::bitset<60> test_flip_reference_helper()
    {
      etl::bitset<60> data;
      data[1].flip();
      data[3].flip();
      data[7].flip();
      data[13].flip();

      return data;
    }

    TEST(test_flip_reference)
    {
      std::bitset<60> compare;
      constexpr etl::bitset<60> data(test_flip_reference_helper());

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
      std::bitset<60> compare;
      etl::bitset<60> data;

      bool bc = ~compare[3];
      bool bd = ~data[3];
      CHECK_EQUAL(bc, bd);
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
    TEST(test_equality_operator)
    {
      constexpr etl::bitset<60> data1(0x123456781234567ULL);
      constexpr etl::bitset<60> data2(0x123456781234567ULL);
      constexpr etl::bitset<60> data3;

      constexpr bool equal     =  (data1 == data2);
      constexpr bool not_equal = !(data1 == data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      constexpr etl::bitset<60> data1(0x123456781234567ULL);
      constexpr etl::bitset<60> data2(0x123456781234567ULL);
      constexpr etl::bitset<60> data3;

      constexpr bool equal = !(data1 != data2);
      constexpr bool not_equal = (data1 != data3);

      CHECK(!(data1 != data2));
      CHECK(data1 != data3);
    }

    ////*************************************************************************
    //TEST(test_shift_left_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2;
    //  etl::bitset<60> shift1(0x2468ACF0UL);
    //  etl::bitset<60> shift2(0x48D159E0UL);
    //  etl::bitset<60> shift11(0x91A2B3C000ULL);

    //  data2 = data1;
    //  data2 <<= 1U;
    //  CHECK(data2 == shift1);

    //  data2 = data1;
    //  data2 <<= 2U;
    //  CHECK(data2 == shift2);

    //  data2 = data1;
    //  data2 <<= 11U;
    //  CHECK(data2 == shift11);
    //}

    ////*************************************************************************
    //TEST(test_shift_left_operator_shift_element_size)
    //{
    //  etl::bitset<60> data(0x12345678UL);
    //  etl::bitset<60> shift(0x1234567800UL);

    //  data <<= 8U;
    //  CHECK_EQUAL_HEX(shift.value<uint64_t>(), data.value<uint64_t>());
    //}

    ////*************************************************************************
    //TEST(test_shift_left_operator_overflow)
    //{
    //  etl::bitset<31> data(0x7FFFFFFFUL);
    //  etl::bitset<31> shifted(0x7FFFFFFEUL);

    //  data <<= 1U;
    //  CHECK(data == shifted);
    //}

    ////*************************************************************************
    //TEST(test_shift_left_copy_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2;
    //  etl::bitset<60> shift1(0x2468ACF0UL);
    //  etl::bitset<60> shift2(0x48D159E0UL);
    //  etl::bitset<60> shift11(0x91A2B3C000ULL);

    //  data2 = data1 << 1U;
    //  CHECK(data2 == shift1);

    //  data2 = data1 << 2U;
    //  CHECK(data2 == shift2);

    //  data2 = data1 << 11U;
    //  CHECK(data2 == shift11);
    //}

    ////*************************************************************************
    //TEST(test_shift_left_operator_all_shifts_full_size)
    //{
    //  constexpr uint64_t value = 0x0123456789ABCDEFULL;

    //  for (int shift = 0; shift < 64; ++shift)
    //  {
    //    etl::bitset<64> data(value);

    //    CHECK_EQUAL_HEX((value << shift), (data << shift).value<uint64_t>());
    //  }

    //  etl::bitset<64> data(value);
    //  CHECK_EQUAL_HEX(0, (data << 64).value<uint64_t>());
    //}

    ////*************************************************************************
    //TEST(test_shift_left_operator_all_shifts_partial_size)
    //{
    //  constexpr uint64_t value = 0x0123456789ABCDEFULL;
    //  constexpr uint64_t mask = 0x0FFFFFFFFFFFFFFFULL;

    //  for (int shift = 0; shift < 64; ++shift)
    //  {
    //    etl::bitset<60> data(value);

    //    CHECK_EQUAL_HEX(((value << shift) & mask), (data << shift).value<uint64_t>());
    //  }

    //  etl::bitset<64> data(value);
    //  CHECK_EQUAL_HEX(0, (data << 64).value<uint64_t>());
    //}

    ////*************************************************************************
    //TEST(test_shift_right_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2;
    //  etl::bitset<60> shift1(0x91A2B3CUL);
    //  etl::bitset<60> shift2(0x48D159EUL);
    //  etl::bitset<60> shift11(0x2468AUL);

    //  data2 = data1;
    //  data2 >>= 1U;
    //  CHECK(data2 == shift1);

    //  data2 = data1;
    //  data2 >>= 2U;
    //  CHECK(data2 == shift2);

    //  data2 = data1;
    //  data2 >>= 11U;
    //  CHECK(data2 == shift11);
    //}

    ////*************************************************************************
    //TEST(test_shift_right_operator_all_shifts_full_size)
    //{
    //  constexpr uint64_t value = 0x0123456789ABCDEFULL;
    //  
    //  for (int shift = 0; shift < 64; ++shift)
    //  {
    //    etl::bitset<64> data(value);

    //    CHECK_EQUAL_HEX((value >> shift), (data >> shift).value<uint64_t>());
    //  }

    //  etl::bitset<64> data(value);
    //  CHECK_EQUAL_HEX(0, (data >> 64).value<uint64_t>());
    //}

    ////*************************************************************************
    //TEST(test_shift_right_operator_all_shifts_partial_size)
    //{
    //  constexpr uint64_t value = 0x0123456789ABCDEFULL;
    //  constexpr uint64_t mask  = 0x0FFFFFFFFFFFFFFFULL;

    //  for (int shift = 0; shift < 64; ++shift)
    //  {
    //    etl::bitset<60> data(value);

    //    CHECK_EQUAL_HEX(((value >> shift) & mask), (data >> shift).value<uint64_t>());
    //  }

    //  etl::bitset<64> data(value);
    //  CHECK_EQUAL_HEX(0, (data >> 64).value<uint64_t>());
    //}

    ////*************************************************************************
    //TEST(test_shift_right_operator_shift_element_size)
    //{
    //  etl::bitset<60> data(0x12345678UL);
    //  etl::bitset<60> shift(0x123456UL);

    //  data >>= 8U;
    //  CHECK_EQUAL_HEX(shift.value<uint64_t>(), data.value<uint64_t>());
    //}

    ////*************************************************************************
    //TEST(test_shift_right_copy_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2;
    //  etl::bitset<60> shift1(0x91A2B3CUL);
    //  etl::bitset<60> shift2(0x48D159EUL);
    //  etl::bitset<60> shift11(0x2468AUL);

    //  data2 = data1 >> 1U;
    //  CHECK(data2 == shift1);

    //  data2 = data1 >> 2U;
    //  CHECK(data2 == shift2);

    //  data2 = data1 >> 11U;
    //  CHECK(data2 == shift11);
    //}

    ////*************************************************************************
    //TEST(test_and_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2(0x23456789UL);
    //  etl::bitset<60> data3;
    //  etl::bitset<60> data4(0x12345678UL & 0x23456789UL);

    //  data3 = data1 & data2;
    //  CHECK(data3 == data4);
    //}

    ////*************************************************************************
    //TEST(test_and_equals_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2(0x23456789UL);
    //  etl::bitset<60> data3(0x12345678UL & 0x23456789UL);

    //  data2 &= data1;
    //  CHECK(data2 == data3);
    //}

    ////*************************************************************************
    //TEST(test_or_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2(0x23456789UL);
    //  etl::bitset<60> data3;
    //  etl::bitset<60> data4(0x12345678UL | 0x23456789UL);

    //  data3 = data1 | data2;
    //  CHECK(data3 == data4);
    //}

    ////*************************************************************************
    //TEST(test_or_equals_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2(0x23456789UL);
    //  etl::bitset<60> data3(0x12345678UL | 0x23456789UL);

    //  data2 |= data1;
    //  CHECK(data2 == data3);
    //}

    ////*************************************************************************
    //TEST(test_xor_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2(0x23456789UL);
    //  etl::bitset<60> data3;
    //  etl::bitset<60> data4(0x12345678UL ^ 0x23456789UL);

    //  data3 = data1 ^ data2;
    //  CHECK(data3 == data4);
    //}

    ////*************************************************************************
    //TEST(test_xor_equals_operator)
    //{
    //  etl::bitset<60> data1(0x12345678UL);
    //  etl::bitset<60> data2(0x23456789UL);
    //  etl::bitset<60> data3(0x12345678UL ^ 0x23456789UL);

    //  data2 ^= data1;
    //  CHECK(data2 == data3);
    //}

    ////*************************************************************************
    //TEST(test_index_operator)
    //{
    //  etl::bitset<60> data1(0x123456781234567ULL);
    //  etl::bitset<60> data2;

    //  for (size_t i = 0UL; i < data1.size(); ++i)
    //  {
    //    data2[i] = data1[i];
    //  }

    //  CHECK(data1 == data2);
    //}

    ////*************************************************************************
    //TEST(test_big_bitset)
    //{
    //  std::bitset<10000> compare;
    //  etl::bitset<10000> data;

    //  for (size_t i = 0UL; i < data.size(); ++i)
    //  {
    //    compare.set(i, (i % 2) == 0);
    //    data.set(i, (i % 2) == 0);
    //  }

    //  CHECK_EQUAL(compare.test(64), data.test(64));

    //  for (size_t i = 0UL; i < data.size(); ++i)
    //  {
    //    CHECK_EQUAL(compare.test(i), data.test(i));
    //  }

    //  compare.flip();
    //  data.flip();

    //  for (size_t i = 0UL; i < data.size(); ++i)
    //  {
    //    CHECK_EQUAL(compare.test(i), data.test(i));
    //  }
    //}

    ////*************************************************************************
    //TEST(test_small_bitset)
    //{
    //  std::bitset<7> compare;
    //  etl::bitset<7> data;

    //  compare.flip();
    //  data.flip();

    //  for (size_t i = 0UL; i < data.size(); ++i)
    //  {
    //    CHECK_EQUAL(compare.test(i), data.test(i));
    //  }
    //}

    ////*************************************************************************
    //TEST(test_find_first)
    //{
    //  etl::bitset<6> data;

    //  data.set("000000");
    //  CHECK_EQUAL(0U, data.find_first(false));
    //  CHECK_EQUAL(etl::ibitset::npos, data.find_first(true));

    //  data.set("111111");
    //  CHECK_EQUAL(etl::ibitset::npos, data.find_first(false));
    //  CHECK_EQUAL(0U, data.find_first(true));

    //  data.set("000001");
    //  CHECK_EQUAL(1U, data.find_first(false));
    //  CHECK_EQUAL(0U, data.find_first(true));

    //  data.set("100000");
    //  CHECK_EQUAL(0U, data.find_first(false));
    //  CHECK_EQUAL(5U, data.find_first(true));

    //  data.set("100001");
    //  CHECK_EQUAL(1U, data.find_first(false));
    //  CHECK_EQUAL(0U, data.find_first(true));

    //  data.set("001110");
    //  CHECK_EQUAL(0U, data.find_first(false));
    //  CHECK_EQUAL(1U, data.find_first(true));

    //  data.set("110001");
    //  CHECK_EQUAL(1U, data.find_first(false));
    //  CHECK_EQUAL(0U, data.find_first(true));
    //}

    ////*************************************************************************
    //TEST(test_find_next)
    //{
    //  etl::bitset<6> data;

    //  data.set("000000");
    //  CHECK_EQUAL(0U, data.find_next(false, 0));
    //  CHECK_EQUAL(1U, data.find_next(false, 1));
    //  CHECK_EQUAL(etl::ibitset::npos, data.find_next(true, 2));

    //  data.set("111111");
    //  CHECK_EQUAL(0U, data.find_next(true,  0));
    //  CHECK_EQUAL(1U, data.find_next(true,  1));
    //  CHECK_EQUAL(etl::ibitset::npos, data.find_next(false, 2));

    //  data.set("001110");
    //  CHECK_EQUAL(0U, data.find_next(false, 0));
    //  CHECK_EQUAL(1U, data.find_next(true,  0));
    //  CHECK_EQUAL(4U, data.find_next(false, 1));

    //  data.set("110001");
    //  CHECK_EQUAL(0U, data.find_next(true,  0));
    //  CHECK_EQUAL(1U, data.find_next(false, 0));
    //  CHECK_EQUAL(4U, data.find_next(true,  1));
    //}

    ////*************************************************************************
    //TEST(test_find_next_multi_byte_bitset_github_issue_336)
    //{
    //  etl::bitset<24> data;
    //  data.set(12);
    //  data.set(22);
    //  CHECK_EQUAL(12U, data.find_next(true,  3));
    //  CHECK_EQUAL(12U, data.find_next(true, 10));

    //  // set first ten bytes
    //  data.set("1111111111");
    //  CHECK_EQUAL(10U, data.find_next(false, 3));
    //  CHECK_EQUAL(10U, data.find_next(false, 9));
    //}

    ////*************************************************************************
    //TEST(test_find_next_github_issue_336)
    //{
    //  etl::bitset<16> bits16;
    //  bits16.set(12);

    //  etl::bitset<24> bits24;
    //  bits24.set(12);

    //  CHECK_EQUAL(12, bits16.find_first(true));
    //  CHECK_EQUAL(12, bits16.find_next(true, 4));
    //  CHECK_EQUAL(etl::ibitset::npos, bits16.find_next(true, 13));

    //  CHECK_EQUAL(12, bits24.find_first(true));
    //  CHECK_EQUAL(12, bits24.find_next(true, 4));
    //  CHECK_EQUAL(etl::ibitset::npos, bits24.find_next(true, 13));
    //}

    ////*************************************************************************
    //TEST(test_swap)
    //{
    //  etl::bitset<6> compare1("101010");
    //  etl::bitset<6> compare2("010101");

    //  etl::bitset<6> data1(compare1);
    //  etl::bitset<6> data2(compare2);

    //  swap(data1, data2);

    //  CHECK(data1 == compare2);
    //  CHECK(data2 == compare1);
    //}

    ////*************************************************************************
    //TEST(test_span)
    //{
    //  using span_t = etl::ibitset::span_type;

    //  etl::bitset<32> b(0x12345678UL);

    //  span_t s = b.span();
    //  CHECK_EQUAL(0x78U, s[0]);
    //  CHECK_EQUAL(0x56U, s[1]);
    //  CHECK_EQUAL(0x34U, s[2]);
    //  CHECK_EQUAL(0x12U, s[3]);

    //  s[2] = 0x9AU;
    //  uint32_t value = b.value<uint32_t>();
    //  CHECK_EQUAL(0x129A5678UL, value);
    //}

    ////*************************************************************************
    //TEST(test_const_span)
    //{
    //  using span_t = etl::ibitset::const_span_type;

    //  const etl::bitset<32> b(0x12345678UL);

    //  span_t s = b.span();
    //  CHECK_EQUAL(0x78U, s[0]);
    //  CHECK_EQUAL(0x56U, s[1]);
    //  CHECK_EQUAL(0x34U, s[2]);
    //  CHECK_EQUAL(0x12U, s[3]);
    //}

    ////*************************************************************************
    //TEST(test_to_string)
    //{
    //  etl::bitset<32> b(0x12345678UL);

    //  etl::string<32> text = b.to_string('.', '*');
    //  std::string stdtext = b.to_string<std::string>('.', '*');

    //  CHECK_THROW(etl::string<32> text1 = b.to_string<etl::string<30>>('.', '*'), etl::bitset_overflow);   
    //  CHECK_EQUAL("...*..*...**.*...*.*.**..****...", text.c_str());
    //  CHECK_EQUAL("...*..*...**.*...*.*.**..****...", stdtext.c_str());
    //}

    ////*************************************************************************
    //TEST(test_issue_497_count_inverted_bits)
    //{
    //  etl::bitset<5U> bits;
    //  std::bitset<5U> stdbits;
    //  etl::bitset<5U> negbits = ~bits;
    //  std::bitset<5U> stdnegbits = ~stdbits;

    //  CHECK_EQUAL(stdbits.count(),    bits.count());
    //  CHECK_EQUAL(stdbits.all(),      bits.all());
    //  CHECK_EQUAL(stdnegbits.count(), negbits.count());
    //  CHECK_EQUAL(stdnegbits.all(),   negbits.all());
    //}

    ////*************************************************************************
    //TEST(test_issue_497_shifted_zero_sized_bitset)
    //{
    //  etl::bitset<0> bits;
    //  std::bitset<0> stdbits;

    //  CHECK_EQUAL(stdbits.size(), bits.size());
    //  CHECK_EQUAL(stdbits.none(), bits.none());
    //  CHECK_EQUAL(stdbits.all(),  bits.all());
    //  CHECK_EQUAL(stdbits.any(),  bits.any());

    //  CHECK_EQUAL(stdbits.count(),     bits.count());
    //  CHECK_EQUAL(stdbits.to_ulong(),  bits.to_ulong());
    //  CHECK_EQUAL(stdbits.to_ullong(), bits.to_ullong());

    //  etl::bitset<0> shiftbits = bits << 1; // No exception.
    //}
  };
}
