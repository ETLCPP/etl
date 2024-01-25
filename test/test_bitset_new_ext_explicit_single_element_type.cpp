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
    TEST(test_constants)
    {
      using bitset_type  = etl::bitset_ext<60, uint64_t>;
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
      constexpr element_type All_Clear_Element1 = bitset_type::All_Clear_Element;
      constexpr element_type All_Clear_Element2 = bitset_type::all_clear_element();

      CHECK_EQUAL(All_Clear_Element, All_Clear_Element1);
      CHECK_EQUAL(All_Clear_Element, All_Clear_Element2);

      //*******************************
      constexpr size_t Allocated_Bits = bitset_type::allocated_bits();

      CHECK_EQUAL(Number_Of_Elements * Bits_Per_Element, Allocated_Bits);

      //*******************************
      constexpr char Storage_Model1 = bitset_type::Storage_Model;
      constexpr char Storage_Model2 = bitset_type::storage_model();

      CHECK_EQUAL(etl::bitset_storage_model::Single, Storage_Model1);
      CHECK_EQUAL(etl::bitset_storage_model::Single, Storage_Model2);

      etl::bitset_storage_model bsm;

      bsm = etl::bitset_storage_model::Undefined;
      CHECK_EQUAL(std::string("Undefined"), std::string(bsm.c_str()));

      bsm = etl::bitset_storage_model::Single;
      CHECK_EQUAL(std::string("Single"), std::string(bsm.c_str()));

      bsm = etl::bitset_storage_model::Multi;
      CHECK_EQUAL(std::string("Multi"), std::string(bsm.c_str()));
    }

    //*************************************************************************
    TEST(test_construct_from_nullptr_buffer)
    {
      using BsExt = etl::bitset_ext<64, uint64_t>;

      BsExt::buffer_type buffer;
      BsExt bs2a(buffer);

      CHECK_THROW(BsExt bs1(nullptr) , etl::bitset_invalid_buffer);
      CHECK_THROW(BsExt bs2b(bs2a, nullptr), etl::bitset_invalid_buffer);
      CHECK_THROW(BsExt bs3(0ULL,  nullptr), etl::bitset_invalid_buffer);
      CHECK_THROW(BsExt bs4("0",   nullptr), etl::bitset_invalid_buffer);
      CHECK_THROW(BsExt bs5(L"0",  nullptr), etl::bitset_invalid_buffer);
      CHECK_THROW(BsExt bs6(u"0",  nullptr), etl::bitset_invalid_buffer);
      CHECK_THROW(BsExt bs7(U"0",  nullptr), etl::bitset_invalid_buffer);
    }

    //*************************************************************************
    TEST(test_default_constructor_from_array)
    {
      etl::bitset_ext<64, uint64_t>::element_type buffer[etl::bitset_ext<64, uint64_t>::Number_Of_Elements];
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<8,  uint8_t>::buffer_type  buffer8;
      etl::bitset_ext<16, uint16_t>::buffer_type buffer16;
      etl::bitset_ext<32, uint32_t>::buffer_type buffer32;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer64;

      etl::bitset_ext<8,  uint8_t>  data8(buffer8);
      etl::bitset_ext<16, uint16_t> data16(buffer16);
      etl::bitset_ext<32, uint32_t> data32(buffer32);
      etl::bitset_ext<64, uint64_t> data64(buffer64);

      CHECK_EQUAL(8,  data8.Size);
      CHECK_EQUAL(16, data16.Size);
      CHECK_EQUAL(32, data32.Size);
      CHECK_EQUAL(64, data64.Size);

      CHECK_EQUAL(1U, data8.Number_Of_Elements);
      CHECK_EQUAL(1U, data16.Number_Of_Elements);
      CHECK_EQUAL(1U, data32.Number_Of_Elements);
      CHECK_EQUAL(1U, data64.Number_Of_Elements);

      CHECK_EQUAL(etl::bitset_storage_model::Single, data8.Storage_Model);
      CHECK_EQUAL(etl::bitset_storage_model::Single, data16.Storage_Model);
      CHECK_EQUAL(etl::bitset_storage_model::Single, data32.Storage_Model);
      CHECK_EQUAL(etl::bitset_storage_model::Single, data64.Storage_Model);
    }

    //*************************************************************************
    TEST(test_construct_from_value)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      std::bitset<64> compare(0x123456731234567ULL);
      etl::bitset_ext<64, uint64_t> data(0x123456731234567ULL, buffer1);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      std::bitset<64> compare(0x123456731234567ULL);
      etl::bitset_ext<64, uint64_t> data(0x123456731234567ULL, buffer1);
      etl::bitset_ext<64, uint64_t> data_copy(data, buffer2);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(0x8765432187654321ULL);
      etl::bitset_ext<64, uint64_t> data(0x8765432187654321ULL, buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data("110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(L"110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(u"110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(U"110001001000110100010101100111001100010010001101000101011001", buffer);

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

      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      etl::bitset_ext<60, uint64_t> data(s, buffer);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_construct_from_nullptr_wchar_t_string)
    {
      const wchar_t* s = nullptr;

      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      etl::bitset_ext<60, uint64_t> data(s, buffer);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_construct_from_nullptr_char16_t_string)
    {
      const char16_t* s = nullptr;

      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      etl::bitset_ext<60, uint64_t> data(s, buffer);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_construct_from_nullptr_char32_t_string)
    {
      const char32_t* s = nullptr;

      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      etl::bitset_ext<60, uint64_t> data(s, buffer);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_construct_from_excess_string)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001111100001");
      etl::bitset_ext<64, uint64_t> data("110001001000110100010101100111001100010010001101000101011001111100001", buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
    TEST(test_set_less_than_full_size)
    {
      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      std::bitset<60> compare;
      etl::bitset_ext<60, uint64_t> data(buffer);

      compare.set();
      data.set();

      auto size = data.size();
      auto count = data.count();
      auto none = data.none();
      auto any = data.any();
      auto all = data.all();

      CHECK_EQUAL(compare.size(), size);
      CHECK_EQUAL(compare.count(), count);
      CHECK_EQUAL(compare.none(), none);
      CHECK_EQUAL(compare.any(), any);
      CHECK_EQUAL(compare.all(), all);

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_set_run_time_position_default_value)
    {
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
    TEST(test_set_run_time_position_value)
    {
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
    TEST(test_set_compile_time_position_run_time_default_value)
    {
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
    TEST(test_set_compile_time_position_run_time_value)
    {
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);

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

      auto size  = data.size();
      auto count = data.count();
      auto none  = data.none();
      auto any   = data.any();
      auto all = data.all();

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
    TEST(test_set_compile_time_position_value)
    {
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
    TEST(test_set_with_char_string)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(buffer);

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
    TEST(test_set_with_char16_t_string)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(buffer);

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
    TEST(test_set_with_char32_t_string)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(buffer);

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
    TEST(test_set_from_char_nullptr)
    {
      const char* s = nullptr;

      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);
      data.set(s);

      CHECK_EQUAL(64, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_set_from_wchar_t_nullptr)
    {
      const wchar_t* s = nullptr;

      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);
      data.set(s);

      CHECK_EQUAL(64, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_set_from_char16_t_nullptr)
    {
      const char16_t* s = nullptr;

      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);
      data.set(s);

      CHECK_EQUAL(64, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_set_from_char32_t_nullptr)
    {
      const char32_t* s = nullptr;

      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);
      data.set(s);

      CHECK_EQUAL(64, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_from_string_with_char)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<64, uint64_t> data(buffer);

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

      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      etl::bitset_ext<60, uint64_t> data(buffer);
      data.from_string(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_from_string_with_wchar_t_nullptr)
    {
      const wchar_t* s = nullptr;

      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      etl::bitset_ext<60, uint64_t> data(buffer);
      data.from_string(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_from_string_with_char16_t_nullptr)
    {
      const char16_t* s = nullptr;

      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      etl::bitset_ext<60, uint64_t> data(buffer);
      data.from_string(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_from_string_with_char32_t_nullptr)
    {
      const char16_t* s = nullptr;

      etl::bitset_ext<60, uint64_t>::buffer_type buffer;
      etl::bitset_ext<60, uint64_t> data(buffer);
      data.from_string(s);

      CHECK_EQUAL(60, data.size());
      CHECK_EQUAL(0, data.count());
    }

    //*************************************************************************
    TEST(test_value_u8_min)
    {
      etl::bitset_ext<8, uint8_t>::buffer_type buffer;
      etl::bitset_ext<8, uint8_t> data((unsigned long long)etl::integral_limits<uint8_t>::min, buffer);
      uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u8_max)
    {
      etl::bitset_ext<8, uint8_t>::buffer_type buffer;
      etl::bitset_ext<8, uint8_t> data((unsigned long long)etl::integral_limits<uint8_t>::max, buffer);
      uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_min)
    {
      etl::bitset_ext<8, uint8_t>::buffer_type buffer;
      etl::bitset_ext<8, uint8_t> data((unsigned long long)etl::integral_limits<uint8_t>::min, buffer);
      uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s8_max)
    {
      etl::bitset_ext<8, uint8_t>::buffer_type buffer;
      etl::bitset_ext<8, uint8_t> data((unsigned long long)etl::integral_limits<uint8_t>::max, buffer);
      uint8_t value = data.value<uint8_t>();

      CHECK_EQUAL(std::numeric_limits<uint8_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_min)
    {
      etl::bitset_ext<16, uint16_t>::buffer_type buffer;
      etl::bitset_ext<16, uint16_t> data((unsigned long long)etl::integral_limits<uint16_t>::min, buffer);
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_max)
    {
      etl::bitset_ext<16, uint16_t>::buffer_type buffer;
      etl::bitset_ext<16, uint16_t> data((unsigned long long)etl::integral_limits<uint16_t>::max, buffer);
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_min)
    {
      etl::bitset_ext<16, uint16_t>::buffer_type buffer;
      etl::bitset_ext<16, uint16_t> data((unsigned long long)etl::integral_limits<uint16_t>::min, buffer);
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_max)
    {
      etl::bitset_ext<16, uint16_t>::buffer_type buffer;
      etl::bitset_ext<16, uint16_t> data((unsigned long long)etl::integral_limits<uint16_t>::max, buffer);
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_min)
    {
      etl::bitset_ext<32, uint32_t>::buffer_type buffer;
      etl::bitset_ext<32, uint32_t> data((unsigned long long)etl::integral_limits<uint32_t>::min, buffer);
      uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_max)
    {
      etl::bitset_ext<32, uint32_t>::buffer_type buffer;
      etl::bitset_ext<32, uint32_t> data((unsigned long long)etl::integral_limits<uint32_t>::max, buffer);
      uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_min)
    {
      etl::bitset_ext<32, uint32_t>::buffer_type buffer;
      etl::bitset_ext<32, uint32_t> data((unsigned long long)etl::integral_limits<uint32_t>::min, buffer);
      uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_max)
    {
      etl::bitset_ext<32, uint32_t>::buffer_type buffer;
      etl::bitset_ext<32, uint32_t> data((unsigned long long)etl::integral_limits<uint32_t>::max, buffer);
      uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_min)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data((unsigned long long)etl::integral_limits<uint64_t>::min, buffer);
      uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_max)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data((unsigned long long)etl::integral_limits<uint64_t>::max, buffer);
      uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_min)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data((unsigned long long)etl::integral_limits<uint64_t>::min, buffer);
      uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_max)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data((unsigned long long)etl::integral_limits<uint64_t>::max, buffer);
      uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_to_ulong_min)
    {
      etl::bitset_ext<etl::integral_limits<unsigned long>::bits, uint64_t>::buffer_type buffer;
      etl::bitset_ext<etl::integral_limits<unsigned long>::bits, uint64_t> data(etl::integral_limits<unsigned long>::min, buffer);
      unsigned long value = data.to_ulong();

      CHECK_EQUAL(std::numeric_limits<unsigned long>::min(), value);
    }

    //*************************************************************************
    TEST(test_to_ulong_max)
    {
      etl::bitset_ext<etl::integral_limits<unsigned long>::bits, uint64_t>::buffer_type buffer;
      etl::bitset_ext<etl::integral_limits<unsigned long>::bits, uint64_t> data(etl::integral_limits<unsigned long>::max, buffer);
      unsigned long value = data.to_ulong();

      CHECK_EQUAL(std::numeric_limits<unsigned long>::max(), value);
    }

    //*************************************************************************
    TEST(test_to_ulong_overflow)
    {
      etl::bitset_ext<etl::integral_limits<unsigned long>::bits + 1, uint64_t>::buffer_type buffer;
      etl::bitset_ext<etl::integral_limits<unsigned long>::bits + 1, uint64_t> data(etl::integral_limits<unsigned long>::min, buffer);
      CHECK_THROW(data.to_ulong(), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_to_ullong_min)
    {
      etl::bitset_ext<etl::integral_limits<unsigned long long>::bits, uint64_t>::buffer_type buffer;
      etl::bitset_ext<etl::integral_limits<unsigned long long>::bits, uint64_t> data(etl::integral_limits<unsigned long long>::min, buffer);
      unsigned long long value = data.to_ullong();

      CHECK_EQUAL(std::numeric_limits<unsigned long>::min(), value);
    }

    //*************************************************************************
    TEST(test_to_ullong_max)
    {
      etl::bitset_ext<etl::integral_limits<unsigned long long>::bits, uint64_t>::buffer_type buffer;
      etl::bitset_ext<etl::integral_limits<unsigned long long>::bits, uint64_t> data(etl::integral_limits<unsigned long long>::max, buffer);
      unsigned long long value = data.to_ullong();

      CHECK_EQUAL(std::numeric_limits<unsigned long long>::max(), value);
    }

    //*************************************************************************
    TEST(test_to_ullong_overflow)
    {
      etl::bitset_ext<etl::integral_limits<unsigned long long>::bits + 1, uint64_t>::buffer_type buffer;
      etl::bitset_ext<etl::integral_limits<unsigned long long>::bits + 1, uint64_t> data(etl::integral_limits<unsigned long long>::min, buffer);    
      
      CHECK_THROW(data.to_ullong(), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_position_set)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<64, uint64_t> data(0xFFFFFFFFFFFFFFFULL, buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<64, uint64_t> data(0xFFFFFFFFFFFFFFFULL, buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare(0x3123456731234567ULL);
      etl::bitset_ext<64, uint64_t> data(0x3123456731234567ULL, buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<64, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data1(ull(0xF000000000000000), buffer1);
      etl::bitset_ext<64, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<64, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data1(ull(0xF000000000000000), buffer1);
      etl::bitset_ext<64, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<64, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<64, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<64, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<64, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<64, uint64_t> data4(ull(0x0FFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_any_with_reduced_active_bits)
    {
      etl::bitset_ext<60, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<60, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<60, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_any_with_mask_with_reduced_active_bits)
    {
      etl::bitset_ext<60, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<60, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<60, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_none_with_reduced_active_bits)
    {
      etl::bitset_ext<60, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<60, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<60, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_none_with_mask_with_reduced_active_bits)
    {
      etl::bitset_ext<60, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<60, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<60, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_all_with_reduced_active_bits)
    {
      etl::bitset_ext<60, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<60, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<60, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_all_with_mask_with_reduced_active_bits)
    {
      etl::bitset_ext<60, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<60, uint64_t>::buffer_type buffer4;
      etl::bitset_ext<60, uint64_t> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60, uint64_t> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60, uint64_t> data3(ull(0x1000010001000100), buffer3);
      etl::bitset_ext<60, uint64_t> data4(ull(0xFFFFFFFFFFFFFFFF), buffer4);

      ull mask = ull(0x00FFFFFFFFFFFFFF);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t> data(buffer);

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
      etl::bitset_ext<64, uint64_t>::buffer_type buffer;
      std::bitset<64> compare;
      etl::bitset_ext<64, uint64_t> data(buffer);

      bool bc = ~compare[3];
      bool bd = ~data[3];
      CHECK_EQUAL(bc, bd);
    }

    //*************************************************************************
    void test_assignment_operator_helper(etl::bitset_ext<64, uint64_t>& from, etl::bitset_ext<64, uint64_t>& to)
    {
      to = from;
    }

    TEST(test_assignment_operator)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      std::bitset<64> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<64, uint64_t> data1(0xFFFFFFFFFFFFFFFULL, buffer1);
      etl::bitset_ext<64, uint64_t> data2(buffer2);

      test_assignment_operator_helper(data1, data2);

      for (size_t i = 0UL; i < data2.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t> data1(0x123456781234567ULL, buffer1);
      etl::bitset_ext<64, uint64_t> data2(0x123456781234567ULL, buffer2);
      etl::bitset_ext<64, uint64_t> data3(buffer3);

      bool equal     =  (data1 == data2);
      bool not_equal = !(data1 == data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t> data1(0x123456781234567ULL, buffer1);
      etl::bitset_ext<64, uint64_t> data2(0x123456781234567ULL, buffer2);
      etl::bitset_ext<64, uint64_t> data3(buffer3);

      bool equal     = !(data1 != data2);
      bool not_equal = (data1 != data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_shift_left_operator)
    {
      uint64_t value = 0x0123456789ABCDEFULL;
      uint64_t mask = 0xFFFFFFFFFFFFFFFFULL;

      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;

      etl::bitset_ext<64, uint64_t> original(0x0123456789ABCDEFULL, buffer1);

      for (size_t shift = 0U; shift < 64ULL; ++shift)
      {
        etl::bitset_ext<64, uint64_t> data(original, buffer2);
        CHECK_EQUAL_HEX(((value & mask) << shift), (data <<= shift).value<uint64_t>());
        mask >>= 1;
      }

      etl::bitset_ext<64, uint64_t> data(original, buffer2);
      CHECK_EQUAL_HEX(0ULL, (data <<= 64U).value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_left_operator_overflow)
    {
      etl::bitset_ext<32, uint32_t>::buffer_type buffer1;
      etl::bitset_ext<32, uint32_t>::buffer_type buffer2;

      etl::bitset_ext<32, uint32_t> data(0xFFFFFFFFULL, buffer1);
      data <<= 1U;
      etl::bitset_ext<32, uint32_t> shifted(0xFFFFFFFEUL, buffer2);

      CHECK_EQUAL_HEX(shifted.value<uint32_t>(), data.value<uint32_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_operator)
    {
      uint64_t value = 0x0123456789ABCDEFULL;

      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;

      etl::bitset_ext<64, uint64_t> original(0x0123456789ABCDEFULL, buffer1);
      
      for (size_t shift = 0U; shift < 64U; ++shift)
      {
        etl::bitset_ext<64, uint64_t> data(original, buffer2);
        CHECK_EQUAL_HEX((value >> shift), (data >>= shift).value<uint64_t>());
      }

      etl::bitset_ext<64, uint64_t> data(original, buffer2);
      CHECK_EQUAL_HEX(0ULL, (data >>= 64U).value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_and_equals_operator)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;

      etl::bitset_ext<64, uint64_t> data1(0x12345678UL, buffer1);
      etl::bitset_ext<64, uint64_t> data2(0x23456789UL, buffer2);
      etl::bitset_ext<64, uint64_t> data3(0x12345678UL & 0x23456789UL, buffer3);

      etl::bitset_ext<64, uint64_t> data4(data1, buffer4);
      data4 &= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_or_equals_operator)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;

      etl::bitset_ext<64, uint64_t> data1(0x12345678UL, buffer1);
      etl::bitset_ext<64, uint64_t> data2(0x23456789UL, buffer2);
      etl::bitset_ext<64, uint64_t> data3(0x12345678UL | 0x23456789UL, buffer3);

      etl::bitset_ext<64, uint64_t> data4(data1, buffer4);
      data4 |= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_xor_equals_operator)
    {
      etl::bitset_ext<64, uint64_t>::buffer_type buffer1;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer2;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer3;
      etl::bitset_ext<64, uint64_t>::buffer_type buffer4;

      etl::bitset_ext<64, uint64_t> data1(0x12345678UL, buffer1);
      etl::bitset_ext<64, uint64_t> data2(0x23456789UL, buffer2);
      etl::bitset_ext<64, uint64_t> data3(0x12345678UL ^ 0x23456789UL, buffer3);

      etl::bitset_ext<64, uint64_t> data4(data1, buffer4);
      data4 ^= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_find_first)
    {
      etl::bitset_ext<8, uint8_t>::buffer_type buffer;

      etl::bitset_ext<8, uint8_t> bs1(ull(0x00), buffer);
      size_t bs1fff = bs1.find_first(false);
      size_t bs1fft = bs1.find_first(true);
      CHECK_EQUAL(0U, bs1fff);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs1fft);

      etl::bitset_ext<8, uint8_t> bs2(ull(0xFF), buffer);
      size_t bs2fff = bs2.find_first(false);
      size_t bs2fft = bs2.find_first(true);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs2fff);
      CHECK_EQUAL(0U, bs2fft);

      etl::bitset_ext<8, uint8_t> bs3(ull(0x01), buffer);
      size_t bs3fff = bs3.find_first(false);
      size_t bs3fft = bs3.find_first(true);
      CHECK_EQUAL(1U, bs3fff);
      CHECK_EQUAL(0U, bs3fft);

      etl::bitset_ext<8, uint8_t> bs4(ull(0x20), buffer);
      size_t bs4fff = bs4.find_first(false);
      size_t bs4fft = bs4.find_first(true);
      CHECK_EQUAL(0U, bs4fff);
      CHECK_EQUAL(5U, bs4fft);

      etl::bitset_ext<8, uint8_t> bs5(ull(0x21), buffer);
      size_t bs5fff = bs5.find_first(false);
      size_t bs5fft = bs5.find_first(true);
      CHECK_EQUAL(1U, bs5fff);
      CHECK_EQUAL(0U, bs5fft);

      etl::bitset_ext<8, uint8_t> bs6(ull(0x0E), buffer);
      size_t bs6fff = bs6.find_first(false);
      size_t bs6fft = bs6.find_first(true);
      CHECK_EQUAL(0U, bs6fff);
      CHECK_EQUAL(1U, bs6fft);

      etl::bitset_ext<8, uint8_t> bs7(ull(0x31), buffer);
      size_t bs7fff = bs7.find_first(false);
      size_t bs7fft = bs7.find_first(true);
      CHECK_EQUAL(1U, bs7fff);
      CHECK_EQUAL(0U, bs7fft);
    }

    //*************************************************************************
    TEST(test_find_next)
    {
      etl::bitset_ext<8, uint8_t>::buffer_type buffer;

      etl::bitset_ext<8, uint8_t> bs1(ull(0x00), buffer);
      size_t bs1fnf0 = bs1.find_next(false, 0);
      size_t bs1fnf1 = bs1.find_next(false, 1);
      size_t bs1fnt2 = bs1.find_next(true, 2);
      CHECK_EQUAL(0U, bs1fnf0);
      CHECK_EQUAL(1U, bs1fnf1);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs1fnt2);

      etl::bitset_ext<8, uint8_t> bs2(ull(0xFF), buffer);
      size_t bs2fnt0 = bs2.find_next(true, 0);
      size_t bs2fnt1 = bs2.find_next(true, 1);
      size_t bs2fnf2 = bs2.find_next(false, 2);
      CHECK_EQUAL(0U, bs2fnt0);
      CHECK_EQUAL(1U, bs2fnt1);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs2fnf2);

      etl::bitset_ext<8, uint8_t> bs3(ull(0x0E), buffer);
      size_t bs3fnf0 = bs3.find_next(false, 0);
      size_t bs3fnt1 = bs3.find_next(true, 1);
      size_t bs3fnf2 = bs3.find_next(false, 2);
      CHECK_EQUAL(0U, bs3fnf0);
      CHECK_EQUAL(1U, bs3fnt1);
      CHECK_EQUAL(4U, bs3fnf2);

      etl::bitset_ext<8, uint8_t> bs4(ull(0x31), buffer);
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
      etl::bitset_ext<16, uint16_t>::buffer_type buffer1;
      etl::bitset_ext<16, uint16_t>::buffer_type buffer2;
      etl::bitset_ext<16, uint16_t>::buffer_type buffer3;
      etl::bitset_ext<16, uint16_t>::buffer_type buffer4;

      etl::bitset_ext<16, uint16_t> compare1(0x2A, buffer1);
      etl::bitset_ext<16, uint16_t> compare2(0x15, buffer2);
      etl::bitset_ext<16, uint16_t> compare1a(0x2A, buffer3);
      etl::bitset_ext<16, uint16_t> compare2a(0x15, buffer4);

      swap(compare1a, compare2a);

      CHECK(compare1 == compare2a);
      CHECK(compare2 == compare1a);
    }

    //*************************************************************************
    TEST(test_span)
    {
      etl::bitset_ext<32, uint32_t>::buffer_type buffer; 
      etl::bitset_ext<32, uint32_t> data(0x12345678UL, buffer);

      using span_t = etl::bitset_ext<32, uint32_t>::span_type;

      span_t s = data.span();
      CHECK_EQUAL(0x12345678UL, s[0]);

      s[0] = 0x87654321UL;
      CHECK_EQUAL(0x87654321UL, data.value<uint32_t>());
    }

    //*************************************************************************
    TEST(test_const_span)
    {
      etl::bitset_ext<32, uint32_t>::buffer_type buffer;
      const etl::bitset_ext<32, uint32_t> data(0x12345678UL, buffer);

      using span_t = const etl::bitset_ext<32, uint32_t>::const_span_type;

      span_t s = data.span();
      CHECK_EQUAL(0x12345678UL, s[0]);
    }

    //*************************************************************************
    TEST(test_to_string)
    {
      using bs32 = etl::bitset_ext<32, uint32_t>;

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
      using bs32 = etl::bitset_ext<32, uint32_t>;

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
      using bs32 = etl::bitset_ext<32, uint32_t>;

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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      etl::u32string<32> text = b.to_string<etl::u32string<32>>(U'.', U'*');
      std::u32string stdtext = b.to_string<std::u32string>(U'.', U'*');

      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(text.c_str()));
      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_extract_6_bit_uint8_t_with_run_time_parameters)
    {
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint8_t(0x38), (b.extract<uint8_t>( 0, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x3C), (b.extract<uint8_t>( 1, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x1E), (b.extract<uint8_t>( 2, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x0F), (b.extract<uint8_t>( 3, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x27), (b.extract<uint8_t>( 4, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x33), (b.extract<uint8_t>( 5, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x19), (b.extract<uint8_t>( 6, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x2C), (b.extract<uint8_t>( 7, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x16), (b.extract<uint8_t>( 8, 6)));
      CHECK_EQUAL_HEX(uint8_t(0x2B), (b.extract<uint8_t>( 9, 6)));
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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint8_t(0x38), (b.extract<uint8_t,  0, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x3C), (b.extract<uint8_t,  1, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x1E), (b.extract<uint8_t,  2, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x0F), (b.extract<uint8_t,  3, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x27), (b.extract<uint8_t,  4, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x33), (b.extract<uint8_t,  5, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x19), (b.extract<uint8_t,  6, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x2C), (b.extract<uint8_t,  7, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x16), (b.extract<uint8_t,  8, 6>()));
      CHECK_EQUAL_HEX(uint8_t(0x2B), (b.extract<uint8_t,  9, 6>()));
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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

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

      CHECK_THROW(b.extract<uint8_t>(26, 7), etl::bitset_overflow);
      CHECK_THROW(b.extract<uint8_t>(27, 6), etl::bitset_overflow);
    }

    //*************************************************************************
    TEST(test_extract_6_bit_int8_t_with_template_parameters)
    {
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

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
      //uint8_t v1 = b.extract<uint8_t, 26, 7>();
      //uint8_t v1 = b.extract<uint8_t, 27, 6>();
    }

    //*************************************************************************
    TEST(test_extract_uint8_t_with_run_time_parameters)
    {
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint8_t(0x78), b.extract<uint8_t>( 0, 8));
      CHECK_EQUAL_HEX(uint8_t(0x3C), b.extract<uint8_t>( 1, 8));
      CHECK_EQUAL_HEX(uint8_t(0x9E), b.extract<uint8_t>( 2, 8));
      CHECK_EQUAL_HEX(uint8_t(0xCF), b.extract<uint8_t>( 3, 8));
      CHECK_EQUAL_HEX(uint8_t(0x67), b.extract<uint8_t>( 4, 8));
      CHECK_EQUAL_HEX(uint8_t(0xB3), b.extract<uint8_t>( 5, 8));
      CHECK_EQUAL_HEX(uint8_t(0x59), b.extract<uint8_t>( 6, 8));
      CHECK_EQUAL_HEX(uint8_t(0xAC), b.extract<uint8_t>( 7, 8));
      CHECK_EQUAL_HEX(uint8_t(0x56), b.extract<uint8_t>( 8, 8));
      CHECK_EQUAL_HEX(uint8_t(0x2B), b.extract<uint8_t>( 9, 8));
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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint8_t(0x78), (b.extract<uint8_t,  0, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x3C), (b.extract<uint8_t,  1, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x9E), (b.extract<uint8_t,  2, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xCF), (b.extract<uint8_t,  3, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x67), (b.extract<uint8_t,  4, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xB3), (b.extract<uint8_t,  5, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x59), (b.extract<uint8_t,  6, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xAC), (b.extract<uint8_t,  7, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x56), (b.extract<uint8_t,  8, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x2B), (b.extract<uint8_t,  9, 8>()));
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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint8_t(0x78), b.extract<uint8_t>( 0, 8));
      CHECK_EQUAL_HEX(uint8_t(0x3C), b.extract<uint8_t>( 1, 8));
      CHECK_EQUAL_HEX(uint8_t(0x9E), b.extract<uint8_t>( 2, 8));
      CHECK_EQUAL_HEX(uint8_t(0xCF), b.extract<uint8_t>( 3, 8));
      CHECK_EQUAL_HEX(uint8_t(0x67), b.extract<uint8_t>( 4, 8));
      CHECK_EQUAL_HEX(uint8_t(0xB3), b.extract<uint8_t>( 5, 8));
      CHECK_EQUAL_HEX(uint8_t(0x59), b.extract<uint8_t>( 6, 8));
      CHECK_EQUAL_HEX(uint8_t(0xAC), b.extract<uint8_t>( 7, 8));
      CHECK_EQUAL_HEX(uint8_t(0x56), b.extract<uint8_t>( 8, 8));
      CHECK_EQUAL_HEX(uint8_t(0x2B), b.extract<uint8_t>( 9, 8));
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
    TEST(test_extract_int8_t_with_template_parameters)
    {
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint8_t(0x78), (b.extract<uint8_t,  0, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x3C), (b.extract<uint8_t,  1, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x9E), (b.extract<uint8_t,  2, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xCF), (b.extract<uint8_t,  3, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x67), (b.extract<uint8_t,  4, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xB3), (b.extract<uint8_t,  5, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x59), (b.extract<uint8_t,  6, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0xAC), (b.extract<uint8_t,  7, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x56), (b.extract<uint8_t,  8, 8>()));
      CHECK_EQUAL_HEX(uint8_t(0x2B), (b.extract<uint8_t,  9, 8>()));
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
    TEST(test_extract_13_bits_uint16_t_with_run_time_parameters)
    {
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint16_t(0x1678), b.extract<uint16_t>( 0, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0B3C), b.extract<uint16_t>( 1, 13));
      CHECK_EQUAL_HEX(uint16_t(0x159E), b.extract<uint16_t>( 2, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0ACF), b.extract<uint16_t>( 3, 13));
      CHECK_EQUAL_HEX(uint16_t(0x0567), b.extract<uint16_t>( 4, 13));
      CHECK_EQUAL_HEX(uint16_t(0x02B3), b.extract<uint16_t>( 5, 13));
      CHECK_EQUAL_HEX(uint16_t(0x1159), b.extract<uint16_t>( 6, 13));
      CHECK_EQUAL_HEX(uint16_t(0x08AC), b.extract<uint16_t>( 7, 13));
      CHECK_EQUAL_HEX(uint16_t(0x1456), b.extract<uint16_t>( 8, 13));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), b.extract<uint16_t>( 9, 13));
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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint16_t(0x1678), (b.extract<uint16_t,  0, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0B3C), (b.extract<uint16_t,  1, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x159E), (b.extract<uint16_t,  2, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0ACF), (b.extract<uint16_t,  3, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x0567), (b.extract<uint16_t,  4, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x02B3), (b.extract<uint16_t,  5, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x1159), (b.extract<uint16_t,  6, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x08AC), (b.extract<uint16_t,  7, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x1456), (b.extract<uint16_t,  8, 13>()));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), (b.extract<uint16_t,  9, 13>()));
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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint16_t(0x5678), b.extract<uint16_t>( 0, 16));
      CHECK_EQUAL_HEX(uint16_t(0x2B3C), b.extract<uint16_t>( 1, 16));
      CHECK_EQUAL_HEX(uint16_t(0x159E), b.extract<uint16_t>( 2, 16));
      CHECK_EQUAL_HEX(uint16_t(0x8ACF), b.extract<uint16_t>( 3, 16));
      CHECK_EQUAL_HEX(uint16_t(0x4567), b.extract<uint16_t>( 4, 16));
      CHECK_EQUAL_HEX(uint16_t(0xA2B3), b.extract<uint16_t>( 5, 16));
      CHECK_EQUAL_HEX(uint16_t(0xD159), b.extract<uint16_t>( 6, 16));
      CHECK_EQUAL_HEX(uint16_t(0x68AC), b.extract<uint16_t>( 7, 16));
      CHECK_EQUAL_HEX(uint16_t(0x3456), b.extract<uint16_t>( 8, 16));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), b.extract<uint16_t>( 9, 16));
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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint16_t(0x5678), (b.extract<uint16_t,  0, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x2B3C), (b.extract<uint16_t,  1, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x159E), (b.extract<uint16_t,  2, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x8ACF), (b.extract<uint16_t,  3, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x4567), (b.extract<uint16_t,  4, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0xA2B3), (b.extract<uint16_t,  5, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0xD159), (b.extract<uint16_t,  6, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x68AC), (b.extract<uint16_t,  7, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x3456), (b.extract<uint16_t,  8, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), (b.extract<uint16_t,  9, 16>()));
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
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint16_t(0x5678), b.extract<uint16_t>( 0, 16));
      CHECK_EQUAL_HEX(uint16_t(0x2B3C), b.extract<uint16_t>( 1, 16));
      CHECK_EQUAL_HEX(uint16_t(0x159E), b.extract<uint16_t>( 2, 16));
      CHECK_EQUAL_HEX(uint16_t(0x8ACF), b.extract<uint16_t>( 3, 16));
      CHECK_EQUAL_HEX(uint16_t(0x4567), b.extract<uint16_t>( 4, 16));
      CHECK_EQUAL_HEX(uint16_t(0xA2B3), b.extract<uint16_t>( 5, 16));
      CHECK_EQUAL_HEX(uint16_t(0xD159), b.extract<uint16_t>( 6, 16));
      CHECK_EQUAL_HEX(uint16_t(0x68AC), b.extract<uint16_t>( 7, 16));
      CHECK_EQUAL_HEX(uint16_t(0x3456), b.extract<uint16_t>( 8, 16));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), b.extract<uint16_t>( 9, 16));
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
    TEST(test_extract_int16_t_with_template_parameters)
    {
      using bs32 = etl::bitset_ext<32, uint32_t>;

      bs32::buffer_type buffer;
      bs32 b(0x12345678UL, buffer);

      CHECK_EQUAL_HEX(uint16_t(0x5678), (b.extract<uint16_t,  0, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x2B3C), (b.extract<uint16_t,  1, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x159E), (b.extract<uint16_t,  2, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x8ACF), (b.extract<uint16_t,  3, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x4567), (b.extract<uint16_t,  4, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0xA2B3), (b.extract<uint16_t,  5, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0xD159), (b.extract<uint16_t,  6, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x68AC), (b.extract<uint16_t,  7, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x3456), (b.extract<uint16_t,  8, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x1A2B), (b.extract<uint16_t,  9, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x8D15), (b.extract<uint16_t, 10, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x468A), (b.extract<uint16_t, 11, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x2345), (b.extract<uint16_t, 12, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x91A2), (b.extract<uint16_t, 13, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x48D1), (b.extract<uint16_t, 14, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x2468), (b.extract<uint16_t, 15, 16>()));
      CHECK_EQUAL_HEX(uint16_t(0x1234), (b.extract<uint16_t, 16, 16>()));

      // The lines below should static assert.
      //uint16_t v1 = b.extract<uint16_t, 16, 17>());
      //uint16_t v2 = b.extract<uint16_t, 17, 16>());
    }

    //*************************************************************************
    bool test_bit(int i)
    {
      return (0x1234 & (1 << i)) != 0;
    }

    TEST(test_test_run_time)
    {
      etl::bitset_ext<16, uint16_t>::buffer_type buffer;

      etl::bitset_ext<16, uint16_t> b(0x1234, buffer);

      bool t0 = b.test(0);
      bool t1 = b.test(1);
      bool t2 = b.test(2);
      bool t3 = b.test(3);
      bool t4 = b.test(4);
      bool t5 = b.test(5);
      bool t6 = b.test(6);
      bool t7 = b.test(7);

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
      etl::bitset_ext<16, uint16_t>::buffer_type buffer;

      etl::bitset_ext<16, uint16_t> b(0x1234, buffer);

      bool t0 = b.test<0>();
      bool t1 = b.test<1>();
      bool t2 = b.test<2>();
      bool t3 = b.test<3>();
      bool t4 = b.test<4>();
      bool t5 = b.test<5>();
      bool t6 = b.test<6>();
      bool t7 = b.test<7>();

      CHECK_EQUAL(test_bit(0), t0);
      CHECK_EQUAL(test_bit(1), t1);
      CHECK_EQUAL(test_bit(2), t2);
      CHECK_EQUAL(test_bit(3), t3);
      CHECK_EQUAL(test_bit(4), t4);
      CHECK_EQUAL(test_bit(5), t5);
      CHECK_EQUAL(test_bit(6), t6);
      CHECK_EQUAL(test_bit(7), t7);
    }
  };
}
