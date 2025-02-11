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
#include "etl/endianness.h"

namespace
{
  using ull = unsigned long long;

  //*************************************************************************
  template <size_t Active_Bits>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits> generate_shift_left_bitset(ull value, size_t shift)
  {
    ull mask = 0ULL;

    if (shift < 64U)
    {
      mask = etl::integral_limits<ull>::max >> shift;
    }

    value &= mask;

    etl::bitset<Active_Bits> data(value);

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
  template <size_t Active_Bits>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits> generate_shift_left_bitset_copy(ull value, size_t shift)
  {
    etl::bitset<Active_Bits> data1(value);
    etl::bitset<Active_Bits> data2;

    if ((shift != 0U) && (shift != 64U))
    {
      data2 = data1 << shift;
    }

    return data2;
  }

  //*************************************************************************
  template <size_t Active_Bits>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits> generate_shift_right_bitset(ull value, size_t shift)
  {
    etl::bitset<Active_Bits> data(value);

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
  template <size_t Active_Bits>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits> generate_shift_right_bitset_copy(ull value, size_t shift)
  {
    etl::bitset<Active_Bits> data1(value);
    etl::bitset<Active_Bits> data2;

    if (shift != 0U)
    {
      data2 = data1 >> shift;
    }

    return data2;
  }

  SUITE(test_bitset_new_default_element_type)
  {
    //*************************************************************************
    TEST(test_constants)
    {
      using bitset_type  = etl::bitset<60>;
      using element_type = bitset_type::element_type;

      constexpr size_t       Size               = 60;
      constexpr size_t       Bits_Per_Element   = etl::integral_limits<element_type>::bits;
      constexpr size_t       Number_Of_Elements = (Size % Bits_Per_Element == 0) ? Size / Bits_Per_Element : Size / Bits_Per_Element + 1;
      constexpr element_type All_Set_Element    = etl::integral_limits<element_type>::max;
      constexpr element_type All_Clear_Element  = etl::integral_limits<element_type>::min;

      //*******************************
      constexpr size_t Size1 = bitset_type::Size;
      constexpr size_t Size2 = bitset_type::size();

      CHECK_EQUAL(Size, Size1);
      CHECK_EQUAL(Size, Size2);

      //*******************************
      constexpr size_t Bits_Per_Element1 = bitset_type::Bits_Per_Element;
      constexpr size_t Bits_Per_Element2 = bitset_type::bits_per_element();

      CHECK_EQUAL(Bits_Per_Element, Bits_Per_Element1);
      CHECK_EQUAL(Bits_Per_Element, Bits_Per_Element2);

      //*******************************
      constexpr size_t Number_Of_Elements1 = bitset_type::Number_Of_Elements;
      constexpr size_t Number_Of_Elements2 = bitset_type::number_of_elements();
      
      CHECK_EQUAL(Number_Of_Elements, Number_Of_Elements1);
      CHECK_EQUAL(Number_Of_Elements, Number_Of_Elements2);

      //*******************************
      constexpr element_type All_Set_Element1 = bitset_type::All_Set_Element;
      constexpr element_type All_Set_Element2 = bitset_type::all_set_element();

      CHECK_EQUAL(All_Set_Element, All_Set_Element1);
      CHECK_EQUAL(All_Set_Element, All_Set_Element2);

      //*******************************
      constexpr size_t All_Clear_Element1 = bitset_type::All_Clear_Element;
      constexpr size_t All_Clear_Element2 = bitset_type::all_clear_element();

      CHECK_EQUAL(All_Clear_Element, All_Clear_Element1);
      CHECK_EQUAL(All_Clear_Element, All_Clear_Element2);

      //*******************************
      constexpr size_t Allocated_Bits = bitset_type::allocated_bits();

      CHECK_EQUAL(Number_Of_Elements * Bits_Per_Element, Allocated_Bits);

      //*******************************
      constexpr char Storage_Model1 = bitset_type::Storage_Model;
      constexpr char Storage_Model2 = bitset_type::storage_model();

      CHECK_EQUAL(etl::bitset_storage_model::Multi, Storage_Model1);
      CHECK_EQUAL(etl::bitset_storage_model::Multi, Storage_Model2);

      etl::bitset_storage_model bsm;
      
      bsm = etl::bitset_storage_model::Undefined;
      CHECK_EQUAL(std::string("Undefined"), std::string(bsm.c_str()));

      bsm = etl::bitset_storage_model::Single;
      CHECK_EQUAL(std::string("Single"), std::string(bsm.c_str()));

      bsm = etl::bitset_storage_model::Multi;
      CHECK_EQUAL(std::string("Multi"), std::string(bsm.c_str()));
    }

    //*************************************************************************
    TEST(test_default_constructor)
    {
      ETL_CONSTEXPR14 std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data;

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
      etl::bitset<8>  data8;
      etl::bitset<16> data16;
      etl::bitset<32> data32;
      etl::bitset<64> data64;

      using element_type  = etl::bitset<8>::element_type;
      size_t element_bits = etl::integral_limits<element_type>::bits;

      CHECK_EQUAL(8,  data8.Size);
      CHECK_EQUAL(16, data16.Size);
      CHECK_EQUAL(32, data32.Size);
      CHECK_EQUAL(64, data64.Size);

      CHECK_EQUAL( 8 / element_bits, data8.Number_Of_Elements);
      CHECK_EQUAL(16 / element_bits, data16.Number_Of_Elements);
      CHECK_EQUAL(32 / element_bits, data32.Number_Of_Elements);
      CHECK_EQUAL(64 / element_bits, data64.Number_Of_Elements);

      CHECK_EQUAL(etl::bitset_storage_model::Single, data8.Storage_Model);
      CHECK_EQUAL(etl::bitset_storage_model::Multi,  data16.Storage_Model);
      CHECK_EQUAL(etl::bitset_storage_model::Multi,  data32.Storage_Model);
      CHECK_EQUAL(etl::bitset_storage_model::Multi,  data64.Storage_Model);
    }

    //*************************************************************************
    TEST(test_construct_from_value)
    {
      ETL_CONSTEXPR14 std::bitset<64> compare(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data(0x123456731234567ULL);

      CHECK_EQUAL(compare.size(), data.size());
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
      ETL_CONSTEXPR14 etl::bitset<64> data(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data_copy(data);

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
      ETL_CONSTEXPR14 etl::bitset<64> data(0x8765432187654321ULL);

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      ETL_CONSTEXPR14 std::bitset<64> compare(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data(0x123456731234567ULL);
      etl::bitset<64> data_copy;

      data_copy =data;

      CHECK_EQUAL(compare.size(), data_copy.size());
      CHECK_EQUAL(compare.count(), data_copy.count());

      for (size_t i = 0UL; i < data_copy.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data_copy.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_char_string)
    {
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data("110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
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
      etl::bitset<64> data(L"110001001000110100010101100111001100010010001101000101011001");

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
      etl::bitset<64> data(u"110001001000110100010101100111001100010010001101000101011001");

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
      etl::bitset<64> data(U"110001001000110100010101100111001100010010001101000101011001");

      CHECK_EQUAL(compare.size(), data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_construct_from_nullptr_char_string)
    {
      const char* s = nullptr;

      etl::bitset<60> data(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_construct_from_nullptr_wchar_t_string)
    {
      const wchar_t* s = nullptr;

      etl::bitset<60> data(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_construct_from_nullptr_char16_t_string)
    {
      const char16_t* s = nullptr;

      etl::bitset<60> data(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_construct_from_nullptr_char32_t_string)
    {
      const char32_t* s = nullptr;

      etl::bitset<60> data(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_construct_from_excess_string)
    {
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001111100001");
      etl::bitset<64> data("110001001000110100010101100111001100010010001101000101011001111100001");

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
    ETL_CONSTEXPR14 etl::bitset<64> test_set_helper()
    {
      etl::bitset<64> data;
      data.set();

      return data;
    }

    TEST(test_set)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_set_helper());

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
    TEST(test_set_less_than_full_size)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

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
    ETL_CONSTEXPR14 etl::bitset<64> test_set_run_time_position_default_value_helper()
    {
      etl::bitset<64> data;

      data.set(2);
      data.set(3);
      data.set(5);
      data.set(7);
      data.set(11);
      data.set(13);
      data.set(17);
      data.set(19);
      data.set(23);
      data.set(29);
      data.set(31);

      return data;
    }

    TEST(test_set_run_time_position_default_value)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_set_run_time_position_default_value_helper());

      compare.set(2);
      compare.set(3);
      compare.set(5);
      compare.set(7);
      compare.set(11);
      compare.set(13);
      compare.set(17);
      compare.set(19);
      compare.set(23);
      compare.set(29);
      compare.set(31);

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
    ETL_CONSTEXPR14 etl::bitset<64> test_set_run_time_position_value_helper()
    {
      etl::bitset<64> data;

      data.set(2, true);
      data.set(3, true);
      data.set(5, true);
      data.set(7, true);
      data.set(11, true);
      data.set(13, true);
      data.set(17, true);
      data.set(19, true);
      data.set(23, true);
      data.set(29, true);
      data.set(31, true);

      return data;
    }

    TEST(test_set_run_time_position_value)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_set_run_time_position_value_helper());

      compare.set(2);
      compare.set(3);
      compare.set(5);
      compare.set(7);
      compare.set(11);
      compare.set(13);
      compare.set(17);
      compare.set(19);
      compare.set(23);
      compare.set(29);
      compare.set(31);

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
    ETL_CONSTEXPR14 etl::bitset<64> test_set_compile_time_position_run_time_default_value_helper()
    {
      etl::bitset<64> data;

      data.set<2>();
      data.set<3>();
      data.set<5>();
      data.set<7>();
      data.set<11>();
      data.set<13>();
      data.set<17>();
      data.set<19>();
      data.set<23>();
      data.set<29>();
      data.set<31>();

      return data;
    }

    TEST(test_set_compile_time_position_run_time_default_value)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_set_compile_time_position_run_time_default_value_helper());

      compare.set(2);
      compare.set(3);
      compare.set(5);
      compare.set(7);
      compare.set(11);
      compare.set(13);
      compare.set(17);
      compare.set(19);
      compare.set(23);
      compare.set(29);
      compare.set(31);

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
    ETL_CONSTEXPR14 etl::bitset<64> test_set_compile_time_position_run_time_value_helper()
    {
      etl::bitset<64> data;

      data.set<2>(true);
      data.set<3>(true);
      data.set<5>(true);
      data.set<7>(true);
      data.set<11>(true);
      data.set<13>(true);
      data.set<17>(true);
      data.set<19>(true);
      data.set<23>(true);
      data.set<29>(true);
      data.set<31>(true);

      return data;
    }

    TEST(test_set_compile_time_position_run_time_value)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_set_compile_time_position_run_time_value_helper());

      compare.set(2);
      compare.set(3);
      compare.set(5);
      compare.set(7);
      compare.set(11);
      compare.set(13);
      compare.set(17);
      compare.set(19);
      compare.set(23);
      compare.set(29);
      compare.set(31);

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
    ETL_CONSTEXPR14 etl::bitset<64> test_set_compile_time_position_value_helper()
    {
      etl::bitset<64> data;

      data.set<2, true>();
      data.set<3, true>();
      data.set<5, true>();
      data.set<7, true>();
      data.set<11, true>();
      data.set<13, true>();
      data.set<17, true>();
      data.set<19, true>();
      data.set<23, true>();
      data.set<29, true>();
      data.set<31, true>();

      return data;
    }

    TEST(test_set_compile_time_position_value)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_set_compile_time_position_value_helper());

      compare.set(2);
      compare.set(3);
      compare.set(5);
      compare.set(7);
      compare.set(11);
      compare.set(13);
      compare.set(17);
      compare.set(19);
      compare.set(23);
      compare.set(29);
      compare.set(31);

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
    TEST(test_set_with_char_string)
    {
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data;

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
    TEST(test_set_with_wchar_t_string)
    {
      std::bitset<64> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data;

      data.set(L"110001001000110100010101100111001100010010001101000101011001");

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
    TEST(test_set_with_char16_t_string)
    {
      std::bitset<64> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data;

      data.set(u"110001001000110100010101100111001100010010001101000101011001");

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
    TEST(test_set_with_char32_t_string)
    {
      std::bitset<64> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data;

      data.set(U"110001001000110100010101100111001100010010001101000101011001");

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
    TEST(test_set_from_char_nullptr)
    {
      const char* s = nullptr;

      etl::bitset<60> data;
      data.set(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_set_from_wchar_t_nullptr)
    {
      const wchar_t* s = nullptr;

      etl::bitset<60> data;
      data.set(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_set_from_char16_t_nullptr)
    {
      const char16_t* s = nullptr;

      etl::bitset<60> data;
      data.set(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_set_from_char32_t_nullptr)
    {
      const char32_t* s = nullptr;

      etl::bitset<60> data;
      data.set(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_from_string_with_char)
    {
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data;

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
    TEST(test_from_string_with_wchar_t)
    {
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data;

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
    TEST(test_from_string_with_char16_t)
    {
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data;

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
    TEST(test_from_string_with_char32_t)
    {
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset<64> data;

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
    TEST(test_from_string_with_char_nullptr)
    {
      const char* s = nullptr;

      etl::bitset<60> data;
      data.from_string(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_from_string_with_wchar_t_nullptr)
    {
      const wchar_t* s = nullptr;

      etl::bitset<60> data;
      data.from_string(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_from_string_with_char16_t_nullptr)
    {
      const char16_t* s = nullptr;

      etl::bitset<60> data;
      data.from_string(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_from_string_with_char32_t_nullptr)
    {
      const char32_t* s = nullptr;

      etl::bitset<60> data;
      data.from_string(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
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
      ETL_CONSTEXPR14 uint64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_max)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::max);
      ETL_CONSTEXPR14 uint64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_min)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::min);
      ETL_CONSTEXPR14 uint64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_max)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<int64_t>::max);
      ETL_CONSTEXPR14 uint64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_to_ulong_min)
    {
      ETL_CONSTEXPR14 etl::bitset<etl::integral_limits<unsigned long>::bits> data(etl::integral_limits<unsigned long>::min);
      unsigned long value = data.to_ulong();

      CHECK_EQUAL(std::numeric_limits<unsigned long>::min(), value);
    }

    //*************************************************************************
    TEST(test_to_ulong_max)
    {
      ETL_CONSTEXPR14 etl::bitset<etl::integral_limits<unsigned long>::bits> data(etl::integral_limits<unsigned long>::max);
      unsigned long value = data.to_ulong();

      CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), value);
    }

    //*************************************************************************
    TEST(test_to_ulong_overflow)
    {
      ETL_CONSTEXPR14 etl::bitset<etl::integral_limits<unsigned long>::bits + 1> data(etl::integral_limits<unsigned long>::min);
      CHECK_THROW(data.to_ulong(), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_to_ullong_min)
    {
      ETL_CONSTEXPR14 etl::bitset<etl::integral_limits<unsigned long long>::bits> data(etl::integral_limits<unsigned long long>::min);
      unsigned long long value = data.to_ullong();

      CHECK_EQUAL(std::numeric_limits<unsigned long>::min(), value);
    }

    //*************************************************************************
    TEST(test_to_ullong_max)
    {
      ETL_CONSTEXPR14 etl::bitset<etl::integral_limits<unsigned long long>::bits> data(etl::integral_limits<unsigned long long>::max);
      unsigned long long value = data.to_ullong();

      CHECK_EQUAL(std::numeric_limits<unsigned long long>::max(), value);
    }

    //*************************************************************************
    TEST(test_to_ullong_overflow)
    {
      ETL_CONSTEXPR14 etl::bitset<etl::integral_limits<unsigned long long>::bits + 1> data(etl::integral_limits<unsigned long long>::min);
      CHECK_THROW(data.to_ullong(), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_position_set)
    {
      std::bitset<64> compare;
      etl::bitset<64> data;

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
      etl::bitset<12> data;

      CHECK_THROW(data.set(13), etl::bitset_overflow);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64> test_reset_helper()
    {
      etl::bitset<64> data(0xFFFFFFFFFFFFFFFULL);
      data.reset();

      return data;
    }

    TEST(test_reset)
    {
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      ETL_CONSTEXPR14 etl::bitset<64> data(test_reset_helper());

      compare.reset();

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
    ETL_CONSTEXPR14 etl::bitset<64> test_position_reset_helper()
    {
      etl::bitset<64> data(0xFFFFFFFFFFFFFFFULL);
      data.reset(1);
      data.reset(3);
      data.reset(7);
      data.reset(13);

      return data;
    }

    TEST(test_position_reset)
    {
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      ETL_CONSTEXPR14 etl::bitset<64> data(test_position_reset_helper());

      compare.reset(1);
      compare.reset(3);
      compare.reset(7);
      compare.reset(13);

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
      ETL_CONSTEXPR14 std::bitset<64> compare(0x3123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data(0x3123456731234567ULL);

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
    ETL_CONSTEXPR14 etl::bitset<64> test_index_operator_write_helper()
    {
      etl::bitset<64> data;

      data[1U] = true;
      data[3U] = true;
      data[7U] = true;
      data[13U] = true;

      return data;
    }

    TEST(test_index_operator_write)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data(test_index_operator_write_helper());

      CHECK(data[1U]);
      CHECK(data[3U]);
      CHECK(data[7U]);
      CHECK(data[13U]);
    }

    //*************************************************************************
    TEST(test_any)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64> data4(ull(0xFFFFFFFFFFFFFFFF));

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
    TEST(test_none)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64> data4(ull(0xFFFFFFFFFFFFFFFF));

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
    TEST(test_all)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<64> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<64> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<64> data4(ull(0xFFFFFFFFFFFFFFFF));

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
    TEST(test_any_with_reduced_active_bits)
    {
      ETL_CONSTEXPR14 etl::bitset<60> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<60> data4(ull(0xFFFFFFFFFFFFFFFF));

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
    TEST(test_none_with_reduced_active_bits)
    {
      ETL_CONSTEXPR14 etl::bitset<60> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<60> data4(ull(0xFFFFFFFFFFFFFFFF));

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
    TEST(test_all_with_reduced_active_bits)
    {
      ETL_CONSTEXPR14 etl::bitset<60> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data3(ull(0x1000010001000100));
      ETL_CONSTEXPR14 etl::bitset<60> data4(ull(0xFFFFFFFFFFFFFFFF));

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
    ETL_CONSTEXPR14 etl::bitset<64> test_flip_helper()
    {
      etl::bitset<64> data;
      data.flip();

      return data;
    }

    TEST(test_flip)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_flip_helper());

      compare.flip();

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64> test_flip_position_helper()
    {
      etl::bitset<64> data;
      data.flip(1);
      data.flip(3);
      data.flip(7);
      data.flip(13);

      return data;
    }

    TEST(test_flip_position)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_flip_position_helper());

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
    ETL_CONSTEXPR14 etl::bitset<64> test_flip_reference_helper()
    {
      etl::bitset<64> data;
      data[1].flip();
      data[3].flip();
      data[7].flip();
      data[13].flip();

      return data;
    }

    TEST(test_flip_reference)
    {
      std::bitset<64> compare;
      ETL_CONSTEXPR14 etl::bitset<64> data(test_flip_reference_helper());

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
      etl::bitset<64> data;

      bool bc = ~compare[3];
      bool bd = ~data[3];
      CHECK_EQUAL(bc, bd);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64> test_assignment_operator_helper(etl::bitset<64> to, const etl::bitset<64>&from)
    {
      to = from;

      return to;
    }

    TEST(test_assignment_operator)
    {
      ETL_CONSTEXPR14 std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      ETL_CONSTEXPR14 etl::bitset<64> data1(0xFFFFFFFFFFFFFFFULL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(test_assignment_operator_helper(etl::bitset<64>(), data1));

      for (size_t i = 0UL; i < data2.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data3;

      ETL_CONSTEXPR14 bool equal = (data1 == data2);
      ETL_CONSTEXPR14 bool not_equal = !(data1 == data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<64> data3;

      ETL_CONSTEXPR14 bool equal = !(data1 != data2);
      ETL_CONSTEXPR14 bool not_equal = (data1 != data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_shift_left_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> shift1(0x2468ACF0ULL);
      ETL_CONSTEXPR14 etl::bitset<64> shift2(0x48D159E0ULL);
      ETL_CONSTEXPR14 etl::bitset<64> shift11(0x91A2B3C000ULL);

      CHECK((generate_shift_left_bitset<64>(0x12345678ULL, 1U)) == shift1);
      CHECK((generate_shift_left_bitset<64>(0x12345678ULL, 2U)) == shift2);
      CHECK((generate_shift_left_bitset<64>(0x12345678ULL, 11U)) == shift11);
    }

    //*************************************************************************
    TEST(test_shift_left_operator_overflow)
    {
      ETL_CONSTEXPR14 etl::bitset<32> data(generate_shift_left_bitset<32>(0x7FFFFFFFULL, 1U));
      ETL_CONSTEXPR14 etl::bitset<32> shifted(0xFFFFFFFEUL);

      CHECK_EQUAL_HEX(shifted.value<uint32_t>(), data.value<uint32_t>());
    }

    //*************************************************************************
    TEST(test_shift_left_copy_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> shift8(0x1234567800ULL);
      CHECK_EQUAL_HEX(shift8.value<int64_t>(), (generate_shift_left_bitset_copy<64>(0x12345678UL, 8U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64> shift16(0x123456780000ULL);
      CHECK_EQUAL_HEX(shift16.value<int64_t>(), (generate_shift_left_bitset_copy<64>(0x12345678UL, 16U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64> shift24(0x12345678000000ULL);
      CHECK_EQUAL_HEX(shift24.value<int64_t>(), (generate_shift_left_bitset_copy<64>(0x12345678UL, 24U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64> shift64(0x0000000000000000ULL);
      CHECK_EQUAL_HEX(shift64.value<int64_t>(), (generate_shift_left_bitset_copy<64>(0x12345678UL, 64U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_left_operator_all_shifts_full_size)
    {
      uint64_t value = 0x0123456789ABCDEFULL;

      CHECK_EQUAL_HEX((value << 0U), (generate_shift_left_bitset<64>(value, 0U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 1U), (generate_shift_left_bitset<64>(value, 1U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 2U), (generate_shift_left_bitset<64>(value, 2U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 3U), (generate_shift_left_bitset<64>(value, 3U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 4U), (generate_shift_left_bitset<64>(value, 4U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 5U), (generate_shift_left_bitset<64>(value, 5U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 6U), (generate_shift_left_bitset<64>(value, 6U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 7U), (generate_shift_left_bitset<64>(value, 7U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 8U), (generate_shift_left_bitset<64>(value, 8U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 9U), (generate_shift_left_bitset<64>(value, 9U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 10U), (generate_shift_left_bitset<64>(value, 10U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 11U), (generate_shift_left_bitset<64>(value, 11U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 12U), (generate_shift_left_bitset<64>(value, 12U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 13U), (generate_shift_left_bitset<64>(value, 13U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 14U), (generate_shift_left_bitset<64>(value, 14U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 15U), (generate_shift_left_bitset<64>(value, 15U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 16U), (generate_shift_left_bitset<64>(value, 16U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 17U), (generate_shift_left_bitset<64>(value, 17U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 18U), (generate_shift_left_bitset<64>(value, 18U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 19U), (generate_shift_left_bitset<64>(value, 19U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 20U), (generate_shift_left_bitset<64>(value, 20U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 21U), (generate_shift_left_bitset<64>(value, 21U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 22U), (generate_shift_left_bitset<64>(value, 22U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 23U), (generate_shift_left_bitset<64>(value, 23U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 24U), (generate_shift_left_bitset<64>(value, 24U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 25U), (generate_shift_left_bitset<64>(value, 25U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 26U), (generate_shift_left_bitset<64>(value, 26U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 27U), (generate_shift_left_bitset<64>(value, 27U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 28U), (generate_shift_left_bitset<64>(value, 28U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 29U), (generate_shift_left_bitset<64>(value, 29U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 30U), (generate_shift_left_bitset<64>(value, 30U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 31U), (generate_shift_left_bitset<64>(value, 31U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 32U), (generate_shift_left_bitset<64>(value, 32U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 33U), (generate_shift_left_bitset<64>(value, 33U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 34U), (generate_shift_left_bitset<64>(value, 34U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 35U), (generate_shift_left_bitset<64>(value, 35U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 36U), (generate_shift_left_bitset<64>(value, 36U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 37U), (generate_shift_left_bitset<64>(value, 37U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 38U), (generate_shift_left_bitset<64>(value, 38U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 39U), (generate_shift_left_bitset<64>(value, 39U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 40U), (generate_shift_left_bitset<64>(value, 40U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 41U), (generate_shift_left_bitset<64>(value, 41U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 42U), (generate_shift_left_bitset<64>(value, 42U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 43U), (generate_shift_left_bitset<64>(value, 43U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 44U), (generate_shift_left_bitset<64>(value, 44U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 45U), (generate_shift_left_bitset<64>(value, 45U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 46U), (generate_shift_left_bitset<64>(value, 46U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 47U), (generate_shift_left_bitset<64>(value, 47U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 48U), (generate_shift_left_bitset<64>(value, 48U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 49U), (generate_shift_left_bitset<64>(value, 49U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 50U), (generate_shift_left_bitset<64>(value, 50U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 51U), (generate_shift_left_bitset<64>(value, 51U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 52U), (generate_shift_left_bitset<64>(value, 52U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 53U), (generate_shift_left_bitset<64>(value, 53U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 54U), (generate_shift_left_bitset<64>(value, 54U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 55U), (generate_shift_left_bitset<64>(value, 55U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 56U), (generate_shift_left_bitset<64>(value, 56U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 57U), (generate_shift_left_bitset<64>(value, 57U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 58U), (generate_shift_left_bitset<64>(value, 58U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 59U), (generate_shift_left_bitset<64>(value, 59U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 60U), (generate_shift_left_bitset<64>(value, 60U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 61U), (generate_shift_left_bitset<64>(value, 61U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 62U), (generate_shift_left_bitset<64>(value, 62U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 63U), (generate_shift_left_bitset<64>(value, 63U).value<uint64_t>()));
      CHECK_EQUAL_HEX(0ULL, (generate_shift_left_bitset<64>(value, 64U).value<uint64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> shift1(0x91A2B3CULL);
      ETL_CONSTEXPR14 etl::bitset<64> shift2(0x48D159EULL);
      ETL_CONSTEXPR14 etl::bitset<64> shift11(0x2468AULL);

      CHECK_EQUAL_HEX(shift1.value<int64_t>(), (generate_shift_right_bitset<64>(0x12345678ULL, 1U).value<int64_t>()));
      CHECK_EQUAL_HEX(shift2.value<int64_t>(), (generate_shift_right_bitset<64>(0x12345678ULL, 2U).value<int64_t>()));
      CHECK_EQUAL_HEX(shift11.value<int64_t>(), (generate_shift_right_bitset<64>(0x12345678ULL, 11U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_copy_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> shift8(0x123456ULL);
      CHECK_EQUAL_HEX(shift8.value<int64_t>(), (generate_shift_right_bitset_copy<64>(0x12345678UL, 8U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64> shift16(0x1234ULL);
      CHECK_EQUAL_HEX(shift16.value<int64_t>(), (generate_shift_right_bitset_copy<64>(0x12345678UL, 16U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64> shift24(0x12ULL);
      CHECK_EQUAL_HEX(shift24.value<int64_t>(), (generate_shift_right_bitset_copy<64>(0x12345678UL, 24U).value<int64_t>()));

      ETL_CONSTEXPR14 etl::bitset<64> shift60(0x00ULL);
      CHECK_EQUAL_HEX(shift60.value<int64_t>(), (generate_shift_right_bitset_copy<64>(0x12345678UL, 60U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_full_size)
    {
      uint64_t value = 0x0123456789ABCDEFULL;

      CHECK_EQUAL_HEX((value >> 0U), (generate_shift_right_bitset<64>(value, 0U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 1U), (generate_shift_right_bitset<64>(value, 1U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 2U), (generate_shift_right_bitset<64>(value, 2U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 3U), (generate_shift_right_bitset<64>(value, 3U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 4U), (generate_shift_right_bitset<64>(value, 4U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 5U), (generate_shift_right_bitset<64>(value, 5U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 6U), (generate_shift_right_bitset<64>(value, 6U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 7U), (generate_shift_right_bitset<64>(value, 7U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 8U), (generate_shift_right_bitset<64>(value, 8U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 9U), (generate_shift_right_bitset<64>(value, 9U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 10U), (generate_shift_right_bitset<64>(value, 10U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 11U), (generate_shift_right_bitset<64>(value, 11U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 12U), (generate_shift_right_bitset<64>(value, 12U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 13U), (generate_shift_right_bitset<64>(value, 13U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 14U), (generate_shift_right_bitset<64>(value, 14U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 15U), (generate_shift_right_bitset<64>(value, 15U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 16U), (generate_shift_right_bitset<64>(value, 16U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 17U), (generate_shift_right_bitset<64>(value, 17U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 18U), (generate_shift_right_bitset<64>(value, 18U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 19U), (generate_shift_right_bitset<64>(value, 19U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 20U), (generate_shift_right_bitset<64>(value, 20U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 21U), (generate_shift_right_bitset<64>(value, 21U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 22U), (generate_shift_right_bitset<64>(value, 22U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 23U), (generate_shift_right_bitset<64>(value, 23U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 24U), (generate_shift_right_bitset<64>(value, 24U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 25U), (generate_shift_right_bitset<64>(value, 25U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 26U), (generate_shift_right_bitset<64>(value, 26U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 27U), (generate_shift_right_bitset<64>(value, 27U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 28U), (generate_shift_right_bitset<64>(value, 28U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 29U), (generate_shift_right_bitset<64>(value, 29U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 30U), (generate_shift_right_bitset<64>(value, 30U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 31U), (generate_shift_right_bitset<64>(value, 31U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 32U), (generate_shift_right_bitset<64>(value, 32U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 33U), (generate_shift_right_bitset<64>(value, 33U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 34U), (generate_shift_right_bitset<64>(value, 34U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 35U), (generate_shift_right_bitset<64>(value, 35U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 36U), (generate_shift_right_bitset<64>(value, 36U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 37U), (generate_shift_right_bitset<64>(value, 37U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 38U), (generate_shift_right_bitset<64>(value, 38U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 39U), (generate_shift_right_bitset<64>(value, 39U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 40U), (generate_shift_right_bitset<64>(value, 40U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 41U), (generate_shift_right_bitset<64>(value, 41U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 42U), (generate_shift_right_bitset<64>(value, 42U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 43U), (generate_shift_right_bitset<64>(value, 43U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 44U), (generate_shift_right_bitset<64>(value, 44U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 45U), (generate_shift_right_bitset<64>(value, 45U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 46U), (generate_shift_right_bitset<64>(value, 46U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 47U), (generate_shift_right_bitset<64>(value, 47U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 48U), (generate_shift_right_bitset<64>(value, 48U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 49U), (generate_shift_right_bitset<64>(value, 49U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 50U), (generate_shift_right_bitset<64>(value, 50U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 51U), (generate_shift_right_bitset<64>(value, 51U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 52U), (generate_shift_right_bitset<64>(value, 52U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 53U), (generate_shift_right_bitset<64>(value, 53U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 54U), (generate_shift_right_bitset<64>(value, 54U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 55U), (generate_shift_right_bitset<64>(value, 55U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 56U), (generate_shift_right_bitset<64>(value, 56U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 57U), (generate_shift_right_bitset<64>(value, 57U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 58U), (generate_shift_right_bitset<64>(value, 58U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 59U), (generate_shift_right_bitset<64>(value, 59U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 60U), (generate_shift_right_bitset<64>(value, 60U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 61U), (generate_shift_right_bitset<64>(value, 61U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 62U), (generate_shift_right_bitset<64>(value, 62U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 63U), (generate_shift_right_bitset<64>(value, 63U).value<int64_t>()));
      CHECK_EQUAL_HEX(0ULL, (generate_shift_right_bitset<64>(value, 64U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_partial_size)
    {
      uint64_t value = 0x0123456789ABCDEFULL;
      uint64_t mask = 0x0FFFFFFFFFFFFFFFULL;

      CHECK_EQUAL_HEX((value >> 0U) & mask, (generate_shift_right_bitset<64>(value, 0U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 1U) & mask, (generate_shift_right_bitset<64>(value, 1U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 2U) & mask, (generate_shift_right_bitset<64>(value, 2U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 3U) & mask, (generate_shift_right_bitset<64>(value, 3U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 4U) & mask, (generate_shift_right_bitset<64>(value, 4U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 5U) & mask, (generate_shift_right_bitset<64>(value, 5U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 6U) & mask, (generate_shift_right_bitset<64>(value, 6U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 7U) & mask, (generate_shift_right_bitset<64>(value, 7U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 8U) & mask, (generate_shift_right_bitset<64>(value, 8U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 9U) & mask, (generate_shift_right_bitset<64>(value, 9U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 10U) & mask, (generate_shift_right_bitset<64>(value, 10U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 11U) & mask, (generate_shift_right_bitset<64>(value, 11U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 12U) & mask, (generate_shift_right_bitset<64>(value, 12U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 13U) & mask, (generate_shift_right_bitset<64>(value, 13U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 14U) & mask, (generate_shift_right_bitset<64>(value, 14U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 15U) & mask, (generate_shift_right_bitset<64>(value, 15U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 16U) & mask, (generate_shift_right_bitset<64>(value, 16U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 17U) & mask, (generate_shift_right_bitset<64>(value, 17U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 18U) & mask, (generate_shift_right_bitset<64>(value, 18U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 19U) & mask, (generate_shift_right_bitset<64>(value, 19U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 20U) & mask, (generate_shift_right_bitset<64>(value, 20U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 21U) & mask, (generate_shift_right_bitset<64>(value, 21U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 22U) & mask, (generate_shift_right_bitset<64>(value, 22U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 23U) & mask, (generate_shift_right_bitset<64>(value, 23U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 24U) & mask, (generate_shift_right_bitset<64>(value, 24U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 25U) & mask, (generate_shift_right_bitset<64>(value, 25U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 26U) & mask, (generate_shift_right_bitset<64>(value, 26U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 27U) & mask, (generate_shift_right_bitset<64>(value, 27U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 28U) & mask, (generate_shift_right_bitset<64>(value, 28U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 29U) & mask, (generate_shift_right_bitset<64>(value, 29U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 30U) & mask, (generate_shift_right_bitset<64>(value, 30U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 31U) & mask, (generate_shift_right_bitset<64>(value, 31U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 32U) & mask, (generate_shift_right_bitset<64>(value, 32U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 33U) & mask, (generate_shift_right_bitset<64>(value, 33U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 34U) & mask, (generate_shift_right_bitset<64>(value, 34U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 35U) & mask, (generate_shift_right_bitset<64>(value, 35U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 36U) & mask, (generate_shift_right_bitset<64>(value, 36U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 37U) & mask, (generate_shift_right_bitset<64>(value, 37U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 38U) & mask, (generate_shift_right_bitset<64>(value, 38U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 39U) & mask, (generate_shift_right_bitset<64>(value, 39U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 40U) & mask, (generate_shift_right_bitset<64>(value, 40U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 41U) & mask, (generate_shift_right_bitset<64>(value, 41U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 42U) & mask, (generate_shift_right_bitset<64>(value, 42U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 43U) & mask, (generate_shift_right_bitset<64>(value, 43U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 44U) & mask, (generate_shift_right_bitset<64>(value, 44U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 45U) & mask, (generate_shift_right_bitset<64>(value, 45U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 46U) & mask, (generate_shift_right_bitset<64>(value, 46U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 47U) & mask, (generate_shift_right_bitset<64>(value, 47U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 48U) & mask, (generate_shift_right_bitset<64>(value, 48U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 49U) & mask, (generate_shift_right_bitset<64>(value, 49U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 50U) & mask, (generate_shift_right_bitset<64>(value, 50U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 51U) & mask, (generate_shift_right_bitset<64>(value, 51U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 52U) & mask, (generate_shift_right_bitset<64>(value, 52U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 53U) & mask, (generate_shift_right_bitset<64>(value, 53U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 54U) & mask, (generate_shift_right_bitset<64>(value, 54U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 55U) & mask, (generate_shift_right_bitset<64>(value, 55U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 56U) & mask, (generate_shift_right_bitset<64>(value, 56U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 57U) & mask, (generate_shift_right_bitset<64>(value, 57U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 58U) & mask, (generate_shift_right_bitset<64>(value, 58U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 59U) & mask, (generate_shift_right_bitset<64>(value, 59U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 60U) & mask, (generate_shift_right_bitset<64>(value, 60U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 61U) & mask, (generate_shift_right_bitset<64>(value, 61U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 62U) & mask, (generate_shift_right_bitset<64>(value, 62U).value<int64_t>()));
      CHECK_EQUAL_HEX((value >> 63U) & mask, (generate_shift_right_bitset<64>(value, 63U).value<int64_t>()));
      CHECK_EQUAL_HEX(0ULL, (generate_shift_right_bitset<64>(value, 64U).value<int64_t>()));
    }

    //*************************************************************************
    TEST(test_and_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x23456789UL);
      ETL_CONSTEXPR14 etl::bitset<64> data4(0x12345678UL & 0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64> data3 = data1 & data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64> test_and_equals_operator_helper(etl::bitset<64> data1, const etl::bitset<64>&data2)
    {
      data1 &= data2;

      return data1;
    }

    TEST(test_and_equals_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64> data3(test_and_equals_operator_helper(data1, data2));

      CHECK((data1 & data2) == data3);
    }

    //*************************************************************************
    TEST(test_or_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x23456789UL);
      ETL_CONSTEXPR14 etl::bitset<64> data4(0x12345678UL | 0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64> data3 = data1 | data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64> test_or_equals_operator_helper(etl::bitset<64> data1, const etl::bitset<64>&data2)
    {
      data1 |= data2;

      return data1;
    }

    TEST(test_or_equals_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64> data3(test_or_equals_operator_helper(data1, data2));

      CHECK((data1 | data2) == data3);
    }

    //*************************************************************************
    TEST(test_xor_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x23456789UL);
      ETL_CONSTEXPR14 etl::bitset<64> data4(0x12345678UL ^ 0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64> data3 = data1 ^ data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 etl::bitset<64> test_xor_equals_operator_helper(etl::bitset<64> data1, const etl::bitset<64>&data2)
    {
      data1 ^= data2;

      return data1;
    }

    TEST(test_xor_equals_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data1(0x12345678UL);
      ETL_CONSTEXPR14 etl::bitset<64> data2(0x23456789UL);

      ETL_CONSTEXPR14 etl::bitset<64> data3(test_xor_equals_operator_helper(data1, data2));

      CHECK((data1 ^ data2) == data3);
    }

    //*************************************************************************
    TEST(test_find_first)
    {
      ETL_CONSTEXPR14 etl::bitset<8> bs1(ull(0x00));
      ETL_CONSTEXPR14 size_t bs1find_first_false = bs1.find_first(false);
      ETL_CONSTEXPR14 size_t bs1find_first_true = bs1.find_first(true);
      CHECK_EQUAL(0U, bs1find_first_false);
      CHECK_EQUAL(etl::bitset<>::npos, bs1find_first_true);

      ETL_CONSTEXPR14 etl::bitset<8> bs2(ull(0xFF));
      ETL_CONSTEXPR14 size_t bs2find_first_false = bs2.find_first(false);
      ETL_CONSTEXPR14 size_t bs2find_first_true = bs2.find_first(true);
      CHECK_EQUAL(etl::bitset<>::npos, bs2find_first_false);
      CHECK_EQUAL(0U, bs2find_first_true);

      ETL_CONSTEXPR14 etl::bitset<8> bs3(ull(0x01));
      ETL_CONSTEXPR14 size_t bs3find_first_false = bs3.find_first(false);
      ETL_CONSTEXPR14 size_t bs3find_first_true = bs3.find_first(true);
      CHECK_EQUAL(1U, bs3find_first_false);
      CHECK_EQUAL(0U, bs3find_first_true);

      ETL_CONSTEXPR14 etl::bitset<8> bs4(ull(0x20));
      ETL_CONSTEXPR14 size_t bs4find_first_false = bs4.find_first(false);
      ETL_CONSTEXPR14 size_t bs4find_first_true = bs4.find_first(true);
      CHECK_EQUAL(0U, bs4find_first_false);
      CHECK_EQUAL(5U, bs4find_first_true);

      ETL_CONSTEXPR14 etl::bitset<8> bs5(ull(0x21));
      ETL_CONSTEXPR14 size_t bs5find_first_false = bs5.find_first(false);
      ETL_CONSTEXPR14 size_t bs5find_first_true = bs5.find_first(true);
      CHECK_EQUAL(1U, bs5find_first_false);
      CHECK_EQUAL(0U, bs5find_first_true);

      ETL_CONSTEXPR14 etl::bitset<8> bs6(ull(0x0E));
      ETL_CONSTEXPR14 size_t bs6find_first_false = bs6.find_first(false);
      ETL_CONSTEXPR14 size_t bs6find_first_true = bs6.find_first(true);
      CHECK_EQUAL(0U, bs6find_first_false);
      CHECK_EQUAL(1U, bs6find_first_true);

      ETL_CONSTEXPR14 etl::bitset<8> bs7(ull(0x31));
      ETL_CONSTEXPR14 size_t bs7find_first_false = bs7.find_first(false);
      ETL_CONSTEXPR14 size_t bs7find_first_true = bs7.find_first(true);
      CHECK_EQUAL(1U, bs7find_first_false);
      CHECK_EQUAL(0U, bs7find_first_true);
    }

    //*************************************************************************
    TEST(test_find_next)
    {
      ETL_CONSTEXPR14 etl::bitset<8> bs1(ull(0x00));
      ETL_CONSTEXPR14 size_t bs1find_next_false0 = bs1.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs1find_next_false1 = bs1.find_next(false, 1);
      ETL_CONSTEXPR14 size_t bs1find_next_true2 = bs1.find_next(true, 2);
      CHECK_EQUAL(0U, bs1find_next_false0);
      CHECK_EQUAL(1U, bs1find_next_false1);
      CHECK_EQUAL(etl::bitset<>::npos, bs1find_next_true2);

      ETL_CONSTEXPR14 etl::bitset<8> bs2(ull(0xFF));
      ETL_CONSTEXPR14 size_t bs2find_next_true0 = bs2.find_next(true, 0);
      ETL_CONSTEXPR14 size_t bs2find_next_true1 = bs2.find_next(true, 1);
      ETL_CONSTEXPR14 size_t bs2find_next_false2 = bs2.find_next(false, 2);
      CHECK_EQUAL(0U, bs2find_next_true0);
      CHECK_EQUAL(1U, bs2find_next_true1);
      CHECK_EQUAL(etl::bitset<>::npos, bs2find_next_false2);

      ETL_CONSTEXPR14 etl::bitset<8> bs3(ull(0x0E));
      ETL_CONSTEXPR14 size_t bs3find_next_false0 = bs3.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs3find_next_true1 = bs3.find_next(true, 1);
      ETL_CONSTEXPR14 size_t bs3find_next_false2 = bs3.find_next(false, 2);
      CHECK_EQUAL(0U, bs3find_next_false0);
      CHECK_EQUAL(1U, bs3find_next_true1);
      CHECK_EQUAL(4U, bs3find_next_false2);

      ETL_CONSTEXPR14 etl::bitset<8> bs4(ull(0x31));
      ETL_CONSTEXPR14 size_t bs4find_next_true0 = bs4.find_next(true, 0);
      ETL_CONSTEXPR14 size_t bs4find_next_false0 = bs4.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs4find_next_true1 = bs4.find_next(true, 1);
      CHECK_EQUAL(0U, bs4find_next_true0);
      CHECK_EQUAL(1U, bs4find_next_false0);
      CHECK_EQUAL(4U, bs4find_next_true1);
    }

    //*************************************************************************
    ETL_CONSTEXPR14 std::pair<etl::bitset<8>, etl::bitset<8>> test_swap_helper()
    {
      etl::bitset<8> data1(ull(0x2A));
      etl::bitset<8> data2(ull(0x15));

      swap(data1, data2);

      return std::pair<etl::bitset<8>, etl::bitset<8>>(data1, data2);
    }

    TEST(test_swap)
    {
      ETL_CONSTEXPR14 etl::bitset<8> compare1(0x2A);
      ETL_CONSTEXPR14 etl::bitset<8> compare2(0x15);

      ETL_CONSTEXPR14 std::pair<etl::bitset<8>, etl::bitset<8>> swapped = test_swap_helper();

      CHECK(swapped.first == compare2);
      CHECK(swapped.second == compare1);
    }

    //*************************************************************************
    TEST(test_span)
    {
      using span_t = etl::bitset<32>::span_type;

      etl::bitset<32> b(0x12345678UL);

      span_t s = b.span();

      if (etl::endianness::value() == etl::endian::little)
      {
        CHECK_EQUAL(0x78UL, s[0]);
        CHECK_EQUAL(0x56UL, s[1]);
        CHECK_EQUAL(0x34UL, s[2]);
        CHECK_EQUAL(0x12UL, s[3]);
      }
      else
      {
        CHECK_EQUAL(0x78UL, s[3]);
        CHECK_EQUAL(0x56UL, s[2]);
        CHECK_EQUAL(0x34UL, s[1]);
        CHECK_EQUAL(0x12UL, s[0]);
      }

      s[2] = 0x9AU;
      uint32_t value = b.value<uint32_t>();
      CHECK_EQUAL(0x129A5678UL, value);
    }

    //*************************************************************************
    TEST(test_const_span)
    {
      using span_t = etl::bitset<32>::const_span_type;

      const etl::bitset<32> b(0x12345678UL);

      span_t s = b.span();

      if (etl::endianness::value() == etl::endian::little)
      {
        CHECK_EQUAL(0x78UL, s[0]);
        CHECK_EQUAL(0x56UL, s[1]);
        CHECK_EQUAL(0x34UL, s[2]);
        CHECK_EQUAL(0x12UL, s[3]);
      }
      else
      {
        CHECK_EQUAL(0x78UL, s[3]);
        CHECK_EQUAL(0x56UL, s[2]);
        CHECK_EQUAL(0x34UL, s[1]);
        CHECK_EQUAL(0x12UL, s[0]);
      }
    }

    //*************************************************************************
    TEST(test_to_string)
    {
      using bs32 = etl::bitset<32>;

      ETL_CONSTEXPR14 bs32 b(0x12345678UL);

      etl::string<32> text = b.to_string('.', '*');
      std::string stdtext = b.to_string<std::string>('.', '*');

      //CHECK_THROW(b.to_string<etl::string<30>>('.', '*'), etl::bitset_overflow);
      CHECK_EQUAL(std::string("...*..*...**.*...*.*.**..****..."), std::string(text.c_str()));
      CHECK_EQUAL(std::string("...*..*...**.*...*.*.**..****..."), std::string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_wstring)
    {
      using bs32 = etl::bitset<32>;

      ETL_CONSTEXPR14 bs32 b(0x12345678UL);

      etl::wstring<32> text = b.to_string<etl::wstring<32>>(L'.', L'*');
      std::wstring stdtext = b.to_string<std::wstring>(L'.', L'*');

      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(text.c_str()));
      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u16string)
    {
      using bs32 = etl::bitset<32>;

      ETL_CONSTEXPR14 bs32 b(0x12345678UL);

      etl::u16string<32> text = b.to_string<etl::u16string<32>>(u'.', u'*');
      std::u16string stdtext = b.to_string<std::u16string>(u'.', u'*');

      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(text.c_str()));
      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u32string)
    {
      using bs32 = etl::bitset<32>;

      ETL_CONSTEXPR14 bs32 b(0x12345678UL);

      etl::u32string<32> text = b.to_string<etl::u32string<32>>(U'.', U'*');
      std::u32string stdtext = b.to_string<std::u32string>(U'.', U'*');

      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(text.c_str()));
      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(stdtext.c_str()));
    }

    ////*************************************************************************
    TEST(test_extract_6_bit_uint8_t_with_run_time_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(uint8_t(0x38), (b.extract<uint8_t>(0, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x3C), (b.extract<uint8_t>(1, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x1E), (b.extract<uint8_t>(2, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x0F), (b.extract<uint8_t>(3, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x27), (b.extract<uint8_t>(4, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x33), (b.extract<uint8_t>(5, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x19), (b.extract<uint8_t>(6, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x2C), (b.extract<uint8_t>(7, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x16), (b.extract<uint8_t>(8, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x2B), (b.extract<uint8_t>(9, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x15), (b.extract<uint8_t>(10, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x0A), (b.extract<uint8_t>(11, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x05), (b.extract<uint8_t>(12, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x22), (b.extract<uint8_t>(13, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x11), (b.extract<uint8_t>(14, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x28), (b.extract<uint8_t>(15, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x34), (b.extract<uint8_t>(16, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x1A), (b.extract<uint8_t>(17, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x0D), (b.extract<uint8_t>(18, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x06), (b.extract<uint8_t>(19, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x23), (b.extract<uint8_t>(20, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x11), (b.extract<uint8_t>(21, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x08), (b.extract<uint8_t>(22, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x24), (b.extract<uint8_t>(23, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x12), (b.extract<uint8_t>(24, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x09), (b.extract<uint8_t>(25, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x04), (b.extract<uint8_t>(26, 6)));

      CHECK_THROW(b.extract<uint8_t>(26, 7), etl::bitset_overflow);
      CHECK_THROW(b.extract<uint8_t>(27, 6), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_extract_6_bit_uint8_t_with_template_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(uint8_t(0x38), (b.extract<uint8_t, 0, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x3C), (b.extract<uint8_t, 1, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x1E), (b.extract<uint8_t, 2, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x0F), (b.extract<uint8_t, 3, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x27), (b.extract<uint8_t, 4, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x33), (b.extract<uint8_t, 5, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x19), (b.extract<uint8_t, 6, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x2C), (b.extract<uint8_t, 7, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x16), (b.extract<uint8_t, 8, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x2B), (b.extract<uint8_t, 9, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x15), (b.extract<uint8_t, 10, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x0A), (b.extract<uint8_t, 11, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x05), (b.extract<uint8_t, 12, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x22), (b.extract<uint8_t, 13, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x11), (b.extract<uint8_t, 14, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x28), (b.extract<uint8_t, 15, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x34), (b.extract<uint8_t, 16, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x1A), (b.extract<uint8_t, 17, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x0D), (b.extract<uint8_t, 18, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x06), (b.extract<uint8_t, 19, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x23), (b.extract<uint8_t, 20, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x11), (b.extract<uint8_t, 21, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x08), (b.extract<uint8_t, 22, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x24), (b.extract<uint8_t, 23, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x12), (b.extract<uint8_t, 24, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x09), (b.extract<uint8_t, 25, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x04), (b.extract<uint8_t, 26, 6>()));

      // The lines below should static assert.
      //uint8_t v1 = b.extract<uint8_t, 26, 7>();
      //uint8_t v1 = b.extract<uint8_t, 27, 6>();
    }

    //*************************************************************************
    TEST(test_extract_6_bit_int8_t_with_run_time_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(int8_t(0xF8), (b.extract<int8_t>(0, 6)));
      CHECK_EQUAL_HEX(int8_t(0xFC), (b.extract<int8_t>(1, 6)));
      CHECK_EQUAL_HEX(int8_t(0x1E), (b.extract<int8_t>(2, 6)));
      CHECK_EQUAL_HEX(int8_t(0x0F), (b.extract<int8_t>(3, 6)));
      CHECK_EQUAL_HEX(int8_t(0xE7), (b.extract<int8_t>(4, 6)));
      CHECK_EQUAL_HEX(int8_t(0xF3), (b.extract<int8_t>(5, 6)));
      CHECK_EQUAL_HEX(int8_t(0x19), (b.extract<int8_t>(6, 6)));
      CHECK_EQUAL_HEX(int8_t(0xEC), (b.extract<int8_t>(7, 6)));
      CHECK_EQUAL_HEX(int8_t(0x16), (b.extract<int8_t>(8, 6)));
      CHECK_EQUAL_HEX(int8_t(0xEB), (b.extract<int8_t>(9, 6)));
      CHECK_EQUAL_HEX(int8_t(0x15), (b.extract<int8_t>(10, 6)));
      CHECK_EQUAL_HEX(int8_t(0x0A), (b.extract<int8_t>(11, 6)));
      CHECK_EQUAL_HEX(int8_t(0x05), (b.extract<int8_t>(12, 6)));
      CHECK_EQUAL_HEX(int8_t(0xE2), (b.extract<int8_t>(13, 6)));
      CHECK_EQUAL_HEX(int8_t(0x11), (b.extract<int8_t>(14, 6)));
      CHECK_EQUAL_HEX(int8_t(0xE8), (b.extract<int8_t>(15, 6)));
      CHECK_EQUAL_HEX(int8_t(0xF4), (b.extract<int8_t>(16, 6)));
      CHECK_EQUAL_HEX(int8_t(0x1A), (b.extract<int8_t>(17, 6)));
      CHECK_EQUAL_HEX(int8_t(0x0D), (b.extract<int8_t>(18, 6)));
      CHECK_EQUAL_HEX(int8_t(0x06), (b.extract<int8_t>(19, 6)));
      CHECK_EQUAL_HEX(int8_t(0xE3), (b.extract<int8_t>(20, 6)));
      CHECK_EQUAL_HEX(int8_t(0x11), (b.extract<int8_t>(21, 6)));
      CHECK_EQUAL_HEX(int8_t(0x08), (b.extract<int8_t>(22, 6)));
      CHECK_EQUAL_HEX(int8_t(0xE4), (b.extract<int8_t>(23, 6)));
      CHECK_EQUAL_HEX(int8_t(0x12), (b.extract<int8_t>(24, 6)));
      CHECK_EQUAL_HEX(int8_t(0x09), (b.extract<int8_t>(25, 6)));
      CHECK_EQUAL_HEX(int8_t(0x04), (b.extract<int8_t>(26, 6)));

      CHECK_THROW(b.extract<int8_t>(26, 7), etl::bitset_overflow);
      CHECK_THROW(b.extract<int8_t>(27, 6), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_extract_6_bit_int8_t_with_template_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(int8_t(0xF8), (b.extract<int8_t, 0, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xFC), (b.extract<int8_t, 1, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x1E), (b.extract<int8_t, 2, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x0F), (b.extract<int8_t, 3, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xE7), (b.extract<int8_t, 4, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xF3), (b.extract<int8_t, 5, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x19), (b.extract<int8_t, 6, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xEC), (b.extract<int8_t, 7, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x16), (b.extract<int8_t, 8, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xEB), (b.extract<int8_t, 9, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x15), (b.extract<int8_t, 10, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x0A), (b.extract<int8_t, 11, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x05), (b.extract<int8_t, 12, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xE2), (b.extract<int8_t, 13, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x11), (b.extract<int8_t, 14, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xE8), (b.extract<int8_t, 15, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xF4), (b.extract<int8_t, 16, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x1A), (b.extract<int8_t, 17, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x0D), (b.extract<int8_t, 18, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x06), (b.extract<int8_t, 19, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xE3), (b.extract<int8_t, 20, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x11), (b.extract<int8_t, 21, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x08), (b.extract<int8_t, 22, 6>()));
      CHECK_EQUAL_HEX(int8_t(0xE4), (b.extract<int8_t, 23, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x12), (b.extract<int8_t, 24, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x09), (b.extract<int8_t, 25, 6>()));
      CHECK_EQUAL_HEX(int8_t(0x04), (b.extract<int8_t, 26, 6>()));

      // The lines below should static assert.
      //int8_t v1 = b.extract<int8_t, 26, 7>();
      //int8_t v1 = b.extract<int8_t, 27, 6>();
    }

    //*************************************************************************
    TEST(test_extract_uint8_t_with_run_time_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(uint8_t(0x78), b.extract<uint8_t>(0, 8));
      CHECK_EQUAL_HEX(uint8_t(0x3C), b.extract<uint8_t>(1, 8));
      CHECK_EQUAL_HEX(uint8_t(0x9E), b.extract<uint8_t>(2, 8));
      CHECK_EQUAL_HEX(uint8_t(0xCF), b.extract<uint8_t>(3, 8));
      CHECK_EQUAL_HEX(uint8_t(0x67), b.extract<uint8_t>(4, 8));
      CHECK_EQUAL_HEX(uint8_t(0xB3), b.extract<uint8_t>(5, 8));
      CHECK_EQUAL_HEX(uint8_t(0x59), b.extract<uint8_t>(6, 8));
      CHECK_EQUAL_HEX(uint8_t(0xAC), b.extract<uint8_t>(7, 8));
      CHECK_EQUAL_HEX(uint8_t(0x56), b.extract<uint8_t>(8, 8));
      CHECK_EQUAL_HEX(uint8_t(0x2B), b.extract<uint8_t>(9, 8));
      CHECK_EQUAL_HEX(uint8_t(0x15), b.extract<uint8_t>(10, 8));
      CHECK_EQUAL_HEX(uint8_t(0x8A), b.extract<uint8_t>(11, 8));
      CHECK_EQUAL_HEX(uint8_t(0x45), b.extract<uint8_t>(12, 8));
      CHECK_EQUAL_HEX(uint8_t(0xA2), b.extract<uint8_t>(13, 8));
      CHECK_EQUAL_HEX(uint8_t(0xD1), b.extract<uint8_t>(14, 8));
      CHECK_EQUAL_HEX(uint8_t(0x68), b.extract<uint8_t>(15, 8));
      CHECK_EQUAL_HEX(uint8_t(0x34), b.extract<uint8_t>(16, 8));
      CHECK_EQUAL_HEX(uint8_t(0x1A), b.extract<uint8_t>(17, 8));
      CHECK_EQUAL_HEX(uint8_t(0x8D), b.extract<uint8_t>(18, 8));
      CHECK_EQUAL_HEX(uint8_t(0x46), b.extract<uint8_t>(19, 8));
      CHECK_EQUAL_HEX(uint8_t(0x23), b.extract<uint8_t>(20, 8));
      CHECK_EQUAL_HEX(uint8_t(0x91), b.extract<uint8_t>(21, 8));
      CHECK_EQUAL_HEX(uint8_t(0x48), b.extract<uint8_t>(22, 8));
      CHECK_EQUAL_HEX(uint8_t(0x24), b.extract<uint8_t>(23, 8));
      CHECK_EQUAL_HEX(uint8_t(0x12), b.extract<uint8_t>(24, 8));

      CHECK_THROW(b.extract<uint8_t>(24, 9), etl::bitset_overflow);
      CHECK_THROW(b.extract<uint8_t>(25, 8), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_extract_uint8_t_with_template_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(uint8_t(0x78), (b.extract<uint8_t, 0, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x3C), (b.extract<uint8_t, 1, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x9E), (b.extract<uint8_t, 2, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xCF), (b.extract<uint8_t, 3, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x67), (b.extract<uint8_t, 4, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xB3), (b.extract<uint8_t, 5, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x59), (b.extract<uint8_t, 6, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xAC), (b.extract<uint8_t, 7, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x56), (b.extract<uint8_t, 8, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x2B), (b.extract<uint8_t, 9, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x15), (b.extract<uint8_t, 10, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x8A), (b.extract<uint8_t, 11, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x45), (b.extract<uint8_t, 12, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xA2), (b.extract<uint8_t, 13, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xD1), (b.extract<uint8_t, 14, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x68), (b.extract<uint8_t, 15, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x34), (b.extract<uint8_t, 16, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x1A), (b.extract<uint8_t, 17, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x8D), (b.extract<uint8_t, 18, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x46), (b.extract<uint8_t, 19, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x23), (b.extract<uint8_t, 20, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x91), (b.extract<uint8_t, 21, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x48), (b.extract<uint8_t, 22, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x24), (b.extract<uint8_t, 23, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x12), (b.extract<uint8_t, 24, 8>()));

      // The lines below should static assert.
      //uint8_t v1 = b.extract<uint8_t, 24, 9>();
      //uint8_t v2 = b.extract<uint8_t, 25, 8>();
    }

    //*************************************************************************
    TEST(test_extract_int8_t_with_run_time_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(int8_t(0x78), b.extract<int8_t>(0, 8));
      CHECK_EQUAL_HEX(int8_t(0x3C), b.extract<int8_t>(1, 8));
      CHECK_EQUAL_HEX(int8_t(0x9E), b.extract<int8_t>(2, 8));
      CHECK_EQUAL_HEX(int8_t(0xCF), b.extract<int8_t>(3, 8));
      CHECK_EQUAL_HEX(int8_t(0x67), b.extract<int8_t>(4, 8));
      CHECK_EQUAL_HEX(int8_t(0xB3), b.extract<int8_t>(5, 8));
      CHECK_EQUAL_HEX(int8_t(0x59), b.extract<int8_t>(6, 8));
      CHECK_EQUAL_HEX(int8_t(0xAC), b.extract<int8_t>(7, 8));
      CHECK_EQUAL_HEX(int8_t(0x56), b.extract<int8_t>(8, 8));
      CHECK_EQUAL_HEX(int8_t(0x2B), b.extract<int8_t>(9, 8));
      CHECK_EQUAL_HEX(int8_t(0x15), b.extract<int8_t>(10, 8));
      CHECK_EQUAL_HEX(int8_t(0x8A), b.extract<int8_t>(11, 8));
      CHECK_EQUAL_HEX(int8_t(0x45), b.extract<int8_t>(12, 8));
      CHECK_EQUAL_HEX(int8_t(0xA2), b.extract<int8_t>(13, 8));
      CHECK_EQUAL_HEX(int8_t(0xD1), b.extract<int8_t>(14, 8));
      CHECK_EQUAL_HEX(int8_t(0x68), b.extract<int8_t>(15, 8));
      CHECK_EQUAL_HEX(int8_t(0x34), b.extract<int8_t>(16, 8));
      CHECK_EQUAL_HEX(int8_t(0x1A), b.extract<int8_t>(17, 8));
      CHECK_EQUAL_HEX(int8_t(0x8D), b.extract<int8_t>(18, 8));
      CHECK_EQUAL_HEX(int8_t(0x46), b.extract<int8_t>(19, 8));
      CHECK_EQUAL_HEX(int8_t(0x23), b.extract<int8_t>(20, 8));
      CHECK_EQUAL_HEX(int8_t(0x91), b.extract<int8_t>(21, 8));
      CHECK_EQUAL_HEX(int8_t(0x48), b.extract<int8_t>(22, 8));
      CHECK_EQUAL_HEX(int8_t(0x24), b.extract<int8_t>(23, 8));
      CHECK_EQUAL_HEX(int8_t(0x12), b.extract<int8_t>(24, 8));

      CHECK_THROW(b.extract<int8_t>(24, 9), etl::bitset_overflow);
      CHECK_THROW(b.extract<int8_t>(25, 8), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_extract_int8_t_with_template_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(int8_t(0x78), (b.extract<int8_t, 0, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x3C), (b.extract<int8_t, 1, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x9E), (b.extract<int8_t, 2, 8>()));
      CHECK_EQUAL_HEX(int8_t(0xCF), (b.extract<int8_t, 3, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x67), (b.extract<int8_t, 4, 8>()));
      CHECK_EQUAL_HEX(int8_t(0xB3), (b.extract<int8_t, 5, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x59), (b.extract<int8_t, 6, 8>()));
      CHECK_EQUAL_HEX(int8_t(0xAC), (b.extract<int8_t, 7, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x56), (b.extract<int8_t, 8, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x2B), (b.extract<int8_t, 9, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x15), (b.extract<int8_t, 10, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x8A), (b.extract<int8_t, 11, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x45), (b.extract<int8_t, 12, 8>()));
      CHECK_EQUAL_HEX(int8_t(0xA2), (b.extract<int8_t, 13, 8>()));
      CHECK_EQUAL_HEX(int8_t(0xD1), (b.extract<int8_t, 14, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x68), (b.extract<int8_t, 15, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x34), (b.extract<int8_t, 16, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x1A), (b.extract<int8_t, 17, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x8D), (b.extract<int8_t, 18, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x46), (b.extract<int8_t, 19, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x23), (b.extract<int8_t, 20, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x91), (b.extract<int8_t, 21, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x48), (b.extract<int8_t, 22, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x24), (b.extract<int8_t, 23, 8>()));
      CHECK_EQUAL_HEX(int8_t(0x12), (b.extract<int8_t, 24, 8>()));

      // The lines below should static assert.
      //int8_t v1 = b.extract<int8_t, 24, 9>();
      //int8_t v2 = b.extract<int8_t, 25, 8>();
    }

    //*************************************************************************
    TEST(test_extract_13_bits_uint16_t_with_run_time_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(uint16_t(0x1678), b.extract<uint16_t>(0, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0B3C), b.extract<uint16_t>(1, 13));
      CHECK_EQUAL_HEX(uint16_t(0x159E), b.extract<uint16_t>(2, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0ACF), b.extract<uint16_t>(3, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0567), b.extract<uint16_t>(4, 13));
      CHECK_EQUAL_HEX(uint16_t(0x02B3), b.extract<uint16_t>(5, 13));
      CHECK_EQUAL_HEX(uint16_t(0x1159), b.extract<uint16_t>(6, 13));
      CHECK_EQUAL_HEX(uint16_t(0x08AC), b.extract<uint16_t>(7, 13));
      CHECK_EQUAL_HEX(uint16_t(0x1456), b.extract<uint16_t>(8, 13));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), b.extract<uint16_t>(9, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0D15), b.extract<uint16_t>(10, 13));
      CHECK_EQUAL_HEX(uint16_t(0x068A), b.extract<uint16_t>(11, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0345), b.extract<uint16_t>(12, 13));
      CHECK_EQUAL_HEX(uint16_t(0x11A2), b.extract<uint16_t>(13, 13));
      CHECK_EQUAL_HEX(uint16_t(0x08D1), b.extract<uint16_t>(14, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0468), b.extract<uint16_t>(15, 13));
      CHECK_EQUAL_HEX(uint16_t(0x1234), b.extract<uint16_t>(16, 13));
      CHECK_EQUAL_HEX(uint16_t(0x091A), b.extract<uint16_t>(17, 13));
      CHECK_EQUAL_HEX(uint16_t(0x048D), b.extract<uint16_t>(18, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0246), b.extract<uint16_t>(19, 13));

      CHECK_THROW(b.extract<uint16_t>(19, 14), etl::bitset_overflow);
      CHECK_THROW(b.extract<uint16_t>(20, 13), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_extract_13_bits_uint16_t_with_template_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(uint16_t(0x1678), (b.extract<uint16_t, 0, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0B3C), (b.extract<uint16_t, 1, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x159E), (b.extract<uint16_t, 2, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0ACF), (b.extract<uint16_t, 3, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0567), (b.extract<uint16_t, 4, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x02B3), (b.extract<uint16_t, 5, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x1159), (b.extract<uint16_t, 6, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x08AC), (b.extract<uint16_t, 7, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x1456), (b.extract<uint16_t, 8, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), (b.extract<uint16_t, 9, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0D15), (b.extract<uint16_t, 10, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x068A), (b.extract<uint16_t, 11, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0345), (b.extract<uint16_t, 12, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x11A2), (b.extract<uint16_t, 13, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x08D1), (b.extract<uint16_t, 14, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0468), (b.extract<uint16_t, 15, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x1234), (b.extract<uint16_t, 16, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x091A), (b.extract<uint16_t, 17, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x048D), (b.extract<uint16_t, 18, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0246), (b.extract<uint16_t, 19, 13>()));

      // The lines below should static assert.
      //uint16_t v1 = b.extract<uint16_t, 19, 14);
      //uint16_t v2 = b.extract<uint16_t, 20, 13);
    }

    //*************************************************************************
    TEST(test_extract_uint16_t_with_run_time_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(uint16_t(0x5678), b.extract<uint16_t>(0, 16));
      CHECK_EQUAL_HEX(uint16_t(0x2B3C), b.extract<uint16_t>(1, 16));
      CHECK_EQUAL_HEX(uint16_t(0x159E), b.extract<uint16_t>(2, 16));
      CHECK_EQUAL_HEX(uint16_t(0x8ACF), b.extract<uint16_t>(3, 16));
      CHECK_EQUAL_HEX(uint16_t(0x4567), b.extract<uint16_t>(4, 16));
      CHECK_EQUAL_HEX(uint16_t(0xA2B3), b.extract<uint16_t>(5, 16));
      CHECK_EQUAL_HEX(uint16_t(0xD159), b.extract<uint16_t>(6, 16));
      CHECK_EQUAL_HEX(uint16_t(0x68AC), b.extract<uint16_t>(7, 16));
      CHECK_EQUAL_HEX(uint16_t(0x3456), b.extract<uint16_t>(8, 16));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), b.extract<uint16_t>(9, 16));
      CHECK_EQUAL_HEX(uint16_t(0x8D15), b.extract<uint16_t>(10, 16));
      CHECK_EQUAL_HEX(uint16_t(0x468A), b.extract<uint16_t>(11, 16));
      CHECK_EQUAL_HEX(uint16_t(0x2345), b.extract<uint16_t>(12, 16));
      CHECK_EQUAL_HEX(uint16_t(0x91A2), b.extract<uint16_t>(13, 16));
      CHECK_EQUAL_HEX(uint16_t(0x48D1), b.extract<uint16_t>(14, 16));
      CHECK_EQUAL_HEX(uint16_t(0x2468), b.extract<uint16_t>(15, 16));
      CHECK_EQUAL_HEX(uint16_t(0x1234), b.extract<uint16_t>(16, 16));

      CHECK_THROW(b.extract<uint16_t>(16, 17), etl::bitset_overflow);
      CHECK_THROW(b.extract<uint16_t>(17, 16), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_extract_uint16_t_with_template_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(uint16_t(0x5678), (b.extract<uint16_t, 0, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x2B3C), (b.extract<uint16_t, 1, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x159E), (b.extract<uint16_t, 2, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x8ACF), (b.extract<uint16_t, 3, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x4567), (b.extract<uint16_t, 4, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0xA2B3), (b.extract<uint16_t, 5, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0xD159), (b.extract<uint16_t, 6, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x68AC), (b.extract<uint16_t, 7, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x3456), (b.extract<uint16_t, 8, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), (b.extract<uint16_t, 9, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x8D15), (b.extract<uint16_t, 10, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x468A), (b.extract<uint16_t, 11, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x2345), (b.extract<uint16_t, 12, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x91A2), (b.extract<uint16_t, 13, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x48D1), (b.extract<uint16_t, 14, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x2468), (b.extract<uint16_t, 15, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x1234), (b.extract<uint16_t, 16, 16>()));

      // The lines below should static assert.
      //uint16_t v1 = b.extract<uint16_t, 16, 17>();
      //uint16_t v2 = b.extract<uint16_t, 17, 16>();
    }

    //*************************************************************************
    TEST(test_extract_int16_t_with_run_time_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(int16_t(0x5678), b.extract<int16_t>(0, 16));
      CHECK_EQUAL_HEX(int16_t(0x2B3C), b.extract<int16_t>(1, 16));
      CHECK_EQUAL_HEX(int16_t(0x159E), b.extract<int16_t>(2, 16));
      CHECK_EQUAL_HEX(int16_t(0x8ACF), b.extract<int16_t>(3, 16));
      CHECK_EQUAL_HEX(int16_t(0x4567), b.extract<int16_t>(4, 16));
      CHECK_EQUAL_HEX(int16_t(0xA2B3), b.extract<int16_t>(5, 16));
      CHECK_EQUAL_HEX(int16_t(0xD159), b.extract<int16_t>(6, 16));
      CHECK_EQUAL_HEX(int16_t(0x68AC), b.extract<int16_t>(7, 16));
      CHECK_EQUAL_HEX(int16_t(0x3456), b.extract<int16_t>(8, 16));
      CHECK_EQUAL_HEX(int16_t(0x1A2B), b.extract<int16_t>(9, 16));
      CHECK_EQUAL_HEX(int16_t(0x8D15), b.extract<int16_t>(10, 16));
      CHECK_EQUAL_HEX(int16_t(0x468A), b.extract<int16_t>(11, 16));
      CHECK_EQUAL_HEX(int16_t(0x2345), b.extract<int16_t>(12, 16));
      CHECK_EQUAL_HEX(int16_t(0x91A2), b.extract<int16_t>(13, 16));
      CHECK_EQUAL_HEX(int16_t(0x48D1), b.extract<int16_t>(14, 16));
      CHECK_EQUAL_HEX(int16_t(0x2468), b.extract<int16_t>(15, 16));
      CHECK_EQUAL_HEX(int16_t(0x1234), b.extract<int16_t>(16, 16));

      CHECK_THROW(b.extract<int16_t>(16, 17), etl::bitset_overflow);
      CHECK_THROW(b.extract<int16_t>(17, 16), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_extract_int16_t_with_template_parameters)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL_HEX(int16_t(0x5678), (b.extract<int16_t, 0, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x2B3C), (b.extract<int16_t, 1, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x159E), (b.extract<int16_t, 2, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x8ACF), (b.extract<int16_t, 3, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x4567), (b.extract<int16_t, 4, 16>()));
      CHECK_EQUAL_HEX(int16_t(0xA2B3), (b.extract<int16_t, 5, 16>()));
      CHECK_EQUAL_HEX(int16_t(0xD159), (b.extract<int16_t, 6, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x68AC), (b.extract<int16_t, 7, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x3456), (b.extract<int16_t, 8, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x1A2B), (b.extract<int16_t, 9, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x8D15), (b.extract<int16_t, 10, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x468A), (b.extract<int16_t, 11, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x2345), (b.extract<int16_t, 12, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x91A2), (b.extract<int16_t, 13, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x48D1), (b.extract<int16_t, 14, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x2468), (b.extract<int16_t, 15, 16>()));
      CHECK_EQUAL_HEX(int16_t(0x1234), (b.extract<int16_t, 16, 16>()));

      // The lines below should static assert.
      //int16_t v1 = b.extract<int16_t, 16, 17>());
      //int16_t v2 = b.extract<int16_t, 17, 16>());
    }

    //*************************************************************************
    bool test_bit(int i)
    {
      return (0x1234 & (1 << i)) != 0;
    }

    TEST(test_test_run_time)
    {
      ETL_CONSTEXPR14 etl::bitset<16> b(0x1234);

      ETL_CONSTEXPR14 bool t0 = b.test(0);
      ETL_CONSTEXPR14 bool t1 = b.test(1);
      ETL_CONSTEXPR14 bool t2 = b.test(2);
      ETL_CONSTEXPR14 bool t3 = b.test(3);
      ETL_CONSTEXPR14 bool t4 = b.test(4);
      ETL_CONSTEXPR14 bool t5 = b.test(5);
      ETL_CONSTEXPR14 bool t6 = b.test(6);
      ETL_CONSTEXPR14 bool t7 = b.test(7);

      CHECK_EQUAL(test_bit(0), t0);
      CHECK_EQUAL(test_bit(1), t1);
      CHECK_EQUAL(test_bit(2), t2);
      CHECK_EQUAL(test_bit(3), t3);
      CHECK_EQUAL(test_bit(4), t4);
      CHECK_EQUAL(test_bit(5), t5);
      CHECK_EQUAL(test_bit(6), t6);
      CHECK_EQUAL(test_bit(7), t7);
    }

    //*************************************************************************
    TEST(test_test_compile_time)
    {
      ETL_CONSTEXPR14 etl::bitset<16> b(0x1234);

      ETL_CONSTEXPR14 bool t0 = b.test<0>();
      ETL_CONSTEXPR14 bool t1 = b.test<1>();
      ETL_CONSTEXPR14 bool t2 = b.test<2>();
      ETL_CONSTEXPR14 bool t3 = b.test<3>();
      ETL_CONSTEXPR14 bool t4 = b.test<4>();
      ETL_CONSTEXPR14 bool t5 = b.test<5>();
      ETL_CONSTEXPR14 bool t6 = b.test<6>();
      ETL_CONSTEXPR14 bool t7 = b.test<7>();

      CHECK_EQUAL(test_bit(0), t0);
      CHECK_EQUAL(test_bit(1), t1);
      CHECK_EQUAL(test_bit(2), t2);
      CHECK_EQUAL(test_bit(3), t3);
      CHECK_EQUAL(test_bit(4), t4);
      CHECK_EQUAL(test_bit(5), t5);
      CHECK_EQUAL(test_bit(6), t6);
      CHECK_EQUAL(test_bit(7), t7);
    }

    //*************************************************************************
    TEST(test_global_size)
    {
      etl::bitset<32> b(0x12345678UL);

      CHECK_EQUAL(32, ETL_OR_STD17::size(b));
    }
  };
}
